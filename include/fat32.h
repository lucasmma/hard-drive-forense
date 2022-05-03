#ifndef SYSFILES_H
    #include "sysfiles.h"
#endif


#ifndef FAT32_H
#define FAT32_H


struct FileInfo {
    char* filename;
    int startingClusterArea;
    int clusterIndex;
    int initialClusterOffSet;
    int startingFileAddress;
    int fileSize;
    int bitFieldAttribute;
};

class Fat32 {
  public:
    Fat32(WCHAR* directoryFile);
    ~Fat32();
    void printSector(int offSet);
    void printSector(char* buffer);
    void printCluster(char* bufferCluster);
    void undeleteFile(char* filename);
    char* readSector(int offSet);
    char* readCluster(int offSet);
    int getFatNfromOffset(int offSet);
    int getIntFromFatN(int offSet);
    void writeSector(int offSet, char* bufferSector);
    void writeCluster(int offSet, char* bufferCluster);
    void printFatInfos();
    FileInfo findArchiveOffset(std::deque<std::string> pathFileName, bool isDeleted, int clusterOffSet = 0);
    char* dirName;
    int bytesPerCluster;
    int bytesPerSector;
    int sectorPerCluster;
    int countSectorReserved;
    int countSectorPerFat;
    int offSetRootDirectory;
    int offSetFat1;
    int offSetFat2;
  private:
    HANDLE _device; 
    char _currentBuffer[512];
    void dismountVolume();
    void fillInfo();
    void setUpFat(WCHAR* directoryName);
};

#endif