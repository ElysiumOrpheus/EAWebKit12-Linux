/*
Copyright (C) 2011, 2012, 2013 Electronic Arts, Inc.  All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

1.  Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
2.  Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
3.  Neither the name of Electronic Arts, Inc. ("EA") nor the names of
    its contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY ELECTRONIC ARTS AND ITS CONTRIBUTORS "AS IS" AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL ELECTRONIC ARTS OR ITS CONTRIBUTORS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "config.h"
#include <internal/include/EAWebKitTextWrapper.h>
#include <internal/include/EAWebKit_p.h>
#include EA_ASSERT_HEADER
#include <coreallocator/icoreallocator_interface.h>
#include <EAWebKit/EAWebKit.h>
#include <EAText/EATextCache.h>
#include <EAIO/EAStreamMemory.h>
#include <internal/include/EAWebKitAssert.h>
#include <EAText/EATextOutlineFont.h>
#include <EAText/EATextCache.h>
#include <EAText/EATextCollation.h>
#include <EAText/EATextBreak.h> 

namespace EA
{
    namespace WebKit
    {

//+ Grabbed these following macro/templates EAText (written by Paul Pedriana)
#define EATEXT_WRAPPER_NEW(Class, pAllocator, pName) \
    new ((pAllocator)->Alloc(sizeof(Class), pName, 0, EA_ALIGN_OF(Class), 0)) Class

template <typename T>
inline void delete_object(T* pObject, EA::Allocator::ICoreAllocator* pAllocator)
{
    if (pObject) // As per the C++ standard, deletion of NULL results in a no-op.
    {
        pObject->~T();
        pAllocator->Free(pObject);
    }
}

#define EATEXT_WRAPPER_DELETE(pObject, pAllocator) delete_object(pObject,pAllocator)
//-
        
        
// Small helper function to find allocator used
static EA::Allocator::ICoreAllocator* GetAllocator_Helper(void)
{
    EA::Allocator::ICoreAllocator* pAllocator = EA::Text::GetAllocator();        
    if (!pAllocator)
        pAllocator = EA::Allocator::ICoreAllocator::GetDefaultAllocator();

    return pAllocator;
}

static void ConvertToWebKit_GlyphMetrics(const EA::Text::GlyphMetrics& in, EA::WebKit::GlyphMetrics& out)
{
    memset(&out,0, sizeof(EA::WebKit::GlyphMetrics));  // We fill for it does not have a constructor            
    
    out.mfSizeX = in.mfSizeX;           
    out.mfSizeY = in.mfSizeY;            
    out.mfHBearingX = in.mfHBearingX;        
    out.mfHBearingY = in.mfHBearingY;        
    out.mfHAdvanceX = in.mfHAdvanceX;     
}

static EA::WebKit::Pitch GetWebKit_Pitch(const EA::Text::Pitch pitchEAText)
{
    EA::WebKit::Pitch pitchWebKit;
    switch (pitchEAText)
    {
        case EA::Text::kPitchVariable:
         pitchWebKit = EA::WebKit::kPitchVariable;
            break;
        case EA::Text::kPitchFixed:
           pitchWebKit = EA::WebKit::kPitchFixed;
           break;
        default:    
            EA_ASSERT_MSG(false, "TextWrapper: Warning: Unknown Pitch Value.");
            pitchWebKit = EA::WebKit::kPitchVariable;
            break;
    }
    return pitchWebKit;
}

static EA::Text::Pitch GetEAText_Pitch(const EA::WebKit::Pitch pitchWebKit)
{
    EA::Text::Pitch pitchEAText;
    switch (pitchWebKit)
    {
        case EA::WebKit::kPitchVariable:
         pitchEAText = EA::Text::kPitchVariable;
            break;
        case EA::WebKit::kPitchFixed:
           pitchEAText = EA::Text::kPitchFixed;
           break;
        default:    
            EA_ASSERT_MSG(false, "TextWrapper: Warning: Unknown Pitch Value.");
            pitchEAText = EA::Text::kPitchVariable;
            break;
    }
    return pitchEAText;
}

static void ConvertToWebKit_FontMetrics(const EA::Text::FontMetrics& in, EA::WebKit::FontMetrics& out)
{
    out.mfSize                  = in.mfSize;                  
    out.mPitch                  = GetWebKit_Pitch(in.mPitch);                   
    out.mfHAdvanceXMax          = in.mfHAdvanceXMax;           
    out.mfVAdvanceYMax          = in.mfVAdvanceYMax;           
    out.mfAscent                = in.mfAscent;                 
    out.mfDescent               = in.mfDescent;                
    out.mfLeading               = in.mfLeading;                
    out.mfBaseline              = in.mfBaseline;               
    out.mfLineHeight            = in.mfLineHeight;            
    out.mfXHeight               = in.mfXHeight;                
    out.mfCapsHeight            = in.mfCapsHeight;             
    out.mfUnderlinePosition     = in.mfUnderlinePosition;      
    out.mfUnderlineThickness    = in.mfUnderlineThickness;     
    out.mfLinethroughPosition   = in.mfLinethroughPosition;    
    out.mfLinethroughThickness  = in.mfLinethroughThickness;   
    out.mfOverlinePosition      = in.mfOverlinePosition;       
    out.mfOverlineThickness     = in.mfOverlineThickness;      
};

static void ConvertToWebKit_Kerning(const EA::Text::Kerning& in, EA::WebKit::Kerning& out)
{
    memset(&out,0, sizeof(EA::WebKit::Kerning));  // We fill for it does not have a constructor            
    out.mfKernX = in.mfKernX;
    out.mfKernY = in.mfKernY;
}

static EA::Text::Script GetEAText_Script(const EA::WebKit::Script scriptWebKit)
{
    EA::Text::Script scriptEAText;            
    
    switch (scriptWebKit)
    {
           case EA::WebKit::kScriptUnknown: 
               scriptEAText = EA::Text::kScriptUnknown;
               break;
           case EA::WebKit::kScriptCommon:
               scriptEAText = EA::Text::kScriptCommon;
               break;
           case EA::WebKit::kScriptInherited:
               scriptEAText = EA::Text::kScriptInherited;
               break;
           case EA::WebKit::kScriptStart:
               scriptEAText = EA::Text::kScriptStart;
               break;
           case EA::WebKit::kScriptArmenian:
               scriptEAText = EA::Text::kScriptArmenian;
               break;
           case EA::WebKit::kScriptBengali:
               scriptEAText = EA::Text::kScriptBengali;
               break;
           case EA::WebKit::kScriptBraille:
               scriptEAText = EA::Text::kScriptBraille;
               break;
           case EA::WebKit::kScriptBopomofo:
               scriptEAText = EA::Text::kScriptBopomofo;
               break;
           case EA::WebKit::kScriptBuhid:
               scriptEAText = EA::Text::kScriptBuhid;
               break;
           case EA::WebKit::kScriptCanadianAboriginal:
               scriptEAText = EA::Text::kScriptCanadianAboriginal;
               break;
           case EA::WebKit::kScriptCherokee:
               scriptEAText = EA::Text::kScriptCherokee;
               break;
           case EA::WebKit::kScriptCoptic:
               scriptEAText = EA::Text::kScriptCoptic;
               break;
           case EA::WebKit::kScriptCypriot:
               scriptEAText = EA::Text::kScriptCypriot;
               break;
           case EA::WebKit::kScriptCyrillic:
               scriptEAText = EA::Text::kScriptCyrillic;
               break;
           case EA::WebKit::kScriptDeseret:
               scriptEAText = EA::Text::kScriptDeseret;
               break;
           case EA::WebKit::kScriptDevanagari:
               scriptEAText = EA::Text::kScriptDevanagari;
               break;
           case EA::WebKit::kScriptEthiopic:
               scriptEAText = EA::Text::kScriptEthiopic;
               break;
           case EA::WebKit::kScriptGeorgian:
               scriptEAText = EA::Text::kScriptGeorgian;
               break;
           case EA::WebKit::kScriptGothic:
               scriptEAText = EA::Text::kScriptGothic;
               break;
           case EA::WebKit::kScriptGreek:
               scriptEAText = EA::Text::kScriptGreek;
               break;
           case EA::WebKit::kScriptGujarati:
               scriptEAText = EA::Text::kScriptGujarati;
               break;
           case EA::WebKit::kScriptGurmukhi:
               scriptEAText = EA::Text::kScriptGurmukhi;
               break;
           case EA::WebKit::kScriptHan:
               scriptEAText = EA::Text::kScriptHan;
               break;
           case EA::WebKit::kScriptHangul:
               scriptEAText = EA::Text::kScriptHangul;
               break;
           case EA::WebKit::kScriptHanunoo:
               scriptEAText = EA::Text::kScriptHanunoo; 
               break;
           case EA::WebKit::kScriptHebrew:
               scriptEAText = EA::Text::kScriptHebrew;
               break;
           case EA::WebKit::kScriptHiragana:
               scriptEAText = EA::Text::kScriptHiragana;
               break;
           case EA::WebKit::kScriptKannada:
               scriptEAText = EA::Text::kScriptKannada;
               break;
           case EA::WebKit::kScriptKatakana:
               scriptEAText = EA::Text::kScriptKatakana;
               break;
           case EA::WebKit::kScriptKhmer:
               scriptEAText = EA::Text::kScriptKhmer;
               break;
           case EA::WebKit::kScriptLao:
               scriptEAText = EA::Text::kScriptLao;
               break;
           case EA::WebKit::kScriptLatin:
               scriptEAText = EA::Text::kScriptLatin;
               break;
           case EA::WebKit::kScriptLimbu:
               scriptEAText = EA::Text::kScriptLimbu;
               break;
           case EA::WebKit::kScriptLinearB:
               scriptEAText = EA::Text::kScriptLinearB;
               break;
           case EA::WebKit::kScriptMalayalam:
               scriptEAText = EA::Text::kScriptMalayalam;
               break;
           case EA::WebKit::kScriptMongolian:
               scriptEAText = EA::Text::kScriptMongolian;
               break;
           case EA::WebKit::kScriptMyanmar:
               scriptEAText = EA::Text::kScriptMyanmar;
               break;
           case EA::WebKit::kScriptOgham:
               scriptEAText = EA::Text::kScriptOgham;
               break;
           case EA::WebKit::kScriptOldItalic:
               scriptEAText = EA::Text::kScriptOldItalic;
               break;
           case EA::WebKit::kScriptOriya:
               scriptEAText = EA::Text::kScriptOriya;
               break;
           case EA::WebKit::kScriptOsmanya:
               scriptEAText = EA::Text::kScriptOsmanya;
               break;
           case EA::WebKit::kScriptRunic:
               scriptEAText = EA::Text::kScriptRunic;
               break;
           case EA::WebKit::kScriptShavian:
               scriptEAText = EA::Text::kScriptShavian;
               break;
           case EA::WebKit::kScriptSinhala:
               scriptEAText = EA::Text::kScriptSinhala;
               break;
           case EA::WebKit::kScriptSyriac:
               scriptEAText = EA::Text::kScriptSyriac;
               break;
           case EA::WebKit::kScriptTagalog:
               scriptEAText = EA::Text::kScriptTagalog;
               break;
           case EA::WebKit::kScriptTagbanwa:
               scriptEAText = EA::Text::kScriptTagbanwa;
               break;
           case EA::WebKit::kScriptTaiLe:
               scriptEAText = EA::Text::kScriptTaiLe;
               break;
           case EA::WebKit::kScriptTamil:
               scriptEAText = EA::Text::kScriptTamil;
               break;
           case EA::WebKit::kScriptTelugu:
               scriptEAText = EA::Text::kScriptTelugu;
               break;
           case EA::WebKit::kScriptThaana:
               scriptEAText = EA::Text::kScriptThaana;
               break;
           case EA::WebKit::kScriptThai:
               scriptEAText = EA::Text::kScriptThai;
               break;
           case EA::WebKit::kScriptTibetan:
               scriptEAText = EA::Text::kScriptTibetan;
               break;
           case EA::WebKit::kScriptUgaritic:
               scriptEAText = EA::Text::kScriptUgaritic;
               break;
           case EA::WebKit::kScriptYi:
               scriptEAText = EA::Text::kScriptYi;
               break;
           default:
               EA_ASSERT_MSG(false, "TextWrapper: Warning: Unknown Script Value.");
               scriptEAText = EA::Text::kScriptUnknown;
               break;
    }    
    return  scriptEAText;
}

static EA::Text::FontType GetEAText_FontType(const EA::WebKit::FontType fontTypeWebKit)
{
    EA::Text::FontType fontTypeEAText;        
    switch (fontTypeWebKit)
    {
        case EA::WebKit::kFontTypeUnknown:
            fontTypeEAText = EA::Text::kFontTypeUnknown;
            break;
        case EA::WebKit::kFontTypeBitmap:
            fontTypeEAText = EA::Text::kFontTypeBitmap;
            break;
        case EA::WebKit::kFontTypeOutline:
            fontTypeEAText = EA::Text::kFontTypeOutline;
            break;
        case EA::WebKit::kFontTypeStroke:
            fontTypeEAText = EA::Text::kFontTypeStroke;
            break;
        case EA::WebKit::kFontTypePolygon:
            fontTypeEAText = EA::Text::kFontTypePolygon;
            break;
        default:
            EA_ASSERT_MSG(false, "TextWrapper: Warning: Unknown Font Type");
            fontTypeEAText = EA::Text::kFontTypeUnknown;
            break;
    }
    return fontTypeEAText;
}

static EA::WebKit::BidiClass GetWebKit_BidiClass(const EA::Text::BidiClass typeEAText)
{
    EA::WebKit::BidiClass typeWebKit;        
    switch (typeEAText)
    {
        case EA::Text::kBidiClassON:
             typeWebKit = EA::WebKit::kBidiClassON; 
             break;
        case EA::Text::kBidiClassL:
             typeWebKit = EA::WebKit::kBidiClassL; 
             break;
        case EA::Text::kBidiClassR:
             typeWebKit = EA::WebKit::kBidiClassR; 
             break;
        case EA::Text::kBidiClassAN:
             typeWebKit = EA::WebKit::kBidiClassAN;
             break;
        case EA::Text::kBidiClassEN:
             typeWebKit = EA::WebKit::kBidiClassEN;
             break;
        case EA::Text::kBidiClassAL:
             typeWebKit = EA::WebKit::kBidiClassAL;
             break;
        case EA::Text::kBidiClassNSM:
             typeWebKit = EA::WebKit::kBidiClassNSM;
             break;
        case EA::Text::kBidiClassCS:
             typeWebKit = EA::WebKit::kBidiClassCS;
             break;
        case EA::Text::kBidiClassES:
             typeWebKit = EA::WebKit::kBidiClassES;
             break;
        case EA::Text::kBidiClassET:
             typeWebKit = EA::WebKit::kBidiClassET;
             break;
        case EA::Text::kBidiClassBN:
             typeWebKit = EA::WebKit::kBidiClassBN;
             break;
        case EA::Text::kBidiClassS:
             typeWebKit = EA::WebKit::kBidiClassS;
             break;
        case EA::Text::kBidiClassWS:
             typeWebKit = EA::WebKit::kBidiClassWS;
             break;
        case EA::Text::kBidiClassB:
             typeWebKit = EA::WebKit::kBidiClassB; 
             break;
        case EA::Text::kBidiClassRLO:
             typeWebKit = EA::WebKit::kBidiClassRLO;
             break;
        case EA::Text::kBidiClassRLE:
             typeWebKit = EA::WebKit::kBidiClassRLE;
             break;
        case EA::Text::kBidiClassLRO:
             typeWebKit = EA::WebKit::kBidiClassLRO;
             break;
        case EA::Text::kBidiClassLRE:
             typeWebKit = EA::WebKit::kBidiClassLRE;
             break;
        case EA::Text::kBidiClassPDF:
            typeWebKit = EA::WebKit::kBidiClassPDF;
             break;
        default:
            EA_ASSERT_MSG(false, "TextWrapper: Warning: Unknown BidClass Type");
            typeWebKit = EA::WebKit::kBidiClassON; 
            break;
    }
    return typeWebKit;
}

static EA::Text::Smooth GetEAText_Smooth(const EA::WebKit::Smooth typeWebKit)
{
    EA::Text::Smooth typeEAText;        
    switch (typeWebKit)
    {
        case EA::WebKit::kSmoothNone:  
            typeEAText = EA::Text::kSmoothNone;
            break;
        case EA::WebKit::kSmoothEnabled:
            typeEAText = EA::Text::kSmoothEnabled;
            break;
        default:
            EA_ASSERT_MSG(false, "TextWrapper: Warning: Unknown Smooth Type"); 
            typeEAText = EA::Text::kSmoothNone;
            break;
    }
    return typeEAText;
}

static EA::Text::Variant GetEAText_Variant(const EA::WebKit::Variant typeWebKit)
{
    EA::Text::Variant typeEAText;        
    switch (typeWebKit)
    {
         case EA::WebKit::kVariantNormal:
            typeEAText = EA::Text::kVariantNormal;
            break;
         case EA::WebKit::kVariantSmallCaps:
            typeEAText = EA::Text::kVariantSmallCaps;
            break;
        default:
             EA_ASSERT_MSG(false, "TextWrapper: Warning: Unknown Variant Type"); 
            typeEAText = EA::Text::kVariantNormal;
            break;
    }
    return typeEAText;
}

static EA::Text::Style GetEAText_Style(const EA::WebKit::Style typeWebKit)
{
    EA::Text::Style typeEAText;        
    switch (typeWebKit)
    {
        case EA::WebKit::kStyleNormal:
            typeEAText = EA::Text::kStyleNormal;
            break;
        case EA::WebKit::kStyleOblique:
            typeEAText = EA::Text::kStyleOblique;
            break;
        case EA::WebKit::kStyleItalic:
            typeEAText = EA::Text::kStyleItalic;
            break;
        default:
             EA_ASSERT_MSG(false, "TextWrapper: Warning: Unknown Style Type"); 
            typeEAText = EA::Text::kStyleNormal;
            break;
    }
    return typeEAText;
}

static uint32_t GetEAText_Effect(uint32_t type)
{
    EA::Text::Effect typeEAText;        
    EA::WebKit::Effect typeWebKit = static_cast<EA::WebKit::Effect>(type);
    switch (typeWebKit)
    {
        case EA::WebKit::kEffectNone:
            typeEAText = EA::Text::kEffectNone;
            break;
        case EA::WebKit::kEffectOutline:
            typeEAText = EA::Text::kEffectOutline;
            break;
        case EA::WebKit::kEffectShadow:
            typeEAText = EA::Text::kEffectShadow;
            break;
        case EA::WebKit::kEffectRaised:
            typeEAText = EA::Text::kEffectRaised;
            break;
        case EA::WebKit::kEffectDepressed:
            typeEAText = EA::Text::kEffectDepressed;
            break;
        case EA::WebKit::kEffectUser:    
            typeEAText = EA::Text::kEffectUser;
            break;
        default:
            EA_ASSERT_MSG(typeWebKit > EA::WebKit::kEffectUser, "TextWrapper: Warning: Unknown Effect Type"); 
            // Possible user effects to be converted back
            typeEAText = static_cast<EA::Text::Effect> (typeWebKit); 
            break;   
    }
    return (static_cast<uint32_t> (typeEAText));
}

static inline EA::Text::Char GetEAText_Char(EA::WebKit::Char c)
{
    // This is just a typdef char16_t 
    return static_cast<EA::Text::Char> (c);
}

static inline EA::WebKit::Char GetWebKit_Char(EA::Text::Char c)
{
    // This is just a typdef char16_t 
    return static_cast<EA::WebKit::Char> (c);
}

static inline EA::WebKit::Color GetWebKitColor(EA::Text::Color color)
{
    // This is just a typedef  typedef uint32_t Color;
    return static_cast<EA::WebKit::Color> (color);
}

static inline EA::Text::Color GetEATextColor(EA::WebKit::Color color)
{
    // This is just a typedef  typedef uint32_t Color;
    return static_cast<EA::Text::Color> (color);
}

static inline EA::Text::GlyphId GetEAText_GlyphId(EA::WebKit::GlyphId glyphId)
{
    // This is just a  typedef uint16_t GlyphId;
    return static_cast<EA::Text::GlyphId> (glyphId);
}

static void ConvertToEAText_TextStyle(const EA::WebKit::TextStyle& in, EA::Text::TextStyle& out)
{
	uint32_t iMax = eastl::min_alt(EA::Text::kFamilyNameArrayCapacity, EA::WebKit::kFamilyNameArrayCapacity);
	uint32_t jMax = eastl::min_alt(EA::Text::kFamilyNameCapacity, EA::WebKit::kFamilyNameCapacity);

	for (uint32_t i = 0; i < iMax; ++i)
	{
		for (uint32_t j = 0; j < jMax; ++j)
		{
			out.mFamilyNameArray[i][j] =  GetEAText_Char(in.mFamilyNameArray[i][j]);
		}
	}

	// Seems the EAText version does not copy everything over in the default copy as not all info is needed to match a font but 
	// better be safe in case it ends up being used differently.
	/*
	for(uint32_t i=0; i < EA::Text::kFamilyNameArrayCapacity; ++i)
    {
         if(i >= EA::WebKit::kFamilyNameArrayCapacity)
                break;
        for(uint32_t j=0; j < EA::Text::kFamilyNameCapacity; ++j)
        {
            if(j >= EA::WebKit::kFamilyNameCapacity)
                break;
            out.mFamilyNameArray[i][j] =  GetEAText_Char(in.mFamilyNameArray[i][j]);
        }
    }
	*/
    out.mfSize              = in.mfSize;     
    out.mStyle              = GetEAText_Style(in.mStyle);                       
    out.mfWeight            = in.mfWeight;
    out.mPitch              = GetEAText_Pitch(in.mPitch);  
    out.mVariant            = GetEAText_Variant(in.mVariant);
    out.mSmooth             = GetEAText_Smooth(in.mSmooth);
    out.mfEffectX           = static_cast<float>(in.mTextEffectData.mX);
    out.mfEffectY           = static_cast<float>(in.mTextEffectData.mY);
    out.mEffectBaseColor    = GetEATextColor(in.mTextEffectData.mCBase);
    out.mEffectColor        = GetEATextColor(in.mTextEffectData.mC);
    //out.mHighLightColor     = GetExteranlColor(in.mHighLightColor);
    out.mfLetterSpacing     = in.mfLetterSpacing;
    out.mfWordSpacing       = in.mfWordSpacing;
}

