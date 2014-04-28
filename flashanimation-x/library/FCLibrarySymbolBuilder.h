//
//  FCLibrarySymbolBuilder.h
//  FlashAnimation-X
//
//  Created by Moky on 12-12-10.
//  Copyright 2012 Slanissue Inc. All rights reserved.
//

#ifndef FlashAnimation_X_FCLibrarySymbolBuilder_h
#define FlashAnimation_X_FCLibrarySymbolBuilder_h

#include "../timeline/FCTimelineLayer.h"

#include "FCLibrarySymbol.h"

NS_FC_BEGIN

class FC_DLL FCLibrarySymbolBuilder : public cocos2d::CCObject
{
public:
	static FCLibrarySymbolBuilder * symbolBuilder(void);
	
	FCLibrarySymbol * buildSymbol(cocos2d::CCDictionary * pSymbolData) const;
	
protected:
	FCTimeline * buildTimeline(cocos2d::CCArray * pLayers) const;
	FCTimelineLayer * buildTimelineLayer(cocos2d::CCDictionary * pLayerData) const;
	FCTimelineFrame * buildTimelineFrame(cocos2d::CCDictionary * pFrameData) const;
	FCTimelineSprite * buildTimelineSprite(cocos2d::CCDictionary * pSpriteData) const;
};

NS_FC_END

#endif
