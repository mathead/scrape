#include "JSLinkFinder.h"
using namespace std;

JSLinkFinder::JSLinkFinder(LinkReplacer *linkReplacer) : LinkFinder(linkReplacer) {}

void JSLinkFinder::find(Response &response, int depth) {
    size_t startpos = 0, start, end;
    while(findTagAttr("<script ", "src=\"", response, startpos, start, end)) {
        string rep = linkReplacer->replace(response.content.substr(start, end - start), response, depth);
        response.content.replace(start, end - start, rep);
    }
}
