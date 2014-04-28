//
//  FCInstanceSpriteDelegate.h
//  FlashAnimation-X
//
//  Created by Moky on 12-12-10.
//  Copyright (c) 2012年 Slanissue Inc. All rights reserved.
//

#ifndef FlashAnimation_X_FCInstanceSpriteDelegate_h
#define FlashAnimation_X_FCInstanceSpriteDelegate_h

#include "../fcMacros.h"

NS_FC_BEGIN

class FCInstanceSprite;

#pragma mark FCInstanceSpriteDelegate

class FC_DLL FCInstanceSpriteDelegate
{
public:
	virtual void instanceSpriteDidStopped(FCInstanceSprite * pSprite, const unsigned int iFrameIndex)
	{
		CC_UNUSED_PARAM(pSprite);
		CC_UNUSED_PARAM(iFrameIndex);
	}
	
	virtual void instanceSpriteDidChanged(FCInstanceSprite * pSprite, const unsigned int iFrameIndex)
	{
		CC_UNUSED_PARAM(pSprite);
		CC_UNUSED_PARAM(iFrameIndex);
	}
};

#pragma mark -

#ifndef NS_INLINE
#define NS_INLINE static inline
#endif

#ifndef BOOL
#define BOOL bool
#endif

#ifndef NSUInteger
#define NSUInteger unsigned int
#endif

#ifndef NSRange
typedef struct _NSRange {
	NSUInteger location;
	NSUInteger length;
} NSRange;

static inline NSRange NSMakeRange(NSUInteger loc, NSUInteger len) {
	NSRange r;
	r.location = loc;
	r.length = len;
	return r;
}

NS_INLINE NSUInteger NSMaxRange(NSRange range) {
    return (range.location + range.length);
}

NS_INLINE BOOL NSLocationInRange(NSUInteger loc, NSRange range) {
    return (loc - range.location < range.length);
}

NS_INLINE BOOL NSEqualRanges(NSRange range1, NSRange range2) {
    return (range1.location == range2.location && range1.length == range2.length);
}
#endif

NS_FC_END

#endif
