#include <iostream>

using std::cin, std::cout, std::ostream;

class Polynom{
    private:

    int degree;
    double* v;

    public:

    Polynom(): degree(0), v(new double[1]) {
        v[0] = 0;
    }
    
    Polynom(int n): degree(n), v(new double[n + 1]) {
        if (n < 0 || n > 12) {
            cout << "Error: Polynomial degree must be in [0,12]\n";
            return;
        }
        for (int i = 0; i <= n; ++i) {
            v[i] = 0;
        }
    }

    void SetDegree(int n) {
        if (n < 0 || n > 12) {
            cout << "Error: Polynomial degree must be in [0,12]\n";
            return;
        }
        delete[] v;
        degree = n;
        v = new double[n + 1];
        for (int i = 0; i <= n; ++i) {
            v[i] = 0;
        }
    }

    int DegreeCheck() const {
        return degree;
    }
    
    double AtPoint(double x) const {
        double result = 0;
        for (int i = degree; i >= 0; --i) {
        result = result * x + v[i];
        }
        return result;
    }

    Polynom Derivative() const { 
        if (degree == 0) {
            return Polynom();
        }
        Polynom res(degree - 1);
        for (int i = 1; i <= degree; ++i) {
            res[i - 1] = v[i] * i;
        }
        return res;
    }

    Polynom& operator=(const Polynom& other) {
        if (this != &other) {
            delete[] v;
            degree = other.degree;
            v = new double[degree + 1];
            for (int i = 0; i <= degree; ++i) {
                v[i] = other.v[i];
            }
        }
        return *this;
    }

    Polynom(const Polynom& other): degree(other.degree), v(new double[other.degree + 1]) {
        for (int i = 0; i <= degree; ++i) {
            v[i] = other.v[i];
        }
    }

    ~Polynom() {
        delete[] v;
    }

    double& operator[](int n) {
    if (n < 0 || n > degree) {
        cout << "Index out of range!\n";
        return v[0]; // вынуждены вернуть какую-то ссылку
    }
    return v[n];
    }

    double operator[](int n) const {
    if (n < 0 || n > degree) {
        cout << "Index out of range!\n";
        return 0.0;
    }
    return v[n];
    }

    friend ostream& operator<<(ostream& os, const Polynom& p) {
        bool first = true;
        for (int i = p.degree; i >= 0 ; --i) {
            if (p.v[i] == 0) continue;
            double absval = std::abs(p.v[i]);

            if (!first)
                os << (p.v[i] > 0 ? " + " : " - ");
            else if (p.v[i] < 0)
                os << "-";
          
            if (absval != 1 || i == 0) os << absval;
            if (i > 0) os << "x";
            if (i > 1) os << "^" << i;

            first = false;
        }
        
        if (first) os << "0";
        return os;
    }
};

int main() {
    Polynom bad1(-1), bad2(13);
    
    // p = 5x^3 - 2x + 1
    Polynom p(3);
    p[0] = 1; p[1] = -2; p[3] = 5;
    cout << "p = " << p << "\n";
    cout << "p(1) = " << p.AtPoint(1) << "\n";
    cout << "p' = " << p.Derivative() << "\n";
    cout << "degree = " << p.DegreeCheck() << "\n";
    cout << "p[0] = " << p[0] << "\n";

    p[99];

    Polynom copy1(p), copy2; copy2 = p;
    p[0] = 99;
    cout << "copy1 = " << copy1 << "\n";

    copy2 = copy1;
    cout << "copy2 = " << copy2 << "\n";
}