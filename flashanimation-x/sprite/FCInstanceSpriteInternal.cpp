//
//  FCInstanceSpriteInternal.cpp
//  FlashAnimation-X
//
//  Created by Moky on 12-12-10.
//  Copyright (c) 2012年 Slanissue Inc. All rights reserved.
//

#include "FCInstanceSprite.h"

#include "../FCAnimation.h"

NS_FC_BEGIN

USING_NS_CC;

CCArray * FCInstanceSprite::flatArrayOfCurrentSubspritesForEachLayer(void) const
{
	if (!m_pCurrentSubspritesForEachLayer)
	{
		FCLog("could not happen");
		return NULL;
	}
	CCArray * pAllFcspritesAtThisFrame = CCArray::arrayWithCapacity(m_pCurrentSubspritesForEachLayer->count() * 2);
	CCObject * obj = NULL;
	CCARRAY_FOREACH(m_pCurrentSubspritesForEachLayer, obj)
	{
		pAllFcspritesAtThisFrame->addObjectsFromArray((CCArray *)obj);
	}
	return pAllFcspritesAtThisFrame;
}

CCArray * FCInstanceSprite::fcspritesAtFrameIndex(const unsigned int iFrameIndex)
{
	CCArray * pTimelineSpritesForEachLayer = m_pTimeline->spritesForEachLayerOfFrameIndex(iFrameIndex);
	return fcspritesWithTimelineSprites(pTimelineSpritesForEachLayer);
}

CCArray * FCInstanceSprite::fcspritesWithTimelineSprites(CCArray * pTimelineSpritesForEachLayer)
{
	CCAssert(pTimelineSpritesForEachLayer, "timeline sprites cannot be NULL");
	
	if (!m_pCurrentSubspritesForEachLayer)
	{
		CCArray * pFcspritesForEachLayer = createFcspritesWithTimelineSpritesForEachLayer(pTimelineSpritesForEachLayer);
		setCurrentSubspritesForEachLayer(pFcspritesForEachLayer);
	}
	else
	{
		//因为_currentSubspritesForEachLayer是之前生成的层数据，而每一桢的层数肯定是一样的，这里断言一下这两个数组的count一定相等，确保没有逻辑错误。因为下面要做的循环，如果数组长度不一样，可能会出现数组越界的情况
		CCAssert(pTimelineSpritesForEachLayer->count() == m_pCurrentSubspritesForEachLayer->count(), "layer count must equal");
		
		unsigned int iLayerCount = pTimelineSpritesForEachLayer->count();
		CCArray * pFcspritesForEachLayer = CCArray::arrayWithCapacity(iLayerCount);
		for (int i = 0; i < iLayerCount; i++)
		{
			CCArray * pLayerTimelineSprites = (CCArray *)pTimelineSpritesForEachLayer->objectAtIndex(i);
			CCArray * pLayerOldFcsprites = (CCArray *)m_pCurrentSubspritesForEachLayer->objectAtIndex(i);
			if (!pLayerOldFcsprites)
			{
				continue;
			}
			CCArray * pNewLayerFcsprites = createFcspritesWithTimelineSprites(pLayerTimelineSprites, pLayerOldFcsprites);
			
			CCObject * obj = NULL;
			CCARRAY_FOREACH(pLayerOldFcsprites, obj)
			{
				m_pAnimation->recycleSprite((FCSprite *)obj);
				// TODO: ?
			}
			
			pFcspritesForEachLayer->addObject(pNewLayerFcsprites);
		}
		
		setCurrentSubspritesForEachLayer(pFcspritesForEachLayer);
	}
	
	return flatArrayOfCurrentSubspritesForEachLayer();
}

CCArray * FCInstanceSprite::createFcspritesWithTimelineSpritesForEachLayer(CCArray * pTimelineSpritesForEachLayer)
{
	unsigned int iLayerCout = pTimelineSpritesForEachLayer->count();
	CCArray * pNewFcspritesForEachLayer = CCArray::arrayWithCapacity(iLayerCout);
	for (int i = 0; i < iLayerCout; i++)
	{
		CCArray * pLayerTimelineSprites = (CCArray *)pTimelineSpritesForEachLayer->objectAtIndex(i);
		CCArray * pNewLayerFcsprites = createFcspritesWithTimelineSprites(pLayerTimelineSprites);
		pNewFcspritesForEachLayer->addObject(pNewLayerFcsprites);
	}
	return pNewFcspritesForEachLayer;
}

CCArray * FCInstanceSprite::createFcspritesWithTimelineSprites(CCArray * pTimelineSprites)
{
	CCArray * pFcsprites = CCArray::arrayWithCapacity(pTimelineSprites->count());
	CCObject * obj = NULL;
	CCARRAY_FOREACH(pTimelineSprites, obj)
	{
		FCSprite * pFcsprite = fcspriteWithTimelineSprite((FCTimelineSprite *)obj);
		if (!pFcsprite)
		{
			continue;
		}
		pFcsprites->addObject(pFcsprite);
	}
	return pFcsprites;
}

