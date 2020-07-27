#!/bin/bash
doxygen Doxyfile
rm -rf build/html/figures
cp -r figures build/html/
rm -rf ../qinzhengke.github.io/*
cp -r build/* ../qinzhengke.github.io/
