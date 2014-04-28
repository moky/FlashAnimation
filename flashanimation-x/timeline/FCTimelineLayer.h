//
//  FCTimelineLayer.h
//  FlashAnimation-X
//
//  Created by Moky on 12-12-9.
//  Copyright 2012 Slanissue Inc. All rights reserved.
//

#ifndef FlashAnimation_X_FCTimelineLayer_h
#define FlashAnimation_X_FCTimelineLayer_h

#include "FCTimelineFrame.h"
#include "FCTimelineSprite.h"
#include "FCTimeline.h"

NS_FC_BEGIN

typedef enum {
    FCTimelineLayerTypeNormal,
} FCTimelineLayerType;

class FC_DLL FCTimelineLayer : public cocos2d::CCObject
{
	CC_SYNTHESIZE(FCTimelineLayerType, m_iType, Type);
	CC_SYNTHESIZE_PASS_BY_REF(std::string, m_sName, Name);
	CC_SYNTHESIZE_RETAIN(cocos2d::CCArray *, m_pKeyFrames, KeyFrames);
	CC_SYNTHESIZE_RETAIN(FCTimeline *, m_pTimeline, Timeline);
	
public:
	FCTimelineLayer(void);
	virtual ~FCTimelineLayer(void);
	virtual bool init(void);
	
	static FCTimelineLayer * timelineLayer(void);
	
	unsigned int frameCount(void) const;
	cocos2d::CCArray * spritesOfFrameIndex(const unsigned int iFrameIndex) const;
	
protected:
	cocos2d::CCArray * spritesOfKeyFrame(FCTimelineFrame * pKeyFrame, const unsigned int iDuration) const;
	cocos2d::CCArray * spritesInMotionTweenFrame(FCTimelineFrame * pCurrentKeyFrame, FCTimelineFrame * pNextKeyFrame, const unsigned int iDuration) const;
	cocos2d::CCArray * spritesInShapeTweenFrame(FCTimelineFrame * pCurrentKeyFrame, FCTimelineFrame * pNextKeyFrame, const unsigned int iDuration) const;
	
	void setupMotionTweenSprite(FCTimelineSprite * pTweenSprite, FCTimelineSprite * pStartState, FCTimelineSprite * pEndState, const float fDelta) const;
	void setupShapeTweenSprite(FCTimelineSprite * pTweenSprite, FCTimelineSprite * pStartState, FCTimelineSprite * pEndState, const float fDelta) const;
};

NS_FC_END

#endif
