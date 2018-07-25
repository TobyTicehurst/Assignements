// Assignment 7
// 3 classes, a cartesian vector, a minkowski 4 vector dervived from vector and particle which has a 4 vector position, a vector velocity and a double mass
// Everything in natural units 

// Vector works like std::vector except it has access to the underlying pointers and it defined a dot product, vector +/-. << override
// 4 vector implements an overloaded dot product and a lorentz boost

// I encountered a Visual studio 2013 bug:
// Compiler Error C2797 see msdn.microsoft.com/en-gb/library/Dn793970.aspx
// things marked with //*!* are changed because of this 

#include <vector>
#include <iostream>
#include <cmath>


// Cartesian Vector of Doubles
class Vector{

private:
	// A pointer to the first element
	double* m_begin;
	// A pointer to the past the end element
	double* m_end;
	// A pointer to the element that would be at the end of allocated memory
	double* m_cap;

	// double the capacity (used when m_end reached m_cap)
	void doubleCapacity() {
		reserve(capaicty() * 2);
	}

public:

	// Default Constructor
	Vector() :
		m_begin{ nullptr },
		m_end{ nullptr },
		m_cap{ nullptr }
	{}

	// Parameterised Constructor with initializer_list
	Vector(const std::initializer_list<double> init) :
		m_begin{ new double[init.size()] },
		m_end{ m_begin + init.size() },
		m_cap{ m_end }
	{
		// copy the data from init
		size_t i = 0;
		for (auto& e : init) {
			m_begin[i] = e;
			i++;
		}
	}

	// Parameterised Constructor with size
	Vector(const size_t size) :
		m_begin{ new double[size] },
		m_end{ m_begin + size },
		m_cap{ m_end }
	{}

	// Parameterised Constructor with pointers
	Vector(double* begin, double* end, double* cap) :
		m_begin{ begin },
		m_end{ end },
		m_cap{ cap }
	{}
	 
	// Copy Constructor
	Vector(const Vector& v) :
		m_begin{ new double[v.capaicty()] },
		m_end{ m_begin + v.size() },
		m_cap{ m_begin + v.capaicty() }
	{
		// Deep copy of data
		for (size_t i = 0; i < size(); i++)
			at(i) = v.at(i);
	}

	// Copy assignment operator
	Vector& operator = (const Vector& v) {
		m_begin = new double[v.capaicty()];
		m_end = m_begin + v.size();
		m_cap = m_begin + v.capaicty();

		// Deep copy of data
		for (size_t i = 0; i < size(); i++)
			at(i) = v.at(i);

		return *this;
	}

	// Move Constructor
	Vector(Vector&& v) :
		m_begin{ v.m_begin },
		m_end{ v.m_end },
		m_cap{ v.m_cap }
	{
		v.m_begin = nullptr;
		v.m_end = nullptr;
		v.m_cap = nullptr;
	}

	// Move assignment operator
	Vector& operator = (Vector&& v) {
		// steal the data
		std::swap(m_begin, v.m_begin);
		std::swap(m_end, v.m_end);
		std::swap(m_cap, v.m_cap);
		return *this;
	}

	// Destructor
	~Vector() {
		delete[] m_begin;
	}


	/// Indexing
	// range checking
	// operator []
	double& operator [] (const size_t i) const {
		if (i < size())
			return m_begin[i];
		else {
			// Error
			std::cout << "Index outside of range!\n";
			return m_begin[0];
		}
	}

	// .at is operator()[]
	double& at(const size_t i) const {
		return operator[](i);
	}


	/// iterators
	// used in for each loops
	double* begin() { return m_begin; }
	const double* begin() const { return m_begin; }
	double* end() { return m_end; }
	const double* end() const { return m_end; }

	// Access
	double*& getbegin() { return m_begin; }
	double*& getend() { return m_end; }
	double*& getcap() { return m_cap; }


	/// Size and Capacity 
	// size of vector
	const size_t size() const {
		return m_end - m_begin;
	}

	// capacity of vector
	const size_t capaicty() const {
		return m_cap - m_begin;
	}


	/// Functions from std::vector
	// extend the capacity m_cap of the vector
	void reserve(const size_t newCap) {

		// cannot use this to shrink
		if (newCap < capaicty()) return;

		// old variables
		size_t oldCap = capaicty();
		size_t length = size();

		// temp storage for the data
		double* temp = new double[newCap];

		// copy over the data
		for (size_t i = 0; i < oldCap; i++)
			temp[i] = m_begin[i];

		// delete the old data
		delete[] m_begin;

		// create the new sized array
		m_begin = new double[newCap];
		m_end = m_begin + length;
		m_cap = m_begin + newCap;

		// copy the data back
		for (size_t i = 0; i < oldCap; i++)
			m_begin[i] = temp[i];

		// delete the temp data
		delete[] temp;
	}

