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

#ifndef EAWEBKIT_TEXT_WRAPPER_H
#define EAWEBKIT_TEXT_WRAPPER_H

#include <EABase/eabase.h>
#include <EAWebKit/EAWebKitConfig.h>
#include <EAWebKit/EAWebKitTextInterface.h>
#include <EAWebKit/EAWebkitAllocator.h> 

#include <EAText/EATextFontServer.h>   
#include <EAText/EATextFont.h>
#include <EAText/EATextCache.h>
#include <EAText/EATextBreak.h> 

#include <EASTL/map.h>

namespace EA
{
namespace WebKit
{
    void InitFontSystem(void);
    void ShutdownFontSystem(void);

class TextSystem;
// This class implements the font interface defined by EAWebKit.
class FontImpl : public EA::WebKit::IFont
{
public:


	FontImpl(EA::WebKit::TextSystem* pTextSystem, EA::Text::Font* pFont);
	~FontImpl(void);

    int AddRef(void);
    int Release(void);

    bool IsItalic(void);
    bool IsSmallCaps(void);
    bool HasEffect(void);   
	float GetSize(void) const;  // This is the font fSize
	bool  SetSize(float fSize);

    bool GetFontMetrics(EA::WebKit::FontMetrics& fontMetricsInternal);
    bool GetGlyphMetrics(EA::WebKit::GlyphId glyphId, EA::WebKit::GlyphMetrics& glyphMetricsInternal);
    uint32_t GetGlyphIds(const EA::WebKit::Char* pCharArray, uint32_t nCharArrayCount, EA::WebKit::GlyphId* pGlyphIdArray = NULL, 
        bool bUseReplacementGlyph = true, const uint32_t nGlyphIdStride = sizeof(EA::WebKit::GlyphId), bool bWriteInvalidGlyphs = false);
	bool GetKerning(EA::WebKit::GlyphId g1, EA::WebKit::GlyphId g2, EA::WebKit::Kerning& kerning, int direction, bool bHorizontalLayout = true);

    bool IsCharSupported(EA::WebKit::Char c, EA::WebKit::Script script = EA::WebKit::kScriptUnknown);

    //bool SetEffect(const EA::WebKit::TextEffectData& effect,EA::WebKit:: IFontServer* const pFontServer);
    void SetSmoothing(EA::WebKit::Smooth type);
    
    // These are used to control which glyph cache to use.  Could also be used for cleaning up after drawing without a cache (once supported).
    void BeginDraw(void);
    void EndDraw(void);

    void SetEffectData(const EA::WebKit::TextEffectData& effectData); 

private:
	friend class TextSystem;
	// Declare these (but don't define) since this is ref counted class. If anybody tries to
	// copy/assign, this will result in build error. You need to correctly implement these methods if want to use them.
	FontImpl(const FontImpl&);
	FontImpl& operator=(const FontImpl&);

	EA::WebKit::TextSystem* mpTextSystem;
	EA::Text::Font* mpFont;

    // The ref count of this instance and not the EA::Text::Font as EA::Text::Font could be used in the application elsewhere.
	int mRefCount;  
};




class TextSystem : public EA::WebKit::ITextSystem
{
public:
    TextSystem(EA::Text::FontServer* pFontServer, EA::Text::GlyphCache* pGlyphCache); 
    TextSystem(EA::Text::FontServer* pFontServer, EA::Text::GlyphCache* pGlyphCache, EA::Text::GlyphCache* pEffectsGlyphCache); 
    virtual ~TextSystem(void);

    virtual bool Init(void);
    virtual bool Shutdown(void);

    // Font Support
    virtual bool                GetGlyphs(EA::WebKit::IFont* pFont, const EA::WebKit::GlyphId* gIdArray, int glyphCount, EA::WebKit::GlyphDrawInfo* outGDI);
    virtual EA::WebKit::IFont*	GetFont(const EA::WebKit::TextStyle& textStyle, EA::WebKit::Char c);
    virtual EA::WebKit::IFont*	CreateNewFont(EA::WebKit::FontType fontType, const void* pSourceData, uint32_t nSourceSize, int nFaceIndex = 0);
    virtual uint32_t			AddFace(IO::IStream* pStream);
    virtual uint32_t			AddDirectory(const char16_t* pFaceDirectory, const char16_t* pFilter);
    virtual bool				AddSubstitution(const char16_t* pFamily, const char16_t* pFamilySubstitution);
	virtual bool				SupportsFormat(EA::WebKit::FontFormat format);

