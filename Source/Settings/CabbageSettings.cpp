/*
  Copyright (C) 2016 Rory Walsh

  Cabbage is free software; you can redistribute it
  and/or modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  Cabbage is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with Csound; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
  02111-1307 USA
*/

#include "CabbageSettings.h"
#include "../Utilities/CabbageUtilities.h"
#include "../Utilities/CabbageStrings.h"
//==============================================================================

CabbageSettings::CabbageSettings():
ApplicationProperties(), 
valueTree("Settings")
{
    valueTree.addListener(this);
}


void CabbageSettings::setDefaultColourSettings()
{

}

void CabbageSettings::setDefaultSettings()
{
    defaultPropSet = new PropertySet();
    ScopedPointer<XmlElement> xml;
    xml = new XmlElement("PLANTS");
    String homeDir = getCommonSettings(true)->getFile().getParentDirectory().getFullPathName();
    String manualPath;
#if !defined(MACOSX)
    manualPath = File::getSpecialLocation(File::currentExecutableFile).getParentDirectory().getFullPathName()+"/CsoundDocs";
#else
    manualPath = "/Library/Frameworks/CsoundLib64.framework/Versions/6.0/Resources/Manual";
#endif

	

    defaultPropSet->setValue("CsoundHelpDir", manualPath);
    defaultPropSet->setValue("CsoundHelpDir", manualPath);
    defaultPropSet->setValue("PlantFileDir", homeDir+"/Plants");
    defaultPropSet->setValue("ExamplesDir", homeDir+"/Examples/Examples");
    defaultPropSet->setValue("MostRecentDirectory", homeDir);
    defaultPropSet->setValue("DisablePluginInfo", 0);
    defaultPropSet->setValue("ShowEditorConsole", 1);
    defaultPropSet->setValue("ExternalEditor", 0);
    defaultPropSet->setValue("UseCabbageIO", 1);
	defaultPropSet->setValue("OpenMostRecentFileOnStartup", 0);

    defaultPropSet->setValue("ShowConsoleWithEditor", 1);
    defaultPropSet->setValue("UsingCabbageCsound", 1);
    defaultPropSet->setValue("AudioEnabled", 1);
    defaultPropSet->setValue("DisableCompilerErrorWarning", 0);
    defaultPropSet->setValue("SetAlwaysOnTop", 1);
    defaultPropSet->setValue("GridSize", 4);
	defaultPropSet->setValue("CompileOnSave", 1);
    defaultPropSet->setValue("PlantRepository", xml);
    defaultPropSet->setValue("EditorColourScheme", 0);
    defaultPropSet->setValue("showTabs", 1);
	defaultPropSet->setValue("showTabs", 1);
    defaultPropSet->setValue("EnablePopupDisplay", 1);
    defaultPropSet->setValue("ShowAutoComplete", 0);
    defaultPropSet->setValue("ShowNativeFileDialogues", 1);
    defaultPropSet->setValue("EnableNativePopup", 0);
    defaultPropSet->setValue("windowX", 100);
    defaultPropSet->setValue("windowY", 100);
	defaultPropSet->setValue("audioSetup", "");

    defaultPropSet->setValue("Colours_"+CabbageColourIds::menuBarBackground,  "ffada9a9");
    defaultPropSet->setValue("Colours_"+CabbageColourIds::menuBarText,  "FFFFFFFF");
    defaultPropSet->setValue("Colours_"+CabbageColourIds::menuBarMouseOverBackground,  "ff9a9a9a");
    defaultPropSet->setValue("Colours_"+CabbageColourIds::popupMenuBackground,  "fff5f5f5");
    defaultPropSet->setValue("Colours_"+CabbageColourIds::popupMenuMouseOverBackground,  "ffa4a4a4");
    defaultPropSet->setValue("Colours_"+CabbageColourIds::popupMenuText,  "ff484848");
    defaultPropSet->setValue("Colours_"+CabbageColourIds::popupMenuHighlightedText,  "ff000000");
	defaultPropSet->setValue("Colours_"+CabbageColourIds::mainBackground, "ff000000");
	defaultPropSet->setValue("Colours_"+CabbageColourIds::statusBar, "ff000000");
	defaultPropSet->setValue("Colours_"+CabbageColourIds::statusBarText, "ffffffff");
	defaultPropSet->setValue("Colours_"+CabbageColourIds::propertyPanelBackground, "ff222222");
    defaultPropSet->setValue("Colours_"+CabbageColourIds::alertWindowBackground, Colour(147, 210,0).toString());
    defaultPropSet->setValue("Colours_"+CabbageColourIds::codeBackground,  "FF222222");
    defaultPropSet->setValue("Colours_"+CabbageColourIds::lineNumberBackground,  "44C1C1C1");
    defaultPropSet->setValue("Colours_"+CabbageColourIds::lineNumbers,  "E9B2B2B2");
    defaultPropSet->setValue("Colours_"+CabbageColourIds::selectTextBackground,  "FF2859AC");
    defaultPropSet->setValue("Colours_"+CabbageColourIds::caret,  "FFFFFFFF");
    defaultPropSet->setValue("Colours_"+CabbageColourIds::identifierLiteral,  "FFCFCFCF");
	defaultPropSet->setValue("Colours_"+CabbageColourIds::stringLiteral,  "FFEE6F6F");
    defaultPropSet->setValue("Colours_"+CabbageColourIds::keyword,  "FFEE6F6F");
    defaultPropSet->setValue("Colours_"+CabbageColourIds::comment,  "FF72D20C");
    defaultPropSet->setValue("Colours_"+CabbageColourIds::numbers,  "FFE60000");
	defaultPropSet->setValue("Colours_"+CabbageColourIds::csdtags, "FFE60034");
	defaultPropSet->setValue("Colours_"+CabbageColourIds::consoleText, "FFFFFFFF");
	defaultPropSet->setValue("Colours_"+CabbageColourIds::consoleBackground, "FF000000");
	defaultPropSet->setValue("Colours_"+CabbageColourIds::consoleOutline, "FF000000");
	defaultPropSet->setValue("Colours_"+CabbageColourIds::genericPluginInterface, "FF666666");
	

    getUserSettings()->setFallbackPropertySet(defaultPropSet);

	valueTree.addChild(ValueTree("Colours"), -1, 0);
	const StringArray colourIDStrings = CabbageStrings::getColourIDStrings();
	
	for(int i=0;i<colourIDStrings.size();i++)
	{
		valueTree.getChildWithName("Colours").setProperty(colourIDStrings[i],  
												getUserSettings()->getValue("Colours_"+colourIDStrings[i]), 0);
	}
	
	
	audioSettingsXml = getUserSettings()->getValue("audioSetup").replace("<?xml version=\"1.0\" encoding=\"UTF-8\"?>", "");

}

