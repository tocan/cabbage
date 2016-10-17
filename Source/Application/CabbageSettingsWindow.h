/*
  ==============================================================================

    CabbageSettingsWindow.h
    Created: 14 Oct 2016 4:12:59pm
    Author:  rory

  ==============================================================================
*/

#ifndef CABBAGESETTINGSWINDOW_H_INCLUDED
#define CABBAGESETTINGSWINDOW_H_INCLUDED

#include "CabbageIds.h"
#include "CabbageSettings.h"
#include "../Utilities/CabbageColourPropertyComponent.h"
#include "../Utilities/CabbageUtilities.h"


class CabbageSettingsWindow : public Component
{

public:
    CabbageSettingsWindow(ValueTree valueTree);
    ~CabbageSettingsWindow() {};

    void addColourProperties();

    void resized() override
    {
        Rectangle<int> r (getLocalBounds());
        panel.setBounds (r.removeFromTop (getHeight() - 28).reduced (4, 2));
        loadButton.setBounds (r.removeFromLeft (getWidth() / 2).reduced (10, 4));
        saveButton.setBounds (r.reduced (10, 3));
    }

private:
    PropertyPanel panel;
    ValueTree valueTree;
    TextButton loadButton, saveButton;


};



#endif  // CABBAGESETTINGSWINDOW_H_INCLUDED