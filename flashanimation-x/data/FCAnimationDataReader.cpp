//
//  FCAnimationDataReader.cpp
//  FlashAnimation-X
//
//  Created by Moky on 12-12-15.
//  Copyright 2012 Slanissue Inc. All rights reserved.
//

#include "FCAnimationDataReader.h"

NS_FC_BEGIN

USING_NS_CC;

CCArray * FCAnimationDataReader_getStringArrayFromBuffer(const unsigned char * pBuffer, const unsigned long iBufferLength)
{
	if (!pBuffer || iBufferLength == 0)
	{
		FCLog("could not happen");
		return NULL;
	}
	
	const char * p = (const char *)pBuffer;
	unsigned short * pCount = (unsigned short *)p; // 0 - 65535
	p += sizeof(unsigned short);
	
	CCArray * array = CCArray::arrayWithCapacity(*pCount);
	char * str = new char[256];
	
	unsigned char * pLen;
	for (int i = 0; i < *pCount; i++)
	{
		pLen = (unsigned char *)p; // 0 - 255
		p += sizeof(unsigned char);
		
		memset(str, 0, 256);
		snprintf(str, *pLen + 1, "%s", p); // would append '\0' to the end
		p += *pLen;
		
		CCString * pString = new CCString(str);
		array->addObject(pString);
		pString->release();
	}
	delete[] str;
	
	return array;
}

CCDictionary * FCAnimationDataReader_getStringDictionaryFromBuffer(const unsigned char * pBuffer, const unsigned long iBufferLength)
{
	if (!pBuffer || iBufferLength == 0)
	{
		FCLog("could not happen");
		return NULL;
	}
	
	const char * p = (const char *)pBuffer;
	unsigned short * pCount = (unsigned short *)p; // 0 - 65535
	p += sizeof(unsigned short);
	
	CCDictionary * dict = CCDictionary::dictionary();
	char * str = new char[256];
	
	unsigned char * pLen;
	for (int i = 0; i < *pCount; i++)
	{
		// key
		pLen = (unsigned char *)p; // 0 - 255
		p += sizeof(unsigned char);
		
		memset(str, 0, 256);
		snprintf(str, *pLen + 1, "%s", p); // would append '\0' to the end
		p += *pLen;
		
		CCAssert(strlen(str) > 0, "could not happen");
		CCString * pKey = new CCString(str);
		
		// value
		pLen = (unsigned char *)p; // 0 - 255
		p += sizeof(unsigned char);
		
		memset(str, 0, 256);
		snprintf(str, *pLen + 1, "%s", p); // would append '\0' to the end
		p += *pLen;
		
		CCString * pValue = new CCString(str);
		
		dict->setObject(pValue, pKey->getCString());
		
		pValue->release();
		pKey->release();
	}
	delete[] str;
	
	return dict;
}

#define FCString(v)  CCString::stringWithCString(v)
#define FCFloat(v)   CCString::stringWithFormat("%f", v)
#define FCInteger(v) CCString::stringWithFormat("%d", v)

FCAnimationDataReader::FCAnimationDataReader(void)
: FCAnimationData()
, m_pFrames(NULL)
, m_pNames(NULL)
{
	
}

FCAnimationDataReader::~FCAnimationDataReader(void)
{
	setFrames(NULL);
	setNames(NULL);
}

bool FCAnimationDataReader::init(void)
{
	if (!FCAnimationData::init())
	{
		FCLog("could not happen");
		return false;
	}
	
	setFrames(NULL);
	setNames(NULL);
	return true;
}

