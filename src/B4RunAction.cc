#include "B4RunAction.hh"
#include "B4Analysis.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include <sstream>


B4RunAction::B4RunAction(Config * pConfig)
: G4UserRunAction(),
  config(pConfig)
{ 
	// set printing event number per each event
	G4RunManager::GetRunManager()->SetPrintProgress(1);

	// Create analysis manager
	// The choice of analysis technology is done via selection of a namespace
	// in B4Analysis.hh
	G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
	G4cout << "Using " << analysisManager->GetType() << G4endl;

	if (config->makePlot){
		// Create directories
		//analysisManager->SetHistoDirectoryName("histograms");
		//analysisManager->SetNtupleDirectoryName("ntuple");
		analysisManager->SetVerboseLevel(1);
		analysisManager->SetFirstHistoId(1);

		// Book histograms, ntuple
		//

		// Creating histograms
		analysisManager->CreateH1("edepabs","Edep in absorber", 100, 0., 10*GeV);
		analysisManager->CreateH1("edepgap","Edep in gap", 100, 0., 200*MeV);
		analysisManager->CreateH1("trackabs","trackL in absorber", 100, 0., 2*m);
		analysisManager->CreateH1("trackgap","trackL in gap", 100, 0., 100*cm);

		// Creating ntuple
		//
		analysisManager->CreateNtuple("B4", "Edep and TrackL");
		analysisManager->CreateNtupleDColumn("Eabs");
		analysisManager->CreateNtupleDColumn("Egap");
		analysisManager->CreateNtupleDColumn("Labs");
		analysisManager->CreateNtupleDColumn("Lgap");
		analysisManager->FinishNtuple();
	}
	if (config->makeHist){
		G4cout<< "Making 1D histogram" << G4endl;
		analysisManager->SetFirstHistoId(0);
		analysisManager->CreateH2("energy", "Measured vs true energy", 100, 0, config->energy*GeV, 100, 0, config->energy*GeV,
								"none", "none", "none", "none", "linear", "linear");
	}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B4RunAction::~B4RunAction()
{
	delete G4AnalysisManager::Instance();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B4RunAction::BeginOfRunAction(const G4Run* /*run*/)
{ 
	//inform the runManager to save random number seed
	//G4RunManager::GetRunManager()->SetRandomNumberStore(true);

	// Get analysis manager
	G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

	// Open an output file
	//
	if (config->makeHist){
		analysisManager->OpenFile("energyPlot.root");
	}
	if (config->makePlot){
		std::stringstream ss;
		ss << config->energy;
		std::string asString;
		ss >> asString;
		G4String fileName = "plots/B4-" + asString + "-GeV.root";
		G4cout << "Plot of energy " + asString + "GeV";
		analysisManager->OpenFile(fileName);

	}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B4RunAction::EndOfRunAction(const G4Run* /*run*/)
{
	// print histogram statistics
	//
	if (config->makePlot){
		G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
		if ( analysisManager->GetH1(1) ) {
			G4cout << G4endl << " ----> print histograms statistic ";

			if(isMaster) {
				G4cout << "for the entire run " << G4endl << G4endl;
			}
			else {
				G4cout << "for the local thread " << G4endl << G4endl;
			}
			/*
    		G4cout << " EAbs from tree : mean = "
	   	   		   << G4BestUnit(analysisManager->GetNtuple(0), "Energy")
	   	   	   	   << " rms = "
	   	   	   	   << G4BestUnit(analysisManager->GetNtuple(0),  "Energy") << G4endl;
			 */
			G4cout << " Measured EAbs : mean = "
					<< G4BestUnit(analysisManager->GetH1(1)->mean(), "Energy")
					<< " rms = "
					<< G4BestUnit(analysisManager->GetH1(1)->rms(),  "Energy") << G4endl;

			/*
			G4cout << " EGap : mean = "
					<< G4BestUnit(analysisManager->GetH1(2)->mean(), "Energy")
					<< " rms = "
					<< G4BestUnit(analysisManager->GetH1(2)->rms(),  "Energy") << G4endl;

			G4cout << " LAbs : mean = "
					<< G4BestUnit(analysisManager->GetH1(3)->mean(), "Length")
					<< " rms = "
					<< G4BestUnit(analysisManager->GetH1(3)->rms(),  "Length") << G4endl;

			G4cout << " LGap : mean = "
					<< G4BestUnit(analysisManager->GetH1(4)->mean(), "Length")
					<< " rms = "
					<< G4BestUnit(analysisManager->GetH1(4)->rms(),  "Length") << G4endl;
			*/

		}
	}
	if (config->makeHist){
		//whee
	}
	if (config->makePlot || config->makeHist){
		G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
		analysisManager->Write();
		analysisManager->CloseFile();
	}

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
