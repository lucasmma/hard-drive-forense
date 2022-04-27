#include <string>
#include <iostream>
#include <dirent.h>

void printDirectory (char* path){
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

int main (int argc, char const **argv) {
  char* path = "/mnt/e";
  printDirectory(path);
  return 0;
}