static EA::WebKit::CharCategory GetWebKit_CharCategory(const EA::Text::CharCategory typeEAText)
{
    EA::WebKit::CharCategory typeWebKit;        
    switch (typeEAText)
    {
        case EA::Text::kCCUnassigned:
        //case EA::Text::kCCGeneralOtherTypes:
            typeWebKit = EA::WebKit::kCCGeneralOtherTypes;      
            break;
        case EA::Text::kCCControlChar:
            typeWebKit = EA::WebKit::kCCControlChar;            
            break;
        case EA::Text::kCCFormatChar:
            typeWebKit = EA::WebKit::kCCFormatChar;             
            break;
        case EA::Text::kCCPrivateUseChar:
            typeWebKit = EA::WebKit::kCCPrivateUseChar;         
            break;
        case EA::Text::kCCSurrogate:
            typeWebKit = EA::WebKit::kCCSurrogate;              
            break;
        case EA::Text::kCCUppercaseLetter:
            typeWebKit = EA::WebKit::kCCUppercaseLetter;        
            break;
        case EA::Text::kCCLowercaseLetter:
            typeWebKit = EA::WebKit::kCCLowercaseLetter;        
            break;
        case EA::Text::kCCTitlecaseLetter:
            typeWebKit = EA::WebKit::kCCTitlecaseLetter;        
            break;
        case EA::Text::kCCModifierLetter:
            typeWebKit = EA::WebKit::kCCModifierLetter;         
            break;
        case EA::Text::kCCOtherLetter:
            typeWebKit = EA::WebKit::kCCOtherLetter;            
            break;
        case EA::Text::kCCNonSpacingMark:
            typeWebKit = EA::WebKit::kCCNonSpacingMark;         
            break;
        case EA::Text::kCCEnclosingMark:
            typeWebKit = EA::WebKit::kCCEnclosingMark;          
            break;
        case EA::Text::kCCCombiningSpacingMark:
            typeWebKit = EA::WebKit::kCCCombiningSpacingMark;   
            break;
        case EA::Text::kCCDecimalDigitNumber:
            typeWebKit = EA::WebKit::kCCDecimalDigitNumber;     
            break;
        case EA::Text::kCCLetterNumber:
            typeWebKit = EA::WebKit::kCCLetterNumber;           
            break;
        case EA::Text::kCCOtherNumber:
            typeWebKit = EA::WebKit::kCCOtherNumber;            
            break;
        case EA::Text::kCCSpaceSeparator:
            typeWebKit = EA::WebKit::kCCSpaceSeparator;         
            break;
        case EA::Text::kCCLineSeparator:
            typeWebKit = EA::WebKit::kCCLineSeparator;          
            break;
        case EA::Text::kCCParagraphSeparator:
            typeWebKit = EA::WebKit::kCCParagraphSeparator;     
            break;
        case EA::Text::kCCDashPunctuation:
            typeWebKit = EA::WebKit::kCCDashPunctuation;        
            break;
        case EA::Text::kCCStartPunctuation:
            typeWebKit = EA::WebKit::kCCStartPunctuation;       
            break;
        case EA::Text::kCCEndPunctuation:
            typeWebKit = EA::WebKit::kCCEndPunctuation;         
            break;
        case EA::Text::kCCConnectorPunctuation:
            typeWebKit = EA::WebKit::kCCConnectorPunctuation;   
            break;
        case EA::Text::kCCOtherPunctuation:
            typeWebKit = EA::WebKit::kCCOtherPunctuation;       
            break;
        case EA::Text::kCCInitialPunctuation:
            typeWebKit = EA::WebKit::kCCInitialPunctuation;     
            break;
        case EA::Text::kCCFinalPunctuation:
            typeWebKit = EA::WebKit::kCCFinalPunctuation;       
            break;
#if EATEXT_VERSION_N >=10800
		case EA::Text::kCCMathSymbol:
#else
		case EA::Text::kCCMathSuymbol:
#endif
            typeWebKit = EA::WebKit::kCCMathSymbol;            
            break;
        case EA::Text::kCCCurrencySymbol:
            typeWebKit = EA::WebKit::kCCCurrencySymbol;         
            break;
        case EA::Text::kCCModifierSymbol:
            typeWebKit = EA::WebKit::kCCModifierSymbol;         
            break;
        case EA::Text::kCCOtherSymbol:
            typeWebKit = EA::WebKit::kCCOtherSymbol;            
            break;
        default:
             EA_ASSERT_MSG(false, "TextWrapper: Warning: Unknown CharCategory Type"); 
            typeWebKit = EA::WebKit::kCCUnassigned;             
            break;
    }
    return typeWebKit;
}

