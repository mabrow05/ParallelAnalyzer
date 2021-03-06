//////////////////////////////////////////////////////////
// Applies Bi pulser gain monitoring corrections to 
// Scintillator PMT signals
//////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>

// ROOT libraries
#include "TRandom3.h"
#include <TTree.h>
#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TF1.h>
#include <TH1D.h>

#include "fullTreeVariables.h"
#include "MWPCGeometry.h"
#include "pedestals.h"
#include "cuts.h"
#include "basic_reconstruction.h"
#include "DataTree.hh"

#include "replay_pass2.h"


using namespace std;

int main(int argc, char *argv[])
{
  cout.setf(ios::fixed, ios::floatfield);
  cout.precision(12);

  cout << "Run " << argv[1] << " ..." << endl;
  cout << "... Applying Bi pulser gain corrections ..." << endl;

  // Read gain corrections file
  char tempFileGain[500];
  sprintf(tempFileGain, "%s/gain_bismuth_%s.dat",getenv("GAIN_BISMUTH"), argv[1]);
  cout << "... Reading: " << tempFileGain << endl;

  double fitMean[8], gainCorrection[8];
  ifstream fileGain(tempFileGain);
  for (int i=0; i<8; i++) {
    fileGain >> fitMean[i] >> gainCorrection[i];
  }
  cout << "...   PMT E1: " << gainCorrection[0] << endl;
  cout << "...   PMT E2: " << gainCorrection[1] << endl;
  cout << "...   PMT E3: " << gainCorrection[2] << endl;
  cout << "...   PMT E4: " << gainCorrection[3] << endl;
  cout << "...   PMT W1: " << gainCorrection[4] << endl;
  cout << "...   PMT W2: " << gainCorrection[5] << endl;
  cout << "...   PMT W3: " << gainCorrection[6] << endl;
  cout << "...   PMT W4: " << gainCorrection[7] << endl;

  // Open output ntuple
  char tempOut[500];
  sprintf(tempOut, "%s/replay_pass2_%s.root",getenv("REPLAY_PASS2"), argv[1]);
  //sprintf(tempOut, "replay_pass2_%s.root", argv[1]);
  DataTree *t = new DataTree();
  t->makeOutputTree(std::string(tempOut),"pass2");

  char tempIn[500];
  sprintf(tempIn, "%s/replay_pass1_%s.root", getenv("REPLAY_PASS1"),argv[1]);
  //sprintf(tempIn, "../replay_pass1/replay_pass1_%s.root", argv[1]);
  t->setupInputTree(std::string(tempIn),"pass1");

  int nEvents = t->getEntries();
  cout << "... Processing nEvents = " << nEvents << endl;

  // Loop over events
  for (Int_t i=0; i<nEvents; i++) {
    t->getEvent(i);

    // Apply gain correction factors
    t->ScintE.q1 = t->ScintE.q1*gainCorrection[0];
    t->ScintE.q2 = t->ScintE.q2*gainCorrection[1];
    t->ScintE.q3 = t->ScintE.q3*gainCorrection[2];
    t->ScintE.q4 = t->ScintE.q4*gainCorrection[3];

    t->ScintW.q1 = t->ScintW.q1*gainCorrection[4];
    t->ScintW.q2 = t->ScintW.q2*gainCorrection[5];
    t->ScintW.q3 = t->ScintW.q3*gainCorrection[6];
    t->ScintW.q4 = t->ScintW.q4*gainCorrection[7]; 

    t->fillOutputTree();
  }

  // Write output ntuple
  t->writeOutputFile();
  delete t;

  return 0;
}

//////// OLD OUTPUT INPUT TREES /////////////////

