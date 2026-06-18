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

using	boost::asio::ip::tcp;
std::string	getFromClientPath(tcp::socket &mysocket,boost::asio::streambuf &sb) {
    boost::system::error_code ec;
    std::string buf = "";
    size_t transferred = boost::asio::read_until(mysocket, sb, '|', ec);
    buf.resize(transferred);
    sb.sgetn(&buf[0], buf.size());
    std::cout << "RECEVIED DATA IS: " << buf << std::endl;
     return buf;
}
std::string	getFromClientFile(tcp::socket &mysocket,  boost::asio::streambuf &sb) {
    boost::system::error_code ec;
    size_t transferred = boost::asio::read_until(mysocket, sb, '|', ec);
    std::string buf;
    buf.resize(transferred);
    sb.sgetn(&buf[0], buf.size());
    buf.pop_back();
    std::cout << buf << std::endl;
    return buf;
}

void doreadFiles(tcp::socket &mysocket, boost::asio::streambuf &sb) {

	boost::system::error_code ec;
	std::string buf = "";
	int counter = 0;
	while (true) {
		buf = "";
		//read data to dynamic buffer or STREAMBUFFER until delmiter matches char or object indicates match
		size_t transferred = boost::asio::read_until(mysocket, sb, '~', ec);

		//resize length of string to a new length of read DATA 
		buf.resize(transferred);

		//std::streambuf::sgetn 
		//read chars from cotntrolled input sequence stroe them iun arrays pointed by type char s [first]
		//until or type streamsize n [second] characters have been extracted or end is reached 
		//
		//s array where char sequence is copied 
		//n max number of chars 

		sb.sgetn(&buf[0], buf.size());
		std::cout << std::endl << buf << std::endl;
		if (buf.find("SENDED_FILES~") != std::string::npos  || buf.find("SENDED_FILES") != std::string::npos) {
			std::cout << "VALID" << std::endl;
			return;
		}
	}
}
