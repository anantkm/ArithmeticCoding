all: aencode adecode

aencode: aencode.cpp
	clang++ -o aencode aencode.cpp -lmpfr -lgmp

adecode: adecode.cpp
	clang++ -o adecode adecode.cpp -lmpfr -lgmp
