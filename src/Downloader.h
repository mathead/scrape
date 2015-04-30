#ifndef SEMESTRALKA_DOWNLOADER_H
#define SEMESTRALKA_DOWNLOADER_H

#include "Response.h"
#include <string>

/**
 * Downloader for HTTP requests.
 * Connects to a server and sends the appropriate HTTP GET headers.
 */
class Downloader {
    std::string server;
    int sock;
    bool verbose;
    const char * additionalHeaders = 
    "User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:37.0) Gecko/20100101 Firefox/37.0\n"
    "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\n"
    "Accept-Charset: utf-8\n"
    // "Connection: keep-alive\n"
    "Cache-Control: max-age=0\n";

    int prepareSock(const char * listenAddr, int port);

    std::string receive();

    /// Get header for HTTP GET request
    std::string getHeader(const std::string& url);
public:
    /**
     * @param server server URL, the server part gets parsed, so it can be a full link to a page
     */
    Downloader(const std::string& server, bool verbose = false);

    ~Downloader();

    /**
     * Download a page from the server
     * @param url the location part get parsed, so it can be a full link to a page
     * @param maxhops maximum number of 301 or 302 redirects (Google likes to redirect you a lot)
     * @return Response with all the content, headers and status. If the download failed, an empty response with status -1 is returned.
     */
    Response download(std::string url, int maxhops = 5);

    /// Parse the server from a full link
    static std::string parseServer(std::string url);
    /// Parse the location on the server from a full link
    static std::string parseUrl(std::string url);
};


#endif //SEMESTRALKA_DOWNLOADER_H
