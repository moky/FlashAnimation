//
//  FCLibrary.cpp
//  FlashAnimation-X
//
//  Created by Moky on 12-12-10.
//  Copyright 2012 Slanissue Inc. All rights reserved.
//

#include "FCLibrary.h"

#include "FCLibrarySymbolBuilder.h"

NS_FC_BEGIN

USING_NS_CC;

FCLibrary::FCLibrary(void)
: CCObject()
, m_pSymbols(NULL)
, m_pSpritePool(NULL)
{
	
}

FCLibrary::~FCLibrary(void)
{
	setSymbols(NULL);
	setSpritePool(NULL);
}

bool FCLibrary::init(void)
{
	setSymbols(NULL);
	setSpritePool(CCDictionary::dictionary());
	return true;
}

bool FCLibrary::init(CCDictionary * pDict)
{
	if (!init())
	{
		return false;
	}
	setSymbols(buildSymbolsWithData(pDict));
	return true;
}

FCLibrarySymbol * FCLibrary::symbolWithName(const std::string & sSymbolName) const
{
	return m_pSymbols ? (FCLibrarySymbol *)m_pSymbols->objectForKey(sSymbolName) : NULL;
}

#pragma mark - build symbol

CCDictionary * FCLibrary::buildSymbolsWithData(CCDictionary * pDict)
{
	CCDictionary * pSymbols = CCDictionary::dictionary();
	
	FCLibrarySymbolBuilder * pSymbolBuilder = FCLibrarySymbolBuilder::symbolBuilder();
	
	CCArray * pAllKeys = pDict->allKeys();
	CCObject * obj = NULL;
	CCARRAY_FOREACH(pAllKeys, obj)
	{
		CCString * key = (CCString *)obj;
		CCDictionary * pSymbolData = (CCDictionary *)pDict->objectForKey(key->getCString());
		FCLibrarySymbol * pSymbol = pSymbolBuilder->buildSymbol(pSymbolData);
		CCAssert(pSymbol, "parse error!");
		pSymbol->setLibrary(this);
		pSymbols->setObject(pSymbol, pSymbol->getName());
	}
	
	return pSymbols;
}

#pragma mark - manage sprite lifetime

FCSprite * FCLibrary::buildSpriteWithSymbolName(const std::string & sSymbolName) const
{
	FCSprite * pSprite = popSpriteFromPoolWithName(sSymbolName);
	if (!pSprite)
	{
		FCLibrarySymbol * pSymbol = symbolWithName(sSymbolName);
		if (pSymbol)
		{
			pSprite = pSymbol->createSprite();
		}
	}
//	if (!pSprite)
//	{
//		FCLog("cannot build sprite with symbol name: %s", pszSymbolName);
//	}
	return pSprite;
}

void FCLibrary::recycleSprite(FCSprite * pSprite) const
{
	if (pSprite->getParent())
	{
		pSprite->removeFromParentAndCleanup(false);
	}
	pushSpriteToPool(pSprite);
}

FCSprite * FCLibrary::popSpriteFromPoolWithName(const std::string & sSpriteName) const
{
	CCArray * pSpritesInPool = (CCArray *)m_pSpritePool->objectForKey(sSpriteName);
	if (!pSpritesInPool || pSpritesInPool->count() == 0)
	{
		return NULL;
	}
	else
	{
		FCSprite * pReuseSprite = (FCSprite *)pSpritesInPool->lastObject();
		pReuseSprite->retain();
		pReuseSprite->autorelease();
		pSpritesInPool->removeLastObject();
		return pReuseSprite;
	}
}

void FCLibrary::pushSpriteToPool(FCSprite * pSprite) const
{
	CCAssert(pSprite, "could not happen!");
	if (!pSprite)
	{
		return;
	}
	CCArray * pSameSpritesInPool = (CCArray *)m_pSpritePool->objectForKey(pSprite->getSymbolName());
	if (!pSameSpritesInPool)
	{
		pSameSpritesInPool = CCArray::arrayWithCapacity(2);
		m_pSpritePool->setObject(pSameSpritesInPool, pSprite->getSymbolName());
	}
	pSameSpritesInPool->addObject(pSprite);
}

NS_FC_END
