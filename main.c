#include <ncursesw/curses.h>
#include <locale.h>
#include <wchar.h>
#include <stdlib.h>
#include <string.h>

FILE *l;
int n, m;
int x, y;

wchar_t cur[2];
cchar_t cc;
attr_t attr;
short color_pair;
wchar_t buf[2];
int width;

void go_end() {
    x = m - 1;
    move(y, x);
    in_wch(&cc);
    getcchar(&cc, cur, &attr, &color_pair, NULL);
    // Skip spaces.
    while (iswspace(cur[0]) && x > 0) {
        x--;
        //fwprintf(l, L"moving %d %d\n", y, x);
        move(y, x);
        in_wch(&cc);
        getcchar(&cc, cur, &attr, &color_pair, NULL);
        //fwprintf(l, L"got char %C (%d) pos %d\n", cur, cur, x);
    }
}

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "");

    l = fopen("log.txt", "w");
    if (l == NULL) {
        perror("fopen");
        abort();
    }
    setbuffer(l, NULL, 0);

    initscr();
    if (start_color() == ERR) {
        return 1;
    }
    cbreak();
    noecho();

    nonl();
    intrflush(stdscr, FALSE);
    keypad(stdscr, TRUE);

    getmaxyx(stdscr, n, m);

    fwprintf(l, L"screen size %d %d\n", n, m);

    wint_t c;
    int i = 0;
    int res;
    while ((res = get_wch(&c)) != ERR && c != WEOF) {
        if (res == KEY_CODE_YES) {
            fwprintf(l, L"keycode: %x\n", c);
        } else {
            fwprintf(l, L"char: '%C' (%d)\n", c, c);
        }
        switch (c) {
            case KEY_HOME:
            {
                x = 0;
                move(y, x);
                refresh();
                break;
            }

            case KEY_END:
            {
                go_end();
                // Stand after last non spacing char.
                if (x != m - 1) {
                    x++;
                }
                //fwprintf(l, L"y %d x %d here\n", y, x);
                move(y, x);
                //fwprintf(l, L"moved.\n");
                refresh();
                break;
            }

            case KEY_UP:
            {
                if (y > 0) y--;
                go_end();
                if (x != m - 1) x++;
                refresh();
                break;
            }

            case KEY_DOWN:
            {
                if (y != n - 1) y++;
                go_end();
                if (x != m - 1) x++;
                refresh();
                break;
            }

            case 13: // Enter
            {
                if (y != n - 1) y++;
                x = 0;
                move(y, x);
                refresh();
                break;
            }

            case KEY_LEFT:
            {
                if (x > 0) x--;
                move(y, x);
                refresh();
                break;
            }

            case KEY_RIGHT:
            {
                if (x != m - 1) x++;
                move(y, x);
                refresh();
                break;
            }

            case 127: // Backspace.
            {
                if (x == 0 && y == 0) {
                    break;
                }
                if (--x < 0) {
                    y--;
                    //fwprintf(l, L"screen size %d %d\n", n, m);
                    x = m - 1;
                    //fwprintf(l, L"moving %d %d\n", y, x);
                    move(y, x);
                    //fwprintf(l, L"after move\n");
                    in_wch(&cc);
                    //fwprintf(l, L"after in_wch\n");
                    getcchar(&cc, cur, &attr, &color_pair, NULL);
                    //fwprintf(l, L"got char %C\n", cur);
                    // Skip spaces.
                    while (iswspace(cur[0]) && x > 0) {
                        x--;
                        //fwprintf(l, L"moving %d %d\n", y, x);
                        move(y, x);
                        in_wch(&cc);
                        getcchar(&cc, cur, &attr, &color_pair, NULL);
                        //fwprintf(l, L"got char %C (%d) pos %d\n", cur, cur, x);
                    }
                }
                move(y, x);
                delch();
                refresh();
                break;
            }

            case 23: // ^W
            {
                in_wch(&cc);
                getcchar(&cc, cur, &attr, &color_pair, NULL);
                // Skip spaces.
                while (iswspace(cur[0]) && x > 0) {
                    delch();
                    x--;
                    move(y, x);
                    in_wch(&cc);
                    getcchar(&cc, cur, &attr, &color_pair, NULL);
                }
                // Remove characters until space.
                while (!iswspace(cur[0])) {
                    delch();
                    if (x > 0) {
                        x--;
                    } else {
                        break;
                    }
                    move(y, x);
                    in_wch(&cc);
                    getcchar(&cc, cur, &attr, &color_pair, NULL);
                }
                move(y, x);
                refresh();
                break;
            }
            default:
            {
                if (res != KEY_CODE_YES) {
                    buf[0] = c;
                    setcchar(&cc, buf, attr, 0, NULL);
                    ins_wch(&cc);
                    width = wcwidth(c);
                    //fwprintf(l, L"width: %d\n", width);
                    x += width;
                    move(y, x);
                }
                break;
            }
        }
    }

    fclose(l);
    return 0;
}