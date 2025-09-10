#ifndef UTILS_H
#define UTILS_H
#include<vector>
#include<string>

// Parse folders inside current folder
std::vector<std::string> ListFoldersInFolder(const std::string_view folder);

//Determine pass separator using OS
char separator();

// split a path, and retrieve the last part of it, split by a separator
std::string GetLastName(const std::string phrase);

#endif // UTILS_H