static EA::WebKit::BitmapFormat GetWebKit_TextureFormat(const uint32_t typeEAText)
{
    EA::WebKit::BitmapFormat out = EA::WebKit::kBFNone; 
    switch (typeEAText)
    {
        case EA::Text::kTextureFormat1Bpp:
            out = EA::WebKit::kBFMonochrome;
            break;
        case EA::Text::kTextureFormat8Bpp:
            out = EA::WebKit::kBFGrayscale;
            break;
        case EA::Text::kTextureFormatARGB: 
            
                 out = EA::WebKit::kBFARGB;
            break;
        case EA::Text::kTextureFormatRGBA: 
            out = EA::WebKit::kBFRGBA;
            break;
        case EA::Text::kTextureFormatDXT3:
            out = EA::WebKit::kBFDXT3;
            break;

        case EA::Text::kTextureFormatNone:
        default:
            out = EA::WebKit::kBFNone;
            break;
    }
    return out;
}

static void ConvertToWebKit_GlyphTextureInfo(const EA::Text::GlyphTextureInfo &in, EA::WebKit::GlyphDrawInfo &out)
{
    if (in.mpTextureInfo)
    {
        out.mpData  = in.mpTextureInfo->mpData;    
        out.mSize   = in.mpTextureInfo->mnSize;
        out.mStride = in.mpTextureInfo->mnStride;
        out.mFormat = GetWebKit_TextureFormat(in.mpTextureInfo->mFormat);
    }
    else
    {
        out.mpData = 0;
    }
    out.mU0 = in.mX1;
    out.mV0 = in.mY1;
    out.mU1 = in.mX2;
    out.mV1 = in.mY2;
}


