//
// Created by Ja on 4/27/2015.
//

#include "MissingLinkReplacer.h"
#include "Scraper.h"
#include <fstream>
#include <iostream>
using namespace std;

MissingLinkReplacer::MissingLinkReplacer(Scraper *scraper) : InternetLinkReplacer(scraper) { }

string MissingLinkReplacer::replace(const string &str, const Response& response, int depth) {
    string link = InternetLinkReplacer::replace(str, response);
    if (scraper->downloaded.count(link))
        return "file://" + scraper->downloaded[link];

    return scraper->getMissingPage();
}
