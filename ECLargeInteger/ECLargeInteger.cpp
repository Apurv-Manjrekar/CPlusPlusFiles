#include "ECLargeInteger.h"
#include <string>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <thread>
#include <chrono>


using namespace std;
// Assume numDigits >= 1
ECLargeInteger::ECLargeInteger(int numDigits) : listDigits(numDigits, 0)
{}

ECLargeInteger::ECLargeInteger(const ECLargeInteger &rhs)
{
    // only copy if rhs is not null and has a valid vector
    if(&rhs != NULL && rhs.GetLength() > 0)
    {
        // resize listDigits to match new length
        this->listDigits.resize(rhs.GetLength());
        // copy listDigits from rhs to this
        copy(rhs.listDigits.begin(), rhs.listDigits.end(), this->listDigits.begin());
    }
    else
    {
        // resize to 0 if rhs is empty
        this->listDigits.resize(0);
    }
}

ECLargeInteger& ECLargeInteger::operator=(const ECLargeInteger &rhs)
{
    // check if assignment to self
    if(&rhs != this)
    {
        // only copy if rhs is not null and has a valid vector
        if(&rhs != NULL && rhs.GetLength() > 0)
        {
            // resize listDigits to match new length
            this->listDigits.resize(rhs.GetLength());
            // copy listDigits from rhs to this
            copy(rhs.listDigits.begin(), rhs.listDigits.end(), this->listDigits.begin());
        }
        else
        {
            // resize to 0 if rhs is empty
            this->listDigits.resize(0);
        }
    }
    // return new this
    return *this;
}

// basic operations
// Get the digit at a position
// pos: position of the digit. pos=0: most siginficant digit
int ECLargeInteger::GetDigit(int pos) const
{
    // return digit at position
    return this->listDigits[pos];
}
// Set the digit at [pos] to val (decimal digit: between 0 to 9)
// pos: position of the digit. pos=0: most siginficant digit
// Throw exception if out of bound or digit is out of range
void ECLargeInteger::SetDigit(int pos, int val)
{
    // Check if in bounds
    if(pos >= this->GetLength() || val < 0 || val > 9)
    {
        // thorw error if out of bounds
        throw invalid_argument("SetDigit: bound or digit is out of range");
    }
    else
    {
        // set val
        this->listDigits[pos] = val;
    }
}  

// arithmetic operations: +/-/*.  
ECLargeInteger ECLargeInteger::operator+(const ECLargeInteger &rhs)
{

    int carry = 0;
    int sum = 0;
    ECLargeInteger larger(0); // keep track of larger value
    ECLargeInteger smaller(0); // keep track of smaller value
    
    // determine larger and smaller
    if(this->GetLength() > rhs.GetLength())
    {
        larger = *this;
        smaller = rhs;
    }
    else
    {
        larger = rhs;
        smaller = *this;
    }
    // result large integer (max size is one plus larger.length)
    ECLargeInteger res(larger.GetLength()+1);
    
    // iterate through digits
    for(int i = 0; i <= larger.GetLength(); ++i)
    {
        // check if smaller is still going
        if(i < smaller.GetLength())
        {
            sum = this->GetDigit(larger.GetLength()-1-i) + rhs.GetDigit(smaller.GetLength()-1-i) + carry;
        }
        else
        {
            sum = this->GetDigit(larger.GetLength()-1-i) + 0 + carry;
        }

        // if sum is greater than or equal to 10, set carry to 1 and subtract 10 from sum
        if(sum >= 10)
        {
            carry = 1;
            sum -= 10;
        }
        else
        {
            carry = 0; // set carry back to 0
        }
        // set digit at pos to sum
        res.SetDigit(res.GetLength()-1-i, sum);
    }

    // remove leading 0s
    while(res.GetLength() > 1 && (res.GetDigit(0) == 0))
    {
        res.listDigits.erase(res.listDigits.begin());
    }
    return res;
}