bool FCAnimationDataReader::init(const std::string & sPath)
{
	if (!FCAnimationData::init(sPath))
	{
		FCLog("could not happen");
		return false;
	}
	
	// init frames array
	unsigned long iFramesBufferLength = 0;
	unsigned char * pFramesBuffer = getFramesBuffer(&iFramesBufferLength); // FCAnimationData::getFramesBuffer(length)
	if (!pFramesBuffer || iFramesBufferLength == 0)
	{
		FCLog("failed to get frames buffer");
		return false;
	}
	setFrames(FCAnimationDataReader_getStringArrayFromBuffer(pFramesBuffer, iFramesBufferLength));
	
	// init names dictionary
	unsigned long iNamesBufferLength = 0;
	unsigned char * pNamesBuffer = getNamesBuffer(&iNamesBufferLength); // FCAnimationData::getNamesBuffer(length)
	if (!pNamesBuffer || iNamesBufferLength == 0)
	{
		FCLog("failed to get names buffer");
		return false;
	}
	setNames(FCAnimationDataReader_getStringDictionaryFromBuffer(pNamesBuffer, iNamesBufferLength));
	
	return true;
}

#pragma mark - Data Interfaces

CCArray * FCAnimationDataReader::getTextures(void) const
{
	if (!checkDataFormat())
	{
		FCLog("could not happen");
		return NULL;
	}
	
	unsigned long iTexturesBufferLength = 0;
	unsigned char * pTexturesBuffer = getTexturesBuffer(&iTexturesBufferLength); // FCAnimationData::getTexturesBuffer(length)
	if (!pTexturesBuffer || iTexturesBufferLength == 0)
	{
		FCLog("failed to get textures buffer");
		return NULL;
	}
	return FCAnimationDataReader_getStringArrayFromBuffer(pTexturesBuffer, iTexturesBufferLength);
}

#pragma mark Symbol

int FCAnimationDataReader::getSymbolLocation(const std::string & sSymbolName) const
{
	if (sSymbolName.empty() || !m_pNames)
	{
		FCLog("error");
		return -1;
	}
	CCString * pIndex = (CCString *)m_pNames->objectForKey(sSymbolName.c_str());
	if (!pIndex)
	{
		FCLog("not exists symbol name: %s", sSymbolName.c_str());
		return -1;
	}
	return pIndex->uintValue();
}

CCDictionary * FCAnimationDataReader::getSymbolDictionary(const std::string & sSymbolName) const
{
	int iSymbolIndex = getSymbolLocation(sSymbolName);
	if (iSymbolIndex < 0 || !checkDataFormat())
	{
		FCLog("cannot get symbol dictionary for %s", sSymbolName.c_str());
		return NULL;
	}
	return getSymbolDictionary(iSymbolIndex);
}

CCDictionary * FCAnimationDataReader::getAllSymbols(void) const
{
	if (!checkDataFormat())
	{
		FCLog("could not happen");
		return NULL;
	}
	if (!m_pNames)
	{
		FCLog("error");
		return NULL;
	}
	
	CCDictionary * pLibrary = CCDictionary::dictionary();
	
	CCArray * allKeys = m_pNames->allKeys();
	CCObject * obj = NULL;
	CCARRAY_FOREACH(allKeys, obj)
	{
		const char * key = ((CCString *)obj)->getCString();
		CCDictionary * pSymbol = getSymbolDictionary(key);
		CCAssert(pSymbol, "could not happen");
		pLibrary->setObject(pSymbol, key);
	}
	
	return pLibrary;
}

CCDictionary * FCAnimationDataReader::getSymbolDictionary(const unsigned int iLocation) const
{
	FCSymbolInfo * pInfo = getSymbol(iLocation);
	if (!pInfo)
	{
		FCLog("failed to get symbol, location: %d", iLocation);
		return NULL;
	}
	return getSymbolDictionary(*pInfo);
}

