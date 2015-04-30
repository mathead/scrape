all: compile doc

compile:
	mkdir -p hlavamat
	g++ -Wall -pedantic -Wno-long-long -ggdb -std=c++11 src/*.cpp -o hlavamat/scrape

run: ./hlavamat/scrape 
	./hlavamat/scrape

doc:
	mkdir -p doc
	doxygen src/Doxyfile

clean:
	$(RM) -r doc hlavamat