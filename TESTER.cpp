#include <iostream>
#include <fstream>
#include <filesystem>
#include <random>

namespace fs = std::filesystem;

// **Base directory for random folders**
fs::path baseTestDir = fs::current_path() / "RandomTestEnvironment";

// **Random number generator setup**
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<int> folderDist(3, 6);  // 3 to 6 folders
std::uniform_int_distribution<int> fileDist(2, 5);    // 2 to 5 files per folder
std::vector<std::string> fileExtensions = { ".txt", ".md", ".log" };

// **Function to create folders and files randomly**
void createRandomFoldersAndFiles() {
  // Remove existing test environment
  if (fs::exists(baseTestDir)) {
    fs::remove_all(baseTestDir);
  }
  fs::create_directory(baseTestDir);

  int numFolders = folderDist(gen);
  std::cout << "[INFO] Creating " << numFolders << " folders in: " << baseTestDir << "\n";

  for (int i = 1; i <= numFolders; ++i) {
    fs::path folderPath = baseTestDir / ("Folder_" + std::to_string(i));
    fs::create_directory(folderPath);
    std::cout << "  - Created folder: " << folderPath << "\n";

    int numFiles = fileDist(gen);
    for (int j = 1; j <= numFiles; ++j) {
      std::uniform_int_distribution<int> extDist(0, fileExtensions.size() - 1);
      std::string fileName = "File_" + std::to_string(j) + fileExtensions[extDist(gen)];
      fs::path filePath = folderPath / fileName;

      std::ofstream(filePath).close();
      std::cout << "    - Created file: " << filePath << "\n";
    }
  }
}

int main() {
  createRandomFoldersAndFiles();
  std::cout << "[INFO] Random folder and file creation complete.\n";
  return 0;
}