FCSprite * FCInstanceSprite::fcspriteWithTimelineSprite(FCTimelineSprite * pTimelineSprite)
{
	FCSprite * pFcsprite = m_pAnimation->spriteWithSymbolName(pTimelineSprite->getSymbolName());
	setupFcspriteWithTimelineSprite(pFcsprite, pTimelineSprite);
	if (FCInstanceSprite * pInstanceSprite = dynamic_cast<FCInstanceSprite *>(pFcsprite))
	{
		if (pInstanceSprite->getType() == FCInstanceSpriteMovicClip)
		{
			pInstanceSprite->play(0);
		}
	}
	return pFcsprite;
}

void FCInstanceSprite::setupFcspriteWithTimelineSprite(FCSprite * pFcsprite, FCTimelineSprite * pTimelineSprite) const
{
	if (!pFcsprite)
	{
//		FCLog("failed to setupFcspriteWithTimelineSprite, pFcsprite is NULL");
		return;
	}
	pFcsprite->setPosition(pTimelineSprite->getPosition());
	pFcsprite->setAnchorPoint(pTimelineSprite->getAnchorPoint());
	pFcsprite->setScaleX(pTimelineSprite->getScaleX());
	pFcsprite->setScaleY(pTimelineSprite->getScaleY());
	pFcsprite->setSkewX(pTimelineSprite->getSkewX());
	pFcsprite->setSkewY(pTimelineSprite->getSkewY());
	pFcsprite->setRotation(pTimelineSprite->getRotation());
	pFcsprite->setAlpha(pTimelineSprite->getAlpha());
	
	if (FCInstanceSprite * pInstanceSprite = dynamic_cast<FCInstanceSprite *>(pFcsprite))
	{
		FCTimelineSpriteType iType = pTimelineSprite->getType();
		if (iType == FCTimelineSpriteTypeShape)
		{
			// do nothing
		}
		else if (iType == FCTimelineSpriteTypeGraphic)
		{
			pInstanceSprite->stop();
			pInstanceSprite->setType(FCInstanceSpriteGraphic);
			
			FCTimelineSpriteLoopType iLoopType = pTimelineSprite->getLoopType();
			if (iLoopType == FCTimelineSpriteLoopTypeLoop)
			{
				pInstanceSprite->setLoopType(FCInstanceLoopTypeLoop);
			}
			else if (iLoopType == FCTimelineSpriteLoopTypeOnce)
			{
				pInstanceSprite->setLoopType(FCInstanceLoopTypeOnce);
			}
			else
			{
				pInstanceSprite->setLoopType(FCInstanceLoopTypeSingleFrame);
			}
			pInstanceSprite->gotoFrame(pTimelineSprite->getFirstFrame());
		}
		else
		{
			bool bNeedPlay = pInstanceSprite->getType() == FCInstanceSpriteGraphic;
			pInstanceSprite->setType(FCInstanceSpriteMovicClip);
			if (bNeedPlay)
			{
				pInstanceSprite->play(0);
			}
		}
	}
}

CCArray * FCInstanceSprite::createFcspritesWithTimelineSprites(CCArray * pTimelineSprites, CCArray * pReuseFcsprites)
{
	CCArray * pNewFcsprites = CCArray::arrayWithCapacity(pTimelineSprites->count());
	CCObject * obj = NULL;
	CCARRAY_FOREACH(pTimelineSprites, obj)
	{
		FCTimelineSprite * pTimelineSprite = (FCTimelineSprite *)obj;
		FCSprite * pFcsprite = NULL;
		
		int iOldCount = pReuseFcsprites->count();
		for (int i = 0; i < iOldCount; i++)
		{
			pFcsprite = (FCSprite *)pReuseFcsprites->objectAtIndex(i);
			if (FCStringEqualToString(pFcsprite->getSymbolName(), pTimelineSprite->getSymbolName()))
			{
				pFcsprite->retain();
				pFcsprite->autorelease();
				pReuseFcsprites->removeObjectAtIndex(i);
				break;
			}
			pFcsprite = NULL;
		}
		
		if (pFcsprite)
		{
			setupFcspriteWithTimelineSprite(pFcsprite, pTimelineSprite);
		}
		else
		{
			pFcsprite = fcspriteWithTimelineSprite(pTimelineSprite);
			if (!pFcsprite)
			{
//				FCLog("failed to get fcsprite with timelinesprite");
				continue;
			}
		}
		pNewFcsprites->addObject(pFcsprite);
	}
	return pNewFcsprites;
}

NS_FC_END
