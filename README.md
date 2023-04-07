# Arithmetic Coding

This is a command-line program that implements arithmetic coding for lossless data compression. The program consists of two parts: `aencode` and `adecode`.

## Getting Started

To use this program, follow these steps:

1. Clone the repository onto your local machine.
2. Compile the program using the provided `makefile`.
3. Run the `aencode` program with the input file as an argument. This will generate a compressed output file.
4. Run the `adecode` program with the compressed file as an argument. This will decode the compressed file and generate the original input file.

## Program Details

### `aencode`

The `aencode` program takes an input file and generates a compressed output file. It uses arithmetic coding to compress the data. The program calculates the probability and range values for each character in the input string and builds a look-up table for encoding. The compressed data is output to a file, along with the precision required to decode the data.

### `adecode`

The `adecode` program takes a compressed file and generates the original input file. It uses arithmetic decoding to decode the compressed data. The program reads the compressed data and the precision from the input file, and builds a look-up table for decoding. It then decodes the compressed data and outputs the original input file.

## Dependencies

This program requires the `mpfr` and `gmp` libraries to be installed on your machine.

## Usage

### `aencode`
./aencode [input file]

### `adecode`
./adecode [compressed file]

## Author

This program was written by Anant Mahale. 
