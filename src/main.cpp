#include <ncurses.h>
#include <unistd.h>
#include <vector>
#include <iostream>
#include <fstream>
#include "window.hpp"
#include "session.hpp"
#include "menu.hpp"
#include "notes.hpp"
#include "view.hpp"

int main() {


  CursesSession session;
  session.add_settings(CursesSession::RAW
                      | CursesSession::KEYPAD
                      | CursesSession::NOECHO
                      | CursesSession::CURSOR_NONE);

  NoteBinder fall2020{fs::path("/home/john/Documents/school")};

  BrowserView view{&session, &fall2020};
  view.start();

  return 0;
}
