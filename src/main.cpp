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
  
  std::ifstream disk("\\\\.\\E:", std::ios_base::binary);

  if(!disk)
    throw(std::runtime_error("/mnt/e" + errno));

  std::cout << disk.peek() << std::endl;
  return 0;
}
