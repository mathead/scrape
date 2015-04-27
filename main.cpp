#include <iostream>
#include "Downloader.h"
#include "Response.h"
#include "Scraper.h"

using namespace std;

int main() {
    // Downloader d("cvut.cz", true);
    // Response r = d.download("/");
    // r.writeFile("out.html");

	Scraper s(true);
	s.scrape("cvut.cz/vitejte-na-cvut", 5);

    return 0;
}