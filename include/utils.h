#ifndef SYSFILES_Hs
    #include "sysfiles.h"
#endif


#ifndef UTILS_H
#define UTILS_H




class Utils {
  public:
    static char* stringToChar(std::string phrase);
    static bool contains(std::vector<int> c, int e);
};

#endif