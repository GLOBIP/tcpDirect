#pragma once
#include "boost/asio/detail/chrono.hpp"
#include "boost/asio/ip/tcp.hpp"
#include "boost/asio/steady_timer.hpp"
#include "boost/system/detail/error_code.hpp"
#include <bits/stdc++.h>
#include <boost/asio.hpp>
#include <exception>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

namespace fs = std::filesystem;

using boost::asio::ip::tcp;
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
void sendDirectoryCLIENT(tcp::socket &mysocket,
                         const std::filesystem::directory_entry &directory) {
  try {
    boost::system::error_code ignored_error;
    std::string path = directory.path().string() + '\n';
    boost::asio::write(mysocket, boost::asio::buffer(path), ignored_error);
  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
}
void ListDirectories(boost::asio::io_context &io, tcp::socket &mysocket,
                     std::string s) {
  for (auto &p : std::filesystem::directory_iterator(s)) {
    if (p.is_directory()) {
      std::cout << p << std::endl;
      sendDirectoryCLIENT(mysocket, p);
    }
  }

  boost::system::error_code ignored_error;
  std::string end = "|";
  boost::asio::steady_timer t(io, boost::asio::chrono::seconds(3));
  t.wait();
  boost::asio::write(mysocket, boost::asio::buffer(end), ignored_error);
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
void sendPathToClient(std::string &path, tcp::socket &mysocket) {
  std::cout << "Wyslij Path";
  path += '~';
  try {
    boost::system::error_code ignored_error;
    boost::asio::write(mysocket, boost::asio::buffer(path), ignored_error);
  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
  path.pop_back();
}
void ChangeDirectory(boost::asio::io_context &io, std::string &path,
                     tcp::socket &mysocket) {
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
    ListDirectories(io, mysocket, path);
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
    sendPathToClient(path, mysocket);
  }
}
