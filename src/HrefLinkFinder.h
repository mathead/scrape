#ifndef SEMESTRALKA_HREFLINKFINDER_H
#define SEMESTRALKA_HREFLINKFINDER_H


#include "LinkFinder.h"
#include "LinkReplacer.h"

/**
 * Main LinkFinder for finding links to other web pages in the "a" tag
 */
class HrefLinkFinder : public LinkFinder {
public:
    HrefLinkFinder(LinkReplacer *linkReplacer);
    virtual void find(Response& response, int depth);
};


#endif //SEMESTRALKA_HREFLINKFINDER_H
