#ifndef SYSFILES_H
    #include "sysfiles.h"
#endif


#ifndef FAT32_H
#define FAT32_H




class Fat32 {
  public:
    Fat32(char* directoryFile);
    ~Fat32();
    void readDisk();
    void printSector(int offSet);
    char* readSector(int offSet);
    void writeSector(int offSet, char* bufferSector);
    void fillInfo();
    void setUpFat();
    void printFatInfos();
    int findArchiveOffset(char* pathFileName);
    char* dirName;
    std::fstream hardDrive;
    int bytesPerCluster;
    int bytesPerSector;
    int sectorPerCluster;
    int countSectorReserved;
    int countSectorPerFat;
    int offSetRootDirectory;
    int offSetFat1;
    int offSetFat2;
  private:
    char _currentBuffer[512];
};

#endif