#pragma once
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#include <bits/stdc++.h>

namespace fs = std::filesystem;

void ListFilesInDirectory(std::string path) {
  std::cout << "CURRENT DIRECTORY IS: \t" << path << '\n';
  const fs::path target_path{path};

  try {
    for (auto const &dir_entry : fs::directory_iterator{target_path}) {
      if (fs::is_regular_file(dir_entry.path())) {
        std::cout << dir_entry.path().filename().string() << std::endl;
      }
    }
  } catch (fs::filesystem_error const &ex) {
    std::cout << "Error occurred during file operations!\n"
              << ex.what() << std::endl;
    return;
  }
}

void ListDirectories(std::string s) {
  for (auto &p : std::filesystem::directory_iterator(s))
    if (p.is_directory())
      std::cout << p << std::endl;
}

std::vector<std::string> goFunc(std::string str) {
  using namespace std;
  string s;
  stringstream ss(str);
  vector<string> v;
  while (getline(ss, s, ' ')) {
    v.push_back(s);
  }
  for (int i = 0; i < v.size(); i++) {
    cout << v[i] << endl;
  }
  return v;
}

void ChangeDirectory(std::string &path) {
  std::cout << "AVAILABLE COMMANDS: " << std::endl;

  std::cout << "<PATH>" << std::endl;
  std::cout << "End file <PATH> with ';' for example: /home/user/file.txt;  "
            << std::endl;

  std::string command = "";

  while (command != ";" || command.back() != ';') {
    std::cout << "----------        ----------";
    std::cout << "\nOUR DIRECTORY CURRENTLY IS: \t" << path << '\n';
    std::cout << "----------        ----------";
    std::cout << "DIRECTORIES ARE: \n";
    ListDirectories(path);
    std::cout << '\n';

    // OUR COMMAND TO USE AND CHECK GODDAMNIT
    std::getline(std::cin, command);
    if (command == "..") {
      std::string pathBackup = path;
      for (int i = path.size() - 1; i >= 0 && path[i] != '/'; i--) {
        pathBackup.pop_back();
      }
      if (!pathBackup.empty()) {
        pathBackup.pop_back();
        path = pathBackup;
      }
    } else if (command.find("go") != std::string::npos) {
      std::vector<std::string> goVec = goFunc(command);
      if (path.back() != '/') {
        path.push_back('/');
      }
      path += goVec[1];
    }

    else if (command.back() == ';') {
      ListFilesInDirectory(path);
      path.push_back('/');
      break;
    } else if (command.find("go") == std::string::npos &&
               command.find("do") == std::string::npos && command != "..") {
      std::cout << "ACCEPT THE FATE";
      path = command;
    }
  }
}
