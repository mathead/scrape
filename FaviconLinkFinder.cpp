//
// Created by Ja on 4/27/2015.
//

#include "FaviconLinkFinder.h"
using namespace std;

FaviconLinkFinder::FaviconLinkFinder(LinkReplacer *linkReplacer) : LinkFinder(linkReplacer) {}

void FaviconLinkFinder::find(Response &response, int depth) {
    size_t startpos = 0, start, end;

    // link rel="shortcut icon" already  managed by CSSLinkFinder
    if (findTagAttr("<meta ", "content=\"", response, startpos, start, end, {"itemprop=\"image\""})) {
        string rep = linkReplacer->replace(response.content.substr(start, end - start), response, depth);
        response.content.replace(start, end - start, rep);
    }
    linkReplacer->replace("/favicon.ico", response); // default location browsers search for
}