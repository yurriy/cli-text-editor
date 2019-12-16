#include <ncursesw/curses.h>
#include <locale.h>
#include <wchar.h>
#include <wctype.h>
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

wchar_t get_cur_char() {
    in_wch(&cc);
    getcchar(&cc, cur, &attr, &color_pair, NULL);
    fwprintf(l, L"got char %C (%d) pos %d\n", cur, cur, x);
    return cur[0];
}

void go_end() {
    x = m - 1;
    move(y, x);
    // Skip spaces.
    while (x > 0 && iswspace(get_cur_char())) {
        x--;
        move(y, x);
    }
    // Stand after last non spacing char.
    if (x != m - 1 && !iswspace(get_cur_char())) {
        x++;
        move(y, x);
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
                refresh();
                break;
            }

            case KEY_UP:
            {
                if (y > 0) y--;
                go_end();
                refresh();
                break;
            }

            case KEY_DOWN:
            {
                if (y != n - 1) y++;
                go_end();
                refresh();
                break;
            }

            case '\r': // Enter
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
                    go_end();
                } else {
                    move(y, x);
                    delch();
                }
                refresh();
                break;
            }

            case KEY_DC:
            {
                delch();
                refresh();
                break;
            }

            case 23: // ^W
            {
                // Skip spaces.
                while (iswspace(get_cur_char()) && x > 0) {
                    delch();
                    x--;
                    move(y, x);
                }
                // Remove characters until space.
                while (!iswspace(get_cur_char())) {
                    delch();
                    if (x > 0) {
                        x--;
                    } else {
                        break;
                    }
                    move(y, x);
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