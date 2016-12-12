/*
  ==============================================================================

    CabbageLookAndFeel2.cpp
    Created: 10 Nov 2016 5:36:11pm
    Author:  rory

  ==============================================================================
*/

#include "CabbageLookAndFeel2.h"

namespace LookAndFeelHelpers
{
static Colour createBaseColour (Colour buttonColour,
                                bool hasKeyboardFocus,
                                bool isMouseOverButton,
                                bool isButtonDown) noexcept
{
    const float sat = hasKeyboardFocus ? 1.3f : 0.9f;
    const Colour baseColour (buttonColour.withMultipliedSaturation (sat));

    if (isButtonDown)      return baseColour.contrasting (0.2f);
    if (isMouseOverButton) return baseColour.contrasting (0.1f);

    return baseColour;
}

static TextLayout layoutTooltipText (const String& text, Colour colour) noexcept
{
    const float tooltipFontSize = 13.0f;
    const int maxToolTipWidth = 400;

    AttributedString s;
    s.setJustification (Justification::centred);
    s.append (text, Font (tooltipFontSize, Font::bold), colour);

    TextLayout tl;
    tl.createLayoutWithBalancedLineLengths (s, (float) maxToolTipWidth);
    return tl;
}
}

//Cabbage IDE look and feel class
CabbageLookAndFeel2::CabbageLookAndFeel2()
{

}

//======== Group Components ======================================================================
void CabbageLookAndFeel2::drawGroupComponentOutline (Graphics &g, int w, int h, const String &text,
        const Justification &position,
        GroupComponent &group)
{
    g.fillAll(Colours::transparentBlack);
    File imgFile(group.getProperties().getWithDefault("imggroupbox", "").toString());
	const int outlineThickness = group.getProperties().getWithDefault("outlinethickness", 1);


    //if valid SVG file....
    if(imgFile.existsAsFile())
	{
		if(imgFile.hasFileExtension("svg"))
			drawFromSVG(g, imgFile, 0, 0, group.getWidth(), group.getHeight(), AffineTransform::identity);
		else if(imgFile.hasFileExtension("png"))
		{
			Image image = ImageCache::getFromFile(imgFile);
            image = image.rescaled(group.getWidth(), group.getHeight());
			g.drawImage(image, 0, 0, group.getWidth(), group.getHeight(), 0, 0, group.getWidth(), group.getHeight());
					
		}
	}
    else
    {

        int corners = group.getProperties().getWithDefault("cornersize", 5);
        Colour col;
        g.setColour (group.findColour(TextButton::buttonColourId));
        g.fillRoundedRectangle (0, 0, w, h, corners);

        //----- Outline
        g.setColour (group.findColour(GroupComponent::outlineColourId));
        g.drawRoundedRectangle (0.5, 0.5, w-1, h-1, corners, outlineThickness);

        if(outlineThickness>0)
        {
            g.drawLine (10, 20, w-10, 20, outlineThickness);
        }
    }

    //----- Text
    String name = group.getText();
    Font font = CabbageUtilities::getTitleFont();
#ifndef MACOSX
    font.setFallbackFontName("Verdana");
#endif
    g.setFont (font);

    g.setColour (group.findColour(GroupComponent::textColourId));
    name = CabbageUtilities::cabbageString (name, font, group.getWidth());
    g.drawText (name, 0, 5, w, font.getHeight(), 36, false);
}
//===========================================================================================
void CabbageLookAndFeel2::drawToggleButton (Graphics &g, ToggleButton &button, bool isMouseOverButton, bool isButtonDown)
{
    Image image;
    bool drawFromFile = true;
    ScopedPointer<DrawableRectangle> drawableRect;
    const File imgButtonOnFile(button.getProperties().getWithDefault(CabbageIdentifierIds::imgbuttonon, "").toString());
    const File imgButtonOffFile(button.getProperties().getWithDefault(CabbageIdentifierIds::imgbuttonoff, "").toString());
    const int corners = button.getProperties().getWithDefault(CabbageIdentifierIds::corners, 2.f);
    const bool isRectangle = button.getProperties().getWithDefault(CabbageIdentifierIds::shape, false);
    float fontSize = jmin (15.0f, button.getHeight() * 0.85f);
    const float tickWidth = button.getHeight() * .9f;
    int imgHeight, imgWidth;
    bool toggleState = button.getToggleState();

    if (button.hasKeyboardFocus (true))
    {
        g.setColour (button.findColour (TextEditor::focusedOutlineColourId));
        g.drawRect (0, 0, button.getWidth(), button.getHeight());
    }


    if(imgButtonOnFile.existsAsFile() && imgButtonOffFile.existsAsFile())	//if image files exist, draw them..
    {
        if(imgButtonOnFile.hasFileExtension("png") && imgButtonOffFile.hasFileExtension("png"))
        {
            image = ImageCache::getFromFile(File(toggleState == true ? imgButtonOnFile : imgButtonOffFile));
            image = image.rescaled(button.getWidth(), button.getHeight());
            g.drawImage(image, 4.0f, (button.getHeight() - tickWidth) * 0.5f, button.getWidth()-4, tickWidth, 0, 0, button.getWidth(), button.getHeight(), false);
        }
        else if(imgButtonOnFile.hasFileExtension("svg") && imgButtonOffFile.hasFileExtension("svg"))
        {
            drawFromSVG(g, toggleState == true ? imgButtonOnFile : imgButtonOffFile, 0, 0, button.getWidth(), button.getHeight(), AffineTransform::identity);
        }
    }

    else	//if files don't exist, draw a native Cabbage checkbox
    {
        image = drawToggleImage(tickWidth, button.getHeight(), button.getToggleState(), button.findColour(toggleState == true ? TextButton::ColourIds::buttonOnColourId : TextButton::ColourIds::buttonColourId), isRectangle, corners);
        g.drawImage(image, 4.0f, (button.getHeight() - tickWidth) * 0.5f, button.getWidth()-4, tickWidth, 0, 0, button.getWidth(), button.getHeight(), false);
    }


    g.setColour (toggleState == true ? button.findColour(TextButton::textColourOnId) : button.findColour(TextButton::textColourOffId));
    g.setFont (fontSize);

    if (! button.isEnabled())
        g.setOpacity (0.5f);

    const int textX = (int) tickWidth + 10;

    int widthButton =  button.getWidth();

    if(button.getButtonText().isNotEmpty())
        g.drawFittedText(button.getButtonText(),
                         textX, 0,
                         jmax(20, button.getWidth() - textX - 2), button.getHeight(),
                         Justification::centredLeft, 5);

}

