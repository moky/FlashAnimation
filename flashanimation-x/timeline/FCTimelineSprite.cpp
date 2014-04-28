//
//  FCTimelineSprite.cpp
//  FlashAnimation-X
//
//  Created by Moky on 12-12-9.
//  Copyright 2012 Slanissue Inc. All rights reserved.
//

#include "FCTimelineSprite.h"

NS_FC_BEGIN

USING_NS_CC;

FCTimelineSprite::FCTimelineSprite(void)
: CCObject()
, m_sSymbolName("")
, m_iType(FCTimelineSpriteTypeShape)
, m_tPosition(CCPointZero)
, m_tAnchorPoint(CCPointMake(0.5, 0.5))
, m_iFirstFrame(0)
, m_iLoopType(FCTimelineSpriteLoopTypeLoop)
, m_tMatrix(CCAffineTransformIdentity)
, m_fScaleX(1)
, m_fScaleY(1)
, m_fSkewX(0)
, m_fSkewY(0)
, m_fRotation(0)
, m_fAlpha(1)
{
	
}

FCTimelineSprite::~FCTimelineSprite(void)
{
	setSymbolName("");
}

bool FCTimelineSprite::init(void)
{
	return init("", FCTimelineSpriteTypeShape,
				CCPointZero, CCPointMake(0.5, 0.5), 0,
				FCTimelineSpriteLoopTypeLoop, CCAffineTransformIdentity,
				1.0, 1.0, 0.0, 0.0, 0.0, 1.0);
}

bool FCTimelineSprite::init(const FCTimelineSprite * pTimelineSprite)
{
	return !pTimelineSprite ? init() : init(pTimelineSprite->getSymbolName(),
											pTimelineSprite->getType(),
											pTimelineSprite->getPosition(),
											pTimelineSprite->getAnchorPoint(),
											pTimelineSprite->getFirstFrame(),
											pTimelineSprite->getLoopType(),
											pTimelineSprite->getMatrix(),
											pTimelineSprite->getScaleX(),
											pTimelineSprite->getScaleY(),
											pTimelineSprite->getSkewX(),
											pTimelineSprite->getSkewY(),
											pTimelineSprite->getRotation(),
											pTimelineSprite->getAlpha());
}

bool FCTimelineSprite::init(const std::string & sSymbolName,
							const FCTimelineSpriteType iType,
							const CCPoint & tPosition,
							const CCPoint & tAnchorPoint,
							const unsigned int iFirstFrame,
							const FCTimelineSpriteLoopType iLoopType,
							const CCAffineTransform tMatrix,
							const CCFloat fScaleX,
							const CCFloat fScaleY,
							const CCFloat fSkewX,
							const CCFloat fSkewY,
							const CCFloat fRotation,
							const CCFloat fAlpha)
{
	setSymbolName(sSymbolName);
	setType(iType);
	
	setPosition(tPosition);
	setAnchorPoint(tAnchorPoint);
	setFirstFrame(iFirstFrame);
	setLoopType(iLoopType);
	setMatrix(tMatrix);
	
	setScaleX(fScaleX);
	setScaleY(fScaleY);
	setSkewX(fSkewX);
	setSkewY(fSkewY);
	setRotation(fRotation);
	setAlpha(fAlpha);
	
	return true;
}

FCTimelineSprite * FCTimelineSprite::timelineSprite(void)
{
	FCTimelineSprite * pSprite = new FCTimelineSprite();
	if (pSprite && pSprite->init())
	{
		pSprite->autorelease();
		return pSprite;
	}
	CC_SAFE_DELETE(pSprite);
	return NULL;
}

CCObject * FCTimelineSprite::copyWithZone(CCZone * pZone)
{
	CCZone * pNewZone = NULL;
	FCTimelineSprite * pCopy = NULL;
	if (pZone && pZone->m_pCopyObject)
	{
		// incase of being called at sub class
		pCopy = (FCTimelineSprite *)(pZone->m_pCopyObject);
	}
	else
	{
		pCopy = new FCTimelineSprite();
		pZone = pNewZone = new CCZone(pCopy);
	}
	
	//CCObject::copyWithZone(pZone);
	
	if (pCopy)
	{
		pCopy->init(this);
	}
	
	CC_SAFE_DELETE(pNewZone);
	return pCopy;
}

NS_FC_END
