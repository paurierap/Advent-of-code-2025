# Advent of Code 2025

Repository with the solutions for this year's edition. Every directory contains the solutions to both parts for every day.

## Usage

For some days, just save an input file `input.txt` in the corresponding directory and create an executable from `main.cpp` with your compiler of choice, e.g.
```bash
g++ main.cpp -o main
```
and run the executable from the command line as
```bash
./main input.txt
```

In most cases, where a newer C++ standard is required, a Makefile is provided. Therefore, to create the executable, just `make` it
```bash
make
```