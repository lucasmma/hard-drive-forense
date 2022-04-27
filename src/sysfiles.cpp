#include "../include/sysfiles.h"

void Sysfiles::printDirectory(char* path){
  if (auto dir = opendir(path)) {
    while (auto f = readdir(dir)) {
        if (!f->d_name || f->d_name[0] == '.')
          continue; // Skip everything that starts with a dot
        
        if (f->d_type == DT_DIR ){
          printf("Directory: %s\n", f->d_name);
          /* code */
        } else{
          printf("File: %s\n", f->d_name);
        }
        
    }
    closedir(dir);
  }
}

void Sysfiles::createDirectory(char* directoryName){

}

void Sysfiles::createFile(char* fileName){

}

void Sysfiles::deleteFile(char* fileName){

}