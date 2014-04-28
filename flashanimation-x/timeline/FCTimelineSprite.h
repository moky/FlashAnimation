//
//  FCTimelineSprite.h
//  FlashAnimation-X
//
//  Created by Moky on 12-12-9.
//  Copyright 2012 Slanissue Inc. All rights reserved.
//

#ifndef FlashAnimation_X_FCTimelineSprite_h
#define FlashAnimation_X_FCTimelineSprite_h

#include "../fcMacros.h"

NS_FC_BEGIN

typedef enum {
	FCTimelineSpriteTypeShape,
	FCTimelineSpriteTypeGraphic,
	FCTimelineSpriteTypeMovieClip,
} FCTimelineSpriteType;

typedef enum {
	FCTimelineSpriteLoopTypeOnce,
	FCTimelineSpriteLoopTypeLoop,
	FCTimelineSpriteLoopTypeSingleFrame,
} FCTimelineSpriteLoopType;

class FC_DLL FCTimelineSprite : public cocos2d::CCObject
{
	CC_SYNTHESIZE_PASS_BY_REF(std::string, m_sSymbolName, SymbolName);
	CC_SYNTHESIZE(FCTimelineSpriteType, m_iType, Type);
	
	CC_SYNTHESIZE_PASS_BY_REF(cocos2d::CCPoint, m_tPosition, Position);
	CC_SYNTHESIZE_PASS_BY_REF(cocos2d::CCPoint, m_tAnchorPoint, AnchorPoint);
	CC_SYNTHESIZE(unsigned int, m_iFirstFrame, FirstFrame);
	CC_SYNTHESIZE(FCTimelineSpriteLoopType, m_iLoopType, LoopType);
	CC_SYNTHESIZE_PASS_BY_REF(cocos2d::CCAffineTransform, m_tMatrix, Matrix);
	
	CC_SYNTHESIZE(cocos2d::CCFloat, m_fScaleX, ScaleX);
	CC_SYNTHESIZE(cocos2d::CCFloat, m_fScaleY, ScaleY);
	CC_SYNTHESIZE(cocos2d::CCFloat, m_fSkewX, SkewX);
	CC_SYNTHESIZE(cocos2d::CCFloat, m_fSkewY, SkewY);
	CC_SYNTHESIZE(cocos2d::CCFloat, m_fRotation, Rotation);
	CC_SYNTHESIZE(cocos2d::CCFloat, m_fAlpha, Alpha);
	
public:
	FCTimelineSprite(void);
	virtual ~FCTimelineSprite(void);
	virtual bool init(void);
	
	virtual bool init(const FCTimelineSprite * pTimelineSprite);
	virtual bool init(const std::string & sSymbolName,
					  const FCTimelineSpriteType iType,
					  const cocos2d::CCPoint & tPosition,
					  const cocos2d::CCPoint & tAnchorPoint,
					  const unsigned int iFirstFrame,
					  const FCTimelineSpriteLoopType iLoopType,
					  const cocos2d::CCAffineTransform tMatrix,
					  const cocos2d::CCFloat fScaleX,
					  const cocos2d::CCFloat fScaleY,
					  const cocos2d::CCFloat fSkewX,
					  const cocos2d::CCFloat fSkewY,
					  const cocos2d::CCFloat fRotation,
					  const cocos2d::CCFloat fAlpha);
	
	static FCTimelineSprite * timelineSprite(void);
	
	virtual cocos2d::CCObject * copyWithZone(cocos2d::CCZone * pZone);
};

NS_FC_END

#endif
