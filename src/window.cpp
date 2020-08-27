#include "window.hpp"

Window::Window(Window *parent, Coords pos, int w, int h)
  : m_parent{parent}, m_pos{pos}, m_width{w}, m_height{h}
{
  // Set width and height to the max width and height if they are negative
  int max_x, max_y;
  if (m_parent)
    getmaxyx(m_parent->m_win, max_y, max_x);
  else
    getmaxyx(stdscr, max_y, max_x);

  if (m_width < 0)
    m_width = max_x - m_pos.x;
  if (m_height < 0)
    m_height = max_y - m_pos.y;

  // Perform ncurses setup for this window
  if (m_parent)
    m_win = subwin(m_parent->m_win, m_height, m_width, pos.y, pos.x);
  else
    m_win = newwin(m_height, m_width, pos.y, pos.x);

  // Make a border around the window
  box(m_win, 0, 0);

  refresh();
}


Window::~Window() {
  wborder(m_win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
  refresh();
  delwin(m_win);
}


void Window::resize(int new_width, int new_height) {
  auto ret = wresize(m_win, new_height, new_width);
  if (ret == ERR) throw;
  update_dims();
}


void Window::move(Coords new_pos) {
  int ret;
  if (m_parent)
    ret = mvderwin(m_win, new_pos.y, new_pos.x);
  else
    ret = mvwin(m_win, new_pos.y, new_pos.x);
  if (ret == ERR) throw;
  update_pos();
}


void Window::refresh() {
  if (m_parent)
    m_parent->refresh();

  touchwin(m_win);
  wrefresh(m_win);
}


void Window::redraw() {
  redrawwin(m_win);
}


void Window::update_pos()  { getyx(m_win, m_pos.y, m_pos.x); }
void Window::update_dims() { getmaxyx(m_win, m_height, m_width); }

