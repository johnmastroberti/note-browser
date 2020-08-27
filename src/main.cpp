#include <ncurses.h>
#include <unistd.h>
#include "window.hpp"
#include "session.hpp"

int main() {

  CursesSession session;
  session.add_settings(CursesSession::RAW
                      | CursesSession::KEYPAD
                      | CursesSession::NOECHO
                      | CursesSession::CURSOR_NONE);

  Window w{nullptr, {3, 3}, 20, 20};

  session.register_callback('a', [&w](decltype(KEY_UP)) {
      w.printf({1,1}, "A pressed");
      w.refresh();
      return false;
    });
  session.register_callback('q', [&w](decltype(KEY_UP)) {
      w.printf({1,1}, "Goodbye    ");
      w.refresh();
      sleep(3);
      return true;
    });

  session.refresh();
  w.refresh();

  session.loop();

  return 0;
}