FontImpl::FontImpl(EA::WebKit::TextSystem* pTextSystem, EA::Text::Font* pFont)
	: mpTextSystem(pTextSystem)
	, mpFont(pFont)
    , mRefCount(1)
{
	mpTextSystem->AddFont(this);
} 

FontImpl::~FontImpl(void)
{
    EA_ASSERT(!mpFont);
	mpTextSystem->RemoveFont(this);
}

int FontImpl::AddRef(void)
{
	mRefCount++; 

	EA_ASSERT(mpFont);
	if (mpFont)
		mpFont->AddRef(); 

	return mRefCount;
}

int FontImpl::Release(void)
{
	mRefCount--;
    int fontCountEA = 0;
	EA_ASSERT(mpFont);
	if (mpFont) 
		 fontCountEA = mpFont->Release();

	if (mRefCount > 0)
		return mRefCount;

    // Remove any cached texture infos for this font if nothing else is using it.
    if (!fontCountEA)
        mpTextSystem->RemoveTextureInfoFromGlyphCache(mpFont);
	mpFont = 0;
    EA::Allocator::ICoreAllocator* pAllocator = GetAllocator_Helper();                
	EATEXT_WRAPPER_DELETE(this, pAllocator);    

	return 0;
}

bool FontImpl::GetFontMetrics(EA::WebKit::FontMetrics& fontMetricsWebKit)
{
    EA_ASSERT(mpFont);
    bool returnFlag = false;
    if (mpFont)
    {
        EA::Text::FontMetrics fontMetricsEAText;
        returnFlag = mpFont->GetFontMetrics(fontMetricsEAText);
        ConvertToWebKit_FontMetrics(fontMetricsEAText, fontMetricsWebKit);
    }
    return returnFlag;
}

bool FontImpl::GetGlyphMetrics(EA::WebKit::GlyphId glyphId, EA::WebKit::GlyphMetrics& glyphMetricsWebKit)
{
    EA_ASSERT(mpFont);
    bool returnFlag = false;
    if (mpFont)
    {
        // 8/23/10 - This it to fix a possible acid 3 crash. 
        // If the transform was not set, it can crash EAText in certain cases, depending on its configuration.
        // Seems if a font is created by type (outline) and then opened, it can bypass the transform setting which is 
        // normally done when using FontServer::CreateNewFont(FaceSource* pFaceSource, const TextStyle& ssCSS, bool bManaged) in EAText.
        // So if no size is found, we assume it has not been set up and init the transforms with a default size ourselves.
        float size = mpFont->GetSize();
        const float kDefaultFontSize = 10.0f;
        if(size == 0.0f)
            mpFont->SetTransform(kDefaultFontSize);

        EA::Text::GlyphMetrics glyphMetricsEAText;
        EA::Text::GlyphId glyphIdEAText = GetEAText_GlyphId(glyphId);
        returnFlag = mpFont->GetGlyphMetrics(glyphIdEAText, glyphMetricsEAText);
        ConvertToWebKit_GlyphMetrics(glyphMetricsEAText, glyphMetricsWebKit);
    }
    return returnFlag;
}

