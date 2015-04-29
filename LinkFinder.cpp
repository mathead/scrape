#include "LinkFinder.h"
#include <iostream>
using namespace std;

LinkFinder::LinkFinder(LinkReplacer* linkReplacer) : linkReplacer(linkReplacer) {}

LinkFinder::~LinkFinder() { delete linkReplacer; }

bool LinkFinder::findTagAttr(const std::string &tag, const std::string &attribute, Response &response, size_t &startpos,
                             size_t &start, size_t &end, list<string> mandatory_attributes) {
    while (true) {
        size_t tagstart, tagend, attrstart;

        // are there any more tags?
        if ((tagstart = response.content.find(tag, startpos)) == string::npos)
            return false;
        if ((tagend = response.content.find(">", tagstart)) == string::npos)
            return false;

        // does the tag have the needed attribute?
        attrstart = response.content.find(attribute, tagstart);
        if (attrstart > tagend) {
            startpos = tagend;
            continue;
        }

        // check for mandatory attributes in tag
        bool missing = false;
        for (auto &a : mandatory_attributes) {
        	size_t manstart = response.content.find(a, tagstart);
        	if (manstart > tagend) {
        		startpos = tagend;
        		missing = true;
        		break;
        	}
        }
        if (missing)
        	continue;

        // set the start and end out parameters to the attribute value (between the quotation marks)
        start = attrstart + attribute.length();
        end = response.content.find('\"', start);
        startpos = tagend;

        // if there are any tags left, return true
        return true;
    }
}
