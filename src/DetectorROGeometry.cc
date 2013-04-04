#include "DetectorROGeometry.hh"
#include "SensitiveDetector.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4Box.hh"
#include "G4ThreeVector.hh"
#include "G4Material.hh"

DetectorROGeometry::DetectorROGeometry()
  : G4VReadOutGeometry()
{
#include "DetectorParameterDef.icc"
}


DetectorROGeometry::DetectorROGeometry(G4String aString)
  : G4VReadOutGeometry(aString)
{
#include "DetectorParameterDef.icc"
}

DetectorROGeometry::~DetectorROGeometry()
{}

G4VPhysicalVolume* DetectorROGeometry::Build()
{

	//Dummy material
	G4Material*  dummyMat  = new G4Material(name="dummyMat", 1., 1.*g/mole, 1.*g/cm3);

	//Builds the readout world:
	G4Box* pROWorldSolid = new G4Box("ROWorldSolid", Wx, Wy, Wz);
	G4LogicalVolume* pROWorldLog = new G4LogicalVolume(pROWorldSolid, dummyMat, "ROWorldLogical");
	G4PVPlacement* pROWorldPhys = new G4PVPlacement(Id_rot,Id_tr,"ROWorldPhysical",pROWorldLog,0,false,0);

	//Readout geometry
	G4Box* pROSolid_X = new G4Box("ROBox_X", ROw, ROh, ROd);
	G4Box* pROSolid_Y = new G4Box("ROBox_Y", ROh, ROw, ROd);
	G4LogicalVolume* pROLog_X = new G4LogicalVolume(pROSolid_X, dummyMat, "ROLogical_X");
	G4LogicalVolume* pROLog_Y = new G4LogicalVolume(pROSolid_Y, dummyMat, "ROLogical_Y");
	G4VPhysicalVolume* ROPhys_X = new G4PVPlacement(Id_rot, Id_tr, pROLog_X, "RO_X", pROWorldLog, false, 0);
	G4VPhysicalVolume* ROPhys_Y = new G4PVPlacement(Id_rot, Id_tr, pROLog_Y, "RO_Y", pROWorldLog, false, 0);
	
	//Readout Division: 25 slices
	G4Box* pRODivSolid_X = new G4Box("RODivBox_X", RODiv, ROh, ROd);
	G4Box* pRODivSolid_Y = new G4Box("RODivBox_Y", ROh, RODiv, ROd);
	G4LogicalVolume* pRODivLog_X = new G4LogicalVolume(pRODivSolid_X, dummyMat, "RODivLogical_X");
	G4LogicalVolume* pRODivLog_Y = new G4LogicalVolume(pRODivSolid_Y, dummyMat, "RODivLogical_Y");
        new G4PVReplica("ROPhysical_X", pRODivLog_X, ROPhys_X, kXAxis, nx, 2.*RODiv);
        new G4PVReplica("ROPhysical_Y", pRODivLog_Y, ROPhys_Y, kYAxis, ny, 2.*RODiv);	

	SensitiveDetector* dummySD = new SensitiveDetector("LYSO/DummySD","LYSODummyHitsCollection");
	pRODivLog_X->SetSensitiveDetector(dummySD);
	pRODivLog_Y->SetSensitiveDetector(dummySD);

	return pROWorldPhys;
}
