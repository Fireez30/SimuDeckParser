#include <string>
#include <vector>
#include <iostream>
#include "algorithms.h"
#include <fstream>
#include <QFile>
#include <filesystem>
#include <card.h>
#include "io.h"
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
namespace fs = std::filesystem;


std::string settings_file(){
    struct passwd *pw = getpwuid(getuid());
    const char *homedir = pw->pw_dir;
    std::string homedirstr = homedir;
    std::string simulator_config_folder = homedirstr+separator()+".config"+separator()+"weiss_simulator_parser"+separator();
    if (!fs::exists(simulator_config_folder)){
        fs::create_directories(simulator_config_folder);
    }
    std::string simulator_config_file = homedirstr+separator()+".config"+separator()+"weiss_simulator_parser"+separator()+"settings.conf";
    if (!fs::exists(simulator_config_file)){
        std::ofstream output(simulator_config_file);
    }

    return simulator_config_file;
}


std::map<std::string,std::string> get_settings(){
    std::string setting_path = settings_file();
    std::map<std::string,std::string> current_settings = {};
    if (fs::exists(setting_path)){
        std::ifstream file(setting_path);
        std::string str;
        bool parsing_set = false;
        std::string set_key;
        std::string set_val;
        std::string del = "'";

        //std::cout << "Serie : " << serie.getName() << std::endl;
        while (std::getline(file, str))
        {
            if (str != ""){
                std::vector<std::string> splited = split(trim(str),':');
                if (splited.size() > 1){
                    current_settings[splited[0]] = splited[1];
                }

            }
        }
    }
    return current_settings;

}

std::string GetSetting(std::string setting_key){
    std::map<std::string,std::string>::iterator it;
    std::map<std::string,std::string> current_settings = get_settings();
    for (it = current_settings.begin(); it != current_settings.end(); it++){
        if (trim((*it).first) == setting_key){
            return trim((*it).second);
        }
    }
    return "";
}

void WriteSettings(std::map<std::string,std::string> setting_map){
    std::string setting_path = settings_file();
    std::ofstream outputFile(setting_path);  // Open/create a file named "test.txt" for writing
    std::map<std::string, std::string>::iterator it;
    if (outputFile.is_open()) {  // Check if the file was successfully opened
        for (it = setting_map.begin(); it != setting_map.end(); it++)
        {
            outputFile << it->first+":"+it->second<< std::endl;  // Write a line of text to the file
        }
        // Write some text into the file

        // Close the file
        outputFile.close();  // Close the file after writing
    }
};

void AddOrUpdateSetting(std::string setting_key,std::string setting_value){

    std::map<std::string,std::string> current_settings = get_settings();
    current_settings[setting_key] = setting_value;
    WriteSettings(current_settings);

}

void DeleteSetting(std::string setting_key){
    std::map<std::string,std::string> current_settings = get_settings();
    std::map<std::string,std::string>::iterator it;
    it = current_settings.find (setting_key);             // by iterator (b), leaves acdefghi.
    current_settings.erase (it);
    WriteSettings(current_settings);
}


void WriteTextToFile(std::string text, std::string file_path){
    std::ofstream outputFile(file_path);  // Open/create a file named "test.txt" for writing

    if (outputFile.is_open()) {  // Check if the file was successfully opened
        // Write some text into the file
        outputFile << text;  // Write a line of text to the file
        // Close the file
        outputFile.close();  // Close the file after writing
    }
}
void WriteCardsToFile(std::vector<Card> cards,std::string file_path){
    std::string text;
    for (Card card : cards){
        text += " ------------------ ";
        text += card.getWholeCardText();
    }
    WriteTextToFile(text,file_path);
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

std::vector<std::string> ListFilesInFolder(const std::string_view folder,std::string extension){
    std::vector<std::string> foundFiles;
    for (const auto & entry : fs::directory_iterator(folder))
        if (!entry.is_directory() && endsWith(entry.path(),"."+extension)){
            foundFiles.push_back(entry.path());
        }
    return foundFiles;
}

