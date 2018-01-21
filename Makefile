.PHONY: all
TARGETS = geninvert invert2index readind2
all: $(TARGETS)
CPPFLAGS = --std=c++11 -g -ggdb -Wall

geninvert: geninvert.cpp

invert2file: invert2index.cpp

readind2: readind2.cpp

clean:
	rm $(TARGETS)
