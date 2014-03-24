//
/// \file library/include/G4CMPProcessUtils.hh
/// \brief Definition of the G4CMPProcessUtils class
///   Provides useful general functions to fetch and store lattice, access
///   and apply lattice parameters for phonons and charge carriers.
///
///   Use via multiple inheritance with concrete or base process classes
//
// $Id$
//
// 20140321  Move lattice-based placement transformations here, via Touchable

#ifndef G4CMPProcessUtils_hh
#define G4CMPProcessUtils_hh 1

#include "globals.hh"
#include "G4AffineTransform.hh"
#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"

class G4LatticePhysical;
class G4Track;
class G4PhononTrackMap;
class G4CMPValleyTrackMap;
class G4VTouchable;


class G4CMPProcessUtils {
public:
  G4CMPProcessUtils();
  ~G4CMPProcessUtils();

  // Fetch lattice (if any) for volume occupied by track
  virtual void LoadDataForTrack(const G4Track* track);
  virtual void ReleaseTrack();
  // NOTE:  Subclasses may overload these, but be sure to callback to base

  // Convert global to local coordinates
  inline G4ThreeVector GetLocalDirection(const G4ThreeVector& dir) const {
    return fGlobalToLocal.TransformAxis(dir);
  }

  inline G4ThreeVector GetLocalPosition(const G4ThreeVector& pos) const {
    return fGlobalToLocal.TransformPoint(pos);
  }

  inline void RotateToLocalDirection(G4ThreeVector& dir) const {
    fGlobalToLocal.ApplyAxisTransform(dir);
  }

  inline void RotateToLocalPosition(G4ThreeVector& pos) const {
    fGlobalToLocal.ApplyPointTransform(pos);
  }

  // Convert local to global coordinates
  inline G4ThreeVector GetGlobalDirection(const G4ThreeVector& dir) const {
    return fLocalToGlobal.TransformAxis(dir);
  }

  inline G4ThreeVector GetGlobalPosition(const G4ThreeVector& pos) const {
    return fLocalToGlobal.TransformPoint(pos);
  }

  inline void RotateToGlobalDirection(G4ThreeVector& dir) const {
    fLocalToGlobal.ApplyAxisTransform(dir);
  }

  inline void RotateToGlobalPosition(G4ThreeVector& pos) const {
    fLocalToGlobal.ApplyPointTransform(pos);
  }

  // Convenience functions to get local position, momenum from track
  G4ThreeVector GetLocalPosition(const G4Track& track) const;
  G4ThreeVector GetLocalPosition(const G4Track* track) const {
    return GetLocalPosition(*track);
  }

  void GetLocalPosition(const G4Track& track, G4double pos[3]) const;
  void GetLocalPosition(const G4Track* track, G4double pos[3]) const {
    return GetLocalPosition(*track, pos);
  }

  G4ThreeVector GetLocalMomentum(const G4Track& track) const;
  G4ThreeVector GetLocalMomentum(const G4Track* track) const {
    return GetLocalMomentum(*track);
  }

  void GetLocalMomentum(const G4Track& track, G4double pos[3]) const;
  void GetLocalMomentum(const G4Track* track, G4double pos[3]) const {
    return GetLocalMomentum(*track, pos);
  }

  // Map phonon types to polarization index
  G4int GetPolarization(const G4Track& track) const;
  inline G4int GetPolarization(const G4Track* track) const {
    return GetPolarization(*track);
  }

  // Generate random phonon polarization from density of states
  // Values passed may be zero to suppress particular states
  G4int ChoosePolarization(G4double Ldos, G4double STdos, G4double FTdos) const;

  // Map charge carrier momentum to valley index
  G4int GetValleyIndex(const G4Track& track) const;
  inline G4int GetValleyIndex(const G4Track* track) const {
    return GetValleyIndex(*track);
  }

  const G4RotationMatrix& GetValley(const G4Track& track) const;
  inline const G4RotationMatrix& GetValley(const G4Track* track) const {
    return GetValley(*track); 
  }

  // Generate random valley for charge carrier
  G4int ChooseValley() const;

  // Construct new phonon track with correct momentum, position, etc.
  G4Track* CreatePhonon(G4int polarization, const G4ThreeVector& K,
			G4double energy) const;

  // Construct new electron or hole track with correct conditions
  G4Track* CreateChargeCarrier(G4int charge, G4int valley,
			       const G4ThreeVector& p, G4double energy) const;

protected:
  const G4LatticePhysical* theLattice;	// For convenient access by processes
  G4PhononTrackMap* trackKmap;
  G4CMPValleyTrackMap* trackVmap;

private:
  const G4Track* currentTrack;		// For use by Start/EndTracking

  G4AffineTransform fLocalToGlobal;	// For converting pos and momentum
  G4AffineTransform fGlobalToLocal;
  void SetTransforms(const G4VTouchable* touchable);

  // hide assignment operators as private 
  G4CMPProcessUtils(G4CMPProcessUtils&);
  G4CMPProcessUtils& operator=(const G4CMPProcessUtils& right);
};

#endif	/* G4CMPProcessUtils_hh */
