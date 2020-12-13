#include "../Includes/file_reader.hpp"

#include<sstream>
#include<fstream>
#include<iostream>
#include<unordered_set>
#include<stack>
#include<QFileSystemModel>
#include <map>
#include <stdlib.h>


std::string& ltrim(std::string& str, const std::string& chars = "\t\n\v\f\r ")
{
    str.erase(0, str.find_first_not_of(chars));
    return str;
}

std::string& rtrim(std::string& str, const std::string& chars = "\t\n\v\f\r ")
{
    str.erase(str.find_last_not_of(chars) + 1);
    return str;
}

std::string& trim(std::string& str, const std::string& chars = "\t\n\v\f\r ")
{
    return ltrim(rtrim(str, chars), chars);
}


std::string& strtolower(std::string &data) {
    std::transform(data.begin(), data.end(), data.begin(),
                          [](unsigned char c){ return std::tolower(c); });
    return data;
}

std::vector<std::string> split (const std::string &line, char delim) {
    std::vector<std::string> entries;
    std::stringstream strm(line);
    std::string tmp;
    while(std::getline(strm, tmp, delim)) {
        entries.push_back(trim(tmp));
    }
    return entries;
}

table loadFromFile(std::string path) {
    table result{};
    std::ifstream inputFile;
    inputFile.open(path);
    if(!inputFile.is_open()) {
        std::cout << "fajl ne postoji" << std::endl;
    }
    else {
        std::string line;
        std::getline(inputFile, line);
        std::vector<std::string> header = split(line,',');
        result.push(header);
        while(std::getline(inputFile, line)) {
            std::vector<std::string> entries = split(line, ',');
            std::vector<std::pair<std::string, entry>> instance;
            for(unsigned i = 0; i<entries.size(); i++) {
                instance.push_back(std::make_pair(header[i], entry(trim(entries[i]))));
            }
            result.push_row(instance);
        }
    }
    result.auto_deduce_types();
    return result;
}

std::vector<std::string> words_from_line(std::string &line) {
    std::vector<std::string> words;
    line.append("*");
    std::string tmp = "";
    bool building_word = false;
    for(char c:line) {
        if(!isalnum(c)) {
            if(building_word) {
                building_word = false;
                words.push_back(tmp);
                tmp = "";
            }
        }
        else {
            if(!building_word)
                building_word = true;
            tmp.push_back(c);
        }
    }

    return words;
}


void loadTextFromFile(std::string& path, std::unordered_set<std::string> &stop_words, table& term_table, bool tolower, bool binary){

    //Load words from the file into the dictionary
    term_table.push_row(std::vector<entry>(term_table.col_n(), entry(0.0)));
    QFileInfo * finfo = new QFileInfo(QString::fromStdString(path));
    term_table[term_table.row_n() - 1].set_name(finfo->fileName().toStdString());
    std::ifstream txtFile;
    txtFile.open(path);
    std::unordered_set<std::string> words_in_table;
    for(unsigned i = 0; i<term_table.col_names().size(); i++) {
        words_in_table.insert(term_table.col_names()[i]);
    }

    if(!txtFile.is_open()) {
        std::cerr << "Tekst fajl ne postoji" << std::endl;
        return;
    }

    std::string line;
    while(std::getline(txtFile,line)) {
        std::vector<std::string> words = words_from_line(line);
        if(tolower)
            std::transform(words.begin(), words.end(), words.begin(), [] (auto w) {return strtolower(w);});
        for(unsigned i = 0; i<words.size(); i++) {
            if(stop_words.find(words[i]) == stop_words.end()){
                if(words_in_table.find(words[i]) == words_in_table.end())
                        {
                            term_table.push(words[i]);
                            words_in_table.insert(words[i]);
                            std::for_each(term_table[words[i]].begin(), term_table[words[i]].end(), [](entry &e){e = 0;});
                            term_table[words[i]][term_table.row_n() - 1]++;
                        }
                    else {
                        if(!binary)
                            term_table[words[i]][term_table.row_n() - 1]++;

                    }
                  }

                }
            }

}


