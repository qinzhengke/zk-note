#!/bin/bash
doxygen Doxyfile
rm -rf build/html/figures
cp -r figures build/html/
