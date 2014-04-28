//
//  FCLibrarySymbolBuilder.cpp
//  FlashAnimation-X
//
//  Created by Moky on 12-12-10.
//  Copyright 2012 Slanissue Inc. All rights reserved.
//

#include "FCLibrarySymbolBuilder.h"

NS_FC_BEGIN

USING_NS_CC;

static FCLibrarySymbolBuilder * s_pSharedSymbolBuilder = NULL;

FCLibrarySymbolBuilder * FCLibrarySymbolBuilder::symbolBuilder(void)
{
	// lock
	if (!s_pSharedSymbolBuilder)
	{
		s_pSharedSymbolBuilder = new FCLibrarySymbolBuilder();
	}
	// unlock
	return s_pSharedSymbolBuilder;
}

FCLibrarySymbol * FCLibrarySymbolBuilder::buildSymbol(CCDictionary * pSymbolData) const
{
	CCString * pType = (CCString *)pSymbolData->objectForKey("type");
	CCString * pName = (CCString *)pSymbolData->objectForKey("id");
	CCString * pWidth = (CCString *)pSymbolData->objectForKey("width");
	CCString * pHeight = (CCString *)pSymbolData->objectForKey("height");
	
	if (!pType || !pName || !pWidth || !pHeight)
	{
		return NULL;
	}
	
	FCLibrarySymbol * pSymbol = FCLibrarySymbol::librarySymbol();
	
	pSymbol->setName(pName->getCString());
	pSymbol->setContentSize(CCSizeMake(pWidth->floatValue(), pHeight->floatValue()));
	
	if (FCStringEqualToString(pType, "shape"))
	{
		CCString * pFile = (CCString *)pSymbolData->objectForKey("file");
		CCAssert(pFile, "shape sprite must has an image file");
		pSymbol->setType(FCLibrarySymbolTypeShape);
		pSymbol->setTextureName(pFile->getCString());
		return pSymbol;
	}
	else if (FCStringEqualToString(pType, "instance"))
	{
		CCArray * pLayers = (CCArray *)pSymbolData->objectForKey("layers");
		FCTimeline * pTimeline = buildTimeline(pLayers);
		
		pSymbol->setType(FCLibrarySymbolTypeMovie);
		pSymbol->setTimeline(pTimeline);
		
		if (pTimeline)
		{
			pTimeline->setSymbol(pSymbol);
		}
		
		return pSymbol;
	}
	else
	{
		CCAssert(false, "can not reach here");
		return NULL;
	}
}

#pragma mark - timeline

FCTimeline * FCLibrarySymbolBuilder::buildTimeline(CCArray * pLayers) const
{
	CCArray * array = CCArray::arrayWithCapacity(pLayers ? pLayers->count() : 1);
	CCObject * obj = NULL;
	CCARRAY_FOREACH(pLayers, obj)
	{
		CCDictionary * pLayerData = (CCDictionary *)obj;
		FCTimelineLayer * pLayer = buildTimelineLayer(pLayerData);
		array->addObject(pLayer);
	}
	
	FCTimeline * pTimeline = FCTimeline::timeline(array);
	return pTimeline;
}

FCTimelineLayer * FCLibrarySymbolBuilder::buildTimelineLayer(CCDictionary * pLayerData) const
{
	FCTimelineLayer * pLayer = FCTimelineLayer::timelineLayer();
	
	CCString * pName = (CCString *)pLayerData->objectForKey("name");
	if (pName)
	{
		pLayer->setName(pName->getCString());
	}
	
	CCString * pType = (CCString *)pLayerData->objectForKey("type");
	if (pType)
	{
		if (FCStringEqualToString(pType, "normal"))
		{
			pLayer->setType(FCTimelineLayerTypeNormal);
		}
	}
	
	CCArray * pFrames = (CCArray *)pLayerData->objectForKey("frames");
	if (pFrames && pFrames->count() > 0)
	{
		CCArray * pKeyFrames = CCArray::arrayWithCapacity(pFrames->count());
		CCObject * obj = NULL;
		CCARRAY_FOREACH(pFrames, obj)
		{
			FCTimelineFrame * pFrame = buildTimelineFrame((CCDictionary *)obj);
			pKeyFrames->addObject(pFrame);
		}
		pLayer->setKeyFrames(pKeyFrames);
	}
	
	return pLayer;
}

FCTimelineFrame * FCLibrarySymbolBuilder::buildTimelineFrame(CCDictionary * pFrameData) const
{
	FCTimelineFrame * pFrame = FCTimelineFrame::timelineFrame();
	
	CCString * pTweenType = (CCString *)pFrameData->objectForKey("type");
	if (!pTweenType)
	{
		// do nothing
	}
	else if (FCStringEqualToString(pTweenType, "none"))
	{
		pFrame->setTweenType(FCTimelineFrameTweenTypeNone);
	}
	else if (FCStringEqualToString(pTweenType, "motion"))
	{
		pFrame->setTweenType(FCTimelineFrameTweenTypeMotion);
	}
	else if (FCStringEqualToString(pTweenType, "shape"))
	{
		pFrame->setTweenType(FCTimelineFrameTweenTypeShape);
	}
	
	CCString * pStartFrame = (CCString *)pFrameData->objectForKey("startFrame");
	if (pStartFrame)
	{
		pFrame->setIndex(pStartFrame->uintValue());
	}
	
	CCString * pDuration = (CCString *)pFrameData->objectForKey("duration");
	if (pDuration)
	{
		pFrame->setDuration(pDuration->uintValue());
	}
	
	CCArray * pSprites = (CCArray *)pFrameData->objectForKey("sprites");
	if (pSprites && pSprites->count() > 0)
	{
		CCArray * array = CCArray::arrayWithCapacity(pSprites->count());
		CCObject * obj = NULL;
		CCARRAY_FOREACH(pSprites, obj)
		{
			FCTimelineSprite * pSprite = buildTimelineSprite((CCDictionary *)obj);
			array->addObject(pSprite);
		}
		pFrame->setSprites(array);
	}
	
	return pFrame;
}

