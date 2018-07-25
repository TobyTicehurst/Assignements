/// Assignment 1
/// Calculating the transition energy using the Bohr formula

// How it works:
// getInput() asks the user for input and holds it in a string. It tests for programming errors eg cin.fail()
// stringToInt() then tries to convert to int and returns true when successful. 
// transitionEnergy() then calculates the energy in J or eV
// the answer is then output using printf to aid formatting (%3.2e)
// y/n to repeat the program

// is Int returns true when every char in a string is a decimal digit. it uses isdigit() in <ctype.h>
// cinFailed tests and deals with cin.fail()

#include <iostream>
#include <stdio.h>
#include <string>
#include <ctype.h>

using namespace std;

// calculate transition energy
float transitionEnergy(unsigned int atomicNumber, unsigned int energyLevel_1, unsigned int energyLevel_2, bool inJoules) { // inJoules is true if output in J, false if in eV

	// Rydberg unit of energy, first in J and then in eV
	float energyRydberg;
	if (inJoules)
		energyRydberg = 2.176e-18f;
	else
		energyRydberg = 13.6f;

	// casts are a c++ feature
	return (energyRydberg * (atomicNumber * atomicNumber) * (1 / float(energyLevel_2 * energyLevel_2) - 1 / float(energyLevel_1 * energyLevel_1)));
}

// tests if every char in a string is a decimal digit using isdigit()
bool isInt(string& testString) {
	// C++11 range based for loop
	for (char& c : testString) {
		if (!isdigit(c))
			return false;
	}
	return true;
}

// tests if cin has failed, if so it cleans up
bool cinFailed() {
	if (cin.fail()) {	// test for failed input
		cin.clear();	// clear any flags
		cin.ignore(numeric_limits<streamsize>::max(), '\n');	// ignore the stream up till \n, and defined max stream size
		cout << "Error: std::cin failed!\n";
		return true;
	}
	return false;
}

// ask the user for input, test its programming validity and return as a string
string getInput(string& request) {
	bool success{ false };
	string input;
	do {										// ask the user till a valid input is given
		cout << request;
		getline(cin, input);
		if (!cinFailed() && !input.empty() && input.length() < 10)		// test for cin.fail() and empty string and a reasonably sized string
			success = true;
		else
			cout << "Please enter a valid input.\n";
	} while (!success);
	return input;
}

// converts a string to an int using stoul, asks the user to truncate on a semi-valid answer
// returns true on successful conversion
// intentionally changes integer to "return" more than one thing
bool stringToInt(string& _string, unsigned int& _integer) {
	// assert input.empty() == false

	if (isdigit(_string[0]))			// test first char so stoul has valid input
		_integer = stoul(_string);		// convert string to int, will truncate if non digit chars are found
	else {
		return false;
	}

	// if input is partially valid eg 123abc, will ask the user to truncate to 123
	if (!isInt(_string)) {

		string answer;
		do {						// ask the user till a valid input is given
			answer = getInput(string("Would you like to truncate the input to " + to_string(_integer) + "? (y/n): "));
		} while (!(answer == "y" || answer == "n"));

		if (answer == "y")
			return true;
		else
			return false;
	}
	else
		return true;
}

int main() {

	// boolian used for asking to repeat the calculation
	bool repeat;

	do { // do this till repeat is false
		// declare variables - could make them unsigned 

		unsigned int atomicNumber;
		unsigned int energyLevel_1;
		unsigned int energyLevel_2;

		// ask user to enter atomic number
		bool success{ false };	// success is used to keep asking till a valid answer is given
		do {
			if (stringToInt(getInput(string("Enter atomic number: ")), atomicNumber)) {
				if (atomicNumber > 0 && atomicNumber < 300) // atomic numbers are between 0 and 300
					success = true;
				else
					cout << "Please enter an atomic number between 0 and 300.\n";
			}
			else
				cout << "Non valid input. Please enter a value between 0 and 300.\n";

		} while (!success);

		// ask user to enter initial energy level
		success = false;
		do {
			if (stringToInt(getInput(string("Enter initial energy level: ")), energyLevel_1)) {
				if (energyLevel_1 > 0) // energy level can't be 0
					success = true;
				else
					cout << "Please enter a positive energy level.\n";
			}
			else
				cout << "Non valid input. Please enter a value greater than 0.\n";

		} while (!success);

		// ask user to enter final energy level
		success = false;
		do {
			if (stringToInt(getInput(string("Enter fianl energy level: ")), energyLevel_2)) {
				if (energyLevel_2 > 0 && energyLevel_2 <= energyLevel_1) // energy level can't be 0 or less than the origonal enery level
					success = true;
				else
					cout << "Please enter a positive energy level less than the initial energy level.\n";
			}
			else
				cout << "Non valid input. Non valid input. Please enter a value greater than 0.\n";

		} while (!success);

		// ask to output in J or eV
		string answer_Je;
		do {						// ask the user till a valid input is given
			answer_Je = getInput(string("Output in Joules (J) or electron volts (e)? (J/e): "));
		} while (!(answer_Je == "J" || answer_Je == "e"));

		// calculate the transition energy
		float energy{ transitionEnergy(atomicNumber, energyLevel_1, energyLevel_2, answer_Je == "J") };

		// output the formated answer followed by the correct unit
		printf("Energy of the transition : %3.2e %s \n", energy, (answer_Je == "J" ? "J" : "eV"));

		// ask to repeat the calculation
		string answer_yn;
		do {						// ask the user till a valid input is given
			answer_yn = getInput(string("Would you like to repeat the calculation? (y/n): "));
		} while (!(answer_yn == "y" || answer_yn == "n"));

		// repeat is true if the answer is "y"
		repeat = (answer_yn == "y");

	} while (repeat);

	return 0;
}