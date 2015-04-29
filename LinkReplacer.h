#ifndef SEMESTRALKA_LINKREPLACER_H
#define SEMESTRALKA_LINKREPLACER_H

#include <string>
#include "Response.h"

class Scraper; // forward declaration

/**
 * Abstract class for replacing links in HTML supplied by LinkFinders.
 * Sometimes does some additional things (like downloading the links and pointing them to files on disk)
 */
class LinkReplacer {
protected:
    Scraper* scraper;
public:
    /**
     * @param scraper the main Scraper for some global methods
     */
    LinkReplacer(Scraper* scraper);

    virtual ~LinkReplacer();

    /**
     * Called to every found link
     * @param str the found link
     * @param response Response object for some useful information
     * @param depth current depth
     * @return link to be replaced for
     */
    virtual std::string replace(const std::string& str, const Response& response, int depth = 0) = 0;
};


#endif //SEMESTRALKA_LINKREPLACER_H
