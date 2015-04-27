//
// Created by Ja on 4/27/2015.
//

#include "InternetLinkReplacer.h"
using namespace std;

string InternetLinkReplacer::replace(const string &str, const Response& response) {
    if (str.length() <= 1)
        return str;
    if (str[0] == '/') {
        if (str[1] == '/')
            return "http:" + str;
        return "http://" + response.server + str;
    }
    if (str.find("//") == string::npos)
        return "http://" + response.server + '/' + str;
    return str;
}
