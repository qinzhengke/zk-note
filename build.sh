#!/bin/bash

ls -d ./build/html/* | grep -E -v '(MathJax)|(figures)' | xargs rm -rf

echo "[zk] Building html ..."
doxygen Doxyfile

if [ $# -gt 0 ]
then 

    if [ $1 == '--all' ]
    then
        ls -d ./build/html/* | grep -E '(MathJax)|(figures)' | xargs rm -rf

        echo "[zk] Copying images ..."
        cp -r figures build/html/
        echo "[zk] Copying MathJax ..."
        cp -r ./3rd-party/MathJax-2.7.8 ./build/html/
    fi

    if [ $1 == '--release' ]
    then
        echo "[zk] Relesing output to ../qinzhengke.github.io ..."
        rm -rf ../qinzhengke.github.io/*
        cp -r build/html/* ../qinzhengke.github.io/
    fi

fi

echo "[zk] All done!"
