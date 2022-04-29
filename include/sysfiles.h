#ifndef SYSFILES_H
#define SYSFILES_H

#include <string>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <cerrno>
#include <stdexcept>
#include <vector>
#include <algorithm>
#include "utils.h"
#include <Windows.h>
#include <filesystem>

#define MAX_FILES 65534;


class Sysfiles {
  public:
    static void printDirectory(char* path);
    static void changeDirectory(char* directoryName);
    static void createDirectory(char* directoryName);
    static void createSizedFile(char* fileName, int sizeBytes);
    static void deleteFile(char* directoryName);
};


#endif