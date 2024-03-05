#include "CathodePixel.hh"

//
CathodePixel::CathodePixel(G4LogicalVolume *l)
{
  log = l;

  G4NistManager* nist_manager = G4NistManager::Instance();
  mat_cu = nist_manager->FindOrBuildMaterial("G4_Cu");

  thickness = CathodeThickness;
  target2cathode = Target2Cathode;

  ring_id = 0;
  sector_id = 0;
  id = 0;
}

//
CathodePixel::~CathodePixel()
{

}

//
G4VPhysicalVolume *CathodePixel::Construct()
{
  G4double theta = (22.+(G4double)ring_id) *deg;
  G4double phi = 2.*(G4double)sector_id *deg;
  inner_radius = target2cathode*std::tan(theta);
  outer_radius = target2cathode*std::tan(theta+1.*deg);

  //
  cathode_pixel_solid = new G4Tubs("cathode_pixel_solid", inner_radius, outer_radius, thickness/2., phi, 2.*deg);
  cathode_pixel_log = new G4LogicalVolume(cathode_pixel_solid, mat_cu, "cathode_pixel_log");
  cathode_pixel_phys = new G4PVPlacement(nullptr, G4ThreeVector(), cathode_pixel_log, "cathde_pixel_phy", log, true, id, true);

  // color
  G4VisAttributes *vis_att = new G4VisAttributes(G4Colour(1.0, 1.0, 1.0, 0.3));
  vis_att->SetForceSolid(true);
  cathode_pixel_log->SetVisAttributes(vis_att);

  return cathode_pixel_phys;
}

//
void CathodePixel::Report()
{
  G4cout << "----> build a Cu Cathode pixel." << G4endl;
  G4cout << "----> Cathode detector material set to " << cathode_pixel_log->GetMaterial()->GetName() << G4endl;
}

//
void CathodePixel::Place(G4ThreeVector p)
{
  cathode_pixel_phys->SetTranslation(p);

  G4RunManager::GetRunManager()->GeometryHasBeenModified();
}

//
void CathodePixel::SetName()
{
  char name[128];
  sprintf(name, "cathode_id%04d_ring%04d_sector%04d", id, ring_id, sector_id);
  cathode_pixel_phys->SetName(name);
}