//==========================================================================================================================================
Image CabbageLookAndFeel2::drawToggleImage (float width, float height, bool isToggleOn, const Colour colour, const bool isRectangle, const float corners)
{
    Image img = Image(Image::ARGB, width, height, true);
    Graphics g (img);
    float opacity = 0;

    if (isRectangle)
    {
        if (isToggleOn == true)
        {
            g.setColour (colour);
            g.fillRoundedRectangle(width*0.01, height*0.01, width*0.93, height*0.93, corners);
            opacity = 0.4;
        }
        else
        {
            for (float i=0.01; i<0.05; i+=0.01)
            {
                g.setColour (Colour::fromRGBA (0, 0, 0, 255/(i*100)));
                g.fillRoundedRectangle (width*i+1, height*i+1,
                                        width*0.95, height*0.95, corners);
            }

            Colour bg1 = colour.darker();
            Colour bg2 = colour;
            ColourGradient cg = ColourGradient (bg1, 0, 0, bg2, width*0.5, height*0.5, false);
            g.setGradientFill (cg);
            g.fillRoundedRectangle (width*0.01, height*0.01, width*0.93, height*0.93, corners);
            opacity = 0.2;
        }

        // For emphasising the top and left edges to give the illusion that light is shining on them
        ColourGradient edgeHighlight = ColourGradient (Colours::whitesmoke, 0, 0,
                                       Colours::transparentWhite, 0, height*0.1, false);
        g.setGradientFill (edgeHighlight);
        g.setOpacity (opacity);
        g.fillRoundedRectangle (0, 0, width*0.95, height*0.95, corners);

        ColourGradient edgeHighlight2 = ColourGradient (Colours::whitesmoke, 0, 0,
                                        Colours::transparentWhite, height*0.1, 0, false);
        g.setGradientFill (edgeHighlight2);
        g.setOpacity (opacity);
        g.fillRoundedRectangle (0, 0, width*0.95, height*0.95, corners);
		
    }
	
    else   //else if round toggle
    {
        ColourGradient base = ColourGradient (Colours::white, width*-0.3, height*-0.3, Colours::black,
                                              width*0.8, height*0.8, false);
        g.setGradientFill(base);
        g.fillEllipse (0, 0, width, height);
        g.setColour(Colours::black);
        g.fillEllipse(width*0.09, height*0.09, width*0.82, height*0.82);
        Colour outline = Colour::fromRGB(70, 70, 70);
        g.setColour(outline.withAlpha(colour.getAlpha()));
        g.fillEllipse(width*0.04, height*0.04, width*0.92, height*0.92);

        if (isToggleOn)   //on
        {
            g.setColour(colour);
            g.fillEllipse(width*0.09, height*0.09, width*0.82, height*0.82);
        }
        else   //off
        {
            g.setColour(Colours::black);
            g.fillEllipse(width*0.09, height*0.09, width*0.82, height*0.82);

            Colour bg1 = Colour::fromRGBA (25, 25, 28, 255);
            Colour bg2 = Colour::fromRGBA (15, 15, 18, 255);
            ColourGradient cg = ColourGradient (bg1, 0, 0, bg2, width*0.5, height*0.5, false);
            g.setGradientFill (cg);
            g.setOpacity(0.4);
            g.fillEllipse(width*0.1, height*0.1, width*0.8, height*0.8);
        }
    }
	
    return img;
}
//==========================================================================================================================================
void CabbageLookAndFeel2::drawRotarySlider (Graphics& g, int x, int y, int width, int height, float sliderPos,
        const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider)
{

    const float radius = jmin (width / 2, height / 2) - 2.0f;
    const float diameter = radius*2.f;
    const float centreX = x + width * 0.5f;
    const float centreY = y + height * 0.5f;
    const float rx = centreX - radius;
    const float ry = centreY - radius;
    const float rw = radius * 2.0f;
    const float angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    const bool isMouseOver = slider.isMouseOverOrDragging() && slider.isEnabled();
    bool useSliderBackgroundImg = false;
    bool useSliderSVG = false;
    Image image;
    const File imgSlider(slider.getProperties().getWithDefault(CabbageIdentifierIds::imgslider, "").toString());
	//CabbageUtilities::debug(slider.getProperties().getWithDefault(CabbageIdentifierIds::imgsliderbg, "").toString());
    const File imgSliderBackground(slider.getProperties().getWithDefault(CabbageIdentifierIds::imgsliderbg, "").toString());
 
    const int svgSliderWidthBg = slider.getProperties().getWithDefault("svgsliderbgwidth", 100);
    const int svgSliderHeightBg = slider.getProperties().getWithDefault("svgsliderbgheight", 100);
    const int svgSliderWidth = slider.getProperties().getWithDefault("svgsliderwidth", 100);
    const int svgSliderHeight = slider.getProperties().getWithDefault("svgsliderheight", 100);

    //if valid background SVG file....
    if(imgSliderBackground.existsAsFile())
	{
        if(imgSliderBackground.hasFileExtension("png"))
        {
            image = ImageCache::getFromFile(imgSliderBackground);
            image = image.rescaled(slider.getWidth(), slider.getHeight());
            g.drawImage(image, rx, ry, diameter, diameter, 0, 0, svgSliderWidthBg, svgSliderHeightBg, false);
        }
        else if(imgSliderBackground.hasFileExtension("svg"))
        {
            drawFromSVG(g, imgSliderBackground, 0, 0, slider.getWidth(), slider.getHeight(), AffineTransform::identity);
        }
		
        useSliderBackgroundImg = true;
    }

    slider.setSliderStyle(Slider::RotaryVerticalDrag);

    if (radius > 12.0f)
    {

        //tracker
        if(slider.findColour (Slider::trackColourId).getAlpha()==0)
            g.setColour(Colours::transparentBlack);
        else
            g.setColour (slider.findColour (Slider::trackColourId).withAlpha (isMouseOver ? 1.0f : 0.9f));

        const float thickness = 0.7f;
        {
            Path filledArc;
            filledArc.addPieSegment (rx, ry, rw, rw, rotaryStartAngle, angle, thickness);
            g.fillPath (filledArc);
        }

        if(imgSlider.existsAsFile())
        {
            if(slider.findColour (Slider::trackColourId).getAlpha()==0)
                g.setColour(Colours::transparentBlack);
            else
                g.setColour (slider.findColour (Slider::trackColourId).withAlpha (isMouseOver ? 1.0f : 0.9f));
            const float thickness = slider.getProperties().getWithDefault("trackerthickness", .7);
            {
                Path filledArc;
                filledArc.addPieSegment (rx, ry, rw, rw, rotaryStartAngle, angle, 1.f-thickness);
                g.fillPath (filledArc);
            }

            g.setOpacity(1.0);
			
			if(imgSlider.hasFileExtension("png"))
			{
				image = ImageCache::getFromFile(imgSlider);
				image = image.rescaled(slider.getWidth(), slider.getHeight());
				ScopedPointer<Drawable> drawable;
				drawable = Drawable::createFromImageFile(imgSlider);
				drawable->setTransformToFit(Rectangle<float>(0, 0, slider.getWidth(), slider.getWidth()), RectanglePlacement::centred);
				drawable->draw(g, 1.f, AffineTransform::rotation(angle,
                                    slider.getWidth()/2, slider.getWidth()/2));
				//g.drawImage(image, rx, ry, diameter, diameter, 0, 0, slider.getWidth(), slider.getHeight(), false);
			}
			else if(imgSlider.hasFileExtension("svg"))
			{
				drawFromSVG(g, imgSlider, 0, 0, slider.getWidth(), slider.getHeight(), AffineTransform::rotation(angle,
                                    slider.getWidth()/2, slider.getWidth()/2));
			}			
			
            useSliderSVG = true;
        }
        else
            useSliderSVG = false;


        //outinecolour
        if(!useSliderBackgroundImg)
        {
            g.setColour (slider.findColour (Slider::rotarySliderOutlineColourId));

            Path outlineArc;
            outlineArc.addPieSegment (rx, ry, rw, rw, rotaryStartAngle, rotaryEndAngle, thickness);
            outlineArc.closeSubPath();

            g.strokePath (outlineArc, PathStrokeType (slider.isEnabled() ? (isMouseOver ? 2.0f : 1.2f) : 0.3f));
        }
        if(!useSliderSVG)
        {
            Path newPolygon;
            Point<float> centre (centreX, centreY);

            if (diameter >= 25)   //If diameter is >= 40 then polygon has 12 steps
            {
                newPolygon.addPolygon(centre, 12.f, radius*.65, 0.f);
                newPolygon.applyTransform (AffineTransform::rotation (angle,
                                           centreX, centreY));
            }
            else //Else just use a circle. This is clearer than a polygon when very small.
                newPolygon.addEllipse (-radius*.2, -radius*.2, radius * .3f, radius * .3f);


            g.setColour (slider.findColour (Slider::thumbColourId));

            Colour thumbColour = slider.findColour (Slider::thumbColourId).withAlpha (isMouseOver ? 1.0f : 0.9f);
            ColourGradient cg = ColourGradient (Colours::white, 0, 0, thumbColour, diameter*0.6, diameter*0.4, false);
            if(slider.findColour (Slider::thumbColourId)!=Colour(0.f,0.f,0.f,0.f))
                g.setGradientFill (cg);
            g.fillPath (newPolygon);
        }
    }
    else
    {
        Path p;
        g.setColour (slider.findColour (Slider::thumbColourId).withAlpha (isMouseOver ? 1.0f : 0.7f));
        p.addEllipse (-0.4f * rw, -0.4f * rw, rw * 0.8f, rw * 0.8f);
        g.fillPath(p, AffineTransform::rotation (angle).translated (centreX, centreY));

        //if (slider.isEnabled())
        g.setColour (slider.findColour (Slider::rotarySliderOutlineColourId).withAlpha (isMouseOver ? 0.7f : 0.5f));
        //else
        //    g.setColour (Colour (0x80808080));

        p.addEllipse (-0.4f * rw, -0.4f * rw, rw * 0.8f, rw * 0.8f);
        PathStrokeType (rw * 0.1f).createStrokedPath (p, p);

        p.addLineSegment (Line<float> (0.0f, 0.0f, 0.0f, -radius), rw * 0.1f);

        g.fillPath (p, AffineTransform::rotation (angle).translated (centreX, centreY));

    }

}

