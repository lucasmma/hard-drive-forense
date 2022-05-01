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
#define UNICODE
#include <windows.h>
#include <Windows.h>
#include "fat32.h"
#include "utils.h"

#define MAX_FILES 65534
#define SECTOR_PER_CLUSTER 2
#define END_OF_FILE 268435455
#define NOT_FOUND 0
#define DEBUG true
#define PENDRIVE_PATH "\\\\.\\E:"


class Sysfiles {
  public:
    static void printDirectory(const char* path);
    static void changeDirectory(const char* directoryName);
    static void createDirectory(const char* directoryName);
    static void createSizedFile(const char* fileName, int sizeBytes);
    static int countFilesInDir(std::string directoryName);
    static void fillDirectory(const char* fileName);
    static void deleteFile(const char* directoryName);
};


#endif