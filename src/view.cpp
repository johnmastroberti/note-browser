#include <algorithm>
#include <numeric>
#include <sys/types.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <fstream>
#include "view.hpp"

template<typename T, typename U>
constexpr std::common_type_t<T, U> MAX(T a, U b) { return (a > b) ? a : b; }

template<typename T, typename U>
constexpr std::common_type_t<T, U> MIN(T a, U b) { return (a > b) ? b : a; }

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
  update_course_menu();

  // Create menu items for each course's notes menu
  std::vector<MenuItem> notes_vec;
  for (const auto& course : *binder) {
    make_notes_items(course, notes_vec);
    m_notes_items.insert({course.get_name(), notes_vec});
  }

  // Create a menu for the first course in the list
  update_notes_menu();

  // Make the preview window
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


void BrowserView::handle_resize() {
  static std::ofstream resize_log("resize_log.txt");
  int br_w, br_h;
  getmaxyx(stdscr, br_h, br_w);
  endwin();
  refresh();
  getch();
  erase();
  int ar_w, ar_h;
  getmaxyx(stdscr, ar_h, ar_w);
  reset_current_item = false;
  auto [new_width, new_height] = m_session->get_dims();
  resize_log << "Handling resize for new width = "
    << new_width << ", new height = " << new_height << std::endl
    << "\t getmaxyx reports dimensions " << br_w << ", " << br_h << " before resize, "
    << ar_w << ", " << ar_h << " after" << std::endl << std::endl;
  constexpr auto min_width = 50;
  constexpr auto min_height = 10;
  if (new_width < min_width || new_height < min_height) {
    std::string too_small_message = "Window too small";
    mvprintw(new_height/2, MAX(0, new_width/2 - strlen(too_small_message.c_str())),
          too_small_message.c_str());
    return;
  }
  // Redraw all three windows
  refresh();
  update_course_menu();
  update_notes_menu();
  update_preview_window();
  reset_current_item = true;
  m_course_menu->refresh();
  m_notes_menu->refresh();
  if (m_preview_window) m_preview_window->refresh();
}


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


void BrowserView::open_pdf() {
  if (!m_current_file_ptr) return;
  const char* pdf_viewer = "zathura";
  auto pdf_path = m_current_file_ptr->m_path;
  pdf_path.replace_extension(".pdf");
  constexpr unsigned n = 200;
  char file[n];
  strncpy(file, pdf_path.string().c_str(), n);

  auto child_pid = fork();
  if (child_pid == 0) {
    execlp("st", "st", "-e", pdf_viewer, file, (const char*) NULL);
    throw; // give up if we can't exec
  }
  return;
}


void BrowserView::register_callbacks() {
  m_session->register_callback('q', [](chtype) { return true; });

  m_session->register_callback('j', [this](chtype) {
      this->notes_scroll_down();
      return false;
  });

  m_session->register_callback('k', [this](chtype) {
      this->notes_scroll_up();
      return false;
  });

  m_session->register_callback('n', [this](chtype) {
      this->course_scroll_down();
      return false;
  });

  m_session->register_callback('p', [this](chtype) {
      this->course_scroll_up();
      return false;
  });

  m_session->register_callback('o', [this](chtype) {
      this->open_pdf();
      return false;
  });

  m_session->register_callback('r', [this](chtype) {
      this->handle_resize();
      return false;
  });
}


void BrowserView::update_course_menu() {
  if (m_course_menu) delete m_course_menu;

  auto max_width = m_session->get_width();

  auto course_name_len = [](const NoteBook& course) { return course.get_name().size(); };
  auto course_name_comp = [&](const NoteBook& course1, const NoteBook& course2) {
    return course_name_len(course1) < course_name_len(course2);
  };
  auto longest_name = std::max_element(m_binder->begin(), m_binder->end(), course_name_comp);
  auto course_menu_width = course_name_len(*longest_name)+2+3;
  course_menu_width = MIN(course_menu_width, max_width/3);

  m_course_menu = new Menu{"Courses"s, m_course_items,
                            nullptr, {0,0}, course_menu_width, -1};
  // Reset current course and associated pointer
  if (reset_current_item) {
    current_course = m_course_menu->get_current_item().name;
    m_current_book_ptr = &(*(m_binder->begin())); // need this garbage to convert iterator to pointer
  } else {
    auto course_it = std::find_if(m_binder->begin(), m_binder->end(),
        [this](const auto& course) { return course.get_name() == current_course; });
    auto course_index = std::distance(m_binder->begin(), course_it);
    m_course_menu->set_current_item(course_index);
  }
  m_course_menu->refresh();
}


void BrowserView::update_notes_menu() {
  if (m_notes_menu) delete m_notes_menu;

  auto max_width = m_session->get_width();
  auto notes_menu_width = std::min(35, max_width/3);
  auto course_menu_width = m_course_menu->get_width();
  m_notes_menu = new Menu{current_course, m_notes_items.at(current_course),
                          nullptr, {course_menu_width+1, 0}, notes_menu_width, -1};

  // Check if there are any notes to display
  if (m_current_book_ptr->size()) {
    if (reset_current_item) {
      current_file = m_notes_menu->get_current_item().name;
      m_current_file_ptr = &(*(m_current_book_ptr->begin()));
    } else {
      auto note_it = std::find_if(m_current_book_ptr->begin(), m_current_book_ptr->end(),
          [this](const auto& note) { return note.m_display_name == current_file; });
      auto note_index = std::distance(m_current_book_ptr->begin(), note_it);
      m_notes_menu->set_current_item(note_index);
    }
  } else {
    m_notes_menu->printf({1, 3}, "<No notes found>");
    m_current_file_ptr = nullptr;
  }
  m_notes_menu->refresh();
}


void BrowserView::update_preview_window() {
  if (m_preview_window) delete m_preview_window;

  auto course_menu_width = m_course_menu->get_width();
  auto notes_menu_width = m_notes_menu->get_width();
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
