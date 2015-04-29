//
// Created by Ja on 4/27/2015.
//

#ifndef SEMESTRALKA_MISSINGLINKREPLACER_H
#define SEMESTRALKA_MISSINGLINKREPLACER_H

#include "InternetLinkReplacer.h"
class Scraper;

class MissingLinkReplacer : public InternetLinkReplacer {
public:
    MissingLinkReplacer(Scraper *scraper);
    std::string replace(const std::string& str, const Response& response, int depth = 0);
};


#endif //SEMESTRALKA_MISSINGLINKREPLACER_H
