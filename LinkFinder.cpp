//
// Created by Ja on 4/27/2015.
//

#include "LinkFinder.h"
using namespace std;

LinkFinder::LinkFinder(LinkReplacer* linkReplacer) : linkReplacer(linkReplacer) {}

LinkFinder::~LinkFinder() { delete linkReplacer; }

bool LinkFinder::findTagAttr(const std::string &tag, const std::string &attribute, Response &response, size_t &startpos,
                             size_t &start, size_t &end) {
    while (true) {
        size_t tagstart, tagend, attrstart;
        if ((tagstart = response.content.find(tag, startpos)) == string::npos)
            return false;
        if ((tagend = response.content.find(">", startpos)) == string::npos)
            return false;

        attrstart = response.content.find(attribute, tagstart);
        if (attrstart > tagend) {
            startpos = tagend;
            continue;
        }

        start = attrstart + attribute.length();
        end = response.content.find_first_of("\" ", attrstart);
        startpos = tagend;
        return true;
    }
}
