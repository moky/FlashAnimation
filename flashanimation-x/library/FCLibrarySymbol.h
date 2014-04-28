//
//  FCLibrarySymbol.h
//  FlashAnimation-X
//
//  Created by Moky on 12-12-10.
//  Copyright 2012 Slanissue Inc. All rights reserved.
//

#ifndef FlashAnimation_X_FCLibrarySymbol_h
#define FlashAnimation_X_FCLibrarySymbol_h

#include "../sprite/FCSprite.h"
#include "../timeline/FCTimeline.h"

NS_FC_BEGIN

typedef enum {
    FCLibrarySymbolTypeShape,
    FCLibrarySymbolTypeMovie
} FCLibrarySymbolType;

class FCLibrary;

class FC_DLL FCLibrarySymbol : public cocos2d::CCObject
{
	CC_PROPERTY(FCLibrary *, m_pLibrary, Library);
	CC_SYNTHESIZE_PASS_BY_REF(std::string, m_sName, Name);
	CC_SYNTHESIZE(FCLibrarySymbolType, m_iType, Type);
	CC_SYNTHESIZE_PASS_BY_REF(cocos2d::CCSize, m_tContentSize, ContentSize);
	CC_SYNTHESIZE_PASS_BY_REF(std::string, m_sTextureName, TextureName); // shape only
	CC_SYNTHESIZE(FCTimeline *, m_pTimeline, Timeline); // movie only
	
public:
	FCLibrarySymbol(void);
	virtual ~FCLibrarySymbol(void);
	virtual bool init(void);
	
	static FCLibrarySymbol * librarySymbol(void);
	
	FCSprite * createSprite(void) const;
};

NS_FC_END

#endif
