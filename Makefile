.PHONY: all
TARGETS = geninvert invert2index
all: $(TARGETS)
CPPFLAGS = --std=c++11 -g -ggdb -Wall

geninvert: geninvert.cpp

invert2file: invert2index.cpp

clean:
	rm $(TARGETS)
