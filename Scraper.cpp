//
// Created by Ja on 4/27/2015.
//

#include "Scraper.h"
#include "InternetLinkReplacer.h"
#include "MissingLinkReplacer.h"
#include "HrefLinkFinder.h"
#include "ImageLinkFinder.h"
#include "CSSLinkFinder.h"
#include "FaviconLinkFinder.h"
#include "JSLinkFinder.h"
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
using namespace std;

Scraper::Scraper(bool verbose) : fileNum(0), verbose(verbose), missingCreated(false) {
	linkFinders.push_back(linkFinderPtr(move(new HrefLinkFinder(new InternetLinkReplacer(this)))));
	linkFinders.push_back(linkFinderPtr(move(new ImageLinkFinder(new InternetLinkReplacer(this)))));
	linkFinders.push_back(linkFinderPtr(move(new CSSLinkFinder(new InternetLinkReplacer(this)))));
	linkFinders.push_back(linkFinderPtr(move(new FaviconLinkFinder(new InternetLinkReplacer(this)))));
	linkFinders.push_back(linkFinderPtr(move(new JSLinkFinder(new InternetLinkReplacer(this)))));

	linkFindersDepth0.push_back(linkFinderPtr(move(new HrefLinkFinder(new MissingLinkReplacer(this)))));
	linkFindersDepth0.push_back(linkFinderPtr(move(new ImageLinkFinder(new InternetLinkReplacer(this)))));
	linkFindersDepth0.push_back(linkFinderPtr(move(new CSSLinkFinder(new InternetLinkReplacer(this)))));
	linkFindersDepth0.push_back(linkFinderPtr(move(new FaviconLinkFinder(new InternetLinkReplacer(this)))));
	linkFindersDepth0.push_back(linkFinderPtr(move(new JSLinkFinder(new InternetLinkReplacer(this)))));

	// create files directory
	struct stat st = {0};

	if (stat(filesPath.c_str(), &st) == -1)
		mkdir(filesPath.c_str(), 0700);
}

void Scraper::scrape(const string& url, int depth, bool first) {
    Downloader downloader(url, verbose);
    Response page = downloader.download(url);

	cout << "downloaded" << endl;
	if (depth > 0)
	    for (auto &linkFinder : linkFinders)
	        linkFinder->find(page, depth);
	else
		for (auto &linkFinder : linkFindersDepth0)
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
        downloaded[url] = filesPath + "/file" + std::to_string(fileNum++) + suffix;
    }
    return downloaded[url];
}