/*
Apurv Manjrekar
CSE 3150-011
PA 7
03/19/2024
*/

#ifndef EC_LARGE_INTEGER
#define EC_LARGE_INTEGER

#include <vector>


// ****************************************************************************
// Large integer: can have hundrends of digits
// Caution: don't include leading zeros (unless it is 1 digit zzero)
#include <cstddef>
using namespace std;

class ECLargeInteger
{
public:
    // Assume numDigits >= 1
    ECLargeInteger(int numDigits);
    ECLargeInteger(const ECLargeInteger &rhs);
    ECLargeInteger& operator=(const ECLargeInteger &rhs);

    // basic operations
    // Get the digit at a position
    // pos: position of the digit. pos=0: most siginficant digit
    int GetDigit(int pos) const;
    // Set the digit at [pos] to val (decimal digit: between 0 to 9)
    // pos: position of the digit. pos=0: most siginficant digit
    // Throw exception if out of bound or digit is out of range
    void SetDigit(int pos, int val);  
    // Get number of digits
    int GetLength() const { return listDigits.size(); }

    // arithmetic operations: +/-/*.  
    // Your code here
    ECLargeInteger operator+(const ECLargeInteger &rhs);
    ECLargeInteger operator-(const ECLargeInteger &rhs);
    ECLargeInteger operator*(const ECLargeInteger &rhs);
    bool operator>=(const ECLargeInteger &rhs);
    ECLargeInteger operator%(const ECLargeInteger &rhs);
    bool operator==(const ECLargeInteger &rhs);
    bool operator<(const ECLargeInteger &rhs);

    ECLargeInteger power(const ECLargeInteger &base, const ECLargeInteger &exp) const;
    ECLargeInteger modular_power(const ECLargeInteger &base, const ECLargeInteger &exp, const ECLargeInteger &modulus) const;
    // division: return quotient and remainder (as passed-in parameter)
    ECLargeInteger DividedBy(const ECLargeInteger &divisor, ECLargeInteger &remainder) const;

    // other operations
    void Dump() const;  // for debugging, dump out integer as what people usually do: like 16234 as a string of digits

    // check primality using Fermat's test
    // base: Fermat's test base: base^{n-1} mod n = remainder 
    // If pRemainder is not NULL, set to the calculated remainder as above (this makes testing a little easier) 
    bool FermatTest(const ECLargeInteger &base, ECLargeInteger *pRemainder = NULL) const;

private:
    // your code here
    vector<int> listDigits;
};

#endif
