#include <curses.h>
#include <locale.h>


int main() {
    setlocale(LC_ALL, "");
    initscr();
    cbreak();
    noecho();

    nonl();
    intrflush(stdscr, FALSE);
    keypad(stdscr, TRUE);

    int n = 0, m = 0;
    getmaxyx(stdscr, n, m);

    int c;
    int x = 0, y = 0;
    int i = 0;
    while ((c = getch()) != ERR) {
        if (x < 0 || x >= m || y < 0 || y >= n) {
            return 1;
        }
        bool match = false;
        //printf("%d\n", c);
        switch (c) {
            case KEY_HOME:
                x = 0;
                move(y, x);
                refresh();
                continue;
            case KEY_END:
                x = m - 1;
                move(y, x);
                refresh();
                continue;
            case KEY_UP:
                if (y > 0) y--;
                move(y, x);
                refresh();
                continue;
            case KEY_DOWN:
            case 13:
                if (y != n - 1) y++;
                move(y, x);
                refresh();
                continue;
            case KEY_LEFT:
                if (x > 0) x--;
                move(y, x);
                refresh();
                continue;
            case KEY_RIGHT:
                if (x != m - 1) x++;
                move(y, x);
                refresh();
                continue;
            case 127:
                if (x == 0 && y == 0) {
                    continue;
                }
                if (--x < 0) {
                    x = m - 1;
                    y--;
                }
                move(y, x);
                delch();
                refresh();
                continue;
            case 23:
                if (x > 0) {
                    move(y, --x);
                }
                chtype cur;
                while (isprint(cur = inch()) && !isspace(cur)) {
                    addch(' ');
                    if (x > 0) {
                        x--;
                        move(y, x);
                    } else {
                        break;
                    }
                }
                refresh();
                continue;
            default:
                if (!isprint(c)) {
                    continue;
                }
                if (y == n - 1 && x == m - 1) {
                    continue;
                }
                addch(c);
                x++;
                if (x == m) {
                    x = 0;
                    y++;
                }
                if (y == n) {
                    y = 0;
                }
                move(y, x);
                refresh();
        }
    }

    return 0;
}