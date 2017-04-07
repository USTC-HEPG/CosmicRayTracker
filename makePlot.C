#include <stdio.h>
#include <vector>

TVectorD toRoot(vector<double> dubbs){
	return TVectorD(dubbs.size(), dubbs.data());
}

void makePlot(){

	const char* dirname = "./plots/";

	TSystemDirectory dir(dirname, dirname);
	TList *files = dir.GetListOfFiles();

	if (files) {
		TSystemFile *file;
		TString fname;
		TFile *rootFile;
		TIter next(files);
		TString hname = "edepabs";

		std::vector<double> rmss;
		std::vector<double> measEnergies;
		std::vector<double> rmsErrors;
		std::vector<double> measErrors;

		//TH2F *hist = new TH2F("hist","measured vs true energy",1000,0.,1000.,1200,0.,1200);

		while ((file = (TSystemFile*) next())) {
			if (!file->IsDirectory()) {
				char b4[10], ext[100];
				double trueEnergy = -1;
				fname = file->GetName();
				sscanf(fname.Data(), "%[^-]-%lf-%[^-]", b4, &trueEnergy, ext);
				cout << "Found file with energy " << trueEnergy << " GeV" << endl;

				//get TFile from file
				rootFile = new TFile("./plots/" + fname);

				//get 4 TH1 from TFile
				TKey *key = rootFile->FindKey(hname);
				if (key == 0) {
					cout << "Histogram does not exist" << endl;
					throw 1;
				}

				TH1F *edepabs = (TH1F*) rootFile->Get(hname);
				cout << "Histogram " + hname + " found" << endl;

				//get RMS from each first TH1
				double rms = edepabs->GetRMS();
				double measEnergy = edepabs->GetMean();

				double numEntries = edepabs->GetEntries();
				double measError = rms/sqrt(numEntries);
				double rmsError = measError*sqrt(3);

				rmss.push_back(rms);
				measEnergies.push_back(measEnergy);
				measErrors.push_back(measError);
				rmsErrors.push_back(rmsError);

			}

		}
		//make it all into a plot

		TCanvas *c1 = new TCanvas("c1", "Energy vs RMS Plot", 200, 10, 700, 500);
		TGraph* gr = new TGraphErrors(toRoot(measEnergies), toRoot(rmss), toRoot(measErrors), toRoot(rmsErrors));
		gr->Draw("A*");
		gr->SetTitle("Energy vs. Resolution");
		gr->GetXaxis()->SetTitle("Energy (MeV)");
		gr->GetYaxis()->SetTitle("Resolution (MeV)");

		gr->SaveAs("EnerVsRes.gif");

	}

}

