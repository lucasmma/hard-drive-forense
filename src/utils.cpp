#include "../include/utils.h"

char* Utils::stringToChar(std::string phrase){
  return const_cast<char*>(phrase.c_str());
}

bool Utils::contains(std::vector<int> c, int e) { 
    return std::find(c.begin(), c.end(), e) != c.end();
};