#ifndef SEMESTRALKA_MISSINGLINKREPLACER_H
#define SEMESTRALKA_MISSINGLINKREPLACER_H

#include "InternetLinkReplacer.h"
class Scraper; // forward declaration

/**
 * LinkReplacer that, if the link is not already downloaded, just points to a missing page
 */
class MissingLinkReplacer : public InternetLinkReplacer {
public:
    MissingLinkReplacer(Scraper *scraper);
    std::string replace(const std::string& str, const Response& response, int depth = 0);
};


#endif //SEMESTRALKA_MISSINGLINKREPLACER_H
