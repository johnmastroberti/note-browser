#include "notes.hpp"
#include <iostream>
#include <cstring>
#include <fstream>
#include <regex>

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
  std::regex section_regex{"\\\\(sub){1,}.*", std::regex_constants::egrep};
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

}


NoteBinder::NoteBinder(const fs::path& semester) : m_books{} {
  for (auto& p : fs::directory_iterator(semester)) {
    if (!fs::is_directory(p)) continue;
    auto notes_path = p / fs::path("notes");
    if (!fs::is_directory(notes_path)) continue;
    m_books.emplace_back(p);
  }
}
