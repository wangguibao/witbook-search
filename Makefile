
.PHONY: all
TARGETS = geninvert invert2index readind2 readindex
all: $(TARGETS)
CPPFLAGS = --std=c++11 -g -ggdb -Wall

geninvert: geninvert.cpp libutil.a
	g++ $(CPPFLAGS) -c -o geninvert.o $<
	g++ -lstdc++ -o $@ geninvert.o libutil.a

invert2file: invert2index.cpp invert2index.h

readind2: readind2.cpp	index.h

readindex: readindex.cpp index.h

libutil.a: util.o
	ar -rc libutil.a $^
clean:
	rm $(TARGETS) *.o *.a
