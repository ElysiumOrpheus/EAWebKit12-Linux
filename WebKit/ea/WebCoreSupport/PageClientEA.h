/*
 * Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies)
 * Copyright (C) 2011, 2012 Electronic Arts, Inc. All rights reserved.
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
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 */

#ifndef PageClientEA_h
#define PageClientEA_h

#include "FrameView.h"
#include "GraphicsContext.h"
#include "IntRect.h"
#include "WebPageClientEA.h"
#include "TiledBackingStore.h"
#include "Timer.h"
#include "WebFrame.h"
#include "WebFrame_p.h"
#include "WebPage.h"
#include "WebPage_p.h"

#include <Settings.h>

#if USE(ACCELERATED_COMPOSITING)
#include "texmap/TextureMapperPlatformLayer.h"
#endif

namespace EA { namespace WebKit {
class View;
}}

namespace WebCore {

#if USE(ACCELERATED_COMPOSITING)
class TextureMapperNodeClientEA
{
public:
    TextureMapperNodeClientEA(EA::WebKit::WebFrame*, GraphicsLayer*);
    virtual ~TextureMapperNodeClientEA();
    void setTextureMapper(const PassOwnPtr<TextureMapper>&);
    void syncRootLayer();
    class TextureMapperNode* rootNode();

private:
    EA::WebKit::WebFrame* m_frame;
    OwnPtr<GraphicsLayer> m_rootGraphicsLayer;
};
#endif

class PageClientWebView : public WebPageClient 
{
public:
	PageClientWebView(EA::WebKit::View* newView, EA::WebKit::WebPage* newPage)
    : view(newView)
    , page(newPage)
#if USE(ACCELERATED_COMPOSITING)
	, mNeedsSync(false)
    , mScheduledSync(false)
#endif
#if ENABLE(TILED_BACKING_STORE)
	, mNonVisibleTilesRemovalTimer(this,&PageClientWebView::removeNonVisibleTiles)
#endif
	{
        ASSERT(view);
    }
    virtual ~PageClientWebView();

    virtual void scroll(int dx, int dy, const IntRect&);
    virtual void update(const IntRect& dirtyRect);
    virtual void setInputMethodEnabled(bool enable);
    virtual bool inputMethodEnabled() const;

    virtual void updateCursor(int type);
	virtual int screenNumber() const;

    virtual IntRect geometryRelativeToOwnerWidget() const;
    virtual bool viewResizesToContentsEnabled() const { return false; }

    virtual IntRect windowRect() const;

	EA::WebKit::View* view;
    EA::WebKit::WebPage* page;

#if USE(ACCELERATED_COMPOSITING)
    virtual void setRootGraphicsLayer(GraphicsLayer*);
    virtual void markForSync(bool scheduleSync);
    void syncLayers(void);


    virtual bool allowsAcceleratedCompositing(void) const;
    OwnPtr<TextureMapperNodeClientEA> textureMapperNodeClient;


    bool mNeedsSync;
    bool mScheduledSync;

#else
    virtual bool allowsAcceleratedCompositing() const { return false; }
#endif

#if ENABLE(TILED_BACKING_STORE)
private:
	// We need a timer otherwise if the user scrolls really fast, it results in screen artifacts.
	WebCore::Timer<PageClientWebView>   mNonVisibleTilesRemovalTimer; 
	void removeNonVisibleTiles(WebCore::Timer<PageClientWebView>*);
#endif

};


}
#endif // PageClientEA
