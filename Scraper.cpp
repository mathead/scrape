//
// Created by Ja on 4/27/2015.
//

#include "Scraper.h"
#include <iostream>
using namespace std;

Scraper::Scraper(bool verbose) : fileNum(0), verbose(verbose) {
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

void Scraper::scrape(const string& url, int depth, bool first) {
    Downloader downloader(url, verbose);
    Response page = downloader.download(url);
	cout << "downloaded" << endl;
    for (auto &linkFinder : linkFinders)
        linkFinder->find(page, depth);
	cout << "replaced" << endl;
	if (first)
		downloaded[url] = "index.html";
    page.writeFile(downloaded[url]);

    if (toDownload.size()) {
        DFile next = toDownload.front();
        toDownload.pop_front();
        scrape(next.url, next.depth, false);
    }
}

string Scraper::enqueueDownload(const string& url, const string& suffix, int depth) {
    if (!downloaded.count(url)) {
        toDownload.push_back(DFile(url, depth));
        downloaded[url] = "files/file" + std::to_string(fileNum++) + suffix;
    }
    return downloaded[url];
}