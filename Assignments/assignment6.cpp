// Assignment 6 - Class for a matrix
// constructors, destructor, move/copy constructors/destructors
// Overloaded + - * friends: << >>

//			j - - - - >
//		i	1	2	3
//		|	4	5	6
//		|	7	8	9
//		v

#include <iostream>
#include <string>
#include <sstream>

using namespace std;

class Matrix {
private:
	// number of rows
	size_t m_rows;
	// number of columns
	size_t m_columns;
	// pointer to array for matrix data
	double* m_matrixData;
public:
	// Default Constructor
	Matrix() :
		m_matrixData{ nullptr },
		m_columns{ 0 },
		m_rows{ 0 }
	{}

	// Parameterised Constructor, array of 0s
	Matrix(size_t rows, size_t columns) :
		m_matrixData{ new double[rows * columns] },	// array of 0s TODO check
		m_columns{ columns },
		m_rows{ rows }
	{
		for (size_t i = 0; i < rows * columns; i++) {
			m_matrixData[i] = 0.0f;
		}
	}

	// Parameterised Constructor with initializer list
	Matrix(size_t rows, size_t columns, const initializer_list<double> list) :
		m_matrixData{ new double[rows * columns] },
		m_columns{ columns },
		m_rows{ rows }
	{
		// loop over the initializer_list
		size_t i = 0;
		for (auto& e : list) {

			// Error if overflow matrix size
			if (i == m_rows * m_columns) { cout << "Error: exceeded matrix size!\n";	return; }

			// add each element to the matrix
			m_matrixData[i] = e;
			i++;	// increment the index
		}
	}

	// Copy Constructor
	Matrix(const Matrix& m) :
		m_matrixData{ nullptr },
		m_columns{ m.m_columns },
		m_rows{ m.m_rows }
	{
		size_t size = m_columns * m_rows;

		// if data is valid for copying
		if (m.m_matrixData != nullptr && size != 0) {

			// declare new same sized array
			m_matrixData = new double[size];

			// copy all its data
			for (size_t i = 0; i < size; ++i)
				m_matrixData[i] = m.m_matrixData[i];
		}
	}

	// Copy assignment operator
	Matrix& operator = (Matrix& m) {

		// No self assignment
		if (&m == this) return *this;

		// delete the old data
		delete[] m_matrixData;

		// copy the data
		m_columns = m.m_columns;
		m_rows = m.m_rows;
		size_t size = m_columns * m_rows;

		// if data is valid for copying
		if (m.m_matrixData != nullptr && size != 0) {

			// declare new same sized vector
			m_matrixData = new double[size];

			// copy all its data
			for (size_t i = 0; i < size; ++i)
				m_matrixData[i] = m.m_matrixData[i];
		}

		return *this;
	}

	// Move Constructor
	Matrix(Matrix&& m) :
		// steal the data
		m_matrixData{ m.m_matrixData },	// copy the pointer
		m_columns{ m.m_columns },
		m_rows{ m.m_rows }
	{
		// delete the old data
		m.m_matrixData = nullptr;
		m.m_columns = 0;
		m.m_rows = 0;
	}

	// Move assignment operator
	Matrix& operator = (Matrix&& m) {
		// steal the data
		swap(m_matrixData, m.m_matrixData);
		swap(m_columns, m.m_columns);
		swap(m_rows, m.m_rows);
		return *this;
	}

	// Destructor
	~Matrix() {
		delete[] m_matrixData;
	}

	/// Operator overriding ////////////////////////////////////////////////////////

	// overriding << operator
	friend ostream& operator << (ostream& os, const Matrix& m);	// TODO const here?

	// overriding >> operator
	friend istream& operator >> (istream& is, Matrix& m);

	// overriding +
	Matrix operator + (const Matrix& m) const {

		// if the dimensions don't match
		if (m_rows != m.m_rows && m_columns != m.m_columns) {
			// Error
			cout << "Error: martix dimension mismatch!\n";
			return Matrix();
		}

		// declare a new matrix of the same size
		Matrix result(m_rows, m_columns);

		// loop through the matrix and add each element
		for (size_t i = 0; i < m_rows * m_columns; i++) {
			result[i] = m_matrixData[i] + m.m_matrixData[i];
		}
		return result;
	}

	// overriding -
	Matrix operator - (const Matrix& m) const {

		// if the dimensions don't match
		if (m_rows != m.m_rows && m_columns != m.m_columns) {
			// Error
			cout << "Error: martix dimension mismatch!\n";
			return Matrix();
		}

		// declare a new matrix of the same size
		Matrix result(m_rows, m_columns);

		// loop through the matrix and add each element
		for (size_t i = 0; i < m_rows * m_columns; i++) {
			result[i] = m_matrixData[i] - m.m_matrixData[i];
		}
		return result;
	}

