//
// Created by Ja on 4/27/2015.
//

#include "HrefLinkFinder.h"
#include <iostream>
using namespace std;

HrefLinkFinder::HrefLinkFinder(LinkReplacer *linkReplacer) : LinkFinder(linkReplacer) { }

void HrefLinkFinder::find(Response &response, int depth) {
    size_t startpos = 0, start, end;
    while(findTagAttr("<a ", "href=\"", response, startpos, start, end)) {
        string rep = linkReplacer->replace(response.content.substr(start, end - start), response);
        response.content.replace(start, end - start, rep);
    }
}
