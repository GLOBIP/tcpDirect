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
std::string getFromClientPath(tcp::socket &mysocket,
                              boost::asio::streambuf &sb) {

  boost::system::error_code ec;

  std::string buf = "";
  size_t transferred = boost::asio::read_until(mysocket, sb, '|', ec);
  buf.resize(transferred);
  sb.sgetn(&buf[0], buf.size());
  std::cout << "RECEVIED DATA IS: " << buf << std::endl;
  return buf;
}
std::string getFromClientFile(tcp::socket &mysocket,
                              boost::asio::streambuf &sb) {
  boost::system::error_code ec;

  size_t transferred = boost::asio::read_until(mysocket, sb, '|', ec);
  std::string buf;
  buf.resize(transferred);
  sb.sgetn(&buf[0], buf.size());
  buf.pop_back();
  std::cout << buf << std::endl;
  return buf;
}
