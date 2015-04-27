#include <iostream>
#include "Downloader.h"

using namespace std;

int main() {
    Downloader d("google.com", true);
    d.download("/");

    return 0;
}