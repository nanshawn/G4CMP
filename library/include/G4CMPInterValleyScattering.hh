/***********************************************************************\
 * This software is licensed under the terms of the GNU General Public *
 * License version 3 or later. See G4CMP/LICENSE for the full license. *
\***********************************************************************/

// $Id$
//
// 20140324  Drop hard-coded IV scattering parameters; get from lattice
// 20140418  Drop valley transforms; get from lattice
// 20170802  Remove MFP calculation; use scattering-rate model

#ifndef G4CMPInterValleyScattering_h
#define G4CMPInterValleyScattering_h 1

#include "globals.hh"
#include "G4CMPVDriftProcess.hh"


class G4CMPInterValleyScattering : public G4CMPVDriftProcess { 
public:
  G4CMPInterValleyScattering();
  virtual ~G4CMPInterValleyScattering();

  virtual G4VParticleChange* PostStepDoIt(const G4Track&, const G4Step&);

  // Only electrons have physical valleys associated with them
  virtual bool IsApplicable(const G4ParticleDefinition&);

private:
  //hide assignment operator as private
  G4CMPInterValleyScattering(G4CMPInterValleyScattering&);
  G4CMPInterValleyScattering& operator=(const G4CMPInterValleyScattering& right);
};

#endif
