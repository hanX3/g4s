#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "Constants.hh"
#include "CathodePixel.hh"

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
#include "tls.hh"

#include "G4Element.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PhysicalConstants.hh"

class G4LogicalVolume;
class G4Material;
class G4UserLimits;
class DetectorMessenger;

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
  DetectorConstruction();
  ~DetectorConstruction() override;

public:
  G4VPhysicalVolume* Construct() override;

  // Set methods
  void SetMaxStep(G4double );
  void SetCheckOverlaps(G4bool );

private:
  // methods
  void DefineMaterials();

  void GetPPACLog(G4String ppac);
  void GetMylarFaceLog(G4String mf);
  void GetAlFaceLog(G4String af);

  void BuildCathodeArray();

  G4VPhysicalVolume* DefineVolumes();

private:
  std::vector<CathodePixel*> v_cathode_array;

  //
  G4double ppac_outer_r;
  G4double ppac_inner_r;
  G4double ppac_thickness;
  G4double ppac_pos_z;

  G4double mylar_face_outer_r;
  G4double mylar_face_inner_r;
  G4double mylar_face_thickness;
  G4double mylar_face_pos_z;

  G4double al_face_outer_r;
  G4double al_face_inner_r;
  G4double al_face_thickness;
  G4double al_face_pos_z;

  //
  G4int rings;
  G4int sectors;
  G4double target2cathode;

private:
  //
  G4LogicalVolume *world_log;
  G4LogicalVolume *ppac_log;
  G4LogicalVolume *mylar_face_log;
  G4LogicalVolume *al_face_log;

  //
  G4Material *mat_air;
  G4Material *mat_vaccum;
  G4Material *mat_al;
  G4Material *mat_mylar;
  G4Material *mat_isobutene;

  G4UserLimits *step_limit;
  G4bool check_overlaps;
};

#endif
