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
    // wanted to do a keep-alive connection, but there were too many problems
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
        // this will append the data even with /0 bytes (which can be in binary data)
        ret.append(buffer, (unsigned int) l);

        // if EOF -> return
        // if ((unsigned int) l < sizeof(buffer)) return ret;
    }
}

string Downloader::getHeader(const string& url) const {
    return "GET http://" + server + url + " HTTP/1.0\n" + "Host: " + server + "\n" + additionalHeaders + "\n";
}

string Downloader::parseUrl(string url) {
	// trim http:// from url
	size_t found;
	if ((found = url.find("://")) != string::npos)
		url = url.substr(found + 3);

    if (url.find('/') == string::npos)
        url += '/';

    // trim the server part of url
    url = url.substr(url.find('/'));

	return url;
}

string Downloader::parseServer(string url) {
    size_t found;
    // trim the http://
    if ((found = url.find("://")) != string::npos)
        url = url.substr(found + 3);

    // get only the server part (before '/')
    if ((found = url.find('/')) != string::npos)
        url = url.substr(0, found);

    return url;
}

Response Downloader::download(string url, int maxhops) {
	sock = prepareSock(server.c_str(), 80);
	if (sock == -1) {
        if (verbose)
            cout << "socket error" << endl;
        // In case of error, we return empty Response with status -1
		return Response("", server, verbose);
	}

	url = parseUrl(url);
    string header = getHeader(url);
    if (verbose) {
        cout << ">>> Requesting " << url << " from server " << server << endl;
        // cout << header << "------------------" << endl << endl;
    }

    // send the request and receive the response
    send(sock, header.c_str(), header.length(), 0);
    Response r(receive(), server, verbose);
    if (verbose)
        cout << ">>> Received " << url << " status: " << r.status << endl;

    close(sock); // for some reason, the closing sometimes takes up to about 5 seconds

    // handle MOVED responses
    if (r.moved && maxhops > 0) {
        string newServer = parseServer(r.headers["Location"]);
        if (newServer != "_") // some sites use _ to redirect to https on the same server
            server = newServer;
        return download(r.headers["Location"], maxhops - 1);
    }


    return r;
}
