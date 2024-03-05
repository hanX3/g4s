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


//
DetectorConstruction::DetectorConstruction()
{
  //
  ppac_outer_r = PPACOuterR;
  ppac_inner_r = PPACInnerR;
  ppac_thickness = PPACThickness;
  ppac_pos_z = PPACPosZ;

  mylar_face_outer_r = MylarFaceOuterR;
  mylar_face_inner_r = MylarFaceInnerR;
  mylar_face_thickness = MylarFaceThickness;
  mylar_face_pos_z = MylarFacePosZ;

  al_face_outer_r = AlFaceOuterR;
  al_face_inner_r = AlFaceInnerR;
  al_face_thickness = AlFaceThickness;
  al_face_pos_z = AlFacePosZ;

  rings = RingNumber;
  sectors = SectorNumber;
  target2cathode = Target2Cathode;

  ppac_log = nullptr;
  mylar_face_log = nullptr;
  al_face_log = nullptr;

  mat_air = nullptr;
  mat_vaccum = nullptr;
  mat_al = nullptr;
  mat_mylar = nullptr;
  mat_isobutene = nullptr;

  check_overlaps = true;
}

//
DetectorConstruction::~DetectorConstruction()
{
  delete step_limit;
}

//
G4VPhysicalVolume *DetectorConstruction::Construct()
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
  mat_al = nist_manager->FindOrBuildMaterial("G4_Al");

  G4Element *el_h = nist_manager->FindOrBuildElement("H");
  G4Element *el_c = nist_manager->FindOrBuildElement("C");
  G4Element *el_o = nist_manager->FindOrBuildElement("O");

  //
  mat_mylar = new G4Material("Mylar", 1.38*g/cm3, 3);
  mat_mylar-> AddElement(el_c, 10); 
  mat_mylar-> AddElement(el_h, 8); 
  mat_mylar-> AddElement(el_o, 4); 

  //
  // temperature set 15C 
  const G4double exp_tmp= STP_Temperature+15.*kelvin;
  // 2.51 mg mL-` , at 15C, 100 kPa
  const G4double isobutane_data_tmp= STP_Temperature+15.*kelvin;
  const G4double pressure = 1.3158e-2*atmosphere; //6 torr = 0.007895 atm, 10 torr = 0.013158, 50 = 0.065789
  const G4double normal_atm = 1.*atmosphere;
  G4double den_isobutene = 2.51e-3*g/cm3*isobutane_data_tmp/exp_tmp*pressure/normal_atm;
  mat_isobutene = new G4Material("Isobutene", den_isobutene, 2, kStateGas, exp_tmp, pressure);
  mat_isobutene->AddElement(el_c, 4);
  mat_isobutene->AddElement(el_h, 10);

  // Print materials
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

//
void DetectorConstruction::GetPPACLog(G4String ppac)
{
  G4cout<<" --->: Build PPAC, begin" << G4endl;

  G4Tubs *ppac_solid = new G4Tubs("ppac_solid", ppac_inner_r, ppac_outer_r, ppac_thickness/2., 0, CLHEP::twopi);
  ppac_log = new G4LogicalVolume(ppac_solid, mat_isobutene, ppac);

  // color
  G4VisAttributes  *vis_att = new G4VisAttributes(G4Colour(1.0, 0.6, 1.0, 0.3));
  vis_att->SetForceSolid(true);
  ppac_log->SetVisAttributes(vis_att);
}

//
void DetectorConstruction::GetMylarFaceLog(G4String mf)
{
  G4cout<<" --->: Build Mylar, begin" << G4endl;

  G4Tubs *mylar_face_solid = new G4Tubs("mylar_face_solid", mylar_face_inner_r, mylar_face_outer_r, mylar_face_thickness/2., 0, CLHEP::twopi);
  mylar_face_log = new G4LogicalVolume(mylar_face_solid, mat_mylar, mf);

  // color
  G4VisAttributes  *vis_att = new G4VisAttributes(G4Colour(1., 1., 1., 0.3));
  vis_att->SetForceSolid(true);
  mylar_face_log->SetVisAttributes(vis_att);
}

