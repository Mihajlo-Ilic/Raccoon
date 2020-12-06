#ifndef FILE_READER_HPP
#define FILE_READER_HPP

#include "table.hpp"
#include <unordered_set>

std::string& ltrim(std::string& str, const std::string& chars);

std::string& rtrim(std::string& str, const std::string& chars );

std::string& trim(std::string& str, const std::string& chars );

std::vector<std::string> split (const std::string &line, char delim);

std::vector<std::string> words_from_line(std::string &line);

std::string& strtolower(std::string &data);

table loadFromFile(std::string path);

void loadTextFromFile(std::string& path, std::unordered_set<std::string> &stop_words, table& term_table, bool tolower, bool binary);

void loadTextFromDir(std::string& path_string, std::unordered_set<std::string> &stop_words, table& term_table, bool tolower, bool binary);
#endif
