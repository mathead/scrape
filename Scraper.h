//
// Created by Ja on 4/27/2015.
//

#ifndef SEMESTRALKA_SCRAPER_H
#define SEMESTRALKA_SCRAPER_H

#include <string>
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
    Scraper() : fileNum(0) {
        linkFinders = {
                linkFinderPtr(new HrefLinkFinder(new InternetLinkReplacer(this))),
//                linkFinderPtr(new HrefLinkFinder(new DownloadLinkReplacer(this, true))),
//                linkFinderPtr(new CSSLinkFinder(new DownloadLinkReplacer(this, false))),
//                linkFinderPtr(new JSLinkFinder(new DownloadLinkReplacer(this, false))),
//                linkFinderPtr(new ImageLinkFinder(new InternetLinkReplacer(this))),
//                linkFinderPtr(new OtherLinkFinder(new DownloadLinkReplacer(this, false))),
        };
    }

    void scrape(const std::string& url, int depth) {
        Downloader downloader(url);
        Response page = downloader.download(url);
        for (auto &linkFinder : linkFinders)
            linkFinder->find(page, depth);
        page.writeFile(downloaded[url]);

        if (toDownload.size()) {
            DFile next = toDownload.front();
            toDownload.pop_front();
            scrape(next.url, next.depth);
        }
    }

    std::string enqueueDownload(const std::string& url, const std::string& suffix, int depth) {
        if (!downloaded.count(url)) {
            toDownload.push_back(DFile(url, depth));
            downloaded[url] = "files/file" + fileNum++ + suffix;
        }
        return downloaded[url];
    }
};


#endif //SEMESTRALKA_SCRAPER_H
