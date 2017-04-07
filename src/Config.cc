/*
 * Config.cc
 *
 *  Created on: Mar 9, 2017
 *      Author: hon-stauffer
 */

#include <Config.hh>

Config::Config(G4double pEnergy, bool pRandomize, bool pHistNotPlot):
	energy(pEnergy),
	randomize(pRandomize),
	histNotPlot(pHistNotPlot){
}

Config::~Config() {
	// TODO Auto-generated destructor stub
}

