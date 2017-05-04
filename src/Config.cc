/*
 * Config.cc
 *
 *  Created on: Mar 9, 2017
 *      Author: hon-stauffer
 */

#include <Config.hh>

Config::Config(G4double pEnergy, bool pRandomize, bool pMakePlot, bool pMakeHist, bool pDistrib):
	energy(pEnergy),
	randomize(pRandomize),
	makePlot(pMakePlot),
	makeHist(pMakeHist),
	distrib(pDistrib){
}

Config::~Config() {
	// TODO Auto-generated destructor stub
}

