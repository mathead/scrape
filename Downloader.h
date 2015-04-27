//
// Created by Ja on 4/27/2015.
//

#ifndef SEMESTRALKA_DOWNLOADER_H
#define SEMESTRALKA_DOWNLOADER_H

#include <string>

class Downloader {
    std::string server;
    int sock;
    bool verbose;
    const char * additionalHeaders = "User-Agent: HTTPTool/1.0\n";

    int prepareSock(const char * listenAddr, int port);
    std::string receive();
    std::string getHeader(std::string url);
public:
    Downloader(std::string server, bool verbose = false);
    ~Downloader();
    std::string download(std::string url);
};


#endif //SEMESTRALKA_DOWNLOADER_H
