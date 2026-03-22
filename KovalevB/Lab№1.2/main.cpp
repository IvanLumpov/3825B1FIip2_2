#include <iostream>

using std::cin, std::cout, std::ostream;
using ull = unsigned long long;

class LongNum {
private:
    unsigned int lo;
    unsigned int hi; 

    void fromBits(ull other) {
        hi = (unsigned int) (other >> 32);
        lo = (unsigned int)(other & 0xFFFFFFFF);
    }
    
    LongNum shl1() const {
        unsigned int bit = lo >> 31;
        return LongNum((hi << 1) | bit, lo << 1);
    }

    LongNum shr1() const {
        unsigned int bit = hi << 31;
        return LongNum(hi >> 1, (lo >> 1) | bit);
    }

    bool isNegative() const {
        return (hi >> 31) != 0;
    }

    bool isZero() const {
        return hi == 0 && lo == 0;
    }
    
    LongNum negate() const {
        unsigned int n = ~lo + 1; 
        return LongNum(~hi + (lo == 0), n);
    }

    LongNum absVal()  const { 
        return isNegative() ? negate() : *this; 
    }

    bool unsignedLess(const LongNum& b) const {
        return hi != b.hi ? hi < b.hi : lo < b.lo;
    }

    LongNum udiv(const LongNum& a, const LongNum& b, LongNum& rem) const {
        LongNum q(0);
        rem = LongNum(0);

        for (int i = 63; i >= 0; i--) {
            // Сдвигаем остаток влево — освобождаем место для нового бита
            rem = rem.shl1();

            // Берём i-й бит делимого и дописываем его в младший бит остатка
            unsigned int bit;
            if (i >= 32)
                bit = (a.hi >> (i - 32)) & 1u; // бит из старшей половины
            else
                bit = (a.lo >> i) & 1u; // бит из младшей половины
            rem.lo |= bit;

            if (!rem.unsignedLess(b)) {
                rem = rem - b;
                if (i >= 32)
                    q.hi |= 1u << (i - 32); // записываем 1 в i-й бит частного
                else
                    q.lo |= 1u << i;
            }
        }
        return q;
    }

    public:

    LongNum() : lo(0), hi(0) {} 

    LongNum(unsigned int h, unsigned int l): hi(h), lo(l) {}

    LongNum(int val): lo((unsigned int)val), hi((unsigned int)(val >> 31)) {}

    LongNum(long long val) { 
        fromBits((ull)val); 
    }

    LongNum operator+(const LongNum& b) const {
        unsigned int new_lo = lo + b.lo;
        unsigned int carry  = (new_lo < lo) ? 1u : 0u;
        return LongNum(hi + b.hi + carry, new_lo);
    }

    LongNum operator-(const LongNum& b) const {
        unsigned int borrow = (lo < b.lo) ? 1u : 0u;
        return LongNum(hi - b.hi - borrow, lo - b.lo);
    }

    // бинарное умножение
    LongNum operator*(const LongNum& b) const {

    LongNum result(0);
    LongNum shifted = *this;
    LongNum mult = b; 

    for (int i = 0; i < 64; i++) {
        if (mult.lo & 1u) {  // если младший бит множителя равен 1
            result = result + shifted;
        }          
        shifted = shifted.shl1();  // удваиваем слагаемое
        mult = mult.shr1();  // переходим к следующему биту множителя
    }    
        return result;
    }

    LongNum operator/(const LongNum& b) const {
        bool neg = isNegative() != b.isNegative();
        LongNum rem, q = udiv(absVal(), b.absVal(), rem);
        return (neg && !q.isZero()) ? q.negate() : q;
    }

    LongNum operator%(const LongNum& b) const {
        LongNum rem;
        udiv(absVal(), b.absVal(), rem);
        return (isNegative() && !rem.isZero()) ? rem.negate() : rem;
    }

    friend ostream& operator<<(ostream& os, const LongNum& n) {
        if (n.isZero())     return os << '0';
        if (n.isNegative()) return os << '-' << n.negate();

        // Получаем цифры числа делением на 10, накапливаются в обратном порядке
        char buf[20];
        int len = 0;
        LongNum cur = n;
        LongNum ten(10);

        while (!cur.isZero()) {
            buf[len++] = '0' + (cur % ten).lo;  // последняя цифра -> символ
            cur = cur / ten;
        }

        for (int i = len - 1; i >= 0; i--)
            os << buf[i];

        return os;
    }
};

int main() {
    LongNum Num1(11 * 1000000000LL); 
    LongNum Num2(6 * 1000000000LL); 

    cout << "Num1 = " << Num1 << '\t' << "Num2 = " << Num2 << '\n' << '\n';
    cout << "Num1 + Num2 = " << (Num1 + Num2) << '\n';
    cout << "Num1 - Num2 = " << (Num1 - Num2) << '\n';
    cout << "Num1 * 2 = " << (Num1 * LongNum(2)) << '\n';
    cout << "Num1 / Num2 = " << (Num1 / Num2) << '\n';
    cout << "Num1 % Num2 = " << (Num1 % Num2) << '\n';
}