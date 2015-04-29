#ifndef SEMESTRALKA_CSSLINKFINDER_H
#define SEMESTRALKA_CSSLINKFINDER_H

#include "LinkFinder.h"
#include "LinkReplacer.h"

/**
 * LinkFinder for CSS (<link href=""> tags)
 */
class CSSLinkFinder : public LinkFinder {
public:
    CSSLinkFinder(LinkReplacer *linkReplacer);
    virtual void find(Response& response, int depth);
};


#endif //SEMESTRALKA_CSSLINKFINDER_H
