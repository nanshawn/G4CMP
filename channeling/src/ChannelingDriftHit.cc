/***********************************************************************\
 * This software is licensed under the terms of the GNU General Public *
 * License version 3 or later. See G4CMP/LICENSE for the full license. *
\***********************************************************************/

/// \file analysis/A01/src/ChannelingDriftHit.cc
/// \brief Implementation of the ChannelingDriftHit class
//
// $Id: d91158d67d0ef3940260066891ac8e405c6d00d8 $
// --------------------------------------------------------------
//
#include "ChannelingDriftHit.hh"
#include "G4ios.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4AttDefStore.hh"
#include "G4AttDef.hh"
#include "G4AttValue.hh"
#include "G4UIcommand.hh"
#include "G4UnitsTable.hh"
#include "G4VisAttributes.hh"
#include "G4LogicalVolume.hh"
#include "G4SystemOfUnits.hh"

G4Allocator<ChannelingDriftHit> ChannelingDriftHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

ChannelingDriftHit::ChannelingDriftHit()
{
    fLayerID = -1;
    fTime = 0.;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

ChannelingDriftHit::ChannelingDriftHit(G4int z)
{
    fLayerID = z;
    fTime = 0.;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

ChannelingDriftHit::~ChannelingDriftHit()
{
    ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

ChannelingDriftHit::ChannelingDriftHit(const ChannelingDriftHit &right): G4VHit(){
    fLayerID = right.fLayerID;
    fWorldPos = right.fWorldPos;
    fLocalPos = right.fLocalPos;
    fTime = right.fTime;
    fEnergy = right.fEnergy;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

const ChannelingDriftHit& ChannelingDriftHit::operator=(const ChannelingDriftHit &right)
{
    fLayerID = right.fLayerID;
    fWorldPos = right.fWorldPos;
    fLocalPos = right.fLocalPos;
    fTime = right.fTime;
    fEnergy = right.fEnergy;
    return *this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

int ChannelingDriftHit::operator==(const ChannelingDriftHit &/*right*/) const
{
    return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void ChannelingDriftHit::Draw()
{
    G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
    if(pVVisManager)
    {
        G4Circle circle(fWorldPos);
        circle.SetScreenSize(2);
        circle.SetFillStyle(G4Circle::filled);
        G4Colour colour(1.,1.,0.);
        G4VisAttributes attribs(colour);
        circle.SetVisAttributes(attribs);
        pVVisManager->Draw(circle);
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

const std::map<G4String,G4AttDef>* ChannelingDriftHit::GetAttDefs() const
{
    G4bool isNew;
    std::map<G4String,G4AttDef>* store = G4AttDefStore::GetInstance("ChannelingDriftHit",isNew);
    if (isNew) {
        G4String HitType("HitType");
        (*store)[HitType] = G4AttDef(HitType,"Hit Type","Physics","","G4String");
        
        G4String ID("ID");
        (*store)[ID] = G4AttDef(ID,"ID","Physics","","G4int");
        
        G4String Time("Time");
        (*store)[Time] = G4AttDef(Time,"Time","Physics","G4BestUnit","G4double");
        
        G4String Pos("Pos");
        (*store)[Pos] = G4AttDef(Pos, "Position","Physics","G4BestUnit","G4ThreeVector");

        G4String En("En");
        (*store)[En] = G4AttDef(En, "Energy","Physics","G4BestUnit","G4double");
    }
    return store;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

std::vector<G4AttValue>* ChannelingDriftHit::CreateAttValues() const
{
    std::vector<G4AttValue>* values = new std::vector<G4AttValue>;
    
    values->push_back(G4AttValue("HitType","DriftChamberHit",""));
    
    values->push_back(G4AttValue("ID",G4UIcommand::ConvertToString(fLayerID),""));
    
    values->push_back(G4AttValue("Time",G4BestUnit(fTime,"Time"),""));
    
    values->push_back(G4AttValue("Pos",G4BestUnit(fWorldPos,"Length"),""));
  
    values->push_back(G4AttValue("Time",G4BestUnit(fEnergy,"Time"),""));

    return values;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void ChannelingDriftHit::Print()
{
    G4cout << "  Layer[" << fLayerID << "] : time " << fTime/ns
    << " (nsec) --- local (x,y) " << fLocalPos.x()
    << ", " << fLocalPos.y() << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