/*TFile *fileOut = new TFile(tempOut,"RECREATE");
  TTree *Tout = new TTree("pass2", "pass2");

  // Variables
  Tout->Branch("pmt0_pass2", &pmt_pass2[0], "pmt0_pass2/D");
  Tout->Branch("pmt1_pass2", &pmt_pass2[1], "pmt1_pass2/D");
  Tout->Branch("pmt2_pass2", &pmt_pass2[2], "pmt2_pass2/D");
  Tout->Branch("pmt3_pass2", &pmt_pass2[3], "pmt3_pass2/D");
  Tout->Branch("pmt4_pass2", &pmt_pass2[4], "pmt4_pass2/D");
  Tout->Branch("pmt5_pass2", &pmt_pass2[5], "pmt5_pass2/D");
  Tout->Branch("pmt6_pass2", &pmt_pass2[6], "pmt6_pass2/D");
  Tout->Branch("pmt7_pass2", &pmt_pass2[7], "pmt7_pass2/D");

  Tout->Branch("AnodeE", &AnodeE, "AnodeE/D");
  Tout->Branch("AnodeW", &AnodeW, "AnodeW/D");

  Tout->Branch("timeE", &timeE, "timeE/D");
  Tout->Branch("timeW", &timeW, "timeW/D");
  Tout->Branch("timeE_BB", &timeE_BB, "timeE_BB/D");
  Tout->Branch("timeW_BB", &timeW_BB, "timeW_BB/D");
  Tout->Branch("UBtime", &UBtime, "UBtime/D");
  Tout->Branch("UBtime_BB", &UBtime_BB, "UBtime_BB/D");
  Tout->Branch("twoFoldE", &twoFoldE, "twoFoldE/D");
  Tout->Branch("twoFoldW", &twoFoldW, "twoFoldW/D");

  Tout->Branch("xE_pass2", &xE_pass2, "xE_pass2/D");
  Tout->Branch("yE_pass2", &yE_pass2, "yE_pass2/D");
  Tout->Branch("xW_pass2", &xW_pass2, "xW_pass2/D");
  Tout->Branch("yW_pass2", &yW_pass2, "yW_pass2/D");

  int xeRC,yeRC,xwRC,ywRC;	
  Tout->Branch("xeRC", &xeRC, "xeRC/I"); //x east response class. 
  Tout->Branch("yeRC", &yeRC, "yeRC/I"); //y east response class... 
  Tout->Branch("xwRC", &xwRC, "xwRC/I");
  Tout->Branch("ywRC", &ywRC, "ywRC/I");

  Tout->Branch("PID_pass2",  &PID_pass2,  "PID_pass2/I");
  Tout->Branch("type_pass2", &type_pass2, "type_pass2/I");
  Tout->Branch("side_pass2", &side_pass2, "side_pass2/I");
  Tout->Branch("posError_pass2", &posError_pass2, "posError_pass2/I");

  // Open input ntuple
  char tempIn[500];
  sprintf(tempIn, "%s/replay_pass1_%s.root", getenv("REPLAY_PASS1"),argv[1]);

  TFile *fileIn = new TFile(tempIn, "READ");
  TTree *Tin = (TTree*)(fileIn->Get("pass1"));

  // Variables
  Tin->SetBranchAddress("pmt0", &pmt[0]);
  Tin->SetBranchAddress("pmt1", &pmt[1]);
  Tin->SetBranchAddress("pmt2", &pmt[2]);
  Tin->SetBranchAddress("pmt3", &pmt[3]);
  Tin->SetBranchAddress("pmt4", &pmt[4]);
  Tin->SetBranchAddress("pmt5", &pmt[5]);
  Tin->SetBranchAddress("pmt6", &pmt[6]);
  Tin->SetBranchAddress("pmt7", &pmt[7]);

  Tin->SetBranchAddress("AnodeE", &AnodeE);
  Tin->SetBranchAddress("AnodeW", &AnodeW);
  
  Tin->SetBranchAddress("timeE", &timeE);
  Tin->SetBranchAddress("timeW", &timeW);
  Tin->SetBranchAddress("timeE_BB", &timeE_BB);
  Tin->SetBranchAddress("timeW_BB", &timeW_BB);
  Tin->SetBranchAddress("UBtime", &UBtime);
  Tin->SetBranchAddress("UBtime_BB", &UBtime_BB);
  Tin->SetBranchAddress("twoFoldE", &twoFoldE);
  Tin->SetBranchAddress("twoFoldW", &twoFoldW);

  Tin->SetBranchAddress("xE", &xE);
  Tin->SetBranchAddress("yE", &yE);
  Tin->SetBranchAddress("xW", &xW);
  Tin->SetBranchAddress("yW", &yW);

  //Adding in wirechamber class variable
  Tin->SetBranchAddress("xeRC",&xeRC);
  Tin->SetBranchAddress("yeRC",&yeRC);
  Tin->SetBranchAddress("xwRC",&xwRC);
  Tin->SetBranchAddress("ywRC",&ywRC);

  Tin->SetBranchAddress("PID",  &PID);
  Tin->SetBranchAddress("type", &type);
  Tin->SetBranchAddress("side", &side);
  Tin->SetBranchAddress("posError", &posError);
*/
