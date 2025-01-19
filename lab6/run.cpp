#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <sstream>
#include <iomanip>

#include <ctime>

class TBigInt {
public:
    TBigInt() {};
    TBigInt(std::string & number);
    TBigInt(int number);
    TBigInt(std::vector<int>::const_iterator begin, std::vector<int>::const_iterator end);
    
    TBigInt operator+(const TBigInt & other);
    TBigInt operator-(const TBigInt & other);

    TBigInt operator*(const TBigInt & other) const;
    TBigInt KaratsubaMultiply(const TBigInt &obj) const;
    TBigInt operator/(const TBigInt & other);
    TBigInt FastDivide(TBigInt & first, const TBigInt &other);
    TBigInt Pow(TBigInt n);

    friend std::ostream& operator<<(std::ostream&, const TBigInt& numb);

    TBigInt& operator=(const TBigInt & other);

    bool operator<(const TBigInt & other) const;
    bool operator>(const TBigInt & other) const;
    bool operator==(const TBigInt & other) const;
    bool operator!=(const TBigInt & other) const;
    bool operator<=(const TBigInt & other) const;
    bool operator>=(const TBigInt & other) const;

    bool Odd() const;
    bool Even() const;

    ~TBigInt() {}; 
private:
    void ShiftLeft(int shift);
    void RemoveLeadingZeroes();
private:
    std::vector<int> Digits;
    const int Base = 10000;
    const int BasePow = 4;
};

