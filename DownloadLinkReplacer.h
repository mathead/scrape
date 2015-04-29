//
// Created by Ja on 4/27/2015.
//

#ifndef SEMESTRALKA_DOWNLOADLINKREPLACER_H
#define SEMESTRALKA_DOWNLOADLINKREPLACER_H

#include "InternetLinkReplacer.h"
class Scraper;

class DownloadLinkReplacer : public InternetLinkReplacer {
	bool propagateDepth;

	int getNextDepth(int current);
public:
    DownloadLinkReplacer(Scraper *scraper, bool propagateDepth = false);
    std::string replace(const std::string& str, const Response& response, int depth = 0);
};


#endif //SEMESTRALKA_DOWNLOADLINKREPLACER_H
