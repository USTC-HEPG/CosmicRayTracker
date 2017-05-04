#include "B4PrimaryGeneratorAction.hh"
#include "B4DetectorConstruction.hh"

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
#include "G4PhysicalConstants.hh"

B4PrimaryGeneratorAction::B4PrimaryGeneratorAction(Config* config)
 : G4VUserPrimaryGeneratorAction(),
   mConfig(config),
   fParticleGun(0),
   lastGeneratedEnergy(-0.1)
{
  // we want to generate 1 particles per event
  G4int nofParticles = 1;
  fParticleGun = new G4ParticleGun(nofParticles);

  G4ParticleDefinition* particleDefinition
  = G4ParticleTable::GetParticleTable()->FindParticle("e-");
  fParticleGun->SetParticleDefinition(particleDefinition);


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
  G4double worldYHalfLength = 0;
  G4LogicalVolume* worlLV
    = G4LogicalVolumeStore::GetInstance()->GetVolume("World");
  G4Box* worldBox = 0;
  if ( worlLV) worldBox = dynamic_cast< G4Box*>(worlLV->GetSolid());
  if ( worldBox ) {
    worldYHalfLength = worldBox->GetYHalfLength();
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
  G4double width = 25.*cm;
  G4double height = 25.*cm;

  G4double xpos = 0.;
  G4double zpos = 0.;

  if(mConfig->distrib){
	  xpos = (width*G4UniformRand())-(width/2);
	  zpos = (height*G4UniformRand())-(height/2);
  }

  fParticleGun->SetParticlePosition(G4ThreeVector(xpos, worldYHalfLength-10, zpos));

  //default- fires particle straight down
   G4double xMom = 0.;
   G4double yMom = -1.;
   G4double zMom = 0.;

   //distrib- fires particles according to cos^2 distribution
   if(mConfig->distrib){
   G4double theta = pi*G4UniformRand();
   G4double psi = -pi*G4UniformRand();

   xMom = sin(theta)*cos(psi);
   yMom = sin(theta)*sin(psi);
   zMom = cos(theta);
   }

   G4ThreeVector momentum = G4ThreeVector(xMom,yMom,zMom);

   fParticleGun->SetParticleMomentumDirection(momentum);

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


