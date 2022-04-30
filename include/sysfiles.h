#ifndef SYSFILES_H
#define SYSFILES_H

#include <vector>
#include <deque>
#include <string>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <ctype.h>
#include <cerrno>
#include <stdexcept>
#include <algorithm>
#include <filesystem>
// #include <Windows.h>
#include "utils.h"
#include "fat32.h"

#define MAX_FILES 65534
#define SECTOR_PER_CLUSTER 2
#define END_OF_FILE 268435455


class Sysfiles {
  public:
    static void printDirectory(const char* path);
    static void changeDirectory(const char* directoryName);
    static void createDirectory(const char* directoryName);
    static void createSizedFile(const char* fileName, int sizeBytes);
    static void deleteFile(const char* directoryName);
};


#endif