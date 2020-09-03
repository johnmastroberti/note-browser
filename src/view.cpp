#include <algorithm>
#include <numeric>
#include "view.hpp"

void make_course_items(const NoteBinder& binder, std::vector<MenuItem>& items) {
  items.resize(binder.size());
  std::transform(binder.begin(), binder.end(), items.begin(),
      [](const auto& course) { return MenuItem{course.get_name(), ""s}; });
}

void make_notes_items(const NoteBook& course, std::vector<MenuItem>& items) {
  items.resize(course.size());
  std::transform(course.begin(), course.end(), items.begin(),
      [](const auto& note) { return MenuItem{note.m_display_name, note.m_file_name}; });
}

BrowserView::BrowserView(CursesSession *session, NoteBinder *binder)
  : m_session{session}, m_binder{binder}
{
  //auto [max_x, max_y] = session->get_dims();

  // Create course menu on the left side
  make_course_items(*binder, m_course_items);
  auto course_name_len = [](const NoteBook& course) { return course.get_name().size(); };
  auto course_name_comp = [&](const NoteBook& course1, const NoteBook& course2) {
    return course_name_len(course1) < course_name_len(course2);
  };
  auto longest_name = std::max_element(binder->begin(), binder->end(), course_name_comp);
  auto course_menu_width = course_name_len(*longest_name)+2+3;
  //auto course_menu_width = course_name_len(*std::max_element(binder->begin(), binder->end(), course_name_len))+2;

  m_course_menu = new Menu{"Courses"s, m_course_items,
                            nullptr, {0,0}, course_menu_width, -1};

  // Create menu items for each course's notes menu
  std::vector<MenuItem> notes_vec;
  for (const auto& course : *binder) {
    make_notes_items(course, notes_vec);
    m_notes_items.insert({course.get_name(), notes_vec});
  }

  // Create a menu for the first course in the list
  current_course = m_course_menu->get_current_item().name;
  m_current_book_ptr = &(*(m_binder->begin())); // need this garbage to convert iterator to pointer
  update_notes_menu();

  // Make the preview window
  m_current_file_ptr = &(*(m_current_book_ptr->begin())); // same here
  update_preview_window();

  register_callbacks();
  session->refresh();
  m_course_menu->refresh();
  m_notes_menu->refresh();
  if (m_preview_window) m_preview_window->refresh();
}


BrowserView::~BrowserView() {
  if (m_course_menu) delete m_course_menu;
  if (m_notes_menu) delete m_notes_menu;
  if (m_preview_window) delete m_preview_window;
}


void BrowserView::start() { m_session->loop(); }


void BrowserView::notes_scroll_up() {
  if (m_current_book_ptr->size()) {
    m_notes_menu->sel_up();
    if (current_file != m_notes_menu->get_current_item().name) {
      m_current_file_ptr--;
      current_file = m_notes_menu->get_current_item().name;
      update_preview_window();
    }
    m_notes_menu->refresh();
  }
}


void BrowserView::notes_scroll_down() {
  if (m_current_book_ptr->size()) {
    m_notes_menu->sel_down();
    if (current_file != m_notes_menu->get_current_item().name) {
      m_current_file_ptr++;
      current_file = m_notes_menu->get_current_item().name;
      update_preview_window();
    }
    m_notes_menu->refresh();
  }
}


void BrowserView::course_scroll_up() {
  m_course_menu->sel_up();
  if (current_course != m_course_menu->get_current_item().name) {
    m_current_book_ptr--;
    current_course = m_course_menu->get_current_item().name;
    update_notes_menu();
    update_preview_window();
  }
  m_course_menu->refresh();
}


void BrowserView::course_scroll_down() {
  m_course_menu->sel_down();
  if (current_course != m_course_menu->get_current_item().name) {
    m_current_book_ptr++;
    current_course = m_course_menu->get_current_item().name;
    update_notes_menu();
    update_preview_window();
  }
  m_course_menu->refresh();
}


void BrowserView::register_callbacks() {
  m_session->register_callback('q', [](decltype(KEY_UP)) { return true; });

  m_session->register_callback('j', [this](decltype(KEY_UP)) {
      this->notes_scroll_down();
      return false;
  });

  m_session->register_callback('k', [this](decltype(KEY_UP)) {
      this->notes_scroll_up();
      return false;
  });

  m_session->register_callback('n', [this](decltype(KEY_UP)) {
      this->course_scroll_down();
      return false;
  });

  m_session->register_callback('p', [this](decltype(KEY_UP)) {
      this->course_scroll_up();
      return false;
  });
}


void BrowserView::update_notes_menu() {
  if (m_notes_menu) delete m_notes_menu;

  constexpr auto notes_menu_width = 35;
  auto course_menu_width = m_course_menu->get_width();
  m_notes_menu = new Menu{current_course, m_notes_items.at(current_course),
                          nullptr, {course_menu_width+1, 0}, notes_menu_width, -1};

  // Check if there are any notes to display
  if (m_current_book_ptr->size()) {
    current_file = m_notes_menu->get_current_item().name;
    m_current_file_ptr = &(*(m_current_book_ptr->begin()));
  } else {
    m_notes_menu->printf({1, 3}, "<No notes found>");
    m_current_file_ptr = nullptr;
  }
  m_notes_menu->refresh();
}


void BrowserView::update_preview_window() {
  if (m_preview_window) delete m_preview_window;

  auto course_menu_width = m_course_menu->get_width();
  constexpr auto notes_menu_width = 35;
  auto preview_offset = notes_menu_width + course_menu_width+2;

  // Only make a new window if there are notes to display
  if (m_current_file_ptr) {
    m_preview_window = new Window{nullptr, {preview_offset, 0}};
    m_preview_window->display_title(current_file);

    display_preview_text();

    m_preview_window->refresh();
  } else {
    m_preview_window = nullptr;
  }
}


void BrowserView::display_preview_text() {
  // Display the outline nicely
  int line = 3;
  for (const auto& s : m_current_file_ptr->m_sections) {
    m_preview_window->printf({s.level*2 + 1, line}, "*");
    m_preview_window->printf({s.level*2 + 3, line}, s.heading.c_str());
    line++;
  }
}
