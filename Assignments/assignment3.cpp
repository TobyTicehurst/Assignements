// Assignment 3 - Read in and sort course titles
// uses a vector of strings and std::sort to order course codes alphabetically and numerically

#include <iostream>
#include <vector>
#include <sstream>		// for stringstream
#include <algorithm>	// for std::sort

using namespace std;

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
string getInput(const string& request) {
	string input;
	while (true) {										// ask the user till a valid input is given
		cout << request;
		getline(cin, input);
		if (cinFailed() || input.empty())				// test for cin.fail() and empty string
			cout << "Please enter a valid input.\n";
		else
			return input;
	}
}

// check if the string is a number
bool isNumber(const string& str) {	// find_if will return the iterator number.end if each character is a digit 
	return !str.empty() && find_if(str.begin(), str.end(), [](char c) { return !isdigit(c); }) == str.end();
}

// gets a string using getInput(request) and checks the form of the string
string getCourseCode(const string& request) {
	while (true) {
		string courseCode{ getInput(request) };		// get input
		// does courseCode start with a 5 digit number and then a space? Or course code is x, to end loop
		if (courseCode == "x" || courseCode.size() > 5 && isNumber(courseCode.substr(0, 5)) && courseCode.at(5) == ' ')				
			return courseCode;
		else
			cout << "Invalid course code! Please enter a numer of the form: 12345 Example text.\n";
	}
}

// used by std::sort to sort course codes numerically from lowest to highest
bool numericalSort(string& str1, string& str2) {
	return stoi(str1.substr(5, 5)) < stoi(str2.substr(5, 5));
}

// used by std::sort to sort course titles from a to z ignoring capitalisation
bool alphabeticalSort(string& str1, string& str2) {
	unsigned int i = 11;	// 11 so that PHYS 12345 is ignored
	while (i < str1.length() && i < str2.length()) {
		if (tolower(str1[i]) != tolower(str2[i]))
			return tolower(str1[i]) < tolower(str2[i]);		// get here on the first different character
		i++;
	}
	return str1.length() < str2.length();		// get here if one string is a substring of the other eg. phys and physics
}

int main() {

	/// PHYS 12345 123example text
	// degreeCode + courseCode + courseTitle = courseTitle

	const string degreeCode("PHYS");

	// declare variables
	std::vector<string> courseTitles;
	bool finished{ false };

	// get user to input course data until x is entered
	do{
		string courseCode{ getCourseCode("Enter a course code and title (or x to finish): ") };
		if (courseCode != "x") {
			stringstream stream;
			stream << degreeCode << string(" ") << courseCode;	// build a courseTitle 
			string courseTitle;
			getline(stream, courseTitle);			// pass the stream into a string
			courseTitles.push_back(courseTitle);	// push the string into a vector
		}
		else
			finished = true;

	} while (!finished);

	// choose the sort type
	string answer;
	bool success{ false };
	do {
		answer = getInput("Sort by: \n\t a: Corse Code \n\t b: Course Title \n (a/b): ");
		if (answer == "A" || answer == "a" || answer == "B" || answer == "b")
			success = true;
		else
			cout << "Invalid input! Please enter either a or b.\n";
	} while (!success);

	// sort the course titles
	sort(courseTitles.begin(), courseTitles.end(), (tolower(answer.at(0)) == 'a') ? numericalSort : alphabeticalSort);

	// print a full list of courses
	for (auto it = courseTitles.begin(); it != courseTitles.end(); ++it)
			cout << *it << "\n";

	// choose the year to view
	string year;
	success = false;
	do {
		year = getInput("Enter a year: ");
		if (isdigit(year.at(0)))
			success = true;
		else
			cout << "Invalid input! Please enter a digit.\n";
	} while (!success);

	// print out the courses
	for (auto it = courseTitles.begin(); it != courseTitles.end(); ++it) {
		if (it->substr(5, 1) == year)
			cout << *it << "\n";
	}

	cout << "Enter a character to end the program: ";
	char x;
	cin >> x;
	cout << x;

	return 0;
}