// Assignment 4 - A class for galaxies
// A class with a default and parameterised contructor, a default destructor, 4 variables and a vector of itself as children
// Getters and setters for each variable

#include <iostream>
#include <vector>
#include <algorithm> // for std::find

using namespace std;

vector<string> hubbleTypes{ "E0-7", "S0", "Sa", "Sb", "Sc", "SBa", "SBb", "SBc", "Irr" };	// vector for testing valid Hubble types

class Galaxy {
public:
	// Default Constructor
	Galaxy() :
		m_hubbleType( "E0-7" ),
		m_redShift{ 0.0f },
		m_totalMass{ 1e+10 },
		m_stellarMassFraction{ 0.025 }
	{
		checkValues();
	}

	// Parameterised Constructor
	Galaxy(string hubbleType, double redShift, double totalMass, double stellarMassFraction) :
		m_hubbleType( hubbleType ),
		m_redShift{ redShift },
		m_totalMass{ totalMass },
		m_stellarMassFraction{ stellarMassFraction }
	{
		checkValues();
	}

	// Default Destructor
	~Galaxy(){ cout << "Destroying Galaxy\n"; }

	// Getters and Setters
	const string getHubbleType() { return m_hubbleType; }
	void setHubbleType(string hubbleType) { m_hubbleType = hubbleType; checkValues(); }
	const double getRedShift() { return m_redShift; }
	void setRedShift(double redShift) { m_redShift = redShift; checkValues(); }
	const double getTotalMass() { return m_totalMass; }
	void setTotalMass(double totalMass) { m_totalMass = totalMass; checkValues(); }
	const double getStellarMassFraction() { return m_stellarMassFraction; }
	void setStellarMassFraction(double fraction) { m_stellarMassFraction = fraction; checkValues(); }

	// Functions
	void printData();
	void addSatellite(Galaxy& galaxy){ m_satellites.push_back(galaxy); }	// copy construct a galaxy
	double getStellarMass() { return m_totalMass * m_stellarMassFraction; }

private:
	// init used by constructors so that variables can be checked
	void checkValues() {
		if (find(hubbleTypes.begin(), hubbleTypes.end(), m_hubbleType) == hubbleTypes.end()) {	// if m_hubbleType isn't a valid hubble type as defined by hubbleTypes
			cout << m_hubbleType.data() << " isn't a valid Hubble type. E0-7 will be used instead.\n";
			m_hubbleType = "E0-7";
		}
		if (m_redShift < 0.0f || m_redShift > 10.0f) {
			cout << "The red shift: " << m_redShift << " isn't between 0 and 10. 0 will be used instead.\n";
			m_redShift = 0.0f;
		}
		if (m_totalMass < 1e+7 || m_totalMass > 1e+12) {
			cout << "The total mass: " << m_totalMass << " isn't between 1e+7 and 1e+12. 1e+10 will be used instead.\n";
			m_totalMass = 1e+10;
		}
		if (m_stellarMassFraction < 0 || m_stellarMassFraction > 0.05) {
			cout << "The stellar mass fraction: " << m_stellarMassFraction << " isn't between 0 and 0.05. 0.025 will be used instead.\n";
			m_stellarMassFraction = 1e+10;
		}
	}

	// Variables
	vector<Galaxy> m_satellites;
	string m_hubbleType;
	double m_redShift, m_totalMass, m_stellarMassFraction;	// total mass in solar units
};

// function to print the data of a galaxy class. If a galaxy has satellites it will print their data too.
void Galaxy::printData() {		// Member function declared outside class
	cout << "Data:\n";
	cout << "Hubble Type: " << m_hubbleType.data() << "\nRedshift: " << m_redShift << "\nTotal mass in solar units: " << m_totalMass << " kg\nSolar mass fraction: " << m_stellarMassFraction << "\n";
	if (!m_satellites.empty()) {
		cout << "\nSatellite data:\n";
		cout << "Number of satellites: " << m_satellites.size() << "\n";
		for (auto it = m_satellites.begin(); it != m_satellites.end(); ++it)
			it->printData();
	}
	cout << "\n";
}

int main() {

	vector<Galaxy> galaxies;					// Declar a vector of Galaxies
	galaxies.reserve(3);						// resize the vector
	galaxies.emplace_back();					// Default Constructor 
	galaxies.emplace_back("E0-7", 1.0, 3e+8, 0.015);	// Parameterised Constructor
	galaxies.emplace_back("SBb", 3.0, 4.0, 0.04);		// total mass of 4 is rubbish and will be replaced

	galaxies.at(0).addSatellite(Galaxy());		// Adding a satellite (will also call destuctor while copying)
	galaxies.at(0).setHubbleType("Irr");		// changing Hubble type
	double stellarMass{ galaxies.at(0).getStellarMass() };	// get the stellar mass
	cout << "Stellar mass: " << stellarMass << "\n";

	Galaxy* ptr = new Galaxy();		
	delete ptr;						// explicitly calling destructor

	cout << "\n";	// for neatness
	for (auto it = galaxies.begin(); it != galaxies.end(); ++it)	// Printing out data
		it->printData();

	cout << "Enter any character to exit program... ";	// Pause at the end of program
	char c;
	cin >> c;
	cout << c;

	return 0;
}