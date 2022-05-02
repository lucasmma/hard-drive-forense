#include "../include/utils.h"

bool Utils::contains(std::vector<int> c, int e) { 
    return std::find(c.begin(), c.end(), e) != c.end();
}

int Utils::calculateIntfromHighLow(int high, int low) {
  std::cout << "high: " << high << std::endl;
  std::cout << "low: " << low << std::endl;
  // std::cout << "low: " << low << std::endl;
    return (high * 65536) + low;
}

std::deque<std::string> Utils::parsePath(char* pathFilename){
  std::deque<std::string> pathParsed;
  std::filesystem::path path(pathFilename);
  int x = 0;
  int j = 0;
  char dirName[9];
  while (path.parent_path().string().c_str()[x] != '\0'){
    dirName[j] = path.parent_path().string().c_str()[x];
    if(path.parent_path().string().c_str()[x+1] == '/' || path.parent_path().string().c_str()[x+1] == '\0'){
      j++;
      while(j != 8){
        dirName[j] = ' ';
        j++;
      }
      dirName[j] = 0;
      char upper[512];
      Utils::toUpper(dirName, upper);
      pathParsed.emplace_back(upper);
      memset(dirName, 0, 9);
      j = -1;
      if(!(path.parent_path().string().c_str()[x+1] == '\0')){
        x++;
      }
    }
    x++;
    j++;
  }
  char buffer[12];
  snprintf(buffer, 12, "%-8s%-3s", path.stem().string().c_str(), path.extension().string().substr(1,3).c_str());
  char upper[512];
  Utils::toUpper(buffer, upper);
  pathParsed.emplace_back(upper);

  return pathParsed;
}

FileInfo Utils::parseFileBytes(Fat32* fat, char* cluster, int offSetCluster, bool isFile){
  static char bufferName[12];
  char bitFieldAttribute = *((unsigned char*)(&cluster[offSetCluster + 11]));
  // concatenar com o starting cluster area high
  int highByteStartCluster = *((unsigned short*)(&cluster[offSetCluster + 20]));
  int lowByteStartCluster = *((unsigned short*)(&cluster[offSetCluster + 26]));
  int startingClusterArea = Utils::calculateIntfromHighLow(highByteStartCluster, lowByteStartCluster);
  int fileSize = *((unsigned int*)(&cluster[offSetCluster + 28]));
  int startingFileAddress = ((startingClusterArea - 2) * fat->bytesPerCluster) + fat->offSetRootDirectory;
  if(!isFile){
    memcpy(bufferName, &cluster[offSetCluster], 8);
    bufferName[8] = 0;
  } else {
    memcpy(bufferName, &cluster[offSetCluster], 11);
    bufferName[11] = 0;
  }
  struct FileInfo fileInfo;
  fileInfo.filename = bufferName;
  fileInfo.startingClusterArea = startingClusterArea;
  fileInfo.startingFileAddress = startingFileAddress;
  fileInfo.fileSize = fileSize;
  fileInfo.bitFieldAttribute = bitFieldAttribute;
  return fileInfo;
}

char* Utils::toUpper(char* text, char* upper){
  int i = 0;
  while(text[i] != '\0'){
    upper[i] = toupper(text[i]);
    i++;
  }
  upper[i] = 0;
  return upper;
}