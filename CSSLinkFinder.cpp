#include "CSSLinkFinder.h"
using namespace std;

CSSLinkFinder::CSSLinkFinder(LinkReplacer *linkReplacer) : LinkFinder(linkReplacer) {}

void CSSLinkFinder::find(Response &response, int depth) {
    size_t startpos = 0, start, end;
    while(findTagAttr("<link ", "href=\"", response, startpos, start, end)) {
        string rep = linkReplacer->replace(response.content.substr(start, end - start), response, depth);
        response.content.replace(start, end - start, rep);
    }
}