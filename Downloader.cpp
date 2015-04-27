//
// Created by Ja on 4/27/2015.
//

#include "Downloader.h"
#include "Response.h"
#include <cstdio>
#include <cstring>
#include <cctype>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <iostream>

using namespace std;


Downloader::Downloader(string server, bool verbose) : server(server), verbose(verbose) {
    sock = prepareSock(server.c_str(), 80);
    if (sock == -1)
        cout << "socket error";
}

Downloader::~Downloader() {
    close(sock);
}

int Downloader::prepareSock(const char *listenAddr, int port) {
    struct addrinfo *ai;
    char portTx[20];

    snprintf(portTx, sizeof(portTx), "%d", port);
    int error;
    if ((error = getaddrinfo(listenAddr, portTx, NULL, &ai)) != 0) { 
    	if (verbose)
    		cerr << gai_strerror(error) << endl;
    	return -1;
    }

    int tsock = socket(ai->ai_family, SOCK_STREAM, 0);
    if (tsock == -1) {
        freeaddrinfo(ai);
        return -1;
    }

    if (connect(tsock, ai->ai_addr, ai->ai_addrlen) != 0) {
        close(tsock);
        freeaddrinfo(ai);
        return -1;
    }
    freeaddrinfo(ai);
    return tsock;
}

string Downloader::receive() {
    string ret;
    while (1) {
        char buffer[50];
        // the reply may be long - we read it in a loop
        int l = recv(sock, buffer, sizeof(buffer) - 1, 0);
        // l < 0 -> error, l == 0 -> finished
        if (l <= 0) return ret;
        buffer[l] = 0;
        if (verbose)
            printf("%s", buffer);
        ret += buffer;
        // if EOF -> return
        if (index(buffer, EOF)) return ret;
    }
}

string Downloader::getHeader(string url) {
    return "GET " + url + " HTTP/1.0\n" + additionalHeaders + "\n";
}

Response Downloader::download(string url) {
    string header = getHeader(url);
    if (verbose) {
        cout << "Downloading " << url << " from server " << server << " with header " << endl;
        cout << header;
        cout << endl << "------------------" << endl << endl;
    }
    send(sock, header.c_str(), header.length(), 0);
    Response r(receive(), verbose);

    return r;
}
