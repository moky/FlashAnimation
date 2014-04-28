//
//  FCInstanceSprite.cpp
//  FlashAnimation-X
//
//  Created by Moky on 12-12-10.
//  Copyright 2012 Slanissue Inc. All rights reserved.
//

#include "FCInstanceSprite.h"

#include "../FCAnimation.h"

NS_FC_BEGIN

USING_NS_CC;

FCInstanceSprite::FCInstanceSprite(void)
: FCSprite()
, m_pTimeline(NULL)
, m_pCurrentSubspritesForEachLayer(NULL)
, m_fLastElapsedTime(0)
, m_iShouldStopAt(-1)
, m_tShouldRepeatAtRange((NSRange){0, 0})
, m_iState(FCInstanceStatePaused)
, m_iCurrentFrameIndex(0)
, m_fSpeed(1)
, m_pControlDelegate(NULL)
, m_iType(FCInstanceSpriteMovicClip)
, m_iLoopType(FCInstanceLoopTypeLoop)
, m_pAnimation(NULL)
{
	
}

FCInstanceSprite::~FCInstanceSprite(void)
{
	setTimeline(NULL);
	setCurrentSubspritesForEachLayer(NULL);
	
	setControlDelegate(NULL);
	
	setAnimation(NULL);
}

bool FCInstanceSprite::init(void)
{
	if (!FCSprite::init())
	{
		return false;
	}
	
	setTimeline(NULL);
	setCurrentSubspritesForEachLayer(NULL);
	m_fLastElapsedTime = 0.0;
	m_iShouldStopAt = -1;
	m_tShouldRepeatAtRange = (NSRange){0, 0};
	
	m_iState = FCInstanceStatePaused;
	m_iCurrentFrameIndex = 0;
	m_fSpeed = 1.0;
	setControlDelegate(NULL);
	
	m_iType = FCInstanceSpriteMovicClip;
	m_iLoopType = FCInstanceLoopTypeLoop;
	setAnimation(NULL);
	
	return true;
}

bool FCInstanceSprite::init(FCTimeline * pTimeline)
{
	if (!init())
	{
		return false;
	}
	
	setTimeline(pTimeline);
	
	return true;
}

FCInstanceSprite * FCInstanceSprite::instanceSprite(FCTimeline * pTimeline)
{
	FCInstanceSprite * pSprite = new FCInstanceSprite();
	if (pSprite && pSprite->init(pTimeline))
	{
		pSprite->autorelease();
		return pSprite;
	}
	CC_SAFE_DELETE(pSprite);
	return NULL;
}

#pragma mark -

void FCInstanceSprite::setControlDelegate(FCInstanceSpriteDelegate * pDelegate)
{
	if (m_pControlDelegate != pDelegate)
	{
		FC_SAFE_RETAIN(pDelegate);
		FC_SAFE_RELEASE(m_pControlDelegate);
		m_pControlDelegate = pDelegate;
	}
}

FCInstanceSpriteDelegate * FCInstanceSprite::getControlDelegate(void)
{
	return m_pControlDelegate;
}

void FCInstanceSprite::setAnimation(FCAnimation * pAnimation)
{
	if (m_pAnimation != pAnimation)
	{
		FC_SAFE_RETAIN(pAnimation);
		FC_SAFE_RELEASE(m_pAnimation);
		m_pAnimation = pAnimation;
	}
}

FCAnimation * FCInstanceSprite::getAnimation(void)
{
	return m_pAnimation;
}

unsigned int FCInstanceSprite::frameCount(void) const
{
	return m_pTimeline ? m_pTimeline->getFrameCount() : 0;
}

#pragma mark -

void FCInstanceSprite::onEnter()
{
	FCSprite::onEnter();
	
	if (!m_pCurrentSubspritesForEachLayer)
	{
		gotoFrame(m_iCurrentFrameIndex);
	}
}

void FCInstanceSprite::onExit()
{
	m_iShouldStopAt = -1;
	m_tShouldRepeatAtRange = (NSRange){0, 0};
	
	FCSprite::onExit();
}

void FCInstanceSprite::cleanup()
{
	stop();
	FCSprite::cleanup();
}