ECLargeInteger ECLargeInteger::operator-(const ECLargeInteger &rhs)
{
    int size = 0;
    int diff = 0;
    int borrow = 0;
    // assume this is larger than rhs
    if(this->GetLength() >= rhs.GetLength())
    {
        size = this->GetLength();
    }
    // create result large integer
    ECLargeInteger res(size);

    // iterate through digits
    for(int i = 0; i < size; ++i)
    {
        // check if rhs is still going, compute sum
        if(i < rhs.GetLength())
        {
            diff = this->GetDigit(this->GetLength()-1-i) - rhs.GetDigit(rhs.GetLength()-1-i) - borrow;
        }
        else
        {
            diff = this->GetDigit(this->GetLength()-1-i) - 0 - borrow;
        }
        
        // if difference is less than 0, borrow from next digit and add 10 to diff
        if(diff < 0)
        {
            borrow = 1;
            diff += 10;
        }
        else
        {
            borrow = 0; // set borrow to 0
        }
        // set digit at pos to diff
        res.SetDigit(res.GetLength()-1-i, diff);
    }

    // remove leading 0s
    while(res.GetLength() > 1 && (res.GetDigit(0) == 0))
    {
        res.listDigits.erase(res.listDigits.begin());
    }
    return res;
}

ECLargeInteger ECLargeInteger::operator*(const ECLargeInteger &rhs)
{
    int sz1 = this->GetLength(); // size of this
    int sz2 = rhs.GetLength(); // size of rhs
    int size = sz1 + sz2; // size of output
    int carry = 0;
    int product = 0;
    // large integer res to hold result
    ECLargeInteger res(size);

    // go through all digits in this
    for(int i = 0; i < sz1; ++i)
    {
        carry = 0; // set carry to 0
        // go through all digits in rhs
        for(int j = 0; j < sz2 || carry; ++j)
        {
            // compute product
            product = (this->GetDigit(sz1-1-i) * rhs.GetDigit(sz2-1-j)) + res.GetDigit(i+j) + carry;
            res.SetDigit(i+j, product % 10); // set digit to product mod 10
            carry = product / 10; // set carry to product /10
        }   
    }
    // reverse listdigits
    reverse(res.listDigits.begin(), res.listDigits.end());
    // remove leading 0s
    while(res.GetLength() > 1 && (res.GetDigit(0) == 0))
    {
        res.listDigits.erase(res.listDigits.begin());
    }
    return res;
}

// overload >=
bool ECLargeInteger::operator>=(const ECLargeInteger &rhs){
    // check lengths
    if(this->GetLength() > rhs.GetLength())
    {
        return true;
    }
    else if(this->GetLength() < rhs.GetLength()) 
    {
        return false;
    }
    else // if lengths are equal
    {
        // iterate through all elements and check if each digit is less
        for(int i = 0; i < this->GetLength(); ++i)
        {
            if(this->listDigits[i] > rhs.listDigits[i])
            {
                return true;
            } 
            else if(this->listDigits[i] < rhs.listDigits[i]) 
            {
                return false;
            }
            else if(i == this->GetLength())
            {
                return true;
            }
        }
    }
    return true;
}


// division: return quotient and remainder (as passed-in parameter)
ECLargeInteger ECLargeInteger::DividedBy(const ECLargeInteger &divisor, ECLargeInteger &remainder) const
{
    // Check if divisor is 0, throw error
    if(divisor.GetLength() == 1 && divisor.GetDigit(0) == 0)
    {
        throw invalid_argument("DividedBy: Divide by zero error");
    }
    // Check if divisor bigger than dividend, if yes remainder is this
    ECLargeInteger dividend(*this);
    if((!(dividend >= divisor)) && (!(dividend == divisor)))
    {
        remainder = *this;
        return ECLargeInteger(1);
    }

    // max result size is length of this minus length of divisor plus 1
    ECLargeInteger res(this->GetLength()-divisor.GetLength()+1);
    ECLargeInteger temp(1); // temp to hold quotient
    remainder = *this; // remainder starts at this
    int quotient = 0; // quotient counter

    // subtrack divisor from remainder while remainder greater than divisor    
    while(remainder >= divisor)
    {
        remainder = remainder - divisor;
        quotient++; // increment quotient
        // extract quotient and add to res if at 9 (can only set digits to 9 at a time)
        if(quotient == 9)
        {
            temp.SetDigit(0, quotient);
            res = res + temp;
            quotient = 0; // set back to 0
        }
    }
    // add any remaining quotient
    temp.SetDigit(0, quotient);
    res = res + temp;
    
    // remove leading 0s
    while(res.GetLength() > 1 && (res.GetDigit(0) == 0))
    {
        res.listDigits.erase(res.listDigits.begin());
    }
    return res;
}

