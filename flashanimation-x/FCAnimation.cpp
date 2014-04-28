//
//  FCAnimation.cpp
//  FlashAnimation-X
//
//  Created by Moky on 12-12-10.
//  Copyright 2012 Slanissue Inc. All rights reserved.
//

#include "FCAnimation.h"

#include "coconut2d.h"

NS_FC_BEGIN

USING_NS_CC;

FCAnimation::FCAnimation(void)
: CCObject()
, m_iFPS(24)
, m_sMainSpriteName("")
, m_pLibrary(NULL)
, m_pTextures(NULL)
{
	
}

FCAnimation::~FCAnimation(void)
{
	setMainSpriteName("");
	setLibrary(NULL);
	
	setTextures(NULL);
}

bool FCAnimation::init(void)
{
	setFPS(24);
	setMainSpriteName("");
	setLibrary(NULL);
	setTextures(NULL);
	return true;
}

bool FCAnimation::init(const std::string & sFilePath)
{
	if (sFilePath.empty() || !init())
	{
		return false;
	}
	
	// get folder path
	std::string sTextureFolderPath(sFilePath);
	std::string::size_type pos = sTextureFolderPath.rfind("/");
	if (pos == std::string::npos)
	{
		return false;
	}
	sTextureFolderPath = sTextureFolderPath.substr(0, pos + 1);
	FCLog("sTextureFolderPath : %s", sTextureFolderPath.c_str());
	
	// get file ext
	pos = sFilePath.rfind(".");
	if (pos == std::string::npos)
	{
		return false;
	}
	std::string ext = sFilePath.substr(pos + 1);
	
	bool flag = false;
	
	if (ext.empty())
	{
		FCLog("error, sFilePath: %s", sFilePath.c_str());
	}
	else if (strcasecmp(ext.c_str(), "plist") == 0)
	{
		CCDictionary * pDict = CCDictionary::dictionaryWithContentsOfFile(sFilePath.c_str());
		flag = init(pDict, sTextureFolderPath);
	}
	else if (strcasecmp(ext.c_str(), "json") == 0)
	{
		FCLog("DO NOT support JsON format anymore!");
	}
	else
	{
		// 'f2c' file?
		FCAnimationDataReader * pData = new FCAnimationDataReader();
		if (pData && pData->init(sFilePath))
		{
			flag = init(pData, sTextureFolderPath);
		}
		CC_SAFE_FREE(pData);
	}
	
	if (flag)
	{
		FCLog("animation init OK: %s", sFilePath.c_str());
		return true;
	}
	else
	{
		FCLog("animation init ERROR: %s", sFilePath.c_str());
		return true;
	}
}

bool FCAnimation::init(FCAnimationDataReader * pData, const std::string & sTextureFolderPath)
{
	if (!pData || sTextureFolderPath.empty())
	{
		return false;
	}
	if (!init())
	{
		return false;
	}
	
	CCArray * pTextures = pData->getTextures();
	setTextures(pTextures, sTextureFolderPath);
	
	m_iFPS = pData->getFrameRate();
	
	CCDictionary * pNames = pData->getNames();
	CCAssert(pNames, "could not happen");
	m_sMainSpriteName = ((CCString *)pNames->objectForKey(pData->getRootId()))->getCString();
	
	FCLibrary * pLibrary = new FCLibrary();
	if (pLibrary && pLibrary->init(pData->getAllSymbols()))
	{
		pLibrary->autorelease();
		setLibrary(pLibrary);
	}
	
	return true;
}

bool FCAnimation::init(CCDictionary * pDict, const std::string & sTextureFolderPath)
{
	if (!pDict || sTextureFolderPath.empty())
	{
		return false;
	}
	if (!init())
	{
		return false;
	}
	
	CCArray * pTextures = (CCArray *)pDict->objectForKey("textures");
	setTextures(pTextures, sTextureFolderPath);
	
	CCString * pFPS = (CCString *)pDict->objectForKey("frameRate");
	if (pFPS)
	{
		m_iFPS = pFPS->uintValue();
	}
	
	CCString * pMainSpriteName = (CCString *)pDict->objectForKey("rootId");
	if (pMainSpriteName)
	{
		m_sMainSpriteName = pMainSpriteName->getCString();
	}
	
	CCDictionary * pSprites = (CCDictionary *)pDict->objectForKey("sprites");
	if (pSprites)
	{
		FCLibrary * pLibrary = new FCLibrary();
		if (pLibrary && pLibrary->init(pSprites))
		{
			pLibrary->autorelease();
			setLibrary(pLibrary);
		}
	}
	
	return true;
}

FCAnimation * FCAnimation::animation(const std::string & sPlistFile)
{
	FCAnimation * pAnimation = new FCAnimation();
	if (pAnimation && pAnimation->init(sPlistFile))
	{
		pAnimation->autorelease();
		return pAnimation;
	}
	CC_SAFE_DELETE(pAnimation);
	return NULL;
}

FCAnimation * FCAnimation::animation(CCDictionary * pDict, const std::string & sTextureFolderPath)
{
	FCAnimation * pAnimation = new FCAnimation();
	if (pAnimation && pAnimation->init(pDict, sTextureFolderPath))
	{
		pAnimation->autorelease();
		return pAnimation;
	}
	CC_SAFE_DELETE(pAnimation);
	return NULL;
}

