#ifndef SYSFILES_H
    #include "sysfiles.h"
#endif


#ifndef UTILS_H
#define UTILS_H




class Utils {
  public:
    static bool contains(std::vector<int> c, int e);
    static char* toUpper(char* text, char* upper);
    static int calculateIntfromHighLow(int high, int low);
    static FileInfo parseFileBytes(Fat32* fat, char* cluster, int offSetCluster, bool isFile);
    static std::deque<std::string> parsePath(char* pathFilename);
};

#endif