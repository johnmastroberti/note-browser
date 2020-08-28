#include <ncurses.h>
#include <unistd.h>
#include <vector>
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

  session.register_callback('j', [&m](decltype(KEY_UP)) {
      m.sel_down();
      m.refresh();
      return false;
    });
  session.register_callback('k', [&m](decltype(KEY_UP)) {
      m.sel_up();
      m.refresh();
      return false;
    });

  session.refresh();
  m.refresh();

  session.loop();

  return 0;
}
