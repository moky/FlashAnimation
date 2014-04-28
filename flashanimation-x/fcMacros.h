//
//  fcMacros.h
//  FlashAnimation-X
//
//  Created by Moky on 12-12-9.
//  Copyright 2012 Slanissue Inc. All rights reserved.
//

#ifndef FlashAnimation_X_fcMacros_h
#define FlashAnimation_X_fcMacros_h

#include "cocos2d.h"




/**
 *  Print log info
 *
 *  Usage:
 *      CNLog(format, ...)
 *      format: "[LineNumber]+[Class Function] > InfoString"
 */
#if 1
#	define NSLog(...)   do {} while(0)
#	define FCLog(...)   do {} while(0)
#else
#	define FCLogFile             std::string(__FILE__).substr(std::string(__FILE__).rfind('/')+1).c_str()
#	define FCLogHead             cocos2d::CCString::stringWithFormat("%s [% 4d] %s", FCLogFile, __LINE__, __FUNCTION__)->getCString()
#	define FCLogStr(format, ...) cocos2d::CCString::stringWithFormat(format, ##__VA_ARGS__)->getCString()
#	define FCLog(format, ...)    cocos2d::CCLog("%s > %s", FCLogHead, FCLogStr(format, ##__VA_ARGS__))
#endif




#define FC_DLL          CC_DLL

// namespace flashanimation {}
#ifdef __cplusplus
#	define NS_FC_BEGIN  namespace flash2cocos2d {
#	define NS_FC_END    }
#	define USING_NS_FC  using namespace flash2cocos2d
#else
#	define NS_FC_BEGIN
#	define NS_FC_END
#	define USING_NS_FC
#endif






#define FC_SAFE_RETAIN(p) /* safe retain begin */ \
	do { \
		cocos2d::CCObject * _##p = dynamic_cast<cocos2d::CCObject *>(p); \
		if (_##p) { \
			_##p->retain(); \
		} \
		} while (0) \
/* safe retain end */

#define FC_SAFE_RELEASE(p) /* safe release begin */ \
	do { \
		cocos2d::CCObject * _##p = dynamic_cast<cocos2d::CCObject *>(p); \
		if (_##p) { \
			_##p->release(); \
		} \
	} while (0) \
/* safe release end */






static inline bool FCStringEqualToString(std::string str1, std::string str2)
{
	return strcasecmp(str1.c_str(), str2.c_str()) == 0;
	return str1 == str2;
}

static inline bool FCStringEqualToString(cocos2d::CCString * str1, std::string str2)
{
	return FCStringEqualToString(str1->getCString(), str2);
}

static inline bool FCStringEqualToString(std::string str1, cocos2d::CCString * str2)
{
	return FCStringEqualToString(str1, str2->getCString());
}

static inline bool FCStringEqualToString(cocos2d::CCString * str1, cocos2d::CCString * str2)
{
	return FCStringEqualToString(str1->getCString(), str2->getCString());
}




#endif
