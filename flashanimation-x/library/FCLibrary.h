//
//  FCLibrary.h
//  FlashAnimation-X
//
//  Created by Moky on 12-12-10.
//  Copyright 2012 Slanissue Inc. All rights reserved.
//

#ifndef FlashAnimation_X_FCLibrary_h
#define FlashAnimation_X_FCLibrary_h

#include "FCLibrarySymbol.h"

NS_FC_BEGIN

class FC_DLL FCLibrary : public cocos2d::CCObject
{
	CC_SYNTHESIZE_RETAIN(cocos2d::CCDictionary *, m_pSymbols, Symbols);
	CC_SYNTHESIZE_RETAIN(cocos2d::CCDictionary *, m_pSpritePool, SpritePool);
	
public:
	FCLibrary(void);
	virtual ~FCLibrary(void);
	virtual bool init(void);
	
	virtual bool init(cocos2d::CCDictionary * pDict);
	
	FCLibrarySymbol * symbolWithName(const std::string & sSymbolName) const;
	
	FCSprite * buildSpriteWithSymbolName(const std::string & sSymbolName) const;
	void recycleSprite(FCSprite * pSprite) const;
	
protected:
	cocos2d::CCDictionary * buildSymbolsWithData(cocos2d::CCDictionary * pDict);
	
	FCSprite * popSpriteFromPoolWithName(const std::string & sSpriteName) const;
	void pushSpriteToPool(FCSprite * pSprite) const;
};

NS_FC_END

#endif
