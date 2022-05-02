#include "../include/sysfiles.h"

void Sysfiles::printDirectory(const char* path){
  for (const auto & entry : std::filesystem::directory_iterator(path))
        std::cout << entry.path() << std::endl;
}

void Sysfiles::changeDirectory(const char* directoryName){
  std::filesystem::current_path(directoryName);
}

void Sysfiles::createDirectory(const char* directoryName){
  std::string directory_name(directoryName);
  std::filesystem::create_directory(directory_name);
}

void Sysfiles::createSizedFile(const char* fileName, int sizeBytes){
    FILE *fp = fopen(fileName, "w");
    fseek(fp, sizeBytes-1 , SEEK_SET);
    fputc('\0', fp);
    fclose(fp);
}

void Sysfiles::deleteFile(const char* directoryName){
  std::filesystem::remove_all(directoryName);
}

int Sysfiles::countFilesInDir(std::string directoryName) {
    auto dirIter = std::filesystem::directory_iterator(directoryName);
    int fileCount = 0;
    for (auto& entry : dirIter){
        if (entry.is_regular_file()){
            ++fileCount;
        }
    }
    return fileCount;
}

void Sysfiles::fillDirectory(const char* directoryName){
  BOOL  fResult;
  unsigned __int64 freeBytesToCaller, totalBytes, freeBytes;
  fResult = GetDiskFreeSpaceEx (L"E:",(PULARGE_INTEGER) &freeBytesToCaller, (PULARGE_INTEGER) &totalBytes, (PULARGE_INTEGER) &freeBytes);
  if (fResult) {
    printf ("Free space on drive  = %I64u MB\n", freeBytes / (1024*1024));
  }
  int countFiles = countFilesInDir(directoryName);
  int fileSize = 1048576; // 2**20 -> ~1MB;
  int fileSizeFilled = 0;
  while(fileSize > 512) {
    while(fileSizeFilled + fileSize <= freeBytes) {
        createSizedFile((std::string(directoryName) + "\\" + std::to_string(countFiles) + ".txt").c_str(), fileSize);
        fileSizeFilled += fileSize;
        countFiles++;
        std::cout << "Current Size Of File: " << fileSize << "B " << fileSizeFilled << "/" << freeBytes << " B" << std::endl;
    }
    fileSize/=2;
  }
}

void Sysfiles::createXFilesInDir(const char* dirName, int maxFiles, int fileSize){
  for (int i = 0; i < maxFiles; i++){
    createSizedFile((std::string(dirName) + std::to_string(i) + ".txt").c_str(), fileSize);
  }
}