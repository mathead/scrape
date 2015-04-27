//
// Created by Ja on 4/27/2015.
//

#ifndef SEMESTRALKA_RESPONSE_H
#define SEMESTRALKA_RESPONSE_H

#include <string>
#include <map>

class Response {
public:
    bool ok; // status is ok and content can be read
    bool moved; // site has moved, another request to Location header has to be made
    bool fail; // something wrong happened when parsing response
    bool verbose;
    int status;
    std::map<std::string, std::string> headers;
    std::string content, server;

    Response(const std::string& response, bool verbose = false);
    void writeFile(const std::string& path);
};


#endif //SEMESTRALKA_RESPONSE_H
