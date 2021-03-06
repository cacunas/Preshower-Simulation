#ifndef SensitiveDetector_h
#define SensitiveDetector_h 1

#include "G4VSensitiveDetector.hh"
#include "Hits.hh"
#include "RunAction.hh"
#include "G4StepPoint.hh"
#include "G4HCofThisEvent.hh"
#include "G4ThreeVector.hh"

class SensitiveDetector : public G4VSensitiveDetector
{

public:
	SensitiveDetector(const G4String&, const G4String&);
	~SensitiveDetector();
  
	void	Initialize(G4HCofThisEvent*);
	G4bool	ProcessHits(G4Step*, G4TouchableHistory*);
	void	EndOfEvent(G4HCofThisEvent*);

private:
	G4StepPoint* point;
	HitsCollection* hitsCollection;	
	G4double energy;
	G4ThreeVector pos;
	G4double eDep;
};

#endif