FCSprite * FCAnimation::mainSprite(void)
{
	if (m_sMainSpriteName.empty())
	{
		return NULL;
	}
	FCSprite * pSprite = spriteWithSymbolName(m_sMainSpriteName);
	if (FCInstanceSprite * pInstanceSprite = dynamic_cast<FCInstanceSprite *>(pSprite))
	{
		pInstanceSprite->setType(FCInstanceSpriteMovicClip);
	}
	return pSprite;
}

FCSprite * FCAnimation::spriteWithSymbolName(const std::string & sSymbolName)
{
	FCSprite * pSprite = m_pLibrary->buildSpriteWithSymbolName(sSymbolName);
	if (FCInstanceSprite * pInstanceSprite = dynamic_cast<FCInstanceSprite *>(pSprite))
	{
		pInstanceSprite->setAnimation(this);
	}
	return pSprite;
}

void FCAnimation::recycleSprite(FCSprite * pSprite) const
{
	if (FCInstanceSprite * pInstanceSprite = dynamic_cast<FCInstanceSprite *>(pSprite))
	{
		pInstanceSprite->stop();
		pInstanceSprite->setAnimation(NULL);
		
		CCPoolManager * pm = CCPoolManager::sharedPoolManager();
		pm->push();
		
		CCArray * pSprites = pInstanceSprite->flatArrayOfCurrentSubspritesForEachLayer();
		CCObject * obj = NULL;
		CCARRAY_FOREACH(pSprites, obj)
		{
			recycleSprite((FCSprite *)obj);
		}
		
		pm->pop();
	}
	
	pSprite->reset();
	
	m_pLibrary->recycleSprite(pSprite);
}

#pragma mark -

// copy from 'CCSpriteFrameCache.cpp'
CCTexture2D * FCAnimation_loadTextureWithPlistPath(const std::string & sPlist)
{
	return coconut2d::CNSpriteFrameCache::loadTextureAndSprites(sPlist); // high speed loader
//	
//	const char * pszPath = CCFileUtils::fullPathFromRelativePath(sPlist.c_str());
//	CCDictionary *dict = CCDictionary::dictionaryWithContentsOfFileThreadSafe(pszPath);
//    
//	std::string texturePath("");
//	
//	CCDictionary* metadataDict = (CCDictionary*)dict->objectForKey("metadata");
//	if (metadataDict)
//	{
//		// try to read  texture file name from meta data
//		texturePath = metadataDict->valueForKey("textureFileName")->getCString();
//	}
//	
//	if (! texturePath.empty())
//	{
//		// build texture path relative to plist file
//		texturePath = CCFileUtils::fullPathFromRelativeFile(texturePath.c_str(), pszPath);
//	}
//	else
//	{
//		// build texture path by replacing file extension
//		texturePath = pszPath;
//		
//		// remove .xxx
//		size_t startPos = texturePath.find_last_of("."); 
//		texturePath = texturePath.erase(startPos);
//		
//		// append .png
//		texturePath = texturePath.append(".png");
//	}
//	
//	CCTexture2D *pTexture = CCTextureCache::sharedTextureCache()->addImage(texturePath.c_str());
//	
//	if (pTexture)
//	{
//		CCSpriteFrameCache::sharedSpriteFrameCache()->
//		addSpriteFramesWithDictionary(dict, pTexture);
//		printf("texture loaded: %s\n", texturePath.c_str());
//	}
//	else
//	{
//		printf("failed to load texture: %s\n", texturePath.c_str());
//	}
//	
//	return pTexture;
}

void FCAnimation_cleanTextures(CCArray * pTextures)
{
	CCObject * obj = NULL;
	CCARRAY_FOREACH(pTextures, obj)
	{
		CCTexture2D * pTexture = (CCTexture2D *)obj;
		CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromTexture(pTexture);
		CCTextureCache::sharedTextureCache()->removeTexture(pTexture);
	}
}

CCArray * FCAnimation::getTextures(void)
{
	return m_pTextures;
}

void FCAnimation::setTextures(CCArray * pTextures)
{
	FCAnimation_cleanTextures(m_pTextures);
	CC_SAFE_RELEASE_NULL(m_pTextures);
	
	if (pTextures)
	{
		m_pTextures = CCArray::arrayWithCapacity(pTextures->count());
		CCObject * obj = NULL;
		CCARRAY_FOREACH(pTextures, obj)
		{
			CCString * pTextureFilename = (CCString *)obj;
			CCTexture2D * pTexture = FCAnimation_loadTextureWithPlistPath(pTextureFilename->getCString());
			if (!pTexture)
			{
				continue;
			}
			m_pTextures->addObject(pTexture);
		}
		m_pTextures->retain();
	}
}

void FCAnimation::setTextures(CCArray * pTextures, const std::string & sTextureFolderPath)
{
	if (pTextures && sTextureFolderPath.length() > 0)
	{
		std::string path(sTextureFolderPath);
		if (path[path.length()-1] != '/')
		{
			path += '/';
		}
		
		for (int i = pTextures->count() - 1; i >= 0; i--)
		{
			CCString * pTextureFilename = (CCString *)pTextures->objectAtIndex(i);
			std::string filename = path + pTextureFilename->getCString();
			pTextures->replaceObjectAtIndex(i, CCString::stringWithCString(filename.c_str()));
		}
	}
	setTextures(pTextures);
}

NS_FC_END
