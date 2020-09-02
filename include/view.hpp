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

    void update_notes_menu();
    void update_preview_window();
    void register_callbacks();


  public:
    BrowserView(CursesSession *session, NoteBinder *binder);
    ~BrowserView();
    void start();

    BrowserView(const BrowserView&) = delete;
    BrowserView& operator=(const BrowserView&) = delete;
    BrowserView(BrowserView&&) = delete;
    BrowserView& operator=(BrowserView&&) = delete;
};
