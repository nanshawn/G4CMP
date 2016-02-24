/***********************************************************************\
 * This software is licensed under the terms of the GNU General Public *
 * License version 3 or later. See G4CMP/LICENSE for the full license. *
\***********************************************************************/

#include "XVCrystalIntegratedDensity.hh"

XVCrystalIntegratedDensity::XVCrystalIntegratedDensity(){
    fNumberOfPoints = 512;
    fIntegrationPoints[0] = 32;
    fIntegrationPoints[1] = 32;
    fIntegrationPoints[2] = 32;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

XVCrystalIntegratedDensity::~XVCrystalIntegratedDensity(){
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void XVCrystalIntegratedDensity::SetIntegrationPoints(unsigned int vIndex,unsigned int vIntegrationPoints){
    if(vIndex<3) {
        if(vIntegrationPoints > 0){
            fIntegrationPoints[vIndex] = vIntegrationPoints;
        }
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

unsigned int XVCrystalIntegratedDensity::GetIntegrationPoints(unsigned int vIndex){
    if(vIndex<3) {
        return fIntegrationPoints[vIndex];
    }
    else{
        return 0;
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

unsigned int XVCrystalIntegratedDensity::GetIntegrationPoints(){
    return fIntegrationPoints[0]*fIntegrationPoints[1]*fIntegrationPoints[2];
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void XVCrystalIntegratedDensity::SetNumberOfPoints(unsigned int vNumberOfPoints){
    fNumberOfPoints = vNumberOfPoints;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

unsigned int XVCrystalIntegratedDensity::GetNumberOfPoints(){
    return fNumberOfPoints;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void XVCrystalIntegratedDensity::SetPotential(XVCrystalCharacteristic* vPotential){
    fPotential = vPotential;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

XVCrystalCharacteristic* XVCrystalIntegratedDensity::GetPotential(){
    return fPotential;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void XVCrystalIntegratedDensity::SetDensity(XVCrystalCharacteristic* vDensity){
    fDensity = vDensity;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

XVCrystalCharacteristic* XVCrystalIntegratedDensity::GetDensity(){
    return fDensity;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void XVCrystalIntegratedDensity::SetXPhysicalLattice(XPhysicalLattice* vLattice){
    fLattice = vLattice;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

XPhysicalLattice*  XVCrystalIntegratedDensity::GetXPhysicalLattice(){
    return fLattice;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void XVCrystalIntegratedDensity::SetParticleCharge(G4int vParticleCharge){
    fParticleCharge = vParticleCharge;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4int  XVCrystalIntegratedDensity::GetParticleCharge(){
    return fParticleCharge;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4double XVCrystalIntegratedDensity::GetStep(){
    return fPotentialRange / fNumberOfPoints;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4bool XVCrystalIntegratedDensity::HasBeenInitialized(XPhysicalLattice* vLattice, G4int vParticleCharge){
    //now it checks only of the table is initialized, it does not check if the particular crystal is initialized. To be changed in the future!
    if(fTableVector->GetVectorLength() == 0) return false;
    else if(vLattice!=fLattice) return false;
    else if(vParticleCharge!=fParticleCharge) return false;
    else return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void XVCrystalIntegratedDensity::ComputePotentialParameters(){
    fPotentialMinimum = fPotential->GetMinimum(fLattice);
    if(fParticleCharge < 0.){
        fPotentialMinimum = - fPotential->GetMaximum(fLattice);
    }
    
    fPotentialMaximum = fPotential->GetMaximum(fLattice);
    if(fParticleCharge < 0.){
        fPotentialMaximum = - fPotential->GetMinimum(fLattice);
    }
    
    fPotentialRange = fabs(fPotentialMaximum - fPotentialMinimum);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void XVCrystalIntegratedDensity::InitializeTable(){
    
    ComputePotentialParameters();
    
    G4cout << "XVCrystalIntegratedDensity::InitializeTable()::Potential Range =  " << fPotentialRange/CLHEP::eV << " - Minimum = " << fPotentialMinimum / CLHEP::eV << " - Maximum " << fPotentialMaximum / CLHEP::eV << std::endl;
    
    G4double vPotentialInitial = 0.;
    
    fTableVector = new G4PhysicsLinearVector(0.,fabs(fPotentialMaximum - fPotentialMinimum),GetNumberOfPoints());
    
    G4double vValue = 0;
    for(unsigned int i=0;i<GetNumberOfPoints();i++){
        vPotentialInitial = (fPotentialMinimum + fPotentialRange * G4double(i+1) / G4double(GetNumberOfPoints()));
        vValue = ComputeIntegratedDensity(vPotentialInitial,fParticleCharge);
        fTableVector->PutValue(i,vValue);
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4double XVCrystalIntegratedDensity::GetIntegratedDensity(G4double vPotential, XPhysicalLattice* vLattice, G4int vParticleCharge){
        
    G4double vPotentialModified = vPotential / fabs(G4double(vParticleCharge)) - fPotentialMinimum  * fabs(G4double(vParticleCharge));
    
    if(!HasBeenInitialized(vLattice,vParticleCharge)) return -1.;
    else if(vPotentialModified >= fabs(fPotentialMaximum - fPotentialMinimum)) return 1.;
    else if(vPotentialModified < 0.) return 1.;
    else{
        return fTableVector->Value(vPotentialModified);
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4double XVCrystalIntegratedDensity::ComputeIntegratedDensity(G4double vPotentialInitial, G4int){
    
    unsigned int i1,i2,i3;
    i1 = i2 = i3 = 0;
    
    G4ThreeVector vPositionTemp = G4ThreeVector(0.,0.,0.);
    G4double vDensity = 0.;
    
    G4ThreeVector vSize = fLattice->GetXUnitCell()->GetSize();
    while(i1<fIntegrationPoints[2]){
        vPositionTemp.setY(G4double(G4double(i3)/G4double(fIntegrationPoints[2])*vSize.z()));
        while(i1<fIntegrationPoints[1]){
            vPositionTemp.setZ(G4double(G4double(i2)/G4double(fIntegrationPoints[1])*vSize.y()));
            while(i1<fIntegrationPoints[0]){
                vPositionTemp.setX(G4double(G4double(i1)/G4double(fIntegrationPoints[0])*vSize.x()));
                if(fPotential->GetEC(vPositionTemp,fLattice).x() < vPotentialInitial){
                    vDensity += fDensity->GetEC(vPositionTemp,fLattice).x();
                }
                i1++;
            };
            i2++;
        };
        i3++;
    };
    
    vDensity *= fLattice->GetXUnitCell()->ComputeVolume();
    vDensity /= GetIntegrationPoints();
    
    return vDensity;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4double XVCrystalIntegratedDensity::FindCatmullRomInterpolate(G4double &p0,G4double &p1, G4double &p2,G4double &p3,G4double &x)
{
    double a0, a1 , a2 , a3 , x2;
    
    x2 = x * x;
    a0 = -0.5 * p0 + 1.5 * p1 - 1.5 * p2 + 0.5 * p3;
    a1 = p0 - 2.5 * p1 + 2.0 * p2 - 0.5 * p3;
    a2 = -0.5 * p0 + 0.5 * p2;
    a3 = p1;
    
    return (a0 * x * x2 + a1 * x2 + a2 * x + a3);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void XVCrystalIntegratedDensity::PrintOnFile(const G4String& filename){
    std::ofstream vFileOut;
    vFileOut.open(filename);
    G4double vStep = GetStep();
    
    vFileOut << "energy,dens" << std::endl;
    for(unsigned int i = 0;i < fNumberOfPoints;i++){
        vFileOut << i * vStep  / CLHEP::eV<< " , " << GetIntegratedDensity((i+1) * vStep  + fPotentialMinimum,fLattice,fParticleCharge) << std::endl;
    }
    vFileOut.close();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void XVCrystalIntegratedDensity::ReadFromFile(const G4String& filename){
    std::ifstream vFileIn;
    vFileIn.open(filename);
    
    fPotentialMinimum = +DBL_MAX;
    fPotentialMaximum = -DBL_MAX;
    G4double vDensity = 0.;
    
    vFileIn >> fPotentialMinimum >> fPotentialMaximum;
    
    std::vector<G4double> fTable;
    
    while(!vFileIn.eof()){
        vFileIn >> vDensity;
        fTable.push_back(vDensity);
    };
    
    fNumberOfPoints = fTable.size();
    
    fTableVector = new G4PhysicsLinearVector(0.,fPotentialMaximum - fPotentialMinimum,GetNumberOfPoints());
    for(unsigned int i=0;i<fTable.size();i++){
        fTableVector->PutValue(i,fTable.at(i));
    }
    
    fPotentialRange = fPotentialMaximum - fPotentialMinimum;
    
    vFileIn.close();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
