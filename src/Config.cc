/*
 * Config.cc
 *
 *  Created on: Mar 9, 2017
 *      Author: hon-stauffer
 */

#include <Config.hh>

Config::Config(G4double pEnergy, bool pRandomize, bool pMakePlot, bool pMakeHist):
	energy(pEnergy),
	randomize(pRandomize),
	makePlot(pMakePlot),
	makeHist(pMakeHist){
}

Config::~Config() {
	// TODO Auto-generated destructor stub
}

