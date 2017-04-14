#include "B4DetectorConstruction.hh"

#include "G4Material.hh"
#include "G4Element.hh"
#include "G4NistManager.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4GlobalMagFieldMessenger.hh"
#include "G4AutoDelete.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include "G4Polyhedra.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreadLocal 
G4GlobalMagFieldMessenger* B4DetectorConstruction::fMagFieldMessenger = 0; 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B4DetectorConstruction::B4DetectorConstruction()
 : G4VUserDetectorConstruction(),
   fAbsorberPV(0),
   fGapPV(0),
   fCheckOverlaps(true)
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B4DetectorConstruction::~B4DetectorConstruction()
{ 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* B4DetectorConstruction::Construct()
{
  // Define materials 
  DefineMaterials();
  
  // Define volumes
  return DefineVolumes();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B4DetectorConstruction::DefineMaterials()
{ 
  // Lead material defined using NIST Manager
  G4NistManager* nistManager = G4NistManager::Instance();
  nistManager->FindOrBuildMaterial("G4_Pb");
  
  // Liquid argon material
  G4double a;  // mass of a mole;
  G4double z;  // z=mean number of protons;  
  G4double density; 
  new G4Material("liquidArgon", z=18., a= 39.95*g/mole, density= 1.390*g/cm3);
         // The argon by NIST Manager is a gas with a different density


  //Polystyrene stuff form this website: http://accms04.physik.rwth-aachen.de/~hgast/cplusplus/vortrag_day5.pdf
  //Elements required for polystyrene
  new G4Element("Hydrogen", "H", 1,
		  1.008 * CLHEP::g / CLHEP::mole);
  new G4Element("Carbon", "C", 6,
		  12.011 * CLHEP::g / CLHEP::mole);
  //POLYSTYRENE
  G4Material* polystyrene = new G4Material("Polystyrene", 1.0320 * CLHEP::g / CLHEP::cm3, 2, kStateSolid);
  polystyrene->AddElement(G4Element::GetElement("Hydrogen", true), 8);//8 is atoms per unit
  polystyrene->AddElement(G4Element::GetElement("Carbon", true), 8);

  // Vacuum
  new G4Material("Galactic", z=1., a=1.01*g/mole,density= universe_mean_density,
                  kStateGas, 2.73*kelvin, 3.e-18*pascal);

  // Print materials
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* B4DetectorConstruction::DefineVolumes()
{
  // Geometry parameters
  G4int nofLayers = 10;
  G4double absoThickness = 10.*mm;
  G4double gapThickness =  5.*mm;
  G4double calorSizeXY  = 10.*cm;

  G4double layerThickness = absoThickness + gapThickness;
  G4double calorThickness = nofLayers * layerThickness;
  G4double worldSizeXY = 1.2 * calorSizeXY;
  G4double worldSizeZ  = 1.2 * calorThickness; 
  
  // Get materials
  G4Material* defaultMaterial = G4Material::GetMaterial("Galactic");
  G4Material* absorberMaterial = G4Material::GetMaterial("G4_Pb");
  G4Material* gapMaterial = G4Material::GetMaterial("liquidArgon");
  G4Material* polystyrene = G4Material::GetMaterial("polystyrene");
  
  if ( ! defaultMaterial || ! absorberMaterial || ! gapMaterial ) {
    G4ExceptionDescription msg;
    msg << "Cannot retrieve materials already defined."; 
    G4Exception("B4DetectorConstruction::DefineVolumes()",
      "MyCode0001", FatalException, msg);
  }  
   
  //     
  // World
  //


  G4VSolid* worldS 
    = new G4Box("World",           // its name
                 worldSizeXY/2, worldSizeXY/2, worldSizeZ/2); // its size
                         
  G4LogicalVolume* worldLV
    = new G4LogicalVolume(
                 worldS,           // its solid
                 defaultMaterial,  // its material
                 "World");         // its name
                                   
  G4VPhysicalVolume* worldPV
    = new G4PVPlacement(
                 0,                // no rotation
                 G4ThreeVector(),  // at (0,0,0)
                 worldLV,          // its logical volume                         
                 "World",          // its name
                 0,                // its mother  volume
                 false,            // no boolean operation
                 0,                // copy number
                 fCheckOverlaps);  // checking overlaps 
  
//  //
//  // Calorimeter
//  //
//  G4VSolid* calorimeterS
//    = new G4Box("Calorimeter",     // its name
//                 calorSizeXY/2, calorSizeXY/2, calorThickness/2); // its size
//
//  G4LogicalVolume* calorLV
//    = new G4LogicalVolume(
//                 calorimeterS,     // its solid
//                 defaultMaterial,  // its material
//                 "Calorimeter");   // its name
//
//  new G4PVPlacement(
//                 0,                // no rotation
//                 G4ThreeVector(),  // at (0,0,0)
//                 calorLV,          // its logical volume
//                 "Calorimeter",    // its name
//                 worldLV,          // its mother  volume
//                 false,            // no boolean operation
//                 0,                // copy number
//                 fCheckOverlaps);  // checking overlaps
//
//  //
//  // Layer
//  //
//  G4VSolid* layerS
//    = new G4Box("Layer",           // its name
//                 calorSizeXY/2, calorSizeXY/2, layerThickness/2); // its size
//
//  G4LogicalVolume* layerLV
//    = new G4LogicalVolume(
//                 layerS,           // its solid
//                 defaultMaterial,  // its material
//                 "Layer");         // its name
//
//  new G4PVReplica(
//                 "Layer",          // its name
//                 layerLV,          // its logical volume
//                 calorLV,          // its mother
//                 kZAxis,           // axis of replication
//                 nofLayers,        // number of replica
//                 layerThickness);  // witdth of replica
//
//  //
//  // Absorber
//  //
//  G4VSolid* absorberS
//    = new G4Box("Abso",            // its name
//                 calorSizeXY/2, calorSizeXY/2, absoThickness/2); // its size
//
//  G4LogicalVolume* absorberLV
//    = new G4LogicalVolume(
//                 absorberS,        // its solid
//                 absorberMaterial, // its material
//                 "Abso");          // its name
//
//  fAbsorberPV
//    = new G4PVPlacement(
//                 0,                // no rotation
//                 G4ThreeVector(0., 0., -gapThickness/2), // its position
//                 absorberLV,       // its logical volume
//                 "Abso",           // its name
//                 layerLV,          // its mother  volume
//                 false,            // no boolean operation
//                 0,                // copy number
//                 fCheckOverlaps);  // checking overlaps
//
//  //
//  // Gap
//  //
//  G4VSolid* gapS
//    = new G4Box("Gap",             // its name
//                 calorSizeXY/2, calorSizeXY/2, gapThickness/2); // its size
//
//  G4LogicalVolume* gapLV
//    = new G4LogicalVolume(
//                 gapS,             // its solid
//                 gapMaterial,      // its material
//                 "Gap");           // its name
//
//  fGapPV
//    = new G4PVPlacement(
//                 0,                // no rotation
//                 G4ThreeVector(0., 0., absoThickness/2), // its position
//                 gapLV,            // its logical volume
//                 "Gap",            // its name
//                 layerLV,          // its mother  volume
//                 false,            // no boolean operation
//                 0,                // copy number
//                 fCheckOverlaps);  // checking overlaps

  //Time to add a triangular prism
  G4double size = 5 * cm;
  G4double height = 10 * cm;
  const G4double outerRadii[] = {0, size, size, 0};
  const G4double zCoords[] = {0, 0, height, height};
  G4VSolid* triangly
  	= new G4Polyhedra(G4String("Triangly"),//name
  			G4double(0), G4double(2 * CLHEP::pi), //draw from angles 0 to 2pi
  			G4int(3), G4int(4), //triangles have 3 sides, and triangular prisms have 4 changes in radius
			outerRadii, zCoords);//the center of the triangle is 0 from the center, while the corners are size from the center
  G4LogicalVolume* trianglyLV = new G4LogicalVolume(triangly, polystyrene, "Triangly");
  G4PVPlacement* trianglyPV = new G4PVPlacement(
	0, //no rotation
	G4ThreeVector(0, 0, 0), //slap that straight on the origin
	trianglyLV,
	"Triangly",
	worldLV, //place it in the world
	false, // no boolean operation (whatever that means)
	0, //copy number apparently
	fCheckOverlaps);//better check for those overlaps

  //tell the compile I don't care this variable wasn't used.
  //Note that really all these things should be deleted at some point.
  (void) trianglyPV;
  
  //
  // print parameters
  //
  G4cout
    << "------------------------------------------------------------" << G4endl
    << "---> The calorimeter is " << nofLayers << " layers of: [ "
    << absoThickness/mm << "mm of " << absorberMaterial->GetName() 
    << " + "
    << gapThickness/mm << "mm of " << gapMaterial->GetName() << " ] " << G4endl
    << "------------------------------------------------------------" << G4endl;
    //                                        
  // Visualization attributes
  //
  worldLV->SetVisAttributes (G4VisAttributes::Invisible);

  G4VisAttributes* simpleBoxVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  simpleBoxVisAtt->SetVisibility(true);
//  calorLV->SetVisAttributes(simpleBoxVisAtt);

  //
  // Always return the physical World
  //
  return worldPV;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B4DetectorConstruction::ConstructSDandField()
{ 
  // Create global magnetic field messenger.
  // Uniform magnetic field is then created automatically if
  // the field value is not zero.
  G4ThreeVector fieldValue = G4ThreeVector();
  fMagFieldMessenger = new G4GlobalMagFieldMessenger(fieldValue);
  fMagFieldMessenger->SetVerboseLevel(1);
  
  // Register the field messenger for deleting
  G4AutoDelete::Register(fMagFieldMessenger);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//FUN FACT: rotate viewer be like: /vis/viewer/set/viewpointThetaPhi 100 50
