#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "Constants.hh"
#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
#include "tls.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;
class G4Material;
class G4UserLimits;

namespace example
{

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

    void GetAuLayerLog(G4String all);
    void GetTargetLog(G4String tl);
    G4VPhysicalVolume* DefineVolumes();

    // data members
    G4LogicalVolume* logic_target = nullptr;
    G4LogicalVolume* logic_au_layer = nullptr;
    G4Material* mat_air = nullptr;
    G4Material* mat_vaccum = nullptr;
    G4Material* mat_si = nullptr;
    G4Material* mat_au = nullptr;
    G4UserLimits* step_limit = nullptr;
    G4bool check_overlaps = true;
};

}

#endif
