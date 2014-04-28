//
//  FCTimelineFrame.cpp
//  FlashAnimation-X
//
//  Created by Moky on 12-12-9.
//  Copyright 2012 Slanissue Inc. All rights reserved.
//

#include "FCTimelineFrame.h"

NS_FC_BEGIN

USING_NS_CC;

FCTimelineFrame::FCTimelineFrame(void)
: CCObject()
, m_iTweenType(FCTimelineFrameTweenTypeNone)
, m_iIndex(0)
, m_iDuration(1)
, m_pSprites(NULL)
{
	
}

FCTimelineFrame::~FCTimelineFrame(void)
{
	setSprites(NULL);
}

bool FCTimelineFrame::init(void)
{
	return init(FCTimelineFrameTweenTypeNone, 0, 1, CCArray::array());
}

bool FCTimelineFrame::init(const FCTimelineFrame * pTimelineFrame)
{
	return !pTimelineFrame ? init() : init(pTimelineFrame->getTweenType(),
										   pTimelineFrame->getIndex(),
										   pTimelineFrame->getDuration(),
										   pTimelineFrame->getSprites());
}

bool FCTimelineFrame::init(const FCTimelineFrameTweenType iTweenType,
						   const unsigned int iIndex,
						   const unsigned int iDuration,
						   CCArray * pSprites)
{
	if (!pSprites)
	{
		return false;
	}
	
	setTweenType(iTweenType);
	setIndex(iIndex);
	setDuration(iDuration);
	setSprites(pSprites);
	
	return true;
}

FCTimelineFrame * FCTimelineFrame::timelineFrame(void)
{
	FCTimelineFrame * pFrame = new FCTimelineFrame();
	if (pFrame && pFrame->init())
	{
		pFrame->autorelease();
		return pFrame;
	}
	CC_SAFE_DELETE(pFrame);
	return NULL;
}

CCObject * FCTimelineFrame::copyWithZone(CCZone * pZone)
{
	CCZone * pNewZone = NULL;
	FCTimelineFrame * pCopy = NULL;
	if (pZone && pZone->m_pCopyObject)
	{
		// incase of being called at sub class
		pCopy = (FCTimelineFrame *)(pZone->m_pCopyObject);
	}
	else
	{
		pCopy = new FCTimelineFrame();
		pZone = pNewZone = new CCZone(pCopy);
	}
	
	CCObject::copyWithZone(pZone);
	
	if (pCopy)
	{
		pCopy->init(this);
	}
	
	CC_SAFE_DELETE(pNewZone);
	return pCopy;
}

NS_FC_END
