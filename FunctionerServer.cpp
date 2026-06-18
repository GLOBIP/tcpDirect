#include "boost/endian/buffers.hpp"
#include "filemanager.h"

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

std::string ChangeDirectory(boost::asio::io_context &io, std::string &path,tcp::socket &mysocket) {

	std::cout << "SERWER URUCHOMIONY W TRYBIE NASŁUCHIWANIA KLIENTA" << std::endl;
	// DIRECTORY SEARCH MODE

	while (true) {
	boost::asio::streambuf sb;
	//associate streambuf input and output sequences with one or more character arrays (strings)
	// access to them is provided to use with I/O operations
	//
	// we use sequence of more character arrays various sizes .
	// ADDITIONAL STRINGS ARE APPENDED TO INCLUDE CHANGES IN SIZE OF CHARACTER SIZE 
	std::cout << "WYSYŁAMY : " << path << std::endl;
	sendPathToClient(path, mysocket); // send path 
	std::cout << "CHECKER " << std::endl;
	doSendDirectories(mysocket, sb);
	// POCZEKAJ CHWILE Z LISTDIRECTORIES
	// path.push_back('/');
	std::cout << "MOJE DIRECTORY TO " << path << std::endl;
	ListDirectories(io, mysocket, path);	  // see directories AND SEND IT TO CLIENT
	std::cout << "\n[SERWER] Aktualna sciezka wyslana: " << path << std::endl;
	std::cout << "[SERWER] Czekam na decyzje klienta..." << std::endl;
	std::string receviedPath = "";
	receviedPath =getFromClientPath(mysocket,sb); // WAITS FOR INPUT // WHY RECEVIED BAD ?????
	std::string normal;
	std::copy_if(receviedPath.begin(), receviedPath.end(),std::back_inserter(normal),[](unsigned char c) { return std::isprint(c); });

	receviedPath = normal;
	receviedPath.pop_back();
	std::cout << "OUR NEW PATH IS:	" << receviedPath << std::endl;
	if (receviedPath.find("EXIT") != std::string::npos &&
		receviedPath.find('/') == std::string::npos) {
		std::cout << "wychodzimy z petli";
		break;
	}
	if (receviedPath.find("GIVE_FILES") != std::string::npos &&
		receviedPath.find('/') == std::string::npos) {
		std::cout << "WYŚWIOETL PLIK " << std::endl;
		ListFilesInDirectory(path, mysocket); // LIST FILES AND SEND TO CLIENT
		doreadFiles(mysocket, sb); 
	}
	if(receviedPath.find('/') != std::string::npos)
	path = receviedPath;

	std::cout << "[SERWER] Klient zmienil sciezke na: " << path << std::endl;
	std::cout << "---------- ----------" << std::endl;
  }

	boost::asio::streambuf sb;
  // SERWER FILE PICK MODE
	ListFilesInDirectory(path, mysocket);
  	std::string whatFile = getFromClientFile(mysocket, sb);
  	if (path.back() != '/') {
		path.push_back('/');
  	}
  	path += whatFile;
  	return path;
}
