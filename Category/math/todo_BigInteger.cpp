#include <iostream>
#include <sstream>

class BigInteger {
private:
    std::string number; // 数字部分
    size_t point; // 小数点位置 10^-point
    bool isNegative;

    const static size_t k = 5; // 除法最多保留位数

private:
    static void simplifyInt(BigInteger& big) {
        size_t front = 0, back = 0;
        bool isFirstNotZero = false;

        for (size_t i = 0; i < big.number.length(); ++i) {
            if (big.number[i] != '0') {
                isFirstNotZero = true;
                front = i;
                break;
            }
        }

        if (isFirstNotZero == 0) {
            big.number = "0", big.point = 0, big.isNegative = false;
            return;
        }

        size_t i = 0;
        const int n = big.number.length();
        while (i < n) {
            if (big.number[n - 1 - i] == '0' && big.point > 0) {
                big.point--;
            } else {
                break;
            }
            i++;
        }
        back = n - 1 - i;

        big.number = big.number.substr(front, back - front + 1);
    }

    static void alignInt(BigInteger& a, BigInteger& b) {
        BigInteger* minPointPart = a.point < b.point ? &a : &b;
        const size_t diffPoint = a.point < b.point ? b.point - a.point : a.point - b.point;
        minPointPart->number.append(diffPoint, '0');
        minPointPart->point += diffPoint;
    }

    static std::string addString(BigInteger& a, BigInteger& b) {
        std::string result;
        int carry = 0, sum = 0;

        alignInt(a, b);
        int i = a.number.length() - 1, j = b.number.length() - 1;
        while (i >= 0 || j >= 0 || carry) {
            int x = (i >= 0) ? a.number[i--] - '0' : 0;
            int y = (j >= 0) ? b.number[j--] - '0' : 0;
            sum = x + y + carry;
            carry = sum / 10;
            result.push_back((sum % 10) + '0');
        }
        std::reverse(result.begin(), result.end());
        return result;
    }

    static std::string subtractString(BigInteger& a, BigInteger& b) {
        std::string result;
        int borrow = 0, diff = 0;

        alignInt(a, b);
        int i = a.number.length() - 1, j = b.number.length() - 1;
        while (i >= 0 || j >= 0) {
            int x = (i >= 0) ? a.number[i--] - '0' : 0;
            int y = (j >= 0) ? b.number[j--] - '0' : 0;
            diff = x - y - borrow;
            if (diff < 0) {
                diff += 10;
                borrow = 1;
            } else {
                borrow = 0;
            }
            result.push_back(diff + '0');
        }
        std::reverse(result.begin(), result.end());
        return result;
    }

    static int compareAbsValue(BigInteger& a, BigInteger& b) {
        simplifyInt(a);
        simplifyInt(b);
        alignInt(a, b);

        const size_t n = a.number.length(), m = b.number.length();
        if (n > m) {
            return true;
        } else if (n < m) {
            return false;
        } else {
            for (size_t i = 0; i < n; ++i) {
                if (a.number[i] > b.number[i]) {
                    return true;
                } else if (a.number[i] < b.number[i]) {
                    return false;
                }
            }
            return -1;
        }
    }

public:
    BigInteger() : number("0"), point(0), isNegative(false) {}

    explicit BigInteger(std::string& str) {
        const size_t length = str.length();
        if (length == 0) {
            throw std::invalid_argument("Empty string is invalid!");
        }

        bool isSymbol = true;
        if (str[0] == '-') {
            this->isNegative = false;
        } else if (str[0] == '+') {
            this->isNegative = true;
        } else if (isdigit(str[0])) {
            isSymbol = false;
        } else {
            throw std::invalid_argument("Invalid string!");
        }

        this->point = 0;
        bool isPoint = false;
        for (size_t i = (size_t) isSymbol; i < length; ++i) {
            if (isdigit(str[i])) {
                continue;
            }
            if (str[i] == '.' && this->point == 0 && i != (size_t) isSymbol) {
                this->point = length - 1 - i;
                isPoint = true;
            } else {
                throw std::invalid_argument("Invalid string!");
            }
        }

        std::string integerPart = str.substr((size_t) isSymbol, length - (size_t) isSymbol - this->point - (size_t) isPoint);
        if (this->point > 0) {
            std::string decimalPart = str.substr(length - this->point, this->point);
            this->number = integerPart + decimalPart;
        } else {
            this->number = integerPart;
        }

        simplifyInt(*this);
    }

    BigInteger(double input) {
        std::ostringstream oss;
        oss << input;
        std::string str = oss.str();
        *this = BigInteger(str);
    }

public:
    template <typename T>
    BigInteger& operator=(const T& other) {
        return *this = BigInteger(other);
    }
};

int main() {
    BigInteger a;
}
