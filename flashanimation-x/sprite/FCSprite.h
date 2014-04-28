//
//  FCSprite.h
//  FlashAnimation-X
//
//  Created by Moky on 12-12-9.
//  Copyright 2012 Slanissue Inc. All rights reserved.
//

#ifndef FlashAnimation_X_FCSprite_h
#define FlashAnimation_X_FCSprite_h

#include "../fcMacros.h"

NS_FC_BEGIN

class FC_DLL FCSprite : public cocos2d::CCSprite
{
	CC_SYNTHESIZE_PASS_BY_REF(std::string, m_sSymbolName, SymbolName);
public:
	cocos2d::CCFloat getAlpha(void) const;
	void setAlpha(const cocos2d::CCFloat fAlpha);
	
public:
	FCSprite(void);
	virtual ~FCSprite(void);
	virtual bool init(void);
	
	virtual void reset(void);
	
	virtual cocos2d::CCAffineTransform nodeToParentTransform(void);
};

NS_FC_END

#endif
