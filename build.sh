#!/bin/bash
doxygen Doxyfile
rm -rf ../qinzhengke.github.io/*
cp -r build/html/* ../qinzhengke.github.io/
