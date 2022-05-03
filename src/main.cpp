#include "../include/sysfiles.h"

void deleteRandomFilesInDirectory(std::string baseFileName, int nFilesToDelete, int lastIndex) {
  int deletedFilesCount = 0;
  std::vector<int> deletedNumbers;

  while (deletedFilesCount != nFilesToDelete){
    int randomIndex = (rand() % (int)(lastIndex/2)) * 2;
    while(Utils::contains(deletedNumbers, randomIndex)){
      randomIndex = (rand() % (int)(lastIndex/2)) * 2;
    }
    Sysfiles::deleteFile((baseFileName + std::to_string(randomIndex) + ".txt").c_str());
    deletedNumbers.push_back(randomIndex);
    deletedFilesCount++;
  }
}

void setupPendrive() {
  std::string baseDirectoryName = "E:\\SD";
  //create 5 subdirectories with files
  for (int i = 0; i < 5; i++) {
    std::string dirName = baseDirectoryName + std::to_string(i);
    Sysfiles::createDirectory(dirName.c_str());
    for (int j = 0; j < 371; j++) {
      std::cout << "Diretorio -> " << dirName << std::endl;
      std::string fileName = dirName + "\\" +  std::to_string(j) + ".txt";
      if(DEBUG){
        std::cout << "Criou o arquivo -> " << fileName << std::endl;
      }
      Sysfiles::createSizedFile(fileName.c_str(), 1000);
    }
  }
  //create 3 subdirectories in SD1
  Sysfiles::createDirectory("E:\\SD1\\SD1.0");
  std::cout << "Criou o diretorio E:\\SD1\\SD1.0" << std::endl;
  Sysfiles::createDirectory("E:\\SD1\\SD1.1");
  std::cout << "Criou o diretorio E:\\SD1\\SD1.1" << std::endl;
  Sysfiles::createDirectory("E:\\SD1\\SD1.2");
  std::cout << "Criou o diretorio E:\\SD1\\SD1.2" << std::endl;
  int maxFiles = 100;
  Sysfiles::createXFilesInDir("E:\\", maxFiles, 512);
  std::cout << "Criou " << maxFiles << " arquivos no diretorio E:\\ com arquivos de 512 bytes" << std::endl << std::endl;
  Sysfiles::createXFilesInDir("E:\\SD1\\SD1.0\\", maxFiles, 4000);
  std::cout << "Criou " << maxFiles << " arquivos no diretorio E:\\SD1\\SD1.1\\ com arquivos de 8 Kb" << std::endl << std::endl;
  Sysfiles::createXFilesInDir("E:\\SD1\\SD1.1\\", maxFiles, 8000);
  std::cout << "Criou " << maxFiles << " arquivos no diretorio E:\\SD1\\SD1.1\\ com arquivos de 8 Kb" << std::endl << std::endl;
  Sysfiles::fillDirectory("E:\\SD1\\SD1.2\\");
  Sysfiles::fillDirectory("E:\\SD1\\SD1.2\\");
  std::cout << std::endl << "Encheu a pasta E:\\SD1\\SD1.2\\" << std::endl << std::endl;
  std::cout << std::endl << "Pendrive cheio" << std::endl << std::endl;
  deleteRandomFilesInDirectory("E:\\SD0\\", 50, 370); //arquivos de 512
  std::cout << "Deletou 50 arquivos da pasta E:\\SD0\\" << std::endl;
  deleteRandomFilesInDirectory("E:\\SD1\\SD1.0\\", 50, 100); //arquivos de 4KB
  std::cout << "Deletou 50 arquivos da pasta E:\\SD1\\SD1.0\\" << std::endl;
  deleteRandomFilesInDirectory("E:\\SD1\\SD1.1\\", 50, 100); //arquivos de 8KB
  std::cout << "Deletou 50 arquivos da pasta E:\\SD1\\SD1.1\\" << std::endl;
  for (int i = 0; i < 10; i++){
    // criar 10 arquivos com 40000
    if(i == 4){
      Sysfiles::createSizedFileWithContent((std::string("E:\\") + std::to_string(i + 100) + ".txt").c_str(), 40000, 'i');
    } else {
      Sysfiles::createSizedFile((std::string("E:\\") + std::to_string(i + 100) + ".txt").c_str(), 40000);
    }
  }
  std::cout << "Criou os 10 arquivos finais" << std::endl;
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
      std::cout << "Starting Area do cluster do arquivo " << path << " --> " << initialOffSet.startingClusterArea << std::endl;
      std::cout << "OffSet Inicial do conteudo do arquivo " << path << " --> " << initialOffSet.startingFileAddress << std::endl;
      std::cout << std::endl;
    }
  }  catch(const std::exception& e) {
    std::cout << "Arquivo inexistente" << std::endl;
    std::cerr << "Motivo: "<< e.what() << '\n\n';
  }
  
}

int main (int argc, char const **argv) {
  WCHAR* hardDrivePath = L"\\\\.\\E:";
  //  seekg para ler 
  //  seekp para escrever

  bool exit = false;
  try {
    Fat32 fat = Fat32(hardDrivePath);
    while(!exit){
      int number;
      std::cout << "--------------------------------------------" << std::endl;
      std::cout << "0 - para dar o setup no pendrive" << std::endl;
      std::cout << "1 - para encher pendrive" << std::endl;
      std::cout << "2 - para deletar os arquivos randomico" << std::endl;
      std::cout << "3 - print fat info" << std::endl;
      std::cout << "4 - para desdeletar um arquivo" << std::endl;
      std::cout << "5 - para achar o offset de um arquivo" << std::endl;
      std::cout << "6 - para sair" << std::endl;
      std::cout << std::endl;
      std::cout << "Escolha uma opcao: ";
      std::cin >> number;
      if(number == 0){
        setupPendrive();
      } else if (number == 1){
        char tmp[50];
        std::cout << "Digite o path do diretorio para ser enchido: ";
        std::cin >> tmp;
        Sysfiles::fillDirectory(tmp);
        Sysfiles::fillDirectory(tmp);
      } else if (number == 2){
        deleteRandomFilesInDirectory("E:\\SD0\\", 50, 370); //arquivos de 512
        deleteRandomFilesInDirectory("E:\\SD1\\SD1.0\\", 50, 100); //arquivos de 4KB
        deleteRandomFilesInDirectory("E:\\SD1\\SD1.1\\", 50, 100); //arquivos de 8KB
      } else if (number == 3){
        std::cout << std::endl;
        fat.printFatInfos();
        std::cout << std::endl;
      } else if (number == 4){
        char tmp[50];
        std::cout << "Digite o path do arquivo para ser desdeletado: ";
        std::cin >> tmp;
        fat.undeleteFile(tmp);
      } else if (number == 5){
        char tmp[50];
        std::cout << "Digite o path do arquivo: ";
        std::cin >> tmp;
        // findOffsetOfFile(fat, "pasta1/pasta2/pasta3/ola.txt");
        findOffsetOfFile(&fat, tmp);
      } else if (number == 6){
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
    
    // fat.printFatInfos();
    // char* filePath = "pasta/ola.txt";
    // int initialOffSet = fat.findArchiveOffset(Utils::parsePath(filePath), false);
    // std::cout << "OffSet Inicial do arquivo " << filePath << " --> " << initialOffSet << std::endl;

    // char* buffer = fat.readSector(0);
    // fat.writeSector(16782336, buffer);
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