//
void DetectorConstruction::GetAlFaceLog(G4String af)
{
  G4cout<<" --->: Build Al, begin" << G4endl;

  G4Tubs *al_face_solid = new G4Tubs("al_face_solid", al_face_inner_r, al_face_outer_r, al_face_thickness/2., 0, CLHEP::twopi);
  al_face_log = new G4LogicalVolume(al_face_solid, mat_al, af);

  // color
  G4VisAttributes  *vis_att = new G4VisAttributes(G4Colour(1., 1., 0., 0.3));
  vis_att->SetForceSolid(true);
  al_face_log->SetVisAttributes(vis_att);
}

//
void DetectorConstruction::BuildCathodeArray()
{
  G4cout<<" --->: Build Cthode array, begin" << G4endl;

  // cathode array
  G4Tubs *cathode_array_solid = new G4Tubs("cathode_array_solid", 0., 0.5*m, 0.5*m, 0., twopi);
  G4LogicalVolume *cathode_array_log = new G4LogicalVolume(cathode_array_solid, mat_vaccum, "cathode_array_log", nullptr, nullptr, nullptr);
  new G4PVPlacement(nullptr, G4ThreeVector(), cathode_array_log, "cathode_array_phys", world_log, false, 0, check_overlaps);

  //
  v_cathode_array.clear();
 
  for(int i=0;i<rings;i++){
    for(int j=0;j<sectors;j++){
      v_cathode_array.push_back(new CathodePixel(cathode_array_log));
    }
  }
 
  std::vector<CathodePixel*>::iterator it = v_cathode_array.begin();
  for(int i=0;i<rings;i++){
    for(int j=0;j<sectors;j++){
      (*it)->SetRingId(i);
      (*it)->SetSectorId(j);
      (*it)->SetId(180*i+j);
      (*it)->Construct();
      (*it)->Place(G4ThreeVector(0., 0., ppac_pos_z+ppac_thickness/2.+1.*mm));
      (*it)->SetName();
      it++;
    }
  }
}

//
G4VPhysicalVolume* DetectorConstruction::DefineVolumes()
{
  // define world
  G4Box *world_solid = new G4Box("World", 0.5*WorldSizeX, 0.5*WorldSizeY, 0.5*WorldSizeZ);
  world_log = new G4LogicalVolume(world_solid, mat_vaccum, "World");
  G4VPhysicalVolume *world_phys = new G4PVPlacement(0, G4ThreeVector(0.,0.,0.), world_log, "World", 0, false, 0, check_overlaps);

  // ppac
  GetPPACLog("PPAC");
  G4ThreeVector ppac_pos = G4ThreeVector(0, 0, ppac_pos_z);
  new G4PVPlacement(0, ppac_pos, ppac_log, "PPAC", world_log, false, 0, check_overlaps);

  // mylar face
  GetMylarFaceLog("MylarFace");
  G4ThreeVector mylar_face_pos = G4ThreeVector(0, 0, mylar_face_pos_z);
  new G4PVPlacement(0, mylar_face_pos, mylar_face_log, "MylarFace", world_log, false, 0, check_overlaps);

  // al face
  GetAlFaceLog("AlFace");
  G4ThreeVector al_face_pos = G4ThreeVector(0, 0, al_face_pos_z);
  new G4PVPlacement(0, al_face_pos, al_face_log, "AlFace", world_log, false, 0, check_overlaps);

  // cathode array
  BuildCathodeArray();

  // Example of User Limits
  // Below is an example of how to set tracking constraints in a given logical volume
  // Sets a max step length in the tracker region, with G4StepLimiter
  G4double max_step = 1. * um;
  step_limit = new G4UserLimits(max_step);
  // logic_target->SetUserLimits(step_limit);

  /// Set additional contraints on the track, with G4UserSpecialCuts
  /// G4double max_length = 2*TargetThickness, max_time = 0.1*ns, min_ekin = 10*MeV;
  /// trackerLV->SetUserLimits(new G4UserLimits(max_step, max_length, max_time, min_ekin));

  // Sensitive detectors
  /*
  G4SDManager* sd_manager = G4SDManager::GetSDMpointer();
  TargetSD *tar_sd = new TargetSD("TargetSD", "TargetHitCollection");
  logic_target->SetSensitiveDetector(tar_sd);
  sd_manager->AddNewDetector(tar_sd);
  */

  // Always return the physical world
  return world_phys;
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

