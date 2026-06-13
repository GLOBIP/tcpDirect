
#include "boost/asio/io_context.hpp"
#include "boost/asio/ip/tcp.hpp"
#include "boost/system/detail/error_code.hpp"
#include <boost/asio.hpp>
#include <boost/endian/arithmetic.hpp>
#include <exception>
#include <filesystem> // 1. Add the filesystem header
#include <fstream>
#include <iostream>
#include <string>
using boost::asio::ip::tcp;
using std::filesystem::path;
using NetworkSize = boost::endian::big_uint64_t;
namespace fs = std::filesystem;
static std::vector<char> read_file(path const &fspec) {
  std::ifstream file(fspec, std::ios::binary);
  return std::vector<char>(std::istreambuf_iterator<char>(file), {});
}

void ListDirectory(std::string path) {
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
void ChangeDirectory(std::string &path) {
  std::cout << "AVAILABLE COMMANDS: " << std::endl;
  std::cout << "cd .." << std::endl;
  std::cout << "cd ~" << std::endl;
  std::cout << "<PATH>" << std::endl;
  std::cout << "End file <PATH> with ';' for example: /home/user/file.txt;  "
            << std::endl;
  std::string command = "";
  while (command != ";" || command.back() != ';') {
    std::cin >> command;
    if (command.back() == ';') {
      path = command;
      path.pop_back();
      ListDirectory(path);
      break;
    }

    if (command != ";") {
      path = command;
    } else {
      break;
    }
    ListDirectory(path);
  }
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
      send_file_content_over_tcp(place, socket);
    }
  } catch (std::exception &e) {
    std::cerr << e.what() << '\n';
  }

  return 0;
}
