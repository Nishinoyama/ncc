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

assert 16 "return 16;"
assert 7 "return 1+6;"
assert 7 "return 3+4; x = 123; y = 123; return x + y;"
assert 11 "a = 3; b = 4; a = b + a; return a + b;"

assert 3 "if(1) return 3; return 2;"
assert 2 "if(0) return 3; return 2;"
assert 10 "a = 100; a = a - 80; if( a > 10 ) a = 10; if( a < 0 ) a = 0; return a;"

assert 5 "if(0) return 3; else if(0) return 4; else if(1) return 5; else return 6;"
assert 3 "x = 1; y = 0; if(x) if(y) y = 4; else y = 3; return y;"
assert 0 "x = 0; y = 0; if(x) if(y) y = 4; else y = 3; return y;"

assert 100 "x = 0; while(x < 100) x = x + 1; return x;"

assert 100 "for(x = 0; x < 100; x = x + 1) x + 1; return x;"
assert 45 "y = 0; for(x = 0; x < 10; x = x + 1) y = x + y; return y;"

echo OK
