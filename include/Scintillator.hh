/*
 * Config.hh
 *
 *  Created on: Mar 9, 2017
 *      Author: hon-stauffer
 */

#ifndef SOURCE_DIRECTORY__INCLUDE_SCINTILLATOR_HH_
#define SOURCE_DIRECTORY__INCLUDE_SCINTILLATOR_HH_

#include "G4Types.hh"
#include "G4ThreeVector.hh"

class Scintillator {
public:
	Scintillator(G4ThreeVector pos);
	virtual ~Scintillator();
	G4ThreeVector getPosition();
private:
	G4ThreeVector mPos;
};

#endif /* SOURCE_DIRECTORY__INCLUDE_SCINTILLATOR_HH_ */
