#include "boost/asio/detail/chrono.hpp"
#include "boost/asio/ip/tcp.hpp"
#include "boost/asio/steady_timer.hpp"
#include "boost/asio/streambuf.hpp"
#include "boost/system/detail/error_code.hpp"
#include "filemanager.h"
#include <bits/stdc++.h>
#include <boost/asio.hpp>
#include <exception>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
namespace fs = std::filesystem;

using boost::asio::ip::tcp;

void doSendDirectories(tcp::socket &mysocket, boost::asio::streambuf &sb) {

  boost::system::error_code ec;
  std::string buf = "";
  int counter = 0;
  while (true) {
    buf = "";
    size_t transferred = boost::asio::read_until(mysocket, sb, '~', ec);
    buf.resize(transferred);
    sb.sgetn(&buf[0], buf.size());
    std::cout << std::endl << buf << std::endl;
    if (buf == "SEND_DIRECTORIES~" || buf == "SEND_DIRECTORIES") {
      std::cout << "VALID" << std::endl;
      return;
    }
  }
}
void sendPathToClient(const std::string &path, tcp::socket &mysocket) {
  std::string path2Send = path;
  path2Send.insert(0, "PATH");
  path2Send.push_back('~');
  try {
    boost::system::error_code ignored_error;
    boost::asio::write(mysocket, boost::asio::buffer(path2Send), ignored_error);
  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
}
void ListDirectories(boost::asio::io_context &io, tcp::socket &mysocket,
                     const std::string &s) {
  std::cout << "PRZEKAZALISMY MU " << s << std::endl;
  const char *c = s.c_str();
  std::cout << "JAKO CONST CHAR TO " << c << std::endl;
  std::string sendDirectoriesString = "";
  int count = 0;
  // TUTAJ JEST ROZWALONA FUNKCJA
  for (auto &p : std::filesystem::directory_iterator(c)) {
    if (p.is_directory()) {
      count++;
      std::cout << p << std::endl;
      sendDirectoriesString += p.path().string();
      sendDirectoriesString.push_back('\n');
    }
  }
  // JEST ZEPSUTA

  sendDirectoriesString.push_back('~');
  if (sendDirectoriesString.size() <= 1) {
    sendDirectoriesString.push_back('~');
  }
  std::cout << "-----------------";
  std::cout << "DANO CLIENTOWI : " << sendDirectoriesString << std::endl;
  std::cout << "-----------------";
  sendDirectoryCLIENT(mysocket, sendDirectoriesString);
}

void sendDirectoryCLIENT(tcp::socket &mysocket,
                         const std::string &sendDirectoriesString) {
  try {
    boost::system::error_code ignored_error;
    boost::asio::write(mysocket, boost::asio::buffer(sendDirectoriesString),
                       ignored_error);
  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
}
void ListFilesInDirectory(const std::string &path, tcp::socket &mysocket) {
  std::cout << "CURRENT DIRECTORY IS: \t" << path << '\n';
  const fs::path target_path{path};
  std::string sendDirectoriesString = "";
  int count = 0;
  try {
    for (auto const &dir_entry : fs::directory_iterator{target_path}) {
      if (fs::is_regular_file(dir_entry.path())) {
        std::cout << dir_entry.path().filename().string() << std::endl;
        std::string fileNameWithNewline =
            dir_entry.path().filename().string() + '\n';
        sendDirectoriesString += fileNameWithNewline;
        count++;
      }
    }
  } catch (fs::filesystem_error const &ex) {
    std::cout << "Error occurred during file operations!\n"
              << ex.what() << std::endl;
    return;
  }

  sendDirectoriesString.push_back('~');

  std::cout << "-----------------";
  std::cout << "DANO CLIENTOWI PLIK DO : " << sendDirectoriesString
            << std::endl;
  std::cout << "-----------------";

  sendDirectoryCLIENT(mysocket, sendDirectoriesString);
}
