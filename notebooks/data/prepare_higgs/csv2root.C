//Author: Omar.Zapata@cern.ch http://oproject.org
#include "Riostream.h"                                                                                                                                                    
#include "TString.h"                                                                                                                                                      
#include "TFile.h"                                                                                                                                                        
#include "TTree.h"                                                                                                                                                        
#include "TSystem.h"                                                                                                                                                      
  
/*
//the 28 variables for dataset                                                                                                                                                                       
//lepton_pT
//lepton_eta
//lepton_phi
//missing_energy_magnitude
//missing_energy_phi
//jet_1_pt
//jet_1_eta
//jet_1_phi
//jet_1_b_tag
//jet_2_pt
//jet_2_eta
//jet_2_phi
//jet_2_b_tag
//jet_3_pt
//jet_3_eta
//jet_3_phi
//jet_3_b_tag
//jet_4_pt
//jet_4_eta
//jet_4_phi
//jet_4_b_tag
//m_jj
//m_jjj
//m_lv
//m_jlv
//m_bb
//m_wbb
//m_wwbb
*/

                                                                                                                                                                          
void csv2root() {                                                                                                                                                         
                                                                                                                                                                          
   TString dir = gSystem->UnixPathName(__FILE__);                                                                                                                         
   dir.ReplaceAll("csv2root.C","");                                                                                                                                       
   dir.ReplaceAll("/./","/");                                                                                                                                             
                                                                                                                                                                          
   TFile *f = new TFile("higgs-dataset.root","RECREATE");                                                                                                                 
   TTree *TresS = new TTree("TreeS","signal data");

   TresS->ReadFile("higgs-signal.csv","lepton_pT/F:lepton_eta/F:lepton_phi/F:missing_energy_magnitude/F:missing_energy_phi/F:jet_1_pt/F:jet_1_eta/F:jet_1_phi/F:jet_1_b_tag/F:jet_2_pt/F:jet_2_eta/F:jet_2_phi/F:jet_2_b_tag/F:jet_3_pt/F:jet_3_eta/F:jet_3_phi/F:jet_3_b_tag/F:jet_4_pt/F:jet_4_eta/F:jet_4_phi/F:jet_4_b_tag/F:m_jj/F:m_jjj/F:m_lv/F:m_jlv/F:m_bb/F:m_wbb/F:m_wwbb/F",',');

   TTree *TreeB = new TTree("TreeB","bkg data");
   TreeB->ReadFile("higgs-bgk.csv","lepton_pT/F:lepton_eta/F:lepton_phi/F:missing_energy_magnitude/F:missing_energy_phi/F:jet_1_pt/F:jet_1_eta/F:jet_1_phi/F:jet_1_b_tag/F:jet_2_pt/F:jet_2_eta/F:jet_2_phi/F:jet_2_b_tag/F:jet_3_pt/F:jet_3_eta/F:jet_3_phi/F:jet_3_b_tag/F:jet_4_pt/F:jet_4_eta/F:jet_4_phi/F:jet_4_b_tag/F:m_jj/F:m_jjj/F:m_lv/F:m_jlv/F:m_bb/F:m_wbb/F:m_wwbb/F",',');
   f->Write();  
}
