//
//  FCAnimationDataTransformer.h
//  FlashAnimation-X
//
//  Created by Moky on 12-12-15.
//  Copyright 2012 Slanissue Inc. All rights reserved.
//

#ifndef FlashAnimation_X_FCAnimationDataTransformer_h
#define FlashAnimation_X_FCAnimationDataTransformer_h

#include "FCAnimationData.h"

NS_FC_BEGIN

class FCAnimationDataTransformer : public FCAnimationData
{
public:
	virtual bool init(const std::string & sPath);
	virtual bool init(cocos2d::CCDictionary * pDict);
};

NS_FC_END

#endif
