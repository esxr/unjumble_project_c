# This is the default target, which will be built when 
# you invoke make
.PHONY: all
all: unjumble

# This rule tells make how to build unjumble from unjumble.cpp
unjumble: unjumble.c
	gcc -o unjumble unjumble.c

# This rule tells make to copy unjumble to the binaries subdirectory,
# creating it if necessary
.PHONY: install
install:
	mkdir -p binaries
	cp -p unjumble binaries

# This rule tells make to delete unjumble and unjumble.o
# .PHONY: clean 
# clean:
# 	rm -f unjumble
