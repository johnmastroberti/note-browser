#include "session.hpp"

CursesSession::CursesSession(uint64_t settings) {
  initscr();
  if(settings) add_settings(settings);
}


CursesSession::~CursesSession() {
  endwin();
}


void CursesSession::add_settings(uint64_t settings) {
  if (settings & RAW)
    raw();
  if (settings & CBREAK)
    cbreak();
  if (settings & ECHO)
    echo();
  if (settings & NOECHO)
    noecho();
  if (settings & CURSOR_NONE)
    curs_set(0);
  if (settings & CURSOR_NORMAL)
    curs_set(1);
  if (settings & CURSOR_BLOCK)
    curs_set(2);
  if (settings & KEYPAD)
    keypad(stdscr, TRUE);
}


void CursesSession::refresh() {
  ::refresh();
}


bool CursesSession::register_callback(ktype key, const std::function<bool(ktype)>& callback) {
  auto ret = m_callbacks.contains(key);
  m_callbacks[key] = callback;
  return ret;
}


void CursesSession::loop() {
  for(;;) {
    auto c = getch();
    if (m_callbacks.contains(c))
      if (m_callbacks[c](c))
        break;
  }
}


std::pair<int, int> CursesSession::get_dims() const {
  int w,h;
  getmaxyx(stdscr, h, w);
  return {w, h};
}