FCTimelineSprite * FCLibrarySymbolBuilder::buildTimelineSprite(CCDictionary * pSpriteData) const
{
	FCTimelineSprite * pSprite = FCTimelineSprite::timelineSprite();
	
	CCString * pSymbolName = (CCString *)pSpriteData->objectForKey("id");
	if (pSymbolName)
	{
		pSprite->setSymbolName(pSymbolName->getCString());
	}
	
	CCString * pType = (CCString *)pSpriteData->objectForKey("type");
	if (!pType)
	{
		// do nothing
	}
	else if (FCStringEqualToString(pType, "shape"))
	{
		pSprite->setType(FCTimelineSpriteTypeShape);
	}
	else if (FCStringEqualToString(pType, "graphic"))
	{
		pSprite->setType(FCTimelineSpriteTypeGraphic);
	}
	else if (FCStringEqualToString(pType, "movie clip"))
	{
		pSprite->setType(FCTimelineSpriteTypeMovieClip);
	}
	
	CCString * pFirstFrame = (CCString *)pSpriteData->objectForKey("firstFrame");
	if (pFirstFrame)
	{
		pSprite->setFirstFrame(pFirstFrame->uintValue());
	}
	
	CCString * pLoop = (CCString *)pSpriteData->objectForKey("loop");
	if (!pLoop)
	{
		// do nothing
	}
	else if (FCStringEqualToString(pLoop, "loop"))
	{
		pSprite->setLoopType(FCTimelineSpriteLoopTypeLoop);
	}
	else if (FCStringEqualToString(pLoop, "play once"))
	{
		pSprite->setLoopType(FCTimelineSpriteLoopTypeOnce);
	}
	else if (FCStringEqualToString(pLoop, "single frame"))
	{
		pSprite->setLoopType(FCTimelineSpriteLoopTypeSingleFrame);
	}
	
	CCDictionary * pMatrix = (CCDictionary *)pSpriteData->objectForKey("matrix");
	if (pMatrix)
	{
		CCString * a = (CCString *)pMatrix->objectForKey("a");
		CCString * b = (CCString *)pMatrix->objectForKey("b");
		CCString * c = (CCString *)pMatrix->objectForKey("c");
		CCString * d = (CCString *)pMatrix->objectForKey("d");
		CCString * tx = (CCString *)pMatrix->objectForKey("tx");
		CCString * ty = (CCString *)pMatrix->objectForKey("ty");
		CCAffineTransform matrix = CCAffineTransformMake(a->floatValue(),
														 b->floatValue(),
														 c->floatValue(),
														 d->floatValue(),
														 tx->floatValue(),
														 ty->floatValue());
		pSprite->setMatrix(matrix);
	}
	
	// position
	CCString * pX = (CCString *)pSpriteData->objectForKey("x");
	CCString * pY = (CCString *)pSpriteData->objectForKey("y");
	if (pX || pY)
	{
		pSprite->setPosition(CCPointMake(pX->floatValue(), pY->floatValue()));
	}
	
	// anchor point
	CCString * pAnchorX = (CCString *)pSpriteData->objectForKey("anchorX");
	CCString * pAnchorY = (CCString *)pSpriteData->objectForKey("anchorY");
	if (pAnchorX || pAnchorY)
	{
		pSprite->setAnchorPoint(CCPointMake(pAnchorX->floatValue(), pAnchorY->floatValue()));
	}
	
	// scale
	CCString * pScaleX = (CCString *)pSpriteData->objectForKey("scaleX");
	if (pScaleX)
	{
		pSprite->setScaleX(pScaleX->floatValue());
	}
	CCString * pScaleY = (CCString *)pSpriteData->objectForKey("scaleY");
	if (pScaleY)
	{
		pSprite->setScaleY(pScaleY->floatValue());
	}
	
	// skew
	CCString * pSkewX = (CCString *)pSpriteData->objectForKey("skewX");
	if (pSkewX)
	{
		pSprite->setSkewX(pSkewX->floatValue());
	}
	CCString * pSkewY = (CCString *)pSpriteData->objectForKey("skewY");
	if (pSkewY)
	{
		pSprite->setSkewY(pSkewY->floatValue());
	}
	
	// rotation
	CCString * pRotation = (CCString *)pSpriteData->objectForKey("rotation");
	if (pRotation)
	{
		pSprite->setRotation(pRotation->floatValue());
	}
	
	// alpha
	CCString * pAlpha = (CCString *)pSpriteData->objectForKey("alpha");
	if (pAlpha)
	{
		pSprite->setAlpha(pAlpha->floatValue());
	}
	
	return pSprite;
}

NS_FC_END
