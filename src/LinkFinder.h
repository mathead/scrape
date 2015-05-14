#ifndef SEMESTRALKA_LINKFINDER_H
#define SEMESTRALKA_LINKFINDER_H

#include "LinkReplacer.h"
#include <list>

/**
 * Abstract class for finding various links in HTML and replacing them with LinkReplacers
 */
class LinkFinder {
protected:
    /// replaces the found strings
    LinkReplacer* linkReplacer;

    /**
     * Utility method for finding tags in HTML
     * @param tag the tag to search for
     * @param attribute attribute in tag  where the replacement gets done
     * @param response Response object with some useful information like headers
     * @param startpos starting point to search from
     * @param[out] start start of the attribute value in the response content
     * @param[out] end end of the attribute value in the response content
     * @param mandatory_attributes list of attributes the tag has to have to be selected
     * @return false if there aren't any tags left
     */
    bool findTagAttr(const std::string& tag, const std::string& attribute, Response& response, size_t& startpos,
                     size_t& start, size_t& end, std::list<std::string> mandatory_attributes = std::list<std::string>()) const;
public:
    /**
     * @param linkReplacer calls the LinkReplacers replace method on the found links
     */
    LinkFinder(LinkReplacer* linkReplacer);

    virtual ~LinkFinder();

    /**
     * Find and replace all the links in the response content
     * @param depth current depth
     */
    virtual void find(Response& response, int depth) = 0;
};


#endif //SEMESTRALKA_LINKFINDER_H
