#include "DetectorConstruction.hh"
#include "TargetSD.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4SDManager.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4GlobalMagFieldMessenger.hh"
#include "G4AutoDelete.hh"

#include "G4GeometryTolerance.hh"
#include "G4GeometryManager.hh"

#include "G4UserLimits.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4SystemOfUnits.hh"

namespace example
{

//
DetectorConstruction::DetectorConstruction()
{

}

//
DetectorConstruction::~DetectorConstruction()
{
  delete step_limit;
}

//
G4VPhysicalVolume* DetectorConstruction::Construct()
{
  DefineMaterials();
  return DefineVolumes();
}

//
void DetectorConstruction::DefineMaterials()
{
  G4NistManager* nist_manager = G4NistManager::Instance();

  mat_air = nist_manager->FindOrBuildMaterial("G4_AIR");
  mat_vaccum = nist_manager->FindOrBuildMaterial("G4_Galactic");
  mat_si = nist_manager->FindOrBuildMaterial("G4_Si");
  mat_au = nist_manager->FindOrBuildMaterial("G4_Au");

  // Print materials
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

//
void DetectorConstruction::GetAuLayerLog(G4String all)
{
  G4cout<<" --->: GetAuLayer, begin" << G4endl;

  G4Tubs *solid_au_layer = new G4Tubs("AuLayer", 0, AuLayerR, AuLayerThickness, 0, CLHEP::twopi);
  logic_au_layer = new G4LogicalVolume(solid_au_layer, mat_au, all);

  // color
  G4VisAttributes  *vis_att = new G4VisAttributes(G4Colour(1.0, 0.6, 1.0, 0.3));
  vis_att->SetForceSolid(true);
  logic_au_layer->SetVisAttributes(vis_att);
}

//
void DetectorConstruction::GetTargetLog(G4String tl)
{
  G4cout<<" --->: GetTarget, begin" << G4endl;

  G4Tubs *solid_target = new G4Tubs("Target", 0, TargetR, TargetThickness, 0, CLHEP::twopi);
  logic_target = new G4LogicalVolume(solid_target, mat_si, tl);

  // color
  G4VisAttributes  *vis_att = new G4VisAttributes(G4Colour(1.0, 1.0, 0.0, 0.3));
  vis_att->SetForceSolid(true);
  logic_target->SetVisAttributes(vis_att);
}

//
G4VPhysicalVolume* DetectorConstruction::DefineVolumes()
{
  // define world
  G4Box* solid_world = new G4Box("World", 0.5 * WorldSizeX, 0.5 * WorldSizeY, 0.5 * WorldSizeZ);
  G4LogicalVolume* logic_world = new G4LogicalVolume(solid_world, mat_vaccum, "World");
  G4VPhysicalVolume* phys_world = new G4PVPlacement(0, G4ThreeVector(), logic_world, "World", 0, false, 0, check_overlaps);

  // au layer
  GetAuLayerLog("AuLayer");
  G4ThreeVector position_au_layer = G4ThreeVector(PosAuLayerX, PosAuLayerY, PosAuLayerZ);
  new G4PVPlacement(0, position_au_layer, logic_au_layer, "AuLayer", logic_world, false, 0, check_overlaps);

  // target
  GetTargetLog("Target");
  G4ThreeVector position_target = G4ThreeVector(PosTarX, PosTarY, PosTarZ);
  new G4PVPlacement(0, position_target, logic_target, "Target", logic_world, false, 0, check_overlaps);


  // Example of User Limits
  // Below is an example of how to set tracking constraints in a given logical volume
  // Sets a max step length in the tracker region, with G4StepLimiter
  G4double max_step = 1. * um;
  step_limit = new G4UserLimits(max_step);
  logic_target->SetUserLimits(step_limit);

  /// Set additional contraints on the track, with G4UserSpecialCuts
  /// G4double max_length = 2*TargetThickness, max_time = 0.1*ns, min_ekin = 10*MeV;
  /// trackerLV->SetUserLimits(new G4UserLimits(max_step, max_length, max_time, min_ekin));

  // Sensitive detectors
  G4SDManager* sd_manager = G4SDManager::GetSDMpointer();
  TargetSD *tar_sd = new TargetSD("TargetSD", "TargetHitCollection");
  logic_target->SetSensitiveDetector(tar_sd);
  sd_manager->AddNewDetector(tar_sd);

  // Always return the physical world
  return phys_world;
}

//
void DetectorConstruction::SetMaxStep(G4double max_step)
{
  if ((step_limit)&&(max_step>0.)) step_limit->SetMaxAllowedStep(max_step);
}

//
void DetectorConstruction::SetCheckOverlaps(G4bool co)
{
  check_overlaps = co;
}

//

}
