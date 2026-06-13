
#include "boost/asio/io_context.hpp"
#include "boost/asio/ip/tcp.hpp"
#include "boost/system/detail/error_code.hpp"
#include "filemanager.h"
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
