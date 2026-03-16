#include <iostream>
#include <cmath>

class Number {
private:
	int a;
	int b;
public:
	Number() : a(0), b(1) {}
	int evclid() const {
		int nod = 0;
		int cb = b;
		int ca = a;
		cb = std::abs(cb);
		ca = std::abs(ca);
		if (ca <= cb) {
			while (cb % ca != 0) {
				int temp = cb;
				cb = ca;
				ca = temp % ca;
			}
			nod = ca;
		}
		else {
			while (ca % cb != 0) {
				int temp = ca;
				ca = cb;
				cb = temp % cb;
			}
			nod = cb;
		}
		return nod;
	}
	void reduce() {
		int nod = evclid();
		a = a / nod;
		b = b / nod;
		if (b < 0) {
			a = -a;
			b = -b;
		}
	}
	void print() const {
		std::cout << "The number is " << a << '/' << b << '\n';
	}
	void scan() {
		while (true) {
			std::cout << "Enter numerator and denominator\n";
			std::cin >> a >> b;
			
			if (b == 0) {
				std::cout << "Error: the denomirator cannot be 0\n" << "Try again\n";
			}
			
			else {
				break;
			}
		}
		reduce();
	}
	void summ() {
		int newa = 0;
		int newb = 0;
		std::cout << "Enter new fraction: ";
		while (true) {
			std::cin >> newa >> newb;
			if (newb == 0) {
				std::cout << "ERROR: incorrect fraction\n";
			}
			else {
				break;
			}
		}
		if (b != newb) {
			a = a * newb + newa * b;
			b = b * newb;
		}
		else {
			a = a + newa;
		}
		reduce();
		print();
	}
	void dif() {
		int newa = 0;
		int newb = 0;
		std::cout << "Enter new fraction: ";
		while (true) {
			std::cin >> newa >> newb;
			if (newb == 0) {
				std::cout << "ERROR: incorrect fraction\n";
			}
			else {
				break;
			}
		}
		if (b != newb) {
			a = a * newb - newa * b;
			b = b * newb;
		}
		else {
			a = a - newa;
		}
		reduce();
		print();
	}
	void mult() {
		int newa = 0;
		int newb = 0;
		std::cout << "Enter new fraction: ";
		while (true) {
			std::cin >> newa >> newb;
			if (newb == 0) {
				std::cout << "ERROR: incorrect fraction\n";
			}
			else {
				break;
			}
		}
		a = a * newa;
		b = b * newb;
		reduce();
		print();
	}
	void div() {
		int newa = 0;
		int newb = 0;
		std::cout << "Enter new fraction: ";
		while (true) {
			std::cin >> newa >> newb;
			if (newb == 0) {
				std::cout << "ERROR: incorrect fraction\n";
			}
			else {
				break;
			}
		}
		if (newa == 0) {
			std::cout << "You cannot divide by 0\n";
		}
		else {
			a = a * newb;
			b = b * newa;
		}
		reduce();
		print();
	}
	
};
void menu() {
	using std::cout;
	cout << '\n' << "Choose the operation:\n" << "1 - to enter number\n" << "2 - to print the number\n" << "3 - to add a fraction\n" << "4 - to substract a fraction\n";
	cout << "5 - to multiply by a fraction\n" << "6 - to divide by a fraction\n" << "7 - Exit the programm\n" << '\n';
}
int main() {
	int mode = 0;
	using std::cout;
	cout << "Welcome to calculator for fractions!!!\n";
	menu();
	Number A;
	while (true) {
		while (true) {
			std::cin >> mode;
			if (mode >= 1 && mode <= 7) {
				break;
			}
			else {
				cout << "ERROR: incorrect input. Try again\n";
				menu();
			}
		}
		switch (mode) {
		case 1:
			A.scan();
			menu();
			break;
		case 2:
			A.print();
			menu();
			break;
		case 3:
			A.summ();
			menu();
			break;
		case 4:
			A.dif();
			menu();
			break;
		case 5:
			A.mult();
			menu();
			break;
		case 6:
			A.div();
			menu();
			break;
		case 7:
			return 0;
			break;
		}
	}
}