
#include "boost/asio/io_context.hpp"
#include "boost/asio/ip/tcp.hpp"
#include "boost/system/detail/error_code.hpp"
#include <bits/stdc++.h>
#include <boost/asio.hpp>
#include <boost/endian/arithmetic.hpp>
#include <exception>
#include <filesystem> // 1. Add the filesystem header
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using boost::asio::ip::tcp;
using std::filesystem::path;
namespace fs = std::filesystem;
using NetworkSize = boost::endian::big_uint64_t;
static std::vector<char> read_file(path const &fspec) {
  std::ifstream file(fspec, std::ios::binary);
  return std::vector<char>(std::istreambuf_iterator<char>(file), {});
}

void send_file_content_over_tcp(path const &fspec, tcp::socket &socket) {
  auto const contents = read_file(fspec);
  std::cout << "File read successfully. File size: " << contents.size()
            << " bytes." << std::endl;

  // Send the file content
  NetworkSize fileSize = contents.size();

  std::vector<boost::asio::const_buffer> payload{
      boost::asio::buffer(&fileSize, sizeof(fileSize)),
      boost::asio::buffer(contents),
  };
  boost::system::error_code ec;
  auto n = write(socket, payload, ec);

  std::cout << "Content bytes sent: " << n << " (" << ec.message() << ")\n";
}

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
int main() {
  try {
    boost::asio::io_context io;
    tcp::acceptor acceptor(io, tcp::endpoint(tcp::v4(), 12234));
    for (;;) {

      tcp::socket socket(io);
      acceptor.accept(socket);
      std::string place = "/home/";
      ChangeDirectory(place);
      std::string whatFile = "";
      std::cout << "GIVE A FILE: ";
      std::cin >> whatFile;
      std::cout << "THANKS";
      place += whatFile;
      std::cout << "WHERE IS: " << place << std::endl;
      send_file_content_over_tcp(place, socket);
    }
  } catch (std::exception &e) {
    std::cerr << e.what() << '\n';
  }

  return 0;
}
