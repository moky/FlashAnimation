//
//  FCTimelineLayer.cpp
//  FlashAnimation-X
//
//  Created by Moky on 12-12-9.
//  Copyright 2012 Slanissue Inc. All rights reserved.
//

#include "FCTimelineLayer.h"

#include "../sprite/FCMatrix.h"
#include "../library/FCLibrary.h"

NS_FC_BEGIN

USING_NS_CC;

FCTimelineLayer::FCTimelineLayer(void)
: CCObject()
, m_iType(FCTimelineLayerTypeNormal)
, m_sName("")
, m_pKeyFrames(NULL)
, m_pTimeline(NULL)
{
	
}

FCTimelineLayer::~FCTimelineLayer(void)
{
	setName("");
	setKeyFrames(NULL);
	setTimeline(NULL);
}

bool FCTimelineLayer::init(void)
{
	m_iType = FCTimelineLayerTypeNormal;
	setName("");
	setKeyFrames(NULL);
	setTimeline(NULL);
	return true;
}

FCTimelineLayer * FCTimelineLayer::timelineLayer(void)
{
	FCTimelineLayer * pLayer = new FCTimelineLayer();
	if (pLayer && pLayer->init())
	{
		pLayer->autorelease();
		return pLayer;
	}
	CC_SAFE_DELETE(pLayer);
	return NULL;
}

unsigned int FCTimelineLayer::frameCount(void) const
{
	FCTimelineFrame * pLastKeyFrame = (FCTimelineFrame *)m_pKeyFrames->lastObject();
	return pLastKeyFrame->getIndex() + pLastKeyFrame->getDuration();
}

#pragma mark - handle frame

CCArray * FCTimelineLayer::spritesOfFrameIndex(const unsigned int iFrameIndex) const
{
	FCTimelineFrame * pNextKeyFrame = NULL;
	FCTimelineFrame * pCurrentKeyFrame = NULL;
	
	for (int i = m_pKeyFrames->count() - 1; i >= 0; i--)
	{
		pCurrentKeyFrame = (FCTimelineFrame *)m_pKeyFrames->objectAtIndex(i);
		if (pCurrentKeyFrame->getIndex() <= iFrameIndex)
		{
			break;
		}
		pNextKeyFrame = pCurrentKeyFrame;
	}
	
	if (!pNextKeyFrame && pCurrentKeyFrame->getIndex() + pCurrentKeyFrame->getDuration() <= iFrameIndex)
	{
		return CCArray::array();
	}
	else if (pCurrentKeyFrame->getIndex() == iFrameIndex || pCurrentKeyFrame->getTweenType() == FCTimelineFrameTweenTypeNone)
	{
		return spritesOfKeyFrame(pCurrentKeyFrame, iFrameIndex - pCurrentKeyFrame->getIndex());
	}
	else if (pCurrentKeyFrame->getTweenType() == FCTimelineFrameTweenTypeMotion)
	{
		return spritesInMotionTweenFrame(pCurrentKeyFrame, pNextKeyFrame, iFrameIndex - pCurrentKeyFrame->getIndex());
	}
	else if (pCurrentKeyFrame->getTweenType() == FCTimelineFrameTweenTypeShape)
	{
		return spritesInShapeTweenFrame(pCurrentKeyFrame, pNextKeyFrame, iFrameIndex - pCurrentKeyFrame->getIndex());
	}
	else
	{
		CCAssert(false, "can not reach here");
		return NULL;
	}
}

CCArray * FCTimelineLayer::spritesOfKeyFrame(FCTimelineFrame * pKeyFrame, const unsigned int iDuration) const
{
	CCArray * pSprites = CCArray::arrayWithArray(pKeyFrame->getSprites());
	// TODO:
	
	CCObject * obj = NULL;
	CCARRAY_FOREACH(pSprites, obj)
	{
		FCTimelineSprite * pSprite = (FCTimelineSprite *)obj;
		if (pSprite->getType() == FCTimelineSpriteTypeGraphic)
		{
			if (pSprite->getLoopType() == FCTimelineSpriteLoopTypeSingleFrame)
			{
				//
			}
			else
			{
				pSprite->setFirstFrame(pSprite->getFirstFrame() + iDuration);
			}
		}
	}
	
	return pSprites;
}

