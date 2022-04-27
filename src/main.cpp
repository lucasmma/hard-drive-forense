#include "../include/sysfiles.h"

int main (int argc, char const **argv) {
  char* path = "/mnt/e";
  Sysfiles::printDirectory(path);
  Sysfiles::createFile("/mnt/e/oi.txt", 8);
  return 0;
}