	// overriding *
	Matrix operator * (const Matrix& m) const {
		// size check
		if (m_columns != m.m_rows) {
			// Error
			cout << "Error: inner dimensions must match for matrix multiplication!\n";
			return Matrix();
		}

		// new parameters
		size_t newRows = m_rows;
		size_t newColumns = m.m_columns;
		size_t commonDimension = m_columns;

		// resulting matrix
		Matrix result(newRows, newColumns);

		// loop over the new matrix
		for (size_t i = 0; i < newRows; i++) {
			for (size_t j = 0; j < newColumns; j++) {
				// summation of multiplication
				for (size_t k = 0; k < commonDimension; k++) {
					result.index0(i, j) += index0(i, k) * m.index0(k, j);
				}
			}
		}
		return result;
	}

	/// Indexing //////////////////////////////////////////////////////////////////
	// Return non constant references to the data

	/// Index starting at 0
	// i and j of matrix
	double& index0(const size_t i, const size_t j) const {
		// Bounds checking
		if (i >= 0 && i < m_rows && j >= 0 && j < m_columns)
			return m_matrixData[j + i * m_columns];

		// Error	// TODO what if size = 0?
		cout << "Error: index out of range!\n";
		return m_matrixData[0];
	}

	// i of underlying array
	double& index0(const size_t i) const { return index0(i / m_columns, i % m_columns); }

	// operator overload for indexing underlying array
	double& operator [] (const size_t i) const { return index0(i); }

	/// Index starting at 1
	// i and j of matrix
	double& index1(const size_t i, const size_t j) const { return index0(i - 1, j - 1); }

	// operator overload for indexing underlying array
	double& operator () (const size_t i, const size_t j) const { return index1(i, j); }

	/// Functions //////////////////////////////////////////////////////////////////////

	// return martix with row and column deleted
	Matrix subMatrix(const size_t row, const size_t column) const {

		// create result matrix 
		Matrix result(m_rows - 1, m_columns - 1);

		// index of result's array
		size_t k = 0;

		// loop over origonal matrix
		for (size_t i = 0; i < m_rows; i++) {
			if (i != row) {
				for (size_t j = 0; j < m_columns; j++) {
					if (j != column) {
						// if not one of the deleted elements add it to the new matrix
						result[k] = index0(i, j);
						k++;	// increment the new matrix index seperately
					}
				}
			}
		}
		return result;
	}

	// calculate the determinant recursively 
	double determinant() const {

		// if not a square matrix
		if (m_columns != m_rows) {
			// Error
			cout << "Error: matrix must be square to calculate its determinant!\n";
			return -1.0f; // not 0.0f as this is a special case
		}

		// the determinant of a 1 by 1 matrix is itself
		if (m_columns == 1) return index0(0);

		// determinant
		double result = 0;

		// along the top row
		for (size_t i = 0; i < m_columns; i++) {

			// if i is odd
			if (i % 2)	result -= index0(i) * subMatrix(0, i).determinant();
			// if i is even
			else		result += index0(i) * subMatrix(0, i).determinant();
		}

		// return the result
		return result;
	}

};

// overriding << operator
ostream& operator<<(ostream& os, const Matrix& m) {

	// Loop through every matrix element 
	for (unsigned int i = 0; i < m.m_rows; ++i) {
		for (unsigned int j = 0; j < m.m_columns; ++j) {
			// Tab between each element
			os << m.m_matrixData[j + i * m.m_columns] << "\t";
		}
		// new line at the end of every row
		os << "\n";
	}
	return os;
}

