#!/usr/bin/env bash

for example in $(ls examples/)
do
    echo "---------------------------------------------------------------------------------"
    echo " Building: $example"
    echo "---------------------------------------------------------------------------------"

    cp Makefile _Makefile.master "examples/$example/"
    cd !$
    make
    cd -
done
