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
#include <fstream>
#include <list>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
using namespace std;

Scraper::Scraper(const list<string>& filters, const list<string>& antifilters, const string& indexName, const string& filesDir, bool stayOnServer, bool verbose) : 
					 filters(filters), antifilters(antifilters), fileNum(0), lastDepth(-1), lastDepthDownloaded(0), stayOnServer(stayOnServer),
					 verbose(verbose), missingCreated(false), indexName(indexName), filesDir(filesDir) {

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


	this->antifilters.push_back("mailto:");

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

bool Scraper::scrape(const string& url, int depth, bool first) {
    Downloader downloader(url, verbose);
    Response page = downloader.download(url);

	if (first) {
		if (!page.ok) {
			cerr << "Couldn't download " << url << endl 
				 << "Server " << page.server << " responded with status " << page.status;
			return 1;
		}

		startDepth = depth;
		startServer = page.server;
		lastDepth = startDepth;
		InternetLinkReplacer i(this);
		downloaded[i.replace(url, page)] = filesPath.substr(0, filesPath.length() - filesDir.length() - 1) + indexName;
		downloaded[url] = indexName;
	}

	if (verbose)
		cout << ">>> Downloaded" << endl;
	updateStatusLine(url, depth);

	if (depth > 0)
	    for (auto &linkFinder : linkFinders)
	        linkFinder->find(page, depth);
	else
		for (auto &linkFinder : linkFindersDepth0)
		    linkFinder->find(page, depth);

	if (verbose)
		cout << ">>> Replaced" << endl << endl;

    page.writeFile(downloaded[url]);


    if(toDownload.size()) {
        DFile next = toDownload.front();
        toDownload.pop_front();

        scrape(next.url, next.depth, false);
    }

    return true;
}

string Scraper::enqueueDownload(const string& url, const string& suffix, int depth) {
    if (!downloaded.count(url)) {
        if (stayOnServer && Downloader::parseServer(url).find(startServer) == string::npos) {
        	if (verbose)
				cout << ">>> Skipping " << url << endl;
        	return url;
        }

        for (auto &f : filters)
			if (url.find(f) == string::npos) {
				if (verbose)
					cout << ">>> Skipping " << url << endl;
				return url;
			}
		for (auto &f : antifilters)
			if (url.find(f) != string::npos) {
				if (verbose)
					cout << ">>> Skipping " << url << endl;
				return url;
			}


		if (verbose)
			cout << ">>> Enqueued " << url << " with suffix " << suffix << " depth " << depth << endl;

        toDownload.push_back(DFile(url, depth));
        downloaded[url] = filesPath + "/file" + std::to_string(fileNum++) + suffix;
    }

    return "file://" + downloaded[url];
}

void Scraper::updateStatusLine(const string& url, int depth) {
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
            lastDepthDownloaded << '/' << total << " files downloaded - Downloading ";
    if (url.length() < 50) {
    	cout << url;
    	for (int i = url.length(); i < 50; i++)
    		cout << ' ';
    } else 
    	cout << url.substr(0, 46) << "...";
    cout << std::flush;
}

string Scraper::getMissingPage() {
	if (!missingCreated) {
        ofstream file(filesPath + "/missing.html");
        file << "Unfortunately, this page isn't downloaded :(";
        file.close();

        missingCreated = true;
    }

    return "file://" + filesPath + "/missing.html";
}