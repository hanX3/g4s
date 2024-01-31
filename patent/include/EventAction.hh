#ifndef B2EventAction_h
#define B2EventAction_h 1

#include "Constants.hh"
#include "DataStructure.hh"
#include "G4UserEventAction.hh"
#include "globals.hh"

namespace example
{

class RootIO;
class TFile;
class PrimaryGeneratorAction;

/// Event action class
class EventAction : public G4UserEventAction
{
public:
  EventAction(PrimaryGeneratorAction *pg, RootIO *rio);
  ~EventAction() override;

  void  BeginOfEventAction(const G4Event* ) override;
  void  EndOfEventAction(const G4Event* ) override;

private:
  EventData event_data;

  PrimaryGeneratorAction* primary;
  RootIO* root_io;    
};

}

#endif
