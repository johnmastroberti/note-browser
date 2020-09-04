#include <ncurses.h>
#include <unistd.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <signal.h>
#include "window.hpp"
#include "session.hpp"
#include "menu.hpp"
#include "notes.hpp"
#include "view.hpp"

BrowserView *GLOBAL_VIEW = nullptr;

void handle_resize(int) {
  GLOBAL_VIEW->handle_resize();
}

int main() {

  std::ofstream log("log.txt");

  try {
    CursesSession session;
    session.add_settings(CursesSession::RAW
                        | CursesSession::KEYPAD
                        | CursesSession::NOECHO
                        | CursesSession::CURSOR_NONE);

    NoteBinder fall2020{fs::path("/home/john/Documents/school")};

    BrowserView view{&session, &fall2020};
    GLOBAL_VIEW = &view;
    signal(SIGWINCH, handle_resize);
    view.start();
  } catch (const std::exception& e) {
    log << "Caught exception: " << e.what() << std::endl;
  }

  return 0;
}
