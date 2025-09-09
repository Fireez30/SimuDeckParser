#include <string>
#include <vector>
#include <filesystem>
namespace fs = std::filesystem;

// Will list all folders inside any folder given in parameter
// return the list of relative paths to those folders
std::vector<std::string> ListFoldersInFolder(const std::string_view folder){
    std::vector<std::string> foundFolders;
    for (const auto & entry : fs::directory_iterator(folder))
        foundFolders.push_back(entry.path());
    return foundFolders;
}

char separator()
{
#if defined _WIN32 || defined __CYGWIN__
    return '\\';
#else
    return '/';
#endif
}

std::string getLastName(const std::string phrase){
    std::string last_name;
    std::string s = std::string(phrase);
    char delimiter = separator();
    size_t pos = 0;
    std::string token;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        last_name = token;
        s.erase(0, pos + sizeof(delimiter));
    }
    return last_name;
}
