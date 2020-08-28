#include <ncurses.h>
#include <unistd.h>
#include "window.hpp"
#include "session.hpp"
#include "menu.hpp"

int main() {

  CursesSession session;
  session.add_settings(CursesSession::RAW
                      | CursesSession::KEYPAD
                      | CursesSession::NOECHO
                      | CursesSession::CURSOR_NONE);

  Menu m{"Test menu"s, {{"Item 1"s, "Desc 1"s}, {"Item 2"s, "Desc 2"s}}};

  session.register_callback('q', [](decltype(KEY_UP)) {
      return true;
    });

  session.refresh();
  m.refresh();

  session.loop();

  return 0;
}
