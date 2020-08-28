#pragma once
#include <ncurses.h>
#include <iostream>

#include "coords.hpp"

class Window;

class Window {
  protected:
    Window *m_parent;
    WINDOW *m_win;
    Coords m_pos;
    int m_width, m_height;

    void update_pos();
    void update_dims();

  public:
    enum WindowOpt : uint64_t { NO_BORDER = 1<<0 };

    Window(Window *parent = nullptr, Coords pos = {0,0}, int w=-1, int h=-1, uint64_t opts=0);

    ~Window();

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;
    Window(Window&&) = default;
    Window& operator=(Window&&) = default;

    void resize(int new_width, int new_height);
    void move(Coords new_pos);
    void refresh();
    void redraw();

    void redraw_border();

    template<typename... Args>
    void printf(const char *fmt, Args... args) {
      auto ret = wprintw(m_win, fmt, args...);
      std::cout << "here";
      if (ret != OK) throw;
    }

    template<typename... Args>
    void printf(Coords pos, const char *fmt, Args... args) {
      auto ret = mvwprintw(m_win, pos.y, pos.x, fmt, args...);
      if (ret != OK) throw;
    }

    void println(Coords pos1, Coords pos2, decltype(ACS_HLINE) symb);

    void printch(Coords pos, decltype(ACS_HLINE) symb);

    inline auto get_winptr() { return m_win; }
    inline auto get_width() const { return m_width; }
    inline auto get_height() const { return m_height; }
    inline auto get_pos() const { return m_pos; }



};

