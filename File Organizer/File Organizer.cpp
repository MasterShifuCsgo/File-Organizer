#include <iostream>
#include <string>
#include <filesystem> 
#include <unordered_map>

namespace fs = std::filesystem;

//used to determine what folder the file goes to depending on the extension
std::unordered_map<std::string, std::string> extensionToFolderName{
  {".txt", "TXT"},
  {".md", "MD"},
};

fs::path startFolder{ fs::current_path() / "Sorted"}; // Folder name where rest of the folders are created.

//creates a directory with given name in the given directory.
bool createDirectory(fs::path& _PATH);

//parses through the current directory.
void parseDirectory(std::vector<fs::path>& _seenDirectories);


int main()
{
  //Sets up the program.
  //holds seen directories for later traversal.
  std::vector<fs::path> seenDirectories{ fs::current_path()};
  std::cout << fs::current_path() << '\n';
  //populate seeDirectories with all the files in the 
  parseDirectory(seenDirectories);

  return 0;
}

//creates a new directory in specified directory when it dosent exist.
bool createDirectory(fs::path& _PATH) {
  return fs::is_directory(_PATH) ? false : fs::create_directory(_PATH);
}

void parseDirectory(std::vector<fs::path>& _seenDirectories) {

  fs::path currentPath; // current path that is being iterated.
  std::string appropriateFolder; // latest determined folder name where a file would go.
  while (!_seenDirectories.empty()){
    currentPath = _seenDirectories.back();
    _seenDirectories.pop_back();
    //parse through the current path
    for (const auto& Entity : fs::directory_iterator(currentPath))
    {
      //if regular file, add the file to a folder that has the same name as the extension, ".txt" file to folder, "TXT"
      if (fs::is_regular_file(Entity.path())) {

        //check if the key exists.
        auto it = extensionToFolderName.find(Entity.path().extension().string());
        // skip when the file extension was not found
        if (it == extensionToFolderName.end()) { std::cerr << "Unknown file, please check you are in the right directory\n"; }

        //get the appropriate folder name.
        appropriateFolder = it->second;

        //create folder if it dosent exist   
        fs::path targetPath = startFolder / appropriateFolder;
        if (createDirectory(targetPath)) {
          std::cerr << "Folder " << targetPath << " Creation failed.\n";
        }

        //move the file into the appropriate folder.
        fs::path destination = startFolder / appropriateFolder / Entity.path().filename();
        fs::rename(Entity.path(), destination);
      }

      // adds to seenDirectories if Entitiy is a directory
      if (fs::is_directory(Entity.path())) {
        _seenDirectories.push_back(Entity.path());
      }

      // ignores symlink (shortcut files).
    }

  }
  std::cout << "Part2! DONE\n";
 }