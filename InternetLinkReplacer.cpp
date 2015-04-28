//
// Created by Ja on 4/27/2015.
//

#include "InternetLinkReplacer.h"
#include <algorithm>
#include <iostream>
using namespace std;

InternetLinkReplacer::InternetLinkReplacer(Scraper *scraper) : LinkReplacer(scraper) { }

string InternetLinkReplacer::replace(const string &str, const Response& response, int depth) {
    string server = response.server;
    if (std::count(server.begin(), server.end(), '.') == 1)
        server = "www." + server;

    if (str[0] == '/') {
        if (str.length() > 1 && str[1] == '/')
            return "http:" + str;
        return "http://" + server + str;
    }
    if (str.find("//") == string::npos)
        return "http://" + server + '/' + str;
    return str;
}
