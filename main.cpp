#include <iostream>
#include <sstream>
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
	bool stay_on_server = false;
	string output_file = "index.html";
	string output_dir = "files";
	string filter = "";
	string antifilter = "";
	int depth = 0;
	int c, d;

	opterr = 0;

	while ((c = getopt(argc, argv, ":vsF:D:d:f:a:")) != -1) {
		switch (c) {
			case 'v':
				verbose = true;
				break;
			case 's':
				stay_on_server = true;
				break;
			case 'F':
				output_file = optarg;
				break;
			case 'D':
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
			case 'f':
				filter = optarg;
				break;
			case 'a':
				antifilter = optarg;
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

	list<string> filters;
	list<string> antifilters;
	istringstream iss(filter);
	string item;
	while (getline(iss, item, ','))
		filters.push_back(item);
	istringstream aiss(antifilter);
	while (getline(aiss, item, ','))
		antifilters.push_back(item);


	Scraper s(filters, antifilters, output_file, output_dir, stay_on_server, verbose);
	bool success = s.scrape(argv[argc - 1], depth);
	cout << endl;

    return !success;
}