//
//  FCTimeline.h
//  FlashAnimation-X
//
//  Created by Moky on 12-12-10.
//  Copyright (c) 2012年 Slanissue Inc. All rights reserved.
//

#ifndef FlashAnimation_X_FCTimeline_h
#define FlashAnimation_X_FCTimeline_h

#include "../fcMacros.h"

NS_FC_BEGIN

class FCLibrarySymbol;

class FC_DLL FCTimeline : public cocos2d::CCObject
{
	CC_SYNTHESIZE(FCLibrarySymbol *, m_pSymbol, Symbol);
	CC_SYNTHESIZE(unsigned int, m_iFrameCount, FrameCount);
	CC_SYNTHESIZE_RETAIN(cocos2d::CCArray *, m_pLayers, Layers);
	
public:
	FCTimeline(void);
	virtual ~FCTimeline(void);
	virtual bool init(void);
	
	virtual bool init(cocos2d::CCArray * pLayers);
	
	static FCTimeline * timeline(cocos2d::CCArray * pLayers);
	
	cocos2d::CCArray * spritesForEachLayerOfFrameIndex(const unsigned int iIndex) const;
};

NS_FC_END

#endif