	// push back an element onto the end of the vector
	void push_back(double& e) {

		// if the vector is at capacity
		if (m_end == m_cap)
			doubleCapacity();	// double its capacity

		// m_end points to the first available slot
		*m_end = e;
		m_end++;
	}


	/// friend function ostream
	// overriding << operator
	friend std::ostream& operator << (std::ostream& os, const Vector& v);


	/// Cartesian stuff
	// Dot Product
	double operator * (const Vector& v) const {

		// Dimensions of vectors must match
		if (size() != v.size()) {
			// Error
			std::cout << "Error: Dimensions must match for dot product!" << std::endl;
			return 0.0f;
		}

		double result = 0;

		// sum of multiples
		for (size_t i = 0; i < size(); i++)
			result += at(i) * v.at(i);

		return result;
	}

	// scalar multiplication - commutative 
	Vector operator * (const double& d) const {

		// deep copy
		Vector result(*this);

		// multiply each element by the scalar
		for (auto& e : result)
			e *= d;

		return result;
	}

	// vector addition
	Vector operator + (const Vector& v) const {

		// Check dimensions
		if (size() != v.size()) {
			// Error
			std::cout << "Error: Dimensions must match for vector addition!" << std::endl;
			return Vector();
		}

		Vector result(size());

		// add each component
		for (size_t i = 0; i < size(); i++) {
			result.at(i) = at(i) + v.at(i);
		}

		return result;
	}

	// vector subtraction
	Vector operator - (const Vector& v) const {

		// Check dimensions
		if (size() != v.size()) {
			// Error
			std::cout << "Error: Dimensions must match for vector subtraction!" << std::endl;
			return Vector();
		}

		Vector result(size());

		// add each component
		for (size_t i = 0; i < size(); i++) {
			result.at(i) = at(i) - v.at(i);
		}

		return result;
	}


};

// scalar multiplication - commutative 
Vector operator * (const double& d, const Vector& v) {

	// deep copy
	Vector result(v);
	for (auto& e : result)
		e *= d;

	return result;
}

// overriding << operator
std::ostream& operator << (std::ostream& os, const Vector& v) {

	os << "(";

	// loop over the vector v
	for (size_t i = 0; i < v.size(); i++) {
		os << v[i];
		// comma between each element
		if (i < v.size() - 1)
			os << ", ";
	}

	os << ")";

	return os;
}

// Minkowski four vector (x, y, z, ct)
class Four_Vector : public Vector {
public:

	// Default Constructor
	Four_Vector() :
		Vector({ 0.0f, 0.0f, 0.0f, 0.0f })
	{}

	// Parameterised Constructor
	Four_Vector(double x, double y, double z, double ct) :
		Vector({ x, y, z, ct })
	{}

	// Parameterised Constructor
	Four_Vector(Vector r, double ct) :
		Vector({ r[0], r[1], r[2], ct })
	{}

	// Copy Constructor
	Four_Vector(const Four_Vector& v) :
		Vector({ v[0], v[1], v[2], v[3] })
	{}

	// Copy assignment operator
	Four_Vector& operator = (const Four_Vector& v) {

		// Deep copy of data
		for (size_t i = 0; i < size(); i++)
			at(i) = v.at(i);

		return *this;
	}

	// Move Constructor
	Four_Vector(Four_Vector&& v) :
		Vector(v.getbegin(), v.getend(), v.getcap())
	{
		// destroy the old pointers
		v.getbegin() = nullptr;
		v.getend() = nullptr;
		v.getcap() = nullptr;
	}

	// Move assignment operator
	Four_Vector& operator = (Four_Vector&& v) {

		// steal the data
		getbegin() = v.getbegin();
		getend() = v.getend();
		getcap() = v.getcap();

		// destroy the old pointers
		v.getbegin() = nullptr;
		v.getend() = nullptr;
		v.getcap() = nullptr;

		return *this;
	}


	// a copy of the spacial components
	Vector r() const {
		return Vector({ at(0), at(1), at(2) });
	}

	// Accessors for 4-Vector components
	double& x() const { return at(0); }
	double& y() const { return at(1); }
	double& z() const { return at(2); }
	double& ct() const { return at(3); }


	// dot product 
	double operator * (const Four_Vector& v) const {
		return ct() * v.ct() - r() * v.r();
	}

	// Lorentz Boost
	Four_Vector lorentzBoost(const Vector velocity) const {	// relative velocity in units of c (beta = v / c)

		// check velocity is of length 3
		if (velocity.size() != 3) {
			// Error
			std::cout << "Error: velocity must be a vector of length 3!" << std::endl;
			return Four_Vector();
		}

		// gamma factor
		double gamma = 1 / sqrt(1 - velocity * velocity);

		// new four vector
		double ctprime = gamma * (ct() - velocity * r());
		Vector rprime = r() + (((gamma - 1) * (r() * velocity) / (velocity * velocity)) * velocity);

		return Four_Vector(rprime, ctprime);
	}


