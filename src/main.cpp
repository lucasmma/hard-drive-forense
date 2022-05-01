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

void fillDirectory(std::string dirname, int fileSize) {
    try {
        int fileNumber = 0;
        while(true) {
          const char* filename = Utils::stringToChar(dirname + std::to_string(fileNumber)+ ".txt" );
          Sysfiles::createSizedFile(filename, fileSize);
          fileNumber++;
          std::cout << "Creating File: " << dirname << fileNumber << ".txt" << std::endl;
        }
    } catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
    }
}


void setupPendrive() {
  std::string baseDirectoryName = "SD";
  //create 5 subdirectories with files
  for (int i = 0; i < 5; i++) {
    std::string dirName = baseDirectoryName + std::to_string(i);
    const char* dirNameChar = Utils::stringToChar(dirName);
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
    const char* dirNameChar = Utils::stringToChar(dirNameBase + std::to_string(i));
    Sysfiles::createDirectory(dirNameChar);
  }
  Sysfiles::changeDirectory("..");


  
  
}

void findOffsetOfFile(Fat32* fat, char* path, bool isDeleted = false){
  try {
    std::deque<std::string> pathParsed = Utils::parsePath(path);
    if(DEBUG){
      for(const std::string &i: pathParsed){
        printf("\033[7m%s\033[27m\n", i.c_str());
      }
    }
    struct FileInfo initialOffSet = fat->findArchiveOffset(pathParsed, isDeleted);
    if (initialOffSet.startingFileAddress == NOT_FOUND){
      std::cout << "Arquivo inexistente" << std::endl;
      std::cout << std::endl;
    } else {
      std::cout << "Starting Area do cluster do arquivo" << path << " --> " << initialOffSet.startingClusterArea << std::endl;
      std::cout << "OffSet Inicial do conteudo do arquivo " << path << " --> " << initialOffSet.startingFileAddress << std::endl;
      std::cout << std::endl;
    }
  }  catch(const std::exception& e) {
    std::cout << "Arquivo inexistente" << std::endl;
    std::cerr << "Motivo: "<< e.what() << '\n\n';
  }
  
}

int main (int argc, char const **argv) {
  char* hardDrivePath = "\\\\.\\E:";
  //  seekg para ler 
  //  seekp para escrever

  bool exit = false;
  try {
    Fat32 *fat = new Fat32(hardDrivePath);
    while(!exit){
      int number;
      std::cout << "--------------------------------------------" << std::endl;
      std::cout << "0 - para dar o setup no pendrive" << std::endl;
      std::cout << "1 - para encher pendrive" << std::endl;
      std::cout << "2 - print fat info" << std::endl;
      std::cout << "3 - para desdeletar um arquivo" << std::endl;
      std::cout << "4 - para achar o offset de um arquivo" << std::endl;
      std::cout << "5 - para sair" << std::endl;
      std::cout << std::endl;
      std::cout << "Escolha uma opcao: ";
      std::cin >> number;
      if(number == 0){
        setupPendrive();
        std::cout << "Pendrive cheio" << std::endl;  
      } else if (number == 1){
        fillDirectory("\\\\.\\E:\\pasta1\\", 1000000);
      } else if (number == 2){
        std::cout << std::endl;
        fat->printFatInfos();
        std::cout << std::endl;
      } else if (number == 3){
        char tmp[50];
        std::cout << "Digite o path do arquivo para ser desdeletado: ";
        std::cin >> tmp;
        fat->undeleteFile(tmp);
      } else if (number == 4){
        char tmp[50];
        // std::cout << "Digite o path do arquivo: ";
        // std::cin >> tmp;
        // findOffsetOfFile(fat, "pasta1/pasta2/pasta3/ola.txt");
        findOffsetOfFile(fat, "pasta1/66712.txt");
      } else if (number == 5){
        exit = true;
        std::cout << "Good Bye!" << std::endl; 
      } else{
        std::cout << std::endl << "Digite um número de 0 a 4" << std::endl;
      }
    }
    // Fat32 *fat = new Fat32(hardDrivePath);
    // std::deque<std::string> teste = Utils::parsePath("pasta/ola.txt");
    // for(const std::string &i: teste){
    //   // std::cout << i << std::endl;
    //   printf("\033[7m%s\033[27m\n", i.c_str());
    // }
    
    // fat->printFatInfos();
    // char* filePath = "pasta/ola.txt";
    // int initialOffSet = fat->findArchiveOffset(Utils::parsePath(filePath), false);
    // std::cout << "OffSet Inicial do arquivo " << filePath << " --> " << initialOffSet << std::endl;

    // char* buffer = fat->readSector(0);
    // fat->writeSector(16782336, buffer);
    // std::cout << 16782336 << std::endl;

    // *((unsigned short*)(&buffer[11]))
    // char* buffer1 = new char[512];

    // memset(buffer1, '0', 512);
    // buffer1[511] = '\0';
    // disk.seekp(0, std::ios_base::beg);
    // disk.write(buffer1, 512);

    // char buffer2[512];
    // disk.read(&buffer2[0], 512);
    // disk.seekg(0, std::ios_base::beg);

    // disk.seekp(0, std::ios_base::beg);
    // disk.write(buffer, 512);

  } catch(const std::exception& e) {
        perror("Invalid disk letter");
        std::cerr << "Disco nao existe: " << e.what() << '\n';
  }
  return 0;
}
