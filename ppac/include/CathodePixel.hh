#ifndef CathodePixel_h
#define CathodePixel_h 1

#include "Constants.hh"

#include "G4Material.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4ThreeVector.hh"
#include "G4Tubs.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4Color.hh"
#include "G4VisAttributes.hh"
#include "G4SDManager.hh"
#include "G4NistManager.hh"
#include "G4Tubs.hh"
#include "G4SubtractionSolid.hh"
#include "G4RunManager.hh"

//
class CathodePixel
{
public:
  CathodePixel(G4LogicalVolume *l);
  ~CathodePixel();

public:
  G4LogicalVolume *log;

public:  
  G4VPhysicalVolume *Construct();
  void SetRingId(G4int i) { ring_id = i; };
  void SetSectorId(G4int i) { sector_id = i; };
  void SetId(G4int i) { id = i; };
  void SetName();
  void Report();
  void Place(G4ThreeVector p);

  G4LogicalVolume *GetLog() { return cathode_pixel_log; };

private:
  // dimensions
  G4double inner_radius;
  G4double outer_radius;
  G4double angle_start;
  G4double thickness;

  // materials
  G4Material *mat_cu;

  // default position
  G4ThreeVector *pos;

  // detector
  G4Tubs *cathode_pixel_solid;
  G4LogicalVolume *cathode_pixel_log;
  G4VPhysicalVolume *cathode_pixel_phys;

  //
  G4double target2cathode;

  G4int ring_id; // 0-52
  G4int sector_id; // 0-179
  G4int id;
};

#endif
