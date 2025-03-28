/*
 * Copyright (C) 2000 Lars Knoll (knoll@kde.org)
 *           (C) 2000 Antti Koivisto (koivisto@kde.org)
 *           (C) 2000 Dirk Mueller (mueller@kde.org)
 * Copyright (C) 2003, 2004, 2005, 2006, 2007, 2008 Apple Inc. All rights reserved.
 * Copyright (C) 2007 Nicholas Shanks <webkit@nickshanks.com>
 * Copyright (C) 2011 Electronic Arts, Inc. All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIother.m_  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USm_
 *
 */

#ifndef FontDescription_h
#define FontDescription_h

#include "FontFamily.h"
#include "FontFeatureSettings.h"
#include "FontOrientation.h"
#include "FontRenderingMode.h"
#include "FontSmoothingMode.h"
#include "FontTraitsMask.h"
#include "FontWidthVariant.h"
#include "TextOrientation.h"
#include "TextRenderingMode.h"
#include "WebKitFontFamilyNames.h"
#include <wtf/MathExtras.h>

#include <wtf/RefPtr.h>

//+EAWebKitChange
//10/17/2011 - This change is so that we can get more easy access to the CSS effect settings.  
// For EAText, the font effects are considered like a style property of the font itself (like bold or italic) and 
// so we need this info before the draw context.  The info needed is contained in the style but it is unclear how 
// to get it when deep inside some calls (e.g. from deep down FontPlatformDataEA.cpp).
// Injecting the info into the font description works (it is done in CSSStyleSelector) and follows somewhat the pattern of the bold and italic handling but
// would really like to find a way avoid this as it complicates our merges... 
#if PLATFORM(EA)
    #include <EAWebKit/EAWebKitTextInterface.h>
#endif
//-EAWebKitChange

namespace WebCore {

using namespace WebKitFontFamilyNames;

enum FontWeight {
    FontWeight100,
    FontWeight200,
    FontWeight300,
    FontWeight400,
    FontWeight500,
    FontWeight600,
    FontWeight700,
    FontWeight800,
    FontWeight900,
    FontWeightNormal = FontWeight400,
    FontWeightBold = FontWeight700
};

enum FontItalic {
    FontItalicOff = 0,
    FontItalicOn = 1
};

enum FontSmallCaps {
    FontSmallCapsOff = 0,
    FontSmallCapsOn = 1
};

class FontDescription {
public:
    enum GenericFamilyType { NoFamily, StandardFamily, SerifFamily, SansSerifFamily, 
                             MonospaceFamily, CursiveFamily, FantasyFamily, PictographFamily };

    FontDescription()
        : m_specifiedSize(0)
        , m_computedSize(0)
        , m_orientation(Horizontal)
        , m_textOrientation(TextOrientationVerticalRight)
        , m_widthVariant(RegularWidth)
        , m_italic(FontItalicOff)
        , m_smallCaps(FontSmallCapsOff)
        , m_isAbsoluteSize(false)
        , m_weight(FontWeightNormal)
        , m_genericFamily(NoFamily)
        , m_usePrinterFont(false)
        , m_renderingMode(NormalRenderingMode)
        , m_keywordSize(0)
        , m_fontSmoothing(AutoSmoothing)
        , m_textRendering(AutoTextRendering)
        , m_isSpecifiedFont(false)
        , m_script(USCRIPT_COMMON)
//+EAWebKitChange
//10/17/2011
#if PLATFORM(EA)
        , m_effectData()
#endif
//-EAWebKitChange
    {
    }

    bool operator==(const FontDescription&) const;
    bool operator!=(const FontDescription& other) const { return !(*this == other); }
    
    const FontFamily& family() const { return m_familyList; }
    FontFamily& firstFamily() { return m_familyList; }
    float specifiedSize() const { return m_specifiedSize; }
    float computedSize() const { return m_computedSize; }
    FontItalic italic() const { return static_cast<FontItalic>(m_italic); }
    int computedPixelSize() const { return int(m_computedSize + 0.5f); }
    FontSmallCaps smallCaps() const { return static_cast<FontSmallCaps>(m_smallCaps); }
    bool isAbsoluteSize() const { return m_isAbsoluteSize; }
    FontWeight weight() const { return static_cast<FontWeight>(m_weight); }
    FontWeight lighterWeight() const;
    FontWeight bolderWeight() const;
    GenericFamilyType genericFamily() const { return static_cast<GenericFamilyType>(m_genericFamily); }
    bool usePrinterFont() const { return m_usePrinterFont; }
    // only use fixed default size when there is only one font family, and that family is "monospace"
    bool useFixedDefaultSize() const { return genericFamily() == MonospaceFamily && !family().next() && family().family() == monospaceFamily; }
    FontRenderingMode renderingMode() const { return static_cast<FontRenderingMode>(m_renderingMode); }
    unsigned keywordSize() const { return m_keywordSize; }
    FontSmoothingMode fontSmoothing() const { return static_cast<FontSmoothingMode>(m_fontSmoothing); }
    TextRenderingMode textRenderingMode() const { return static_cast<TextRenderingMode>(m_textRendering); }
    UScriptCode script() const { return m_script; }

    FontTraitsMask traitsMask() const;
    bool isSpecifiedFont() const { return m_isSpecifiedFont; }
    FontOrientation orientation() const { return m_orientation; }
    TextOrientation textOrientation() const { return m_textOrientation; }
    FontWidthVariant widthVariant() const { return m_widthVariant; }
    FontFeatureSettings* featureSettings() const { return m_featureSettings.get(); }
    FontDescription makeNormalFeatureSettings() const;