void CabbageSettings::set(String child, Identifier identifier, var value)
{
	if(valueTree.getChildWithName(child).isValid())
		valueTree.getChildWithName(child).setProperty(identifier,  value, 0);
	else{
		valueTree.addChild(ValueTree(child), -1, 0);
		valueTree.getChildWithName(child).setProperty(identifier,  value, 0);
	}	
}

void CabbageSettings::setProperty(String child, var value)
{
	getUserSettings()->setValue(child, value);
}

int CabbageSettings::getIntProperty(String child)
{
	getUserSettings()->getIntValue(child);
}

String CabbageSettings::get(String child, String identifier)
{
    valueTree.getChildWithName(child).getProperty(identifier);
}

void CabbageSettings::set(ValueTree tree, String child, Identifier identifier, var value)
{
	CabbageUtilities::debug(value.toString());
	if(tree.getChildWithName(child).isValid())
		tree.getChildWithName(child).setProperty(identifier,  value.toString(), 0);
	else{
		tree.addChild(ValueTree(child), -1, 0);
		tree.getChildWithName(child).setProperty(identifier,  value.toString(), 0);
	}
}

String CabbageSettings::get(ValueTree tree, String child, String identifier)
{
	tree.getChildWithName(child).getProperty(identifier);	
}

Colour CabbageSettings::getColourFromValueTree(ValueTree tree, Identifier identifier, Colour defaultColour)
{
    const String colour = tree.getChildWithName("Colours").getProperty(identifier, defaultColour.toString()).toString();
    return Colour::fromString(colour);
}

String CabbageSettings::getColourPropertyName(ValueTree tree, int index)
{
    return tree.getChildWithName("Colours").getPropertyName(index).toString();
}

Colour CabbageSettings::getColourFromValueTree(ValueTree tree, int index, Colour defaultColour)
{
    if(index<=tree.getChildWithName("Colours").getNumProperties())
    {
        const String colour = tree.getChildWithName("Colours").getProperty(tree.getChildWithName("Colours").getPropertyName(index), defaultColour.toString()).toString();
        return Colour::fromString(colour);
    }
    else
        return defaultColour;
}

ValueTree CabbageSettings::getValueTree()
{
    return valueTree;
}

void CabbageSettings::valueTreePropertyChanged (ValueTree& tree, const Identifier& value)
{
	CabbageUtilities::debug(value.toString());
	if(valueTree.getChildWithName("Colours").getProperty(value).toString().isNotEmpty())
		getUserSettings()->setValue("Colours_"+value.toString(), valueTree.getChildWithName("Colours").getProperty(value).toString());

	sendChangeMessage();
}
	
XmlElement* CabbageSettings::getXML(String identifier)
{
    return getUserSettings()->getXmlValue(identifier);
}

int CabbageSettings::getIndexOfProperty(String child, String identifier)
{
    return valueTree.indexOf(valueTree.getChildWithName(child));
}

void CabbageSettings::updateRecentFilesList(File file)
{
    recentFiles.restoreFromString (getUserSettings()->getValue ("recentlyOpenedFiles"));
    recentFiles.addFile (file);
    getUserSettings()->setValue ("recentlyOpenedFiles", recentFiles.toString());	
}

void CabbageSettings::updateRecentFilesList()
{
    recentFiles.restoreFromString (getUserSettings()->getValue ("recentlyOpenedFiles"));
    getUserSettings()->setValue ("recentlyOpenedFiles", recentFiles.toString());	
}

File CabbageSettings::getMostRecentFile()
{
    recentFiles.restoreFromString (getUserSettings()->getValue ("recentlyOpenedFiles"));
    return recentFiles.getFile(0);	
}


