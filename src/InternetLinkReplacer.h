#ifndef SEMESTRALKA_INTERNETLINKREPLACER_H
#define SEMESTRALKA_INTERNETLINKREPLACER_H

#include "LinkReplacer.h"
class Scraper;

/**
 * LinkReplacer for the links to be pointing at correct location on the internet.
 * Tries to be as standardized as possible, so that we can detect the same links and point them to same files
 */
class InternetLinkReplacer : public LinkReplacer {
public:
    InternetLinkReplacer(Scraper *scraper);
    std::string replace(const std::string& str, const Response& response, int depth = 0);
};


#endif //SEMESTRALKA_INTERNETLINKREPLACER_H
