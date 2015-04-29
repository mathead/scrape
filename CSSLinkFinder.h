//
// Created by Ja on 4/27/2015.
//

#ifndef SEMESTRALKA_CSSLINKFINDER_H
#define SEMESTRALKA_CSSLINKFINDER_H

#include "LinkFinder.h"
#include "LinkReplacer.h"

class CSSLinkFinder : public LinkFinder {
public:
    CSSLinkFinder(LinkReplacer *linkReplacer);
    virtual void find(Response& response, int depth);
};


#endif //SEMESTRALKA_CSSLINKFINDER_H
