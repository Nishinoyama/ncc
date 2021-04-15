#!/bin/bash
assert() {
    expected="$1"
    input="$2"

    ./ncc "$input" > tmp.s
    cc -o tmp tmp.s
    ./tmp
    actual="$?"

    if [ "$actual" = "$expected" ]; then
        echo "$input => $actual"
    else
        echo "$input => $expected expected, but got $actual."
        exit 1
    fi
}

assert 0 0
assert 42 42
assert 255 255

assert 3 1+2
assert 2 3-1
assert 5 14-12+3
assert 1 "12 - 11"
assert 0 "1+21  +    2   -   13   +   4   - 15"
assert 0 "42 + 35 - 77"

echo OK
