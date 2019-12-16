1. Install ncurses with wide-char/UTF-8 to contrib/curses.

```
cd contrib/ncurses-6.1
./configure --prefix=$(pwd)/../curses --enable-widec
make
make install
```

2. Compile editor.

```
cmake .
make
```
