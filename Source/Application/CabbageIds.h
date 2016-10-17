/*
  ==============================================================================

    CabbageCommandIDs.h
    Created: 12 Oct 2016 11:49:31am
    Author:  rory

  ==============================================================================
*/

#ifndef CABBAGECOMMANDIDS_H_INCLUDED
#define CABBAGECOMMANDIDS_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

namespace CommandIDs
{
enum
{
    newProject              = 0x200010,
    open                    = 0x200020,
    closeDocument           = 0x200030,
    saveDocument            = 0x200040,
    saveDocumentAs          = 0x200041,

    closeProject            = 0x200051,
    saveProject             = 0x200060,
    saveAll                 = 0x200080,
    openInIDE               = 0x200072,
    saveAndOpenInIDE        = 0x200073,
    createNewExporter       = 0x20007d,

    showUTF8Tool            = 0x200076,
    showGlobalPreferences   = 0x200077,
    showConfigPanel         = 0x200074,
    showFilePanel           = 0x200078,
    showTranslationTool     = 0x200079,
    showProjectSettings     = 0x20007a,
    showProjectModules      = 0x20007b,
    showSVGPathTool         = 0x20007c,

    closeWindow             = 0x201001,
    closeAllDocuments       = 0x201000,
    goToPreviousDoc         = 0x201002,
    goToNextDoc             = 0x201003,
    goToCounterpart         = 0x201004,
    deleteSelectedItem      = 0x201005,

    showFindPanel           = 0x2010a0,
    findSelection           = 0x2010a1,
    findNext                = 0x2010a2,
    findPrevious            = 0x2010a3,


    showBuildTab            = 0x410001,
    cleanAll                = 0x410010,
    enableBuild             = 0x410011,
    showWarnings            = 0x410012,
    reinstantiateComp       = 0x410013,
    launchApp               = 0x410014,
    killApp                 = 0x410015,
    buildNow                = 0x41001a,
    toggleContinuousBuild   = 0x41001b,

    enableSnapToGrid        = 0x410016,
    zoomIn                  = 0x410017,
    zoomOut                 = 0x410018,
    zoomNormal              = 0x410019,
    spaceBarDrag            = 0x4100a1,

    nextError               = 0x4100b1,
    prevError               = 0x4100b2,

    loginLogout             = 0x4100c1,

    lastCommandIDEntry
};
}

namespace CommandCategories
{
static const char* const general       = "General";
static const char* const editing       = "Editing";
static const char* const view          = "View";
static const char* const windows       = "Windows";
}

// Handy list of static Identifiers..
namespace Ids
{
#define DECLARE_ID(name)      const Identifier name (#name)

DECLARE_ID (name);
DECLARE_ID (font);
DECLARE_ID (colour);
DECLARE_ID (csoundManualDirectory);
DECLARE_ID (cabbagePlantDirectory);
DECLARE_ID (cabbageExamplesDirectory);
const Identifier ID ("id");
const Identifier ID_uppercase ("ID");
const Identifier class_ ("class");

#undef DECLARE_ID
};

// Handy list of static Identifiers..
namespace CabbageColourIds
{
static const Identifier menuBarBackground = "MenuBarBackground";
static const Identifier menuBarText = "MenuBarText";
static const Identifier menuBarMouseOverBackground = "MenuBarMouseOverBackground";
static const Identifier popupMenuMouseOverBackground = "PopupMenuMouseOverBackground";
static const Identifier popupMenuBackground = "PopupMenuBackground";
static const Identifier popupMenuText = "PopupMenuText";
static const Identifier popupMenuHighlightedText = "PopupMenuHighlightedText";
static const Identifier mainBackground = "MainBackground";
static const Identifier codeBackground = "CodeBackground";
static const Identifier lineNumberBackground = "LineNumberBackground";
static const Identifier lineNumbers = "LineNumbers";
static const Identifier plainText = "PlainText";
static const Identifier selectTextBackground = "SelectTextBackground";
static const Identifier caret = "Caret";
static const Identifier preprocessor = "Preprocessor";
static const Identifier punctuation = "Punctuation";
static const Identifier bracket = "Bracket";
static const Identifier stringLiteral = "String";
static const Identifier floatLiteral = "Float";
static const Identifier integerLiteral = "Integer";
static const Identifier identifierLiteral = "Identifier";
static const Identifier operatorLiteral = "Operator";
static const Identifier keyword = "Keyword";
static const Identifier comment = "Comment";
static const Identifier error = "Error";
static const Identifier alertWindowBackground = "AlartWindowBackground";
}

#endif  // CABBAGECOMMANDIDS_H_INCLUDED