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

struct NoteSection {
  unsigned level;
  std::string heading;
};


struct NoteFile {
  fs::path m_path;
  std::string m_file_name;
  std::string m_display_name;
  std::vector<NoteSection> m_sections;

  NoteFile(const fs::path& file);
};


class NoteBook {
  public:
    fs::path m_course_path;
    fs::path m_notes_path;
    std::string m_course_name;
    std::vector<NoteFile> m_notes;

  public:
    NoteBook(const fs::path& course);
};
