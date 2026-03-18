//Библиотеки и пространства имён.
#include <iostream>
using std::ostream;
using std::string;
using std::endl;
using std::cout;
using std::cin;


//Класс полинома.
class Polynom {
private:

    //Степень и указатель на массив с коэффициентами.
    int degree;
    double* coefficients;

public:

    //Конструктор по умолчанию.Степень 0. Константа в коэффициентах.
    Polynom(): degree(0), coefficients(new double[1]) {
        coefficients[0] = 0;
    }

    //Конструктор от степени.
    Polynom(int n): degree(n), coefficients(new double[n + 1]) {
        if (n < 0 || n > 12) {
            string error("ERR:Degree of the polynomial must be in [0,12].\n");
            throw error;
        }
        for (int i = 0; i <= n; ++i) {
            coefficients[i] = 0;
        }
    }

    //Конструктор копирования.
    Polynom(const Polynom& other) : degree(other.degree), coefficients(new double[other.degree + 1]) {
        for (int i = 0; i <= degree; ++i) {
            coefficients[i] = other.coefficients[i];
        }
    }

    //Деструктор, очистка массива коэффициентов.
    ~Polynom() {
        delete[] coefficients;
    }

    //Сеттер степени полинома.
    void SetDegree(int n) {
        if (n < 0 || n > 12) {
            string error("ERR:Degree of the polynomial must be in [0,12].\n");
            throw error;
        }
        delete[] coefficients;
        degree = n;
        coefficients = new double[n + 1];
        for (int i = 0; i <= n; ++i) {
            coefficients[i] = 0;
        }
    }

    //Геттер степени.
    int GetDegree() const {
        return degree;
    }

    //Вычисление значения в точке.
    double Calculate(double x) const {
        double result = 0;
        for (int i = degree; i >= 0; --i) {
            result = result * x + coefficients[i];
        }
        return result;
    }

    //Вычисление производной полинома.
    Polynom Derivative() const {
        if (degree == 0) {
            return Polynom(0);
        }
        Polynom result(degree - 1);
        for (int i = 1; i <= degree; ++i) {
            result[i - 1]= coefficients[i] * i;
        }
        return result;
    }

    //Перегрузка оператора присваивания. Необходима из-за наличия динамического массива т.е. указателя.
    Polynom& operator = (const Polynom& other) {
        if (this != &other) {
            delete[] coefficients;
            degree = other.degree;
            coefficients = new double[degree + 1];
            for (int i = 0; i <= degree; ++i) {
                coefficients[i] = other.coefficients[i];
            }
        }
        return *this;
    }

    //Перегрузка []. Две версии для получения и присваивания значения.
    double& operator[](int index) {
        if (index < 0 || index > degree) {
            string error("ERR: Index out of range.");
            throw error;
        }
        return coefficients[index];
    }

    const double& operator[](int index) const {
        if (index < 0 || index > degree) {
            string error("ERR: Index out of range.");
            throw error;
        }
        return coefficients[index];
    }

    //Перегрузка вывода.
    friend ostream& operator << (ostream& os, const Polynom& p);
};


ostream& operator << (ostream& os, const Polynom& p) {
    bool first = true;
    for (int i = p.degree; i >= 0; --i) {
        if (p.coefficients[i] != 0) {
            if (!first && p.coefficients[i] > 0) {
                os << "+";
            }
            else if (p.coefficients[i] < 0) {
                os << "-";
            }
            double abs_coef = abs(p.coefficients[i]);
            if (i == 0 || abs_coef != 1) {
                os << abs_coef;
            }
            if (i > 0) {
                os << "x";
                if (i > 1) {
                    os << "^" << i;
                }
            }
            first = false;
        }
    }
    if (first) {
        os << "0";
    }
    os << endl;
    return os;
}


//Демонстрация примеров использования класса.
int main() {
    try {
        int k = 5;
        Polynom p(k);
        for (int i = 0; i <= k; ++i) {
            p[i] = -1 * (double)i;
            cout << p[i] << '\n';
        }
        cout << p << p.Calculate(4) << '\n' << p.Derivative();
        Polynom b = p;
        b.SetDegree(p.GetDegree());
        Polynom a(b);
    }
    catch (const string& e) {
        std::cerr << e << endl;
    }
}