CCArray * FCTimelineLayer::spritesInMotionTweenFrame(FCTimelineFrame * pCurrentKeyFrame, FCTimelineFrame * pNextKeyFrame, const unsigned int iDuration) const
{
	if (!pCurrentKeyFrame)
	{
		return NULL;
	}
	CCArray * pCurrentKeyFrameSprites = pCurrentKeyFrame->getSprites();
	if (!pNextKeyFrame)
	{
		return pCurrentKeyFrameSprites;
	}
	CCArray * pNextKeyFrameSprites = pNextKeyFrame->getSprites();
	if (!pCurrentKeyFrameSprites || pCurrentKeyFrameSprites->count() == 0 ||
		!pNextKeyFrameSprites || pNextKeyFrameSprites->count() == 0)
	{
		return pCurrentKeyFrameSprites;
	}
	
	//补间动画中，起始桢只能有一个实例
	FCTimelineSprite * pStartSprite = (FCTimelineSprite *)pCurrentKeyFrameSprites->objectAtIndex(0);
	if (pStartSprite->getType() == FCTimelineSpriteTypeShape)
	{
		return pCurrentKeyFrameSprites;
	}
	
	FCTimelineSprite * pEndSprite = NULL;
	CCObject * obj = NULL;
	CCARRAY_FOREACH(pNextKeyFrameSprites, obj)
	{
		pEndSprite = (FCTimelineSprite *)obj;
		if (FCStringEqualToString(pEndSprite->getSymbolName(), pStartSprite->getSymbolName()))
		{
			break;
		}
	}
	
	if (!pEndSprite)
	{
		return spritesOfKeyFrame(pCurrentKeyFrame, iDuration);
	}
	else
	{
		FCTimelineSprite * pTweenSprite = (FCTimelineSprite *)pStartSprite->copy();
		setupMotionTweenSprite(pTweenSprite, pStartSprite, pEndSprite, (CCFloat)iDuration / (CCFloat)pCurrentKeyFrame->getDuration());
		if (pTweenSprite->getType() == FCTimelineSpriteTypeGraphic)
		{
			if (pTweenSprite->getLoopType() == FCTimelineSpriteLoopTypeSingleFrame)
			{
				//
			}
			else
			{
				pTweenSprite->setFirstFrame(pTweenSprite->getFirstFrame() + iDuration);
			}
		}
		pTweenSprite->autorelease();
		return CCArray::arrayWithObject(pTweenSprite);
	}
}

CCArray * FCTimelineLayer::spritesInShapeTweenFrame(FCTimelineFrame * pCurrentKeyFrame, FCTimelineFrame * pNextKeyFrame, const unsigned int iDuration) const
{
	if (!pCurrentKeyFrame)
	{
		return NULL;
	}
	CCArray * pCurrentKeyFrameSprites = pCurrentKeyFrame->getSprites();
	if (!pNextKeyFrame)
	{
		return pCurrentKeyFrameSprites;
	}
	CCArray * pNextKeyFrameSprites = pNextKeyFrame->getSprites();
	if (!pCurrentKeyFrameSprites || pCurrentKeyFrameSprites->count() == 0 ||
		!pNextKeyFrameSprites || pNextKeyFrameSprites->count() == 0)
	{
		return pCurrentKeyFrameSprites;
	}
	
	//补间动画中，起始桢只能有一个实例
	FCTimelineSprite * pStartSprite = (FCTimelineSprite *)pCurrentKeyFrameSprites->objectAtIndex(0);
	if (pStartSprite->getType() != FCTimelineSpriteTypeShape)
	{
		return pCurrentKeyFrameSprites;
	}
	
	FCTimelineSprite * pEndSprite = (FCTimelineSprite *)pNextKeyFrameSprites->objectAtIndex(0);
	if (pEndSprite->getType() != FCTimelineSpriteTypeShape)
	{
		return pCurrentKeyFrameSprites;
	}
	
	if (!pEndSprite)
	{
		return spritesOfKeyFrame(pCurrentKeyFrame, iDuration);
	}
	else
	{
		FCTimelineSprite * pTweenSprite = (FCTimelineSprite *)pStartSprite->copy();
		setupShapeTweenSprite(pTweenSprite, pStartSprite, pEndSprite, (CCFloat)iDuration / (CCFloat)pCurrentKeyFrame->getDuration());
		pTweenSprite->autorelease();
		return CCArray::arrayWithObject(pTweenSprite);
	}
}

