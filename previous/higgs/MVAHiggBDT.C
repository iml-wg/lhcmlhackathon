#include <cstdlib>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TObjString.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TPluginManager.h"
#include <fstream>
#include <iostream>
#include <string>    

#include "TMVA/Factory.h"
#include "TMVA/DataLoader.h"
#include "TMVA/Tools.h"
#include "TMVA/TMVAGui.h"

int MVAHiggBDT() {  

   // This loads the library
  TMVA::Tools::Instance();      

  TFile* outputFile = TFile::Open( "TMVA.root", "RECREATE" );
  TMVA::Factory* factory = new TMVA::Factory("tmvaTest", outputFile, "!V:!Silent:Color:DrawProgressBar:Transformations=I;D;P;G,D:AnalysisType=Classification");   

  TMVA::DataLoader *dataloader=new TMVA::DataLoader("dataset");  
  
  dataloader->AddVariable( "m_jj", 'F'); 
  dataloader->AddVariable( "m_jjj", 'F'); 
  dataloader->AddVariable( "m_lv", 'F'); 
  dataloader->AddVariable( "m_jlv", 'F'); 
  dataloader->AddVariable( "m_bb", 'F'); 

  dataloader->AddVariable( "m_wbb", 'F'); 
  dataloader->AddVariable( "m_wwbb", 'F');
  dataloader->AddVariable( "lepton_pT", 'F'); 
  dataloader->AddVariable( "missing_energy_magnitude", 'F'); 
  dataloader->AddVariable( "jet1_pt+jet2_pt+jet3_pt+jet4_pt","HT_Jets", 'F');
 

  dataloader->AddVariable( "jet1_pt", 'F');
  dataloader->AddVariable( "jet2_pt", 'F');  
  dataloader->AddVariable( "jet3_pt", 'F');
  dataloader->AddVariable( "jet4_pt", 'F'); 
   
  TFile* datafile = new TFile("/gpfs/atlas/mfaraj/INFN-2019-Stat-Tutorial/Higgs-Analysis-INFN/lhcmlhackathon/notebooks/data/Higgs_data.root");

// get the TTree objects from the input files

  TTree* sig = (TTree*)datafile->Get("sig_tree");
  TTree* bkg = (TTree*)datafile->Get("bkg_tree");
  
  int nSig = sig->GetEntries();
  int nBkg = bkg->GetEntries();  
  
  std::cout<<"NSig = \t"<<nSig<<std::endl;
  std::cout<<"NBkg = \t"<<nBkg<<std::endl;

  double sigWeight = 1.0;        // global weight
  double bkgWeight = 1.0;
  //factory->SetInputTrees(sig, bkg, sigWeight, bkgWeight);
  dataloader->AddSignalTree(sig,sigWeight);
  dataloader->AddBackgroundTree(bkg,bkgWeight);
  


//  dataloader->SetSignalWeightExpression    ("(1.0*(weight_normalise*weight_mc*weight_pileup*weight_jvt*weight_leptonSF*weight_bTagSF_MV2c10_77))*(43593.8/125469.66*(year == 2017)+(3219.56+32965.3)/125469.66*(year < 2017 && year > 2000) + 45691.0/125469.66*(year < 2000 || year > 2017) )");

//  dataloader->SetBackgroundWeightExpression("(1.0*(weight_normalise*weight_mc*weight_pileup*weight_jvt*weight_leptonSF*weight_bTagSF_MV2c10_77))*(43593.8/125469.66*(year == 2017)+(3219.56+32965.3)/125469.66*(year < 2017 && year > 2000) + 45691.0/125469.66*(year < 2000 || year > 2017) )");

//  TCut mycuts = "met_met>20000 && (mtw+met_met)>60000 && m_nBtags>=4 && m_nJets >=6";
//  TCut mycutb = "met_met>20000 && (mtw+met_met)>60000 && m_nBtags>=4 && m_nJets >=6";

  TCut mycuts = "";
  TCut mycutb = "";
  TCut mycut = "";
  int nTrain_Signal = 7000;
  int nTest_Signal = 3000;
  int nTrain_Background = 7000;
  int nTest_Background = 3000;

  dataloader->PrepareTrainingAndTestTree( mycuts, mycutb ,"nTrain_Signal=7000:nTrain_Background=7000:SplitMode=Random:NormMode=EqualNumEvents:!V" );
  //dataloader->PrepareTrainingAndTestTree(mycut, nTrain_Signal, 
   //      nTrain_Background, nTest_Signal, nTest_Background, splitMode);  

  factory->BookMethod( dataloader, TMVA::Types::kBDT, "BDTG",
                           "!H:!V:NTrees=400:MinNodeSize=2%:BoostType=Grad:Shrinkage=0.10:UseBaggedBoost:BaggedSampleFraction=0.5:nCuts=30:MaxDepth=3" );
  factory->BookMethod(dataloader, TMVA::Types::kBDT, "BDT",
        "!H:!V:NTrees=400:MinNodeSize=2%:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.5:UseBaggedBoost:BaggedSampleFraction=0.5:SeparationType=GiniIndex:nCuts=20");
        
factory->BookMethod(dataloader ,TMVA::Types::kPDERS, "PDERS",
"!H:V:NormTree=T:VolumeRangeMode=Adaptive:KernelEstimator=Gauss:GaussSigma=0.3:NEventsMin=400:NEventsMax=600");
// k-Nearest Neighbour method (similar to PDE-RS)
factory->BookMethod(dataloader ,TMVA::Types::kKNN, "KNN",
"H:nkNN=20:ScaleFrac=0.8:SigmaFact=1.0:Kernel=Gaus:UseKernel=F:UseWeight=T:!Trim" );    
   //  Feed-forward multilayer perceptron impementation
   //  4 hidden layers (separated by ",") 
   //  Activation function is TANH
   //  Activation function for the output layer is LINEAR
   //  First hidden layer has 16 nodes, then 8, then 4, then 2
   //  The error of the neural net is determined by a cross entropy function

   // let's add a new layer and change 
   TString layoutString ("Layout=TANH|32,TANH|16,TANH|8,TANH|4,TANH|2,LINEAR");
    
   TString training0 ("LearningRate=1e-1,Momentum=0.0,Repetitions=1,ConvergenceSteps=100,BatchSize=100,TestRepetitions=15,WeightDecay=0.001,Regularization=NONE,DropConfig=0.0+0.5+0.5+0.5,DropRepetitions=1,Multithreading=True,Optimizer=ADAM");
    
   TString training1 ("LearningRate=1e-2,Momentum=0.5,Repetitions=1,ConvergenceSteps=100,BatchSize=100,TestRepetitions=7,WeightDecay=0.001,Regularization=L2,Multithreading=True,DropConfig=0.0+0.1+0.1+0.1,DropRepetitions=1,Optimizer=ADAM");

   TString training2 ("LearningRate=1e-3,Momentum=0.4,Repetitions=1,ConvergenceSteps=50,BatchSize=100,TestRepetitions=7,WeightDecay=0.001,Regularization=L2,Multithreading=True,DropConfig=0.0+0.1+0.1+0.1,DropRepetitions=1,Optimizer=ADAM");

   TString training3 ("LearningRate=1e-4,Momentum=0.3,Repetitions=1,ConvergenceSteps=50,BatchSize=100,TestRepetitions=7,WeightDecay=0.0001,Regularization=L2,Multithreading=True,Optimizer=ADAM");

   TString training4 ("LearningRate=1e-4,Momentum=0.1,Repetitions=1,ConvergenceSteps=50,BatchSize=100,TestRepetitions=7,WeightDecay=0.0001,Regularization=NONE,Multithreading=True,Optimizer=ADAM");
   TString training5 ("LearningRate=1e-4,Momentum=0.1,Repetitions=1,ConvergenceSteps=50,BatchSize=100,TestRepetitions=7,WeightDecay=0.0001,Regularization=L2,Multithreading=True,Optimizer=ADAM");
/*   
   TString training6 ("LearningRate=1e-4,Momentum=0.1,Repetitions=1,ConvergenceSteps=50,BatchSize=100,TestRepetitions=7,WeightDecay=0.0001,Regularization=NONE,Multithreading=True,Optimizer=ADAM");   
   
   TString training7 ("LearningRate=1e-4,Momentum=0.1,Repetitions=1,ConvergenceSteps=50,BatchSize=100,TestRepetitions=7,WeightDecay=0.0001,Regularization=NONE,Multithreading=True,Optimizer=ADAM");      
*/
   TString trainingStrategyString ("TrainingStrategy=");
   trainingStrategyString += training0 + "|" + training1 + "|" + training2 + "|" + training3 + "|" + training4 + "|" + training5 ;

   TString dnnOptions ("!H:V:ErrorStrategy=CROSSENTROPY:VarTransform=G:WeightInitialization=XAVIERUNIFORM");

   dnnOptions.Append (":"); dnnOptions.Append (layoutString);
   dnnOptions.Append (":"); dnnOptions.Append (trainingStrategyString);

   TString cpuOptions = dnnOptions + ":Architecture=CPU";
   factory->BookMethod(dataloader, TMVA::Types::kDNN, "DNN", cpuOptions);  
   
// Artificial Neural Network (Multilayer perceptron) - TMVA version
factory->BookMethod( dataloader,TMVA::Types::kMLP, "MLP",
"H:!V:NeuronType=tanh:VarTransform=N:NCycles=600:HiddenLayers=N+5:TestRate=5" );  

// NN with BFGS quadratic minimisation
factory->BookMethod( dataloader,TMVA::Types::kMLP, "MLPBFGS",
"H:!V:NeuronType=tanh:VarTransform=N:NCycles=600:HiddenLayers=N+5:TestRate=5:TrainingMethod=BFGS" ); 

// NN (Multilayer perceptron) - ROOT version
factory->BookMethod( dataloader,TMVA::Types::kTMlpANN, "TMlpANN",
"!H:!V:NCycles=200:HiddenLayers=N+1,N:LearningMethod=BFGS:ValidationFraction=0.3" );

// NN (Multilayer perceptron) - ALEPH version (depreciated)
factory->BookMethod( dataloader,TMVA::Types::kCFMlpANN, "CFMlpANN",
"!H:!V:NCycles=2000:HiddenLayers=N+1,N" );
// Support Vector Machine
factory->BookMethod( dataloader,TMVA::Types::kSVM, "SVM", "Gamma=0.25:Tol=0.001" );

// Boosted Decision Trees with bagging
factory->BookMethod( dataloader,TMVA::Types::kBDT, "BDTB",
"!H:!V:NTrees=400:BoostType=Bagging:SeparationType=GiniIndex:nCuts=20:PruneMethod=NoPruning" );
  
/*        
  factory->BookMethod(dataloader, TMVA::Types::kPyGTB, "PyGTB",
        "H:!V:VarTransform=G:NEstimators=400:LearningRate=0.1:MaxDepth=3");   
        
          
  factory->BookMethod(dataloader, TMVA::Types::kPyRandomForest, "PyRandomForest",
        "!V:VarTransform=G:NEstimators=400:Criterion=gini:MaxFeatures=auto:MaxDepth=6:MinSamplesLeaf=3:MinWeightFractionLeaf=0:Bootstrap=kTRUE");             
  factory->BookMethod(dataloader, TMVA::Types::kPyAdaBoost, "PyAdaBoost",
        "!V:VarTransform=G:NEstimators=400");          
*/
   // Train MVAs using the set of training events
   factory->TrainAllMethods();

   // ---- Evaluate all MVAs using the set of test events
   factory->TestAllMethods();

   // ----- Evaluate and compare performance of all configured MVAs
   factory->EvaluateAllMethods();

   // --------------------------------------------------------------

   // Save the output
   outputFile->Close();
   //float SepValue = 
   std::cout << "==> Wrote root file: " << outputFile->GetName() << std::endl;
   std::cout << "==> TMVAClassification is done!" << std::endl;

   delete factory;
   delete dataloader;
  return 0;
}
