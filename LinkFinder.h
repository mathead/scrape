//
// Created by Ja on 4/27/2015.
//

#ifndef SEMESTRALKA_LINKFINDER_H
#define SEMESTRALKA_LINKFINDER_H

#include <memory>
#include "Response.h"
#include "LinkReplacer.h"

class LinkFinder {
protected:
    LinkReplacer* linkReplacer;
public:
    LinkFinder(LinkReplacer* linkReplacer) : linkReplacer(linkReplacer) {}
    virtual ~LinkFinder() { delete linkReplacer; }
    virtual void find(Response& response, int depth) = 0;
};


#endif //SEMESTRALKA_LINKFINDER_H
