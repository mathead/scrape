#include "HrefLinkFinder.h"
using namespace std;

HrefLinkFinder::HrefLinkFinder(LinkReplacer *linkReplacer) : LinkFinder(linkReplacer) { }

void HrefLinkFinder::find(Response &response, int depth) {
    size_t startpos = 0, start, end;
    while (findTagAttr("<a ", "href=\"", response, startpos, start, end)) {
        string rep = linkReplacer->replace(response.content.substr(start, end - start), response, depth);
        response.content.replace(start, end - start, rep);
    }
}
