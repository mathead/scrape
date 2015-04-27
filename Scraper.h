//
// Created by Ja on 4/27/2015.
//

#ifndef SEMESTRALKA_SCRAPER_H
#define SEMESTRALKA_SCRAPER_H

#include <list>
#include <memory>
#include "Downloader.h"
#include "Response.h"
#include "LinkFinder.h"

typedef std::unique_ptr<LinkFinder> linkFinderPtr;

struct DFile {
    std::string url;
    int depth;
    DFile(const std::string& url, int depth) : url(url), depth(depth) {}
};

class Scraper {
    std::list<linkFinderPtr> linkFinders;
    std::list<linkFinderPtr> linkFindersDepth0;
    std::list<DFile> toDownload;
    std::map<std::string, std::string> downloaded;
    int fileNum;
    bool verbose;
public:
    bool missingCreated;
    std::string filesPath = "files";

    Scraper(bool verbose = false);
    void scrape(const std::string& url, int depth, bool first = true);
    std::string enqueueDownload(const std::string& url, const std::string& suffix, int depth);
};


#endif //SEMESTRALKA_SCRAPER_H
