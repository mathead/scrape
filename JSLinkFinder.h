//
// Created by Ja on 4/27/2015.
//

#ifndef SEMESTRALKA_JSLINKFINDER_H
#define SEMESTRALKA_JSLINKFINDER_H

#include "LinkFinder.h"
#include "LinkReplacer.h"

class JSLinkFinder : public LinkFinder {
public:
    JSLinkFinder(LinkReplacer *linkReplacer);
    virtual void find(Response& response, int depth);
};


#endif //SEMESTRALKA_JSLINKFINDER_H
