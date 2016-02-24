/***********************************************************************\
 * This software is licensed under the terms of the GNU General Public *
 * License version 3 or later. See G4CMP/LICENSE for the full license. *
\***********************************************************************/

/// \file analysis/A01/src/ChannelingPrimaryGeneratorMessenger.cc
/// \brief Implementation of the ChannelingPrimaryGeneratorMessenger class
//
// $Id: 7f116fa0d1b13ca65b7517209f01a8bed38e715b $
// --------------------------------------------------------------
//

#include "ChannelingPrimaryGeneratorMessenger.hh"
#include "ChannelingPrimaryGeneratorAction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4ios.hh"

ChannelingPrimaryGeneratorMessenger::ChannelingPrimaryGeneratorMessenger(ChannelingPrimaryGeneratorAction * mpga)
:fTarget (mpga)
{
    fDivergenceDistribution = new G4UIcmdWithAString("/gun/setDivDistr",this);
    fDivergenceDistribution->SetGuidance("Set beam divergence distribution.");
    fDivergenceDistribution->SetParameterName("beamdivdistr",true);
    fDivergenceDistribution->SetDefaultValue("");
    
    fCutX = new G4UIcmdWithADoubleAndUnit("/gun/setCutX",this);
    fCutX->SetGuidance("Set beam cut Y.");
    fCutX->SetParameterName("beamcutx",true);
    fCutX->SetDefaultValue(0.);
    fCutX->SetDefaultUnit("rad");
    
    fCutY = new G4UIcmdWithADoubleAndUnit("/gun/setCutY",this);
    fCutY->SetGuidance("Set beam cut X.");
    fCutY->SetParameterName("beamcuty",true);
    fCutY->SetDefaultValue(0.);
    fCutY->SetDefaultUnit("rad");

    fDivergenceX = new G4UIcmdWithADoubleAndUnit("/gun/setDivX",this);
    fDivergenceX->SetGuidance("Set beam divergence Y.");
    fDivergenceX->SetParameterName("beamdivx",true);
    fDivergenceX->SetDefaultValue(0.);
    fDivergenceX->SetDefaultUnit("rad");

    fDivergenceY = new G4UIcmdWithADoubleAndUnit("/gun/setDivY",this);
    fDivergenceY->SetGuidance("Set beam divergence X.");
    fDivergenceY->SetParameterName("beamdivy",true);
    fDivergenceY->SetDefaultValue(0.);
    fDivergenceY->SetDefaultUnit("rad");

    fWidthX = new G4UIcmdWithADoubleAndUnit("/gun/setWidthX",this);
    fWidthX->SetGuidance("Set beam width X.");
    fWidthX->SetParameterName("beamwidthx",true);
    fWidthX->SetDefaultValue(0.);
    fWidthX->SetDefaultUnit("mm");

    fWidthY = new G4UIcmdWithADoubleAndUnit("/gun/setWidthY",this);
    fWidthY->SetGuidance("Set beam width Y.");
    fWidthY->SetParameterName("beamwidthy",true);
    fWidthY->SetDefaultValue(0.);
    fWidthY->SetDefaultUnit("mm");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

ChannelingPrimaryGeneratorMessenger::~ChannelingPrimaryGeneratorMessenger()
{
    delete fDivergenceDistribution;
    delete fCutX;
    delete fCutY;
    delete fDivergenceX;
    delete fDivergenceY;
    delete fWidthX;
    delete fWidthY;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void ChannelingPrimaryGeneratorMessenger::SetNewValue(G4UIcommand * command,G4String newValue)
{
    if(command==fDivergenceDistribution ){
        fTarget->SetBeamDivergencDistribution(newValue);
    }
    if(command==fCutX ){
        fTarget->SetBeamCutX(fCutX->GetNewDoubleValue(newValue));
    }
    if(command==fCutY ){
        fTarget->SetBeamCutX(fCutY->GetNewDoubleValue(newValue));
    }
    if(command==fDivergenceX ){
        fTarget->SetBeamDivergenceX(fDivergenceX->GetNewDoubleValue(newValue));
    }
    if(command==fDivergenceY ){
        fTarget->SetBeamDivergenceY(fDivergenceY->GetNewDoubleValue(newValue));
    }
    if(command==fWidthX ){
        fTarget->SetBeamWidthX(fWidthX->GetNewDoubleValue(newValue));
    }
    if(command==fWidthY ){
        fTarget->SetBeamWidthY(fWidthY->GetNewDoubleValue(newValue));
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4String ChannelingPrimaryGeneratorMessenger::GetCurrentValue(G4UIcommand * command)
{
    G4String cv;
    if( command==fDivergenceDistribution ){
        cv = fTarget->GetBeamDivergencDistribution();
    }
    if( command==fCutX ){
        cv = fCutX->ConvertToString(fTarget->GetBeamCutX(),"rad");
    }
    if( command==fCutY ){
        cv = fCutY->ConvertToString(fTarget->GetBeamCutY(),"rad");
    }
    if( command==fDivergenceX ){
        cv = fDivergenceX->ConvertToString(fTarget->GetBeamDivergenceX(),"rad");
    }
    if( command==fDivergenceY ){
        cv = fDivergenceY->ConvertToString(fTarget->GetBeamDivergenceY(),"rad");
    }
    if( command==fWidthX ){
        cv = fWidthX->ConvertToString(fTarget->GetBeamWidthX(),"rad");
    }
    if( command==fWidthY ){
        cv = fWidthY->ConvertToString(fTarget->GetBeamWidthY(),"rad");
    }
    
    return cv;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
