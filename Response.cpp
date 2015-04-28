//
// Created by Ja on 4/27/2015.
//

#include "Response.h"
#include <sstream>
#include <fstream>
#include <iostream>

using namespace std;

Response::Response(const string& response, const string& server, bool verbose) :
        ok(false), moved(false), fail(true), verbose(verbose), server(server) {
   // if (verbose)
   //     cout << ">>> Parsing response:" << endl << response << endl << "------------------" << endl << endl;

    // parse response in member fields
    string line;
    istringstream responsess(response); // read response line by line

    // protocol & status
    if (!getline(responsess, line)) { 
        content = response;
        return; 
    }

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
    while (getline(responsess, line))
        content += line + '\n';
    if (content.length())
        content.pop_back();

    // insert utf-8 charset meta tag, as it gets lost in headers
    if (ok && headers.count("Content-Type") && headers["Content-Type"].find("html") != string::npos) {
        size_t pos = 0;
        if ((pos = content.find("<head>")) != string::npos)
            content.insert(pos + 6, "\n<meta http-equiv=\"Content-Type\" content=\"charset=UTF-8\">\n");
    }

    if (status >= 300 && status < 400 && headers.count("Location"))
        moved = true;

    fail = false;
}

void Response::writeFile(const string& path) {
    ofstream file(path, ios::binary);
    // file.write(content.c_str(), content.length());
    file << content;
    file.close();
}