void FCTimelineLayer::setupMotionTweenSprite(FCTimelineSprite * tweenSprite, FCTimelineSprite * startState, FCTimelineSprite * endState, const float delta) const
{
	CCPoint startPosition = startState->getPosition();
	CCPoint endPosition = endState->getPosition();
	if (endPosition.x != startPosition.x || endPosition.y != startPosition.y) {
		CCPoint tweenPosition = CCPointMake(
											startPosition.x + (endPosition.x - startPosition.x) * delta,
											startPosition.y + (endPosition.y - startPosition.y) * delta
											);
		tweenSprite->setPosition(tweenPosition);
	}
	
	if (endState->getAlpha() != startState->getAlpha()) {
		CCFloat tweenAlpha = startState->getAlpha() + (endState->getAlpha() - startState->getAlpha()) * delta;
		tweenSprite->setAlpha(tweenAlpha);
	}
	
	CCAffineTransform startMatrix = startState->getMatrix();
	CCAffineTransform endMatrix = endState->getMatrix();
	
	CCAffineTransform tweenMatrix = CCAffineTransformMake(
														  startMatrix.a + (endMatrix.a - startMatrix.a) * delta,
														  startMatrix.b + (endMatrix.b - startMatrix.b) * delta,
														  startMatrix.c + (endMatrix.c - startMatrix.c) * delta,
														  startMatrix.d + (endMatrix.d - startMatrix.d) * delta,
														  0.0,
														  0.0
														  );
	
	if (endState->getScaleX() != startState->getScaleX()) {
		tweenSprite->setScaleX(getMatrixScaleX(tweenMatrix));
	}
	
	if (endState->getScaleY() != startState->getScaleY()) {
		tweenSprite->setScaleY(getMatrixScaleY(tweenMatrix));
	}
	
	if (endState->getSkewX() != startState->getSkewX()) {
		tweenSprite->setSkewX(getMatrixSkewX(tweenMatrix));
	}
	
	if (endState->getSkewY() != startState->getSkewY()) {
		tweenSprite->setSkewY(getMatrixSkewY(tweenMatrix));
	}
	//因为flash中的skewX和skewY值与rotation这个值相互冲突，所以既然上面已经处理了skew值，下面就不处理rotation了
	if (endState->getRotation() != startState->getRotation()) {
		
	}
}

/*
 * 对于形状补间，这里只是粗略地对位置和比例大小进行插值。并没有真实还原flash中的形状补间动画
 */
void FCTimelineLayer::setupShapeTweenSprite(FCTimelineSprite * tweenSprite, FCTimelineSprite * startState, FCTimelineSprite * endState, const float delta) const
{
	CCPoint startPosition = startState->getPosition();
	CCPoint endPosition = endState->getPosition();
	if (endPosition.x != startPosition.x || endPosition.y != startPosition.y) {
		CCPoint tweenPosition = CCPointMake(
											startPosition.x + (endPosition.x - startPosition.x) * delta,
											startPosition.y + (endPosition.y - startPosition.y) * delta
											);
		tweenSprite->setPosition(tweenPosition);
	}
	
	if (endState->getAlpha() != startState->getAlpha()) {
		CCFloat tweenAlpha = startState->getAlpha() + (endState->getAlpha() - startState->getAlpha()) * delta;
		tweenSprite->setAlpha(tweenAlpha);
	}
	
	FCLibrary * library = m_pTimeline->getSymbol()->getLibrary();
	FCLibrarySymbol * startShapeSymbol = library->symbolWithName(startState->getSymbolName());
	FCLibrarySymbol * endShapeSymbol = library->symbolWithName(endState->getSymbolName());
	
	if (!CCSize::CCSizeEqualToSize(startShapeSymbol->getContentSize(), endShapeSymbol->getContentSize())) {
		CCFloat startWidth = startShapeSymbol->getContentSize().width;
		CCFloat endWidth = endShapeSymbol->getContentSize().width;
		
		tweenSprite->setScaleX((startWidth + (endWidth - startWidth) * delta) / startWidth);
		
		CCFloat startHeight = startShapeSymbol->getContentSize().height;
		CCFloat endHeight = endShapeSymbol->getContentSize().height;
		
		tweenSprite->setScaleY((startHeight + (endHeight - startHeight) * delta) / startHeight);
	}
}

NS_FC_END
