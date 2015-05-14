//
// Created by Ja on 4/27/2015.
//

#ifndef SEMESTRALKA_RESPONSE_H
#define SEMESTRALKA_RESPONSE_H

#include <string>
#include <map>

/**
 * Class that parses and wraps the HTTP response from server.
 * It saves the content, status and headers
 */
class Response {
public:
    /// status is ok and content can be read
    bool ok;
    /// site has moved, another request to Location header has to be made
    bool moved;
    /// something wrong happened when parsing response
    bool fail;
    bool verbose;
    /// if not set, status is -1 (usually on socket error)
    int status;
    /// all headers from the response
    std::map<std::string, std::string> headers;
    /// the actual content of the response
    std::string content;
    /// name of the server the response came from
    std::string server;

    /**
     * Parses the raw HTTP response from server
     */
    Response(const std::string& response, const std::string& server, bool verbose = false);

    /**
     * Write the content to a binary file
     */
    void writeFile(const std::string& path) const;
};


#endif //SEMESTRALKA_RESPONSE_H
