#ifndef SEMESTRALKA_SCRAPER_H
#define SEMESTRALKA_SCRAPER_H

#include <list>
#include <memory>
#include "Downloader.h"
#include "Response.h"
#include "LinkFinder.h"

typedef std::unique_ptr<LinkFinder> linkFinderPtr;

/**
 * Struct for queuing what to download next and to what depth
 */
struct DFile {
    std::string url;
    int depth;
    DFile(const std::string& url, int depth) : url(url), depth(depth) {}
};

/**
 * The main class that manages the whole workflow - downloading, queueing and saving.
 * It enables most of the configurability of the application. Typically only one instance is created.
 */
class Scraper {
    /// list of LinkFinders that are all applied to every page
    std::list<linkFinderPtr> linkFinders;
    /// list of LinkFinders that are all applied to every page when the depth reaches zero (the last depth) -
    /// the behaviour usually differs from normal downloading
    std::list<linkFinderPtr> linkFindersDepth0;
    /// list of strings that have to be found in the url for it to be downloaded
    std::list<std::string> filters;
    /// list of strings that can't be found in the url for it to be downloaded
    std::list<std::string> antifilters;
    /// the main queue of pages to be downloaded
    std::list<DFile> toDownload;
    /// the last suffix id of file saved
    int fileNum;
    /// member variables for the status line
    int startDepth, lastDepth, lastDepthDownloaded;
    /// whether we should download only pages from the starting server
    bool stayOnServer;
    /// for additional output - propagated to all other objects
    bool verbose;
    /// flags if the missing page and files directory have been created
    bool missingCreated, filesCreated;
    /// name of the main file to be downloaded (index.html)
    std::string indexName;
    /// the starting server
    std::string startServer;

    /**
     * Prints out a helpful status line with information about the download progress
     */
    void updateStatusLine(const std::string& url, int depth);
public:
    /// map of paths on disk to pages that have already been downloaded
    std::map<std::string, std::string> downloaded;
    /// the absolute path to files directory
    std::string filesPath;
    /// name of the additional files directory
    std::string filesDir;

    /**
     * Configures the scraper and gets it ready for downloading
     * @param filters list of strings that have to be found in the url for it to be downloaded
     * @param antifilters list of strings that can't be found in the url for it to be downloaded
     * @param indexName name of the main file to be downloaded (index.html)
     * @param filesDir name of the directory where additional pages get downloaded
     * @param stayOnServer whether we should download only pages from the starting server
     * @param verbose for additional output - propagated to all other objects
     * @param downloadImages whether we should download images or point to them on the internet
     * @param downloadExtras whether we should download css, js and favicon files or point to them on the internet (downloading these usually breaks the site)
     * @param lastMissing whether the last web page links should point to a missing page or to the original URL
     */
    Scraper(const std::list<std::string>& filters = {}, const std::list<std::string>& antifilters = {}, const std::string& indexName = "index.html",
            const std::string& filesDir = "files", bool stayOnServer = false, bool verbose = false, bool downloadImages = false, 
            bool downloadExtras = false, bool lastMissing = false);

    /**
     * Start scraping the specified URL
     * @param url what to scrape
     * @param depth to what depth
     * @return if the first (main) download was successful
     */
    bool scrape(const std::string& url, int depth, bool first = true);

    /**
     * Enqueue an URL to be downloaded to some depth
     * @param url what to be downloaded
     * @param suffix with what suffix the file should be saved
     * @depth to what depth
     * @return link to the file on disk
     */
    std::string enqueueDownload(const std::string& url, const std::string& suffix, int depth);

    /**
     * Get path to the missing page (gets created the first time called)
     */
    std::string getMissingPage();

    /**
     * Get path to the additional files directory (gets created the first time called)
     */
    std::string getFilesPath();

    /**
     * Get absolute path to a relative file/directory
     */
    static std::string getAbsPath(std::string file);
};


#endif //SEMESTRALKA_SCRAPER_H