//========= linear slider ================================================================================
void CabbageLookAndFeel2::drawLinearSlider (Graphics& g, int x, int y, int width, int height,
        float sliderPos, float minSliderPos, float maxSliderPos,
        const Slider::SliderStyle style, Slider& slider)
{

    if (style == Slider::LinearBar || style == Slider::LinearBarVertical)
    {
        g.setColour(slider.findColour (Slider::thumbColourId));
        g.fillRoundedRectangle(x, y, width, height, 3);
    }
    else
    {
        drawLinearSliderBackground (g, x, y, width, height, sliderPos, minSliderPos, maxSliderPos, style, slider);
        drawLinearSliderThumb (g, x, y, width, height, sliderPos, minSliderPos, maxSliderPos, style, slider);
    }
}

Slider::SliderLayout CabbageLookAndFeel2::getSliderLayout(Slider& slider)
{
    Slider::SliderLayout layout;
    layout.sliderBounds = slider.getLocalBounds();
	
    int minXSpace = 0;
    int minYSpace = 0;

    Slider::TextEntryBoxPosition textBoxPos = slider.getTextBoxPosition();

    if (textBoxPos == Slider::TextBoxLeft || textBoxPos == Slider::TextBoxRight)
        minXSpace = 30;
    else
        minYSpace = 15;

    Rectangle<int> localBounds = slider.getLocalBounds();
	
	if(slider.getSliderStyle() == Slider::LinearBarVertical)
		return layout;
	

    const int textBoxWidth = jmax (0, jmin (slider.getTextBoxWidth(),  localBounds.getWidth() - minXSpace));
    const int textBoxHeight = jmax (0, jmin (slider.getTextBoxHeight(), localBounds.getHeight() - minYSpace));


    if (textBoxPos != Slider::NoTextBox)
    {
        if (slider.isBar())
        {
            layout.textBoxBounds = localBounds;
        }
        else
        {
            layout.textBoxBounds.setWidth (textBoxWidth);
            layout.textBoxBounds.setHeight (textBoxHeight);

            if (textBoxPos == Slider::TextBoxLeft)           layout.textBoxBounds.setX (0);
            else if (textBoxPos == Slider::TextBoxRight)     layout.textBoxBounds.setX (localBounds.getWidth() - textBoxWidth);
            else /* above or below -> centre horizontally */ layout.textBoxBounds.setX ((localBounds.getWidth() - textBoxWidth) / 2);

            if (textBoxPos == Slider::TextBoxAbove)          layout.textBoxBounds.setY (0);
            else if (textBoxPos == Slider::TextBoxBelow)     layout.textBoxBounds.setY (localBounds.getHeight() - textBoxHeight);
            else /* left or right -> centre vertically */    layout.textBoxBounds.setY ((localBounds.getHeight() - textBoxHeight) / 2);
        }
    }

    layout.sliderBounds = localBounds;

    if (slider.isBar())
    {
        layout.sliderBounds.reduce (1, 1);   // bar border
    }
    else
    {
        if (textBoxPos == Slider::TextBoxLeft)       layout.sliderBounds.removeFromLeft (textBoxWidth);
        else if (textBoxPos == Slider::TextBoxRight) layout.sliderBounds.removeFromRight (textBoxWidth);
        else if (textBoxPos == Slider::TextBoxAbove) layout.sliderBounds.removeFromTop (textBoxHeight);
        else if (textBoxPos == Slider::TextBoxBelow) layout.sliderBounds.removeFromBottom (textBoxHeight);

        const int thumbIndent = getSliderThumbRadius (slider);

        if (slider.isHorizontal())    layout.sliderBounds.reduce (thumbIndent, 0);
        else if (slider.isVertical()) layout.sliderBounds.reduce (0, thumbIndent);
    }

    return layout;
}