	/// friend function cout
	// overriding << operator for std::cout
	friend std::ostream& operator << (std::ostream& os, const Four_Vector& v);

};

// overriding << operator
std::ostream& operator << (std::ostream& os, const Four_Vector& v) {

	os << "(";

	// loop over the Four_Vector v
	for (size_t i = 0; i < v.size(); i++) {
		os << v[i];
		// comma between each element
		if (i < v.size() - 1)
			os << ", ";
	}

	os << ")";

	return os;
}

// Particle - all in natural units
class Particle {

public:

	Four_Vector m_position;
	// Velocity relative to the speed of light
	Vector m_velocity;
	// Mass in MeV / c^2
	double m_mass;
	
	// Default constructor
	Particle() :
		m_position( 0.0f, 0.0f, 0.0f, 0.0f ),
		m_velocity( Vector{ 0.0f, 0.0f, 0.0f } ),	 //*!* have to copy construct
		m_mass{ 0.0f }
	{}
	

	// Parameterised constructor
	Particle(Four_Vector position, Vector velocity, double mass) :
		m_position{ position },
		m_velocity( velocity ),		//*!* () instead of {}
		m_mass{ mass }
	{
		if (velocity.size() != 3) {
			// Error
			std::cout << "Error: velocity must be a vector of length 3!" << std::endl;
			velocity = { 0.0f, 0.0f, 0.0f }; 
		}
	}


	// Gamma factor
	double gamma() const {
		return 1 / sqrt(1 - m_velocity * m_velocity);
	}

	// Energy 
	double energy() const {
		return gamma() * m_mass;
	}

	// Momentum
	Vector momentum() const {
		return gamma() * m_mass * m_velocity;
	}

};



