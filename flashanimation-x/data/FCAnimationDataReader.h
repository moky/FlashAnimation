//
//  FCAnimationDataReader.h
//  FlashAnimation-X
//
//  Created by Moky on 12-12-15.
//  Copyright 2012 Slanissue Inc. All rights reserved.
//

#ifndef FlashAnimation_X_FCAnimationDataReader_h
#define FlashAnimation_X_FCAnimationDataReader_h

#include "FCAnimationData.h"

NS_FC_BEGIN

class FCAnimationDataReader : public FCAnimationData
{
	CC_SYNTHESIZE_RETAIN(cocos2d::CCArray *, m_pFrames, Frames); // filename array for sprites ('fileId')
	CC_SYNTHESIZE_RETAIN(cocos2d::CCDictionary *, m_pNames, Names); // 'symbol name -> index' for 'id'
	
public:
	FCAnimationDataReader(void);
	virtual ~FCAnimationDataReader(void);
	virtual bool init(void);
	
	virtual bool init(const std::string & sPath);
	
#pragma mark Data Interfaces
	// textures
	cocos2d::CCArray * getTextures(void) const;
	// symbol
	int getSymbolLocation(const std::string & sSymbolName) const;
	cocos2d::CCDictionary * getSymbolDictionary(const std::string & sSymbolName) const;
	cocos2d::CCDictionary * getAllSymbols(void) const;
	cocos2d::CCDictionary * getSymbolDictionary(const unsigned int iLocation) const;
	cocos2d::CCDictionary * getSymbolDictionary(const FCSymbolInfo & symbol) const;
	// timeline layer
	cocos2d::CCDictionary * getTimelineLayerDictionary(const unsigned int iLocation) const;
	cocos2d::CCDictionary * getTimelineLayerDictionary(const FCTimelineLayerInfo & timelineLayer) const;
	// timeline frame
	cocos2d::CCDictionary * getTimelineFrameDictionary(const unsigned int iLocation) const;
	cocos2d::CCDictionary * getTimelineFrameDictionary(const FCTimelineFrameInfo & timelineFrame) const;
	// timeline sprite
	cocos2d::CCDictionary * getTimelineSpriteDictionary(const unsigned int iLocation) const;
	cocos2d::CCDictionary * getTimelineSpriteDictionary(const FCTimelineSpriteInfo & timelineSprite) const;
};

NS_FC_END

#endif
