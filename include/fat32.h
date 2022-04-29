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
    void printSector(int offSet);
    void fillInfo();
    std::fstream hardDrive;
    // int bytesPerSector;
    // int hardDrive;
    // int hardDrive;
    // int hardDrive;
};

#endif