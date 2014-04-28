//
//  FCShapeSprite.h
//  FlashAnimation-X
//
//  Created by Moky on 12-12-9.
//  Copyright 2012 Slanissue Inc. All rights reserved.
//

#ifndef FlashAnimation_X_FCShapeSprite_h
#define FlashAnimation_X_FCShapeSprite_h

#include "FCSprite.h"

NS_FC_BEGIN

class FC_DLL FCShapeSprite : public FCSprite
{
public:
	virtual bool init(const std::string & sTextureName, const cocos2d::CCSize & tContentSize);
	
	static FCShapeSprite * shapeSprite(const std::string & sTextureName, const cocos2d::CCSize & tContentSize);
	
protected:
	void setupVextices(void);
};

NS_FC_END

#endif
