#include "InternetLinkReplacer.h"
#include <algorithm>
#include <iostream>
using namespace std;

InternetLinkReplacer::InternetLinkReplacer(Scraper *scraper) : LinkReplacer(scraper) { }

string InternetLinkReplacer::replace(const string &str, const Response& response, int depth) {
    string server = response.server;

    // prepend www. if needed - the returned URL should be as standardized as possible
    if (std::count(server.begin(), server.end(), '.') == 1)
        server = "www." + server;

    // prepend http:// if needed
    if (str[0] == '/') {
        if (str.length() > 1 && str[1] == '/')
            return "http:" + str;
        return "http://" + server + str;
    }
    if (str.find("//") == string::npos)
        return "http://" + server + '/' + str;

    return str;
}
