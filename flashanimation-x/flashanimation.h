//
//  FlashAnimation.h
//  FlashAnimation-X
//
//  Created by Moky on 12-12-9.
//  Copyright 2012 Slanissue Inc. All rights reserved.
//

#ifndef FlashAnimation_X_FlashAnimation_h
#define FlashAnimation_X_FlashAnimation_h

#include "fcMacros.h"
#include "FCAnimation.h"
// library
#include "library/FCLibrarySymbol.h"
#include "library/FCLibrarySymbolBuilder.h"
#include "library/FCLibrary.h"
// sprite
#include "sprite/FCMatrix.h"
#include "sprite/FCSprite.h"
#include "sprite/FCShapeSprite.h"
#include "sprite/FCInstanceSprite.h"
// timeline
#include "timeline/FCTimelineFrame.h"
#include "timeline/FCTimelineSprite.h"
#include "timeline/FCTimelineLayer.h"
#include "timeline/FCTimeline.h"
// data
#include "data/FCAnimationData.h"
#include "data/FCAnimationDataReader.h"
#include "data/FCAnimationDataTransformer.h"

NS_FC_BEGIN

const char * version(void);

NS_FC_END

#endif
