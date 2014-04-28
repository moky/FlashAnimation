//
//  FCInstanceSprite.h
//  FlashAnimation-X
//
//  Created by Moky on 12-12-10.
//  Copyright 2012 Slanissue Inc. All rights reserved.
//

#ifndef FlashAnimation_X_FCInstanceSprite_h
#define FlashAnimation_X_FCInstanceSprite_h

#include "FCSprite.h"
#include "../timeline/FCTimeLine.h"

#include "FCInstanceSpriteDelegate.h"

#include "../timeline/FCTimelineSprite.h"

NS_FC_BEGIN

class FCAnimation;

typedef enum {
    FCInstanceStatePlaying,
    FCInstanceStatePaused,
} FCInstanceState;

// internal
typedef enum {
    FCInstanceSpriteMovicClip,
    FCInstanceSpriteGraphic,
} FCInstanceType;

// internal
typedef enum {
    FCInstanceLoopTypeOnce,
    FCInstanceLoopTypeLoop,
    FCInstanceLoopTypeSingleFrame,
} FCInstanceLoopType;

#pragma mark - FCInstanceSprite

class FC_DLL FCInstanceSprite : public FCSprite
{
	// private
	CC_SYNTHESIZE_RETAIN(FCTimeline *, m_pTimeline, Timeline);
	CC_SYNTHESIZE_RETAIN(cocos2d::CCArray *, m_pCurrentSubspritesForEachLayer, CurrentSubspritesForEachLayer);
	CC_SYNTHESIZE(cocos2d::ccTime, m_fLastElapsedTime, LastElapsedTime);
	CC_SYNTHESIZE(int, m_iShouldStopAt, ShouldStopAt);
	CC_SYNTHESIZE_PASS_BY_REF(NSRange, m_tShouldRepeatAtRange, ShouldRepeatAtRange);
	
	// public
	CC_SYNTHESIZE(FCInstanceState, m_iState, State);
	CC_SYNTHESIZE(unsigned int, m_iCurrentFrameIndex, CurrentFrameIndex);
	CC_SYNTHESIZE(cocos2d::CCFloat, m_fSpeed, Speed);
	CC_PROPERTY(FCInstanceSpriteDelegate *, m_pControlDelegate, ControlDelegate);
	
	// private
	CC_SYNTHESIZE(FCInstanceType, m_iType, Type);
	CC_SYNTHESIZE(FCInstanceLoopType, m_iLoopType, LoopType);
	CC_PROPERTY(FCAnimation *, m_pAnimation, Animation);
	
public:
	unsigned int frameCount(void) const;
	
public:
	FCInstanceSprite(void);
	virtual ~FCInstanceSprite(void);
	virtual bool init(void);
	
	virtual bool init(FCTimeline * pTimeline);
	
	static FCInstanceSprite * instanceSprite(FCTimeline * pTimeline);
	
	virtual void onEnter();
	virtual void onExit();
	
	virtual void cleanup();
	
#pragma mark -
	
	virtual void tick(cocos2d::ccTime dt);
	
	// 
	virtual void reset(void);
	
	// controls
	void repeat(const NSRange & tFrameRange);
	void repeat(const unsigned int iStartFrameIndex, const unsigned int iEndFrameIndex);
	
	void play(void);
	void play(const unsigned int iStartFrameIndex);
	void play(const NSRange & tRange);
	void play(const unsigned int iStartFrameIndex, const unsigned int iEndFrameIndex);
	
	void stop(void);
	void stop(const unsigned int iFrameIndex);
	
protected:
	void gotoFrame(const unsigned int iFrameIndex);
	
	bool shouldStop(const unsigned int iCurrentFameIndex,
					const unsigned int iTargetFrameIndex,
					const unsigned int iActualTargetFrameIndex,
					unsigned int * iStopFrameIndex);
	
	bool shouldRepeat(const unsigned int iCurrentFrameIndex,
					  const unsigned int iTargetFrameIndex,
					  const unsigned int iActualTargetFrameIndex,
					  unsigned int * iGotoFrameIndex);
	
public:
	cocos2d::CCArray * flatArrayOfCurrentSubspritesForEachLayer(void) const;
	
#pragma mark - Internal
	
protected:
	cocos2d::CCArray * fcspritesAtFrameIndex(const unsigned int iFrameIndex);
	cocos2d::CCArray * fcspritesWithTimelineSprites(cocos2d::CCArray * pTimelineSpritesForEachLayer);
	
	cocos2d::CCArray * createFcspritesWithTimelineSpritesForEachLayer(cocos2d::CCArray * pTimelineSpritesForEachLayer);
	cocos2d::CCArray * createFcspritesWithTimelineSprites(cocos2d::CCArray * pTimelineSprites);
	
	FCSprite * fcspriteWithTimelineSprite(FCTimelineSprite * pTimelineSprite);
	void setupFcspriteWithTimelineSprite(FCSprite * pFcsprite, FCTimelineSprite * pTimelineSprite) const;
	
	cocos2d::CCArray * createFcspritesWithTimelineSprites(cocos2d::CCArray * pTimelineSprites, cocos2d::CCArray * pReuseFcsprites);
};

NS_FC_END

#endif