uint32_t FontImpl::GetGlyphIds(const EA::WebKit::Char* pCharArray, uint32_t nCharArrayCount, EA::WebKit::GlyphId* pGlyphIdArray, 
                             bool bUseReplacementGlyph, const uint32_t nGlyphIdStride, bool bWriteInvalidGlyphs)
{
    EA_ASSERT(mpFont);
    int returnValue = 0;
    if (mpFont)
    {
        // Seems that in all EAWebKit use cases, only 1 count is used so casting the typdef is probably less dangerous here than it appears.
        // We also runtime compile check to make sure the sizes match
        returnValue = mpFont->GetGlyphIds( static_cast<const EA::Text::Char*> (pCharArray), nCharArrayCount, static_cast<EA::Text::GlyphId*> (pGlyphIdArray),
                                           bUseReplacementGlyph, nGlyphIdStride, bWriteInvalidGlyphs);
    }
    return returnValue;
}

bool FontImpl::IsCharSupported(EA::WebKit::Char c, EA::WebKit::Script scriptWebKit)
{
    EA_ASSERT(mpFont);
    bool returnFlag = false;
    if (mpFont)
    {
        EA::Text::Script scriptEAText = GetEAText_Script(scriptWebKit);
        EA::Text::Char cEAText = GetEAText_Char(c);
        returnFlag = mpFont->IsCharSupported( cEAText, scriptEAText);
    }
    return returnFlag;
}

float FontImpl::GetSize(void) const
{
    EA_ASSERT(mpFont);
    float returnValue = 0.0f;
    if(mpFont) 
        returnValue =  mpFont->GetSize();

    return returnValue;
}

bool FontImpl::IsItalic(void)
{
    EA_ASSERT(mpFont);
    if (mpFont) 
    {
        EA::Text::FontDescription fd; 
        if (mpFont->GetFontDescription(fd))    
            return fd.mStyle == EA::Text::kStyleItalic; 
    }
    return false;        
}

bool FontImpl::IsSmallCaps(void)
{
    EA_ASSERT(mpFont);
    if (mpFont) 
    {
        EA::Text::FontDescription fd; 
        if (mpFont->GetFontDescription(fd))    
            return fd.mVariant == EA::Text::kVariantSmallCaps;
    }
    return false;
}

bool FontImpl::GetKerning(EA::WebKit::GlyphId g1, EA::WebKit::GlyphId g2, EA::WebKit::Kerning& kerningWebKit, int direction, bool bHorizontalLayout)
{
    EA_ASSERT(mpFont);
    bool returnFlag = false;
    if (mpFont)
    {
        EA::Text::Kerning kerningEAText;
        
        EA::Text::GlyphId g1EAText = GetEAText_GlyphId(g1);
        EA::Text::GlyphId g2EAText = GetEAText_GlyphId(g2);
        
        returnFlag = mpFont->GetKerning(g1EAText, g2EAText, kerningEAText, direction, bHorizontalLayout);
        ConvertToWebKit_Kerning(kerningEAText, kerningWebKit);
    }
    return returnFlag;
}

bool FontImpl::SetSize(float fSize)
{
    EA_ASSERT(mpFont);
    return mpFont->SetTransform(fSize);
}

void FontImpl::SetSmoothing(EA::WebKit::Smooth type)
{
    EA_ASSERT(mpFont);
    
    if (mpFont->GetFontType() == EA::Text::kFontTypeOutline)
    {
        EA::Text::OutlineFont* pOutlineFont = static_cast<EA::Text::OutlineFont*> (mpFont);         
        EA::Text::Smooth smooth = GetEAText_Smooth(type);
        pOutlineFont->SetSmoothing(smooth);
    }
}

bool FontImpl::HasEffect(void)
{
    EA::Text::FontDescription fontDescription;
    mpFont->GetFontDescription(fontDescription);
    const bool hasEffect = fontDescription.mEffect == EA::Text::kEffectNone ? false : true;
    return hasEffect;
}

void FontImpl::BeginDraw(void)
{
    const bool textHasEffect = HasEffect();
    mpTextSystem->SetUpDrawGlyphCacheType(textHasEffect);
}

void FontImpl::EndDraw(void)
{
    mpTextSystem->SetUpDrawGlyphCacheType();
}

void FontImpl::SetEffectData(const EA::WebKit::TextEffectData& effectData)
{
    EA::Text::FontDescription fontDescription;
    mpFont->GetFontDescription(fontDescription);
    if ((fontDescription.mEffect != EA::Text::kEffectNone) || (effectData.mType == EA::WebKit::kEffectNone) || (mpFont->GetFontType() != EA::Text::kFontTypeOutline))
        return;

    // Find if we need an fx script (like for shadows).
    uint32_t effectID =  GetEAText_Effect(effectData.mType); 
    if (effectID == EA::Text::kEffectShadow)
        effectID = mpTextSystem->GetShadowEffectID();

    EA::Text::OutlineFont* pOutlineFont = static_cast<EA::Text::OutlineFont*> (mpFont);   
    pOutlineFont->SetEffect(effectID, static_cast<float>(effectData.mX), static_cast<float>(effectData.mY),
                        GetEATextColor(effectData.mCBase), GetEATextColor(effectData.mC));  
}



// Text System Wrapper

// Deprecated...
TextSystem::TextSystem(EA::Text::FontServer* pFontServer, EA::Text::GlyphCache* pGlyphCache)
    : mpFontServer(pFontServer)
    , mpGlyphCache(pGlyphCache)
    , mpGlyphEffectsCache(NULL)
    , mOwnFontServer(false)    
    , mOwnGlyphCache(false)
    , mpCurrentDrawGlyphCache(NULL)
{
    // Create our own server here if 
    // EAWebKitTODO: add...
    
    EA_ASSERT(mpFontServer);
    EA_ASSERT(mpGlyphCache);
}

TextSystem::TextSystem(EA::Text::FontServer* pFontServer, EA::Text::GlyphCache* pGlyphCache, EA::Text::GlyphCache* pGlyphEffectsCache)
    : mpFontServer(pFontServer)
    , mpGlyphCache(pGlyphCache)
    , mpGlyphEffectsCache(pGlyphEffectsCache)
    , mOwnFontServer(false)    
    , mOwnGlyphCache(false)
    , mUserShadowEffectID(0)
    , mpCurrentDrawGlyphCache(NULL)
{
    // Create our own server here if 
    // EAWebKitTODO: add...
    
    EA_ASSERT(mpFontServer);
    EA_ASSERT(mpGlyphCache);
}

TextSystem::~TextSystem()
{

}

bool TextSystem::Init(void)
{
	EA_ASSERT_MSG(!mActiveFonts.size(), "Active fonts list should be empty at the beginning");
	return !mActiveFonts.size();
}

bool TextSystem::Shutdown(void)
{
	eastl::list<EA::WebKit::FontImpl*, EA::WebKit::EASTLAllocator> currentlyActiveFonts = mActiveFonts;
	eastl::list<EA::WebKit::FontImpl*, EA::WebKit::EASTLAllocator>::iterator iter = currentlyActiveFonts.begin();
	while (iter != currentlyActiveFonts.end())
	{
		EA::WebKit::FontImpl* pFontImpl = *iter;
		// Release all the refs.
		while (pFontImpl->Release())
		{

		}
		++iter;
	}

	EA_ASSERT_MSG(!mActiveFonts.size(), "Active fonts list should be empty at the end");
	return !mActiveFonts.size();
}

EA::WebKit::IFont* TextSystem::GetFont(const EA::WebKit::TextStyle& textStyle, EA::WebKit::Char c)
{
	EA::WebKit::IFont* pIFont = NULL;
	if (mpFontServer)
	{
		EA::Text::TextStyle textStyleEA;     
		ConvertToEAText_TextStyle(textStyle, textStyleEA);
		EA::Text::Char sampleCharEA = GetEAText_Char(c);
        
         EA::Text::Font* pFont = mpFontServer->GetFont(&textStyleEA, NULL, 0, sampleCharEA);
		if (pFont)
			pIFont = CreateFontImpl(pFont);
	}
	return  pIFont; 
}

