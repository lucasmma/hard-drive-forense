#include "../include/fat32.h"

// std::fstream* hardDrive;
Fat32::Fat32(char* directoryFile){
  readDisk(directoryFile);
  fillInfo();
}

Fat32::~Fat32(){

}

void Fat32::printSector(int offSet){
  char buffer[512];
  hardDrive.seekg(offSet, std::ios_base::beg);
  hardDrive.read(&buffer[0], 512);
  std::cout << std::endl << std::endl;
  std::cout << "-------------------------------------------------------------------";
  std::cout << std::endl << std::endl << "    ";
  for (int x = 0; x < 16; x++){
    std::cout<< ((x <= 9) ? "0" : "") << (x) << " ";
  }
  std::cout << std::endl;
  for (int i = 0; i < 32; i++){
    printf("%s%s%d ",(i*16 <= 9) ? "0" : "", (i*16 <= 99) ? "0" : "", i*16);
    for (int j = 0; j < 16; j++){
      printf("%02X ", *((unsigned char*)&buffer[(i*16) + j]));
    }
    printf("\n");
  }
  std::cout << std::endl << std::endl;
  std::cout << "-------------------------------------------------------------------";
  std::cout << std::endl << std::endl;
}

void Fat32::fillInfo(){

}

void Fat32::readDisk(char* directoryName){
  std::fstream disk;
  hardDrive.open(directoryName, std::fstream::in | std::fstream::out | std::fstream::binary);

  if(!hardDrive)
    throw(std::runtime_error(directoryName + errno));

  // char buffer[512];

  // disk.read(&buffer[0], 512);
  // printf("%02x %02x %d \n", (unsigned char)buffer[510], (unsigned char)buffer[511], *((unsigned short*)(&buffer[11])));
}