void FCInstanceSprite::tick(ccTime dt)
{
	if (!m_bIsRunning)
	{
		return ;
	}
	
	m_fLastElapsedTime += dt; //_lastElapsedTime指的是从上一桢开始，到这个tick为止，所流逝的时间
	
	ccTime frameInterval = 1.0 / m_pAnimation->getFPS() / m_fSpeed; //连续连桢之间的时间间隔
	
	if (m_fLastElapsedTime >= frameInterval)
	{
		//如果流逝的时间大于一个桢的时间间隔的话，就可以进入下一桢了
		unsigned int iFalloverFrameCount = floorf(m_fLastElapsedTime / frameInterval); //算出流逝的帧数
		gotoFrame(m_iCurrentFrameIndex + iFalloverFrameCount); //跳到指定的桢数
		m_fLastElapsedTime = fmodf(m_fLastElapsedTime, frameInterval); //把_lastElapsedTime的值调整回来
	}
}

#pragma mark - FCSprite

void FCInstanceSprite::reset(void)
{
	FCSprite::reset();
	
	m_iState = FCInstanceStatePaused;
	m_iLoopType = FCInstanceLoopTypeLoop;
	m_iType = FCInstanceSpriteMovicClip;
	
	m_iCurrentFrameIndex = 0;
	m_fLastElapsedTime = 0.0;
	m_iShouldStopAt = -1;
	m_tShouldRepeatAtRange = (NSRange){0, 0};
	
	setCurrentSubspritesForEachLayer(NULL);
}

#pragma mark - movie control

void FCInstanceSprite::repeat(const NSRange & tFrameRange)
{
	unsigned int iFrameCount = m_pTimeline->getFrameCount();
	CCAssert(iFrameCount > 0, "frame count must > 0");
	unsigned int iActualLocation = tFrameRange.location % iFrameCount;
	unsigned int iActualLength = tFrameRange.length + iActualLocation > iFrameCount ? iFrameCount - iActualLocation : tFrameRange.length;
	if (iActualLength == 0)
	{
		return;
	}
	m_tShouldRepeatAtRange = (NSRange){0, 0};
	m_iShouldStopAt = -1;
	m_fLastElapsedTime = 0.0;
	gotoFrame(iActualLocation);
	m_tShouldRepeatAtRange = NSMakeRange(iActualLocation, iActualLength);
	play();
}

void FCInstanceSprite::repeat(const unsigned int iStartFrameIndex, const unsigned int iEndFrameIndex)
{
	repeat(NSMakeRange(iStartFrameIndex, iEndFrameIndex - iStartFrameIndex + 1));
}

void FCInstanceSprite::play(void)
{
	if (m_iType == FCInstanceSpriteMovicClip)
	{
		if (m_iState != FCInstanceStatePlaying)
		{
			//scheduleSelector(schedule_selector(CCTextureCache::addImageAsyncCallBack), this, 0, false);
			schedule(schedule_selector(FCInstanceSprite::tick));
			m_iState = FCInstanceStatePlaying;
			m_fLastElapsedTime = 0.0;
		}
	}
}

// gotoAndPlay:
void FCInstanceSprite::play(const unsigned int iStartFrameIndex)
{
	m_iShouldStopAt = -1;
	m_tShouldRepeatAtRange = (NSRange){0, 0};
	if (iStartFrameIndex != m_iCurrentFrameIndex)
	{
		gotoFrame(iStartFrameIndex);
	}
	play();
}

// gotoAndPlay:stopAt:
void FCInstanceSprite::play(const unsigned int iStartFrameIndex, const unsigned int iEndFrameIndex)
{
	m_iShouldStopAt = -1;
	m_tShouldRepeatAtRange = (NSRange){0, 0};
	m_fLastElapsedTime = 0.0;
	gotoFrame(iStartFrameIndex);
	m_iShouldStopAt = iEndFrameIndex % m_pTimeline->getFrameCount();
	play();
}

void FCInstanceSprite::stop(void)
{
	unscheduleAllSelectors();
	m_iState = FCInstanceStatePaused;
	m_fLastElapsedTime = 0.0;
}