FontImpl* TextSystem::CreateFontImpl(EA::Text::Font* pFont)
{ 
	if (!pFont)
		return 0;     

    EA::Allocator::ICoreAllocator* pAllocator = GetAllocator_Helper();   
	
	/* Debug code to see which Font is leaking.
	EA::Text::FontDescription fd;
	pFont->GetFontDescription(fd);
	char8_t tag[64];
	memset(tag, 0, sizeof(tag));
	int i = 0;
 	for(; i<EA::Text::kFamilyNameCapacity; ++i)
	{
		if(fd.mFamily[i])
			tag[i] = fd.mFamily[i];
		else
			break;
	}
	sprintf(&tag[i],"FontImplSize-%f",fd.mfSize);
	*/

	FontImpl* pFontImpl = EATEXT_WRAPPER_NEW(FontImpl, pAllocator, "FontImpl") (this, pFont);
	EA_ASSERT(pFontImpl);                
	return pFontImpl;
}

EA::WebKit::IFont* TextSystem::CreateNewFont(EA::WebKit::FontType fontTypeWebKit, const void* pSourceData, uint32_t nSourceSize, int nFaceIndex)
{
    EA::WebKit::IFont* pIFont = NULL;
	if (mpFontServer)
	{   
		EA::Text::FontType fontTypeEAText = GetEAText_FontType(fontTypeWebKit);
		EA::Text::Font* pFont = mpFontServer->CreateNewFont(fontTypeEAText);
		pIFont = CreateFontImpl(pFont);

		bool success = false;
		switch (fontTypeEAText)
		{
		    case EA::Text::kFontTypeOutline:
			{
				EA::Text::OutlineFont* pOutlineFont = static_cast<EA::Text::OutlineFont*> (pFont);                  
                success = pOutlineFont->Open(pSourceData, nSourceSize, nFaceIndex);
			}
			break;
		    
            default:
			EA_ASSERT_MSG(false, "Support other than outline fonts not enabled yet");
			break;
		}

		if (!success)
		{
			pIFont->Release();
			pIFont = NULL;
		}
	}

	return pIFont;
}

void TextSystem::FlushGlyphCache(void)
{
    EA::Text::GlyphCache* pCache = mpCurrentDrawGlyphCache ? mpCurrentDrawGlyphCache : mpGlyphCache;               

    // Flush each page.
    const uint32_t count = pCache->GetTextureInfoCount(); 
    for (int i = 0; i < count; ++i)
    {
        EA::Text::TextureInfo* pInfo =  pCache->GetTextureInfo(i);
        if (pInfo)
            pCache->ClearTexture(pInfo);
    }
}

bool TextSystem::FillGlyphDrawInfo(EA::WebKit::IFont* pFont, const EA::WebKit::GlyphId* gIdArray, int glyphCount, EA::WebKit::GlyphDrawInfo* outGDI, bool abortOnFail)
{
    bool returnFlag = true;

    for (int i=0; i < glyphCount; i++)
    {
        if(!GetCachedGlyph(pFont, gIdArray[i], outGDI[i]))
        {
            outGDI[i].mpData = NULL;    // Make sure we invalidate for EAWebKit check.
            returnFlag = false;
            if (abortOnFail)            // Fast exit since we might flush so no point in going further.
                break;
        }
    }
    return returnFlag;
}

bool TextSystem::GetGlyphs(EA::WebKit::IFont* pFont, const EA::WebKit::GlyphId* gIdArray, int glyphCount, EA::WebKit::GlyphDrawInfo* outGDI)
{
    // Try to draw/cache all the glyphs in the run.
    bool abortOnFail = true;
    bool returnFlag = FillGlyphDrawInfo(pFont, gIdArray, glyphCount, outGDI, abortOnFail);
    if (!returnFlag)
    {
        // Flush the cache if we need to retry
        FlushGlyphCache();
        abortOnFail = false;   // We attempt to draw what we can, even if a glyph fails after flushing. 
        returnFlag = FillGlyphDrawInfo(pFont, gIdArray, glyphCount, outGDI, abortOnFail);
    }
    return returnFlag;
}

bool TextSystem::GetCachedGlyph(EA::WebKit::IFont* pFont, EA::WebKit::GlyphId g, EA::WebKit::GlyphDrawInfo& gdi)
{
    EA::Text::GlyphTextureInfo  gtiEA;
	EA::Text::Font* pFontEA =  static_cast<EA::WebKit::FontImpl*> (pFont)->mpFont;
    EA::Text::GlyphId gEA = GetEAText_GlyphId(g);
    
    EA::Text::GlyphCache* pCache = mpCurrentDrawGlyphCache;               
    if(!pCache)
        pCache = mpGlyphCache; 

    bool returnFlag = pCache->GetGlyphTextureInfo(pFontEA, gEA, gtiEA);
    if (!returnFlag)
    {
        // Draw the glyph
        const EA::Text::Font::GlyphBitmap* pGlyphBitmap = 0;    
        if (pFontEA->RenderGlyphBitmap(&pGlyphBitmap, gEA))
        {
            // During the load to the glyphcache, the glyph bitmap format can get converted to the user glyph cache format.
            if (pCache->AddGlyphTexture(pFontEA, gEA, pGlyphBitmap->mpData, pGlyphBitmap->mnWidth, pGlyphBitmap->mnHeight, 
                                                   pGlyphBitmap->mnStride, (uint32_t) pGlyphBitmap->mBitmapFormat, gtiEA))
            {
                pCache->EndUpdate(gtiEA.mpTextureInfo);
                returnFlag = true;
            }
            else
            {
                EA_ASSERT_MSG(false, "FontEA: Out of glyph cache space.  Please consider increasing size");
            }
            pFontEA->DoneGlyphBitmap(pGlyphBitmap);
        } 
        else
        {
            EA_ASSERT_MSG(false, "FontEA: drawGlyphs: invalid glyph/Font combo.");
        }
    }

    // Now convert back to EAWebKitFormat
    if (returnFlag)
        ConvertToWebKit_GlyphTextureInfo(gtiEA,gdi);    
    else
        gdi.mpData = 0;
    
    return returnFlag;
}

uint32_t TextSystem::AddFace(IO::IStream* pStream)
{
    uint32_t returnValue =0;
    EA_ASSERT(mpFontServer);   
    if (mpFontServer)    
    {
        returnValue = mpFontServer->AddFace(pStream, EA::Text::kFontTypeOutline);
    }
    return returnValue;
}

uint32_t TextSystem::AddDirectory(const char16_t* pFaceDirectory, const char16_t* pFilter)
{
	uint32_t returnValue =0;
    EA_ASSERT(mpFontServer);   
    if (mpFontServer)    
	    returnValue =  mpFontServer->AddDirectory(pFaceDirectory, pFilter);

    return returnValue;
}

bool TextSystem::AddSubstitution(const char16_t* pFamily, const char16_t* pFamilySubstitution)
{
    bool returnFlag = false;

#if EATEXT_FAMILY_SUBSTITUTION_ENABLED
    EA_ASSERT(mpFontServer);   
    if (mpFontServer)    
	    returnFlag = mpFontServer->AddSubstitution(pFamily,pFamilySubstitution);
#endif
	return returnFlag;
}

bool TextSystem::SupportsFormat(EA::WebKit::FontFormat format)
{
	if(format == EA::WebKit::kTrueType || format == EA::WebKit::kOpenType)
		return true;

	return false;
}

uint32_t TextSystem::GetShadowEffectID(void)  
{
    return mUserShadowEffectID ? mUserShadowEffectID : EA::Text::kEffectShadow;
}

// Text break iterator statics 
static EA::Text::CharacterBreakIterator sCharacterBreakIterator;
static EA::Text::WordBreakIterator      sWordBreakIterator;
static EA::Text::LineBreakIterator      sLineBreakIterator;
static EA::Text::SentenceBreakIterator  sSentenceBreakIterator;
static EA::Text::TextRun                sTextRun;
static EA::Text::BreakIteratorBase*     sTextBreakIterator;
const int TextBreakDone = -1;

void* TextSystem::CharacterBreakIterator(EA::WebKit::Char* pText, int length)
{
    // To do: Have EAText iterators keep a member TextRun so you don't need to maintain one EATextly.
    sTextRun.mpText     = (EA::Text::Char*) pText;
    sTextRun.mnTextSize = (uint32_t)length;
    sCharacterBreakIterator.GetIterator().SetTextRunArray(&sTextRun, 1);
    sTextBreakIterator = &sCharacterBreakIterator;

    return sTextBreakIterator;
}

