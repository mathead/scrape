//
// Created by Ja on 4/27/2015.
//

#ifndef SEMESTRALKA_INTERNETLINKREPLACER_H
#define SEMESTRALKA_INTERNETLINKREPLACER_H


#include "LinkReplacer.h"

class InternetLinkReplacer : public LinkReplacer {
public:
    InternetLinkReplacer(Scraper *scraper) : LinkReplacer(scraper) { }
    std::string replace(const std::string& str, const Response& response);
};


#endif //SEMESTRALKA_INTERNETLINKREPLACER_H
