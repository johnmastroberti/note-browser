#include <ncurses.h>
#include "window.hpp"

int main() {

  initscr();
  raw();
  keypad(stdscr, TRUE);
  noecho();

  auto *w = new Window(nullptr, {3, 3}, 20, 20);
  w->printf("test");

  refresh();
  getch();

  delete w;
  endwin();
  return 0;
}
