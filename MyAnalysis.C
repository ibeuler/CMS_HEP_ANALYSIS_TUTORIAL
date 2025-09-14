#define MyAnalysis_cxx
// The class definition in MyAnalysis.h has been generated automatically
// by the ROOT utility TTree::MakeSelector(). This class is derived
// from the ROOT class TSelector. For more information on the TSelector
// framework see $ROOTSYS/README/README.SELECTOR or the ROOT User Manual.

// The following methods are defined in this file:
//    Begin():        called every time a loop on the tree starts,
//                    a convenient place to create your histograms.
//    SlaveBegin():   called after Begin(), when on PROOF called only on the
//                    slave servers.
//    Process():      called for each event, in this function you decide what
//                    to read and fill your histograms.
//    SlaveTerminate: called at the end of the loop on the tree, when on PROOF
//                    called only on the slave servers.
//    Terminate():    called at the end of the loop on the tree,
//                    a convenient place to draw/fit your histograms.
//
// To use this file, try the following session on your Tree T:
//
// Root > T->Process("MyAnalysis.C")
// Root > T->Process("MyAnalysis.C","some options")
// Root > T->Process("MyAnalysis.C+")
//

#include "MyAnalysis.h"
#include <iostream>
#include <TH1F.h>
#include <TLatex.h>
#include <cmath>
#include "TSystem.h"
#include "TCanvas.h"
#include "TString.h" // for Form(...)

using namespace std;

TH1F* h_muonMultiplicity;
TH1F* h_invariantMassEx1;
TH1F* h_jetMultiplicity;
TH1F* h_bJetMultiplicity;
TH1F* h_muonPt;
TH1F* h_jetPt;




void MyAnalysis::BuildEvent() {
   
   Muons.clear();
   for (int i = 0; i < NMuon; ++i) {
      MyMuon muon(Muon_Px[i], Muon_Py[i], Muon_Pz[i], Muon_E[i]);
      muon.SetIsolation(Muon_Iso[i]);
      muon.SetCharge(Muon_Charge[i]);
      Muons.push_back(muon);
   }
   
   Electrons.clear();
   for (int i = 0; i < NElectron; ++i) {
      MyElectron electron(Electron_Px[i], Electron_Py[i], Electron_Pz[i], Electron_E[i]);
      electron.SetIsolation(Electron_Iso[i]);
      electron.SetCharge(Electron_Charge[i]);
      Electrons.push_back(electron);
   }
   
   Photons.clear();
   for (int i = 0; i < NPhoton; ++i) {
      MyPhoton photon(Photon_Px[i], Photon_Py[i], Photon_Pz[i], Photon_E[i]);
      photon.SetIsolation(Photon_Iso[i]);
      Photons.push_back(photon);
   }
   
   Jets.clear();
   for (int i = 0; i < NJet; ++i) {
      MyJet jet(Jet_Px[i], Jet_Py[i], Jet_Pz[i], Jet_E[i]);
      jet.SetBTagDiscriminator(Jet_btag[i]);
      jet.SetJetID(Jet_ID[i]);
      Jets.push_back(jet);
   }
   
   hadB.SetXYZM(MChadronicBottom_px, MChadronicBottom_py, MChadronicBottom_pz, 4.8);
   lepB.SetXYZM(MCleptonicBottom_px, MCleptonicBottom_py, MCleptonicBottom_pz, 4.8);
   hadWq.SetXYZM(MChadronicWDecayQuark_px, MChadronicWDecayQuark_py, MChadronicWDecayQuark_pz, 0.0);
   hadWqb.SetXYZM(MChadronicWDecayQuarkBar_px, MChadronicWDecayQuarkBar_py, MChadronicWDecayQuarkBar_pz, 0.0);
   lepWl.SetXYZM(MClepton_px, MClepton_py, MClepton_pz, 0.0);
   lepWn.SetXYZM(MCneutrino_px, MCneutrino_py, MCneutrino_pz, 0.0);
   met.SetXYZM(MET_px, MET_py, 0., 0.);
  
   EventWeight *= weight_factor;
   
}

void MyAnalysis::Begin(TTree * /*tree*/) {
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).
   
   TString option = GetOption();

}

