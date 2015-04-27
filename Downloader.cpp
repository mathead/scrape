//
// Created by Ja on 4/27/2015.
//

#include "Downloader.h"
#include <cstdio>
#include <cstring>
#include <cctype>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <iostream>

using namespace std;


Downloader::Downloader(string server) : server(server) {
    socket = prepareSock(server.c_str(), 80);
    if (socket == -1)
        cout << "socket error";
}

Downloader::~Downloader() {
    close(socket);
}

int Downloader::prepareSock(const char *listenAddr, int port) {
    struct addrinfo *ai;
    char portTx[20];

    snprintf(portTx, sizeof(portTx), "%d", port);
    if (getaddrinfo(listenAddr, portTx, NULL, &ai) != 0) return -1;

    int sock = socket(ai->ai_family, SOCK_STREAM, 0);
    if (sock == -1) {
        freeaddrinfo(ai);
        return -1;
    }

    if (connect(sock, ai->ai_addr, ai->ai_addrlen) != 0) {
        close(sock);
        freeaddrinfo(ai);
        return -1;
    }
    freeaddrinfo(ai);
    return sock;
}

string Downloader::receive() {
    // receive & print everything until newline.
    while (1) {
        char buffer[50];
        // the reply may be long - we read it in a loop
        int l = recv(socket, buffer, sizeof(buffer) - 1, 0);
        // l < 0 -> error, l == 0 -> finished
        if (l <= 0) return "";
        buffer[l] = 0;
        printf("%s", buffer);
        // if newline -> return
        if (index(buffer, '\n')) return "";
    }
}

string Downloader::getHeader(string url) {
    return "GET " + url + " HTTP/1.0\n" + additionalHeaders + "\n";
}

string Downloader::download(string url) {
    string header = getHeader(url);
    send(socket, header.c_str(), header.length(), 0);

    return receive();
}
