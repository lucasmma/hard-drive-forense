#ifndef SYSFILES_H
#define SYSFILES_H

#include <string>
#include <iostream>
#include <fstream>
#include <dirent.h>
#include <stdio.h>
#include <sys/stat.h>
#include <cstdlib>

class Sysfiles {
  public:
    static void printDirectory(char* path);
    static void createDirectory(char* directoryName);
    static void createSizedFile(char* fileName, int sizeBytes);
    static void deleteFile(char* directoryName);
};


#endif