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
  for (int x=1; x<m_width-1; x++)
    for (int y=1; y<m_height-1; y++)
      this->printch({x,y}, ' ');
  this->refresh();
  delwin(m_win);
}


void Window::resize(int new_width, int new_height) {
  auto ret = wresize(m_win, new_height, new_width);
  if (ret == ERR) throw std::out_of_range("Bad width or height for window resize");
  update_dims();
}


void Window::move(Coords new_pos) {
  int ret;
  if (m_parent)
    ret = mvderwin(m_win, new_pos.y, new_pos.x);
  else
    ret = mvwin(m_win, new_pos.y, new_pos.x);
  if (ret == ERR) throw std::out_of_range("Bad coordinates for new window position");
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


void Window::println(Coords pos1, Coords pos2, chtype symb) {
  auto diff = pos2 - pos1;
  if (diff.x == 0) {
    auto& top = (diff.y > 0) ? pos1 : pos2;
    mvwvline(m_win, top.y, top.x, symb, std::abs(diff.y));
  } else if (diff.y == 0) {
    auto left = (diff.x > 0) ? pos1 : pos2;
    mvwhline(m_win, left.y, left.x, symb, std::abs(diff.x));
  }
}


void Window::printch(Coords pos, chtype symb) {
  mvwaddch(m_win, pos.y, pos.x, symb);
}


void Window::update_pos()  { getyx(m_win, m_pos.y, m_pos.x); }
void Window::update_dims() { getmaxyx(m_win, m_height, m_width); }

void Window::display_title(std::string title) {
  // Writes the string title on the first line of the window,
  // followed by a horizontal line below
  if (title.size()) {
    // Trim title length if necessary
    if (title.size() > static_cast<unsigned>(get_width()-2)) {
      title = title.substr(0, get_width() - 2);
      title[get_width()-2] = '.';
      title[get_width()-3] = '.';
      title[get_width()-4] = '.';
    }

    // Fix up the dividing line between title and menu items
    println({0,2}, {get_width(),2}, ACS_HLINE);
    printch({0,2}, ACS_LTEE);
    printch({get_width()-1,2}, ACS_RTEE);

    printf({1,1}, title.c_str());
  } else {
    // Clear the title and line below
    std::string blank;
    blank.resize(get_width()-2);
    for (auto& c : blank) c = ' ';
    printf({1,1}, blank.c_str());
    printf({1,2}, blank.c_str());
    // Fix up the border
    printch({0,2}, ACS_VLINE);
    printch({get_width()-1,2}, ACS_VLINE);
  }
}

