#pragma once
#include <ncurses.h>
#include <menu.h>
#include <string>
#include <vector>
#include "window.hpp"

using namespace std::string_literals;

struct MenuItem {
  std::string name, desc;
};

class Menu : public Window {
  private:
    std::string m_title;
    std::vector<MenuItem> m_items;
    MENU *m_menu;
    ITEM **m_itemsptr;
    Window m_subwin;

  public:
    Menu(const std::string& title = ""s,
        const std::vector<MenuItem>& items = {},
        Window *parent = nullptr,
        Coords pos = {0,0},
        int w=-1, int h=-1, uint64_t opts=0);
    ~Menu();
    Menu(const Menu&) = delete;
    Menu& operator=(const Menu&) = delete;
    Menu(Menu&&) = default;
    Menu& operator=(Menu&&) = default;

    void set_title(const std::string& title);
    void set_items(const std::vector<MenuItem>& items);

    MenuItem get_current_item() const;
    void set_current_item(int index);
    void sel_up();
    void sel_down();
    void scroll_up();
    void scroll_down();
};

