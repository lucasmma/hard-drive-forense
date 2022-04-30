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
  std::string command = std::string("rm -R ") + directoryName;
  system(command.c_str());
}