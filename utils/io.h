#ifndef IO_H
#define IO_H

#include <string>
#include <vector>
#include "algorithms.h"
#include <QFile>
#include <card.h>

std::map<std::string,std::string> get_settings();
std::string GetSetting(std::string setting_key);
void WriteSettings(std::map<std::string,std::string> setting_map);
void AddOrUpdateSetting(std::string setting_key,std::string setting_value);
void DeleteSetting(std::string setting_key);
void WriteTextToFile(std::string text, std::string file_path);
void WriteCardsToFile(std::vector<Card> cards,std::string file_path);
std::string GetLastName(const std::string phrase);
std::vector<std::string> ListFoldersInFolder(const std::string_view folder);
std::vector<std::string> ListFilesInFolder(const std::string_view folder,std::string extension="jpg");

#endif // IO_H
