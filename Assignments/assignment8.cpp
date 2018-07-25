// Assignment 8
// Abstract Shape, Shape2D and Shape3d classes with 4 2D and 4 3D derived shapes and a prism class
// Shape declares area/volume pure virtual functions and a virtual destructor
// Shape2D adds the pure virtual perimeter function used by the prism class
// main demonstrates their usage

#include <math.h>
#include <iostream>
#include <string>

const double PI = 3.141592653589793;

// Abstract base class for 2/3D shapes
class Shape {

public:

	// Constructor
	Shape() {}

	// Virtual Destructor
	virtual ~Shape() {}

	// Virtual area function
	virtual double area() const = 0;

	// Virtual volume function
	virtual double volume() const = 0;

	// Print data
	void print() const {
		std::cout << "Area: " << area() << ", Volume: " << volume() << std::endl;
	}

};

// Abstract base class for 2D shapes
class Shape2D : public Shape {

public:

	// Constructor
	Shape2D() :
		Shape()
	{}

	// Virtual Destructor
	virtual ~Shape2D() {}

	// Virtual perimeter function - used by prism
	virtual double perimeter() const = 0;

	// Virtual area function
	virtual double area() const = 0;

	// Volume function - 2D shapes have 0 volume
	double volume() const { return 0.0f; }
};

// Abstract base class for 3D shapes
class Shape3D : public Shape {

public:

	// Constructor
	Shape3D() :
		Shape()
	{}

	// Virtual Destructor
	virtual ~Shape3D() {}

	// Virtual area function
	virtual double area() const = 0;

	// Virtual volume function
	virtual double volume() const = 0;
};

// Rectangle
class Rectangle : public Shape2D {

public:

	// Constructor
	Rectangle(double width, double height) :
		Shape2D(),
		m_width{ abs(width) },
		m_height{ abs(height) }
	{}

	// Overriden Destructor
	~Rectangle() override {
		std::cout << "Rectangle Destructor!" << std::endl;
	}

	// Perimeter
	double perimeter() const override {
		return 2 * (m_width + m_height);
	}

	// Area
	double area() const override {
		return m_width * m_height;
	}

protected:
	// Rectangle Dimensions
	// width and height will be positive (using abs() function)
	double m_width;
	double m_height;

};

// Square - is a rectangle
class Square : public Rectangle {

public:

	// Constructor
	Square(double length) :
		Rectangle(length, length)
	{}

	// Overriden Destructor
	~Square() override {
		std::cout << "Square Destructor!" << std::endl;
	}

};

// Ellipse
class Ellipse : public Shape2D {

public:

	// Constructor
	Ellipse(double semiMajorAxis, double semiMinorAxis) :
		Shape2D(),
		m_semiMajorAxis{ abs(semiMajorAxis) },
		m_semiMinorAxis{ abs(semiMinorAxis) }
	{}

	// Overriden Destructor
	~Ellipse() override {
		std::cout << "Ellipse Destructor!" << std::endl;
	}

	// Perimeter - Approximate formula - virtual so circle can override with the exact formula
	virtual double perimeter() const override {
		return 2 * PI * pow((pow(m_semiMajorAxis, 2) + pow(m_semiMinorAxis, 2)) / 2, 0.5f);
	}

	// Area
	double area() const override {
		return m_semiMajorAxis * m_semiMinorAxis * PI;
	}


protected:
	// Ellispe Dimensions
	double m_semiMajorAxis;
	double m_semiMinorAxis;

};

// Circle - is an ellipse
class Circle : public Ellipse {

public:

	// Constructor
	Circle(double radius) :
		Ellipse(radius, radius)
	{}

	// Overriden Destructor
	~Circle() override {
		std::cout << "Circle Destructor!" << std::endl;
	}

	double perimeter() const override {
		return 2 * PI * pow(m_semiMajorAxis, 2);
	}

};

// Cuboid
class Cuboid : public Shape3D {

public:

	// Constructor
	Cuboid(double width, double height, double length) :
		Shape3D(),
		m_width{ abs(width) },
		m_height{ abs(height) },
		m_length{ abs(length) }
	{}

	// Overriden Destructor
	~Cuboid() override {
		std::cout << "Cuboid Destructor!" << std::endl;
	}

	// Area
	double area() const override {
		return 2 * (m_width * m_height + m_width * m_length + m_height * m_length);
	}

