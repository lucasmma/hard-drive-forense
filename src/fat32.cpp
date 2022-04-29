#include "../include/fat32.h"

// std::fstream* hardDrive;
Fat32::Fat32(char* directoryFile){
  readDisk(directoryFile);
  // fillInfo();
}

Fat32::~Fat32(){

}

void Fat32::print(){
  char buffer[512];
  hardDrive.read(&buffer[0], 512);

  printf("%d\n", *((unsigned short*)(&buffer[11])));
}

void Fat32::readDisk(char* directoryName){
  std::fstream disk;
  std::cout << directoryName << std::endl;
  hardDrive.open(directoryName, std::fstream::in | std::fstream::out | std::fstream::binary);

  if(!hardDrive)
    throw(std::runtime_error(directoryName + errno));

  // char buffer[512];

  // disk.read(&buffer[0], 512);
  // printf("%02x %02x %d \n", (unsigned char)buffer[510], (unsigned char)buffer[511], *((unsigned short*)(&buffer[11])));
}