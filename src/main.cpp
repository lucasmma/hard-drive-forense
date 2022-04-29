#include "../include/sysfiles.h"

void deleteRandomFilesInDirectory(std::string baseFileName, int nFilesToDelete, int lastIndex) {
  int deletedFiles = 0;
  std::vector<int> deletedNumbers;

  while (deletedFiles != nFilesToDelete){
    int randomIndex = (rand() % (int)(lastIndex/2)) * 2;
    while(Utils::contains(deletedNumbers, randomIndex)){
      randomIndex = (rand() % (int)(lastIndex/2)) * 2;
    }
    Sysfiles::deleteFile(Utils::stringToChar(baseFileName + std::to_string(randomIndex)));
    deletedNumbers.push_back(randomIndex);
    deletedFiles++;
  }
}

void fillDirectoryUntillFull(std::string dirname) {
    try
    {
        int maxFiles = 0;
        while(true) {
            // Sysfiles::createSizedFile(dirname + "/" + std::to_string(maxFiles) + ".txt", 10000000);
            maxFiles ++;
            std::cout << "Creating File: " << dirname << "/" << maxFiles << std::endl;
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    std::cout << "Full" << std::endl;
}


void setupPendrive() {
  std::string baseDirectoryName = "SD";
  //create 5 subdirectories with files
  for (int i = 0; i < 5; i++) {
    std::string dirName = baseDirectoryName + std::to_string(i);
    char* dirNameChar = Utils::stringToChar(dirName);
    Sysfiles::createDirectory(dirNameChar);
    Sysfiles::changeDirectory(dirNameChar);
    for (int j = 0; j < 371; j++) {
      Sysfiles::createSizedFile(Utils::stringToChar(std::to_string(j)), 1000);
    }
    Sysfiles::changeDirectory("..");
  }
  
  //create 3 subdirectories in SD1
  Sysfiles::changeDirectory("SD1");
  for (int i = 0; i < 3; i++){
    std::string dirNameBase = "SD1.";
    char* dirNameChar = Utils::stringToChar(dirNameBase + std::to_string(i));
    Sysfiles::createDirectory(dirNameChar);
  }
  Sysfiles::changeDirectory("..");


  
  
}

int main (int argc, char const **argv) {
  char* path = "/mnt/e";
  char* currentDirectory = ".";
  // Sysfiles::changeDirectory(path);
  // setupPendrive();

  // try {
  //   FILE* disk = fopen("\\\\.\\E:", "w+b");

  //   if(!disk)
  //     throw(std::runtime_error("/mnt/e" + errno));
      
  //   unsigned char buffer[512];
  //   fread(buffer, sizeof(unsigned char), 512, disk);
  //   unsigned char t = buffer[510];
  //   unsigned char t1 = buffer[511];
  //   printf("%2x%2x", t, t1);
  // } catch(const std::exception& e) {
  //       std::cerr << e.what() << " " << "erro" << '\n';
  // }

  //  seekg para ler 
  //  seekp para escrever
  try {
    std::fstream disk;
    disk.open("\\\\.\\E:", std::fstream::in | std::fstream::out | std::fstream::binary);


    if(!disk)
      throw(std::runtime_error("/mnt/e" + errno));

    char buffer[512];

    disk.read(&buffer[0], 512);
    printf("%02x %02x %d \n", (unsigned char)buffer[510], (unsigned char)buffer[511], *((unsigned short*)(&buffer[11])));

    // char* buffer1 = new char[512];

    // memset(buffer1, '0', 512);
    // buffer1[511] = '\0';
    // disk.seekp(0, std::ios_base::beg);
    // disk.write(buffer1, 512);

    // disk.seekg(0, std::ios_base::beg);
    // char buffer2[512];
    // disk.read(&buffer2[0], 512);
    // printf("%02x %02x\n", (unsigned char)buffer2[510], (unsigned char)buffer2[511]);

    // disk.seekp(0, std::ios_base::beg);
    // disk.write(buffer, 512);

  } catch(const std::exception& e) {
        std::cerr << e.what() << " " << "erro" << '\n';
  }
  return 0;
}
