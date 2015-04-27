//
// Created by Ja on 4/27/2015.
//

#ifndef SEMESTRALKA_SCRAPER_H
#define SEMESTRALKA_SCRAPER_H

#include <vector>
#include <list>
#include <memory>
#include "Downloader.h"
#include "Response.h"
#include "LinkFinder.h"
#include "HrefLinkFinder.h"
#include "InternetLinkReplacer.h"

typedef std::unique_ptr<LinkFinder> linkFinderPtr;

struct DFile {
    std::string url;
    int depth;
    DFile(const std::string& url, int depth) : url(url), depth(depth) {}
};

class Scraper {
    std::vector<linkFinderPtr> linkFinders;
    std::list<DFile> toDownload;
    std::map<std::string, std::string> downloaded;
    int fileNum;
public:
    Scraper();

    void scrape(const std::string& url, int depth);

    std::string enqueueDownload(const std::string& url, const std::string& suffix, int depth);
};


#endif //SEMESTRALKA_SCRAPER_H
