#include <iostream>
#include <unistd.h>
#include "Downloader.h"
#include "Response.h"
#include "Scraper.h"

using namespace std;

int main(int argc, char **argv) {
    // Downloader d("cvut.cz", true);
    // Response r = d.download("/documents/10552/2e52fe20-e64e-4951-9199-6a35a8496302");
    // r.writeFile("out.pdf");

	bool verbose = false;
	string output_file = "index.html";
	string output_dir = "files";
	int depth = 0;
	int c, d;

	opterr = 0;

	while ((c = getopt(argc, argv, ":vf:o:d:")) != -1) {
		switch (c) {
			case 'v':
				verbose = true;
				break;
			case 'f':
				output_file = optarg;
				break;
			case 'o':
				output_dir = optarg;
				break;
			case 'd':
				try {
					d = stoi(optarg);
				} catch (invalid_argument) {
					cerr << "Argument for '-d' (depth) has to be a number." << endl;
					return 1;
				}

				if (d < 0) {
					cerr << "Argument for '-d' (depth) can't be negative." << endl;
					return 1;
				}

				depth = d;
				break;
			case '?':
				cerr << "Unknown option '-" << (char) optopt << "'." << endl;
				return 1;
			case ':':
				cerr << "Option '" << (char) optopt << "' requires an argument." << endl;
				return 1;
		}
	}

	if (optind >= argc) {
		cerr << "No URL to scrape." << endl;
		return 1;
	}

	if (optind < argc - 1) {
		cerr << "Too many arguments." << endl;
		return 1;
	}

	Scraper s(output_file, output_dir, verbose);
	bool success = s.scrape(argv[argc - 1], depth);
	cout << endl;

    return !success;
}