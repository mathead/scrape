#ifndef SEMESTRALKA_FAVICONLINKFINDER_H
#define SEMESTRALKA_FAVICONLINKFINDER_H

#include "LinkFinder.h"
#include "LinkReplacer.h"

/**
 * LinkFinder for searching for favicons (not very effective as often they are identified by their path and not a html tag)
 */
class FaviconLinkFinder : public LinkFinder {
public:
    FaviconLinkFinder(LinkReplacer *linkReplacer);
    virtual void find(Response& response, int depth);
};


#endif //SEMESTRALKA_FAVICONLINKFINDER_H
