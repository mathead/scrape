//
// Created by Ja on 4/27/2015.
//

#ifndef SEMESTRALKA_FAVICONLINKFINDER_H
#define SEMESTRALKA_FAVICONLINKFINDER_H

#include "LinkFinder.h"
#include "LinkReplacer.h"

class FaviconLinkFinder : public LinkFinder {
public:
    FaviconLinkFinder(LinkReplacer *linkReplacer);
    virtual void find(Response& response, int depth);
};


#endif //SEMESTRALKA_FAVICONLINKFINDER_H
