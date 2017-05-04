#include "Layout.hh"
#include "G4ThreeVector.hh"
#include "G4SystemOfUnits.hh"
std::vector<Scintillator> makeLayout(){
	std::vector<Scintillator> scintilators = std::vector<Scintillator>();
	scintilators.push_back(Scintillator(G4ThreeVector(0,0,0)));
	scintilators.push_back(Scintillator(G4ThreeVector(0,0,10*cm)));
	scintilators.push_back(Scintillator(G4ThreeVector(0,0,-10*cm)));

	scintilators.push_back(Scintillator(G4ThreeVector(0,-20*cm,0)));
	scintilators.push_back(Scintillator(G4ThreeVector(0,-20*cm,10*cm)));
	scintilators.push_back(Scintillator(G4ThreeVector(0,-20*cm,-10*cm)));
	return scintilators;
}
