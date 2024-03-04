#include "EventAction.hh"

#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4ios.hh"
#include "G4SDManager.hh"
#include "G4RunManager.hh"

#include "TargetHit.hh"
#include "RootIO.hh"

#include "TMath.h"


//
EventAction::EventAction(PrimaryGeneratorAction *pg, RootIO *rio)
: G4UserEventAction(),
  primary(pg),
  root_io(rio)
{
  event_data.Clear();
}

//
EventAction::~EventAction()
{}

//
void EventAction::BeginOfEventAction(const G4Event*)
{}

//
void EventAction::EndOfEventAction(const G4Event* event)
{
  auto sd_manager = G4SDManager::GetSDMpointer();
  G4double idd = sd_manager->GetCollectionID("TargetSD/TargetHitCollection");

  if((MASK&0b001)==0b001){
    // Get hits collections
    auto hce = event->GetHCofThisEvent();
    auto hc = static_cast<TargetHitsCollection*>(hce->GetHC(idd));
    event_data.e = (*hc)[0]->GetEdep();
    event_data.x = (*hc)[0]->GetPos().x();
    event_data.y = (*hc)[0]->GetPos().y();
    event_data.z = (*hc)[0]->GetPos().z();
    root_io->FillEventTree(event_data);
  }

  // periodic printing
  G4int eventID = event->GetEventID();
  if ( eventID < 10 || eventID % 1000 == 0) {
    G4cout << ">>> Event: " << eventID  << G4endl;
  }
}

//

