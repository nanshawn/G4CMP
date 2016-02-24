/***********************************************************************\
 * This software is licensed under the terms of the GNU General Public *
 * License version 3 or later. See G4CMP/LICENSE for the full license. *
\***********************************************************************/

// $Id$
// File:  G4CMPConfigMessenger.hh
//
// Description:	Macro command defitions to set user configuration in
//		G4CMPConfigManager.
//
// 20140904  Michael Kelsey
// 20141029  Add command to set output e/h positions file
// 20150106  Add command to toggle generate Luke phonons
// 20150122  Add command to rescale Epot file voltage by some factor
// 20150603  Add command to limit reflections in DriftBoundaryProcess

#include "G4CMPConfigMessenger.hh"
#include "G4CMPConfigManager.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcommand.hh"
#include "G4UIcommandTree.hh"
#include "G4UImanager.hh"


// Constructor and destructor

G4CMPConfigMessenger::G4CMPConfigMessenger(G4CMPConfigManager* mgr)
  : theManager(mgr), localCmdDir(false), cmdDir(0), verboseCmd(0),
    bounceCmd(0), voltageCmd(0), escaleCmd(0), fileCmd(0), dirCmd(0),
    hitsCmd(0) {
  CreateDirectory("/g4cmp/",
		  "User configuration for G4CMP phonon/charge carrier library");

  verboseCmd = CreateCommand<G4UIcmdWithAnInteger>("verbose",
					   "Enable diagnostic messages");

  voltageCmd = CreateCommand<G4UIcmdWithADoubleAndUnit>("voltage",
				"Set voltage for uniform electric field");
  voltageCmd->SetUnitCategory("Electric potential");

  minstepCmd = CreateCommand<G4UIcmdWithADouble>("minimumStep",
			 "Set fraction of L0 for charge carrier minimum step");

  makePhononCmd = CreateCommand<G4UIcmdWithADouble>("producePhonons",
          "Set rate of production of Luke and relaxation phonons");

  escaleCmd = CreateCommand<G4UIcmdWithADouble>("scaleEpot",
		"Set a scale factor for voltages in Epot electric field file");

  bounceCmd = CreateCommand<G4UIcmdWithAnInteger>("chargeBounces",
		  "Maximum number of reflections allowed for charge carriers");

  fileCmd = CreateCommand<G4UIcmdWithAString>("EpotFile",
			      "Set filename for non-uniform electric field");

  dirCmd = CreateCommand<G4UIcmdWithAString>("LatticeData",
			     "Set directory for lattice configuration files");
  dirCmd->AvailableForStates(G4State_PreInit);

  hitsCmd = CreateCommand<G4UIcmdWithAString>("HitsFile",
			      "Set filename for output of e/h hit locations");
}


G4CMPConfigMessenger::~G4CMPConfigMessenger() {
  delete verboseCmd; verboseCmd=0;
  delete bounceCmd; bounceCmd=0;
  delete voltageCmd; voltageCmd=0;
  delete minstepCmd; minstepCmd=0;
  delete makePhononCmd; makePhononCmd=0;
  delete escaleCmd; escaleCmd=0;
  delete fileCmd; fileCmd=0;
  delete dirCmd; dirCmd=0;
  delete hitsCmd; hitsCmd=0;

  if (localCmdDir) delete cmdDir; cmdDir=0;
}


// Create or access directory path common to all commands

void G4CMPConfigMessenger::CreateDirectory(const char* path, const char* desc) {
  G4UImanager* UIman = G4UImanager::GetUIpointer();
  if (!UIman) return;

  // Prepend /CDMS/ if user specified "relative path" (e.g., module name)
  G4String fullPath = path;
  if (fullPath(0) != '/') fullPath.prepend("/g4cmp/");
  if (fullPath(fullPath.length()-1) != '/') fullPath.append("/");

  // See if input path has already been registered
  G4UIcommand* foundPath = UIman->GetTree()->FindPath(fullPath);
  if (foundPath) cmdDir = dynamic_cast<G4UIdirectory*>(foundPath);

  if (!cmdDir) {		// Create local deletable directory
    localCmdDir = true;
    cmdDir = new G4UIdirectory(fullPath);
    cmdDir->SetGuidance(desc);
  }
}


// Parse user input and add to configuration

void G4CMPConfigMessenger::SetNewValue(G4UIcommand* cmd, G4String value) {
  if (cmd == verboseCmd) theManager->SetVerboseLevel(StoI(value));
  if (cmd == voltageCmd) theManager->SetVoltage(voltageCmd->GetNewDoubleValue(value));
  if (cmd == minstepCmd) theManager->SetMinStepScale(StoD(value));
  if (cmd == makePhononCmd) theManager->SetGenPhonons(StoD(value));
  if (cmd == escaleCmd) theManager->SetEpotScale(escaleCmd->GetNewDoubleValue(value));
  if (cmd == bounceCmd) theManager->SetMaxChargeBounces(StoI(value));
  if (cmd == fileCmd) theManager->SetEpotFile(value);
  if (cmd == dirCmd) theManager->SetLatticeDir(value);
  if (cmd == hitsCmd) theManager->SetHitOutput(value);
}

