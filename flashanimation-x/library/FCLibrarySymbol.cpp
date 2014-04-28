//
//  FCLibrarySymbol.cpp
//  FlashAnimation-X
//
//  Created by Moky on 12-12-10.
//  Copyright 2012 Slanissue Inc. All rights reserved.
//

#include "FCLibrarySymbol.h"

#include "FCLibrary.h"
#include "../sprite/FCShapeSprite.h"
#include "../sprite/FCInstanceSprite.h"


NS_FC_BEGIN

USING_NS_CC;

FCLibrarySymbol::FCLibrarySymbol(void)
: CCObject()
, m_pLibrary(NULL)
, m_sName("")
, m_iType(FCLibrarySymbolTypeShape)
, m_tContentSize(CCSizeZero)
, m_sTextureName("")
, m_pTimeline(NULL)
{
	
}

FCLibrarySymbol::~FCLibrarySymbol(void)
{
	setLibrary(NULL);
	setName("");
	setTextureName("");
	setTimeline(NULL);
}

bool FCLibrarySymbol::init(void)
{
	setLibrary(NULL);
	setName("");
	setType(FCLibrarySymbolTypeShape);
	setContentSize(CCSizeZero);
	setTextureName("");
	setTimeline(NULL);
	return true;
}

FCLibrary * FCLibrarySymbol::getLibrary(void)
{
	return m_pLibrary;
}

void FCLibrarySymbol::setLibrary(FCLibrary * pLibrary)
{
	if (m_pLibrary != pLibrary)
	{
//		FA_SAFE_RETAIN(pLibrary);
//		FA_SAFE_RELEASE(m_pLibrary);
		m_pLibrary = pLibrary;
	}
}

FCLibrarySymbol * FCLibrarySymbol::librarySymbol(void)
{
	FCLibrarySymbol * pSymbol = new FCLibrarySymbol();
	if (pSymbol && pSymbol->init())
	{
		pSymbol->autorelease();
		return pSymbol;
	}
	CC_SAFE_DELETE(pSymbol);
	return NULL;
}

FCSprite * FCLibrarySymbol::createSprite(void) const
{
	if (m_iType == FCLibrarySymbolTypeShape)
	{
		FCShapeSprite * pSprite = FCShapeSprite::shapeSprite(m_sTextureName, m_tContentSize);
		if (pSprite)
		{
			pSprite->setSymbolName(m_sName);
		}
		return pSprite;
	}
	else if (m_iType == FCLibrarySymbolTypeMovie)
	{
		FCInstanceSprite * pSprite = FCInstanceSprite::instanceSprite(m_pTimeline);
		if (pSprite)
		{
			pSprite->setSymbolName(m_sName);
			pSprite->setContentSize(m_tContentSize);
		}
		return pSprite;
	}
	else
	{
		CCAssert(false, "can not reach here");
		return NULL;
	}
}

NS_FC_END
