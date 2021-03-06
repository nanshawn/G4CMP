/***********************************************************************\
 * This software is licensed under the terms of the GNU General Public *
 * License version 3 or later. See G4CMP/LICENSE for the full license. *
\***********************************************************************/

/// \file library/include/G4CMPIVRateEdelweiss.hh
/// \brief Compute electron intervalley scattering rate using Edelweiss
///	   parametrization vs. electric field.
//
// $Id$
//
// 20170815  Move G4CMPProcessUtils inheritance to base class

#ifndef G4CMPIVRateEdelweiss_hh
#define G4CMPIVRateEdelweiss_hh 1

#include "G4CMPVScatteringRate.hh"


class G4CMPIVRateEdelweiss : public G4CMPVScatteringRate {
public:
  G4CMPIVRateEdelweiss() : G4CMPVScatteringRate("IVEdelweiss") {;}
  virtual ~G4CMPIVRateEdelweiss() {;}

  virtual G4double Rate(const G4Track& aTrack) const;
};

#endif	/* G4CMPIVRateEdelweiss_hh */
