#include "../include/utils.h"

char* Utils::stringToChar(std::string phrase){
  return const_cast<char*>(phrase.c_str());
}

bool Utils::contains(std::vector<int> c, int e) { 
    return std::find(c.begin(), c.end(), e) != c.end();
}

std::vector<std::string> Utils::parsePath(char* pathFilename){
  std::vector<std::string> pathParsed;
  std::filesystem::path path(pathFilename);
  int x = 0;
  int j = 0;
  char dirName[9];
  while (path.parent_path().string().c_str()[x] != '\0'){
    dirName[j] = path.parent_path().string().c_str()[x];
    if(path.parent_path().string().c_str()[x+1] == '/' || path.parent_path().string().c_str()[x+1] == '\0'){
      j++;
      while(j != 8){
        dirName[j] = ' ';
        j++;
      }
      dirName[j] = 0;
      char upper[512];
      Utils::toUpper(dirName, upper);
      pathParsed.emplace_back(upper);
      memset(dirName, 0, 9);
      j = -1;
      if(!(path.parent_path().string().c_str()[x+1] == '\0')){
        x++;
      }
    }
    x++;
    j++;
  }


  char buffer[12];
  snprintf(buffer, 12, "%-8s%-3s", path.stem().string().c_str(), path.extension().string().substr(1,3).c_str());
  char upper[512];
  Utils::toUpper(buffer, upper);
  pathParsed.emplace_back(upper);

  return pathParsed;
}

char* Utils::toUpper(char* text, char* upper){
  int i = 0;
  while(text[i] != '\0'){
    upper[i] = toupper(text[i]);
    i++;
  }
  upper[i] = 0;
  return upper;
}