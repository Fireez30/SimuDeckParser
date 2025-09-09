#ifndef UTILS_H
#define UTILS_H
#include<vector>
#include<string>

std::vector<std::string> ListFoldersInFolder(const std::string_view folder);
char separator();
std::string getLastName(const std::string phrase);

#endif // UTILS_H
