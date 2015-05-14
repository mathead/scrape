CXX=g++
CFLAGS=-Wall -pedantic -Wno-long-long -ggdb -O0 -std=c++11 
SRC=$(wildcard src/*.cpp)
OBJ=$(SRC:.cpp=.o)

all: compile doc

compile: $(OBJ)
	mkdir -p hlavamat
	$(CXX) $(CFLAGS) -o hlavamat/scrape $^

run: ./hlavamat/scrape 
	./hlavamat/scrape

doc:
	mkdir -p doc
	doxygen src/Doxyfile

clean:
	$(RM) -r doc hlavamat src/*.o

CSSLinkFinder.o: src/CSSLinkFinder.cpp src/CSSLinkFinder.h \
 src/LinkFinder.h src/LinkReplacer.h src/Response.h
DownloadLinkReplacer.o: src/DownloadLinkReplacer.cpp \
 src/DownloadLinkReplacer.h src/InternetLinkReplacer.h src/LinkReplacer.h \
 src/Response.h src/Scraper.h src/Downloader.h src/LinkFinder.h
Downloader.o: src/Downloader.cpp src/Downloader.h src/Response.h
FaviconLinkFinder.o: src/FaviconLinkFinder.cpp src/FaviconLinkFinder.h \
 src/LinkFinder.h src/LinkReplacer.h src/Response.h
HrefLinkFinder.o: src/HrefLinkFinder.cpp src/HrefLinkFinder.h \
 src/LinkFinder.h src/LinkReplacer.h src/Response.h
ImageLinkFinder.o: src/ImageLinkFinder.cpp src/ImageLinkFinder.h \
 src/LinkFinder.h src/LinkReplacer.h src/Response.h
InternetLinkReplacer.o: src/InternetLinkReplacer.cpp \
 src/InternetLinkReplacer.h src/LinkReplacer.h src/Response.h
JSLinkFinder.o: src/JSLinkFinder.cpp src/JSLinkFinder.h src/LinkFinder.h \
 src/LinkReplacer.h src/Response.h
LinkFinder.o: src/LinkFinder.cpp src/LinkFinder.h src/LinkReplacer.h \
 src/Response.h
LinkReplacer.o: src/LinkReplacer.cpp src/LinkReplacer.h src/Response.h \
 src/Scraper.h src/Downloader.h src/LinkFinder.h
MissingLinkReplacer.o: src/MissingLinkReplacer.cpp \
 src/MissingLinkReplacer.h src/InternetLinkReplacer.h src/LinkReplacer.h \
 src/Response.h src/Scraper.h src/Downloader.h src/LinkFinder.h
Response.o: src/Response.cpp src/Response.h
Scraper.o: src/Scraper.cpp src/Scraper.h src/Downloader.h src/Response.h \
 src/LinkFinder.h src/LinkReplacer.h src/InternetLinkReplacer.h \
 src/MissingLinkReplacer.h src/DownloadLinkReplacer.h \
 src/HrefLinkFinder.h src/ImageLinkFinder.h src/CSSLinkFinder.h \
 src/FaviconLinkFinder.h src/JSLinkFinder.h
main.o: src/main.cpp src/Downloader.h src/Response.h src/Scraper.h \
 src/LinkFinder.h src/LinkReplacer.h

%.o: %.cpp
	$(CXX) $(CFLAGS) -c $< -o $@