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
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdlib.h>
using namespace std;

Scraper::Scraper(const list<string>& filters, const list<string>& antifilters, const string& indexName, const string& filesDir,
	             bool stayOnServer, bool verbose, bool downloadImages, bool downloadExtras, bool lastMissing) : 
					 filters(filters), antifilters(antifilters), fileNum(0), lastDepth(-1), lastDepthDownloaded(0), stayOnServer(stayOnServer),
					 verbose(verbose), missingCreated(false), filesCreated(false), indexName(indexName), filesDir(filesDir) {

	// Initialize the lists of linkFinders that are applied to every downloaded page
	// A bit messy here, but the pointers have to be moved to unique_ptr and there is a lot of config options...

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
	// update that nifty loading bar
	if (!first)
		updateStatusLine(url, depth);

	// download the page
    Downloader downloader(url, verbose);
    Response page = downloader.download(url);

	if (first) {
		// some initialization if this is the main index.html page
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
		updateStatusLine(url, depth);
	}

	if (verbose)
		cout << ">>> Downloaded" << endl;

	// Let all the LinkFinders do their thing
	// if this is the last depth, it gets handled differently
	if (depth > 0)
	    for (auto &linkFinder : linkFinders)
	        linkFinder->find(page, depth);
	else
		for (auto &linkFinder : linkFindersDepth0)
		    linkFinder->find(page, depth);

	if (verbose)
		cout << ">>> Replaced" << endl << endl;

	// save the modified page
    page.writeFile(downloaded[url]);


	// if there is something in the queue, scrape it next
    if(toDownload.size()) {
        DFile next = toDownload.front();
        toDownload.pop_front();

        scrape(next.url, next.depth, false);
    }

    return true;
}

string Scraper::enqueueDownload(const string& url, const string& suffix, int depth) {
	// didn't we download this already?
    if (!downloaded.count(url)) {
		// run it through the filters
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

		// enqueue it if it got here
        toDownload.push_back(DFile(url, depth));
		// and save the reference to the file on disk
        downloaded[url] = getFilesPath() + "/file" + std::to_string(fileNum++) + suffix;
    }

    return "file://" + downloaded[url];
}

void Scraper::updateStatusLine(const string& url, int depth) {
	lastDepthDownloaded++;

	// get the current depth (it's not always sorted)
	if (depth == 0) {
		for (auto &f : toDownload) {
			if (f.depth > depth)
				depth = f.depth;
		}
	}

	// count the files in the current depth
	int count = 0;
	for (auto &f : toDownload) {
		if (f.depth >= depth)
			count++;
	}

	// is this the next phase?
	if (depth != lastDepth) {
		cout << endl;
		lastDepth = depth;
		lastDepthDownloaded = 0;
	}

	// get the terminal width
	struct winsize w;
	ioctl(0, TIOCGWINSZ, &w);

	// delete the last and print out the updated status line
	cout << "\rDepth: " << startDepth - depth << " - [";
	// the download bar
	int total = max(count + lastDepthDownloaded, 1);
	for (int i = 0; i < 20; i++) {
		if (lastDepthDownloaded * 20 / total > i)
			cout << '=';
		else
			cout << ' ';
	}
	// also in percent
	cout << "] " << lastDepthDownloaded * 100 / total << "% - " << 
            lastDepthDownloaded << '/' << total << " files downloaded - Downloading ";

	// print the page we are currently downloading (limit length to terminal width)
    if (url.length() + 85 < w.ws_col) { // 85 - length of previous output
    	cout << url;
    	for (int i = url.length() + 85; i < w.ws_col; i++)
    		cout << ' ';
    } else 
    	cout << url.substr(0, w.ws_col - 88) << "...";

    cout << std::flush; // we need to flush it, because we didn't have endl anywhere
}

string Scraper::getMissingPage() {
	// create the missing page only if it is ever needed
	if (!missingCreated) {
        ofstream file(getFilesPath() + "/missing.html");
        file << "Unfortunately, this page isn't downloaded :(";
        file.close();

        missingCreated = true;
    }

    return "file://" + getFilesPath() + "/missing.html";
}

string Scraper::getFilesPath() {
	// create the files directory only if it is ever needed
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
	free(abspath); // it's allocated in the realpath

	return ret;
}