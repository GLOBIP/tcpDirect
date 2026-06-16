#pragma once
#include "boost/asio/detail/chrono.hpp"
#include "boost/asio/ip/tcp.hpp"
#include "boost/asio/steady_timer.hpp"
#include "boost/asio/streambuf.hpp"
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

// SERWER - |
// CLIENT - ~
void sendDirectoryCLIENT(tcp::socket &mysocket,
                         const std::string &sendDirectoriesString);
void ListFilesInDirectory(const std::string &path, tcp::socket &mysocket);
void ListDirectories(boost::asio::io_context &io, tcp::socket &mysocket,
                     const std::string &s);
void sendPathToClient(const std::string &path, tcp::socket &mysocket);
std::string getFromClientPath(tcp::socket &mysocket,
                              boost::asio::streambuf &sb);
std::string getFromClientFile(tcp::socket &mysocket,
                              boost::asio::streambuf &sb);
void doSendDirectories(tcp::socket &mysocket, boost::asio::streambuf &sb);
std::string ChangeDirectory(boost::asio::io_context &io, std::string &path,
                            tcp::socket &mysocket);
