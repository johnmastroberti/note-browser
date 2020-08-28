#include <ncurses.h>
#include <unistd.h>
#include <vector>
#include <iostream>
#include <fstream>
#include "window.hpp"
#include "session.hpp"
#include "menu.hpp"

int main() {

  CursesSession session;
  session.add_settings(CursesSession::RAW
                      | CursesSession::KEYPAD
                      | CursesSession::NOECHO
                      | CursesSession::CURSOR_NONE);

  std::ifstream source;
  source.open("src/main.cpp");
  std::vector<MenuItem> items;
  for(;;) {
    std::string line;
    std::getline(source, line);
    if (!source.good()) break;
    if (line.size())
      items.push_back({line, "hello"s});
  }

  auto [max_width, max_height] = session.get_dims();

  Menu m1{"Choose a line"s, items, nullptr, {0,0}, max_width/2, max_height};
  Menu m2{"Choose another line"s, items, nullptr, {max_width/2+1,0},
    max_width - max_width/2 - 1, max_height};

  std::string s1, s2;
  session.register_callback('q', [&m1, &m2, &s1, &s2](decltype(KEY_UP)) {
      s1 = m1.get_current_item().name;
      s2 = m2.get_current_item().name;
      return true;
    });

  session.register_callback('j', [&m1](decltype(KEY_UP)) {
      m1.sel_down();
      m1.refresh();
      return false;
    });
  session.register_callback('k', [&m1](decltype(KEY_UP)) {
      m1.sel_up();
      m1.refresh();
      return false;
    });

  session.refresh();
  m1.refresh();
  m2.refresh();

  session.loop();

  std::ofstream log;
  log.open("log.txt");
  log << "s1 = " << s1 << std::endl;
  log << "s2 = " << s2 << std::endl;

  return 0;
}
