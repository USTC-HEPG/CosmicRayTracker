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
	Config(G4double pEnergy, bool pRandomize, bool pMakePlot, bool pMakeHist, bool pDistrib);
	virtual ~Config();
	const G4double energy;
	const bool randomize;
	const bool makePlot;
	const bool makeHist;
	const bool distrib;
};

#endif /* SOURCE_DIRECTORY__INCLUDE_CONFIG_HH_ */
