#include "menu.hpp"

Menu::Menu(const std::string& title,
    const std::vector<MenuItem>& items,
    Window *parent,
    Coords pos,
    int w, int h, uint64_t opts)
  : Window{parent, pos, w, h},
    m_title{title}, m_items{items},
    m_subwin{this, {pos.x+1, pos.y + 3}, w-2, h-4, Window::NO_BORDER | opts}
{
  // Initialize the items array
  m_itemsptr = new ITEM*[items.size()+1];
  for (unsigned i = 0; i<items.size(); i++)
    m_itemsptr[i] = new_item(m_items.at(i).name.c_str(),
                             m_items.at(i).desc.c_str());
  m_itemsptr[items.size()] = nullptr;

  // Initialize the menu and connect it to the windows
  m_menu = new_menu(m_itemsptr);
  set_menu_mark(m_menu, "");
  set_menu_win(m_menu, get_winptr());
  set_menu_sub(m_menu, m_subwin.get_winptr());

  // Allow as many menu items to display as possible
  set_menu_format(m_menu, m_subwin.get_height(), 1);

  // Draw the title
  if (m_title.size() > static_cast<unsigned>(get_width() - 2))
    m_title = m_title.substr(0, get_width() - 2);

  // Fix up the dividing line between title and menu items
  println({0,2}, {get_width(),2}, ACS_HLINE);
  printch({0,2}, ACS_LTEE);
  printch({get_width()-1,2}, ACS_RTEE);

  printf({1,1}, m_title.c_str());

  // Place the menu in the subwindow
  post_menu(m_menu);
}


Menu::~Menu() {
  // Cleanup menu
  unpost_menu(m_menu);
  free_menu(m_menu);
  // Cleanup items
  for (unsigned i = 0; i<m_items.size(); i++)
    free_item(m_itemsptr[i]);
  delete[] m_itemsptr;
}


MenuItem Menu::get_current_item() const {
  if (auto curr = current_item(m_menu))
    return m_items.at(item_index(curr));
  else
    return MenuItem{};
}


void Menu::sel_up() {
  menu_driver(m_menu, REQ_UP_ITEM);
}


void Menu::sel_down() {
  menu_driver(m_menu, REQ_DOWN_ITEM);
}


void Menu::scroll_up() {
  menu_driver(m_menu, REQ_SCR_ULINE);
}


void Menu::scroll_down() {
  menu_driver(m_menu, REQ_SCR_DLINE);
}


void Menu::set_title(const std::string& ) {}
void Menu::set_items(const std::vector<MenuItem>& ) {}
