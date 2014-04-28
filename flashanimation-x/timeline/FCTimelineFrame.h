//
//  FCTimelineFrame.h
//  FlashAnimation-X
//
//  Created by Moky on 12-12-9.
//  Copyright 2012 Slanissue Inc. All rights reserved.
//

#ifndef FlashAnimation_X_FCTimelineFrame_h
#define FlashAnimation_X_FCTimelineFrame_h

#include "../fcMacros.h"

NS_FC_BEGIN

typedef enum {
	FCTimelineFrameTweenTypeNone,
	FCTimelineFrameTweenTypeMotion,
	FCTimelineFrameTweenTypeShape,
} FCTimelineFrameTweenType;

class FC_DLL FCTimelineFrame : public cocos2d::CCObject
{
	CC_SYNTHESIZE(FCTimelineFrameTweenType, m_iTweenType, TweenType);
	CC_SYNTHESIZE(unsigned int, m_iIndex, Index);
	CC_SYNTHESIZE(unsigned int, m_iDuration, Duration);
	CC_SYNTHESIZE_RETAIN(cocos2d::CCArray *, m_pSprites, Sprites);
	
public:
	FCTimelineFrame(void);
	virtual ~FCTimelineFrame(void);
	virtual bool init(void);
	
	virtual bool init(const FCTimelineFrame * pTimelineFrame);
	virtual bool init(const FCTimelineFrameTweenType iTweenType,
					  const unsigned int iIndex,
					  const unsigned int iDuration,
					  cocos2d::CCArray * pSprites);
	
	static FCTimelineFrame * timelineFrame(void);
	
	virtual cocos2d::CCObject * copyWithZone(cocos2d::CCZone * pZone);
};

NS_FC_END

#endif