void* TextSystem::WordBreakIterator(EA::WebKit::Char* pText, int length)
{
    // To do: Have EAText iterators keep a member TextRun so you don't need to maintain one EATextly.
    sTextRun.mpText     = (EA::Text::Char*) pText;
    sTextRun.mnTextSize = (uint32_t)length;
    sWordBreakIterator.GetIterator().SetTextRunArray(&sTextRun, 1);
    sTextBreakIterator = &sWordBreakIterator;

    return sTextBreakIterator;
}

void* TextSystem::LineBreakIterator(EA::WebKit::Char* pText, int length)
{
    // To do: Have EAText iterators keep a member TextRun so you don't need to maintain one EATextly.
    sTextRun.mpText     = (EA::Text::Char*) pText;
    sTextRun.mnTextSize = (uint32_t)length;
    sLineBreakIterator.GetIterator().SetTextRunArray(&sTextRun, 1);
    sTextBreakIterator = &sLineBreakIterator;

    return sTextBreakIterator;
}

void* TextSystem::SentenceBreakIterator(EA::WebKit::Char* pText, int length)
{
    // To do: Have EAText iterators keep a member TextRun so you don't need to maintain one EATextly.
    sTextRun.mpText     = (EA::Text::Char*) pText;
    sTextRun.mnTextSize = (uint32_t)length;
    sSentenceBreakIterator.GetIterator().SetTextRunArray(&sTextRun, 1);
    sTextBreakIterator = &sSentenceBreakIterator;

    return sTextBreakIterator;
}

void* TextSystem::AcquireLineBreakIterator(EA::WebKit::Char* pText, int length)
{
    EA::Text::LineBreakIterator *newIterator = new (EA::WebKit::GetAllocator()->Malloc(sizeof(EA::Text::LineBreakIterator),0,0)) EA::Text::LineBreakIterator();
    EA::Text::TextRun *newRun = new (EA::WebKit::GetAllocator()->Malloc(sizeof(EA::Text::TextRun),0,0)) EA::Text::TextRun();
    mSavedTextRuns[newIterator] = newRun;

    newRun->mpText = pText;
    newRun->mnTextSize = (uint32_t)length;
    newIterator->GetIterator().SetTextRunArray(newRun, 1);

    return newIterator;
}

void TextSystem::ReleaseLineBreakIterator(void *pIterator)
{
    EA::Text::LineBreakIterator *iterator = reinterpret_cast<EA::Text::LineBreakIterator*>(pIterator);
    EA::Text::TextRun *run = mSavedTextRuns[iterator];
    mSavedTextRuns.erase(iterator);

    run->~TextRun();
    EA::WebKit::GetAllocator()->Free(run, 0);

    iterator->~LineBreakIterator();
    EA::WebKit::GetAllocator()->Free(iterator, 0);
}

int TextSystem::TextBreakFirst(void* pIter)
{
    EA::Text::BreakIteratorBase* pIterator = static_cast<EA::Text::BreakIteratorBase*> (pIter);
    
    // We have a problem: EATextBreak doesn't implement a pure-virtual NextBreak/PrevBreak function.
    // It's easy to do this but it requires us to update EAText. We do something of a hack in the 
    // short run to allow this to work now by checking each of the global variables manually.
    pIterator->SetPosition(0);
    int returnValue = TextBreakNext(pIterator);

    // Verify that we still a next break after this one. 
    if (pIterator->AtEnd())
    {
        // If no other break was found, reset to start.    
        returnValue = pIterator->SetPosition(0);    
    }    
    return returnValue;
}

int TextSystem::TextBreakNext(void* pIter)
{
	// 04/26/2011 - abaldeva - Disabled old code and replaced it with an optimized version.
	EA::Text::BreakIteratorBase* pIterator = static_cast<EA::Text::BreakIteratorBase*> (pIter);
	
	if (pIterator->AtEnd())
		return TextBreakDone;

	int result;

	if (pIterator == &sCharacterBreakIterator)
		result		= sCharacterBreakIterator.GetNextCharBreak();
	else if (pIterator == &sWordBreakIterator)
		result      = sWordBreakIterator.GetNextWordBreak();
	else if (pIterator == &sLineBreakIterator)
		result      = sLineBreakIterator.GetNextLineBreak();
	else // if(pIterator == &sSentenceBreakIterator)
		result      = sSentenceBreakIterator.GetNextSentenceBreak();

	return result;
}

int TextSystem::TextBreakCurrent(void* pIter)
{
    EA::Text::BreakIteratorBase* pIterator = static_cast<EA::Text::BreakIteratorBase*> (pIter);
       
    return static_cast<int> (pIterator->GetPosition());
}


int TextSystem::TextBreakPrevious(void* pIter)
{
    EA::Text::BreakIteratorBase* pIterator = static_cast<EA::Text::BreakIteratorBase*> (pIter);
    if (pIterator->AtBegin())
		return TextBreakDone;

	int result;
	if (pIterator == &sCharacterBreakIterator)
		result      = sCharacterBreakIterator.GetPrevCharBreak();
	else if (pIterator == &sWordBreakIterator)
		result      = sWordBreakIterator.GetPrevWordBreak();
	else if (pIterator == &sLineBreakIterator)
		result      = sLineBreakIterator.GetPrevLineBreak();
	else // if(pIterator == &gSentenceBreakIterator)
		result      = sSentenceBreakIterator.GetPrevSentenceBreak();

	return result;
}

int TextSystem::TextBreakPreceding(void* pIter, int position)
{
	EA::Text::BreakIteratorBase* pIterator = static_cast<EA::Text::BreakIteratorBase*> (pIter);
	pIterator->SetPosition((uint32_t)position);
	return  TextSystem::TextBreakPrevious(pIterator);
}

int TextSystem::TextBreakFollowing(void* pIter, int position)
{
    EA::Text::BreakIteratorBase* pIterator = static_cast<EA::Text::BreakIteratorBase*> (pIter);

    pIterator->SetPosition((uint32_t)position);
    return TextBreakNext(pIterator);
}

bool TextSystem::IsTextBreak(void* pIter, int position)
{
    EA::Text::BreakIteratorBase* pIterator = static_cast<EA::Text::BreakIteratorBase*> (pIter);

    // If we are being asked about the most recent break position, then we know it is a break position.
    // Otherwise the only means we have to implementing this is to walk through the entire string
    // and check each break against position.
    if (position == static_cast<int> (pIterator->GetPosition()))
        return true;

    pIterator->SetPosition(0);

    while ((static_cast<int> (pIterator->GetPosition()) < position) && !pIterator->AtEnd())
        TextBreakNext(pIterator);

    return (position == static_cast<int> (pIterator->GetPosition()));
}

int TextSystem::FindNextWordFromIndex(EA::WebKit::Char* chars, int len, int position, bool forward)
{
    /// The arrows below show the positions that would be returned by 
    /// successive calls to GetNextWordBreak with the default wordBreakType.
    ///    "Hello   world   hello   world"
    ///     ^    ^  ^    ^  ^    ^  ^    ^

    EA::Text::TextRun           tr((EA::Text::Char*)chars, (uint32_t)len);
    EA::Text::WordBreakIterator wbi;

    wbi.SetIterator(EA::Text::TextRunIterator(&tr));
    wbi.SetPosition((uint32_t)position);

    if (forward)
        return static_cast<int> (wbi.GetNextWordBreak(EA::Text::kWordBreakTypeBegin));    // It turns out that as of this writing EAText doesn't implement kWordBreakTypeBegin/End. We will need to make sure that's working so this code can work as expected.
    else
        return static_cast<int> (wbi.GetPrevWordBreak(EA::Text::kWordBreakTypeBegin));
}