	// Volume
	double volume() const override{
		return m_width * m_height * m_length;
	}

protected:
	// Cuboid Dimensions
	double m_width;
	double m_height;
	double m_length;

};

// Cube - is a cuboid
class Cube : public Cuboid {

public:

	// Constructor
	Cube(double length) :
		Cuboid(length, length, length)
	{}

	// Overriden Destructor
	~Cube() override {
		std::cout << "Cube Destructor!" << std::endl;
	}

};

// Ellipsoid
class Ellipsoid : public Shape3D {

public:

	// Constructor
	Ellipsoid(double axisX, double axisY, double axisZ) :
		Shape3D(),
		m_axisX{ axisX },
		m_axisY{ axisY },
		m_axisZ{ axisZ }
	{}

	// Overriden Destructor
	~Ellipsoid() override {
		std::cout << "Ellipsoid Destructor!" << std::endl;
	}

	// Area - Approximate formula - virtual so sphere can override with the exact formula
	virtual double area() const override {
		// p is an approximation - it is a constant used in the formula
		double p = 1.6075f;
		return 4 * PI * pow((pow(m_axisX * m_axisY, p) + pow(m_axisX * m_axisZ, p) + pow(m_axisY * m_axisZ, p)) / 3, 1 / p);
	}

	// Volume
	double volume() const override {
		return m_axisX * m_axisY * m_axisZ * PI * 4 / 3;
	}

protected:
	// Ellipsoid Dimensions - the largest and smallest diameters are alligned to 2 of the axis and the perpendicular diameter is the 3rd axis
	double m_axisX;
	double m_axisY;
	double m_axisZ;

};

// Sphere - is an ellipsoid
class Sphere : public Ellipsoid {

public:
	// Constructor
	Sphere(double radius) :
		Ellipsoid(radius, radius, radius)
	{}

	// Overriden Destructor
	~Sphere() override {
		std::cout << "Sphere Destructor!" << std::endl;
	}

	// Area - Exact
	double area() const override {
		return 4 * PI * pow(m_axisX, 2);
	}

};

// Prism
class Prism : public Shape3D {

public:

	// Constructor
	Prism(Shape2D* shape2d, double depth) :
		Shape3D(),
		m_shape2d{ shape2d },
		m_depth{ depth }
	{}

	// Overriden Destructor
	~Prism() override {
		std::cout << "Sphere Destructor!" << std::endl;
		// call the destructor of the 2d shape
		delete m_shape2d;
	}

	// Area
	double area() const override {
		return 2 * m_shape2d->area() + m_depth * m_shape2d->perimeter();
	}

	// Volume
	double volume() const override {
		return m_depth * m_shape2d->area();
	}

protected:
	// Prism dimensions
	Shape2D* m_shape2d;
	double m_depth;
};

int main() {

	// to keep track of the length of the shapes array
	const unsigned int numberOfShapes = 10;

	// array of pointers to the abstract Shape base class 
	Shape* shapes[numberOfShapes] = {
		new Rectangle(1.0f, 2.0f),
		new Square(3.0f),
		new Ellipse(4.0f, 5.0f),
		new Circle(6.0f),
		new Cuboid(1.0f, 2.0f, 3.0f),
		new Cube(4.0f),
		new Ellipsoid(5.0f, 6.0f, 7.0f),
		new Sphere(8.0f),
		new Prism(new Rectangle(9.0f, 10.0f), 11.0f),
		new Prism(new Circle(12.0f), 13.0f)
	};

	// array of names for clarity when printing out
	std::string names[numberOfShapes] = {
		"Rectangle",
		"Square",
		"Ellipse",
		"Circle",
		"Cuboid",
		"Cube",
		"Ellipsoid",
		"Sphere",
		"Cuboid (Rectangle prism)",
		"Cylinder (Circle prism)"
	};

	// print data
	for (unsigned int i = 0; i < numberOfShapes; i++) {
		std::cout << names[i] << std::endl;
		shapes[i]->print();
		std::cout << std::endl;
	}

	// Call destructors
	for (unsigned int i = 0; i < numberOfShapes; i++) {
		delete shapes[i];
		std::cout << std::endl;
	}


	// Pause at the end of the program
	char c;
	std::cin >> c;
	std::cout << c;

	return 0;
}