#ifndef SYSFILES_H
#define SYSFILES_H

#include <string>
#include <iostream>
#include <fstream>
#include <dirent.h>

class Sysfiles {
  public:
    static void printDirectory(char* path);
    static void createDirectory(char* directoryName);
    static void createFile(char* fileName);
    static void deleteFile(char* fileName);
};


#endif