#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "SensitiveDetector.hh"
#include "G4VPhysicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"

class DetectorConstruction : public G4VUserDetectorConstruction
{

public:
	DetectorConstruction();	
	~DetectorConstruction();
	G4VPhysicalVolume* Construct();
	
private:
	SensitiveDetector* pSD;
	G4VPhysicalVolume* pWorldPhys;

#include "DetectorParameterDef.hh"
};

#endif