//=========== Linear Slider Background ===========================================================================
void CabbageLookAndFeel2::drawLinearSliderBackground (Graphics &g, int x, int y, int width, int height, float sliderPos,
        float minSliderPos,
        float maxSliderPos,
        const Slider::SliderStyle style,
        Slider &slider)
{

    const float sliderRadius = (float) (getSliderThumbRadius (slider) - 2);
    float xOffset = (sliderRadius/width);
    const Colour trackColour (slider.findColour (Slider::trackColourId));
    float zeroPosProportional = 0;
    if (slider.getMinimum() < 0)
        zeroPosProportional = slider.valueToProportionOfLength(0); //takes into account skew factor

    const int useGradient = slider.getProperties().getWithDefault("gradient", 1);
    const float trackerThickness = slider.getProperties().getWithDefault("trackerthickness", .75);
    bool usingImg=false;

    const File imgSliderBackground(slider.getProperties().getWithDefault("imgsliderbg", "").toString());
	
    //if valid background SVG file....
    if(imgSliderBackground.existsAsFile())
	{
        if(imgSliderBackground.hasFileExtension("png"))
        {
            Image image = ImageCache::getFromFile(imgSliderBackground);
            image = image.rescaled(slider.getWidth(), slider.getHeight());
			if(slider.isHorizontal())
				g.drawImage(image, 0, 0,  width*(1+xOffset*2), height, 0, 0, slider.getWidth(), slider.getHeight(), false);
			else
				g.drawImage(image, 0, height*.05, width, height+sliderRadius, 0, 0, slider.getWidth(), slider.getHeight(), false);
			
			usingImg = true;
        }
        else if(imgSliderBackground.hasFileExtension("svg"))
        {
			if(slider.isVertical())
				drawFromSVG(g, imgSliderBackground, 0, height*.05,  width, height*1.1, AffineTransform::identity);
			else
				drawFromSVG(g, imgSliderBackground, 0, 0, width*1.08, height, AffineTransform::identity);
				
			usingImg = true;
		}
		
    }	

    Path indent;
    if (slider.isHorizontal())
    {
        if(!usingImg)
        {
            width = width-8;
            g.setColour (Colours::whitesmoke);
            g.setOpacity (0.6);
            const float midPoint = (width/2.f+sliderRadius)+3;
            const float markerGap = width/9.f;
            g.drawLine (midPoint, height*0.25, midPoint, height*0.75, 1.5);
            g.setOpacity (0.3);
            for (int i=1; i<5; i++)
            {
                g.drawLine (midPoint+markerGap*i, height*0.3, midPoint+markerGap*i, height*0.7, .7);
                g.drawLine (midPoint-markerGap*i, height*0.3, midPoint-markerGap*i, height*0.7, .7);
            }
            //backgrounds
            g.setColour (Colours::whitesmoke);
            g.setOpacity (0.1);
            g.fillRoundedRectangle (sliderRadius, height*0.44, width*1.021, height*0.15, height*0.05); //for light effect
            g.setColour (Colour::fromRGBA(5, 5, 5, 255));
            g.fillRoundedRectangle (sliderRadius, height*0.425, width*1.016, height*0.15, height*0.05); //main rectangle
        }

        const float scale = trackerThickness;
        const float ih = (height * scale);
        const float iy = ((height-ih)/2.f);

        //gradient fill for tracker...
		if(useGradient)
		{
			if(slider.getMinimum()>=0)
				g.setGradientFill(ColourGradient (Colours::transparentBlack, 0, 0, trackColour, width*0.25, 0, false));
			else
				g.setGradientFill(ColourGradient(Colours::transparentBlack,
												 (slider.getValue()<= 0 ? zeroPosProportional*width*1.25 : zeroPosProportional*width),
												 0,
												 trackColour,
												 (slider.getValue()<= 0 ? 0 : width),
												 0,
												 false));
		}
		else
			g.setColour(trackColour);

		if(slider.getValue()>0)
			g.fillRoundedRectangle (zeroPosProportional*width + sliderRadius, iy,
									sliderPos - sliderRadius*0.5 - zeroPosProportional*width, ih,
									5.0f);
		else
			g.fillRoundedRectangle(sliderPos, iy,
								   jmax(0.f, zeroPosProportional*width + sliderRadius - sliderPos), ih,
								   5.0f);


        if(!usingImg)
        {
            g.fillPath (indent);
            g.setColour (Colour (0x4c000000));
            g.strokePath (indent, PathStrokeType (0.3f));
        }

    }
    else //vertical
    {
        if(!usingImg)
        {
            height = height-6;
            g.setColour (Colours::whitesmoke);
            g.setOpacity (0.6);
            const float midPoint = (height/2.f+sliderRadius)+3;
            const float markerGap = height/9.f;
            g.drawLine (width*0.25, midPoint, width*0.75, midPoint, 1.59);
            g.setOpacity (0.3);

            for (int i=1; i<5; i++)
            {
                g.drawLine (width*0.3, midPoint+markerGap*i, width*0.7, midPoint+markerGap*i, .7);
                g.drawLine (width*0.3, midPoint-markerGap*i, width*0.7, midPoint-markerGap*i, .7);
            }

            g.setColour(Colours::whitesmoke);
            g.setOpacity (0.1);
            g.fillRoundedRectangle(width*0.44, sliderRadius, width*0.15, height*1.003, width*0.05);
            g.setColour (Colour::fromRGBA(5, 5, 5, 255));
            g.fillRoundedRectangle (width*0.425, sliderRadius, width*0.15, height*1.1, width*0.05);
        }

        const float scale = trackerThickness;
        const float iw = (width* scale);
        const float ix = ((width-iw)/2.f);

		if(useGradient)
		{
			if(slider.getMinimum()>=0)
				g.setGradientFill(ColourGradient(Colours::transparentBlack, 0, height, trackColour, 0, height*0.8, false));
			else
				g.setGradientFill(ColourGradient(Colours::transparentBlack,
												 0,
												 (slider.getValue()<= 0 ? zeroPosProportional*height : zeroPosProportional*height*1.25),
												 trackColour,
												 0,
												 (slider.getValue()<= 0 ? height : 0),
												 false));
		}
		else
			g.setColour(trackColour);


		if(slider.getValue()>=0)
		{
			const int sliderHeight = jmax(0.f, height - sliderPos + sliderRadius+1.5f - zeroPosProportional*height);
			g.fillRoundedRectangle(ix, y + sliderPos - sliderRadius*2,
									iw, sliderHeight,
									3.0f);
		}
		else
			g.fillRoundedRectangle (ix, zeroPosProportional*height+sliderRadius,
									iw, sliderPos - sliderRadius - zeroPosProportional*height,
									3.0f);


        if(!usingImg)
        {
            g.fillPath (indent);
            g.setColour (Colour (0x4c000000));
            g.strokePath (indent, PathStrokeType (0.3f));
        }
    }

}


