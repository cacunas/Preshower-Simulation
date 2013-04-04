/**
 * Detector construction class for PreShower Matrix
 * 
 * @author ezambrano jgpavez
 */

#include "globals.hh"

#include "DetectorConstruction.hh"
#include "SensitiveDetector.hh"
#include "DetectorROGeometry.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Element.hh"
#include "G4VisAttributes.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4UnionSolid.hh"
#include "G4AssemblyVolume.hh"
#include "G4PVReplica.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4SDManager.hh"
#include "G4MaterialPropertyVector.hh"
#include "G4VSDFilter.hh"
#include "G4SDParticleFilter.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"

DetectorConstruction::DetectorConstruction()
	: pSD(0),
	  pWorldPhys(0)
{
#include "DetectorParameterDef.icc"
}

DetectorConstruction::~DetectorConstruction()
{}

G4VPhysicalVolume* DetectorConstruction::Construct()
{

    //LYSO Material____________________________________________________________

    //Get Elements From Material Data Base
    G4NistManager* manager = G4NistManager::Instance();

    G4Element* H  = manager->FindOrBuildElement("H");
    G4Element* C  = manager->FindOrBuildElement("C");
    G4Element* O  = manager->FindOrBuildElement("O");
    G4Element* Lu = manager->FindOrBuildElement("Lu");
    G4Element* Y  = manager->FindOrBuildElement("Y");

    G4Material* Polystyrene  = manager->FindOrBuildMaterial("G4_POLYSTYRENE");
    G4Material* PMMA	 = manager->FindOrBuildMaterial("G4_PLEXIGLASS");
    G4Material* Air          = manager->FindOrBuildMaterial("G4_AIR");
    G4Material* UVTAcrylic	 = manager->FindOrBuildMaterial("G4_PLEXIGLASS");

    //Fluorinated Polymer
    G4Material* FPolymer = new G4Material("Fluorinated Polymer",1.43*g/cm3 , 3);
    FPolymer->AddElement(H, 08.0538*perCent);
    FPolymer->AddElement(C, 59.9848*perCent);
    FPolymer->AddElement(O, 31.9614*perCent);

    //Lutetium Oxide Lu2O3
    G4Material* LutetiumOxide = new G4Material("LutetiumOxide", 9.41*g/cm3, 2);
    LutetiumOxide->AddElement(Lu, 2);	
    LutetiumOxide->AddElement(O, 3);

    //Silicon Dioxide SiO2
    G4Material* SiliconDioxide  = manager->FindOrBuildMaterial("G4_SILICON_DIOXIDE");

    //Yttrium Oxide Y2O3
    G4Material* YttriumOxide = new G4Material("YttriumOxide", 5.01*g/cm3, 2);
    YttriumOxide->AddElement(Y, 2);
    YttriumOxide->AddElement(O, 3);

    //Build LYSO Material
    G4Material* LYSO = new G4Material("LYSO", 7.1*g/cm3, 3);
    LYSO->AddMaterial(LutetiumOxide,  81*perCent);
    LYSO->AddMaterial(SiliconDioxide, 14*perCent);
    LYSO->AddMaterial(YttriumOxide,    5*perCent);				


    //Optical properties Attached to Materials_____________________________



    const G4int nEntries = 45;


    //LYSO Crystal
    G4double PhotonEnergy[nEntries] =
    { 1.94*eV,2.00*eV,2.06*eV,2.12*eV,2.18*eV,
      2.24*eV,2.30*eV,2.36*eV,2.42*eV,2.48*eV,
      2.54*eV,2.60*eV,2.66*eV,2.72*eV,2.78*eV,
      2.84*eV,2.90*eV,2.96*eV,3.02*eV,3.08*eV,
      3.14*eV,3.20*eV,3.26*eV,3.32*eV,3.38*eV,
      3.44*eV,3.50*eV,3.56*eV,3.62*eV,3.68*eV,
      3.74*eV,3.8*eV,3.86*eV,3.92*eV,3.98*eV,
      4.04*eV,4.1*eV,4.16*eV,4.22*eV,4.28*eV,
      4.34*eV,4.4*eV,4.46*eV,4.52*eV,4.58*eV};

    G4double fastLYSO[nEntries]  =    
            { 0.092795968802, 0.374539584218, 0.963888919018, 1.65682419462,2.43831159163 ,
              3.71632697909, 5.50077214492, 7.56698642074, 9.99928599144,12.4188742608 ,
              15.1862445009, 17.7883990961, 20.3955005348, 22.5827235294, 24.0555735326,
              24.792566649, 24.3898099075, 22.6112651148, 19.2450936031, 14.653497191,
              9.55177610256, 5.4441767564, 2.66771641639,1.0200045365 , 0.463423437655,
              0.13095859308, 0.0504877716094, 0.0511161, 0.0, 0.0,
              0.0, 0.0, 0.0, 0.0, 0.0,
              0.0, 0.0, 0.0, 0.0, 0.0,
              0.0, 0.0, 0.0, 0.0, 0.0
             };

    G4double rLYSO[nEntries] =        
             {
              1.81, 1.81, 1.81, 1.81, 1.81,
              1.81, 1.81, 1.81, 1.81, 1.81,
              1.81, 1.81, 1.81, 1.81, 1.81,
              1.81, 1.81, 1.81, 1.81, 1.81,
              1.81, 1.81, 1.81, 1.81, 1.81,
              1.81, 1.81, 1.81, 1.81, 1.81,
              1.81, 1.81, 1.81, 1.81, 1.81,
              1.81, 1.81, 1.81, 1.81, 1.81,
              1.81, 1.81, 1.81, 1.81, 1.81
            };

    G4double absLYSO[nEntries]   =     
         {50*cm, 50*cm, 50*cm, 50*cm, 50*cm,
          50*cm, 50*cm, 50*cm, 50*cm, 50*cm,
          50*cm, 50*cm, 50*cm, 50*cm, 50*cm,
          50*cm, 50*cm, 50*cm, 50*cm, 50*cm,
          50*cm, 50*cm, 50*cm, 50*cm, 50*cm,
          50*cm, 50*cm, 50*cm, 50*cm, 50*cm,
          50*cm, 50*cm, 50*cm, 50*cm, 50*cm,
          50*cm, 50*cm, 50*cm, 50*cm, 50*cm,
          50*cm, 50*cm, 50*cm, 50*cm, 50*cm
          };

    G4MaterialPropertiesTable* LYSO_MPT = new G4MaterialPropertiesTable();
     LYSO_MPT->AddProperty("FASTCOMPONENT", PhotonEnergy, fastLYSO, nEntries);
    LYSO_MPT->AddProperty("ABSLENGTH", PhotonEnergy, absLYSO, nEntries);
    LYSO_MPT->AddProperty("RINDEX", PhotonEnergy, rLYSO , nEntries);
    LYSO_MPT->AddConstProperty("SCINTILLATIONYIELD",32./keV);
    LYSO_MPT->AddConstProperty("FASTTIMECONSTANT",41*ns);
    LYSO_MPT->AddConstProperty("RESOLUTIONSCALE", 1);

    LYSO->SetMaterialPropertiesTable(LYSO_MPT);


    //WLS Fiber

    G4double RefractiveIndexWLSfiber[nEntries] =
    { 1.58666, 1.58838, 1.59017, 1.592,1.59386 ,
      1.59575, 1.59764, 1.59953, 1.60142, 1.60332,
      1.60525, 1.60722, 1.6093, 1.61153, 1.61401,
      1.61683, 1.62013,1.62408 , 1.62886, 1.63473,
      1.64195, 1.65084, 1.66177,1.67515 ,1.69143 ,
      1.71112, 1.73476, 1.76292,1.79619 , 1.8352,
      1.88057, 1.9329, 1.99281, 2.06086, 2.13759,
      2.2235, 2.31905, 2.42464, 2.54065, 2.66741,
      2.80522, 2.95435 , 3.11506, 3.28757, 3.47212
    };

    G4double AbsWLSfiber[nEntries] =
    { 0.15886875498*m,0.15886875498*m,0.15886875498*m,0.15886875498*m,0.15886875498*m,
      0.15886875498*m,0.15886875498*m,0.15886875498*m,0.15886875498*m,0.15886875498*mm,
      0.15886875498*mm,0.21854127879*mm,0.3614942435*mm,0.458550982808*mm,0.434161551652*mm,
      0.466832621324*mm,0.468697682766*mm,0.408069251303*mm,0.390288203361*mm,0.358362694603*mm,
      0.31342035112*mm,0.284871136412*mm,0.262551953159*mm,0.236219871618*mm,0.218087774682*mm,
      0.204859281866*mm,0.196130649511*mm,0.193091306219*mm,0.193091306219*m,0.193091306219*m,
      0.193091306219*m,0.193091306219*m,0.193091306219*m,0.193091306219*m,0.193091306219*m,
      0.193091306219*m,0.193091306219*m,0.193091306219*m,0.193091306219*m,0.193091306219*m,
      0.193091306219*m,0.193091306219*m,0.193091306219*m,0.193091306219*m,0.193091306219*m
    };

    G4double EmissionFib[nEntries] =
    { 0.015291943656, 0.015291943656, 0.015291943656, 0.0359502961269, 0.0786961554992,
      0.155343756728, 0.270814720038, 0.417733669604, 0.710069393144, 0.733923388889,
      0.76742491587, 1.00118998597, 0.462649791083, 0.0612096233999, 0.00385229,
      0.0, 0.0, 0.0, 0.0 ,0.0,
      0.0, 0.0, 0.0, 0.0 ,0.0,
      0.0, 0.0, 0.0, 0.0 ,0.0,
      0.0, 0.0, 0.0, 0.0 ,0.0,
      0.0, 0.0, 0.0, 0.0 ,0.0,
      0.0, 0.0, 0.0, 0.0 ,0.0
    };

    G4double RefractiveIndexClad1[nEntries] =
    { 1.48871, 1.48964 ,1.49059 , 1.49156, 1.4925,
      1.49341, 1.49428 , 1.49508, 1.49582, 1.49651,
      1.49718, 1.49786 ,1.49861 , 1.49952, 1.50071,
      1.50233, 1.50456, 1.50763, 1.51182, 1.51746,
      1.52494, 1.5347, 1.54725, 1.56313, 1.58297,
      1.60742, 1.63718, 1.67296, 1.71549, 1.76548,
      1.82363, 1.89059, 1.96697, 2.05331, 2.15012,
      2.25783, 2.37681, 2.50741, 2.64992, 2.80461,
      2.97172, 3.15149, 3.34412, 3.54986, 3.76891};

    G4double RefractiveIndexClad2[nEntries] =
    { 1.42, 1.42, 1.42, 1.42, 1.42, 1.42, 1.42, 1.42, 1.42, 1.42,
      1.42, 1.42, 1.42, 1.42, 1.42, 1.42, 1.42, 1.42, 1.42, 1.42,
      1.42, 1.42, 1.42, 1.42, 1.42, 1.42, 1.42, 1.42, 1.42, 1.42,
      1.42, 1.42, 1.42, 1.42, 1.42, 1.42, 1.42, 1.42, 1.42, 1.42,
      1.42, 1.42, 1.42, 1.42, 1.42};

    G4double AbsClad[nEntries] =
    {1.7808198996*m,1.7808198996*m,1.7808198996*m,1.7808198996*m,1.81183280053*m,
     1.92304849047*m,2.03987561458*m,2.18517177214*m,2.31249401015*m,2.33531956944*m,
     2.1534931793*m,2.10913923769*m,1.90410196961*m,1.79075976129*m,1.6224094288*m,
     1.45759897726*m,1.30577565519*m,1.15524974773*m,1.07303545321*m,0.973368771545*m,
     0.85339374284*m,0.752281299523*m,0.637198032814*m,0.529954555401*m,0.407842596777*m,
     0.314328906406*m,0.252565493761*m,0.219694957671*m,0.184955744079*m,0.152089864431*m,
     0.127853080991*m,0.106200749982*m,0.0891175898941*m,0.0587398769858*m,0.0364349404235*m,
     0.0250578999335*m,0.0157848026639*m,0.00964887284047*m,0.00438323405628*m,0.00235890486715*m,
     0.00131427368883*m,0.00100755935566*m,0.00100755935566*m,0.00100755935566*m,0.00100755935566*m};

    G4MaterialPropertiesTable* Polystyrene_MPT = new G4MaterialPropertiesTable();
    G4MaterialPropertiesTable* PMMA_MPT = new G4MaterialPropertiesTable();
    G4MaterialPropertiesTable* FPolymer_MPT = new G4MaterialPropertiesTable();

    Polystyrene_MPT->AddProperty("WLSABSLENGTH", PhotonEnergy, AbsWLSfiber, nEntries);
    Polystyrene_MPT->AddProperty("WLSCOMPONENT", PhotonEnergy, EmissionFib, nEntries);
    Polystyrene_MPT->AddProperty("RINDEX"      , PhotonEnergy, RefractiveIndexWLSfiber , nEntries);
    Polystyrene_MPT->AddConstProperty("WLSTIMECONSTANT",0.5*ns);

    PMMA_MPT->AddProperty("RINDEX"   , PhotonEnergy, RefractiveIndexClad1 , nEntries);
    PMMA_MPT->AddProperty("ABSLENGTH", PhotonEnergy, AbsClad, nEntries);

    FPolymer_MPT->AddProperty("RINDEX"   , PhotonEnergy, RefractiveIndexClad2 , nEntries);
    FPolymer_MPT->AddProperty("ABSLENGTH", PhotonEnergy, AbsClad, nEntries);

    Polystyrene->SetMaterialPropertiesTable(Polystyrene_MPT);
    PMMA->SetMaterialPropertiesTable(PMMA_MPT);
    FPolymer->SetMaterialPropertiesTable(FPolymer_MPT);

    //UVT Acrylic

    G4double AbsUVT[nEntries] =
    { 37.0*mm,37.0*mm,37.0*mm,37.0*mm,37.0*mm,37.0*mm,37.0*mm,37.0*mm,37.0*mm,37.0*mm,
      37.0*mm,37.0*mm,37.0*mm,37.0*mm,37.0*mm,37.0*mm,37.0*mm,37.0*mm,37.0*mm,37.0*mm,
      37.0*mm,37.0*mm,37.0*mm,37.0*mm,37.0*mm,37.0*mm,37.0*mm,37.0*mm,37.0*mm,37.0*mm,
      37.0*mm,37.0*mm,37.0*mm,37.0*mm,37.0*mm,37.0*mm,37.0*mm,37.0*mm,37.0*mm,37.0*mm,
      37.0*mm,37.0*mm,37.0*mm,37.0*mm,37.0*mm };

    G4MaterialPropertiesTable* UVT_MPT = new G4MaterialPropertiesTable();
    UVT_MPT->AddProperty("RINDEX"   , PhotonEnergy, RefractiveIndexClad1 , nEntries);
    UVT_MPT->AddProperty("ABSLENGTH", PhotonEnergy, AbsUVT, nEntries);

    UVTAcrylic->SetMaterialPropertiesTable(UVT_MPT);

    //Air
    G4double RefractiveIndex[nEntries] =
    { 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
      1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
      1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
      1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
      1.00, 1.00, 1.00, 1.00, 1.00 };

    G4MaterialPropertiesTable* Air_MPT = new G4MaterialPropertiesTable();

    Air_MPT->AddProperty("RINDEX", PhotonEnergy, RefractiveIndex, nEntries);
    Air->SetMaterialPropertiesTable(Air_MPT);


    //Volumes Definition________________________________________

    //World
    G4Box* pWorldSolid = new G4Box("WorldBox", Wx, Wy, Wz);
    G4LogicalVolume* pWorldLog = new G4LogicalVolume(pWorldSolid, Air, "WorldLogical");
    pWorldPhys = new G4PVPlacement(Id_rot, Id_tr, pWorldLog, "World", 0, false, 0);
    pWorldLog->SetVisAttributes(G4VisAttributes(false));

    //Detector
    G4Box* pDetSolid = new G4Box("DetBox", Dx, Dy, Dz);
    G4LogicalVolume* pDetLog = new G4LogicalVolume(pDetSolid, Air, "DetLogical");
    G4PVPlacement* pDetPhys = new G4PVPlacement(Id_rot, Id_tr, pDetLog, "Detector", pWorldLog, false, 0);
    G4VisAttributes* pDetVA = new G4VisAttributes(false);
    pDetVA->SetForceWireframe(true);
    pDetLog->SetVisAttributes(pDetVA);

    //Acrylic Plate
    G4Box* pPlateSolid = new G4Box("PlateBox", Px, Py, Pz);
    G4LogicalVolume* pPlateLog = new G4LogicalVolume(pPlateSolid, UVTAcrylic, "PlateLogical");
    G4PVPlacement* pPlatePhys = new G4PVPlacement(Id_rot, G4ThreeVector(0,0,Dz-Pz), pPlateLog, "Plate", pDetLog, false, 0);
    G4VisAttributes* plateVA = new G4VisAttributes(true, G4Colour::White());
    plateVA->SetForceWireframe(true);
    pPlateLog->SetVisAttributes(plateVA);

    //Fiber slot
    G4Box*  pSlotSolid = new G4Box("SlotBox", Sx, Sy, Sz) ;
    G4LogicalVolume* pSlotLog = new G4LogicalVolume(pSlotSolid, PMMA, "SlotLogical"); 
    pSlotLog->SetVisAttributes(G4VisAttributes(true, G4Colour::White()));

    //Fiber
    G4Tubs* pCoreSolid  = new G4Tubs("CoreTube" , 0, CoreR , Px, 0, Phi);
    G4Tubs* pClad1Solid = new G4Tubs("Clad1Tube", 0, Clad1R, Px, 0, Phi);
    G4Tubs* pClad2Solid = new G4Tubs("Clad2Tube", 0, Clad2R, Px, 0, Phi);

    G4LogicalVolume* pCoreLog  = new G4LogicalVolume(pCoreSolid,  Polystyrene, "CoreLogical" );
    G4VisAttributes* coreVA = new G4VisAttributes(false);
    coreVA->SetForceWireframe(true);
    pCoreLog->SetVisAttributes(coreVA);

    G4LogicalVolume* pClad1Log = new G4LogicalVolume(pClad1Solid, PMMA,        "Clad1Logical");
    G4VisAttributes* clad1VA = new G4VisAttributes(false);
    clad1VA->SetForceWireframe(true);
    pClad1Log->SetVisAttributes(clad1VA);

    G4LogicalVolume* pClad2Log = new G4LogicalVolume(pClad2Solid, FPolymer ,   "Clad2Logical");
    G4VisAttributes* clad2VA = new G4VisAttributes(false);
    clad2VA->SetForceWireframe(true);
    pClad2Log->SetVisAttributes(clad2VA);

    G4LogicalVolume* pFiberLog = new G4LogicalVolume(pClad2Solid, FPolymer ,   "FiberLogical");	
    pFiberLog->SetVisAttributes(G4VisAttributes(true, G4Colour::Green()));

    G4PVPlacement* pCorePhys  = new G4PVPlacement(Id_rot, Id_tr, pCoreLog , "Core"     , pClad1Log, false, 0);
    G4PVPlacement* pClad1Phys = new G4PVPlacement(Id_rot, Id_tr, pClad1Log, "Cladding1", pClad2Log, false, 0);
    G4PVPlacement* pClad2Phys = new G4PVPlacement(Id_rot, Id_tr, pClad2Log, "Cladding2", pFiberLog, false, 0);

    //Matrix
    G4Box* pMatrixSolid = new G4Box("MatrixBox", Mx, My, Mz);
    G4LogicalVolume* pMatrixLog = new G4LogicalVolume(pMatrixSolid, Air, "MatrixLogical");
    G4PVPlacement* pMatrixPhys = new G4PVPlacement(Id_rot, G4ThreeVector(0., 0., Dz-2*Pz-Mz), pMatrixLog, "Matrix", pDetLog, false, 0);
    G4VisAttributes* matrixVA = new G4VisAttributes(false);
    matrixVA->SetForceWireframe(true);
    pMatrixLog->SetVisAttributes(matrixVA);

    //Xaxis Segments
    G4Box* pXDivSolid = new G4Box("XSegmentBox", CSx, My, Mz);
    G4LogicalVolume* pXDivLog = new G4LogicalVolume(pXDivSolid, Air, "XSegmentLogical");
    G4VPhysicalVolume* pXDivPhys = new G4PVReplica("XSegment", pXDivLog, pMatrixPhys, kXAxis, nx, 2.*CSx);
    G4VisAttributes* pXDiv_VA = new G4VisAttributes(false);
    pXDiv_VA->SetForceWireframe(true);
    pXDivLog->SetVisAttributes(pXDiv_VA);

    //Crystal surface
    G4Box* pCSurfSolid = new G4Box("CrystalSurfaceBox", CSx, CSy, CSz);
    G4LogicalVolume* pCSurfLog = new G4LogicalVolume(pCSurfSolid, Air, "GapLogical");
    G4VisAttributes* pCSurf_VA = new G4VisAttributes(false);
    pCSurf_VA->SetForceWireframe(true);
    pCSurfLog->SetVisAttributes(pCSurf_VA);

    //Yaxis Division
    G4VPhysicalVolume* pCSurfPhys = new G4PVReplica("YDiv", pCSurfLog, pXDivPhys, kYAxis, ny, 2.*CSy);

    //Crystal
    G4Box* pCrySolid = new G4Box("CrystalBox", Cx, Cy, Cz);
    G4LogicalVolume* pCryLog = new G4LogicalVolume(pCrySolid, LYSO, "CrystalLogical");
    G4PVPlacement* pCrystalPhys = new G4PVPlacement(Id_rot, G4ThreeVector(0.,0.,CG), pCryLog, "Crystal", pCSurfLog, false, 0);
    pCryLog->SetVisAttributes(G4VisAttributes(true, G4Colour::White()));

    //Fiber Assembly
    G4int x;
    G4ThreeVector tr;
    rot = new G4RotationMatrix();
    rot->rotateX(90*deg);
    G4AssemblyVolume* slot_assembly = new G4AssemblyVolume(pFiberLog, Id_tr, rot);
    for(x = -12; x <= 12; x++){
    	tr = G4ThreeVector( x*2*CSx, 0., 0.);
    	slot_assembly->AddPlacedVolume(pFiberLog, tr, rot);
    }
    tr =  G4ThreeVector(0., 0., -Pz+Sz);
    slot_assembly->MakeImprint(pPlateLog, tr, Id_rot);
    rot = new G4RotationMatrix();
    rot->rotateY(180.*deg);
    rot->rotateZ(90.*deg);
    tr =  G4ThreeVector(0., 0., Pz-Sz);
    slot_assembly->MakeImprint(pPlateLog, tr, rot);

    //Readout geometry
    G4Box* pReadoutSolid_X = new G4Box("ReadoutBox_X", ROw, ROh, ROd);
    G4Box* pReadoutSolid_Y = new G4Box("ReadoutBox_Y", ROh, ROw, ROd);
    G4LogicalVolume* pReadoutLog_X = new G4LogicalVolume(pReadoutSolid_X, Air, "ReadoutLogical_X");
    G4LogicalVolume* pReadoutLog_Y = new G4LogicalVolume(pReadoutSolid_Y, Air, "ReadoutLogical_Y");

    G4VisAttributes* readout_VA = new G4VisAttributes(true, G4Colour::Grey());
    readout_VA->SetForceWireframe(true);
    pReadoutLog_X->SetVisAttributes(readout_VA);
    pReadoutLog_Y->SetVisAttributes(readout_VA);

    G4PVPlacement* ROPhys_X = new G4PVPlacement(Id_rot, G4ThreeVector(0., Px+ROh, Dz-Pz), pReadoutLog_X, "Readout_X", pDetLog, false, 0);
    G4PVPlacement* ROPhys_Y = new G4PVPlacement(Id_rot, G4ThreeVector(Py+ROh, 0., Dz-Pz), pReadoutLog_Y, "Readout_Y", pDetLog, false, 0);

    //Readout Division: 25 slices
    G4Box* pRODivSolid_X = new G4Box("RODivBox_X", RODiv, ROh, ROd);
    G4Box* pRODivSolid_Y = new G4Box("RODivBox_Y", ROh, RODiv, ROd);
    G4LogicalVolume* pRODivLog_X = new G4LogicalVolume(pRODivSolid_X, Air, "RODivLogical_X");
    G4LogicalVolume* pRODivLog_Y = new G4LogicalVolume(pRODivSolid_Y, Air, "RODivLogical_Y");
    pRODivLog_X->SetVisAttributes(G4VisAttributes(true, G4Colour::Grey()));
    pRODivLog_Y->SetVisAttributes(G4VisAttributes(true, G4Colour::Grey()));
    G4VPhysicalVolume* pRODivPhys_X = new G4PVReplica("RO_X", pRODivLog_X, ROPhys_X, kXAxis, nx, 2.*RODiv);
    G4VPhysicalVolume* pRODivPhys_Y = new G4PVReplica("RO_Y", pRODivLog_Y, ROPhys_Y, kYAxis, ny, 2.*RODiv);	

    //Sensitive Detector_______________________________________________________

    pSD = new SensitiveDetector("LYSO/SensitiveDetector","LYSOHitsCollection");
    G4SDParticleFilter* particleFilter = new G4SDParticleFilter("PhotonFilter","opticalphoton");
    pSD->SetFilter(particleFilter);

    G4SDManager* sdm = G4SDManager::GetSDMpointer();
    sdm->AddNewDetector(pSD);

    pRODivLog_X->SetSensitiveDetector(pSD);
    pRODivLog_Y->SetSensitiveDetector(pSD);

    //Material Properties Tables Attached to Optical Surfaces___________________

    const G4int n = 2;

    G4double pp[n] = {0.1*eV, 10.*eV};
    G4double crystalReflectivity[n] = {0.97, 0.97};
    G4double   plateReflectivity[n] = {0.9, 0.9};
    G4double   fiberReflectivity[n] = {0.9, 0.9};

    G4MaterialPropertiesTable* CMPT = new G4MaterialPropertiesTable();
    G4MaterialPropertiesTable* PMPT = new G4MaterialPropertiesTable();
    G4MaterialPropertiesTable* FMPT = new G4MaterialPropertiesTable();

    CMPT->AddProperty("REFLECTIVITY",pp,crystalReflectivity,n);
    PMPT->AddProperty("REFLECTIVITY",pp,plateReflectivity,n);
    FMPT->AddProperty("REFLECTIVITY",pp,fiberReflectivity,n);

    //Optical Surfaces___________________________________________________________

    //Crystal
    G4OpticalSurface* crystalOpSurface = new G4OpticalSurface("CrystalOpticalSurface");
    crystalOpSurface->SetModel(unified);
    crystalOpSurface->SetType(dielectric_dielectric);
    crystalOpSurface->SetFinish(polishedfrontpainted);
    crystalOpSurface->SetMaterialPropertiesTable(CMPT);
    new G4LogicalBorderSurface("CrystalSurface",pCrystalPhys,pCSurfPhys,crystalOpSurface);

    //Plate
    G4OpticalSurface* plateOpSurface = new G4OpticalSurface("PlateOpticalSurface");
    plateOpSurface->SetModel(unified);
    plateOpSurface->SetType(dielectric_dielectric);
    plateOpSurface->SetFinish(groundfrontpainted);
    plateOpSurface->SetMaterialPropertiesTable(PMPT);
    new G4LogicalBorderSurface("PlateSurface",pPlatePhys,pDetPhys,plateOpSurface);
    new G4LogicalBorderSurface("PlateSurface",pPlatePhys,pWorldPhys,plateOpSurface);
    new G4LogicalBorderSurface("PlateSurfaceX",pPlatePhys,pRODivPhys_X,plateOpSurface);
    new G4LogicalBorderSurface("PlateSurfaceY",pPlatePhys,pRODivPhys_Y,plateOpSurface);

    //Fiber end
    G4OpticalSurface* fiberOpSurface = new G4OpticalSurface("FiberOpticalSurface");
    fiberOpSurface->SetModel(unified);
    fiberOpSurface->SetType(dielectric_dielectric);
    fiberOpSurface->SetFinish(polishedfrontpainted);
    fiberOpSurface->SetMaterialPropertiesTable(FMPT);
    new G4LogicalBorderSurface("PaintedCore", pCorePhys, pDetPhys,fiberOpSurface);
    new G4LogicalBorderSurface("PaintedClad1",pClad1Phys,pDetPhys,fiberOpSurface);
    new G4LogicalBorderSurface("PaintedClad2",pClad2Phys,pDetPhys,fiberOpSurface);

    return pWorldPhys;
}