    void setFamily(const FontFamily& family) { m_familyList = family; }
    void setComputedSize(float s) { ASSERT(isfinite(s)); m_computedSize = s; }
    void setSpecifiedSize(float s) { ASSERT(isfinite(s)); m_specifiedSize = s; }
    void setItalic(FontItalic i) { m_italic = i; }
    void setItalic(bool i) { setItalic(i ? FontItalicOn : FontItalicOff); }
    void setSmallCaps(FontSmallCaps c) { m_smallCaps = c; }
    void setSmallCaps(bool c) { setSmallCaps(c ? FontSmallCapsOn : FontSmallCapsOff); }
    void setIsAbsoluteSize(bool s) { m_isAbsoluteSize = s; }
    void setWeight(FontWeight w) { m_weight = w; }
    void setGenericFamily(GenericFamilyType genericFamily) { m_genericFamily = genericFamily; }
#if PLATFORM(CHROMIUM) && OS(DARWIN)
    void setUsePrinterFont(bool) { }
#else
    void setUsePrinterFont(bool p) { m_usePrinterFont = p; }
#endif
    void setRenderingMode(FontRenderingMode mode) { m_renderingMode = mode; }
    void setKeywordSize(unsigned s) { m_keywordSize = s; }
    void setFontSmoothing(FontSmoothingMode smoothing) { m_fontSmoothing = smoothing; }
    void setTextRenderingMode(TextRenderingMode rendering) { m_textRendering = rendering; }
    void setIsSpecifiedFont(bool isSpecifiedFont) { m_isSpecifiedFont = isSpecifiedFont; }
    void setOrientation(FontOrientation orientation) { m_orientation = orientation; }
    void setTextOrientation(TextOrientation textOrientation) { m_textOrientation = textOrientation; }
    void setWidthVariant(FontWidthVariant widthVariant) { m_widthVariant = widthVariant; }
    void setScript(UScriptCode s) { m_script = s; }
    void setFeatureSettings(PassRefPtr<FontFeatureSettings> settings) { m_featureSettings = settings; }
    
//+EAWebKitChange
//10/17/2011
#if PLATFORM(EA)    
    const EA::WebKit::TextEffectData& getTextEffectData() const { return m_effectData; }
    void setEffect(int x, int y, int blur, EA::WebKit::Color c, EA::WebKit::Color cBase, EA::WebKit::Effect type = EA::WebKit::kEffectShadow )
        { m_effectData.mX = x; m_effectData.mY = y; m_effectData.mBlur = blur, m_effectData.mC = c; m_effectData.mCBase = cBase; m_effectData.mType = type; }
#endif
//-EAWebKitChange

private:
    FontFamily m_familyList; // The list of font families to be used.

    float m_specifiedSize;   // Specified CSS value. Independent of rendering issues such as integer
                             // rounding, minimum font sizes, and zooming.
    float m_computedSize;    // Computed size adjusted for the minimum font size and the zoom factor.  

    FontOrientation m_orientation; // Whether the font is rendering on a horizontal line or a vertical line.
    TextOrientation m_textOrientation; // Only used by vertical text. Determines the default orientation for non-ideograph glyphs.

    FontWidthVariant m_widthVariant;

    RefPtr<FontFeatureSettings> m_featureSettings;

    unsigned m_italic : 1; // FontItalic
    unsigned m_smallCaps : 1; // FontSmallCaps
    bool m_isAbsoluteSize : 1;   // Whether or not CSS specified an explicit size
                                 // (logical sizes like "medium" don't count).
    unsigned m_weight : 8; // FontWeight
    unsigned m_genericFamily : 3; // GenericFamilyType
    bool m_usePrinterFont : 1;

    unsigned m_renderingMode : 1;  // Used to switch between CG and GDI text on Windows.

    unsigned m_keywordSize : 4; // We cache whether or not a font is currently represented by a CSS keyword (e.g., medium).  If so,
                           // then we can accurately translate across different generic families to adjust for different preference settings
                           // (e.g., 13px monospace vs. 16px everything else).  Sizes are 1-8 (like the HTML size values for <font>).

    unsigned m_fontSmoothing : 2; // FontSmoothingMode
    unsigned m_textRendering : 2; // TextRenderingMode
    bool m_isSpecifiedFont : 1; // True if a web page specifies a non-generic font family as the first font family.
    UScriptCode m_script; // Used to help choose an appropriate font for generic font families.

//+EAWebKitChange
//10/17/2011
#if PLATFORM(EA)    
        EA::WebKit::TextEffectData  m_effectData;   // So that we can pass down the text effect when we create a font.
#endif
//-EAWebKitChange
};

inline bool FontDescription::operator==(const FontDescription& other) const
{
    return m_familyList == other.m_familyList
        && m_specifiedSize == other.m_specifiedSize
        && m_computedSize == other.m_computedSize
        && m_italic == other.m_italic
        && m_smallCaps == other.m_smallCaps
        && m_isAbsoluteSize == other.m_isAbsoluteSize
        && m_weight == other.m_weight
        && m_genericFamily == other.m_genericFamily
        && m_usePrinterFont == other.m_usePrinterFont
        && m_renderingMode == other.m_renderingMode
        && m_keywordSize == other.m_keywordSize
        && m_fontSmoothing == other.m_fontSmoothing
        && m_textRendering == other.m_textRendering
        && m_isSpecifiedFont == other.m_isSpecifiedFont
        && m_orientation == other.m_orientation
        && m_textOrientation == other.m_textOrientation
        && m_widthVariant == other.m_widthVariant
        && m_script == other.m_script
//+EAWebKitChange
//10/17/2011
#if PLATFORM(EA)
        && m_effectData == other.m_effectData
#endif
//-EAWebKitChange
        && m_featureSettings == other.m_featureSettings;
}

}

#endif
