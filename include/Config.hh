/*
 * Config.hh
 *
 *  Created on: Mar 9, 2017
 *      Author: hon-stauffer
 */

#ifndef SOURCE_DIRECTORY__INCLUDE_CONFIG_HH_
#define SOURCE_DIRECTORY__INCLUDE_CONFIG_HH_

#include "G4Types.hh"

class Config {
public:
	Config(G4double pEnergy);
	virtual ~Config();
	const G4double energy;
};

#endif /* SOURCE_DIRECTORY__INCLUDE_CONFIG_HH_ */
