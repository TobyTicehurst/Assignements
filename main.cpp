#include <iostream>
#include <vector>
#include <math.h>

using namespace std;

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
	~ComplexNumber() {}

	// Copy Constructor
	ComplexNumber(const ComplexNumber &obj) :
		m_real{ obj.m_real },
		m_complex{ obj.m_complex }
	{}

	ComplexNumber operator + (ComplexNumber& c) {
		return ComplexNumber(m_real + c.getReal(), m_complex + c.getComplex());
	}

	ComplexNumber operator - (ComplexNumber& c) {
		return ComplexNumber(m_real - c.getReal(), m_complex - c.getComplex());
	}

	ComplexNumber operator * (ComplexNumber& c) {
		return ComplexNumber(m_real * c.getReal() - m_complex * c.getComplex(),
							 m_complex * c.getReal() + m_real * c.getComplex());
	}

	ComplexNumber operator / (ComplexNumber& c) {
		return ComplexNumber((m_real * c.getReal() + m_complex * c.getComplex()) / (pow(c.getReal(), 2) + pow(c.getComplex(), 2)),
							 (m_complex * c.getReal() - m_real * c.getComplex()) / (pow(c.getReal(), 2) + pow(c.getComplex(), 2)));
	}

	// Getters and Setters
	double getReal() { return m_real; }
	double getComplex() { return m_complex; }
	void setReal(double real) { m_real = real; }
	void setComplex(double complex) { m_complex = complex; }
	void set(double real, double complex) { m_real = real; m_complex = complex; }

	// Complex Conjugate
	ComplexNumber conjugate() { return ComplexNumber(m_real, -m_complex); }

	// Modulus
	double modulus() { return sqrt(pow(m_real, 2) + pow(m_complex, 2)); }

private:
	double m_real;
	double m_complex;
};

int main() {
	ComplexNumber c1(1.0f, 2.0f);
	ComplexNumber c2(3.0f, 4.0f);
	ComplexNumber c3;

	c3 = c1 + c2;

	cout << c3.getComplex();

	char x;
	cin >> x;
	cout << x;
	
	return 0;
}