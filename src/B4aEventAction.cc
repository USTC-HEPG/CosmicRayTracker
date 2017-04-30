//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id: B4aEventAction.cc 75604 2013-11-04 13:17:26Z gcosmo $
// 
/// \file B4aEventAction.cc
/// \brief Implementation of the B4aEventAction class

#include "B4aEventAction.hh"
#include "B4RunAction.hh"
#include "B4Analysis.hh"

#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4UnitsTable.hh"

#include "Randomize.hh"
#include "Config.hh"
#include <map>

B4aEventAction::B4aEventAction(Config *pConfig,
		B4PrimaryGeneratorAction *pPrimGens)
: G4UserEventAction(),
  config(pConfig),
  primGens(pPrimGens),
  triggered()
{}

B4aEventAction::~B4aEventAction()
{}

void B4aEventAction::BeginOfEventAction(const G4Event* /*event*/)
{  
}

void B4aEventAction::EndOfEventAction(const G4Event* event)
{
	for (std::map<G4String, bool>::iterator it = triggered.begin();
			it != triggered.end(); ++it){
		if (it->second){
			G4cout << "Triangle " << it->first << " triggered" << G4endl;
		}
	}
}  

void B4aEventAction::trigger(G4String name){
	triggered[name] = true;
}
