//
//  FCAnimation.h
//  FlashAnimation-X
//
//  Created by Moky on 12-12-10.
//  Copyright 2012 Slanissue Inc. All rights reserved.
//

#ifndef FlashAnimation_X_FCAnimation_h
#define FlashAnimation_X_FCAnimation_h

#include "fcMacros.h"
#include "library/FCLibrary.h"
#include "sprite/FCInstanceSprite.h"
#include "data/FCAnimationDataReader.h"

NS_FC_BEGIN

class FC_DLL FCAnimation : public cocos2d::CCObject
{
	CC_SYNTHESIZE(unsigned int, m_iFPS, FPS);
	CC_SYNTHESIZE_PASS_BY_REF(std::string, m_sMainSpriteName, MainSpriteName);
	CC_SYNTHESIZE_RETAIN(FCLibrary *, m_pLibrary, Library);
	CC_PROPERTY(cocos2d::CCArray *, m_pTextures, Textures);
	
	void setTextures(cocos2d::CCArray * pTextures, const std::string & sTextureFolderPath);
	
public:
	FCAnimation(void);
	virtual ~FCAnimation(void);
	virtual bool init(void);
	
	virtual bool init(const std::string & sFilePath);
	virtual bool init(cocos2d::CCDictionary * pDict, const std::string & sTextureFolderPath);
	virtual bool init(FCAnimationDataReader * pF2CReader, const std::string & sTextureFolderPath);
	
	static FCAnimation * animation(const std::string & sPlistFile);
	static FCAnimation * animation(cocos2d::CCDictionary * pDict, const std::string & sTextureFolderPath);
	
	FCSprite * mainSprite(void);
	
	FCSprite * spriteWithSymbolName(const std::string & sSymbolName);
	void recycleSprite(FCSprite * pSprite) const;
};

NS_FC_END

#endif