    // Text Layout Support
    virtual void*	CharacterBreakIterator(EA::WebKit::Char* pText, int length);
    virtual void*	WordBreakIterator(EA::WebKit::Char* pText, int length);
    virtual void*	LineBreakIterator(EA::WebKit::Char* pText, int length);
    virtual void*	SentenceBreakIterator(EA::WebKit::Char* pText, int length);
    virtual int		TextBreakFirst(void* pIter);
    virtual int		TextBreakNext(void* pIter);
    virtual int		TextBreakCurrent(void* pIter);
    virtual int		TextBreakPrevious(void* pIterator);
    virtual int		TextBreakPreceding(void* pIter, int position);
    virtual int		TextBreakFollowing(void* pIter, int position);
    virtual bool	IsTextBreak(void* pIter, int position);
    virtual int		FindNextWordFromIndex(EA::WebKit::Char* chars, int len, int position, bool forward);
    virtual void	FindWordBoundary(EA::WebKit::Char* chars, int len, int position, int* start, int* end);

    void*           AcquireLineBreakIterator(EA::WebKit::Char* pText, int length);
    void            ReleaseLineBreakIterator(void *pIterator);

    // Unicode Support
    virtual int32_t						GetCombiningClass(EA::WebKit::Char c);
    virtual EA::WebKit::Char			GetMirrorChar(EA::WebKit::Char c);
    virtual EA::WebKit::CharCategory	GetCharCategory(EA::WebKit::Char c);
    virtual EA::WebKit::BidiClass		GetBidiClass(EA::WebKit::Char c);
    virtual uint32_t ConvertCase(const EA::WebKit::Char* pTextInput, uint32_t nTextLength, EA::WebKit::Char* pTextOutput, uint32_t nTextOutputCapacity, EA::WebKit::CaseType caseType);

    uint32_t GetFamilyNameArrayCapacity(void);

    virtual void SetUserShadowEffectID(uint32_t id);

    // Implementation specific methods of this class
    EA::Text::FontServer* GetFontServer(void) 
    { 
	    return mpFontServer; 
    }

    void AddFont(EA::WebKit::FontImpl* pFontImpl)
    {
	    mActiveFonts.push_back(pFontImpl);
    }
    void RemoveFont(EA::WebKit::FontImpl* pFontImpl)
    {
	    mActiveFonts.remove(pFontImpl);
    }

    void SetUpDrawGlyphCacheType(bool textHasEffect = false);
    void FlushGlyphCache(void);
    void RemoveTextureInfoFromGlyphCache(EA::Text::Font* pFont);   
    uint32_t GetShadowEffectID(void);  

 private:
    FontImpl* CreateFontImpl(EA::Text::Font* pFont);
    bool FillGlyphDrawInfo(EA::WebKit::IFont* pFont, const EA::WebKit::GlyphId* gIdArray, int glyphCount, EA::WebKit::GlyphDrawInfo* outGDI, bool abortOnFail);
    bool GetCachedGlyph(EA::WebKit::IFont* pFont, EA::WebKit::GlyphId g, EA::WebKit::GlyphDrawInfo& gti);
 
    eastl::list<EA::WebKit::FontImpl*, EA::WebKit::EASTLAllocator> mActiveFonts;

    EA::Text::FontServer* mpFontServer;
    EA::Text::GlyphCache* mpGlyphCache;         // The normal glyph cache for text.  Might want to keep it 8bit to save RAM.    
    EA::Text::GlyphCache* mpGlyphEffectsCache;  // This is an optional cache for using wiht EA Text effect scripts so it needs to be in 32 bit to get color.
    
    bool mOwnFontServer;                        //true if we create ours
    bool mOwnGlyphCache;  

    uint32_t    mUserShadowEffectID; // Optional user shadow effect ID.  

    EA::Text::GlyphCache* mpCurrentDrawGlyphCache; // The glyph cache to use for the current draw.
    eastl::map<EA::Text::LineBreakIterator*, EA::Text::TextRun*, eastl::less<EA::Text::LineBreakIterator*>, EASTLAllocator> mSavedTextRuns;
};

} // Namespace WebKit

} // Namespace EA

#endif // Header include guard
