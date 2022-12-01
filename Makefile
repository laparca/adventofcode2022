DIRS=$(wildcard day*)
export CXXFLAGS=-Wall -Werror -pedantic -O2 --std=c++20 -I$(shell pwd)/utilities

.PHONY: all clean $(DIRS)

all: ACTION=all
all: $(DIRS)

clean: ACTION=clean
clean: $(DIRS)

$(DIRS):
	$(MAKE) -C $@ $(ACTION)