void MyAnalysis::SlaveBegin(TTree * /*tree*/) {
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).
   
   TString option = GetOption();
   
   h_Mmumu = new TH1F("Mmumu", "Invariant di-muon mass; m_{#mu#mu} [GeV]; Events", 60, 60, 120);
   h_Mmumu->Sumw2();
   histograms.push_back(h_Mmumu);
   histograms_MC.push_back(h_Mmumu);
   
   h_NMuon = new TH1F("NMuon", "Number of muons; No. Muons; Events", 4, 0, 4);
   h_NMuon->Sumw2();
   histograms.push_back(h_NMuon);
   histograms_MC.push_back(h_NMuon);

   h_muonMultiplicity = new TH1F("MuonMultiplicity", "Muon Multiplicity; Number of Isolated Muons; Events", 4, 0, 4);
   histograms.push_back(h_muonMultiplicity);
   histograms_MC.push_back(h_muonMultiplicity);
   
   h_jetMultiplicity = new TH1F("JetMultiplicity", "Jet Multiplicity; Number of Jets; Events", 4, 0, 4);
   histograms.push_back(h_jetMultiplicity);
   histograms_MC.push_back(h_jetMultiplicity);

   h_muonPt = new TH1F("MuonPt", "Muon p_{T}; p_{T}^{#mu} [GeV]; Events", 100, 0, 300);
   histograms.push_back(h_muonPt);
   histograms_MC.push_back(h_muonPt);

   h_jetPt = new TH1F("JetPt", "Jet p_{T}; p_{T}^{jet} [GeV]; Events", 100, 0, 300);
   histograms.push_back(h_jetPt);
   histograms_MC.push_back(h_jetPt);

   h_bJetMultiplicity = new TH1F("BJetMultiplicity", "b-tagged Jet Multiplicity; N_{b-jets}; Events", 10, 0, 10);
   histograms.push_back(h_bJetMultiplicity);
   histograms_MC.push_back(h_bJetMultiplicity);

   // Exercise 3: cutflow and MET
   h_Cutflow = new TH1F("Cutflow", "Cutflow (Exercise 3); Selection Step; Events", 8, 0.5, 8.5);
   h_Cutflow->Sumw2();
   h_Cutflow->GetXaxis()->SetBinLabel(1, "All");
   h_Cutflow->GetXaxis()->SetBinLabel(2, "Trigger");
   h_Cutflow->GetXaxis()->SetBinLabel(3, "1 Iso Muon");
   h_Cutflow->GetXaxis()->SetBinLabel(4, "Muon Kin");
   h_Cutflow->GetXaxis()->SetBinLabel(5, "Ele Veto");
   h_Cutflow->GetXaxis()->SetBinLabel(6, ">=4 Jets");
   h_Cutflow->GetXaxis()->SetBinLabel(7, ">=1 b-tag");
   h_Cutflow->GetXaxis()->SetBinLabel(8, "MET>20");
   histograms.push_back(h_Cutflow);
   histograms_MC.push_back(h_Cutflow);

   h_MET = new TH1F("MET", "Missing E_{T} (Exercise 3); E_{T}^{miss} [GeV]; Events", 40, 0, 200);
   h_MET->Sumw2();
   histograms.push_back(h_MET);
   histograms_MC.push_back(h_MET);

// 

}

