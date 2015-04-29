#ifndef SEMESTRALKA_IMAGELINKFINDER_H
#define SEMESTRALKA_IMAGELINKFINDER_H

#include "LinkFinder.h"
#include "LinkReplacer.h"

/**
 * LinkFinder for finding images in src attribute of "img" tags
 */
class ImageLinkFinder : public LinkFinder {
public:
    ImageLinkFinder(LinkReplacer *linkReplacer);
    virtual void find(Response& response, int depth);
};


#endif //SEMESTRALKA_IMAGELINKFINDER_H
