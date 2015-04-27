#include <iostream>
#include "Downloader.h"

using namespace std;

int main() {
    Downloader d("http://www.jmarshall.com/");
    d.download("/");

    return 0;
}