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
/// \file library/src/G4LukeScattering.cc
/// \brief Implementation of the G4LukeScattering class
//
// $Id$
//

#include "G4CMPhLukeScattering.hh"
#include "G4CMPDriftHole.hh"

#include "G4LatticeManager.hh"
#include "G4LatticePhysical.hh"

#include "G4Step.hh"
#include "G4StepPoint.hh"
#include "G4VParticleChange.hh"
#include "Randomize.hh"
#include "G4RandomDirection.hh"

#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4FieldManager.hh"
#include "G4Field.hh"

#include "G4Geantino.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

#include "math.h"


G4CMPhLukeScattering::G4CMPhLukeScattering(G4VProcess* stepper)
:G4VPhononProcess("hLukeScattering")
{
    velLong=5400*m/s;
    l0Hole = 108e-6*m;
    massFreeElectron=electron_mass_c2/c_squared;
    massHole=0.35*massFreeElectron;
    //ksound_Hole=1.6306e7/m;
    ksound_Hole=velLong*massHole/hbar_Planck;
    stepLimiter = stepper;

    if(verboseLevel>1){
	G4cout<<GetProcessName()<<" is created "<<G4endl;
      }
}

G4CMPhLukeScattering::~G4CMPhLukeScattering()
{ ; }


G4double G4CMPhLukeScattering::GetMeanFreePath(const G4Track& aTrack, G4double, G4ForceCondition* condition)
{
    G4StepPoint* stepPoint  = aTrack.GetStep()->GetPostStepPoint();
    G4double velocity = stepPoint->GetVelocity();
    G4double kmag = velocity*massHole / hbar_Planck;

    *condition = Forced;

    if (kmag<=ksound_Hole)
    {
	//G4cout <<  "G4CMPhLukeScattering: DBL_MAX" <<  G4endl;
	return DBL_MAX;
    }

    G4double mfp= velocity / ( velLong / (3*l0Hole)
			    * (kmag / ksound_Hole)*(kmag / ksound_Hole)
			    * ((1- ksound_Hole /kmag))
			    * ((1- ksound_Hole /kmag))
			    * ((1- ksound_Hole /kmag)));


    //G4cout <<  "G4CMPhLukeScattering: " <<  mfp <<  G4endl;
    return mfp;

}

G4VParticleChange* G4CMPhLukeScattering::PostStepDoIt(const G4Track& aTrack, const G4Step& aStep)
{

    aParticleChange.Initialize(aTrack);  

    //Do nothing other than re-calculate mfp when step limit reached or leaving volume
    G4StepPoint* postStepPoint = aStep.GetPostStepPoint();
    //G4cout<<"\nLukeScattering::PostStepDoIt: Step limited by process " <<postStepPoint->GetProcessDefinedStep()->GetProcessName() <<  G4endl;


    if((postStepPoint->GetProcessDefinedStep()==stepLimiter)
	|| (postStepPoint->GetStepStatus()==fGeomBoundary))
	return &aParticleChange;
	//return G4VDiscreteProcess::PostStepDoIt(aTrack,aStep);


    G4double velocity = postStepPoint->GetVelocity();
    //G4double p = postStepPoint->GetMomentum().mag()/c_light;
    //G4cout << "momentum: " <<  p <<  " " << velocity*massHole << G4endl;
    G4double kmag = velocity*massHole / hbar_Planck;
    G4double theta_phonon=MakeTheta(kmag, ksound_Hole);
    G4double theta_charge=
      acos((kmag*kmag - 2*ksound_Hole
	    *(kmag*cos(theta_phonon) - ksound_Hole) 
	    - 2 * (kmag*cos(theta_phonon) - ksound_Hole)
	    * (kmag*cos(theta_phonon) - ksound_Hole)) 
	   / kmag/ (sqrt(kmag*kmag - 4*ksound_Hole
			 *(kmag*cos(theta_phonon) - ksound_Hole))));

    G4double q = 2*(kmag*cos(theta_phonon)-ksound_Hole);
    G4double T = hbar_Planck*hbar_Planck*kmag*kmag/2/massHole;
    G4double qEnergy = velLong*hbar_Planck*q;
    //G4double knew = sqrt(kmag*kmag + q*q - kmag*q*cos(theta_phonon));

    G4ThreeVector momentum = G4ThreeVector(aTrack.GetMomentumDirection());
    G4ThreeVector orth = momentum.orthogonal();
    G4ThreeVector newDir = momentum.rotate(orth, theta_charge);
    newDir.rotate(aTrack.GetMomentumDirection(), G4UniformRand()*2*pi);

    aParticleChange.ProposeMomentumDirection(newDir);
    aParticleChange.ProposeEnergy(T-qEnergy);  
    ResetNumberOfInteractionLengthLeft();    
  
    return &aParticleChange;

}

