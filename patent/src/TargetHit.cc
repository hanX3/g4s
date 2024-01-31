#include "TargetHit.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

#include <iomanip>

namespace example
{

G4ThreadLocal G4Allocator<TargetHit>* TargetHitAllocator = nullptr;

//
G4bool TargetHit::operator==(const TargetHit& right) const
{
  return (this == &right)?true:false;
}

//
void TargetHit::Draw()
{
  G4VVisManager* vis_manager = G4VVisManager::GetConcreteInstance();
  if(vis_manager)
  {
    G4Circle circle(pos);
    circle.SetScreenSize(4.);
    circle.SetFillStyle(G4Circle::filled);
    G4Colour colour(1.,0.,0.);
    G4VisAttributes attribs(colour);
    circle.SetVisAttributes(attribs);
    vis_manager->Draw(circle);
  }
}

//
void TargetHit::Print()
{
  G4cout
     << "  track id: " << track_id << "energy dep: " << std::setw(7) << G4BestUnit(e_dep, "Energy")
     << " Position: " << std::setw(7) << G4BestUnit(pos, "Length") << G4endl;
}

//
}