int main() {

	// Vector
	std::cout << "Demo of Vector class: \n \n";

	// default constructor
	std::cout << "Using Vector's default constructor\n";
	Vector vector1;
	std::cout << "Size of a default vector should be 0: " << vector1.size() << "\n";

	// seperator 
	std::cout << "\n--------------------------------------------------------------------\n\n";

	// parameterised constructor
	std::cout << "Using Vector's parameterised constructor to create testVector\n";
	Vector vector2{ 1.0f, 2.0f };
	std::cout << "Printing out testVector: " << vector2 << "\n";

	// seperator 
	std::cout << "\n--------------------------------------------------------------------\n\n";

	// copy constructing 
	std::cout << "Using Vector's copy constructor\n";
	Vector vector3(vector2);
	std::cout << "Copy constructing: " << vector3 << "from: " << vector2 << "\n";
	vector2 = vector2 * 100;
	std::cout << "Changed initial vector to: " << vector2 << " and the new vector remains unchanged: " << vector3 << "\n";

	// seperator 
	std::cout << "\n--------------------------------------------------------------------\n\n";

	// copy assignment
	std::cout << "Using Vector's copy assignment\n";
	Vector vector4{ 1.0f, 2.0f, 3.0f };
	Vector vector5(vector4);
	std::cout << "Copy assigning: " << vector4 << " to a new vector: " << vector5 << "\n";
	vector4 = vector4 * 0.01f;
	std::cout << "Changed the initial vector to: " << vector4 << " and the new vector remains unchanged: " << vector5 << "\n";

	// seperator 
	std::cout << "\n--------------------------------------------------------------------\n\n";

	// move construction
	std::cout << "Using Vector's move constructor\n";
	Vector vector6{ 5.0f, 1.0f };
	std::cout << "Move constructing from: " << vector6 << "\n";
	Vector vector7(vector6);
	std::cout << "Vector should now be the same: " << vector7 << "\n";

	// seperator 
	std::cout << "\n--------------------------------------------------------------------\n\n";

	// move assignment
	std::cout << "Using Vector's move assignment\n";
	Vector vector8 = vector7;
	std::cout << "Vector should now be the same as before: " << vector8 << "\n";

	// seperator 
	std::cout << "\n--------------------------------------------------------------------\n\n";

	// indexing
	std::cout << "Indexing\n";
	Vector vector9{ 10.0f, 3.0f, -1.0f };
	std::cout << "The first element of: " << vector9 << " is: " << vector9[0] << "\n";

	// seperator 
	std::cout << "\n--------------------------------------------------------------------\n\n";

	// dot product
	Vector vector10{ 1.0f, 2.0f };
	Vector vector11{ 3.0f, 4.0f };

	std::cout << "Dot product\n";
	std::cout << vector10 << " * " << vector11 << " = " << vector10 * vector11 << "\n";

	// double seperator 
	std::cout << "\n--------------------------------------------------------------------\n\n";
	std::cout << "\n--------------------------------------------------------------------\n\n";

	// Minkowski Four Vector
	std::cout << "Demo of Minkowski Four Vector: \n \n";

	// Parameterised constructors:
	Four_Vector fvector1{ 0.0f, 1.0f, 2.0f, 3.0f };
	Vector vector12{ 4.0f, 5.0f, 6.0f };
	Four_Vector fvector2(vector12, 7.0f);

	std::cout << "Parameterised constructor from 4 doubles: " << fvector1 << "\n";
	std::cout << "Parameterised constructor from a vector and a double: " << fvector2 << "\n";

	// seperator 
	std::cout << "\n--------------------------------------------------------------------\n\n";

	// copy constructing 
	std::cout << "Using Four_Vector's copy constructor\n";
	Four_Vector fvector3(fvector1);
	std::cout << "Copy constructing: " << fvector3 << " from: " << fvector1 << "\n";
	fvector1[0] = 100.0f;
	std::cout << "Changed initial Four_Vector to: " << fvector1 << " and the new Four_Vector remains unchanged: " << fvector3 << "\n";

	// seperator 
	std::cout << "\n--------------------------------------------------------------------\n\n";

	// copy assignment
	std::cout << "Using Four_Vector's copy assignment\n";
	Four_Vector fvector4 = fvector1;
	std::cout << "Copy assigning: " << fvector1 << " to a new vector: " << fvector4 << "\n";
	fvector1[0] = 0.0f;
	std::cout << "Changed initial Four_Vector to: " << fvector1 << " and the new Four_Vector remains unchanged: " << fvector4 << "\n";

	// seperator 
	std::cout << "\n--------------------------------------------------------------------\n\n";

	// move construction
	std::cout << "Using Four_Vector's move constructor\n";
	Four_Vector fvector5{ 5.0f, 1.0f, 3.0f, 10.0f };
	std::cout << "Move constructing from: " << fvector5 << "\n";
	Four_Vector fvector6(fvector5);
	std::cout << "Four_Vector should now be the same: " << fvector6 << "\n";

	// seperator 
	std::cout << "\n--------------------------------------------------------------------\n\n";

	// move assignment
	std::cout << "Using Four_Vector's move assignment\n";
	Four_Vector fvector7 = fvector6;
	std::cout << "Four_Vector should now be the same as before: " << fvector7 << "\n";

	// seperator 
	std::cout << "\n--------------------------------------------------------------------\n\n";

	// Accessors 
	std::cout << "Accessors\n";
	std::cout << fvector7 << " Components: " << fvector7.x() << ", " << fvector7.y() << ", " << fvector7.z() << ", " << fvector7.ct() << "\n";

	// seperator 
	std::cout << "\n--------------------------------------------------------------------\n\n";

	// Dot product
	std::cout << "Dot product of a four vector\n";
	Four_Vector fvector8{ 1.0f, 2.0f, 3.0f, 4.0f };
	Four_Vector fvector9{ 4.0f, 3.0f, 2.0f, 1.0f };
	std::cout << fvector8 << " * " << fvector9 << " = " << fvector8 * fvector9 << "\n";

	// seperator 
	std::cout << "\n--------------------------------------------------------------------\n\n";

	// Lorentz Boost
	std::cout << "Lorentz Boost\n";
	Vector boost{ 0.5f, 0.0f, 0.0f };
	Four_Vector fvector10{ 1.0f, 2.0f, 3.0f, 4.0f };
	std::cout << "Four vector: " << fvector10 << "\n";
	std::cout << "Boost: " << boost << "\n";
	std::cout << "Lorentz boosted four vector: " << fvector10.lorentzBoost(boost) << "\n";
	
	// double seperator 
	std::cout << "\n--------------------------------------------------------------------\n\n";
	std::cout << "\n--------------------------------------------------------------------\n\n";

	// Particle class
	std::cout << "Demo of Particle class: \n \n";

	// Parameterised constructor
	std::cout << "Parameterised constructor\n";

	Four_Vector position{ -10.0f, 42.0f, 2.4643f, 0.0f };
	Vector velocity{ 0.5f, 0.0f, 0.0f };
	double mass{ 3.0f };

	Particle particle(position, velocity, mass);

	// seperator 
	std::cout << "\n--------------------------------------------------------------------\n\n";

	// gamma
	std::cout << "Gamma = " << particle.gamma() << "\n";

	// energy
	std::cout << "Energy = " << particle.energy() << " MeV \n";

	// momentum 
	std::cout << "Momentum = " << particle.momentum() << " MeV \n";

	std::cout << "\nIn natural units\n";

	// seperator 
	std::cout << "\n--------------------------------------------------------------------\n\n";


	char c;
	std::cin >> c;
	std::cout << c;

	return 0;
}
