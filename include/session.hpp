#pragma once
#include <ncurses.h>
#include <functional>
#include <unordered_map>

class CursesSession {
  // Abstraction of the ncurses free functions related to global
  // session settings and attributes
  private:
    typedef decltype(KEY_UP) ktype;
    std::unordered_map<ktype, std::function<bool(ktype)>> m_callbacks;

  public:
    CursesSession(uint64_t settings=0);
    ~CursesSession();

    enum Setting : uint64_t {
      RAW           = 1<<0,
      CBREAK        = 1<<1,
      ECHO          = 1<<2,
      NOECHO        = 1<<3,
      CURSOR_NONE   = 1<<4,
      CURSOR_NORMAL = 1<<5,
      CURSOR_BLOCK  = 1<<6,
      KEYPAD        = 1<<7
    };
    void add_settings(uint64_t settings);
    void refresh();

    bool register_callback(ktype key, const std::function<bool(ktype)>& callback);
    void loop();
};

