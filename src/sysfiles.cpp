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

void Sysfiles::createSizedFile(char* fileName, int sizeBytes){
    FILE *fp = fopen(fileName, "w");
    fseek(fp, sizeBytes-1 , SEEK_SET);
    fputc('\0', fp);
    fclose(fp);
}

void Sysfiles::deleteFile(char* fileName){

}