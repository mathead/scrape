#include <iostream>
#include "Downloader.h"

using namespace std;

int main() {
    Downloader d("google.com", true);
    d.download("/?gfe_rd=cr&ei=yhE-VdebBcyk8weEx4HwDQ");

    return 0;
}