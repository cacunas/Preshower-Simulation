/**
 * RunAction for Preshower Matrix Simulation
 *
 * Creates two histograms, one for each axis (X and Y).
 *
 */


#include "Analysis.hh"
#include "RunAction.hh"
#include "G4Run.hh"

RunAction::RunAction() 
	: G4UserRunAction()
{}

RunAction::~RunAction()
{}

void RunAction::BeginOfRunAction(const G4Run* run)
{
	G4cout<<"Run "<<run->GetRunID()<<" start."<<G4endl;

	G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();


	analysisManager->OpenFile("matrix");

	analysisManager->CreateNtuple("nTuple","event-axis-channel");
	analysisManager->CreateNtupleIColumn("event");
	analysisManager->CreateNtupleIColumn("axis");
	analysisManager->CreateNtupleIColumn("channel");
	analysisManager->FinishNtuple();

	analysisManager->SetFirstHistoId(1);
	analysisManager->CreateH1("Histogram_X","Fibers Readout X", 25, 0.5, 25.5);
	analysisManager->CreateH1("Histogram_Y","Fibers Readout Y", 25, 0.5, 25.5);
}

void RunAction::EndOfRunAction(const G4Run* run)
{
	G4cout<<"Run "<<run->GetRunID()<<" done."<<G4endl;

	G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
	analysisManager->Write();
	analysisManager->CloseFile();
	delete G4AnalysisManager::Instance();
}
