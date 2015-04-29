//
// Created by Ja on 4/27/2015.
//

#include "DownloadLinkReplacer.h"
#include "Scraper.h"
#include <fstream>
#include <iostream>
using namespace std;

DownloadLinkReplacer::DownloadLinkReplacer(Scraper *scraper, bool propagateDepth)
 : InternetLinkReplacer(scraper), propagateDepth(propagateDepth) { }

int DownloadLinkReplacer::getNextDepth(int current) {
	return propagateDepth * (current - 1);
}

string DownloadLinkReplacer::replace(const string &str, const Response& response, int depth) {
	size_t double_slash = str.find("//");
    size_t dot = str.rfind('.');
    size_t slash = str.rfind('/');

    if (dot == string::npos || slash == string::npos || slash > dot || slash <= double_slash + 1)
    	return scraper->enqueueDownload(InternetLinkReplacer::replace(str, response), "", getNextDepth(depth));

    // guessing file suffix from address came out as ineffective
    string suffix = str.substr(dot);
    return scraper->enqueueDownload(InternetLinkReplacer::replace(str, response), "", getNextDepth(depth));
}
