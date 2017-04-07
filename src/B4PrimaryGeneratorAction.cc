#include "B4PrimaryGeneratorAction.hh"

#include "G4RunManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

B4PrimaryGeneratorAction::B4PrimaryGeneratorAction(Config* config)
 : G4VUserPrimaryGeneratorAction(),
   mConfig(config),
   fParticleGun(0),
   lastGeneratedEnergy(-0.1)
{
  // we want to generate 1 particles per event
  G4int nofParticles = 1;
  fParticleGun = new G4ParticleGun(nofParticles);

  // default particle kinematic
  //
  // we want to make electrons
  G4ParticleDefinition* particleDefinition
  = G4ParticleTable::GetParticleTable()->FindParticle("e-");
  fParticleGun->SetParticleDefinition(particleDefinition);
  // we want the direction of the particle to be in the positive z direction
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));

  fParticleGun->SetParticleEnergy(config->energy*GeV);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B4PrimaryGeneratorAction::~B4PrimaryGeneratorAction()
{
  delete fParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B4PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  // This function is called at the begining of event

  // In order to avoid dependence of PrimaryGeneratorAction
  // on DetectorConstruction class we get world volume
  // from G4LogicalVolumeStore
  //
  G4double worldZHalfLength = 0;
  G4LogicalVolume* worlLV
    = G4LogicalVolumeStore::GetInstance()->GetVolume("World");
  G4Box* worldBox = 0;
  if ( worlLV) worldBox = dynamic_cast< G4Box*>(worlLV->GetSolid()); 
  if ( worldBox ) {
    worldZHalfLength = worldBox->GetZHalfLength();  
  }
  else  {
    G4ExceptionDescription msg;
    msg << "World volume of box not found." << G4endl;
    msg << "Perhaps you have changed geometry." << G4endl;
    msg << "The gun will be place in the center.";
    G4Exception("B4PrimaryGeneratorAction::GeneratePrimaries()",
      "MyCode0002", JustWarning, msg);
  } 
  
  // Set gun position
  // here we set the creation point of the particle
  // in the geometry routine, we define a box called the "world" and the detector and everything must be inside the box
  // if a particle goes outside the box, it is removed from the particle list and no longer tracked.
  // here we are creating our particle at x,y,z = 0,0, -z edge of the box
  fParticleGun
    ->SetParticlePosition(G4ThreeVector(0., 0., -worldZHalfLength));

  if (mConfig->randomize){
	  lastGeneratedEnergy = mConfig->energy * GeV * G4UniformRand();
  } else {
	  lastGeneratedEnergy = mConfig->energy * GeV;
  }
  fParticleGun->SetParticleEnergy(lastGeneratedEnergy);


  fParticleGun->GeneratePrimaryVertex(anEvent);
}
G4double B4PrimaryGeneratorAction::getLastGeneratedEnergy(){
	return lastGeneratedEnergy;
}


