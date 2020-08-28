#include <cmath>
#include "window.hpp"

Window::Window(Window *parent, Coords pos, int w, int h, uint64_t opts)
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
  if ((opts & NO_BORDER) == 0)
    box(m_win, 0, 0);

  this->refresh();
}


Window::~Window() {
  wborder(m_win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
  this->refresh();
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


void Window::redraw_border() {
  box(m_win, 0, 0);
}


void Window::println(Coords pos1, Coords pos2, decltype(ACS_HLINE) symb) {
  auto diff = pos2 - pos1;
  if (diff.x == 0) {
    auto& top = (diff.y > 0) ? pos1 : pos2;
    mvwvline(m_win, top.y, top.x, symb, std::abs(diff.y));
  } else if (diff.y == 0) {
    auto left = (diff.x > 0) ? pos1 : pos2;
    mvwhline(m_win, left.y, left.x, symb, std::abs(diff.x));
  }
}


void Window::printch(Coords pos, decltype(ACS_HLINE) symb) {
  mvwaddch(m_win, pos.y, pos.x, symb);
}


void Window::update_pos()  { getyx(m_win, m_pos.y, m_pos.x); }
void Window::update_dims() { getmaxyx(m_win, m_height, m_width); }

