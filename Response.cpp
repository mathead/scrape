//
// Created by Ja on 4/27/2015.
//

#include "Response.h"
#include <sstream>
#include <iostream>

using namespace std;

Response::Response(string response, bool verbose) : ok(false), moved(false), fail(true), verbose(verbose) {
    if (verbose)
        cout << ">>> Parsing response:" << endl << response << endl << "------------------" << endl << endl;

    // parse response in member fields
    string line;
    istringstream responsess(response); // read response line by line

    // protocol & status
    if (!getline(responsess, line)) return;

    istringstream liness(line);
    liness.ignore(20, ' '); // ignore the HTTP/1.0 bit
    liness >> status;
    if (!liness.good()) return;
    if (status >= 200 && status < 300)
        ok = true;

    // headers
    while (getline(responsess, line)) {
        if (line[line.length() - 1] == 13) // handle CRLF
            line.resize(line.length() - 1);
        
        if (!line.length())
            break; // end of headers

        istringstream headerss(line);
        string name, header_content;
        if (!getline(headerss, name, ':')) return; // wrong header format
        headerss.ignore(1);
        if (!getline(headerss, header_content)) return;
        headers[name] = header_content;
    }

    // content
    getline(responsess, content, '\0');

    if (status >= 300 && status < 400 && headers.count("Location"))
        moved = true;

    fail = false;
}
