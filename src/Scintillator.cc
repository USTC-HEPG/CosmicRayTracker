/*
 * Config.cc
 *
 *  Created on: Mar 9, 2017
 *      Author: hon-stauffer
 */

#include "Scintillator.hh"
#include <Config.hh>

Scintillator::Scintillator(G4ThreeVector pos):
	mPos(pos){
}

G4ThreeVector Scintillator::getPosition(){
	return mPos;
}

Scintillator::~Scintillator() {
	// TODO Auto-generated destructor stub
}