//========== Linear Slider Thumb =========================================================================
void CabbageLookAndFeel2::drawLinearSliderThumb (Graphics& g, int x, int y, int width, int height,
        float sliderPos, float minSliderPos, float maxSliderPos,
        const Slider::SliderStyle style, Slider& slider)
{
    const float sliderRadius = (float) (getSliderThumbRadius (slider) - 2);
    float sliderWidth, sliderHeight;
    bool useImg=false;
    
	const File imgSlider(slider.getProperties().getWithDefault("imgslider", "").toString());
    
	if(imgSlider.existsAsFile())
    {		
        if(imgSlider.hasFileExtension("png"))
        {
            g.setOpacity(1.0);
			Image image = ImageCache::getFromFile(imgSlider);
            image = image.rescaled(slider.getWidth(), slider.getHeight());
			if(slider.isHorizontal())
				g.drawImage(image, 0, sliderPos-width*.05, width, height, 0, 0, slider.getWidth(), slider.getHeight(), false);
			else
			{
				sliderHeight = sliderRadius * 1.5f;	
				g.drawImage(image, 0, sliderPos-height*.05, width, sliderHeight, 0, 0, slider.getWidth(), slider.getHeight(), false);
			}
			useImg = true;
        }
        else if(imgSlider.hasFileExtension("svg"))
        {
			if(slider.isVertical())
			{
                sliderHeight = sliderRadius * 1.5f;	
				drawFromSVG(g, imgSlider, 0,  sliderPos-height*.05,  width, sliderHeight, AffineTransform::identity);
			}
			else
			{
				sliderWidth = sliderRadius * 2.0f;	
				drawFromSVG(g, imgSlider, sliderPos-width*.05, 0, sliderWidth, height, AffineTransform::identity);
			}	
			useImg = true;
		}		
    }


    if(!useImg)
    {
        Colour knobColour (LookAndFeelHelpers::createBaseColour (slider.findColour (Slider::thumbColourId),
                           slider.hasKeyboardFocus (false) && slider.isEnabled(),
                           slider.isMouseOverOrDragging() && slider.isEnabled(),
                           slider.isMouseButtonDown() && slider.isEnabled()));

        const float outlineThickness = slider.isEnabled() ? 0.8f : 0.3f;

        if (style == Slider::LinearHorizontal || style == Slider::LinearVertical)
        {
            float kx, ky;
            if (style == Slider::LinearVertical)
            {
                kx = x + width * 0.5f;
                ky = sliderPos;
                sliderWidth = sliderRadius * 2.0f;
                sliderHeight = sliderRadius * 1.5f;
            }
            else
            {
                kx = sliderPos;
                ky = y + height * 0.5f;
                sliderWidth = sliderRadius * 1.5f;
                sliderHeight = sliderRadius * 2.0f;
            }

            drawSphericalThumb(g,
                                       kx - sliderRadius,
                                       ky - sliderRadius,
                                       sliderWidth,
                                       sliderHeight,
                                       knobColour, outlineThickness);
        }
    }
}

