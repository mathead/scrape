//
// Created by Ja on 4/27/2015.
//

#include "LinkFinder.h"

LinkFinder::LinkFinder(LinkReplacer* linkReplacer) : linkReplacer(linkReplacer) {}

LinkFinder::~LinkFinder() { delete linkReplacer; }