Bool_t MyAnalysis::Process(Long64_t entry) {
   // The Process() function is called for each entry in the tree (or possibly
   // keyed object in the case of PROOF) to be processed. The entry argument
   // specifies which entry in the currently loaded tree is to be processed.
   // It can be passed to either MyAnalysis::GetEntry() or TBranch::GetEntry()
   // to read either all or the required parts of the data. When processing
   // keyed objects with PROOF, the object is already loaded and is available
   // via the fObject pointer.
   //
   // This function should contain the "body" of the analysis. It can contain
   // simple or elaborate selection criteria, run algorithms on the data
   // of the event and typically fill histograms.
   //
   // The processing can be stopped by calling Abort().
   //
   // Use fStatus to set the return value of TTree::Process().
   //
   // The return value is currently not used.
   
   ++TotalEvents;
   ++nProcessedEvents;
   
   GetEntry(entry);
   
   if (TotalEvents % 10000 == 0)
      cout << "Next event -----> " << TotalEvents << endl;
   
   BuildEvent();
   // Bookkeeping for cross-section normalization (MC)
   sumWAll += EventWeight;
   
   double MuonPtCut = 25.;
   double MuonRelIsoCut = 0.10;
   
   //   cout << "Jets: " << endl;
   //   for (vector<MyJet>::iterator it = Jets.begin(); it != Jets.end(); ++it) {
   //      cout << "pt, eta, phi, btag, id: " << it->Pt() << ", " << it->Eta() << ", " << it->Phi() << ", " << it->IsBTagged() << ", " << it->GetJetID()
   //      << endl;
   //   }
   //   cout << "Muons: " << endl;
   //   for (vector<MyMuon>::iterator it = Muons.begin(); it != Muons.end(); ++it) {
   //      cout << "pt, eta, phi, iso, charge: " << it->Pt() << ", " << it->Eta() << ", " << it->Phi() << ", "
   //      << it->GetIsolation() << ", " << it->GetCharge() << endl;
   //   }
   //   cout << "Electrons: " << endl;
   //   for (vector<MyElectron>::iterator it = Electrons.begin(); it != Electrons.end(); ++it) {
   //      cout << "pt, eta, phi, iso, charge: " << it->Pt() << ", " << it->Eta() << ", " << it->Phi() << ", "
   //      << it->GetIsolation() << ", " << it->GetCharge() << endl;
   //   }
   //   cout << "Photons: " << endl;
   //   for (vector<MyPhoton>::iterator it = Photons.begin(); it != Photons.end(); ++it) {
   //      cout << "pt, eta, phi, iso: " << it->Pt() << ", " << it->Eta() << ", " << it->Phi() << ", " << it->GetIsolation()
   //      << endl;
   //   }
   
   
   //////////////////////////////
   // Exercise 1: Invariant Di-Muon mass
   int N_IsoMuon = 0;
   std::vector<MyMuon*> isolatedMuons;
   for (vector<MyMuon>::iterator jt = Muons.begin(); jt != Muons.end(); ++jt) {
      if (jt->IsIsolated(MuonRelIsoCut)) {
         ++N_IsoMuon;
         isolatedMuons.push_back(&(*jt));
      }
   }
   h_muonMultiplicity->Fill(N_IsoMuon, EventWeight);
   
   // number of all muons
   h_NMuon->Fill(Muons.size(), EventWeight);

   std::vector<MyMuon*> invariantMuonmassPairs;
   for (size_t i = 0; i < isolatedMuons.size(); ++i) {
      for (size_t j = i+1; j < isolatedMuons.size(); ++j) {
         if (isolatedMuons[i]->GetCharge() != isolatedMuons[j]->GetCharge()) {
            // Calculate di-muon invariant mass using TLorentzVector addition
            double invMass = (*isolatedMuons[i] + *isolatedMuons[j]).M();
            h_Mmumu->Fill(invMass, EventWeight);
            invariantMuonmassPairs.push_back(isolatedMuons[i]);
            invariantMuonmassPairs.push_back(isolatedMuons[j]);
         }
      }
   }
   // exersice 2:
   std::vector<MyJet> btaggedJets;
   Long64_t numjets = 0;
   for (vector<MyJet>::iterator it = Jets.begin(); it != Jets.end(); ++it) {
      if (it->IsBTagged()) btaggedJets.push_back(*it);
      if (it->Pt() > 30. && fabs(it->Eta()) < 2.5 && it->GetJetID()) ++numjets;
   }

   h_jetMultiplicity->Fill(numjets, EventWeight);
   h_bJetMultiplicity->Fill(btaggedJets.size(), EventWeight);

   for (vector<MyJet>::iterator it = Jets.begin(); it != Jets.end(); ++it) {
      if (it->Pt() > 30. && fabs(it->Eta()) < 2.5 && it->GetJetID())
         h_jetPt->Fill(it->Pt(), EventWeight);
   }

   std::vector<MyMuon*> MuonPt;
   for (vector<MyMuon>::iterator jt = Muons.begin(); jt != Muons.end(); ++jt) {
      h_muonPt->Fill(jt->Pt(), EventWeight);
      MuonPt.push_back(&(*jt));
   }

   // exercise 3: mu+jets selection, trigger, cutflow, MET
   auto fill_cut = [&](int bin, double w){ if (h_Cutflow) h_Cutflow->Fill(bin, w); };
   // 1) all events
   fill_cut(1, EventWeight);

   // 2) trigger handling
   bool passTrigger = triggerIsoMu24;
   double w = EventWeight;
   if (isData) {
      if (!passTrigger) return kTRUE;
   } else {
      w *= epsilon_trigg_mu; // apply trigger efficiency to MC
   }
   fill_cut(2, w);

   // 3) exactly one isolated muon
   const double muPtCut = 25.0;
   const double muRelIsoCut = 0.10;
   std::vector<MyMuon*> goodMu;
   for (auto &mu : Muons) {
      if (mu.Pt() > muPtCut && fabs(mu.Eta()) < 2.4 && mu.IsIsolated(muRelIsoCut))
         goodMu.push_back(&mu);
   }
   if (goodMu.size() != 1) return kTRUE;
   fill_cut(3, w);

   // 4) muon kinematics already applied
   fill_cut(4, w);

   // 5) electron veto (no isolated e with pT>20)
   bool vetoEle = false;
   for (auto &el : Electrons) {
      double relIso = (el.Pt() > 0.) ? (el.GetIsolation() / el.Pt()) : 999.;
      if (el.Pt() > 20. && fabs(el.Eta()) < 2.5 && relIso < 0.15) { vetoEle = true; break; }
   }
   if (vetoEle) return kTRUE;
   fill_cut(5, w);

   // 6) at least 4 jets
   std::vector<MyJet*> selJets;
   for (auto &j : Jets) {
      if (j.Pt() > 30. && fabs(j.Eta()) < 2.5 && j.GetJetID()) selJets.push_back(&j);
   }
   if (selJets.size() < 4) return kTRUE;
   fill_cut(6, w);

   // 7) at least 1 b-tag
   int nb = 0; for (auto j : selJets) if (j->IsBTagged()) ++nb;
   if (nb < 1) return kTRUE;
   fill_cut(7, w);

   // 8) MET > 20 GeV
   double met_pt = sqrt(MET_px*MET_px + MET_py*MET_py);
   if (met_pt < 20.) return kTRUE;
   fill_cut(8, w);
   if (h_MET) h_MET->Fill(met_pt, w);

   sumWSelected += w;
   ++nSelected;



   return kTRUE;
}