// overriding >> operator
istream& operator >> (istream& is, Matrix& m) {
	// each row will end with a new line
	// the whole thing will end with a double new line

	// create initial array
	size_t size = 4;
	double* arr = new double[size];
	size_t index = 0;	// index for arr
	size_t columns = 0;	// number of columns
	bool firstRow = true;

	bool finished = false;
	while (!finished) {

		// wait for a line to be written
		// read it in using stringstreams
		stringstream stream;
		string temp;
		getline(is, temp);
		stream << temp;

		// current row length
		size_t rowLength = 0;

		// ignore whitespace
		//stream >> skipws;

		// if the stream doesn't contain anything, the matrix is finished
		finished = !(stream.rdbuf()->in_avail());		// it is possible to make a 0 size matrix

		// if need to extend the array
		if (!finished && (size < columns + index)) {

			
			// round size down if it isnt a multiple of columns and then double it
			size_t oldsize = size;
			size -= size % columns;
			size *= 2;
			// temp storage of data
			double* newArr = new double[oldsize];
			for (size_t i = 0; i < oldsize; i++) {
				newArr[i] = arr[i];
			}

			// delete old data
			delete[] arr;

			// allocate new memory
			arr = new double[size];

			// copy over data
			if (oldsize > size) {
				for (size_t i = 0; i < size; i++) {
					arr[i] = newArr[i];
				}
			}
			else {
				for (size_t i = 0; i < oldsize; i++) {
					arr[i] = newArr[i];
				}
			}

			// delete the temp array
			delete[] newArr;
		}

		// while not at the end of the line
		while (!finished && !stream.eof()) {

			// if the next character is a digit
			if (isdigit(stream.peek()) || stream.peek() == '-') {

				// if on the first row the index has exceeded the array size
				if (firstRow && index == size) {

					// double array size
					size_t oldsize = size;
					size *= 2;

					// temp storage of data
					double* newArr = new double[oldsize];
					for (size_t i = 0; i < oldsize; i++) {
						newArr[i] = arr[i];
					}

					// delete old data
					delete[] arr;

					// allocate new memory
					arr = new double[size];

					// copy over data
					if (oldsize > size) {
						for (size_t i = 0; i < size; i++) {
							arr[i] = newArr[i];
						}
					}
					else {
						for (size_t i = 0; i < oldsize; i++) {
							arr[i] = newArr[i];
						}
					}

					// delete the temp array
					delete[] newArr;
				}

				// read into the array
				stream >> arr[index];

				if (stream.fail()) {
					stream.clear();
					stream.ignore();
				}
				else {
					// increment the index and row length
					index++;
					rowLength++;
				}
			}

			// else ignore the character
			else stream.ignore(1);
		}

		// clear the eof flag
		stream.clear();

		// if on first row
		if (firstRow) {

			// now know the nubmer of columns
			columns = rowLength;
			firstRow = false;
		}

		// if the row is the wrong length
		else if (rowLength % columns) {
			// Error, row not the right size
			cout << "Error: row length " << rowLength << " not the correct size " << columns << "!\n";

			// default matrix
			m = Matrix();

			// clean up
			delete[] arr;

			return is;
		}
	}

	// shrink to fit data
	size_t oldsize = index;

	// temp storage of data
	double* newArr = new double[oldsize];
	for (size_t i = 0; i < oldsize; i++) {
		newArr[i] = arr[i];
	}

	// delete old data
	delete[] arr;

	// allocate new memory
	arr = new double[size];

	// copy over data
	if (oldsize > size) {
		for (size_t i = 0; i < size; i++) {
			arr[i] = newArr[i];
		}
	}
	else {
		for (size_t i = 0; i < oldsize; i++) {
			arr[i] = newArr[i];
		}
	}

	// delete the temp array
	delete[] newArr;

	// copy the data
	m.m_columns = columns;
	m.m_rows = index / columns;

	m.m_matrixData = new double[index];
	for (size_t i = 0; i < index; i++) {
		m.m_matrixData[i] = arr[i];
	}

	// clean up
	delete[] arr;

	return is;
}

int main() {


	Matrix m1(2, 3, { 1.0f, 2.0f, 3.0f,
		4.0f, 5.0f, 6.0f });

	Matrix m2(2, 3, { 6.0f, 5.0f, 4.0f,
		3.0f, 2.0f, 1.0f });

	Matrix m3(3, 2, { 1.0f, 2.0f,
		3.0f, 4.0f,
		5.0f, 6.0f });

	Matrix m4(3, 3, { 1.0f, 2.0f, 3.0f,
		4.0f, 5.0f, 6.0f,
		7.0f, 8.0f, 9.0f });

	// operator overloading

	// +
	cout << "Adding\n";
	cout << m1 << " +\n" << m2 << " =\n" << m1 + m2 << "\n---------------------------------------------\n";

	// -
	cout << "Subtracting\n";
	cout << m1 << " -\n" << m2 << " =\n" << m1 - m2 << "\n---------------------------------------------\n";

	// *
	cout << "Multiplying\n";
	cout << m1 << " *\n" << m3 << " =\n" << m1 * m3 << "\n---------------------------------------------\n";

	// determinant
	cout << "The determinant of: \n" << m4 << "is: \n" << m4.determinant() << "\n---------------------------------------------\n";

	// subMatrix
	cout << "Submatrix(0, 0) of: \n" << m4 << "is: \n" << m4.subMatrix(0, 0) << "\n---------------------------------------------\n";

	// copy assignment
	cout << "Copy assignment!\n";
	m1 = m2;
	cout << "Matrix 1: \n" << m1 << "is now the same as matrix 2: \n" << m2 << "\n---------------------------------------------\n";

	// copy constructor
	Matrix m6(m1);
	cout << "Martix 6: \n" << m6 << "constructed with matrix 1: \n" << m1 << "\n---------------------------------------------\n";

	// move assignment
	cout << "Matrix 3: \n" << m3 << "Matrix 1: \n" << m1;
	cout << "Move assigning matrix 3 to matrix 1.\n";
	m1 = move(m3);
	cout << "Matrix 3: \n" << m3 << "Matrix 1: \n" << m1 << "\n---------------------------------------------\n";

	// move constructing
	cout << "Matrix 1: \n" << m1;
	cout << "Move constructing matrix 7 from matrix 1.\n";
	Matrix m7(move(m1));
	cout << "Matrix 7: \n" << m7;
	cout << "Matrix 1 has been deleted; \n" << m1 << "\n---------------------------------------------\n";

	// << and >> overloading
	cout << "Enter a matrix:\n";
	cout << "Use an endline to denote the end of a row and another newline when done.\n";
	Matrix m5;
	cin >> m5;
	cout << m5 << "\n---------------------------------------------\n";

	

	// End of program
	cout << "Enter any character to end the program:\n";
	char c;
	cin >> c;
	cout << c;


	return 0;
}