CCDictionary * FCAnimationDataReader::getSymbolDictionary(const FCSymbolInfo & symbol) const
{
	CCDictionary * pSymbolData = CCDictionary::dictionary();
	
	CCString * pSymbolName = FCInteger(symbol.id);
	pSymbolData->setObject(pSymbolName, "id");
	pSymbolData->setObject(FCFloat(symbol.width), "width");
	pSymbolData->setObject(FCFloat(symbol.height), "height");
	
	if (symbol.type == FCLibrarySymbolTypeMovie)
	{
		pSymbolData->setObject(FCString("instance"), "type");
		// sub layers
		if (symbol.layers.count > 0)
		{
			CCArray * pLayers = CCArray::arrayWithCapacity(symbol.layers.count);
			FCTimelineLayerInfo * pLayer = getTimelineLayer(symbol.layers.location); // FCAnimationData::getTimelineLayer(location)
			if (pLayer)
			{
				for (int iLayerIndex = 0; iLayerIndex < symbol.layers.count; iLayerIndex++, pLayer++)
				{
					pLayers->addObject(getTimelineLayerDictionary(*pLayer));
				}
			}
			pSymbolData->setObject(pLayers, "layers");
		}
	}
	else if (symbol.type == FCLibrarySymbolTypeShape)
	{
		pSymbolData->setObject(FCString("shape"), "type");
		CCString * pFilename = (CCString *)m_pFrames->objectAtIndex(symbol.fileId);
		CCAssert(pFilename, "could not happen");
		pSymbolData->setObject(pFilename, "file");
	}
	else
	{
		FCLog("error");
	}
	
	return pSymbolData;
}

#pragma mark Timeline Layer

CCDictionary * FCAnimationDataReader::getTimelineLayerDictionary(const unsigned int iLocation) const
{
	FCTimelineLayerInfo * pInfo = getTimelineLayer(iLocation);
	if (!pInfo)
	{
		FCLog("failed to get timeline layer, location: %d", iLocation);
		return NULL;
	}
	return getTimelineLayerDictionary(*pInfo);
}

CCDictionary * FCAnimationDataReader::getTimelineLayerDictionary(const FCTimelineLayerInfo & timelineLayer) const
{
	CCDictionary * pLayerData = CCDictionary::dictionary();
	
	if (timelineLayer.type == FCTimelineLayerTypeNormal)
	{
		pLayerData->setObject(FCString("normal"), "type");
	}
	else
	{
		FCLog("unrecognized layer type");
	}
	
	// sub frames
	if (timelineLayer.frames.count > 0)
	{
		CCArray * pFrames = CCArray::arrayWithCapacity(timelineLayer.frames.count);
		FCTimelineFrameInfo * pFrame = getTimelineFrame(timelineLayer.frames.location); // FCAnimationData::getTimelineFrame(location)
		if (pFrame)
		{
			for (int iFrameIndex = 0; iFrameIndex < timelineLayer.frames.count; iFrameIndex++, pFrame++)
			{
				pFrames->addObject(getTimelineFrameDictionary(*pFrame));
			}
		}
		pLayerData->setObject(pFrames, "frames");
	}
	
	return pLayerData;
}

#pragma mark Timeline Frame

CCDictionary * FCAnimationDataReader::getTimelineFrameDictionary(const unsigned int iLocation) const
{
	FCTimelineFrameInfo * pInfo = getTimelineFrame(iLocation);
	if (!pInfo)
	{
		FCLog("failed to get timeline frame, location: %d", iLocation);
		return NULL;
	}
	return getTimelineFrameDictionary(*pInfo);
}

CCDictionary * FCAnimationDataReader::getTimelineFrameDictionary(const FCTimelineFrameInfo & timelineFrame) const
{
	CCDictionary * pFrameData = CCDictionary::dictionary();
	
	if (timelineFrame.type == FCTimelineFrameTweenTypeNone)
	{
		pFrameData->setObject(FCString("none"), "type");
	}
	else if (timelineFrame.type == FCTimelineFrameTweenTypeMotion)
	{
		pFrameData->setObject(FCString("motion"), "type");
	}
	else if (timelineFrame.type == FCTimelineFrameTweenTypeShape)
	{
		pFrameData->setObject(FCString("shape"), "type");
	}
	else
	{
		FCLog("unrecognized frame type");
	}
	
	pFrameData->setObject(FCInteger(timelineFrame.startFrame), "startFrame");
	
	pFrameData->setObject(FCInteger(timelineFrame.duration), "duration");
	
	// sub sprites
	if (timelineFrame.sprites.count > 0)
	{
		CCArray * pSprites = CCArray::arrayWithCapacity(timelineFrame.sprites.count);
		FCTimelineSpriteInfo * pSprite = getTimelineSprite(timelineFrame.sprites.location);
		if (pSprite)
		{
			for (int iSpriteIndex = 0; iSpriteIndex < timelineFrame.sprites.count; iSpriteIndex++, pSprite++)
			{
				pSprites->addObject(getTimelineSpriteDictionary(*pSprite));
			}
		}
		pFrameData->setObject(pSprites, "sprites");
	}
	
	return pFrameData;
}

