//
// Created by Ja on 4/27/2015.
//

#include "Scraper.h"
#include "InternetLinkReplacer.h"
#include "MissingLinkReplacer.h"
#include "DownloadLinkReplacer.h"
#include "HrefLinkFinder.h"
#include "ImageLinkFinder.h"
#include "CSSLinkFinder.h"
#include "FaviconLinkFinder.h"
#include "JSLinkFinder.h"
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
using namespace std;

Scraper::Scraper(bool verbose) : fileNum(0), lastDepth(-1), lastDepthDownloaded(0), verbose(verbose),
								 missingCreated(false), filesDir("files") {
	linkFinders.push_back(linkFinderPtr(move(new HrefLinkFinder(new DownloadLinkReplacer(this, true)))));
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

	if (stat(filesDir.c_str(), &st) == -1)
		mkdir(filesDir.c_str(), 0700);

	// get absolute path
	char *abspath = realpath(filesDir.c_str(), NULL);
	filesPath = abspath;
	filesPath = filesPath;
	free(abspath);
}

void Scraper::scrape(const string& url, int depth, bool first) {
    Downloader downloader(url, verbose);
    Response page = downloader.download(url);

	if (first) {
		startDepth = depth;
		lastDepth = startDepth;
		InternetLinkReplacer i(this);
		downloaded[i.replace(url, page)] = filesPath.substr(0, filesPath.length() - filesDir.length() - 1) + "index.html";
		downloaded[url] = "index.html";
	}

	if (verbose)
		cout << ">>> Downloaded" << endl;
	updateStatusLine(depth);

	if (depth > 0)
	    for (auto &linkFinder : linkFinders)
	        linkFinder->find(page, depth);
	else
		for (auto &linkFinder : linkFindersDepth0)
		    linkFinder->find(page, depth);

	if (verbose)
		cout << ">>> Replaced" << endl << endl;

    page.writeFile(downloaded[url]);


    if (toDownload.size()) {
        DFile next = toDownload.front();
        toDownload.pop_front();
        scrape(next.url, next.depth, false);
    }
}

string Scraper::enqueueDownload(const string& url, const string& suffix, int depth) {
    if (!downloaded.count(url)) {
		if (verbose)
			cout << "Enqueued " << url << " with suffix " << suffix << " depth " << depth << endl;

        toDownload.push_back(DFile(url, depth));
        downloaded[url] = filesPath + "/file" + std::to_string(fileNum++) + suffix;
    }

    return downloaded[url];
}

void Scraper::updateStatusLine(int depth) {
	lastDepthDownloaded++;

	if (depth == 0) {
		for (auto &f : toDownload) {
			if (f.depth > depth)
				depth = f.depth;
		}
	}

	int count = 0;
	for (auto &f : toDownload) {
		if (f.depth >= depth)
			count++;
	}

	if (depth != lastDepth) {
		cout << endl;
		lastDepth = depth;
		lastDepthDownloaded = 0;
	}

	cout << "\rDepth: " << startDepth - depth << " - [";
	int total = max(count + lastDepthDownloaded, 1);
	for (int i = 0; i < 20; i++) {
		if (lastDepthDownloaded * 20 / total > i)
			cout << '=';
		else
			cout << ' ';
	}
	cout << "] " << lastDepthDownloaded * 100 / total << "% - " << 
            lastDepthDownloaded << '/' << total << " files downloaded              " << std::flush;
}