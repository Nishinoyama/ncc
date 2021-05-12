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

assert 0 "0;"
assert 42 "42;"
assert 255 "255;"

assert 3 "1+2;"
assert 2 "3-1;"
assert 5 "14-12+3;"
assert 1 "12 - 11;"
assert 0 "1+21  +    2   -   13   +   4   - 15;"

assert 7 "1 + 2 * 3;"
assert 5 "(1 + 2) * (3 / 4) + 5;"
assert 9 "(1 + 2) * ((3 / 4) + 5) - 6;"

assert 9 "+ 3 + 6;"
assert 6 "- 3 * 6 + 24;"

assert 0 "1 <= 0;"
assert 1 "0 <= 0;"

assert 1 "-1 < 0;"
assert 0 "0 < 0;"

assert 1 "0 >= 0;"
assert 0 "-1 >= 0;"

assert 1 "1 > 0;"
assert 0 "0 > 0;"

assert 1 "0 == 0;"
assert 0 "1 == 0;"

assert 0 "0 != 0;"
assert 1 "1 != 0;"


assert 1 "((1+2)+3 == 1+(2+3)) == ((1*2)*3 == 1*(2*3));"
assert 1 "((1+2)*3 == 9) == (1+(2*3) > 6);"

assert 1 "1+2; 3-2;"

assert 1 "a = 1; a;"
assert 2 "a = 1; b = a; a + b;"
assert 13 "a = 1; b = 1; c = b + a; d = c + b; e = d + c; f = e + d; g = f + e;"

assert 42 "answer_to_the_ultimate_question_of_life_the_universe_and_everything_is_42 = 42; answer_to_the_ultimate_question_of_life_the_universe_and_everything_is_42;"
assert 54 "__SIX__ = 6; __NINE__ = 9; __SIX__ * __NINE__;"

echo OK