void TextSystem::FindWordBoundary(EA::WebKit::Char* chars, int len, int position, int* start, int* end)
{
    /// The arrows below show the positions that would be returned by 
    /// successive calls to GetNextWordBreak with the default wordBreakType.
    ///    "Hello   world   hello   world"
    ///     ^    ^  ^    ^  ^    ^  ^    ^

    EA::Text::TextRun           tr( static_cast<EA::Text::Char*> (chars), static_cast<uint32_t> (len));
    EA::Text::WordBreakIterator wbi;

    wbi.SetIterator(EA::Text::TextRunIterator(&tr));
    wbi.SetPosition( static_cast<uint32_t> (position));

    *start = static_cast<int> (wbi.GetPrevWordBreak(EA::Text::kWordBreakTypeBegin));

    wbi.SetPosition(static_cast<uint32_t> (*start));

    // 04/26/2011 - abaldeva: Fixed a bug otherwise last char of a word is not selected
	// when double clicked (Though double clicking to select a word does not work in 1.x). 
	*end = static_cast<int> (wbi.GetNextWordBreak(EA::Text::kWordBreakTypeEnd)/* - 1*/);
}

// Access to some general unicode interface functions
int32_t TextSystem::GetCombiningClass(EA::WebKit::Char c)
{
    EA::Text::Char cEAText = GetEAText_Char(c);
    
    return EA::Text::GetCombiningClass(cEAText);            
}

EA::WebKit::Char TextSystem::GetMirrorChar(EA::WebKit::Char c)
{
    EA::Text::Char cEAText = GetEAText_Char(c);
    
    return GetWebKit_Char(EA::Text::GetMirrorChar(cEAText));                 
}

EA::WebKit::CharCategory TextSystem::GetCharCategory(EA::WebKit::Char c)
{
    EA::Text::Char cEAText = GetEAText_Char(c);
    EA::Text::CharCategory catEAText = EA::Text::GetCharCategory(cEAText);            
    return GetWebKit_CharCategory(catEAText);
}

EA::WebKit::BidiClass TextSystem::GetBidiClass(EA::WebKit::Char c)
{
    EA::Text::Char cEAText = GetEAText_Char(c);
    EA::Text::BidiClass bidiEAText = EA::Text::GetBidiClass(cEAText);
    return GetWebKit_BidiClass(bidiEAText);
}

uint32_t TextSystem::ConvertCase(const EA::WebKit::Char* pTextInput, uint32_t nTextLength, EA::WebKit::Char* pTextOutput, uint32_t nTextOutputCapacity, EA::WebKit::CaseType caseType)
{
    EA::Text::CaseType caseTypeEAText;
    switch(caseType)
    {
    case EA::WebKit::kCaseTypeLower:     
        caseTypeEAText = EA::Text::kCaseTypeLower;    
        break;

    case EA::WebKit::kCaseTypeUpper:    
        caseTypeEAText = EA::Text::kCaseTypeUpper;    
        break;

    case EA::WebKit::kCaseTypeTitle:    
        caseTypeEAText = EA::Text::kCaseTypeTitle;    
        break;

        // No current use in EAWebKit for the reverse convert.
    default:
        caseTypeEAText = EA::Text::kCaseTypeNone;
        EA_ASSERT(0);
        break;
    }

    uint32_t returnVal =  EA::Text::ConvertCase(static_cast<const EA::Text::Char*>(pTextInput), nTextLength, static_cast<EA::Text::Char*>(pTextOutput), nTextOutputCapacity, caseTypeEAText);
    return returnVal;
}

uint32_t TextSystem::GetFamilyNameArrayCapacity(void)
{
    int size = EA::Text::kFamilyNameArrayCapacity < EA::WebKit::kFamilyNameArrayCapacity ?  EA::Text::kFamilyNameArrayCapacity : EA::WebKit::kFamilyNameArrayCapacity;
    return size;
}


void TextSystem::SetUpDrawGlyphCacheType(bool textHasEffect)
{
	// We save some time by setting up just once the glyph cache to use for a text run draw. 
	if (textHasEffect)
	{
		mpCurrentDrawGlyphCache = (mpGlyphEffectsCache ? mpGlyphEffectsCache : mpGlyphCache);
	}
	else
	{
		mpCurrentDrawGlyphCache = mpGlyphCache;
	}

#if _DEBUG
    static bool sTextEffectGlyphFormatCheck = false;
    if ((!sTextEffectGlyphFormatCheck) && (textHasEffect) && (mpCurrentDrawGlyphCache))
    {
        EA::Text::TextureInfo* textureInfo = mpCurrentDrawGlyphCache->GetTextureInfo(0);
        if (textureInfo)
        {
            bool glyphCacheCanRenderEffects = false;
            uint32_t format = textureInfo->mFormat;
            if((format == EA::Text::kTextureFormatARGB) || (format == EA::Text::kTextureFormatRGBA) || (format == EA::Text::kTextureFormatABGR)|| (format == EA::Text::kTextureFormatDXT3))
                glyphCacheCanRenderEffects = true;
            EA_ASSERT_MSG(glyphCacheCanRenderEffects, "Rendering text effect requires a glyph cache with 32-bit support. Text may not display correctly.");
            sTextEffectGlyphFormatCheck = true;
        }
    }
#endif
}

void TextSystem::RemoveTextureInfoFromGlyphCache(EA::Text::Font* pFont)
{
    if (mpGlyphCache)
    {
        mpGlyphCache->RemoveTextureInfo(pFont);
    }    

    if (mpGlyphEffectsCache)
    {
        mpGlyphEffectsCache->RemoveTextureInfo(pFont);
    }
}

void TextSystem::SetUserShadowEffectID(uint32_t id)
{
    mUserShadowEffectID = id;
}

// Default font system

static TextSystem* spTextSystem = 0;
static EA::Text::GlyphCache*  spGlyphCache = 0;
static EA::Text::FontServer*  spFontServer = 0;
static bool sFontSystemHasInit = false;

void InitFontSystem(void)
{
    if(!sFontSystemHasInit)
    {
        EA::Text::Init();
        EA::Text::SetAllocator(EA::Allocator::ICoreAllocator::GetDefaultAllocator());

        if(!spGlyphCache)
        {
            const EA::Text::TextureFormat format = EA::Text::kTextureFormatARGB; // 32 bit
        	
            spGlyphCache = new EA::Text::GlyphCache_Memory(format);
            EA_ASSERT_MSG(spGlyphCache, "FontSystem: Failed to create the glyph cache.");  
            if (!spGlyphCache)
                return;    
    
            spGlyphCache->SetAllocator(EA::Allocator::ICoreAllocator::GetDefaultAllocator());
            spGlyphCache->SetOption(EA::Text::GlyphCache::kOptionDefaultSize, 512);
            spGlyphCache->SetOption(EA::Text::GlyphCache::kOptionDefaultFormat, format);
        
            // This kOptionAutoTextureCreate setting allows the glyph cache to allocate more pages if
            // it runs out of space and nMaxTextureCount > 1. 
            spGlyphCache->SetOption(EA::Text::GlyphCache::kOptionAutoTextureCreate, true);  
            spGlyphCache->Init(8, 1); // (nMaxTextureCount = 8, nInitialTextureCount = 1) 
        }
        
        if(!spFontServer)
        {
            // Set up the FontServer.
            spFontServer = new EA::Text::FontServer;
            EA_ASSERT_MSG(spFontServer, "FontSystem: Failed to create the font server.");     
            if (!spFontServer)
                return;
           
            EA::Text::SetFontServer(spFontServer);
            spFontServer->SetDefaultGlyphCache(spGlyphCache);
        }

        spTextSystem = new TextSystem(spFontServer, spGlyphCache, 0);
        if(spTextSystem)
        {
            EA::WebKit::SetTextSystem(spTextSystem);
            sFontSystemHasInit = true;       
        }
    }
}

void ShutdownFontSystem(void)
{
    if(sFontSystemHasInit)
    {
        if(spTextSystem)
        {
            delete spTextSystem;
            spTextSystem = 0;
        }

        if(spFontServer)
        {
           delete spFontServer; 
           spFontServer = 0;
        }

        if(spGlyphCache)
        {
           spGlyphCache->Shutdown(); 
           delete spGlyphCache; 
           spGlyphCache = 0;
        }

        EA::Text::Shutdown();  
        sFontSystemHasInit = false;
    }
}    

    } // Namespace WebKit
} // Namespace EA

