/// Assignment 2
/// Calculate the mean, error on the mean and the standard deviation of data in a .dat file

// Mean, standard deviation and error on the mean are calculated in seperate functions
// StringToDouble uses atof to convert from a string to a double with error checking

#include <iostream>
#include <fstream>
#include <cmath>
#include <string>

using namespace std;

// Calculate the mean
double Mean(const double* data, const int dataLength) {

	// compute the sum of the data
	double sum{ 0 };
	for (int i = 0; i < dataLength; ++i)
		sum += *(data + i);

	// return the mean
	return sum / dataLength;
}

// Calculate the standard deviation of the data
double StandardDeviation(const double* data, const int dataLength, const double mean) {
	// compute the sum of the squared deviation of each element
	double sum{ 0 };
	for (int i = 0; i < dataLength; ++i)
		sum += pow(*(data + i) - mean, 2);

	// return the standard deviation
	return sqrt(sum / (dataLength - 1));
}

// Calculate the standard error on the mean 
double StandardError(const double standardDeviation, const int dataLength) {
	return standardDeviation / sqrt(dataLength);
}

// convert from string to double, print error on invalid input
bool StringToDouble(string& _string, double& _double) {
	
	if (!_string.empty() && atof(_string.data())) {				// is the string empty
		_double = atof(_string.data());	// convert string to double, will truncate if non digit chars are found
		return true;
	}
	else {
		cout << "Error: found invalid input! Calculation will ignore it.\n";
		return false;
	}
}

int main() {

	// ask the user for the file name with extension
	string myFileName;
	ifstream myFile;
	bool success{ false };
	do {
		cout << "Enter the file name with the extension: ";
		cin >> myFileName;
		myFile.open(myFileName);
		if (myFile.fail()) {
			cout << "Invalid Name!\n";
			myFile.clear();		// clear the fail flag
			myFile.ignore();
		}
		else
			success = true;
	} while (!success);

	// Check if the file is still open
	if (myFile.is_open()) {
	
		// find the number of lines in the file;
		unsigned int numberOfLines{ 0 };	
		string line;	// used as a temp hold for strings

		while (getline(myFile, line))	// while getline doesn't fail
			++numberOfLines;
		myFile.clear(); // as the wile loop relies on the .eof flag
		myFile.ignore();

		myFile.seekg(0, ios::beg); // go back to the begining of the file

		// allocate memory for data 
		double* data = new double[numberOfLines];	// array will fill such that any empty slots will be at the end
		int dataLength{ 0 };						// keeps track of data's (valid) length

		// read in the data
		for (int i = 0; i < numberOfLines; ++i) {
			if (getline(myFile, line)) {
				double _double;
				if (StringToDouble(line, _double)) {// change string to double and discard non valid data
					data[dataLength] = _double;
					++dataLength;
				}
			}
			else
				cout << "Failed to read in line: " << i << " of file: " << myFileName.data() << "\n";
		}

		// close the file
		myFile.close();

		// print number of measurements read in
		printf("%s %i %s", "Number of measurements read in: ", dataLength, "\n");

		// Calculate the mean, the error in the mean (standard error) and the standard deviatoin
		double mean = Mean(data, dataLength);
		double standardDeviation = StandardDeviation(data, dataLength, mean);
		double standardError = StandardError(standardDeviation, dataLength);

		printf("%s %3.2e %s %3.2e %s %3.2e %s", "Mean: ", mean, " +/- ", standardError, "\nStandard Deviation: ", standardDeviation, "\n");

		// delete the array (free memory)
		delete[] data;

		// Pause at the end to read results
		cout << "Enter any key to continue: ";
		string x;
		cin >> x;
		cout << x.data();

	}

	return 0;
}