
#ifndef DetectorROGeometry_h
#define DetectorROGeometry_h 1

#include "G4VReadOutGeometry.hh"

class DetectorROGeometry : public G4VReadOutGeometry
{

public:
	DetectorROGeometry();
	DetectorROGeometry(G4String);
	~DetectorROGeometry();

private:
	G4VPhysicalVolume* Build();
	
#include "DetectorParameterDef.hh"

};

#endif