void MyAnalysis::SlaveTerminate() {
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.
  
}

void MyAnalysis::Terminate() {
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.
   
   // Create results directory
   const char *outdir = "results";
   gSystem->Exec(Form("mkdir -p %s", outdir));

   // Helper to draw and save a histogram with a visible main title
   auto drawAndSave = [&](TH1* h){
      if (!h) return;
      TCanvas c;
      c.cd();
      // Use the histogram title for axis labels; also draw a visible title above
      TString fullTitle = h->GetTitle();
      // In ROOT, title string format is "MainTitle;X;Y". We'll extract the main title
      TString mainTitle = fullTitle;
      if (fullTitle.Contains(";")) mainTitle = fullTitle(0, fullTitle.First(";"));
      h->Draw();
      // Draw a TLatex at NDC coordinates for a clear main title
      TLatex latex;
      latex.SetNDC();
      latex.SetTextFont(42);
      latex.SetTextSize(0.04);
      latex.DrawLatex(0.15, 0.93, mainTitle);
      // Save
      TString filename = Form("%s/%s.png", outdir, h->GetName());
      c.SaveAs(filename);
   };

   // Save all histograms we created in SlaveBegin
   for (auto h : histograms) drawAndSave(h);

   // Also save MC histograms collection if different
   for (auto h : histograms_MC) {
      // avoid double saving same pointers
      if (std::find(histograms.begin(), histograms.end(), h) == histograms.end()) drawAndSave(h);
   }

}
