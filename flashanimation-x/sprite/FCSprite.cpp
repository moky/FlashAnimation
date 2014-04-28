//
//  FCSprite.cpp
//  FlashAnimation-X
//
//  Created by Moky on 12-12-9.
//  Copyright 2012 Slanissue Inc. All rights reserved.
//

#include "FCSprite.h"

#include "FCMatrix.h"

NS_FC_BEGIN

USING_NS_CC;

FCSprite::FCSprite(void)
: CCSprite()
, m_sSymbolName("")
{
	
}

FCSprite::~FCSprite(void)
{
	setSymbolName("");
}

bool FCSprite::init(void)
{
	if (!CCSprite::init())
	{
		return false;
	}
	setSymbolName("");
	setAlpha(1);
	return true;
}

CCFloat FCSprite::getAlpha(void) const
{
	return m_nOpacity / 255.0;
}

void FCSprite::setAlpha(const CCFloat fAlpha)
{
	setOpacity(floorf(fAlpha * 255));
}

void FCSprite::reset(void)
{
	setScaleX(1.0);
	setScaleY(1.0);
	setSkewX(0.0);
	setSkewY(0.0);
	setRotation(0.0);
	setPosition(CCPointZero);
	setAnchorPoint(CCPointMake(0.5, 0.5));
	setAlpha(1.0);
}

CCAffineTransform FCSprite::nodeToParentTransform(void)
{
	if (!dynamic_cast<FCSprite *>(getParent()))
	{
		return CCSprite::nodeToParentTransform();
	}
	
	if (m_bIsTransformDirty)
	{
		m_tTransform = CCAffineTransformIdentity;
		
		if (!m_bIsRelativeAnchorPoint && !CCPoint::CCPointEqualToPoint(m_tAnchorPointInPoints, CCPointZero))
		{
			m_tTransform = CCAffineTransformTranslate(m_tTransform, m_tAnchorPointInPoints.x, m_tAnchorPointInPoints.y);
		}
		
		if (!CCPoint::CCPointEqualToPoint(m_tPosition, CCPointZero))
		{
			m_tTransform = CCAffineTransformTranslate(m_tTransform, m_tPosition.x, m_tPosition.y);
		}
		
		if (m_fSkewX != 0 || m_fSkewY != 0)
		{
			setMatrixSkewX(&m_tTransform, -m_fSkewX);
			setMatrixSkewY(&m_tTransform, -m_fSkewY);
		}
		else if (m_fRotation != 0)
		{
			setMatrixRotation(&m_tTransform, -m_fRotation);
		}
		
		if (m_fScaleX != 1 || m_fScaleY != 1)
		{
			setMatrixScaleX(&m_tTransform, m_fScaleX);
			setMatrixScaleY(&m_tTransform, m_fScaleY);
		}
		
		if (!CCPoint::CCPointEqualToPoint(m_tAnchorPointInPoints, CCPointZero))
		{
			m_tTransform = CCAffineTransformTranslate(m_tTransform, -m_tAnchorPointInPoints.x, -m_tAnchorPointInPoints.y);
		}
		
		m_bIsTransformDirty = false;
	}
	
	return m_tTransform;
}

NS_FC_END
