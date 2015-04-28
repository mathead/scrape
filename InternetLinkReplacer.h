//
// Created by Ja on 4/27/2015.
//

#ifndef SEMESTRALKA_INTERNETLINKREPLACER_H
#define SEMESTRALKA_INTERNETLINKREPLACER_H

#include "LinkReplacer.h"
class Scraper;

class InternetLinkReplacer : public LinkReplacer {
public:
    InternetLinkReplacer(Scraper *scraper);
    std::string replace(const std::string& str, const Response& response, int depth = 0);
};


#endif //SEMESTRALKA_INTERNETLINKREPLACER_H
