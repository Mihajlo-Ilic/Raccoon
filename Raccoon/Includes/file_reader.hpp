#ifndef FILE_READER_HPP
#define FILE_READER_HPP

#include "table.hpp"

std::string& ltrim(std::string& str, const std::string& chars);

std::string& rtrim(std::string& str, const std::string& chars );

std::string& trim(std::string& str, const std::string& chars );

std::vector<std::string> split (const std::string &line, char delim);

table loadFromFile(std::string path);

#endif
