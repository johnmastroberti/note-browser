#pragma once
#include <ncurses.h>
#include <menu.h>
#include <string>

class Menu : public Window {
  private:
    std::string m_title;
    MENU *m_menu;
    ITEM **m_items;

  public:
    Menu();
};

