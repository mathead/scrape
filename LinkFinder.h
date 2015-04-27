//
// Created by Ja on 4/27/2015.
//

#ifndef SEMESTRALKA_LINKFINDER_H
#define SEMESTRALKA_LINKFINDER_H

#include "LinkReplacer.h"

class LinkFinder {
protected:
    LinkReplacer* linkReplacer;
public:
    LinkFinder(LinkReplacer* linkReplacer);
    virtual ~LinkFinder();
    virtual void find(Response& response, int depth) = 0;
};


#endif //SEMESTRALKA_LINKFINDER_H
