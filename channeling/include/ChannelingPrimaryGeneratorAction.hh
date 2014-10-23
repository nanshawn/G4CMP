//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
// $Id$
// GEANT4 tag $Name: not supported by cvs2svn $
//
//

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef ChannelingPrimaryGeneratorAction_h
#define ChannelingPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "ChannelingPrimaryGeneratorMessenger.hh"
#include "globals.hh"

class G4ParticleGun;
class G4Event;
class ChannelingDetectorConstruction;
class PrimaryGeneratorMessenger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class ChannelingPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
    ChannelingPrimaryGeneratorAction();
    virtual ~ChannelingPrimaryGeneratorAction();
    
    void GeneratePrimaries(G4Event*);

    void SetBeamDivergencDistribution(G4String div) {fDivDistribution = div;};
    G4String GetBeamDivergencDistribution() {return fDivDistribution;};

    void SetBeamDivergenceX(G4double div) {fDivX = div;};
    G4double GetBeamDivergenceX() {return fDivX;};

    void SetBeamDivergenceY(G4double div) {fDivY=div;};
    G4double GetBeamDivergenceY() {return fDivY;};

    void SetBeamCutX(G4double div) {fCutX = div;};
    G4double GetBeamCutX() {return fCutX;};
    
    void SetBeamCutY(G4double div) {fCutY = div;};
    G4double GetBeamCutY() {return fCutY;};

    void SetBeamWidthX(G4double width) {fWidthX = width;};
    G4double GetBeamWidthX() {return fWidthX;};
    
    void SetBeamWidthY(G4double width) {fWidthY=width;};
    G4double GetBeamWidthY() {return fWidthY;};

private:
    G4ParticleGun* fParticleGun;	 //pointer a to G4  class
    
    ChannelingPrimaryGeneratorMessenger *fMessenger;

    
    G4String fDivDistribution;
    G4double fCutX;
    G4double fCutY;
    G4double fDivX;
    G4double fDivY;
    G4double fWidthX;
    G4double fWidthY;
    
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