// other operations
void ECLargeInteger::Dump() const  // for debugging, dump out integer as what people usually do: like 16234 as a string of digits
{
    string largeInteger = "";
    for(int digit : this->listDigits)
    {
        largeInteger += to_string(digit);
    }
    cout << largeInteger << endl;
}

// overload %
ECLargeInteger ECLargeInteger::operator%(const ECLargeInteger &rhs)
{
    // call divided by and return remainder
    ECLargeInteger rem(*this);
    this->DividedBy(rhs, rem);
    return rem;
}

// overload ==
bool ECLargeInteger::operator==(const ECLargeInteger &rhs)
{
    // make sure lenghts equal
    if(this->GetLength() == rhs.GetLength())
    {
        // iterate through and make sure all digits match
        for(int i = 0; i < this->GetLength(); ++i)
        {
            if(this->GetDigit(i) != rhs.GetDigit(i))
            {
                return false;
            }
        }
        return true;
    }
    return false;
}

// modular exponentiation
ECLargeInteger ECLargeInteger::modular_power(const ECLargeInteger &base, const ECLargeInteger &exp, const ECLargeInteger &modulus) const
{
    // res holds result
    ECLargeInteger res(1);
    res.SetDigit(0, 1);

    // holds the value 1
    ECLargeInteger one(1);
    one.SetDigit(0, 1);

    // holds the value 2
    ECLargeInteger two(1);
    two.SetDigit(0, 2);

    // copy constant inputs
    ECLargeInteger baseCopy(base);
    ECLargeInteger expCopy(exp);
    ECLargeInteger mod(modulus);

    ECLargeInteger rem(1); // dummy var to hold remainder
    
    // Compute modular exponentiation algorithm
    baseCopy = baseCopy % mod; 

    while(expCopy >= one) // keep going while exponent is greater than 0
    {
        if (expCopy.GetDigit(expCopy.GetLength()-1) % 2 == 1) // if exponent is odd
        {
            res = (res * baseCopy) % mod;
            expCopy = expCopy - one;
        }
        expCopy = expCopy.DividedBy(two, rem);
        baseCopy = (baseCopy * baseCopy) % mod; 
    }

    return res; // return result
}

//overload <
bool ECLargeInteger::operator<(const ECLargeInteger &rhs)
{
    // copy inputs
    ECLargeInteger rhscopy(rhs);
    ECLargeInteger curr(*this);

    // check lengths
    if(curr.GetLength() < rhscopy.GetLength())
    {
        return true;
    }
    else if(curr.GetLength() > rhscopy.GetLength())
    {
        return false;
    }
    else // lengths are equal
    {
        // iterate through all digits and check if this is less than rhs
        for(int i = 0; i < curr.GetLength(); ++i)
        {
            if(curr.GetDigit(i) < rhscopy.GetDigit(i))
            {
                return true;
            }
            if(curr.GetDigit(i) > rhscopy.GetDigit(i))
            {
                return false;
            }
        }
        return true;
    }
}

// check primality using Fermat's test
// base: Fermat's test base: base^{n-1} mod n = remainder 
// If pRemainder is not NULL, set to the calculated remainder as above (this makes testing a little easier) 
bool ECLargeInteger::FermatTest(const ECLargeInteger &base, ECLargeInteger *pRemainder) const
{
    // holds the value 1
    ECLargeInteger one(1);
    one.SetDigit(0, 1);

    // holds this
    ECLargeInteger n(*this);

    // only compute if number is greater than 1 (1 and 0 are not prime)
    if (one < n)
    {
        // get exponent (this - 1)
        ECLargeInteger exp(*this);
        exp = exp - one; 
        // get mod (this)
        ECLargeInteger mod(*this);
        // call modular exponentiation and set result to res
        ECLargeInteger res = modular_power(base, exp, mod);

        // update pRemainder if it is not null
        if (pRemainder != nullptr)
            *pRemainder = res;

        // return true if remainder is 1, else return false
        return res == one;
    }
    else
    {
        return false; // return false if number is 1 or 0
    }
}
