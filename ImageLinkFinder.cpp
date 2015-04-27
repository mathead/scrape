//
// Created by Ja on 4/27/2015.
//

#include "ImageLinkFinder.h"
using namespace std;

ImageLinkFinder::ImageLinkFinder(LinkReplacer *linkReplacer) : LinkFinder(linkReplacer) {}

void ImageLinkFinder::find(Response &response, int depth) {
    size_t startpos = 0, start, end;
    while(findTagAttr("<img ", "src=\"", response, startpos, start, end)) {
        string rep = linkReplacer->replace(response.content.substr(start, end - start), response);
        response.content.replace(start, end - start, rep);
    }
}