#pragma mark Timeline Sprite

CCDictionary * FCAnimationDataReader::getTimelineSpriteDictionary(const unsigned int iLocation) const
{
	FCTimelineSpriteInfo * pInfo = getTimelineSprite(iLocation);
	if (!pInfo)
	{
		FCLog("failed to get timeline sprite, location: %d", iLocation);
		return NULL;
	}
	return getTimelineSpriteDictionary(*pInfo);
}

CCDictionary * FCAnimationDataReader::getTimelineSpriteDictionary(const FCTimelineSpriteInfo & timelineSprite) const
{
	CCDictionary * pSpriteData = CCDictionary::dictionary();
	
	pSpriteData->setObject(FCInteger(timelineSprite.id), "id");
	
	if (timelineSprite.type == FCTimelineSpriteTypeShape)
	{
		pSpriteData->setObject(FCString("shape"), "type");
	}
	else if (timelineSprite.type == FCTimelineSpriteTypeGraphic)
	{
		pSpriteData->setObject(FCString("graphic"), "type");
	}
	else if (timelineSprite.type == FCTimelineSpriteTypeMovieClip)
	{
		pSpriteData->setObject(FCString("movie clip"), "type");
	}
	else
	{
		FCLog("unrecognized sprite type");
	}
	
	pSpriteData->setObject(FCInteger(timelineSprite.firstFrame), "firstFrame");
	
	if (timelineSprite.loop == FCTimelineSpriteLoopTypeLoop)
	{
		pSpriteData->setObject(FCString("loop"), "loop");
	}
	else if (timelineSprite.loop == FCTimelineSpriteLoopTypeOnce)
	{
		pSpriteData->setObject(FCString("play once"), "loop");
	}
	else if (timelineSprite.loop == FCTimelineSpriteLoopTypeSingleFrame)
	{
		pSpriteData->setObject(FCString("single frame"), "loop");
	}
	else
	{
		FCLog("unrecognized loop type");
	}
	
	CCDictionary * pMatrix = CCDictionary::dictionary();
	pMatrix->setObject(FCFloat(timelineSprite.matrix.a), "a");
	pMatrix->setObject(FCFloat(timelineSprite.matrix.b), "b");
	pMatrix->setObject(FCFloat(timelineSprite.matrix.c), "c");
	pMatrix->setObject(FCFloat(timelineSprite.matrix.d), "d");
	pMatrix->setObject(FCFloat(timelineSprite.matrix.tx), "tx");
	pMatrix->setObject(FCFloat(timelineSprite.matrix.ty), "ty");
	pSpriteData->setObject(pMatrix, "matrix");
	
	pSpriteData->setObject(FCFloat(timelineSprite.x), "x");
	pSpriteData->setObject(FCFloat(timelineSprite.y), "y");
	
	pSpriteData->setObject(FCFloat(timelineSprite.anchorX), "anchorX");
	pSpriteData->setObject(FCFloat(timelineSprite.anchorY), "anchorY");
	
	pSpriteData->setObject(FCFloat(timelineSprite.scaleX), "scaleX");
	pSpriteData->setObject(FCFloat(timelineSprite.scaleY), "scaleY");
	
	pSpriteData->setObject(FCFloat(timelineSprite.skewX), "skewX");
	pSpriteData->setObject(FCFloat(timelineSprite.skewY), "skewY");
	
	pSpriteData->setObject(FCFloat(timelineSprite.rotation), "rotatioin");
	
	pSpriteData->setObject(FCFloat(timelineSprite.alpha), "alpha");
	
	return pSpriteData;
}

NS_FC_END
