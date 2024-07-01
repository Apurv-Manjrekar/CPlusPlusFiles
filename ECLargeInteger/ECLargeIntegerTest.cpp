// Test ECLargeInteger. To build: g++ ECLargeInteger.cpp ECLargeIntegerTest.cpp -std=c++11
#include "ECLargeInteger.h"
#include <iostream>
using namespace std;

// simple addition
static void Test1()
{
    ECLargeInteger x(3), y(2); //134 + 32
    x.SetDigit(0, 1);
    x.SetDigit(1, 3);
    x.SetDigit(2, 4);
    y.SetDigit(0, 3);
    y.SetDigit(1, 2);
    ECLargeInteger s = x+y;
    cout << "Sum of x and y: ";
    // should dump 166
    s.Dump();
    cout << endl;
}

// addition with carry
static void Test2() //189 + 47
{
    ECLargeInteger x(3), y(2);
    x.SetDigit(0, 1);
    x.SetDigit(1, 8);
    x.SetDigit(2, 9);
    y.SetDigit(0, 4);
    y.SetDigit(1, 7);
    ECLargeInteger s = x+y;
    cout << "Sum of x and y: ";
    // should dump 236
    s.Dump();
    cout << endl;
}

// addition with carry, more tests
static void Test3()
{
    ECLargeInteger x(3), y(2); // 989 + 47 
    x.SetDigit(0, 9);
    x.SetDigit(1, 8);
    x.SetDigit(2, 9);
    y.SetDigit(0, 4);
    y.SetDigit(1, 7);
    ECLargeInteger s = x+y;
    cout << "Sum of x and y: ";
    // should dump 1036
    s.Dump();
    cout << endl;
}

// test subtraction 
static void Test3a()
{
    ECLargeInteger x(3), y(2);
    x.SetDigit(0, 9);
    x.SetDigit(1, 8);
    x.SetDigit(2, 9);
    y.SetDigit(0, 4);
    y.SetDigit(1, 7);
    ECLargeInteger s = x-y;
    cout << "Subtraction of x and y: ";
    // should dump 942
    s.Dump();
    cout << endl;
}

// test subtraction: borrowing
static void Test3b()
{
    ECLargeInteger x(3), y(2);
    x.SetDigit(0, 1);
    x.SetDigit(1, 0);
    x.SetDigit(2, 3);
    y.SetDigit(0, 4);
    y.SetDigit(1, 5);
    ECLargeInteger s = x-y;
    cout << "Subtraction of x and y: ";
    // should dump 58
    s.Dump();
    cout << endl;
}

// simple multiply
static void Test4()
{
    ECLargeInteger x(3), y(2);
    x.SetDigit(0, 1);
    x.SetDigit(1, 2);
    x.SetDigit(2, 1);
    y.SetDigit(0, 2);
    y.SetDigit(1, 1);
    // should be: 2541
    ECLargeInteger p = x*y;
    cout << "Product of x and y: ";
    p.Dump();
    cout << endl;
}

// more multiply
static void Test5()
{
    ECLargeInteger x(3), y(3);
    x.SetDigit(0, 7);
    x.SetDigit(1, 3);
    x.SetDigit(2, 4);
    y.SetDigit(0, 8);
    y.SetDigit(1, 2);
    y.SetDigit(2, 9);
    // should be: 608486
    ECLargeInteger p = x*y;
    cout << "Product of x and y: ";
    p.Dump();
    cout << endl;

    // now multipy by a zero, should get a zero
    ECLargeInteger z(1);
    z.SetDigit(0,0);
    ECLargeInteger p2 = x*z;
    cout << "product of x and zero: should be zero: did you get it? ";
    p2.Dump();
    cout << endl;
}

// a longer multiplication: you probably won't be able to do this multiply on a calculator!
static void Test6()
{
    ECLargeInteger x(100), y(100);
    // let x = y = 10^100-1 (i.e., 100 digits of 9)
    for(int i=0; i<100; ++i)
    {
        x.SetDigit(i, 9);
        y.SetDigit(i, 9);
    }
    // should be: 10^200 - 2*10^100 +1
    // that is, it starts with 99 digit 9s and then 8, and then 99 digit 0 and end with a 1
    ECLargeInteger p = x*y;
    cout << "Product of x and y: ";
    p.Dump();
    cout << endl;
}

