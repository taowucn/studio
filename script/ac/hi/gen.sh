#!/bin/bash

# Gen configure.ascan -> configure.ac
#autoscan;

# Gen Makefile.in
aclocal; autoconf; autoheader; automake --add-missing

# Compile
./configure; make