void CabbageLookAndFeel2::drawButtonBackground (Graphics& g, Button& button, const Colour& backgroundColour, bool isMouseOverButton, bool isButtonDown)
{
	const int width = button.getWidth();
	const int height = button.getHeight();
	float opacity = 0.1;
	const bool toggleState = button.getToggleState();
	
	File imgButtonOnFile = File(button.getProperties().getWithDefault("imgbuttonon", "").toString());
    File imgButtonOffFile = File(button.getProperties().getWithDefault("imgbuttonoff", "").toString());
	
    if(imgButtonOnFile.existsAsFile() && imgButtonOffFile.existsAsFile())	//if image files exist, draw them..
    {
        if(imgButtonOnFile.hasFileExtension("png") && imgButtonOffFile.hasFileExtension("png"))
        {
            Image image = ImageCache::getFromFile(File(toggleState == true ? imgButtonOnFile : imgButtonOffFile));
            image = image.rescaled(button.getWidth(), button.getHeight());
            g.drawImage(image, 0.0f, 0, button.getWidth(), button.getHeight(), 0, 0, button.getWidth(), button.getHeight(), false);
        }
        else if(imgButtonOnFile.hasFileExtension("svg") && imgButtonOffFile.hasFileExtension("svg"))
        {
            drawFromSVG(g, toggleState == true ? imgButtonOnFile : imgButtonOffFile, 0, 0, button.getWidth(), button.getHeight(), AffineTransform::identity);
        }
    }

    else	//if files don't exist, draw a native Cabbage checkbox
    {
		//----- Outline
		g.setColour (Colour::fromRGBA (10, 10, 10, 255));
		g.fillRoundedRectangle (0, 0, width*0.95, height*0.95, height*0.1);

		//----- If "off"
		if (isButtonDown == false)
		{
			//----- Shadow
			for (float i=0.01; i<0.05; i+=0.01)
			{
				g.setColour (Colour::fromRGBA (0, 0, 0, 255/(i*100)));
				g.fillRoundedRectangle (width*i, height*i,
										width*0.95, height*0.95, height*0.1);
				opacity = 0.3;
			}
		}
		
		//----- Filling in the button
		//Colour bg1 = Colour::fromRGBA (25, 25, 28, 255);
		//Colour bg2 = Colour::fromRGBA (15, 15, 18, 255);
		Colour bg1 = button.findColour(toggleState == true ? TextButton::buttonOnColourId : TextButton::buttonColourId);
		Colour bg2 = bg1.darker();

		ColourGradient cg = ColourGradient (bg1, 0, 0, bg2, width*0.5, height*0.5, false);
		g.setGradientFill (cg);
		g.fillRoundedRectangle (width*0.01, height*0.01, width*0.93, height*0.93, height*0.1);

		//----- For emphasising the top and left edges to give the illusion that light is shining on them
		ColourGradient edgeHighlight = ColourGradient (Colours::whitesmoke, 0, 0,
									   Colours::transparentWhite, 0, height*0.1, false);
		g.setGradientFill (edgeHighlight);
		g.setOpacity (opacity);
		g.fillRoundedRectangle (0, 0, width*0.95, height*0.95, height*0.1);

		ColourGradient edgeHighlight2 = ColourGradient (Colours::whitesmoke, 0, 0,
										Colours::transparentWhite, height*0.1, 0, false);
		g.setGradientFill (edgeHighlight2);
		g.setOpacity (opacity);
		g.fillRoundedRectangle (0, 0, width*0.95, height*0.95, height*0.1);
	}
}
//==========================================================================================================================================
void CabbageLookAndFeel2::drawSphericalThumb (Graphics& g, const float x, const float y,
								const float w, const float h, const Colour& colour,
								const float outlineThickness)
{
	ColourGradient cg = ColourGradient (Colours::white, 0, 0, colour, w/2, h/2, false);
	cg.addColour (0.4, Colours::white.overlaidWith (colour));
	g.setGradientFill (cg);
	g.fillEllipse (x, y, w, h);
	g.setOpacity(.4);
	g.fillEllipse (x+1, y+1, w, h);
}
//====================================================================================================
void CabbageLookAndFeel2::drawGlassPointer (Graphics& g, float x, float y, float diameter,
							  const Colour& colour, float outlineThickness, int direction)
{
	if (diameter <= outlineThickness)
		return;

	Path p;

	p.startNewSubPath (x + diameter * 0.5f, y);
	p.lineTo (x + diameter*.9f, y + diameter * 0.6f);
	//    p.lineTo (x + diameter, y + diameter);
	p.lineTo (diameter*.1f+x, y + diameter*0.6f);
	//    p.lineTo (x, y + diameter * 0.6f);
	p.closeSubPath();

	p.applyTransform(AffineTransform::rotation (direction * (float_Pi * 0.5f), x + diameter * 0.5f, y + diameter * 0.5f));

	{
		ColourGradient cg (Colours::white.overlaidWith (colour.withMultipliedAlpha (0.7f)), 0, y,
						   Colours::white.overlaidWith (colour.withMultipliedAlpha (0.3f)), 0, y + diameter, false);

		cg.addColour (0.4, Colours::white.overlaidWith (colour));

		g.setGradientFill (cg);
		g.fillPath (p);
	}

	ColourGradient cg (Colours::transparentBlack,
					   x + diameter * 0.5f, y + diameter * 0.5f,
					   Colours::black.withAlpha (0.5f * outlineThickness * colour.getFloatAlpha()),
					   x - diameter * 0.2f, y + diameter * 0.5f, true);

	cg.addColour (0.5, Colours::transparentBlack);
	cg.addColour (0.7, Colours::black.withAlpha (0.07f * outlineThickness));

	g.setGradientFill (cg);
	g.fillPath (p);

	g.setColour (Colours::black.withAlpha (0.5f * colour.getFloatAlpha()));
	g.strokePath (p, PathStrokeType (outlineThickness));
}
//if using an SVG..
void CabbageLookAndFeel2::drawFromSVG(Graphics& g, File svgFile, int x, int y, int newWidth, int newHeight, AffineTransform affine)
{
    ScopedPointer<XmlElement> svg (XmlDocument::parse(svgFile.loadFileAsString()));
    if(svg == nullptr)
        jassert(false);

    ScopedPointer<Drawable> drawable;

    if (svg != nullptr)
    {
        drawable = Drawable::createFromSVG (*svg);
        drawable->setTransformToFit(Rectangle<float>(x, y, newWidth, newHeight), RectanglePlacement::stretchToFit);
        drawable->draw(g, 1.f, affine);
    }
}