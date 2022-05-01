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
    void undeleteFile(char* filename);
    char* readSector(int offSet);
    char* readCluster(int offSet);
    int getFatNfromOffset(int offSet);
    int getIntFromFatN(int offSet);
    void writeSector(int offSet, char* bufferSector);
    void fillInfo();
    void setUpFat();
    void printFatInfos();
    int findArchiveOffset(std::deque<std::string> pathFileName, bool isDeleted, int clusterOffSet = 0);
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