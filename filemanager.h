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

void sendFileClient(tcp::socket &mysocket, std::string file) {
  try {
    boost::system::error_code ignored_error;
    file.push_back('\n');
    boost::asio::write(mysocket, boost::asio::buffer(file), ignored_error);
  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
}

void ListFilesInDirectory(std::string path, tcp::socket &mysocket) {
  std::cout << "CURRENT DIRECTORY IS: \t" << path << '\n';
  const fs::path target_path{path};

  try {
    for (auto const &dir_entry : fs::directory_iterator{target_path}) {
      if (fs::is_regular_file(dir_entry.path())) {
        std::cout << dir_entry.path().filename().string() << std::endl;
        std::string fileNameWithNewline =
            dir_entry.path().filename().string() + '\n';
        sendFileClient(mysocket, fileNameWithNewline);
      }
    }
  } catch (fs::filesystem_error const &ex) {
    std::cout << "Error occurred during file operations!\n"
              << ex.what() << std::endl;
    return;
  }
  boost::system::error_code ignored_error;
  boost::asio::write(mysocket, boost::asio::buffer("|"), ignored_error);
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
  std::cout << std::endl;
  for (auto &p : std::filesystem::directory_iterator(s)) {
    if (p.is_directory()) {
      std::cout << p << std::endl;
      sendDirectoryCLIENT(mysocket, p);
    }
  }

  boost::system::error_code ignored_error;
  std::string end = "|";

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

void getFromClientPath(std::string &path, tcp::socket &mysocket) {

  boost::system::error_code ec;
  boost::asio::streambuf sb;
  size_t transferred = boost::asio::read_until(mysocket, sb, '`', ec);
  std::string buf;
  buf.resize(transferred);
  sb.sgetn(&buf[0], buf.size());
  buf.pop_back();
  std::cout << buf << std::endl;
  path = buf;
}

std::string getFromClientFile(tcp::socket &mysocket) {
  boost::system::error_code ec;
  boost::asio::streambuf sb;
  size_t transferred = boost::asio::read_until(mysocket, sb, '`', ec);
  std::string buf;
  buf.resize(transferred);
  sb.sgetn(&buf[0], buf.size());
  buf.pop_back();
  std::cout << buf << std::endl;
  return buf;
}
void ChangeDirectory(boost::asio::io_context &io, std::string &path,
                     tcp::socket &mysocket) {

  std::cout << "SERWER URUCHOMIONY W TRYBIE NASŁUCHIWANIA KLIENTA" << std::endl;
  // DIRECTORY SEARCH MODE
  bool showDirectories = true;
  while (true) {

    sendPathToClient(path, mysocket); // send path
    if (showDirectories)
      ListDirectories(io, mysocket,
                      path); // see directories AND SEND IT TO CLIENT
    else
      showDirectories = true;
    std::cout << "\n[SERWER] Aktualna sciezka wyslana: " << path << std::endl;

    std::cout << "[SERWER] Czekam na decyzje klienta..." << std::endl;
    std::string receviedPath = "";
    getFromClientPath(receviedPath, mysocket);
    if (receviedPath == "EXIT") {
      std::cout << "wychodzimy z petli";
      break;
    }
    if (receviedPath == "GIVE_FILES") {
      std::cout << "WYŚWIOETL PLIK " << std::endl;
      ListFilesInDirectory(path, mysocket); // LIST FILES AND SEND TO CLIENT
      showDirectories = false;
      receviedPath = path;
    } else {
      path = receviedPath;
    }
    std::cout << "[SERWER] Klient zmienil sciezke na: " << path << std::endl;
    std::cout << "---------- ----------" << std::endl;
  }
  // SERWER FILE PICK MODE
  ListFilesInDirectory(path, mysocket);
  std::string whatFile = getFromClientFile(mysocket);
  if (path.back() != '/') {
    path.push_back('/');
  }
  path += whatFile;
}
