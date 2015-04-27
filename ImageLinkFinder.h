//
// Created by Ja on 4/27/2015.
//

#ifndef SEMESTRALKA_IMAGELINKFINDER_H
#define SEMESTRALKA_IMAGELINKFINDER_H

#include "LinkFinder.h"
#include "LinkReplacer.h"

class ImageLinkFinder : public LinkFinder {
public:
    ImageLinkFinder(LinkReplacer *linkReplacer);
    virtual void find(Response& response, int depth);
};


#endif //SEMESTRALKA_IMAGELINKFINDER_H
