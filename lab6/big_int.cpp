#include "big_int.h"

TBigInt::TBigInt(std::string & numb) 
{   
    std::stringstream radixStrStream;
    // Зачищаем число от ведущих нулей
    if (numb[0] == '0') {
        int it = 1;
        
        while(numb[it] == 0) {
            it++;
        }

        if (it == numb.size()) {
            numb = "0";
        } else {
            numb = numb.substr(it);
        }
    } 

    Digits.clear();

    for (int i = numb.size() - 1; i >= 0; i -= BasePow) {
        int left = i - BasePow + 1;
        if (left < 0) {
            left = 0;
        }

        int right = i - left + 1;
        radixStrStream << numb.substr(left, right);
        
        int tmp = 0;
        radixStrStream >> tmp;

        Digits.push_back(tmp);
        radixStrStream.clear();
    }
}

TBigInt::TBigInt(int numb) 
{
    if (numb < Base) {
        Digits.push_back(numb);
    } else {
        while(numb) {
            Digits.push_back(numb % Base);
            numb /= Base;
        } 
    }
}

TBigInt::TBigInt(std::vector<int>::const_iterator begin, std::vector<int>::const_iterator end)
{
    // Очищаем текущие данные и копируем диапазон из вектора
    Digits.assign(begin, end);
}


TBigInt TBigInt::operator+(const TBigInt & other)
{
    TBigInt res;
    int remainder = 0;

    for (size_t i = 0; i < std::max(Digits.size(), other.Digits.size()) || remainder; ++i) {
        int first, second;
        // Закончился разряд у 1 числа
        if (i < Digits.size()) {
            first = Digits[i];
        } else {
            first = 0;
        }
        // Закончился разряд у 2 числа
        if (i < Digits.size()) {
            second = other.Digits[i];
        } else {
            second = 0;
        }

        // Складываем разряды
        res.Digits.push_back(first + second + remainder);
        
        remainder = (res.Digits.back() >= Base);

        if (remainder) {
            res.Digits.back() -= Base;
        }
    }

    return res;
}

TBigInt TBigInt::operator-(const TBigInt & other)
{
    TBigInt res;
    int remainder = 0;

    for (size_t i = 0; i < Digits.size() || remainder; ++i) {
        int first, second;
        // Закончился разряд у 1 числа
        if (i < Digits.size()) {
            first = Digits[i];
        } else {
            first = 0;
        }
        // Закончился разряд у 2 числа
        if (i < Digits.size()) {
            second = other.Digits[i];
        } else {
            second = 0;
        }

        res.Digits.push_back(first - second - remainder);

        remainder = res.Digits.back() < 0;

        if (remainder) {
            res.Digits.back() += Base;
        }
    }
    res.RemoveLeadingZeroes();

    return res;
}

TBigInt TBigInt::operator*(const TBigInt & other) const
{
    TBigInt res;
    res.Digits.resize(Digits.size() + other.Digits.size());
    int num1Size = (int) Digits.size();
    int num2Size = (int) other.Digits.size();

    for (int i = 0; i < num1Size; ++i) {
        int carry = 0;
        for (int j = 0; j < num2Size || carry; ++j) {
            int bb = j < num2Size ? other.Digits[j] : 0;
            res.Digits[i + j] += Digits[i] * bb + carry;
            carry = res.Digits[i + j] / Base;
            res.Digits[i + j] -= carry * Base;
        }
    }
    res.RemoveLeadingZeroes();
    
    return res;
}

