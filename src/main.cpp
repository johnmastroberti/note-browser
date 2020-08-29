#include <ncurses.h>
#include <unistd.h>
#include <vector>
#include <iostream>
#include <fstream>
#include "window.hpp"
#include "session.hpp"
#include "menu.hpp"
#include "notes.hpp"

int main() {

  NoteBook book{fs::path("/home/john/Documents/school/p506")};

  getchar();
  CursesSession session;
  session.add_settings(CursesSession::RAW
                      | CursesSession::KEYPAD
                      | CursesSession::NOECHO
                      | CursesSession::CURSOR_NONE);

  std::ofstream log("log.txt");

  std::vector<MenuItem> items;
  items.resize(book.m_notes.size());
  log << items.size() << std::endl;
  std::transform(book.m_notes.begin(), book.m_notes.end(), items.begin(),
      [](const auto& note) { return MenuItem{note.m_display_name, note.m_file_name}; });
  Menu m {book.m_course_name, items};

  for (const auto& item : items)
    log << item.name << '\t' << item.desc << std::endl;


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
