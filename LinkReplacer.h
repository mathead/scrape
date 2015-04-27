//
// Created by Ja on 4/27/2015.
//

#ifndef SEMESTRALKA_LINKREPLACER_H
#define SEMESTRALKA_LINKREPLACER_H

#include <string>
#include "Scraper.h"
#include "Response.h"

class LinkReplacer {
protected:
    Scraper* scraper;
public:
    LinkReplacer(Scraper* scraper) : scraper(scraper) {}
    virtual ~LinkReplacer() {}
    virtual std::string replace(const std::string& str, const Response& response) = 0;
};


#endif //SEMESTRALKA_LINKREPLACER_H