TBigInt::TBigInt(std::string & numb) 
{   
    std::stringstream radixStrStream;
    // Зачищаем число от ведущих нулей
    if (numb[0] == '0') {
        size_t it = 0;
        while (it < numb.size() && numb[it] == '0') {
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
        if (i < other.Digits.size()) {
            second = other.Digits[i];
        } else {
            second = 0;
        }

        // Складываем разряды
        res.Digits.push_back(first + second + remainder);
        
        remainder = res.Digits.back() >= Base;

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
        if (i < other.Digits.size()) {
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
    int s1 = (int) Digits.size();
    int s2 = (int) other.Digits.size();

    for (int i = 0; i < s1; ++i) {
        int remainder = 0;
        for (int j = 0; j < s2 || remainder; ++j) {
            int second;
            if (j < s2) {
                second = other.Digits[j];
            } else {
                second = 0;
            }
            res.Digits[i + j] += Digits[i] * second + remainder;
            remainder = res.Digits[i + j] / Base;
            res.Digits[i + j] -= remainder * Base;
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
        
        if (!currVal.Digits.empty() && !currVal.Digits.back()) {
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

std::ostream &operator<<(std::ostream &stream, const TBigInt &num)
{
    int s = num.Digits.size();
    if (!s) {
        return stream;
    }
    stream << num.Digits[s - 1];
    for (int i = s - 2; i >= 0; --i) {
        stream << std::setfill('0') << std::setw(num.BasePow) << num.Digits[i];
    }

    return stream;
}

bool TBigInt::Odd() const 
{
    if (Digits.size() == 0) {
        return false;
    }

    return (bool)(Digits[0] & 1);
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

TBigInt TBigInt::FastDivide(TBigInt & first, const TBigInt & other) {
    if (first < other) {
        return TBigInt(0);
    }

    TBigInt dividend = first;
    TBigInt divisor = other;

    int divisorSize = divisor.Digits.size();
    int dividendSize = dividend.Digits.size();

    int normalizationFactor = Base / (divisor.Digits.back() + 1);
    dividend = dividend * TBigInt(normalizationFactor);
    divisor = divisor * TBigInt(normalizationFactor);

    TBigInt quotient;
    quotient.Digits.resize(dividendSize - divisorSize + 1);

    for (int i = dividendSize - divisorSize; i >= 0; --i) {
        int dividendDigit1 = i + divisorSize < dividend.Digits.size() ? dividend.Digits[i + divisorSize] : 0;
        int dividendDigit2 = i + divisorSize - 1 < dividend.Digits.size() ? dividend.Digits[i + divisorSize - 1] : 0;
        int dividendDigit3 = i + divisorSize - 2 < dividend.Digits.size() ? dividend.Digits[i + divisorSize - 2] : 0;

        int q = (dividendDigit1 * Base + dividendDigit2) / divisor.Digits.back();
        int r = (dividendDigit1 * Base + dividendDigit2) % divisor.Digits.back();

        while (q == Base || (q * divisor.Digits[divisorSize - 2] > r * Base + dividendDigit3)) {
            --q;
            r += divisor.Digits.back();
            if (r >= Base) break;
        }

        TBigInt multDivisor = divisor * TBigInt(q);
        TBigInt currentSubDividend;
        currentSubDividend.Digits.insert(currentSubDividend.Digits.begin(), dividend.Digits.begin() + i, dividend.Digits.begin() + i + divisorSize + 1);

        if (currentSubDividend < multDivisor) {
            --q;
            multDivisor = divisor * TBigInt(q);
        }

        quotient.Digits[i] = q;

        // Вычитаем умноженное частное из делимого
        for (size_t j = 0; j < multDivisor.Digits.size(); ++j) {
            dividend.Digits[i + j] -= multDivisor.Digits[j];
            if (dividend.Digits[i + j] < 0) {
                dividend.Digits[i + j] += Base;
                dividend.Digits[i + j + 1]--;
            }
        }
    }

    quotient.RemoveLeadingZeroes();

    return quotient;
}

TBigInt TBigInt::KaratsubaMultiply(const TBigInt &other) const
{
    // Если одно из чисел небольшое - умножаем как обычно
    if (Digits.size() < 2 || other.Digits.size() < 2) {
        return (*this) * other;
    }

    int size1 = Digits.size();
    int size2 = other.Digits.size();
    int maxSize = std::max(size1, size2);

    // Приводим числа к одинаковой длине добавлением нулей
    TBigInt A = *this;
    TBigInt B = other;

    A.Digits.resize(maxSize, 0);
    B.Digits.resize(maxSize, 0);

    int halfSize = maxSize / 2;

    // Разделяем числа на старшую и младшую части
    TBigInt low1(A.Digits.begin(), A.Digits.begin() + halfSize);
    TBigInt high1(A.Digits.begin() + halfSize, A.Digits.end()); 
      
    TBigInt low2(B.Digits.begin(), B.Digits.begin() + halfSize);
    TBigInt high2(B.Digits.begin() + halfSize, B.Digits.end()); 

    // Рекурсивно умножаем части
    TBigInt z = low1.KaratsubaMultiply(low2);      // z = low1 * low2
    TBigInt y = high1.KaratsubaMultiply(high2);    // y = high1 * high2

    // (low1 + high1) * (low2 + high2)
    TBigInt sum1 = low1 + high1; 
    TBigInt sum2 = low2 + high2;
    TBigInt x = sum1.KaratsubaMultiply(sum2);

    // Вычисляем промежуточные результаты
    x = x - y - z;

    // Сдвигаем результаты
    y.ShiftLeft(2 * halfSize);  // Сдвиг на 2 * halfSize
    x.ShiftLeft(halfSize);      // Сдвиг на halfSize

    // Собираем результат
    TBigInt result = y + x + z;

    // Удаляем ведущие нули
    result.RemoveLeadingZeroes();

    return result;
}

void TBigInt::RemoveLeadingZeroes()
{
    while (Digits.size() > 1 && !Digits.back()) {
        Digits.pop_back();
    }   
}

std::string RandomNumberString(size_t size)
{
    srand(time(nullptr));
    std::string str;
    std::string tmp;
    for (size_t i = 0; i < size; ++i) {
        
        tmp = (rand() % 9) + '0';

        str.append(tmp);
    }

    return str;
}

int main() {
    // clock_t start, end;
    // std::string s1, s2, s3;
    // char op;

    // s1 = RandomNumberString(100000);
    // s2 = RandomNumberString(200000);
    // s3 = RandomNumberString(300000);

    // std::string ss = RandomNumberString(9000);

    // TBigInt n1(s3), n2(ss);
    // start = clock();

    // TBigInt a;
    // TBigInt nn1(s3), nn2(ss);
    // a = nn1 / nn2;

    // end = clock();
    // std::cout << " C++ time: " << (double)(end - start) / CLOCKS_PER_SEC << " sec." << std::endl;
    // start = clock();

    // TBigInt as = a.FastDivide(n1, n2);

    // end = clock();
    // std::cout << "C++ time: " << (double)(end - start) / CLOCKS_PER_SEC << " sec." << std::endl;


    std::string s1, s2;
    char operand;
    while (std::cin >> s1 >> s2 >> operand) {
        TBigInt n1(s1);
        TBigInt n2(s2);
        switch (operand) {
        case '+':
            std:: cout << n1 + n2 << '\n';
            break;
        case '-':
            if (n1 < n2) {
                std::cout << "Error" << '\n';
            } else {
                std::cout << n1 - n2 << '\n';
            }
            break;
        case '*':
            std::cout << n1 * n2 << '\n';
            break;
        case '/':
            if (n2 == TBigInt(0)) {
                 std::cout << "Error" << '\n';
            } else {
                TBigInt tmp;
                std::cout << tmp.FastDivide(n1, n2)  << '\n';
            }
            break;
        case '^':
            if (n1 == TBigInt(0)) {
                if (n2 == TBigInt(0)) {
                    std::cout << "Error" << '\n';
                } else {
                    std::cout << "0" << '\n';
                }
            } else if (n1 == TBigInt(1)) {
                std::cout << "1" << '\n';
            } else {
                std::cout << n1.Pow(s2) << '\n';
            }
            break;
        case '>':
            if (n1 > n2) {
                std::cout << "true" << '\n';
            } else {
                std::cout << "false" << '\n';
            }
            break;
        case '<':
            if (n1 < n2) {
                std::cout << "true" << '\n';
            } else {
                std::cout << "false" << '\n';
            }
            break;
        case '=':
            if (n1 == n2) {
                std::cout << "true" << '\n';
            } else {
                std::cout << "false" << '\n';
            }
            break;
        }
    }
    return 0;
}