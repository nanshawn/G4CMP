/***********************************************************************\
 * This software is licensed under the terms of the GNU General Public *
 * License version 3 or later. See G4CMP/LICENSE for the full license. *
\***********************************************************************/

// $Id: dc2ebe3cfaab935ea4e308bb32bedcfafed16818 $

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef ChannelingDetectorConstruction_h
#define ChannelingDetectorConstruction_h 1
#endif

#include "G4VUserDetectorConstruction.hh"
#include "ChannelingDetectorMessenger.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4RunManager.hh"

#include "globals.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class ChannelingDetectorConstruction : public G4VUserDetectorConstruction
{
public:
    
    ChannelingDetectorConstruction();
    ~ChannelingDetectorConstruction();
    
    void DefineMaterials();
    
private:
    ChannelingDetectorMessenger* fMessenger;
    
private:
    void ConstructWorld();
    void ConstructSiliconStripDetectors();
    void ConstructScintillators();
    void ConstructRBSDetector();
    void ConstructXtalTarget();
    
public:
    void AddSiliconStripDetectors(G4String);
    
    void AddScintillators() {
        bSCI = true;
        G4RunManager::GetRunManager()->GeometryHasBeenModified();
    };
    void AddRBSDetector() {
        bRBS = true;
        G4RunManager::GetRunManager()->GeometryHasBeenModified();
    };
    void AddXtalTarget() {
        bXtal = true;
        G4RunManager::GetRunManager()->GeometryHasBeenModified();
    };
    
public:
    void SetWorldMaterial(const G4String& name);
    G4String GetWorldMaterial();

private:
    G4double fWorldSizeXZ;
    G4double fWorldSizeY;
    G4Box* fWorldSolid;
    G4LogicalVolume* fWorldLogic;
    G4VPhysicalVolume* fWorldPhysical;
    G4Material* fWorldMaterial;

public:
    void SetSSDSize(G4ThreeVector);
    G4ThreeVector GetSSDSize() {return fSSDSize;};
    void SetSSD0XtalDistance(G4double);
    G4double GetSSD0XtalDistance() {return fSSD0XtalDistance;};
    void SetSSD1XtalDistance(G4double);
    G4double GetSSD1XtalDistance() {return fSSD1XtalDistance;};
    void SetSSD2XtalDistance(G4double);
    G4double GetSSD2XtalDistance() {return fSSD2XtalDistance;};
    
private:
    G4bool bSSD1;
    G4bool bSSD2;
    G4bool bSSD3;
    
    G4double fSSD0XtalDistance;
    G4double fSSD1XtalDistance;
    G4double fSSD2XtalDistance;
    G4ThreeVector fSSDSize;
    G4Box* fSSDSolid;
    G4LogicalVolume* fSSDLogic;
    G4VPhysicalVolume* fSSDPhysical;
    
    
public:
    void SetSCISize(G4ThreeVector);
    G4ThreeVector GetSCISize() {return fSCISize;};
    void SetSCIRelativeDistance(G4double);
    G4double GetSCIRelativeDistance() {return fSCIRelativeDistance;};
    void SetSCIXtalDistance(G4double);
    G4double GetSCIXtalDistance() {return fSCIXtalDistance;};
    
private:
    G4bool bSCI;
    
    G4double fSCIXtalDistance;
    G4double fSCIRelativeDistance;
    G4ThreeVector fSCISize;
    G4Box* fSCISolid;
    G4LogicalVolume* fSCILogic;
    G4VPhysicalVolume* fSCIPhysical;
    
public:
    void SetRBSDistanceR(G4double);
    G4double GetRBSDistanceR() {return fRBSDistanceR;};
    void SetRBSAngleTheta(G4double);
    G4double GetRBSAngleTheta() {return fRBSAngleTheta;};
    void SetRBSAnglePhi(G4double);
    G4double GetRBSAnglePhi() {return fRBSAnglePhi;};
    
private:
    G4bool bRBS;
    
    G4double fRBSDistanceR;
    G4double fRBSAngleTheta;
    G4double fRBSAnglePhi;
    
    G4double fRBSSizeZ;
    G4double fRBSSizeR;
    G4Tubs* fRBSSolid;
    G4LogicalVolume* fRBSLogic;
    G4VPhysicalVolume* fRBSPhysical;
    
public:
    void SetXtalMaterial(const G4String& name);
    G4String GetXtalMaterial();
    void SetXtalCurvatureRadius(G4ThreeVector);
    G4ThreeVector GetXtalCurvatureRadius() {return fXtalCurvatureRadius;};
    void SetXtalSize(G4ThreeVector);
    G4ThreeVector GetXtalSize() {return fXtalSize;};
    void SetXtalAngle(G4ThreeVector); //planar case only set.z(); for axial also set.x()
    G4ThreeVector GetXtalAngle() {return fXtalAngle;};
    void SetXtalCellSize(G4ThreeVector);
    G4ThreeVector GetXtalCellSize() {return fXtalCellSize;};
    void SetXtalCellAngle(G4ThreeVector); //planar case only set.z(); for axial also set.x()
    G4ThreeVector GetXtalCellAngle() {return fXtalCellAngle;};
    void SetXtalThermalVibrationAmplitude(G4double);
    G4double GetXtalThermalVibrationAmplitude() {return fXtalThermalVibrationAmplitude;};
    
private:
    G4bool bXtal;
    
    G4ThreeVector fXtalCurvatureRadius;
    
    G4Material* fXtalMaterial;
    G4ThreeVector fXtalAngle;
    G4ThreeVector fXtalSize;
    G4ThreeVector fXtalCellSize;
    G4ThreeVector fXtalCellAngle;
    G4double fXtalThermalVibrationAmplitude;
    
    G4VSolid* fXtalSolid;
    G4LogicalVolume* fXtalLogic;
    G4VPhysicalVolume* fXtalPhysical;
    
public:
    G4VPhysicalVolume* Construct();
};


