#include <iostream>
#include "Downloader.h"
#include "Response.h"
#include "Scraper.h"

using namespace std;

int main() {
    // Downloader d("cvut.cz", true);
    // Response r = d.download("/documents/10552/2e52fe20-e64e-4951-9199-6a35a8496302");
    // r.writeFile("out.pdf");

	Scraper s;
	s.scrape("dontclick.it", 2);
	cout << endl;

    return 0;
}