#ifndef SEMESTRALKA_DOWNLOADLINKREPLACER_H
#define SEMESTRALKA_DOWNLOADLINKREPLACER_H

#include "InternetLinkReplacer.h"
class Scraper; // forward declaration

/**
 * LinkReplacer that enqueues found links to be downloaded next
 */
class DownloadLinkReplacer : public InternetLinkReplacer {
	bool propagateDepth;

    /// Get 1 lower depth or 0 according to propagateDepth
	int getNextDepth(int current);
public:
    /**
     * @param propagateDepth sets if the found links should be searched too or just downloaded
     */
    DownloadLinkReplacer(Scraper *scraper, bool propagateDepth = false);
    std::string replace(const std::string& str, const Response& response, int depth = 0);
};


#endif //SEMESTRALKA_DOWNLOADLINKREPLACER_H
