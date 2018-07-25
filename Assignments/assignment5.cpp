// Assignment 5 - a class to hold a complex number
// overloading + - * / and using friend functions to overload << >> from streams

#include <iostream>
#include <math.h>

using namespace std;

// tests if cin has failed, if so it cleans up
bool istreamFailed(istream& is) {
	if (is.fail()) {	// test for failed input
		is.clear();		// clear any flags
		is.ignore(numeric_limits<streamsize>::max(), '\n');	// ignore the stream up till \n, and defined max stream size
		cout << "Error: istream failed!\n";
		return true;
	}
	return false;
}

// A class to hold a complex number
class ComplexNumber {
public:
	// Default Constructor
	ComplexNumber() :
		m_real{ 0.0f },
		m_complex{ 0.0f }
	{}

	// Constructor
	ComplexNumber(double real, double complex) :
		m_real{ real },
		m_complex{ complex }
	{}

	// Default Destructor
	~ComplexNumber() {
		//cout << "Complex Number Destructor!\n";
	}

	// Overriding + - * /
	ComplexNumber operator + (const ComplexNumber& z) const {
		return ComplexNumber(m_real + z.getReal(), m_complex + z.getComplex());
	}

	ComplexNumber operator - (const ComplexNumber& z) const {
		return ComplexNumber(m_real - z.getReal(), m_complex - z.getComplex());
	}

	ComplexNumber operator * (const ComplexNumber& z) const {
		return ComplexNumber(m_real * z.getReal() - m_complex * z.getComplex(),
			m_complex * z.getReal() + m_real * z.getComplex());
	}

	ComplexNumber operator / (const ComplexNumber& z) const {
		return ComplexNumber((m_real * z.getReal() + m_complex * z.getComplex()) / (pow(z.getReal(), 2) + pow(z.getComplex(), 2)),
			(m_complex * z.getReal() - m_real * z.getComplex()) / (pow(z.getReal(), 2) + pow(z.getComplex(), 2)));
	}

	// override insertion operator from ostream
	friend ostream& operator << (ostream& os, const ComplexNumber& z);

	// override extraction operator from istream
	friend istream& operator >> (istream& is, ComplexNumber& z);

	// Getters and Setters
	double getReal() const { return m_real; }
	double getComplex() const { return m_complex; }
	void setReal(double real) { m_real = real; }
	void setComplex(double complex) { m_complex = complex; }
	void set(double real, double complex) { m_real = real; m_complex = complex; }

	// Complex Conjugate
	ComplexNumber conjugate() const { return ComplexNumber(m_real, -m_complex); }

	// Modulus
	double modulus() const { return sqrt(pow(m_real, 2) + pow(m_complex, 2)); }

private:
	// Real part
	double m_real;
	// Complex part
	double m_complex; 
};

// override insertion operator from ostream
ostream& operator << (ostream& os, const ComplexNumber& z)
{
	if (z.m_complex < 0)
		os << z.m_real << z.m_complex << "i";
	else
		os << z.m_real << "+" << z.m_complex << "i";
	return os;
}

// override extraction operator from istream
istream& operator >> (istream& is, ComplexNumber& z)
{
	// read in the real part
	is >> ws;	// ignore whitespace
	is >> z.m_real;
	if (istreamFailed(is))
		return is;

	// read in the sign
	is >> ws;
	char sign;
	is >> sign;
	if (istreamFailed(is))
		return is;

	// i may be before or after the number
	bool i_found = false;
	if (is.peek() == 'i') {
		// ignore the i
		is >> ws;
		is.ignore(1);
		i_found = true;
	}

	// read in the complex part
	is >> ws;
	is >> z.m_complex;
	if (istreamFailed(is))
		return is;

	if (is.peek() == 'i') {
		// ignore the i
		is >> ws;
		is.ignore(1);
		i_found = true;
	}

	if (!i_found)

		cout << "No complex part was found, taken to be after the " << sign << "\n";

	// deal with the sign
	if (sign == '-')
		z.m_complex *= -1;
	else if (sign != '+')
		cout << "Sign must be + or -!";

	return is;
}

int main() {
	// parameterised constructors
	ComplexNumber z_a(3.0f, 4.0f);
	ComplexNumber z_b(1.0f, -2.0f);

	// using the overloading of + - * / << 
	cout << z_a << " + " << z_b << " = " << z_a + z_b << "\n";
	cout << z_a << " - " << z_b << " = " << z_a - z_b << "\n";
	cout << z_a << " * " << z_b << " = " << z_a * z_b << "\n";
	cout << z_a << " / " << z_b << " = " << z_a / z_b << "\n";


	ComplexNumber z_c;
	cout << "Enter a complex number: ";
	// using the overloading of >>
	cin >> z_c;
	// using all the functions
	cout << "z = " << z_c.getReal() << (z_c.getComplex() < 0 ? "" : "+") << z_c.getComplex() << "i\nz* = " << z_c.conjugate() << "\nmod(z) = " << z_c.modulus() << "\n";


	// Pause at the end of the program
	char x;
	cin >> x;
	cout << x;

	return 0;
}
