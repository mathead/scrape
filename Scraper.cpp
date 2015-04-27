//
// Created by Ja on 4/27/2015.
//

#include "Scraper.h"
using namespace std;

Scraper::Scraper() : fileNum(0) {
	linkFinders.push_back(linkFinderPtr(move(new HrefLinkFinder(new InternetLinkReplacer(this)))));
    // linkFinders = {
            // linkFinderPtr(move(new HrefLinkFinder(new InternetLinkReplacer(this)))),
//                linkFinderPtr(new HrefLinkFinder(new DownloadLinkReplacer(this, true))),
//                linkFinderPtr(new CSSLinkFinder(new DownloadLinkReplacer(this, false))),
//                linkFinderPtr(new JSLinkFinder(new DownloadLinkReplacer(this, false))),
//                linkFinderPtr(new ImageLinkFinder(new InternetLinkReplacer(this))),
//                linkFinderPtr(new OtherLinkFinder(new DownloadLinkReplacer(this, false))),
    // };
}

void Scraper::scrape(const string& url, int depth) {
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

string Scraper::enqueueDownload(const string& url, const string& suffix, int depth) {
    if (!downloaded.count(url)) {
        toDownload.push_back(DFile(url, depth));
        downloaded[url] = "files/file" + std::to_string(fileNum++) + suffix;
    }
    return downloaded[url];
}