G4double G4CMPhLukeScattering::MakeTheta(G4double& k, G4double& ks){
    //G4double u = G4UniformRand();

//Analytical method to compute theta

/*

    double base = -(u-1)+3*(u-1)*(ks/k)-3*(u-1)*(ks/k)*(ks/k)+(u-1)*(ks/k)*(ks/k)*(ks/k);
    double exponent =1.0/3.0;
    G4double operand = ks/k+pow(base, exponent);   

    if(operand>1.0) {
	// G4cout<<"\nTruncating operand from"<<operand<<" to 1.0";
	operand=1.0;
      }
    if(operand<0.0) G4cout<<"\noperand out of range: operand = "<<operand;
    G4double theta=acos(operand);
    //  G4cout<<"\n"<<theta;

    if(acos(ks/k)<theta) G4cout<<"\n  THETA OUT OF BOUNDS!!! (theta>acos(ks/k)) theta:"<<theta<<" acos(ks/k):"<<acos(ks/k);
    if(pi/2<=theta) G4cout<<"\n THETA OUT OF BOUNDS!!! (pi/2 < theta)";

 */


    //Rejection method for determining theta

    G4double theta;
    G4double pValue=2;
    G4double pDensity = 1;

    G4double thetaMax;
    G4double pValMax;
    thetaMax=acos(ks/k);
    pValMax=(1-(ks/k))*(1-(ks/k));

    //bool first=false;

    while(pValue>pDensity){
	theta=G4UniformRand()*thetaMax;
	pValue=G4UniformRand();        // *pValMax;//  *(1+2*ks/k+(ks/k)*(ks/k));   
	//need to multiply by unit 's' to make it dimensionless
	pDensity = (cos(theta)-(ks/k))*(cos(theta)-(ks/k))*sin(theta);
	if(pDensity>pValMax) G4cout<<"\nLukeScattering::PostStepDoIt: Error: pDensity should never exceed pValMax "<<pValMax;


      //    if(!first){
	//G4cout<<"\nG4CMPhLukeScattering::MakeTheta: pDensity calculated as: "<<pDensity;
	// G4cout<<"\n\tpValue: "<<pValue;
	// G4cout<<"\n\ttheta: "<<theta/rad;
	// G4cout<<"\n\tk: "<<k*m;
	// G4cout<<"\n\tks: "<<ks*m;
	// G4cout<<endl;
	//}
      //first=true;

      }

    return theta;
  }

G4double G4CMPhLukeScattering::MakePhi(G4double& k,G4double& ks, G4double& theta){


    G4double phi=acos((k*k - 2*ks*(k*cos(theta)-ks)-2*(k*cos(theta)-ks)*(k*cos(theta)-ks))/(k*sqrt(k*k-4*ks*(k*cos(theta)-ks))));

    return phi;
  }

G4bool G4CMPhLukeScattering::IsApplicable(const G4ParticleDefinition& aPD)
{
    return(&aPD==G4CMPDriftHole::Definition() );
}