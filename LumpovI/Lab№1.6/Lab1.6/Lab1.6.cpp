#include <iostream>

class Temp {
private:
	float temperature;
public:
	Temp(float temp) {
		set_temp(temp);
	}

	float get_temp() const {
		return temperature;
	}

	void set_temp(float temp) {
		temperature = temp;
	}

	float ConvertToFahrenheit() const {
		return get_temp() * 1.8f + 32;
	}

	float ConvertToKelvin() const {
		return get_temp() + 273.15f;
	}

	friend std::ostream& operator<<(std::ostream& os, const Temp& t) {
		os << "Temperature:" << t.temperature << "C, " << t.ConvertToFahrenheit() << "F, " << t.ConvertToKelvin() << "K" << std::endl;
		return os;
	}
};

int main() {
	int c = 0;
	float t = 0;
	float s = 0;
	std::cout << "Input the degrees (Celsius)\n";
	std::cin >> t;
	if (t < -273.15f) {
		std::cout << "Error!!!: temperature cannot be below absolute zero\n";
		return 1;
	}
	Temp a(t);
	do {
		std::cout << "Enter mode\n" << "1.Convert to Fahrenheit\n" << "2.Convert to Kelvin\n" << "3.Show your temperatur in Celsius\n" << "4.Temperature set\n" << "5.All value\n" << "6.Exit\n" << std::endl;
		std::cin >> c;
		if (c < 1 || c > 6) {
			std::cout << "Error\n";
			return 1;
		}
		switch (c) {
		case 1:
			std::cout << "Result:" << a.ConvertToFahrenheit() << "F\n";
			break;
		case 2:
			std::cout << "Result:" << a.ConvertToKelvin() << "K\n";
			break;
		case 3:
			std::cout << "Temperature in Celsius: " << a.get_temp() << std::endl;
			break;
		case 4:
			std::cout << "Input new value of temperature\n";
			std::cin >> s;
			a.set_temp(s);
			break;
		case 5:
			std::cout << a << std::endl;
			break;
		case 6:
			std::cout << "Goodbye!!!\n";
			break;
		}

	} while (c != 6);
	
	return 0;
}