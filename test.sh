#!/bin/bash
assert() {
    expected="$1"
    input="$2"

    ./ncc "$input" > tmp.s
    cc -o tmp tmp.s meta_func.o
    ./tmp
    actual="$?"

    if [ "$actual" = "$expected" ]; then
        echo "$input => $actual"
    else
        echo "$input => $expected expected, but got $actual."
        exit 1
    fi
}

assert_main() {
    expected="$1"
    input="int main(){$2}"
    assert "$expected" "$input"
}


#assert_main 0 "0;"
#assert_main 42 "42;"
#assert_main 255 "255;"
#
#assert_main 3 "1+2;"
#assert_main 2 "3-1;"
#assert_main 5 "14-12+3;"
#assert_main 1 "12 - 11;"
#assert_main 0 "1+21  +    2   -   13   +   4   - 15;"
#
#assert_main 7 "1 + 2 * 3;"
#assert_main 5 "(1 + 2) * (3 / 4) + 5;"
#assert_main 9 "(1 + 2) * ((3 / 4) + 5) - 6;"
#
#assert_main 9 "+ 3 + 6;"
#assert_main 6 "- 3 * 6 + 24;"
#
#assert_main 0 "1 <= 0;"
#assert_main 1 "0 <= 0;"
#
#assert_main 1 "-1 < 0;"
#assert_main 0 "0 < 0;"
#
#assert_main 1 "0 >= 0;"
#assert_main 0 "-1 >= 0;"
#
#assert_main 1 "1 > 0;"
#assert_main 0 "0 > 0;"
#
#assert_main 1 "0 == 0;"
#assert_main 0 "1 == 0;"
#
#assert_main 0 "0 != 0;"
#assert_main 1 "1 != 0;"
#
#
#assert_main 1 "((1+2)+3 == 1+(2+3)) == ((1*2)*3 == 1*(2*3));"
#assert_main 1 "((1+2)*3 == 9) == (1+(2*3) > 6);"
#
#assert_main 1 "1+2; 3-2;"
#
#assert_main 1 "a = 1; a;"
#assert_main 2 "a = 1; b = a; a + b;"
#assert_main 13 "a = 1; b = 1; c = b + a; d = c + b; e = d + c; f = e + d; g = f + e;"
#
#assert_main 42 "answer_to_the_ultimate_question_of_life_the_universe_and_everything_is_42 = 42; answer_to_the_ultimate_question_of_life_the_universe_and_everything_is_42;"
#assert_main 54 "__SIX__ = 6; __NINE__ = 9; __SIX__ * __NINE__;"
#
#assert_main 16 "return 16;"
#assert_main 7 "return 1+6;"
#assert_main 7 "return 3+4; x = 123; y = 123; return x + y;"
#assert_main 11 "a = 3; b = 4; a = b + a; return a + b;"
#
#assert_main 3 "if(1) return 3; return 2;"
#assert_main 2 "if(0) return 3; return 2;"
#assert_main 10 "a = 100; a = a - 80; if( a > 10 ) a = 10; if( a < 0 ) a = 0; return a;"
#
#assert_main 5 "if(0) return 3; else if(0) return 4; else if(1) return 5; else return 6;"
#assert_main 3 "x = 1; y = 0; if(x) if(y) y = 4; else y = 3; return y;"
#assert_main 0 "x = 0; y = 0; if(x) if(y) y = 4; else y = 3; return y;"
#
#assert_main 100 "x = 0; while(x < 100) x = x + 1; return x;"
#
#assert_main 100 "for(x = 0; x < 100; x = x + 1) x + 1; return x;"
#assert_main 45 "y = 0; for(x = 0; x < 10; x = x + 1) y = x + y; return y;"
#
#assert_main 9 "{ x = 3; y = 3; z = 3; } return x + y + z;"
#assert_main 55 "x = 0; y = 1; for(i = 0; i < 10; i = i + 1){ y = x + y; x = y - x; } return x;"
#assert_main 20 "x = 0; y = 0; z = 0; while(z < 20) { x = x + 1; if( x > 1000 ) { x = 0; y = y + 1; } if( y > 1000 ) { y = 0; z = z + 1; } } return z;"
#assert_main 106 "x = 0; for( i = 0; i < 100000; i = i + 1 ) { for( j = 0; j < 1000; j = j + 1 ) { x = x + 1; if( x == 314 ) x = 0; } } return x;"
#
#assert_main 0 "foo(); return foo();"
#assert_main 4 "a = 1; return a + return_3_function();"
#assert_main 1 "a = 4; return bar(bar(4, 1), bar(a, 2));"
#assert_main 5 "pi = barbar(3,1,4,1,5,9); e = barbar(2,7,1,8,2,8); return e - pi;"
#
#assert 1 "main(){ return 1; }"
#assert 2 "main(){ x = 3; y = 5; if (x > y) { return x - y; } else { return y - x; } }"
#assert 6 "ncc_func(){ a = 3; return a; } ncc_func2(){ a = 2; return a; } main(){ a = 1; return a * ncc_func() * ncc_func2(); }"
#assert 1 "main(){ a = 4; return bar(bar(4, 1), bar(a, 2)); }"

assert 3 "int sub(int a, int b){ return a - b; } int main(){ return sub(7, 4); }"
assert 21 "int summer(int a, int b, int c, int d, int e, int f){ return a + b + c + d + e + f; } int main() { return summer(1, 2, 3, 4, 5, 6); }"
assert 55 "int fib(int n){ if ( n < 2 ) return n; else return fib(n-2) + fib(n-1); } int main(){ return fib(10); }"

assert_main 3 "int x; x = 3; int y; y = &x; return *y;"

echo OK
