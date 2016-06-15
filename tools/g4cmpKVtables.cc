//
//  g4cmpKVtables -- Generate wavevector mapping tables for G4CMP
//
//  Uses G4CMPPhononKVgMap, written by Daniel Palken 2014.
//

#include "G4CMPPhononKVgMap.hh"
#include "G4ThreeVector.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4LatticeManager.hh"
#include "G4LatticeLogical.hh"
#include "G4SystemOfUnits.hh"
#include <fstream>
#include <iomanip>
#include <string>
#include <math.h>
using namespace std;


// Get lattice configuration for density and elasticity matrix
G4LatticeLogical* GetLattice(const G4String& name) {
  G4Material* mat = G4NistManager::Instance()->FindOrBuildMaterial("G4_"+name);
  return G4LatticeManager::Instance()->LoadLattice(mat, name);
}

int main(int argc, const char * argv[])
{
  // 1. choose material
  G4LatticeLogical* lattice = GetLattice((argc>1) ? argv[1] : "Ge");
  if (!lattice) {
    cerr << argv[0] << " Invalid material " << argv[1] << " specified" << endl;
    ::exit(1);
  }

  // 2. set up the entire interpolation infrastrcuture
  G4CMPPhononKVgMap *map = new G4CMPPhononKVgMap(lattice);
  
  map->writeLookupTable();		// Dump Dan's version of data file
  
  // 3. G4CMP uses (theta,phi) binning
  const int Ntheta = 161;		// These must match config.txt values
  const int Nphi = 321;
  
  // 4. Loop over modes (L, ST, FT) to set up output files
  for (int mode=G4CMPPhononKVgMap::L; mode<G4CMPPhononKVgMap::NUM_MODES; mode++) {
    string vgname = map->getModeName(mode) + ".ssv";
    ofstream vgfile(vgname, ios::trunc);
    vgfile << scientific << setprecision(7);
    
    string vdirname = map->getModeName(mode) + "Vec.ssv";
    ofstream vdirfile(vdirname, ios::trunc);
    vdirfile << scientific << setprecision(7);
    
    cout << "Generating " << lattice->GetName() << " "
	 << map->getModeName(mode) << " files " << Ntheta << " x " << Nphi
	 << endl;
    
    G4ThreeVector kvec, Vg;
    double theta, phi;
    for (int itheta = 0; itheta<Ntheta; itheta++) {
      cout << "." << flush;

      theta = M_PI*itheta / Ntheta;		// Bin edges

      for (int iphi = 0; iphi<Nphi; iphi++) {
	phi = 2.*M_PI*iphi / Nphi;		// Bin edges

	// Convert phi, theta bin center to unit vector
	kvec.setRThetaPhi(1.,theta,phi);
	
	// 5. Get group velocity for given direction, write to file
	Vg = map->getGroupVelocity(mode, kvec) / (m/s);
	
	vgfile << setw(16) << Vg.mag() << endl;
	
	vdirfile << setw(16) << Vg.x() << setw(16) << Vg.y()
		 << setw(16) << Vg.z() << endl;
      }	// phi
    }	// theta
    cout << endl;
  }		// mode
}