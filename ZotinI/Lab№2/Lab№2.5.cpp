#include <iostream>
#include <cmath>
#include <cstring>

using std::cout;
using std::cin;
using std::ostream;


class arr {
private:
	size_t n;
	float* array;
public:
	arr(size_t k = 6) {
		n = k;
		array = new float[n];
		for (size_t i = 0; i < n; ++i) {
			array[i] = (float)(rand() / (float)RAND_MAX) * 100 + 10;
		}
	}

	arr& operator=(const arr& other) {
		if (this != &other) {
			delete[] array;
			n = other.n;

			array = new float[n];
			memcpy(array, other.array, n*sizeof(float));
		}
		return *this;
	}

	arr(const arr& other) : n(other.n) {
		array = new float[n];
		memcpy(array, other.array, n*sizeof(float));
	}

	void install_size() {
		size_t k;
		do {
			cout << "Enter size:\n";
			cin >> k;
		} while (k < 1);

		if (n == k) {
			cout << "Size unchanged\n";
			return;
		}

		float* p = new float[k];

		size_t sz;
		if (n < k) {
			sz = n;
		}
		else {
			sz = k;
		}
		memcpy(p, array, sz * sizeof(float));
		
		if (k > n) {
			for (size_t j = n; j < k; ++j) {
				p[j] = 0.0f;
			}
		}
		delete[] array;
		array = p;
		n = k;

		cout << "Size changed to " << n << '\n';
	}

	size_t size() const {
		return n;
	}

	friend ostream& operator<<(ostream& stream, const arr& a) {
		stream << "Count elements in the array: " << a.n << '\n';
		for (size_t i = 0; i < a.n; ++i) {
			stream << a.array[i] << ' ';
		}
		stream << '\n';
		return stream;
	}

	float& operator[](size_t ind) {
		return array[ind];
	}

	const float& operator[](size_t ind) const {
		return array[ind];
	}

	float min() const {
		float m = array[0];
		for (size_t k = 1; k < n; ++k) {
			if (m > array[k]) {
				m = array[k];
			}
		}
		return m;
	}

	bool is_sort() const {
		for (size_t i = 0; i < n - 1; ++i) {
			if (array[i] > array[i + 1]) {
				return false;
			}
		}
		return true;
	}

	float* odd_indexed() const {
		size_t res_size = n/ 2;
		if (res_size == 0) {
			return nullptr;
		}
		float* result = new float[res_size];

		for (size_t i = 0; i < res_size; ++i) {
			result[i] = array[2*i + 1];
		}
		return result;
	}

	~arr() {
		delete[] array;
	}
};

int main() {
	srand(time(nullptr));
	int c;
	arr a;

	size_t index, ind;
	float val;
	float* odd_array = nullptr;
	do {
		cout << "Choice operation: \n1. set the size of the array\n" <<
			"2. find out the size of the array\n" <<
			"3. set the element of the array by its index\n" << "4. find out the element of the array by its index\n" <<
			"5. find the minimum element of the array\n" << "6. check whether the array is ordered\n" <<
			"7. select from the array an array with elements with odd indexes\n" << "8. Print array\n" << "0. exit\n";
		cin >> c;

		switch (c) {
		case 1:
			a.install_size();
			break;
		case 2:
			cout << "Size of the array: " << a.size() << '\n';
			break;
		case 3:
			cout << "Enter index: ";
			cin >> index;
			if (index < a.size()) {
				cout << "Enter value: ";
				cin >> val;
				a[index] = val;
				cout << "Operation completed successfully\n";
			}
			else {
				cout << "Index out of range\n";
			}
			break;
		case 4:
			cout << "Enter index: ";
			cin >> ind;
			if (ind < a.size()) {
				cout << "Element at index: " << a[ind] << '\n';
			}
			else {
				cout << "Index out of range\n";
			}
			break;
		case 5:
			cout << "Minimal element: " << a.min() << '\n';
			break;
		case 6:
			if (a.is_sort()) {
				cout << "The array is ordered\n";
			}
			else {
				cout << "The array is unordered\n";
			}
			break;
		case 7: 
			odd_array = a.odd_indexed();
			if (odd_array) {
				cout << "Elements at odd indexes: ";
				for (size_t i = 0; i < ceil(a.size() / 2); ++i) {
					cout << odd_array[i] << ' ';
				}
				cout << '\n';
				delete[] odd_array;
			}
			else {
				cout << "No elements with odd indexes\n";
			}
			break;
		case 8:
			cout << a;
			break;
		case 0:
			break;
		default:
			cout << "Input error. Enter number from 0 to 7\n";
			break;
		}

		cout << '\n';
	} while (c != 0);

	return 0;
}
