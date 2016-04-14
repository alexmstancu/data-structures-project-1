#!/bin/bash

echo "Running tests ............................................."

num_tests=0
num_oks=0

for file in tests/*.bin
do
    file_nice=$(printf '%32s' "$file")
    ./$file
    if [ $? -eq 0 ]; then
        num_oks=$(($num_oks + 1))
        echo "Testing $file_nice .............. OK!"
    else
        echo "Testing $file_nice .......... FAILED!"
    fi

    num_tests=$(($num_tests + 1))
done

echo "$num_oks test(s) out of $num_tests test(s) passed successfully."
