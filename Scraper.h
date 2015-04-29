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
    std::list<std::string> filters;
    std::list<std::string> antifilters;
    std::list<DFile> toDownload;
    int fileNum, startDepth, lastDepth, lastDepthDownloaded;
    bool stayOnServer;
    bool verbose;
    bool missingCreated;
    std::string indexName;
    std::string startServer;

    void updateStatusLine(const std::string& url, int depth);
public:
    std::map<std::string, std::string> downloaded;
    std::string filesPath;
    std::string filesDir;

    Scraper(const std::list<std::string>& filters, const std::list<std::string>& antifilters, const std::string& indexName = "index.html", 
            const std::string& filesDir = "files", bool stayOnServer = false, bool verbose = false);
    bool scrape(const std::string& url, int depth, bool first = true);
    std::string enqueueDownload(const std::string& url, const std::string& suffix, int depth);
    std::string getMissingPage();
};


#endif //SEMESTRALKA_SCRAPER_H
