#pragma once

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <sstream>
#include <iomanip>

class TBigInt {
public:
    TBigInt() {};
    TBigInt(std::string & number);
    TBigInt(int number);
    TBigInt(std::vector<int>::const_iterator begin, std::vector<int>::const_iterator end);

    void Print();
    
    TBigInt operator+(const TBigInt & other);
    TBigInt operator-(const TBigInt & other);

    TBigInt operator*(const TBigInt & other) const;
    TBigInt KaratsubaMultiply(const TBigInt &obj) const;
    TBigInt operator/(const TBigInt & other);

    TBigInt Pow(TBigInt n);

    TBigInt& operator=(const TBigInt & other);

    bool operator<(const TBigInt & other) const;
    bool operator>(const TBigInt & other) const;
    bool operator==(const TBigInt & other) const;
    bool operator!=(const TBigInt & other) const;
    bool operator<=(const TBigInt & other) const;
    bool operator>=(const TBigInt & other) const;

    ~TBigInt() {}; 
private:
    bool Odd() const;
    bool Even() const;

    void ShiftLeft(int shift);
    void RemoveLeadingZeroes();
private:
    std::vector<int> Digits;
    const int Base = 10000;
    const int BasePow = 4;
};