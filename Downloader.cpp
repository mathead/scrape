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


Downloader::Downloader(const string& server, bool verbose) : verbose(verbose) {
    this->server = parseServer(server);
    // sock = prepareSock(server.c_str(), 80);
}

Downloader::~Downloader() {
    // close(sock);
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
	if (verbose)
        cout << ">>> Receiving from server " << server << endl;

    string ret;
    while (1) {
        char buffer[1024];
        // the reply may be long - we read it in a loop
        int l = recv(sock, buffer, sizeof(buffer), 0);
        // l < 0 -> error, l == 0 -> finished
        if (l <= 0) return ret;
        ret.append(buffer, l);

        // if EOF -> return
        // if ((unsigned int) l < sizeof(buffer)) return ret;
    }
}

string Downloader::getHeader(const string& url) {
    return "GET " + url + " HTTP/1.0\n" + additionalHeaders + "\n";
}

string Downloader::parseUrl(string url) {
	// trim http:// from url
	size_t found;
	if ((found = url.find("://")) != string::npos)
		url = url.substr(found + 3);

    if (url.find('/') == string::npos)
        url += '/';

    url = url.substr(url.find('/'));

	return url;
}

string Downloader::parseServer(string url) {
    size_t found;
    if ((found = url.find("://")) != string::npos)
        url = url.substr(found + 3);

    if ((found = url.find('/')) != string::npos)
        url = url.substr(0, found);

    return url;
}

Response Downloader::download(string url, int maxhops) {
	sock = prepareSock(server.c_str(), 80);
	if (sock == -1) {
        if (verbose)
            cout << "socket error" << endl;
		return Response("", server, verbose);
	}

	url = parseUrl(url);
    string header = getHeader(url);
    if (verbose) {
        cout << ">>> Requesting " << url << " from server " << server << endl;
        // cout << header << "------------------" << endl << endl;
    }

    send(sock, header.c_str(), header.length(), 0);
    Response r(receive(), server, verbose);
    if (verbose)
        cout << ">>> Received " << url << " status: " << r.status << endl;
    
    close(sock);

    // handle MOVED responses
    if (r.moved && maxhops > 0) {
        server = parseServer(r.headers["Location"]);
        return download(r.headers["Location"], maxhops - 1);
    }


    return r;
}
