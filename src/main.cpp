#include "../include/sysfiles.h"

int main (int argc, char const **argv) {
  char* path = "/mnt/e";
  Sysfiles::printDirectory(path);

  return 0;
}