// test division
static void Test7()
{
    ECLargeInteger x(2), y(2), r(1); //734 / 29
    x.SetDigit(0, 7);
    x.SetDigit(1, 3);
    //x.SetDigit(2, 4);
    y.SetDigit(0, 7);
    y.SetDigit(1, 3);
    // should be: quotient = 25, remainder=9
    ECLargeInteger q = x.DividedBy(y, r);
    cout << "Quotient of x divided by y: ";
    q.Dump();
    cout << endl;
    cout << "Remainder: ";
    r.Dump();
    cout << endl;
}

// Fermat's test
static void Test8()
{
    // Use wikipedia example n=221: https://en.wikipedia.org/wiki/Fermat_primality_test
    ECLargeInteger x(3), y(2), z(2);
    x.SetDigit(0, 2);
    x.SetDigit(1, 2);
    x.SetDigit(2, 1);
    y.SetDigit(0, 3);
    y.SetDigit(1, 8);
    z.SetDigit(0, 2);
    z.SetDigit(1, 4);
    bool fy = x.FermatTest(y);
    if( fy )
    {
        cout << "Possibly a prime: ";
    }
    else
    {
        cout << "*** NOT a prime: ";
    }
    cout << " by witness ";
    y.Dump();
    cout << endl;

    bool fz = x.FermatTest(z);
    // remainder should be 81
    if( fz )
    {
        cout << "Possibly a prime: ";
    }
    else
    {
        cout << "**** NOT a prime: ";
    }
    cout << " by witness ";
    z.Dump();
    cout << endl;
}
// another test of Fermat's: from: https://kconrad.math.uconn.edu/blurbs/ugradnumthy/fermattest.pdf
static void Test8a()
{
    // n= 415693, base=2
    ECLargeInteger x(6), y(1);
    x.SetDigit(0, 4);
    x.SetDigit(1, 1);
    x.SetDigit(2, 5);
    x.SetDigit(3, 6);
    x.SetDigit(4, 9);
    x.SetDigit(5, 3);
    y.SetDigit(0, 2);
    // remainder should be 58346
    ECLargeInteger rem(1);
    bool fy = x.FermatTest(y, &rem);
    if( fy )
    {
        cout << "Possibly a prime: ";
    }
    else
    {
        cout << "**** NOT a prime: ";
    }
    cout << " by witness ";
    y.Dump();
    cout << ", modulo remainder: ";
    rem.Dump();
    cout << endl;
}
// Yet another Fermat's test: 
static void Test8b()
{
    // Carmichael number: 8911
    ECLargeInteger x(4), y(3), z(3);
    x.SetDigit(0, 8);
    x.SetDigit(1, 9);
    x.SetDigit(2, 1);
    x.SetDigit(3, 1);
    y.SetDigit(0, 1);
    y.SetDigit(1, 1);
    y.SetDigit(2, 3);
    z.SetDigit(0, 2);
    z.SetDigit(1, 8);
    z.SetDigit(2, 3);
    // should all report to POSSIBLE PRIME
    bool fy = x.FermatTest(y);
    if( fy )
    {
        cout << "Possibly a prime: ";
    }
    else
    {
        cout << "**** YOUR CODE IS WRONG ";
    }
    cout << " by witness ";
    y.Dump();
    cout << endl;

    bool fz = x.FermatTest(z);
    if( fz )
    {
        cout << "Possibly a prime: ";
    }
    else
    {
        cout << "**** YOUR CODE IS WRONG ";
    }
    cout << " by witness ";
    z.Dump();
    cout << endl;
}

static void Test9()
{
    // n= 415693, base=2
    ECLargeInteger x(6), y(1);
    x.SetDigit(0, 4);
    x.SetDigit(1, 1);
    x.SetDigit(2, 5);
    x.SetDigit(3, 6);
    x.SetDigit(4, 9);
    x.SetDigit(5, 3);
    y.SetDigit(0, 2);
    // remainder should be 58346
    ECLargeInteger rem(1);
    // output: **** NOT a prime:  by witness 2 , modulo remainder: 58346

    // assert( x.FermatTest(y, &rem) == false );
    // assert(GetNumber(y) == 2);
    // assert(GetNumber(rem) == 58346);
    bool fz = x.FermatTest(y, &rem);
    if( fz )
    {
        cout << "**** YOUR CODE IS WRONG ";
    }
    else
    {
        cout << "not a prime";
    }
    cout << " by witness ";
    y.Dump();
    rem.Dump(); 
    cout << endl;

}

int main()
{
    Test1();
    Test2();
    Test3();
    Test3a();
    Test3b();
    Test4();
    Test5();
    Test6();
    Test7();
    Test8();
    Test8a();
    Test8b();
    Test9();
}
