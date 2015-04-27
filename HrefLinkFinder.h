//
// Created by Ja on 4/27/2015.
//

#ifndef SEMESTRALKA_HREFLINKFINDER_H
#define SEMESTRALKA_HREFLINKFINDER_H


#include "LinkFinder.h"
#include "LinkReplacer.h"

class HrefLinkFinder : public LinkFinder {

public:
    HrefLinkFinder(LinkReplacer *linkReplacer) : LinkFinder(linkReplacer) { }
    virtual void find(Response& response, int depth);
};


#endif //SEMESTRALKA_HREFLINKFINDER_H
