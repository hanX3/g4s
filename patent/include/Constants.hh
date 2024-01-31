#ifndef Constants_h
#define Constants_h 1

#include "globals.hh"
#include "G4SystemOfUnits.hh"

#define DATAPATH "./data"  // no "/"

namespace example
{
// 0b0001 event data
// 0b0010 track data
// 0b0100 step data
constexpr G4int MASK = 0b001;

constexpr G4double WorldSizeX = 0.2 * m;
constexpr G4double WorldSizeY = 0.2 * m;
constexpr G4double WorldSizeZ = 0.2 * m;

// target
constexpr G4double TargetR = 25. * mm;
constexpr G4double TargetThickness = 5. * mm;
constexpr G4double PosTarX = 0. * mm;
constexpr G4double PosTarY = 0. * mm;
constexpr G4double PosTarZ = 0. * mm;

// au layer
constexpr G4double AuLayerR = 25. * mm;
constexpr G4double AuLayerThickness = 0.5 * um;
constexpr G4double PosAuLayerX = 0. * mm;
constexpr G4double PosAuLayerY = 0. * mm;
constexpr G4double PosAuLayerZ = -(TargetThickness+AuLayerThickness) * mm;

//beam
constexpr G4double PosBeamX = 0. * mm;
constexpr G4double PosBeamY = 0. * mm;
constexpr G4double PosBeamZ = -10. * mm;
constexpr G4double BeamSigma = 1. * MeV;
//step limit

}
#endif
