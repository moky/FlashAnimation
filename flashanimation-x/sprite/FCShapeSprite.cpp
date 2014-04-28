//
//  FCShapeSprite.cpp
//  FlashAnimation-X
//
//  Created by Moky on 12-12-9.
//  Copyright 2012 Slanissue Inc. All rights reserved.
//

#include "FCShapeSprite.h"

NS_FC_BEGIN

USING_NS_CC;

FCShapeSprite * FCShapeSprite::shapeSprite(const std::string & sTextureName, const CCSize & tContentSize)
{
	FCShapeSprite * pSprite = new FCShapeSprite();
	if (pSprite && pSprite->init(sTextureName, tContentSize))
	{
		pSprite->autorelease();
		return pSprite;
	}
	CC_SAFE_DELETE(pSprite);
	return NULL;
}

bool FCShapeSprite::init(const std::string & sTextureName, const CCSize & tContentSize)
{
	CCAssert(sTextureName.length() > 0, "Invalid texture name for FCShapeSprite");
	
	CCSpriteFrame * pFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(sTextureName.c_str());
	if (!pFrame)
	{
		FCLog("failed to get sprite from cache, name: %s", sTextureName.c_str());
		if (!FCSprite::init())
		{
			return false;
		}
		setContentSize(tContentSize);
		setupVextices();
		return true;
	}
	
//	pFrame->setOriginalSizeInPixels(tContentSize);
	pFrame->setOriginalSize(tContentSize);
	
	bool flag = FCSprite::initWithSpriteFrame(pFrame);
	setupVextices();
	
	return flag;
}

void FCShapeSprite::setupVextices(void)
{
	CCSize tSize = getContentSize();
	CCFloat x1 = 0.0;
	CCFloat y1 = 0.0;
	CCFloat x2 = tSize.width;
	CCFloat y2 = tSize.height;
	
	// Don't update Z.
	m_sQuad.bl.vertices = (ccVertex3F) { x1, y1, 0 };
	m_sQuad.br.vertices = (ccVertex3F) { x2, y1, 0 };
	m_sQuad.tl.vertices = (ccVertex3F) { x1, y2, 0 };
	m_sQuad.tr.vertices = (ccVertex3F) { x2, y2, 0 };
}

NS_FC_END
