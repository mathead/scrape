//
// Created by Ja on 4/27/2015.
//

#include "HrefLinkFinder.h"
#include <iostream>
using namespace std;

HrefLinkFinder::HrefLinkFinder(LinkReplacer *linkReplacer) : LinkFinder(linkReplacer) { }

void HrefLinkFinder::find(Response &response, int depth) {
    size_t startpos = 0;
    while((startpos = response.content.find("<a ", startpos)) != string::npos) {
        size_t hrefstart = response.content.find("href=\"", startpos);
        if (hrefstart == string::npos) { startpos++; continue; }
        hrefstart += 6;
        size_t hrefend = response.content.find_first_of('"', hrefstart);

        string rep = linkReplacer->replace(response.content.substr(hrefstart, hrefend - hrefstart), response);
        response.content.replace(hrefstart, hrefend - hrefstart, rep);
        
        startpos++;
    }
}
