#ifndef SYSFILES_Hs
    #include "sysfiles.h"
#endif


#ifndef FAT32_H
#define FAT32_H




class Fat32 {
  public:
    Fat32(char* directoryFile);
    ~Fat32();
    void readDisk(char* directoryName);
    void print();
    std::fstream hardDrive;
};

#endif