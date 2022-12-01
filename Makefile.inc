TARGETS=$(patsubst %.cc,%,$(wildcard *.cc))
export CXXFLAGS=-Wall -Werror -pedantic -O2 --std=c++20 -I../utilities

all: $(TARGETS)

clean:
	rm *.o $(TARGETS) || true
