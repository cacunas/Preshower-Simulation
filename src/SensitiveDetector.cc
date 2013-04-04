#include "Analysis.hh"
#include "SensitiveDetector.hh"
#include "Hits.hh"
#include "RunAction.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4EventManager.hh"
#include "G4OpticalPhoton.hh"
#include "G4SDManager.hh"
#include "G4StepPoint.hh"

SensitiveDetector::SensitiveDetector(const G4String& name, const G4String& hitsCName) 
	: G4VSensitiveDetector(name),
	  point(NULL),
	  hitsCollection(NULL),
	  energy(0),
	  pos(G4ThreeVector()),
	  eDep(0)
{
	  collectionName.insert(hitsCName);
}

SensitiveDetector::~SensitiveDetector()
{}

void SensitiveDetector::Initialize(G4HCofThisEvent* /*hce*/)
{

	eDep = 0;

}

G4bool SensitiveDetector::ProcessHits(G4Step* step, G4TouchableHistory*)
{ 

	point  = step->GetPreStepPoint();

	G4TouchableHandle tHandle = point->GetTouchableHandle();	
	G4int channel = tHandle->GetReplicaNumber();

	G4String rName = tHandle->GetVolume()->GetName();
	G4int axis;
	if(rName == "RO_X") axis = 1;
	else if(rName == "RO_Y") axis = 2;
	else{
		axis = 0;
		G4cout<<"************************ Readout error**************************"<<G4endl;
	}

	G4cout<<"Name: "<<rName<<"\tReplica: "<<channel<<G4endl;

	G4RunManager* runManager = G4RunManager::GetRunManager();
	G4int event = runManager->GetCurrentEvent()->GetEventID();

	G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
	analysisManager->FillNtupleIColumn(0,event);
	analysisManager->FillNtupleIColumn(1,axis);
	analysisManager->FillNtupleIColumn(2,channel);
	analysisManager->AddNtupleRow();

	analysisManager->FillH1(axis,channel);	

	step->GetTrack()->SetTrackStatus(fStopAndKill);

	return true;
}

void SensitiveDetector::EndOfEvent(G4HCofThisEvent*)
{

	G4RunManager* runManager = G4RunManager::GetRunManager();
	G4int event = runManager->GetCurrentEvent()->GetEventID();

	G4cout<<"Event: "<<event<<G4endl;

}

