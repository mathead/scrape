//
// Created by Ja on 4/27/2015.
//

#ifndef SEMESTRALKA_DOWNLOADER_H
#define SEMESTRALKA_DOWNLOADER_H

#include "Response.h"
#include <string>

class Downloader {
    std::string server;
    int sock;
    bool verbose;
    const char * additionalHeaders = 
    "User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:37.0) Gecko/20100101 Firefox/37.0\n"
    "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\n"
    "Connection: keep-alive\n";

    int prepareSock(const char * listenAddr, int port);
    std::string receive();
    std::string getHeader(std::string url);
    std::string parseUrl(const std::string& url);
public:
    Downloader(std::string server, bool verbose = false);
    ~Downloader();
    Response download(std::string url);
};


#endif //SEMESTRALKA_DOWNLOADER_H
