/*
  Copyright (C) 2016 Rory Walsh

  Cabbage is free software; you can redistribute it
  and/or modify it under the terms of the GNU General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  Cabbage is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU General Public
  License along with Csound; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
  02111-1307 USA
*/

#ifndef CabbageNumberSlider_H_INCLUDED
#define CabbageNumberSlider_H_INCLUDED

#include "../CabbageCommonHeaders.h"
#include "CabbageWidgetBase.h"


class SliderLookAndFeel : public LookAndFeel_V2
{
	Colour fontColour = { 0,0,0 };
	int fontSize = 0;
public:

	SliderLookAndFeel(Colour col, int fS):fontColour(col), fontSize(fS){};
	~SliderLookAndFeel() {};

	Label* createSliderTextBox(Slider& slider)
	{
		auto l = new Label();

		l->setColour(Label::textColourId, fontColour);
		l->setJustificationType(Justification::centred);
		l->setKeyboardType(TextInputTarget::decimalKeyboard);

		l->setColour(Label::textColourId, slider.findColour(Slider::textBoxTextColourId));
		l->setColour(Label::backgroundColourId,
			(slider.getSliderStyle() == Slider::LinearBar || slider.getSliderStyle() == Slider::LinearBarVertical)
			? Colours::transparentBlack
			: slider.findColour(Slider::textBoxBackgroundColourId));
		l->setColour(Label::outlineColourId, slider.findColour(Slider::textBoxOutlineColourId));
		l->setColour(TextEditor::textColourId, slider.findColour(Slider::textBoxTextColourId));
		l->setColour(TextEditor::backgroundColourId,
			slider.findColour(Slider::textBoxBackgroundColourId)
			.withAlpha((slider.getSliderStyle() == Slider::LinearBar || slider.getSliderStyle() == Slider::LinearBarVertical)
				? 0.7f : 1.0f));
		l->setColour(TextEditor::outlineColourId, slider.findColour(Slider::textBoxOutlineColourId));
		l->setColour(TextEditor::highlightColourId, slider.findColour(Slider::textBoxHighlightColourId));
		if(fontSize!=-1)
			l->setFont(Font(fontSize));

		return l;
	}

	//========= linear slider ================================================================================
	void drawLinearSlider(Graphics& g, int x, int y, int width, int height,
		float sliderPos, float minSliderPos, float maxSliderPos,
		const Slider::SliderStyle style, Slider& slider)
	{
		g.setColour(slider.findColour(Slider::thumbColourId));
		g.fillRoundedRectangle(x, y, width, height, 3);		
	}

};

class CabbageNumberSlider : public Component, public ValueTree::Listener, public CabbageWidgetBase
{
    Slider slider;
    Label label;
    String text="", align="", postfix="";

public:
	SliderLookAndFeel sliderLookAndFeel;
    CabbageNumberSlider (ValueTree wData);
	~CabbageNumberSlider() {
		slider.setLookAndFeel(nullptr);

	};

    //ValueTree::Listener virtual methods....
    void valueTreePropertyChanged (ValueTree& valueTree, const Identifier&) override;
    void valueTreeChildAdded (ValueTree&, ValueTree&)override {};
    void valueTreeChildRemoved (ValueTree&, ValueTree&, int) override {}
    void valueTreeChildOrderChanged (ValueTree&, int, int) override {}
    void valueTreeParentChanged (ValueTree&) override {};
    void resized() override;

    Slider& getSlider()
    {
        return slider;
    };

    ValueTree widgetData;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CabbageNumberSlider);
};


#endif  // CabbageNumberSlider_H_INCLUDED
