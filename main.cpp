#include <iostream>
#include "Downloader.h"
#include "Response.h"

using namespace std;

int main() {
    Downloader d("cvut.cz", true);
    Response r = d.download("/");
    r.writeFile("out.html");

    return 0;
}