// gotoAndStop:
void FCInstanceSprite::stop(const unsigned int iFrameIndex)
{
	m_iShouldStopAt = -1;
	m_tShouldRepeatAtRange = (NSRange){0, 0};
	stop();
	gotoFrame(iFrameIndex);
}

void FCInstanceSprite::gotoFrame(const unsigned int iFrameIndex)
{
	unsigned int iFrameCount = m_pTimeline->getFrameCount();
	CCAssert(iFrameCount > 0, "frame count must > 0");
	unsigned int iActualFrameIndex = 0;
	if (m_iType == FCInstanceSpriteGraphic && m_iLoopType == FCInstanceLoopTypeOnce)
	{
		iActualFrameIndex = iFrameIndex >= iFrameCount ? iFrameCount - 1 : iFrameIndex;
	}
	else
	{
		iActualFrameIndex = iFrameIndex % iFrameCount;
	}
	
	unsigned int iStopAtIndex = 0;
	bool bShouldStop = shouldStop(m_iCurrentFrameIndex, iFrameIndex, iActualFrameIndex,
								  &iStopAtIndex);
	if (bShouldStop)
	{
		stop();
		iActualFrameIndex = iStopAtIndex;
	}
	else
	{
		unsigned int iGotoFrameInRepeat = 0;
		bool bShouldRepeat = shouldRepeat(m_iCurrentFrameIndex, iFrameIndex, iActualFrameIndex,
										  &iGotoFrameInRepeat);
		if (bShouldRepeat)
		{
			iActualFrameIndex = iGotoFrameInRepeat;
		}
	}
	
	if (iActualFrameIndex == m_iCurrentFrameIndex && m_pCurrentSubspritesForEachLayer)
	{
		m_iCurrentFrameIndex = iActualFrameIndex;
		return ;
	}
	
	CCArray * pSprites = fcspritesAtFrameIndex(iActualFrameIndex);
	removeAllChildrenWithCleanup(false);
	CCObject * obj = NULL;
	CCARRAY_FOREACH(pSprites, obj)
	{
		FCSprite * sprite = (FCSprite *)obj;
		if (sprite->getParent())
		{
			sprite->removeFromParentAndCleanup(false);
		}
		addChild(sprite);
	}
	
	m_iCurrentFrameIndex = iActualFrameIndex;
	
	if (m_pControlDelegate)
	{
		m_pControlDelegate->instanceSpriteDidChanged(this, m_iCurrentFrameIndex);
	}
	
	if (bShouldStop && m_pControlDelegate)
	{
		m_pControlDelegate->instanceSpriteDidStopped(this, m_iCurrentFrameIndex);
	}
}

/*
 * 在跳转到下一桢时，判断是否应该暂停播放，如果应该暂停播放的话，则通过gotoFrameIndexRef返回应该呈现的帧
 *
 * @param NSUInteger currentFrameIndex 当前帧
 * @param NSUInteger targetFrameIndex 要跳转的目标桢, 这个值可能要比桢总数还大，超出桢总数的部分是应该回到动画的开始部分开始播放
 * @param NSUInteger actualTargetFrameIndex 实际要跳转的目标桢，这个值应该是targetFrameIndex对总帧数取模后的值
 * @param NSUInteger *gotoFrameIndexRef 如果需要暂停播放的话，通过这个值返回实际暂停的桢
 *
 * @return BOOL 如果应该跳转到其他桢时（非目标桢），则返回YES，否则返回NO
 */
