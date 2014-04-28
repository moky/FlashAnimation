//
//  FCMatrix.h
//  FlashAnimation-X
//
//  Created by Moky on 12-12-9.
//  Copyright 2012 Slanissue Inc. All rights reserved.
//

#ifndef FlashAnimation_X_FCMatrix_h
#define FlashAnimation_X_FCMatrix_h

#include "../fcMacros.h"

NS_FC_BEGIN

static inline cocos2d::CCFloat getMatrixScaleX(cocos2d::CCAffineTransform m) {
	return sqrt(m.a * m.a + m.b * m.b);
}

static inline cocos2d::CCFloat getMatrixScaleY(cocos2d::CCAffineTransform m) {
	return sqrt(m.c * m.c + m.d * m.d);
}

static inline cocos2d::CCFloat getMatrixSkewXRadians(cocos2d::CCAffineTransform m) {
	return atan2f(-m.c, m.d);
}

static inline cocos2d::CCFloat getMatrixSkewYRadians(cocos2d::CCAffineTransform m) {
	return atan2f(m.b, m.a);
}

static inline cocos2d::CCFloat getMatrixSkewX(cocos2d::CCAffineTransform m) {
	return atan2f(-m.c, m.d) * (180.0 / M_PI);
}

static inline cocos2d::CCFloat getMatrixSkewY(cocos2d::CCAffineTransform m) {
	return atan2f(m.b, m.a) * (180.0 / M_PI);
}

static inline cocos2d::CCFloat getMatrixRotationRadians(cocos2d::CCAffineTransform m) {
	cocos2d::CCFloat skewEqualityDelta = .0001; // customizable
	cocos2d::CCFloat skewXRad = getMatrixSkewXRadians(m);
	cocos2d::CCFloat skewYRad = getMatrixSkewYRadians(m);
	if (fabsf(skewXRad - skewYRad) <= skewEqualityDelta) {
		return skewYRad;
	} else {
		return 0;
	}
}

static inline cocos2d::CCFloat getMatrixRotation(cocos2d::CCAffineTransform m) {
	return getMatrixRotationRadians(m) * (180.0 / M_PI);
}

static inline void setMatrixScaleX(cocos2d::CCAffineTransform *m, cocos2d::CCFloat scaleX) {
	cocos2d::CCFloat oldValue = getMatrixScaleX(*m);
	// avoid division by zero
	if (oldValue) {
		cocos2d::CCFloat ratio = scaleX / oldValue;
		m->a *= ratio;
		m->b *= ratio;
	} else {
		cocos2d::CCFloat skewYRad = getMatrixSkewYRadians(*m);
		m->a = cosf(skewYRad) * scaleX;
		m->b = sinf(skewYRad) * scaleX;
	}
}

static inline void setMatrixScaleY(cocos2d::CCAffineTransform *m, cocos2d::CCFloat scaleY) {
	cocos2d::CCFloat oldValue = getMatrixScaleY(*m);
	// avoid division by zero
	if (oldValue) {
		cocos2d::CCFloat ratio = scaleY / oldValue;
		m->c *= ratio;
		m->d *= ratio;
	} else {
		cocos2d::CCFloat skewXRad = getMatrixSkewXRadians(*m);
		m->c = -sinf(skewXRad) * scaleY;
		m->d =  cosf(skewXRad) * scaleY;
	}
	
}

static inline void setMatrixSkewXRadians(cocos2d::CCAffineTransform *m, cocos2d::CCFloat skewX) {
	cocos2d::CCFloat scaleY = getMatrixScaleY(*m);
	m->c = -scaleY * sinf(skewX);
	m->d =  scaleY * cosf(skewX);
}

static inline void setMatrixSkewYRadians(cocos2d::CCAffineTransform *m, cocos2d::CCFloat skewY) {
	cocos2d::CCFloat scaleX = getMatrixScaleX(*m);
	m->a = scaleX * cosf(skewY);
	m->b = scaleX * sinf(skewY);
}

static inline void setMatrixSkewX(cocos2d::CCAffineTransform *m, cocos2d::CCFloat skewX) {
	setMatrixSkewXRadians(m, skewX * (M_PI / 180.0));
}

static inline void setMatrixSkewY(cocos2d::CCAffineTransform *m, cocos2d::CCFloat skewY) {
	setMatrixSkewYRadians(m, skewY * (M_PI / 180.0));
}

static inline void setMatrixRotationRadans(cocos2d::CCAffineTransform *m, cocos2d::CCFloat rotation) {
	setMatrixSkewXRadians(m, rotation);
	setMatrixSkewYRadians(m, rotation);
}

static inline void setMatrixRotation(cocos2d::CCAffineTransform *m, cocos2d::CCFloat rotation) {
	setMatrixRotationRadans(m, rotation * (M_PI / 180.0));
}

NS_FC_END

#endif
