#pragma once
#include <string>
#include <vector>
#include <map>
#include "window.hpp"
#include "menu.hpp"
#include "notes.hpp"
#include "session.hpp"

class BrowserView {
  private:
    // Owning:
    Menu *m_course_menu = nullptr;
    Menu *m_notes_menu = nullptr;
    Window *m_preview_window = nullptr;

    // Non-owning
    CursesSession *m_session;
    NoteBinder *m_binder;
    NoteBook *m_current_book_ptr;
    NoteFile *m_current_file_ptr;

    std::string current_course, current_file;

    std::vector<MenuItem> m_course_items;
    std::map<std::string, std::vector<MenuItem>> m_notes_items;

    bool reset_current_item = true;

    void update_course_menu();
    void update_notes_menu();
    void update_preview_window();
    void register_callbacks();
    void display_preview_text();

    void open_pdf();
    void open_tex();


  public:
    BrowserView(CursesSession *session, NoteBinder *binder);
    ~BrowserView();
    void start();

    void course_scroll_up();
    void course_scroll_down();
    void notes_scroll_up();
    void notes_scroll_down();
    void handle_resize();

    BrowserView(const BrowserView&) = delete;
    BrowserView& operator=(const BrowserView&) = delete;
    BrowserView(BrowserView&&) = delete;
    BrowserView& operator=(BrowserView&&) = delete;
};
