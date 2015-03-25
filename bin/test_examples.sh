#!/usr/bin/env bash

CURRENT_DIR=`pwd`

for example in $(ls examples/)
do
    example_dir="examples/$example/"

    if [ -d "$example_dir" ] ; then
        echo "---------------------------------------------------------------------------------"
        echo " Building: $example"
        echo "---------------------------------------------------------------------------------"

        cp examples/Makefile examples/_Makefile.master "$example_dir"
        cd "$example_dir"
        make
        cd "$CURRENT_DIR"
    fi
done