bool FCInstanceSprite::shouldStop(const unsigned int iCurrentFameIndex, const unsigned int iTargetFrameIndex, const unsigned int iActualTargetFrameIndex, unsigned int *iStopFrameIndex)
{
	if (m_iShouldStopAt < 0)
	{
		//_shouldStopAt少于0的话，表明没有设置过暂停的桢
		return false;
	}
	
	if (iCurrentFameIndex <= m_iShouldStopAt)
	{
		if (iActualTargetFrameIndex >= m_iShouldStopAt)
		{
			if (iStopFrameIndex)
			{
				*iStopFrameIndex = m_iShouldStopAt;
			}
			m_iShouldStopAt = -1;
			return true;
		}
		else
		{
			if (iActualTargetFrameIndex < iTargetFrameIndex)
			{
				//目标桢越过最大桢，回到开始时播放，这时候是肯定越过了暂停的桢
				if (iStopFrameIndex)
				{
					*iStopFrameIndex = m_iShouldStopAt;
				}
				m_iShouldStopAt = -1;
				return true;
			}
		}
	}
	else
	{
		unsigned int iFrameCount = m_pTimeline->getFrameCount();
		if (iTargetFrameIndex > iFrameCount)
		{
			if (iTargetFrameIndex - iFrameCount > m_iShouldStopAt)
			{
				if (iStopFrameIndex)
				{
					*iStopFrameIndex = m_iShouldStopAt;
				}
				m_iShouldStopAt = -1;
				return true;
			}
		}
	}
	return false;
}

/*
 * 在跳转到下一桢时，判断是否应该循环播放，如果应该循环播放的话，则通过gotoFrameIndexRef返回应该播放的帧
 * 
 * @param NSUInteger currentFrameIndex 当前帧
 * @param NSUInteger targetFrameIndex 要跳转的目标桢, 这个值可能要比桢总数还大，超出桢总数的部分是应该回到动画的开始部分开始播放
 * @param NSUInteger actualTargetFrameIndex 实际要跳转的目标桢，这个值应该是targetFrameIndex对总帧数取模后的值
 * @param NSUInteger *gotoFrameIndexRef 如果需要循环播放的话，通过这个值返回需要实际跳转的桢
 *
 * @return BOOL 如果应该跳转到其他桢时（非目标桢），则返回YES，否则返回NO
 *
 */
bool FCInstanceSprite::shouldRepeat(const unsigned int iCurrentFrameIndex, const unsigned int iTargetFrameIndex, const unsigned int iActualTargetFrameIndex, unsigned int *iGotoFrameIndex)
{
	if (m_tShouldRepeatAtRange.length <= 0)
	{
		//_shouldRepeatAtRange的长度为0表面没有设置过循环播放
		return false;
	}
	
	unsigned int iRangeStartFrameIndex = m_tShouldRepeatAtRange.location;
	unsigned int iRangeEndFrameIndex = m_tShouldRepeatAtRange.location + m_tShouldRepeatAtRange.length - 1;
	CCAssert(iRangeEndFrameIndex >= iRangeStartFrameIndex, @"end index must larger than start index");
	CCAssert(iRangeStartFrameIndex <= m_iCurrentFrameIndex && m_iCurrentFrameIndex <= iRangeEndFrameIndex, @"current frame index should in repeat range");
	
	if (iRangeStartFrameIndex == iRangeEndFrameIndex)
	{
		//在同一桢里循环，肯定是跳转到同一桢里
		if (iGotoFrameIndex)
		{
			*iGotoFrameIndex = iRangeStartFrameIndex;
		}
		return true;
	}
	else
	{
		if (iTargetFrameIndex < iRangeStartFrameIndex)
		{
			//如果目标桢是少于循环的开始桢的话，跳到循环的开始桢播放
			if (iGotoFrameIndex)
			{
				*iGotoFrameIndex = iRangeStartFrameIndex;
			}
			return true;
		}
		else if (iTargetFrameIndex <= iRangeEndFrameIndex)
		{
			//如果目标桢大于等于循环开始桢，少于等于循环结束桢，那表明当前动画还在循环的桢范围中，不需要跳转
			return false;
		}
		else
		{
			//如果目标桢大于循环结束桢，那表明需要回到循环开始桢处来开始播放
			unsigned int iExceedFrameCount = iTargetFrameIndex - iRangeEndFrameIndex;
			unsigned int iElapseFrameInRepeat = (iExceedFrameCount - 1) % m_tShouldRepeatAtRange.length;
			unsigned int iShouldGotoFrameInRepeat = iRangeStartFrameIndex + iElapseFrameInRepeat;
			if (iGotoFrameIndex)
			{
				*iGotoFrameIndex = iShouldGotoFrameInRepeat;
			}
			return true;
		}
	}
}

NS_FC_END
