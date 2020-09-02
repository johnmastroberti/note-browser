#pragma once
#include <vector>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

// Problems that need to be solved:
  // Get the list of classes from somewhere (hardcoded?)
  // Get the list of notes files for each class
  // Generate display names for each file (date?)
  // Read each notes file for section headings

// Represents one section heading in a notes file
struct NoteSection {
  unsigned level;
  std::string heading;
};


// Represents one notes files, containing all the file's section headings
struct NoteFile {
  fs::path m_path;
  std::string m_file_name;
  std::string m_display_name;
  std::vector<NoteSection> m_sections;

  NoteFile(const fs::path& file);
};


// Contains all the notes for a single course
class NoteBook {
  private:
    fs::path m_course_path;
    fs::path m_notes_path;
    std::string m_course_name;
    std::vector<NoteFile> m_notes;

  public:
    NoteBook(const fs::path& course);

    inline auto get_name() const { return m_course_name; }
    inline auto get_path() const { return m_course_path; }
    inline auto get_notes_dir() const { return m_notes_path; }
    inline auto size() const { return m_notes.size(); }

    // Iterators to the note files
    inline auto begin() { return m_notes.begin(); }
    inline auto end() { return m_notes.end(); }
    inline auto begin() const { return m_notes.begin(); }
    inline auto end() const { return m_notes.end(); }
    inline auto cbegin() const { return m_notes.cbegin(); }
    inline auto cend() const { return m_notes.cend(); }
    inline auto rbegin() { return m_notes.rbegin(); }
    inline auto rend() { return m_notes.rend(); }
    inline auto rbegin() const { return m_notes.rbegin(); }
    inline auto rend() const { return m_notes.rend(); }
    inline auto crbegin() const { return m_notes.crbegin(); }
    inline auto crend() const { return m_notes.crend(); }

};

// Represents all courses in a semester
class NoteBinder {
  private:
    std::vector<NoteBook> m_books;

  public:
    NoteBinder(const fs::path& semester);

    inline auto size() const { return m_books.size(); }

    // Iterators to the note books
    inline auto begin() { return m_books.begin(); }
    inline auto end() { return m_books.end(); }
    inline auto begin() const { return m_books.begin(); }
    inline auto end() const { return m_books.end(); }
    inline auto cbegin() const { return m_books.cbegin(); }
    inline auto cend() const { return m_books.cend(); }
    inline auto rbegin() { return m_books.rbegin(); }
    inline auto rend() { return m_books.rend(); }
    inline auto rbegin() const { return m_books.rbegin(); }
    inline auto rend() const { return m_books.rend(); }
    inline auto crbegin() const { return m_books.crbegin(); }
    inline auto crend() const { return m_books.crend(); }
};

