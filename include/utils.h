#ifndef SYSFILES_H
    #include "sysfiles.h"
#endif


#ifndef UTILS_H
#define UTILS_H




class Utils {
  public:
    static const char* stringToChar(std::string phrase);
    static bool contains(std::vector<int> c, int e);
    static char* toUpper(char* text, char* upper);
    static std::deque<std::string> parsePath(char* pathFilename);
};

#endif