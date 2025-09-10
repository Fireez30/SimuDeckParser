#include <string>
#include <vector>
#include <filesystem>
namespace fs = std::filesystem;

bool endsWith(const std::string& s, const std::string& suffix)
{
    return s.rfind(suffix) == (s.size() - suffix.size());
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
            // if we reached the end, only take the last part with the previous stored position
            break_loop = true;
            token = s.substr(prev_pos, s.size());
            last_name = token;
        }
        else {
            // If we're in the middle of the path, use current position
            token = s.substr(0, pos);
            last_name = token;
            s.erase(0, pos + sizeof(delimiter));
            pos = s.find(delimiter);
        }
    }
    return last_name;
}


// Parse folders inside current folder
std::vector<std::string> ListFoldersInFolder(const std::string_view folder){
    std::vector<std::string> foundFolders;
    for (const auto & entry : fs::directory_iterator(folder))
        if (entry.is_directory()){
            std::string name_dic = GetLastName(entry.path());
            if (name_dic.substr(0,1) != "."){
                foundFolders.push_back(entry.path());
            }
        }
    return foundFolders;
}

std::vector<std::string> ListFilesInFolder(const std::string_view folder,std::string extension="jpg"){
    std::vector<std::string> foundFiles;
    for (const auto & entry : fs::directory_iterator(folder))
        if (!entry.is_directory() && endsWith(entry.path(),"."+extension)){
            foundFiles.push_back(entry.path());
        }
    return foundFiles;
}


