#include <iostream>
#include <sstream>
#include <unistd.h>
#include "Downloader.h"
#include "Response.h"
#include "Scraper.h"

using namespace std;

void usage() {
	// help message
	cout << "Usage: scrape [-F <path>] [-D <path>] [-d <depth>] [-f <filters>] [-a <antifilters>] [-ieshv] <URL>" << endl;
}

int main(int argc, char **argv) {
	// default options
	bool verbose = false;
	bool stay_on_server = false;
	bool images = false;
	bool extras = false;
	bool missing = false;
	string output_file = "index.html";
	string output_dir = "files";
	string filter = "";
	string antifilter = "";
	int depth = 0;
	int c, d;

	opterr = 0;

	// parse the command line options with getopt
	while ((c = getopt(argc, argv, ":vsF:D:d:f:a:iemh")) != -1) {
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
			case 'i':
				images = true;
				break;
			case 'e':
				extras = true;
				break;
			case 'm':
				missing = true;
				break;
			case 'h':
				cout << "scrape - tool for downloading web pages" << endl;
				usage();
				return 0;
				break;
			case '?':
				cerr << "Unknown option '-" << (char) optopt << "'." << endl;
				usage();
				return 1;
			case ':':
				cerr << "Option '" << (char) optopt << "' requires an argument." << endl;
				usage();
				return 1;
		}
	}

	if (optind >= argc) {
		cerr << "No URL to scrape." << endl;
		usage();
		return 1;
	}

	if (optind < argc - 1) {
		cerr << "Too many arguments." << endl;
		usage();
		return 1;
	}

	// split the filters by commas
	list<string> filters;
	list<string> antifilters;
	istringstream iss(filter);
	string item;
	while (getline(iss, item, ','))
		filters.push_back(item);
	istringstream aiss(antifilter);
	while (getline(aiss, item, ','))
		antifilters.push_back(item);


	// setup the scraper with collected options
	Scraper s(filters, antifilters, output_file, output_dir, stay_on_server, verbose, images, extras, missing);
	// and run it!
	bool success = s.scrape(argv[argc - 1], depth);
	cout << endl;

    return !success;
}