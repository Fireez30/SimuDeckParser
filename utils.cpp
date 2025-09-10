#include <string>
#include <vector>
#include <filesystem>
#include <iostream>
namespace fs = std::filesystem;

// Parse folders inside current folder
std::vector<std::string> ListFoldersInFolder(const std::string_view folder){
    std::vector<std::string> foundFolders;
    for (const auto & entry : fs::directory_iterator(folder))
        foundFolders.push_back(entry.path());
    return foundFolders;
}


//Determine pass separator using OS
char separator()
{
#if defined _WIN32 || defined __CYGWIN__
    return '\\';
#else
    return '/';
#endif
}

// split a path, and retrieve the last part of it, split by a separator

std::string GetLastName(const std::string phrase){
    std::string last_name;
    std::string s = std::string(phrase);
    char delimiter = separator();
    size_t pos = 0;
    std::string token;
     // this part is here to continue after finding the last separator, to get the real last term
    bool break_loop {false};
    size_t prev_pos = 0;
    while ((pos == std::string::npos && !break_loop) || (pos != std::string::npos))  {
        if (pos == std::string::npos){
            break_loop = true;
            token = s.substr(prev_pos, s.size());
            last_name = token;
        }
        else {

            token = s.substr(0, pos);
            last_name = token;
            s.erase(0, pos + sizeof(delimiter));
            pos = s.find(delimiter);
        }
    }
    return last_name;
}