TBigInt TBigInt::operator/(const TBigInt & other)
{
    TBigInt res, currVal = TBigInt(0);
    res.Digits.resize(Digits.size());

    for (int i = (int)Digits.size() - 1; i >= 0; --i) {
        currVal.Digits.insert(currVal.Digits.begin(), Digits[i]);
        
        if (!currVal.Digits.back()) {
            currVal.Digits.pop_back();
        }

        int quotient = 0, left = 0, right = Base;
        while (left <= right) {
            int mid = (left + right) / 2;
            TBigInt tmp = other * TBigInt(mid);
            if (tmp <= currVal) {
                quotient = mid;
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }

        res.Digits[i] = quotient;
        currVal = currVal - other * TBigInt(quotient);
    }

    res.RemoveLeadingZeroes();

    return res;
}

TBigInt TBigInt::Pow(TBigInt n)
{
    TBigInt res(1);

    while (n != TBigInt(0)) {
        if (n.Odd()) {
            res = res * (*this);
        }

        (*this) = (*this) * (*this);
        n = n / TBigInt(2);
    }
    
    return res;
}

TBigInt& TBigInt::operator=(const TBigInt & other)
{
    // Защита от самоприсваивания
    if (this == &other) {
        return *this;
    }

    Digits = other.Digits; 

    return *this;
}

bool TBigInt::operator<(const TBigInt & other) const
{
    if (Digits.size() != other.Digits.size()) {
        return Digits.size() < other.Digits.size();
    }

    return std::lexicographical_compare(
        Digits.rbegin(), 
        Digits.rend(), 
        other.Digits.rbegin(),
        other.Digits.rend()
    );
}

bool TBigInt::operator>(const TBigInt & other) const
{
    if (Digits.size() != other.Digits.size()) {
        return Digits.size() > other.Digits.size();
    }

    return std::lexicographical_compare( 
        other.Digits.rbegin(),
        other.Digits.rend(),
        Digits.rbegin(), 
        Digits.rend()
    );
}

bool TBigInt::operator==(const TBigInt & other) const
{
    return Digits == other.Digits;
}

bool TBigInt::operator!=(const TBigInt & other) const
{
    return !(*(this) == other);
}

bool TBigInt::operator<=(const TBigInt & other) const
{
    return *(this) < other || *(this) == other;
}

bool TBigInt::operator>=(const TBigInt & other) const
{
    return *(this) > other || *(this) == other;
}

void TBigInt::Print()
{
    size_t s = Digits.size();
    if (!s) {
        return;
    }

    std::cout << Digits[s - 1];
    for (int i = s - 2; i >= 0; i--) {
        std::cout << std::setfill('0') << std::setw(BasePow) << Digits[i];
    }

    std::cout << '\n';
}

bool TBigInt::Odd() const 
{
    if (Digits.size() == 0) {
        return false;
    }

    return Digits[0] & 1;
}

bool TBigInt::Even() const 
{
    return !this->Odd();
}

void TBigInt::ShiftLeft(int shift)
{
    if (shift <= 0) {
        return;
    }

    Digits.insert(Digits.begin(), shift, 0);
}

TBigInt TBigInt::KaratsubaMultiply(const TBigInt &other) const
{
    // Если числа небольшие - умножаем как обычно
    if (Digits.size() == 1 || other.Digits.size() == 1) {
        TBigInt tmp = *(this) * other; 
        return tmp;
    }

    int size1 = Digits.size();
    int size2 = other.Digits.size();
    int halfSize = std::min(size1, size2) / 2;

    // Разделяем текущее число на две части: старшую и младшую
    TBigInt low1(Digits.begin(), Digits.begin() + halfSize);   // Младшие цифры
    TBigInt high1(Digits.begin() + halfSize, Digits.end());    // Старшие цифры

    // Разделяем other на две части
    TBigInt low2(other.Digits.begin(), other.Digits.begin() + halfSize);   // Младшие цифры
    TBigInt high2(other.Digits.begin() + halfSize, other.Digits.end());    // Старшие цифры

    // Вычисляем z = low1 * low2
    TBigInt z = low1.KaratsubaMultiply(low2);

    // Вычисляем y = high1 * high2
    TBigInt y = high1.KaratsubaMultiply(high2);

    // Вычисляем z1 = (low1 + high1) * (low2 + high2)
    TBigInt sum1 = low1 + high1;  // low1 + high1
    TBigInt sum2 = low2 + high2;  // low2 + high2
    TBigInt x = sum1.KaratsubaMultiply(sum2);

    x = x - y - z;

    // Сдвигаем y на 2 * halfSize
    y.ShiftLeft(2 * halfSize * BasePow);

    // Сдвигаем x на halfSize
    x.ShiftLeft(halfSize * BasePow);

    // Складываем все части: y * B^{2m} + (x - y - z) * B^m + z
    TBigInt result = y + x + z;

    return result;
}


void TBigInt::RemoveLeadingZeroes()
{
    while (Digits.size() > 1 && !Digits.back()) {
        Digits.pop_back();
    }   
}

