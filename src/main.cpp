/**
 * @file main.cpp
 * @brief Entry point for the File Integrity Monitor application.
 *
 * This application monitors file integrity and reports any unauthorized changes
 * using hashing and file system checks.
 *
 * @author      Obed G.
 * @date        July 23, 2025
 * @version     0.1
 * @license     MIT License
 */

#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include <cstdio>
#include <map>
#include <vector>
#include <sstream>
#include "hasher.hpp"

namespace fs = std::filesystem;

std::string calculateSha256(const std::vector<char>& buffer)
{
    return "ebc234234f324d32c323b323";
}

std::vector<char> readFileIntoBuffer(const fs::path &filename)
{
    std::ifstream file(filename, std::ios::binary | std::ios::ate);

    if (!file.is_open())
    {
        std::cerr << "Error: could not open file " << filename << std::endl;
        return {};
    }
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<char> buffer(size);
    file.read(buffer.data(), size);

    return buffer;
}

std::string monitor_file_integrity(const std::string path_str)
{
    try
    {
        // the file integrity monitoring will go here.
    } catch ( ... )
    {
        std::cerr << "An error occurred." << std::endl;
    }
}

std::pair<std::string, std::string> split_string(const std::string &text_line, const std::string &delimiter)
{
    std::string file_path, file_hash;
    std::pair<std::string, std::string> file_pair;

    size_t start = 0;
    size_t end = text_line.find(delimiter);

    while(end != std::string::npos){
        file_path = text_line.substr(start, end - start);

        start = end + delimiter.length();
        end = text_line.find(delimiter, start);
    }
    file_hash = text_line.substr(start, end - start);
    file_pair.first = file_path;
    file_pair.second = file_hash;

    return file_pair;
}

int main(int argc, char *argv[])
{
    // Command line parsing
    if (argc != 3)
    {
        std::cout << "Cmd Parameter Usage: " << std::endl;
        std::cout << "[Usage] --new   :   Create a new baseline" << std::endl;
        std::cout << "[Usage] --mon  :   Monitor using available baseline" << std::endl;
        std::cout << "Example: ./fim --new baseline.txt" << std::endl;
        std::cout << "         ./fim --mon baseline.txt" << std::endl;

        return 1;
    }

    std::string command = argv[1];
    std::string baseline_file = argv[2];
    std::string path_str = ".";
    fs::path dir_path(path_str);
    Hasher hasher;

    if (command == "--new")
    {
        // check if baseline file exists
        if (fs::exists(baseline_file))
        {
            if (remove(baseline_file.c_str()) == 0)
                std::cout << "Baseline file deleted!" << std::endl;
        }
        else
        {
            std::cerr << "Error: File couldn't delete!" << std::endl;
        }
        std::ofstream baseline_text(baseline_file, std::ios::app);
        if (baseline_text.is_open())
        {
            std::cout << "New baseline.txt created successfully" << std::endl;
            if (fs::exists(dir_path) && fs::is_directory(dir_path))
            {
                std::cout << "Hashing the files in directory: " << dir_path << std::endl;
                for (const auto &entry : fs::directory_iterator(dir_path))
                {
                    fs::path current_entry_path = entry.path();
                    if (fs::is_regular_file(current_entry_path))
                    { 
                        std::vector<char> fileContent = readFileIntoBuffer(current_entry_path);
                        
                        // std::string file_hash = calculateSha256(fileContent);
                        const char* file_content = fileContent.data();
                        std::vector<BYTE> file_hash;
                        
                        int result = hasher.createHash(file_content, file_hash);

                        std::ostringstream oss;
                        for (BYTE b : file_hash) {
                            oss << std::hex <<std::setw(2) << std::setfill('0') << (int)b;
                        }
                        std::string out = oss.str();
                        
                        baseline_text << current_entry_path.filename() << " | " << out << std::endl;
                    }
                }
            }
            else
            {
                std::cerr << "Error: Directory not found or not a directory." << std::endl;
            }
            baseline_text.close();
        }
        else
        {
            std::cerr << "Error file couldn't open" << std::endl;
            return 1;
        }
    }
    else if (command == "--mon")
    {
        // read baseline.txt
        std::ifstream baseline_text;
        baseline_text.open(baseline_file);

        if (!baseline_text.is_open())
        {
            std::cerr << "Error opening file!" << std::endl;
            return 1;
        }

        std::string line;
        std::map<std::string, std::string> filehash_dict;
        while (std::getline(baseline_text, line))
        {
            filehash_dict.insert(split_string(line, "|"));
        }
        for (const auto &pair : filehash_dict)
        {
            std::cout << pair.first << " : " << pair.second << std::endl;
        }
    }
    else
    {
        std::cerr << "Error: Incorrect command argument provided!" << std::endl;
    }

    return 0;
}
