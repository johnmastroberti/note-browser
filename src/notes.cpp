#include "notes.hpp"
#include <iostream>
#include <cstring>
#include <fstream>
#include <regex>
#include <algorithm>

NoteFile::NoteFile(const fs::path& file)
  : m_path{file}, m_file_name{file.filename()}
{
  // Open file for reading
  std::ifstream notefile;
  notefile.open(file);
  if (!notefile.good()) {
    m_display_name = "FILE NOT FOUND";
    return;
  }

  // Read out the date line and any section heading lines from the file
  std::string date;
  std::vector<std::string> section_lines;
  std::string scratch;
  std::regex date_regex{"\\\\date{.*}", std::regex_constants::grep};
  std::regex section_regex{"\\\\(sub)*section.*", std::regex_constants::egrep};
  while(notefile.good()) {
    std::getline(notefile, scratch);
    if (!notefile.good()) break;
    if (std::regex_match(scratch, date_regex))
      date = scratch;
    else if (std::regex_match(scratch, section_regex))
      section_lines.push_back(scratch);
  }

  // Set display name to date contents
  m_display_name = date.substr(strlen("\\date{"),
                              date.size()-strlen("\\date{")-strlen("}"));

  // Parse section headings into NoteSection objects
  for (auto& line : section_lines) {
    auto sec_start = line.find("section");
    m_sections.push_back({static_cast<unsigned>((sec_start-1)/3), line.substr(sec_start+8, line.size()-sec_start-8-1)});
  }


}


NoteBook::NoteBook(const fs::path& course)
  : m_course_path{course},
    m_notes_path{course / "notes"}
{
  if (!fs::is_directory(m_notes_path)) throw;
  // Get the course name
  std::ifstream readme;
  readme.open(course / "README.md");
  std::getline(readme, m_course_name);
  m_course_name = m_course_name.substr(2);

  // Make NoteFile's for each appropriately
  // formatted tex file in the notes directory
  std::regex note_format{".*[0-9]{2}-[0-9]{2}.tex", std::regex_constants::egrep};
  for (const auto& file : fs::directory_iterator(m_notes_path)) {
    //std::cout << file.path().string();
    if (std::regex_match(file.path().string(), note_format)) {
      //std::cout << " matches";
      m_notes.push_back(NoteFile{file.path()});
    }
    //std::cout << "\n";
  }

  // Sort the notes files in choronological order
  auto note_sort = [](const NoteFile& file1, const NoteFile& file2) {
    const auto& filename1 = file1.m_file_name;
    const auto& filename2 = file2.m_file_name;
    // File name format: mm-dd.tex
    auto mm1 = stoi(filename1.substr(0, 2));
    auto dd1 = stoi(filename1.substr(3, 2));
    auto mm2 = stoi(filename2.substr(0, 2));
    auto dd2 = stoi(filename2.substr(3, 2));
    return std::make_pair(mm1, dd1) < std::make_pair(mm2, dd2);
  };
  std::sort(m_notes.begin(), m_notes.end(), note_sort);

}


NoteBinder::NoteBinder(const fs::path& semester) : m_books{} {
  for (auto& p : fs::directory_iterator(semester)) {
    if (!fs::is_directory(p)) continue;
    auto notes_path = p / fs::path("notes");
    if (!fs::is_directory(notes_path)) continue;
    m_books.emplace_back(p);
  }

  // Sort the books in alphabetical order
  std::sort(m_books.begin(), m_books.end(),
      [](const auto& book1, const auto& book2) { return book1.get_name() < book2.get_name(); });
}
