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

Scraper::Scraper(const list<string>& filters, const list<string>& antifilters, const string& indexName, const string& filesDir,
	             bool stayOnServer, bool verbose, bool downloadImages, bool downloadExtras, bool lastMissing) : 
					 filters(filters), antifilters(antifilters), fileNum(0), lastDepth(-1), lastDepthDownloaded(0), stayOnServer(stayOnServer),
					 verbose(verbose), missingCreated(false), filesCreated(false), indexName(indexName), filesDir(filesDir) {

	// LinkReplacers for standard scraping according to arguments
    if (downloadImages)
		linkFinders.push_back(linkFinderPtr(move(new ImageLinkFinder(new DownloadLinkReplacer(this, false)))));
	else
		linkFinders.push_back(linkFinderPtr(move(new ImageLinkFinder(new InternetLinkReplacer(this)))));

	if (downloadExtras) {
		linkFinders.push_back(linkFinderPtr(move(new CSSLinkFinder(new DownloadLinkReplacer(this, false)))));
		linkFinders.push_back(linkFinderPtr(move(new FaviconLinkFinder(new DownloadLinkReplacer(this, false)))));
		linkFinders.push_back(linkFinderPtr(move(new JSLinkFinder(new DownloadLinkReplacer(this, false)))));
	} else {
		linkFinders.push_back(linkFinderPtr(move(new CSSLinkFinder(new InternetLinkReplacer(this)))));
		linkFinders.push_back(linkFinderPtr(move(new FaviconLinkFinder(new InternetLinkReplacer(this)))));
		linkFinders.push_back(linkFinderPtr(move(new JSLinkFinder(new InternetLinkReplacer(this)))));
	}

	// links should be downloaded last so we can view the page (with images) as soon as possible
	linkFinders.push_back(linkFinderPtr(move(new HrefLinkFinder(new DownloadLinkReplacer(this, true)))));


	// LinkReplacers for last depth
	if (downloadImages)
		linkFindersDepth0.push_back(linkFinderPtr(move(new ImageLinkFinder(new DownloadLinkReplacer(this, false)))));
	else
		linkFindersDepth0.push_back(linkFinderPtr(move(new ImageLinkFinder(new InternetLinkReplacer(this)))));

	if (downloadExtras) {
		linkFindersDepth0.push_back(linkFinderPtr(move(new CSSLinkFinder(new DownloadLinkReplacer(this, false)))));
		linkFindersDepth0.push_back(linkFinderPtr(move(new FaviconLinkFinder(new DownloadLinkReplacer(this, false)))));
		linkFindersDepth0.push_back(linkFinderPtr(move(new JSLinkFinder(new DownloadLinkReplacer(this, false)))));
	} else {
		linkFindersDepth0.push_back(linkFinderPtr(move(new CSSLinkFinder(new InternetLinkReplacer(this)))));
		linkFindersDepth0.push_back(linkFinderPtr(move(new FaviconLinkFinder(new InternetLinkReplacer(this)))));
		linkFindersDepth0.push_back(linkFinderPtr(move(new JSLinkFinder(new InternetLinkReplacer(this)))));
	}

	if (lastMissing)
		linkFindersDepth0.push_back(linkFinderPtr(move(new HrefLinkFinder(new MissingLinkReplacer(this)))));
	else
		linkFindersDepth0.push_back(linkFinderPtr(move(new HrefLinkFinder(new InternetLinkReplacer(this)))));


	this->antifilters.push_back("mailto:"); // don't try to download mailto links
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
		string abspath = getAbsPath(".") + "/" + indexName;
		downloaded[i.replace(url, page)] = abspath;
		downloaded[url] = abspath;
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
        downloaded[url] = getFilesPath() + "/file" + std::to_string(fileNum++) + suffix;
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
        ofstream file(getFilesPath() + "/missing.html");
        file << "Unfortunately, this page isn't downloaded :(";
        file.close();

        missingCreated = true;
    }

    return "file://" + getFilesPath() + "/missing.html";
}

string Scraper::getFilesPath() {
	if (!filesCreated) {
		// create files directory
		struct stat st = {0};

		if (stat(filesDir.c_str(), &st) == -1)
			mkdir(filesDir.c_str(), 0700);

		filesPath = getAbsPath(filesDir);
		filesCreated = true;
	}

	return filesPath;
}

string Scraper::getAbsPath(string file) {
	char *abspath = realpath(file.c_str(), NULL);
	string ret = abspath;
	free(abspath);

	return ret;
}