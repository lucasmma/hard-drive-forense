#include "../include/fat32.h"

Fat32::Fat32(WCHAR* directoryName){
  dirName = PENDRIVE_PATH;
  setUpFat(directoryName);
}



void Fat32::setUpFat(WCHAR* directoryName){
  _device = CreateFileW(directoryName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
  if(_device == INVALID_HANDLE_VALUE) {
    std::cout << "Handle nao criado" << std::endl;
  }
  fillInfo();
}

Fat32::~Fat32(){
  CloseHandle(_device);
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

void Fat32::printSector(char* buffer){
  std::cout << std::endl << std::endl;
  std::cout << "-------------------------------------------------------------------" << std::endl;
  std::cout << "Buffer " << std::endl;
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
void Fat32::dismountVolume(){
  DWORD returned;
  if (!DeviceIoControl(_device, FSCTL_DISMOUNT_VOLUME, NULL, 0, NULL, 0, &returned, NULL)){
      DWORD err = GetLastError();
      printf("Error %d attempting to dismount volume, error code\n",err);
  }
}

char* Fat32::readSector(int offSet){
  DWORD returned;
  SetFilePointer(_device, offSet, NULL, FILE_BEGIN);
  if(!ReadFile(_device, &_currentBuffer[0], 512, &returned, NULL)){
    DWORD err = GetLastError();
    printf("Error %d attempting to read volume on index %d, error code\n",err, offSet);
    // getchar();
  }
  // hardDrive.seekg(offSet, std::ios_base::beg);
  // hardDrive.read(&_currentBuffer[0], 512);
  return _currentBuffer;
}

char* Fat32::readCluster(int offSet){
  static char cluster[SECTOR_PER_CLUSTER * 512];
  for(int i = 0; i < SECTOR_PER_CLUSTER; i ++){
    memcpy(&cluster[i*512], readSector(offSet + (i * 512)), 512);
  }
  return cluster;
}

void Fat32::printCluster(char* bufferCluster){
  printSector(bufferCluster);
  printSector(&bufferCluster[512]);
}

void Fat32::writeSector(int offSet, char* bufferSector){
  DWORD returned;
  if(DeviceIoControl(_device, FSCTL_IS_VOLUME_MOUNTED, NULL, 0, NULL, 0, &returned, NULL)){
    // if (!DeviceIoControl(_device, FSCTL_DISMOUNT_VOLUME,
    //     NULL, 0, NULL, 0, &returned, NULL)){
    //     DWORD err = GetLastError();
    //     printf("Error %d attempting to dismount volume, error code\n",err);
    // }
    
    if(!DeviceIoControl(_device, FSCTL_LOCK_VOLUME, NULL, 0, NULL, 0, &returned, NULL)){
      DWORD err = GetLastError();
      printf("Error %d attempting to lock volume, error code\n",err);
    }
    if(DEBUG) {
      std::cout << "Volume Locked " << std::endl;
    }
  }

  SetFilePointer(_device, offSet, NULL, FILE_BEGIN);
  WriteFile(_device, bufferSector, 512, &returned, NULL);

  if(!DeviceIoControl(_device, FSCTL_UNLOCK_VOLUME, NULL, 0, NULL, 0, &returned, NULL)){
    DWORD err = GetLastError();
    printf("Error %d attempting to unlock volume, error code\n",err);
  }
  if(DEBUG) {
    std::cout << "Volume Unocked " << std::endl;
  }
}

void Fat32::writeCluster(int offSet, char* bufferCluster){
  writeSector(offSet, bufferCluster);
  writeSector(offSet + 512, &bufferCluster[512]);
}


int Fat32::getFatNfromOffset(int offSet){
  return (unsigned int)((offSet - offSetRootDirectory) / bytesPerCluster) + 2;
}

int Fat32::getIntFromFatN(int n){
  int offSetSector = offSetFat1 + n / 128 * 512;
  // std::cout << offSetSector << std::endl;
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

FileInfo Fat32::findArchiveOffset(std::deque<std::string> pathFileName, bool isDeleted, int clusterOffSet){
  int initialClusterOffSet = clusterOffSet;
  if(clusterOffSet == 0){
    initialClusterOffSet = offSetRootDirectory;
  }
  
  if(DEBUG){
    printf("--------------------------------------\n");
    std::cout << "FindArchiveOffset ClusterOffsetInicial e: " << initialClusterOffSet << std::endl;;
  }
  char* cluster = readCluster(initialClusterOffSet);
  if(DEBUG){
    printSector(initialClusterOffSet);
    printSector(initialClusterOffSet + 512);
  }
  for (int i = 0; i < 512*SECTOR_PER_CLUSTER/32; i++){
    struct FileInfo fileInfo = Utils::parseFileBytes(this, cluster, i*32, pathFileName.size() == 1);
    char bufferFilename[12];
    if(isDeleted && pathFileName.size() == 1){
      strcpy(bufferFilename, pathFileName[0].c_str());
      bufferFilename[0] = 0xE5;
    } else{
      strcpy(bufferFilename, pathFileName[0].c_str());
    }
    if(strcmp(fileInfo.filename, bufferFilename) == 0 && fileInfo.fileSize > 0 && fileInfo.startingClusterArea > 2){
      // ACHOU
      pathFileName.pop_front();
      if(DEBUG){
        printf("Offset do cluster: %d\n", initialClusterOffSet);
        printf("Index no cluster: %d\n", i*32 + initialClusterOffSet);
        printf("Name: %s -> HEX: %02X\n", fileInfo.filename, *((unsigned long long*)(&fileInfo.filename)));
        printf("Bit Attribute: %02X\n", fileInfo.bitFieldAttribute);
        printf("Starting Cluster Area: %d\n", fileInfo.startingClusterArea);
        printf("Bytes Size: %d\n", fileInfo.fileSize);
        std::cout << "Bytes Size > 0: " << (fileInfo.fileSize > 0) << std::endl;
        printf("Starting File Address: %d\n", fileInfo.startingFileAddress);
        printf("--------------------------------------\n");
      }
      if(pathFileName.size() == 0){
        if(DEBUG){
          printf("Endereco do arquivo -> %d\n", fileInfo.startingFileAddress);
        }
        fileInfo.initialClusterOffSet = initialClusterOffSet;
        fileInfo.clusterIndex = i*32;
        return fileInfo;
      }
      return findArchiveOffset(pathFileName, isDeleted, fileInfo.startingFileAddress);
    }
  }
  int numberInFat = getIntFromFatN(getFatNfromOffset(initialClusterOffSet));
  if(DEBUG){
    std::cout << "N fat -> " << getFatNfromOffset(initialClusterOffSet) << std::endl;
    std::cout << "Number in Fat -> " << numberInFat << std::endl;
  }
  if(numberInFat == END_OF_FILE){
    struct FileInfo fileInfo;
    fileInfo.startingClusterArea = NOT_FOUND;
    fileInfo.clusterIndex = NOT_FOUND;
    fileInfo.startingFileAddress = NOT_FOUND;
    return fileInfo;
  } else {
    int nextCluserOffset = (numberInFat - 2) * bytesPerCluster + offSetRootDirectory;
    return findArchiveOffset(pathFileName, isDeleted, nextCluserOffset);
  }
}

void Fat32::undeleteFile(char* filename){
  // para recuperar um arquivo pequeno
  // achar o offset
  FileInfo fileInfo = findArchiveOffset(Utils::parsePath(filename), true);
  if(fileInfo.startingFileAddress == NOT_FOUND){
    std::cout << "Arquivo não encontrado" << std::endl;
    return;
  }
  // ler o cluster do arquivo
  char* archiveCluster = readCluster(fileInfo.initialClusterOffSet);
  archiveCluster[fileInfo.clusterIndex] = Utils::parsePath(filename).back()[0];
  // int fileSize = 1000;
  // memcpy(&archiveCluster[fileInfo.clusterIndex + 28], (unsigned char*)&fileSize, 4);
  std::cout << "Initial cluster off set" << fileInfo.initialClusterOffSet << std::endl; 
  std::cout << "Cluster Index " << fileInfo.clusterIndex << std::endl; 
  std::cout << "Size do arquivo " << fileInfo.fileSize << std::endl; 
  printCluster(archiveCluster);
  // escrever o cluster do arquivo
  writeCluster(fileInfo.initialClusterOffSet, archiveCluster);
  int nOffset = fileInfo.startingClusterArea;
  int fat1SectorOffset = (nOffset*4) + offSetFat1 - (((nOffset*4) + offSetFat1) % 512);
  int fat2SectorOffset = (nOffset*4) + offSetFat2 - (((nOffset*4) + offSetFat2) % 512);
  char* fatSector = readSector(fat1SectorOffset);
  std::cout << "Numero do offset " << nOffset << std::endl;
  std::cout << "Numero do offset do fat 1 " << fat1SectorOffset << std::endl;
  std::cout << "Numero do offset do fat 2 " << fat2SectorOffset << std::endl;
  std::cout << "Numero do index do fat " << ((nOffset*4) + offSetFat1) % 512 << std::endl;
  // escrever na fat se é o fim do arquivo ou não
  // Criar uma função que escreve na fat
  // printCluster(fatSector);
  // fatSector[((nOffset*4) + offSetFat1) % 512] = *(unsigned char*)END_OF_FILE;
  printSector(fatSector);
  memcpy(&fatSector[((nOffset*4) + offSetFat1) % 512], (unsigned char*)&END_OF_FILE, 4);
  printSector(fatSector);
  writeSector(fat1SectorOffset, fatSector);
  writeSector(fat2SectorOffset, fatSector);

}

// void Fat32::readDisk(){
//   std::fstream disk;
//   hardDrive.open(PENDRIVE_PATH, std::fstream::in | std::fstream::out | std::fstream::binary);

//   if(!hardDrive)
//     throw(std::runtime_error(PENDRIVE_PATH + errno));

//   // char buffer[512];

//   // disk.read(&buffer[0], 512);
//   // printf("%02x %02x %d \n", (unsigned char)buffer[510], (unsigned char)buffer[511], *((unsigned short*)(&buffer[11])));
// }