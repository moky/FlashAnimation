//
//  FCTimeline.cpp
//  FlashAnimation-X
//
//  Created by Moky on 12-12-10.
//  Copyright (c) 2012年 Slanissue Inc. All rights reserved.
//

#include "FCTimeline.h"

#include "FCTimelineLayer.h"

NS_FC_BEGIN

USING_NS_CC;

FCTimeline::FCTimeline(void)
: CCObject()
, m_pSymbol(NULL)
, m_iFrameCount(0)
, m_pLayers(NULL)
{
	
}

FCTimeline::~FCTimeline(void)
{
	setSymbol(NULL);
	setLayers(NULL);
}

bool FCTimeline::init(void)
{
	setSymbol(NULL);
	setFrameCount(0);
	setLayers(NULL);
	return true;
}

bool FCTimeline::init(CCArray * pLayers)
{
	if (!init())
	{
		return false;
	}
	setLayers(pLayers);
	
	CCObject * obj = NULL;
	CCARRAY_FOREACH(m_pLayers, obj)
	{
		FCTimelineLayer * pLayer = (FCTimelineLayer *)obj;
		pLayer->setTimeline(this);
		unsigned int iLayerFrameCount = pLayer->frameCount();
		if (iLayerFrameCount > m_iFrameCount)
		{
			m_iFrameCount = iLayerFrameCount;
		}
	}
	
	return true;
}

FCTimeline * FCTimeline::timeline(CCArray * pLayers)
{
	FCTimeline * pTimeline = new FCTimeline();
	if (pTimeline && pTimeline->init(pLayers))
	{
		pTimeline->autorelease();
		return pTimeline;
	}
	CC_SAFE_DELETE(pTimeline);
	return NULL;
}

CCArray * FCTimeline::spritesForEachLayerOfFrameIndex(const unsigned int iIndex) const
{
	CCArray * pSpritesForEachLayer = CCArray::arrayWithCapacity(m_pLayers->count());
	
	CCObject * obj = NULL;
	CCARRAY_FOREACH(m_pLayers, obj)
	{
		FCTimelineLayer * pLayer = (FCTimelineLayer *)obj;
		CCArray * pSprites = pLayer->spritesOfFrameIndex(iIndex);
		if (!pSprites)
		{
			continue;
		}
		pSpritesForEachLayer->addObject(pSprites);
	}
	
	return pSpritesForEachLayer;
}

NS_FC_END
