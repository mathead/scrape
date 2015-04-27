//
// Created by Ja on 4/27/2015.
//

#include "HrefLinkFinder.h"
using namespace std;

void HrefLinkFinder::find(Response &response, int depth) {
    size_t startpos = 0;
    while((startpos = response.content.find("<a", startpos)) != std::string::npos){
        size_t endpos = response.content.find(">", startpos);
        size_t hrefstart = response.content.find("href=\"", startpos, endpos) + 6;
        size_t hrefend = response.content.find_first_of('"', hrefstart);
        string rep = linkReplacer->replace(response.content.substr(hrefstart, hrefend), response);
        response.content.replace(hrefstart, hrefend - hrefstart, rep);
        startpos++;
    }
}
