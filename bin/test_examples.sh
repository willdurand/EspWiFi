#!/usr/bin/env bash

CURRENT_DIR=`pwd`

for example in $(ls examples/)
do
    echo "---------------------------------------------------------------------------------"
    echo " Building: $example"
    echo "---------------------------------------------------------------------------------"

    example_dir="examples/$example/"

    cp Makefile _Makefile.master "$example_dir"
    cd "$example_dir"
    make
    cd "$CURRENT_DIR"
done
