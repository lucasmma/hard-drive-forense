#include "../include/fat32.h"

// std::fstream* hardDrive;
Fat32::Fat32(char* directoryName){
  dirName = directoryName;
  setUpFat();
}

void Fat32::setUpFat(){
  readDisk();
  fillInfo();
}

Fat32::~Fat32(){

}

void Fat32::printFatInfos(){
  std::cout << "-------------------------------------------------------------------" << std::endl;
  std::cout << "Bytes por Setor " << bytesPerSector << std::endl;
  std::cout << "Setores por Cluster " << sectorPerCluster << std::endl;
  std::cout << "Numero de Setores Reservado " << countSectorReserved << std::endl;
  std::cout << "Numero de Setores por FAT " << countSectorPerFat << std::endl;
  std::cout << "Bytes por Cluster " << bytesPerCluster << std::endl;
  std::cout << "Offset do FAT1 " << offSetFat1 << std::endl;
  std::cout << "Offset do FAT2 " << offSetFat2 << std::endl;
  std::cout << "Offset ate Root Directory " << offSetRootDirectory << std::endl;
  std::cout << "-------------------------------------------------------------------" << std::endl;
}

void Fat32::printSector(int offSet){
  char* buffer = readSector(offSet);
  std::cout << std::endl << std::endl;
  std::cout << "-------------------------------------------------------------------" << std::endl;
  std::cout << "Starting offSET " << offSet << std::endl;
  std::cout << std::endl << std::endl << "    ";
  for (int x = 0; x < 16; x++){
    std::cout<< ((x <= 9) ? "0" : "") << (x) << " ";
  }
  std::cout << std::endl;
  for (int i = 0; i < 32; i++){
    printf("%s%s%d ",(i*16 <= 99) ? "0" : "",(i*16 <= 9) ? "0" : "", i*16);
    for (int j = 0; j < 16; j++){
      printf("%02X ", *((unsigned char*)&buffer[(i*16) + j]));
    }
    printf("\n");
  }
  std::cout << std::endl << std::endl;
  std::cout << "-------------------------------------------------------------------";
  std::cout << std::endl << std::endl;
}

char* Fat32::readSector(int offSet){
  hardDrive.seekg(offSet, std::ios_base::beg);
  hardDrive.read(&_currentBuffer[0], 512);
  return _currentBuffer;
}

char* Fat32::readCluster(int offSet){
  static char cluster[SECTOR_PER_CLUSTER * 512];
  for(int i = 0; i < SECTOR_PER_CLUSTER; i ++){
    memcpy(&cluster[i*512], readSector(offSet + (i * 512)), 512);
  }
  return cluster;
}

void Fat32::writeSector(int offSet, char* bufferSector){
  hardDrive.seekp(offSet, std::ios_base::beg);
  hardDrive.write(bufferSector, 512);
}

int Fat32::getFatNfromOffset(int offSet){
  return (int)((offSet - offSetRootDirectory) / bytesPerCluster) + 2;
}

int Fat32::getIntFromFatN(int n){
  int offSetSector = (n*4) + offSetFat1 - (((n*4) + offSetFat1) % 512);
  std::cout << offSetSector << std::endl;
  char* fatSector = readSector(offSetSector);
  return *((unsigned int*)(&fatSector[(((n*4) + offSetFat1) % 512)]));
}

void Fat32::fillInfo(){
  char* firstSector =  readSector(0);
  bytesPerSector = *((unsigned short*)(&firstSector[11]));
  sectorPerCluster = *((unsigned char*)(&firstSector[13]));
  countSectorReserved = *((unsigned short*)(&firstSector[14]));
  countSectorPerFat = *((unsigned int*)(&firstSector[36]));
  bytesPerCluster = bytesPerSector * sectorPerCluster;
  offSetRootDirectory = bytesPerSector * (countSectorReserved + (countSectorPerFat * 2));
  offSetFat1 = bytesPerSector * countSectorReserved;
  offSetFat2 = bytesPerSector * (countSectorReserved + countSectorPerFat);
}

int Fat32::findArchiveOffset(std::deque<std::string> pathFileName, bool isDeleted, int clusterOffSet){
  int initialClusterOffSet = clusterOffSet;
  if(clusterOffSet == 0){
    initialClusterOffSet = offSetRootDirectory;
  }
  char* rootDirectory = readCluster(initialClusterOffSet);
  for (int i = 0; i < 512*SECTOR_PER_CLUSTER/32; i++){
    char bufferName[12];
    char bitFieldAttribute = *((unsigned char*)(&rootDirectory[i * 32 + 11]));
    int startingClusterArea = *((unsigned short*)(&rootDirectory[i * 32 + 26]));
    int fileSize = *((unsigned int*)(&rootDirectory[i * 32 + 28]));
    int startingFileAddress = (startingClusterArea - 2) * bytesPerCluster + offSetRootDirectory;
    char bufferFilename[11];
    if(pathFileName.size() != 1){
      memcpy(bufferName, &rootDirectory[i*32], 8);
      bufferName[8] = 0;
    } else {
      memcpy(bufferName, &rootDirectory[i*32], 11);
      bufferName[11] = 0;
    }
    if(isDeleted && pathFileName.size() == 1){
      strcpy(bufferFilename, pathFileName[0].c_str());
      bufferFilename[0] = 0xE5;
    } else{
      strcpy(bufferFilename, pathFileName[0].c_str());
    }
    if(strcmp(bufferName, bufferFilename) == 0){
      // ACHOU
      pathFileName.pop_front();
      printf("Name: %s -> HEX: %02X\n", bufferName, *((unsigned int*)(&bufferName)));
      printf("Bit Attribute: %02X\n", bitFieldAttribute);
      printf("Starting Cluster Area: %d\n", startingClusterArea);
      printf("Bytes Size: %d\n", fileSize);
      printf("Starting File Address: %d\n", startingFileAddress);
      printf("Cluster offset: %d\n\n", initialClusterOffSet);
      printf("--------------------------------------\n");
      if(pathFileName.size() == 0){
        printf("retornou\n");
        return startingFileAddress;
      }
      return findArchiveOffset(pathFileName, isDeleted, startingFileAddress);
    }
  }
  std::cout << "N fat -> " << getFatNfromOffset(initialClusterOffSet) << std::endl;
  int numberInFat = getIntFromFatN(getFatNfromOffset(initialClusterOffSet));
  std::cout << "Number in Fat -> " << numberInFat << std::endl;
  if(numberInFat == END_OF_FILE){
    return NOT_FOUND;
  } else {
    int nextCluserOffset = (numberInFat - 2) * bytesPerCluster + offSetRootDirectory;
    return findArchiveOffset(pathFileName, isDeleted, nextCluserOffset);
  }
}

void Fat32::undeleteFile(char* filename){

}

void Fat32::readDisk(){
  std::fstream disk;
  hardDrive.open(dirName, std::fstream::in | std::fstream::out | std::fstream::binary);

  if(!hardDrive)
    throw(std::runtime_error(dirName + errno));

  // char buffer[512];

  // disk.read(&buffer[0], 512);
  // printf("%02x %02x %d \n", (unsigned char)buffer[510], (unsigned char)buffer[511], *((unsigned short*)(&buffer[11])));
}