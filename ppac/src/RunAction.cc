#include "RunAction.hh"
#include "RootIO.hh"
#include "PrimaryGeneratorAction.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4ios.hh"
#include "G4Timer.hh"
#include "G4UnitsTable.hh"
#include "G4ParticleGun.hh"
#include "unistd.h"
#include <fstream>
#include <string>


//
RunAction::RunAction(PrimaryGeneratorAction* pg, RootIO* rio)
: G4UserRunAction(),
  pri_gen(pg),
  root_io(rio)
{
  // set printing event number per each 100000 events
  G4RunManager::GetRunManager()->SetPrintProgress(100000);

  timer = new G4Timer();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
RunAction::~RunAction()
{
  delete timer;
  timer = NULL;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void RunAction::BeginOfRunAction(const G4Run* run)
{
  //inform the runManager to save random number seed
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);

  if((MASK&0b001)==0b001){
    root_io->OpenEventFile();
    G4cout << "open event root file " << G4endl;
  }
  if((MASK&0b010)==0b010){
    root_io->OpenTrackFile();
    G4cout << "open track root file " << G4endl;
  }
  if((MASK&0b100)==0b100){
    root_io->OpenStepFile();
    G4cout << "open step root file " << G4endl;
  }

  int run_id = run->GetRunID();
  timer->Start();
  G4cout << "======================   RunID = " << run_id << "  ======================" << G4endl;

  G4cout << "1 energy " << G4endl;
  G4cout << "2 track id " << G4endl;
  G4cout << "3 particle name " << G4endl;
  G4cout << "4 pre postion " << G4endl;
  G4cout << "5 post postion " << G4endl;
  G4cout << "6 detector name " << G4endl;
  G4cout << "7 step length " << G4endl;
  G4cout << "8 delat postion " << G4endl;
  G4cout << "9 delat time " << G4endl;
  G4cout << "10 delat energy " << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run* run)
{
  if((MASK&0b001)==0b001){
    root_io->CloseEventFile();
  }
  if((MASK&0b010)==0b010){
    root_io->CloseTrackFile();
  }
  if((MASK&0b100)==0b100){
    root_io->CloseStepFile();
  }

  // Print results
  G4cout << "  The run was " << run->GetNumberOfEvent() << " events " << G4endl;

  timer->Stop();
  G4cout << " time:  " << *timer << G4endl;
}

