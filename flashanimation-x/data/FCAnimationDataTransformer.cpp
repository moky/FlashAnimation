//
//  FCAnimationDataTransformer.cpp
//  FlashAnimation-X
//
//  Created by Moky on 12-12-15.
//  Copyright 2012 Slanissue Inc. All rights reserved.
//

#include "FCAnimationDataTransformer.h"

NS_FC_BEGIN

USING_NS_CC;

unsigned char * FCAnimationDataTransformer_createBufferWithStringArray(CCArray * pArray,
																	   unsigned int * pBufferLength)
{
	if (!pArray || pArray->count() == 0 || !pBufferLength)
	{
		return NULL;
	}
	CCAssert(pArray->count() < 65536, "too big array");
	
	unsigned int iMaxLength = 256 * pArray->count();
	
	unsigned char * pBuffer = (unsigned char *)malloc(iMaxLength);
	memset(pBuffer, 0, iMaxLength);
	char * p = (char *)pBuffer;
	
	// save 'count' first
	unsigned short * pCount = (unsigned short *)p; // 0 - 65535
	p += sizeof(unsigned short);
	*pCount = pArray->count();
	
	unsigned char * pLen;
	for (int i = 0; i < *pCount; i++)
	{
		CCString * pString = (CCString *)pArray->objectAtIndex(i);
		CCAssert(pString->length() < 256, "too long string");
		
		pLen = (unsigned char *)p; // 0 - 255
		p += sizeof(unsigned char);
		*pLen = pString->length();
		
		sprintf(p, "%s", pString->getCString());
		p += *pLen;
	}
	
	*pBufferLength = (unsigned char *)p - pBuffer;
	return pBuffer;
}

unsigned char * FCAnimationDataTransformer_createBufferWithStringDictionary(CCDictionary * pDictionary, unsigned int * pBufferLength)
{
	if (!pDictionary || !pBufferLength)
	{
		return NULL;
	}
	CCArray * pArray = pDictionary->allKeys();
	CCAssert(pArray->count() < 65536, "too big dictionary");
	
	unsigned int iMaxLength = 256 * pArray->count();
	
	unsigned char * pBuffer = (unsigned char *)malloc(iMaxLength);
	memset(pBuffer, 0, iMaxLength);
	char * p = (char *)pBuffer;
	
	// save 'count' first
	unsigned short * pCount = (unsigned short *)p; // 0 - 65535
	p += sizeof(unsigned short);
	*pCount = pArray->count();
	
	unsigned char * pLen;
	for (int i = 0; i < *pCount; i++)
	{
		CCString * pKey = (CCString *)pArray->objectAtIndex(i);
		CCAssert(pKey->length() < 256, "too long string");
		
		pLen = (unsigned char *)p; // 0 - 255
		p += sizeof(unsigned char);
		*pLen = pKey->length();
		
		sprintf(p, "%s", pKey->getCString());
		p += *pLen;
		
		// value
		CCString * pValue = (CCString *)pDictionary->objectForKey(pKey->getCString());
		
		pLen = (unsigned char *)p; // 0 - 255
		p += sizeof(unsigned char);
		*pLen = pValue->length();
		
		sprintf(p, "%s", pValue->getCString());
		p += *pLen;
	}
	
	*pBufferLength = (unsigned char *)p - pBuffer;
	return pBuffer;
}

#pragma mark Textures

unsigned char * FCAnimationDataTransformer_createTextureBuffer(CCArray * pTextures,
															   unsigned int * pBufferLength)
{
	return FCAnimationDataTransformer_createBufferWithStringArray(pTextures, pBufferLength);
}

#pragma mark Frames array

unsigned char * FCAnimationDataTransformer_createFrameBuffer(CCArray * pFrames, unsigned int * pBufferLength)
{
	return FCAnimationDataTransformer_createBufferWithStringArray(pFrames, pBufferLength);
}

#pragma mark Symbol name-offset map

unsigned char * FCAnimationDataTransformer_createNameBuffer(CCDictionary * pNames, unsigned int * pBufferLength)
{
	return FCAnimationDataTransformer_createBufferWithStringDictionary(pNames, pBufferLength);
}

#pragma mark Symbol Library (sprites)

void FCAnimationDataTransformer_processTimelineSprite(CCDictionary * pSpriteData,
													  // buffers
													  FCTimelineSpriteInfo * pTimelineSpriteBuffer, unsigned int * pTotalSpritesCount,
													  // others
													  CCArray * pSpritesNotLocated, CCDictionary * pNames)
{
	CCAssert(pSpriteData, "invalid sprite data");
	FCTimelineSpriteInfo * pSprite = pTimelineSpriteBuffer + *pTotalSpritesCount;
	
	// sprite name
	CCString * pSpriteName = (CCString *)pSpriteData->objectForKey("id");
	CCString * pIndex = (CCString *)pNames->objectForKey(pSpriteName->getCString());
	if (pIndex)
	{
		pSprite->id = pIndex->uintValue();
	}
	else
	{
		CCDictionary * pDict = CCDictionary::dictionary();
		pDict->setObject(pSpriteName, "id");
		pDict->setObject(CCString::stringWithFormat("%d", *pTotalSpritesCount), "location");
		pSpritesNotLocated->addObject(pDict);
	}
	
	// sprite type
	CCString * pSpriteType = (CCString *)pSpriteData->objectForKey("type");
	if (!pSpriteType)
	{
		FCLog("sprite type is empty");
	}
	else if (FCStringEqualToString(pSpriteType, "shape"))
	{
		pSprite->type = FCTimelineSpriteTypeShape;
	}
	else if (FCStringEqualToString(pSpriteType, "graphic"))
	{
		pSprite->type = FCTimelineSpriteTypeGraphic;
	}
	else if (FCStringEqualToString(pSpriteType, "movie clip"))
	{
		pSprite->type = FCTimelineSpriteTypeMovieClip;
	}
	
	// first frame
	CCString * pFirstFrame = (CCString *)pSpriteData->objectForKey("firstFrame");
	pSprite->firstFrame = pFirstFrame ? pFirstFrame->uintValue() : 0;
	
	// loop
	CCString * pSpriteLoop = (CCString *)pSpriteData->objectForKey("loop");
	if (!pSpriteLoop)
	{
	}
	else if (FCStringEqualToString(pSpriteLoop, "loop"))
	{
		pSprite->loop = FCTimelineSpriteLoopTypeLoop;
	}
	else if (FCStringEqualToString(pSpriteLoop, "play once"))
	{
		pSprite->loop = FCTimelineSpriteLoopTypeOnce;
	}
	else if (FCStringEqualToString(pSpriteLoop, "single frame"))
	{
		pSprite->loop = FCTimelineSpriteLoopTypeSingleFrame;
	}
	else
	{
		FCLog("unrecognized loop type: %s", pSpriteLoop->getCString());
	}
	
	// matrix
	CCDictionary * pMatrix = (CCDictionary *)pSpriteData->objectForKey("matrix");
	if (pMatrix)
	{
		CCString * a = (CCString *)pMatrix->objectForKey("a");
		CCString * b = (CCString *)pMatrix->objectForKey("b");
		CCString * c = (CCString *)pMatrix->objectForKey("c");
		CCString * d = (CCString *)pMatrix->objectForKey("d");
		CCString * tx = (CCString *)pMatrix->objectForKey("tx");
		CCString * ty = (CCString *)pMatrix->objectForKey("ty");
		pSprite->matrix.a = a->floatValue();
		pSprite->matrix.b = b->floatValue();
		pSprite->matrix.c = c->floatValue();
		pSprite->matrix.d = d->floatValue();
		pSprite->matrix.tx = tx->floatValue();
		pSprite->matrix.ty = ty->floatValue();
	}
	
	// position
	CCString * pX = (CCString *)pSpriteData->objectForKey("x");
	CCString * pY = (CCString *)pSpriteData->objectForKey("y");
	pSprite->x = pX ? pX->floatValue() : 0;
	pSprite->y = pY ? pY->floatValue() : 0;
	
	// anchor point
	CCString * pAnchorX = (CCString *)pSpriteData->objectForKey("anchorX");
	CCString * pAnchorY = (CCString *)pSpriteData->objectForKey("anchorY");
	pSprite->anchorX = pAnchorX ? pAnchorX->floatValue() : 0.5;
	pSprite->anchorY = pAnchorY ? pAnchorY->floatValue() : 0.5;
	
	// scale
	CCString * pScaleX = (CCString *)pSpriteData->objectForKey("scaleX");
	CCString * pScaleY = (CCString *)pSpriteData->objectForKey("scaleY");
	pSprite->scaleX = pScaleX ? pScaleX->floatValue() : 1;
	pSprite->scaleY = pScaleY ? pScaleY->floatValue() : 1;
	
	// skew
	CCString * pSkewX = (CCString *)pSpriteData->objectForKey("skewX");
	CCString * pSkewY = (CCString *)pSpriteData->objectForKey("skewY");
	pSprite->skewX = pSkewX ? pSkewX->floatValue() : 0;
	pSprite->skewY = pSkewY ? pSkewY->floatValue() : 0;
	
	// rotation
	CCString * pRotation = (CCString *)pSpriteData->objectForKey("rotation");
	pSprite->rotation = pRotation ? pRotation->floatValue() : 0;
	
	// alpha
	CCString * pAlpha = (CCString *)pSpriteData->objectForKey("alpha");
	pSprite->alpha = pAlpha ? pAlpha->floatValue() : 1;
	
	(*pTotalSpritesCount)++;
}

void FCAnimationDataTransformer_processTimelineFrame(CCDictionary * pFrameData,
													 // buffers
													 FCTimelineFrameInfo * pTimelineFrameBuffer, unsigned int * pTotalFramesCount,
													 FCTimelineSpriteInfo * pTimelineSpriteBuffer, unsigned int * pTotalSpritesCount,
													 // others
													 CCArray * pSpritesNotLocated, CCDictionary * pNames)
{
	CCAssert(pFrameData, "invalid timeline frame data");
	FCTimelineFrameInfo * pFrame = pTimelineFrameBuffer + *pTotalFramesCount;
	
	// frame type
	CCString * pFrameType = (CCString *)pFrameData->objectForKey("type");
	if (!pFrameType)
	{
		FCLog("frame type is empty");
	}
	else if (FCStringEqualToString(pFrameType, "none"))
	{
		pFrame->type = FCTimelineFrameTweenTypeNone;
	}
	else if (FCStringEqualToString(pFrameType, "motion"))
	{
		pFrame->type = FCTimelineFrameTweenTypeMotion;
	}
	else if (FCStringEqualToString(pFrameType, "shape"))
	{
		pFrame->type = FCTimelineFrameTweenTypeShape;
	}
	else
	{
		FCLog("unrecognized frame type: %s", pFrameType->getCString());
	}
	
	// start frame
	CCString * pStartFrame = (CCString *)pFrameData->objectForKey("startFrame");
	pFrame->startFrame = pStartFrame ? pStartFrame->uintValue() : 0;
	
	// duration
	CCString * pDuration = (CCString *)pFrameData->objectForKey("duration");
	pFrame->duration = pDuration ? pDuration->uintValue() : 0;
	
	
	
	
	
	// start processing sprites
	CCArray * pSprites = (CCArray *)pFrameData->objectForKey("sprites");
	CCAssert(pSprites, "sprites should not be NULL");
	
	pFrame->sprites.location = *pTotalSpritesCount;
	pFrame->sprites.count = pSprites->count();
	
	for (int iSpriteIndex = 0; iSpriteIndex < pFrame->sprites.count; iSpriteIndex++)
	{
		CCDictionary * pSpriteData = (CCDictionary *)pSprites->objectAtIndex(iSpriteIndex);
		CCAssert(pSpriteData, "could not happen");
		
		FCAnimationDataTransformer_processTimelineSprite(pSpriteData,
														 // buffers
														 pTimelineSpriteBuffer, pTotalSpritesCount,
														 // others
														 pSpritesNotLocated, pNames);
	} // EOF sprites
	
	
	
	
	
	(*pTotalFramesCount)++;
}

void FCAnimationDataTransformer_processTimelineLayer(CCDictionary * pLayerData,
													 // buffers
													 FCTimelineLayerInfo * pTimelineLayerBuffer, unsigned int * pTotalLayersCount,
													 FCTimelineFrameInfo * pTimelineFrameBuffer, unsigned int * pTotalFramesCount,
													 FCTimelineSpriteInfo * pTimelineSpriteBuffer, unsigned int * pTotalSpritesCount,
													 // others
													 CCArray * pSpritesNotLocated, CCDictionary * pNames)
{
	CCAssert(pLayerData, "invalid timeline layer data");
	FCTimelineLayerInfo * pLayer = pTimelineLayerBuffer + *pTotalLayersCount;
	
	// layer name
	CCString * pName = (CCString *)pLayerData->objectForKey("name");
	if (pName)
	{
		FCLog("layer name: %s", pName->getCString());
	}
	
	// layer type
	CCString * pLayerType = (CCString *)pLayerData->objectForKey("type");
	if (!pLayerType)
	{
		FCLog("layer type is empty");
	}
	else if (FCStringEqualToString(pLayerType, "normal"))
	{
		pLayer->type = FCTimelineLayerTypeNormal;
	}
	else
	{
		FCLog("unrecognized layer type: %s", pLayerType->getCString());
	}
	
	
	
	
	
	// start processing frames
	CCArray * pFrames = (CCArray *)pLayerData->objectForKey("frames");
	CCAssert(pFrames, "frames should not be NULL");
	
	pLayer->frames.location = *pTotalFramesCount;
	pLayer->frames.count = pFrames->count();
	
	for (int iFrameIndex = 0; iFrameIndex < pLayer->frames.count; iFrameIndex++)
	{
		CCDictionary * pFrameData = (CCDictionary *)pFrames->objectAtIndex(iFrameIndex);
		CCAssert(pFrameData, "could not happen");
		
		FCAnimationDataTransformer_processTimelineFrame(pFrameData,
														// buffers
														pTimelineFrameBuffer, pTotalFramesCount,
														pTimelineSpriteBuffer, pTotalSpritesCount,
														// others
														pSpritesNotLocated, pNames);
	} // EOF frames
	
	
	
	
	
	(*pTotalLayersCount)++;
}

void FCAnimationDataTransformer_processSymbol(CCDictionary * pSymbolData, CCArray * pSpritesNotLocated,
											  // buffers
											  FCSymbolInfo * pSymbolBuffer, unsigned int * pTotalSymbolsCount,
											  FCTimelineLayerInfo * pTimelineLayerBuffer, unsigned int * pTotalLayersCount,
											  FCTimelineFrameInfo * pTimelineFrameBuffer, unsigned int * pTotalFramesCount,
											  FCTimelineSpriteInfo * pTimelineSpriteBuffer, unsigned int * pTotalSpritesCount,
											  // others
											  CCArray * pFrames, CCDictionary * pNames)
{
	CCAssert(pSymbolData, "invalid symbol data");
	FCSymbolInfo * pSymbol = pSymbolBuffer + *pTotalSymbolsCount;
	// symbol id
	pSymbol->id = *pTotalSymbolsCount;
	
	// symbol width,height
	CCString * pSymbolWidth = (CCString *)pSymbolData->objectForKey("width");
	pSymbol->width = pSymbolWidth ? pSymbolWidth->floatValue() : 0;
	CCString * pSymbolHeight = (CCString *)pSymbolData->objectForKey("height");
	pSymbol->height = pSymbolHeight ? pSymbolHeight->floatValue() : 0;
	
	// symbol type
	CCString * pSymbolType = (CCString *)pSymbolData->objectForKey("type");
	if (!pSymbolType)
	{
		FCLog("symbol type is empty");
	}
	else if (FCStringEqualToString(pSymbolType, "shape"))
	{
		pSymbol->type = FCLibrarySymbolTypeShape;
		pSymbol->fileId = pFrames->count();
		pFrames->addObject((CCString *)pSymbolData->objectForKey("file"));
	}
	else if (FCStringEqualToString(pSymbolType, "instance"))
	{
		pSymbol->type = FCLibrarySymbolTypeMovie;
		
		
		
		
		
		// start processing layers
		CCArray * pLayers = (CCArray *)pSymbolData->objectForKey("layers");
		CCAssert(pLayers, "layers should not be NULL");
		
		pSymbol->layers.location = *pTotalLayersCount;
		pSymbol->layers.count = pLayers->count();
		
		FCTimelineLayerInfo * pLayer = pTimelineLayerBuffer + pSymbol->layers.location;
		for (int iLayerIndex = 0; iLayerIndex < pSymbol->layers.count; iLayerIndex++, pLayer++)
		{
			CCDictionary * pLayerData = (CCDictionary *)pLayers->objectAtIndex(iLayerIndex);
			CCAssert(pLayerData, "could not happen");
			
			FCAnimationDataTransformer_processTimelineLayer(pLayerData,
															// buffers
															pTimelineLayerBuffer, pTotalLayersCount,
															pTimelineFrameBuffer, pTotalFramesCount,
															pTimelineSpriteBuffer, pTotalSpritesCount,
															// others
															pSpritesNotLocated, pNames);
		} // EOF layers
		
		
		
		
		
	}
	else
	{
		FCLog("unrecognized symbol type: %s", pSymbolType->getCString());
	}
	
	(*pTotalSymbolsCount)++;
}

unsigned char * FCAnimationDataTransformer_createLibraryBuffer(CCDictionary * pSymbols, unsigned int * pBufferLength,
															   CCArray * pFrames, CCDictionary * pNames)
{
	if (!pFrames || !pBufferLength || !pNames || !pSymbols)
	{
		FCLog("error");
		return NULL;
	}
	
	CCArray * pSymbolKeys = pSymbols->allKeys();
	
	// create buffers
	FCSymbolInfo * pSymbolBuffer = (FCSymbolInfo *)malloc(sizeof(FCSymbolInfo) * pSymbolKeys->count());
	FCTimelineLayerInfo * pTimelineLayerBuffer = (FCTimelineLayerInfo *)malloc(sizeof(FCTimelineLayerInfo) * 65536);
	FCTimelineFrameInfo * pTimelineFrameBuffer = (FCTimelineFrameInfo *)malloc(sizeof(FCTimelineFrameInfo) * 65536);
	FCTimelineSpriteInfo * pTimelineSpriteBuffer = (FCTimelineSpriteInfo *)malloc(sizeof(FCTimelineSpriteInfo) * 65536);
	if (!pSymbolBuffer || !pTimelineLayerBuffer || !pTimelineFrameBuffer || !pTimelineSpriteBuffer)
	{
		FCLog("not enough memory");
		CC_SAFE_FREE(pSymbolBuffer);
		CC_SAFE_FREE(pTimelineLayerBuffer);
		CC_SAFE_FREE(pTimelineFrameBuffer);
		CC_SAFE_FREE(pTimelineSpriteBuffer);
		return NULL;
	}
	memset(pSymbolBuffer, 0, sizeof(FCSymbolInfo) * pSymbolKeys->count());
	memset(pTimelineLayerBuffer, 0, sizeof(FCTimelineLayerInfo) * 65536);
	memset(pTimelineFrameBuffer, 0, sizeof(FCTimelineFrameInfo) * 65536);
	memset(pTimelineSpriteBuffer, 0, sizeof(FCTimelineSpriteInfo) * 65536);
	
	unsigned int iTotalSymbolsCount = 0;
	unsigned int iTotalLayersCount = 0;
	unsigned int iTotalFramesCount = 0;
	unsigned int iTotalSpritesCount = 0;
	
	CCArray * pSpritesNotLocated = CCArray::arrayWithCapacity(65536); // the symbols maybe disordered
	
	
	
	
	
	// start processing symbols
	for (int iSymbolIndex = 0; iSymbolIndex < pSymbolKeys->count(); iSymbolIndex++)
	{
		// symbol name
		CCString * pSymbolName = (CCString *)pSymbolKeys->objectAtIndex(iSymbolIndex);
		CCDictionary * pSymbolData = (CCDictionary *)pSymbols->objectForKey(pSymbolName->getCString());
		CCAssert(pSymbolData, "could not happen");
		
		// safe 'symbol name -> id' pairs
		pNames->setObject(CCString::stringWithFormat("%d", iSymbolIndex), pSymbolName->getCString());
		
		FCAnimationDataTransformer_processSymbol(pSymbolData, pSpritesNotLocated,
												 // buffers
												 pSymbolBuffer, &iTotalSymbolsCount,
												 pTimelineLayerBuffer, &iTotalLayersCount,
												 pTimelineFrameBuffer, &iTotalFramesCount,
												 pTimelineSpriteBuffer, &iTotalSpritesCount,
												 // others
												 pFrames, pNames);
	} // EOF symbols
	
	
	
	
	
	// relocate
	FCLog("%d sprite(s) lost, relocated them", pSpritesNotLocated->count());
	for (int pos = 0; pos < pSpritesNotLocated->count(); pos++)
	{
		CCDictionary * pDict = (CCDictionary *)pSpritesNotLocated->objectAtIndex(pos);
		CCString * pLocation = (CCString *)pDict->objectForKey("location");
		FCTimelineSpriteInfo * pSprite = pTimelineSpriteBuffer + pLocation->uintValue();
		
		CCString * pSpriteName = (CCString *)pDict->objectForKey("id");
		pLocation = (CCString *)pNames->objectForKey(pSpriteName->getCString());
		CCAssert(pLocation, "could not happen");
		pSprite->id = pLocation->uintValue();
	}
	
	// merging
	FCLog("All done! now merging the 4 buffers...");
	unsigned long iBufferLength = sizeof(FCLibraryBufferHead)
	+ sizeof(FCSymbolInfo) * iTotalSymbolsCount
	+ sizeof(FCTimelineLayerInfo) * iTotalLayersCount
	+ sizeof(FCTimelineFrameInfo) * iTotalFramesCount
	+ sizeof(FCTimelineSpriteInfo) * iTotalSpritesCount;
	
	FCLog("creating buffer length: %ld", iBufferLength);
	unsigned char * pDataBuffer = (unsigned char *)malloc(iBufferLength);
	if (!pDataBuffer)
	{
		FCLog("not enough memory");
		CC_SAFE_FREE(pSymbolBuffer);
		CC_SAFE_FREE(pTimelineLayerBuffer);
		CC_SAFE_FREE(pTimelineFrameBuffer);
		CC_SAFE_FREE(pTimelineSpriteBuffer);
		return NULL;
	}
	memset(pDataBuffer, 0, iBufferLength);
	
	// set head values
	FCLibraryBufferHead * pLibHead = (FCLibraryBufferHead *)pDataBuffer;
	
	pLibHead->symbolsBuffer.offset = sizeof(FCLibraryBufferHead);
	pLibHead->symbolsBuffer.length = sizeof(FCSymbolInfo) * iTotalSymbolsCount;
	
	pLibHead->timelineLayersBuffer.offset = pLibHead->symbolsBuffer.offset + pLibHead->symbolsBuffer.length;
	pLibHead->timelineLayersBuffer.length = sizeof(FCTimelineLayerInfo) * iTotalLayersCount;
	
	pLibHead->timelineFramesBuffer.offset = pLibHead->timelineLayersBuffer.offset + pLibHead->timelineLayersBuffer.length;
	pLibHead->timelineFramesBuffer.length = sizeof(FCTimelineFrameInfo) * iTotalFramesCount;
	
	pLibHead->timelineSpritesBuffer.offset = pLibHead->timelineFramesBuffer.offset + pLibHead->timelineFramesBuffer.length;
	pLibHead->timelineSpritesBuffer.length = sizeof(FCTimelineSpriteInfo) * iTotalSpritesCount;
	
	// copy buffers
	unsigned char * p;
	
	p = pDataBuffer + pLibHead->symbolsBuffer.offset;
	memcpy(p, pSymbolBuffer, pLibHead->symbolsBuffer.length);
	
	p = pDataBuffer + pLibHead->timelineLayersBuffer.offset;
	memcpy(p, pTimelineLayerBuffer, pLibHead->timelineLayersBuffer.length);
	
	p = pDataBuffer + pLibHead->timelineFramesBuffer.offset;
	memcpy(p, pTimelineFrameBuffer, pLibHead->timelineFramesBuffer.length);
	
	p = pDataBuffer + pLibHead->timelineSpritesBuffer.offset;
	memcpy(p, pTimelineSpriteBuffer, pLibHead->timelineSpritesBuffer.length);
	
	// free temporary buffers
	CC_SAFE_FREE(pSymbolBuffer);
	CC_SAFE_FREE(pTimelineLayerBuffer);
	CC_SAFE_FREE(pTimelineFrameBuffer);
	CC_SAFE_FREE(pTimelineSpriteBuffer);
	
	// return buffer and length
	*pBufferLength = iBufferLength;
	return pDataBuffer;
}

#pragma mark -

bool FCAnimationDataTransformer::init(const std::string & sPath)
{
	if (sPath.empty())
	{
		FCLog("path is NULL");
		return false;
	}
	std::size_t pos = sPath.rfind(".");
	if (pos == std::string::npos)
	{
		FCLog("error path: %s", sPath.c_str());
		return false;
	}
	std::string ext = sPath.substr(pos + 1);
	if (strcasecmp(ext.c_str(), "plist") != 0)
	{
		FCLog("warning: invalid file ext, path = %s", sPath.c_str());
	}
	
	CCDictionary * pDict = CCDictionary::dictionaryWithContentsOfFile(sPath.c_str());
	return init(pDict);
}

bool FCAnimationDataTransformer::init(CCDictionary * pDict)
{
	if (!pDict)
	{
		return false;
	}
	
	CCArray * pFrames = CCArray::arrayWithCapacity(256);
	CCDictionary * pNames = CCDictionary::dictionary();
	
	// symbols library (symbols + layers + frames + sprites)
	CCDictionary * pSymbols = (CCDictionary *)pDict->objectForKey("sprites");
	unsigned int iLibraryBufferLength = 0;
	unsigned char * pLibraryBuffer = FCAnimationDataTransformer_createLibraryBuffer(pSymbols, &iLibraryBufferLength,
																					pFrames, pNames);
	if (!pLibraryBuffer || iLibraryBufferLength == 0)
	{
		FCLog("error");
		CC_SAFE_FREE(pLibraryBuffer);
		return false;
	}
	
	// sprite frames (file id-name array)
	unsigned int iFrameBufferLength = 0;
	unsigned char * pFrameBuffer = FCAnimationDataTransformer_createFrameBuffer(pFrames, &iFrameBufferLength);
	if (iFrameBufferLength == 0 || !pFrameBuffer || pFrames->count() == 0)
	{
		FCLog("error");
		CC_SAFE_FREE(pFrameBuffer);
		CC_SAFE_FREE(pLibraryBuffer);
		return false;
	}
	
	//  symbol names (symbol name-offset map dictionary)
	unsigned int iNameBufferLength = 0;
	unsigned char * pNameBuffer = FCAnimationDataTransformer_createNameBuffer(pNames, &iNameBufferLength);
	if (!pNameBuffer || iNameBufferLength == 0)
	{
		FCLog("error");
		CC_SAFE_FREE(pNameBuffer);
		CC_SAFE_FREE(pFrameBuffer);
		CC_SAFE_FREE(pLibraryBuffer);
		return false;
	}
	
	// textures
	CCArray * pTextures = (CCArray *)pDict->objectForKey("textures");
	unsigned int iTextureBufferLength = 0;
	unsigned char * pTextureBuffer = FCAnimationDataTransformer_createTextureBuffer(pTextures, &iTextureBufferLength);
	if (!pTextureBuffer || iTextureBufferLength == 0)
	{
		FCLog("error");
		CC_SAFE_FREE(pTextureBuffer);
		CC_SAFE_FREE(pNameBuffer);
		CC_SAFE_FREE(pFrameBuffer);
		CC_SAFE_FREE(pLibraryBuffer);
		return false;
	}
	
	//---- OK!
	FCLog("sizeof(FCAnimationDataHead) = %ld, iFrameBufferLength = %d, iSymbolBufferLength = %d, iNameBufferLength = %d, iTextureBufferLength = %d",
		  sizeof(FCAnimationDataHead), iFrameBufferLength, iLibraryBufferLength, iNameBufferLength, iTextureBufferLength);
	
	
	unsigned long iBufferLength = sizeof(FCAnimationDataHead) + iFrameBufferLength + iLibraryBufferLength + iNameBufferLength + iTextureBufferLength;
	FCLog("iBufferLength = %d", iBufferLength);
	
	if (!FCAnimationData::init(iBufferLength))
	{
		CC_SAFE_FREE(pTextureBuffer);
		CC_SAFE_FREE(pNameBuffer);
		CC_SAFE_FREE(pFrameBuffer);
		CC_SAFE_FREE(pLibraryBuffer);
		return false;
	}
	
	// init head
	unsigned int iHeadOffset = 0;
	unsigned int iHeadLength = sizeof(FCAnimationDataHead);
	FCAnimationDataHead * pHeadInfo = (FCAnimationDataHead *)&m_pDataBuffer[iHeadOffset];
	
	// frame rate
	CCString * pFrameRate = (CCString *)pDict->objectForKey("frameRate");
	if (pFrameRate)
	{
		pHeadInfo->info.frameRate = pFrameRate->uintValue();
	}
	
	// root id
	CCString * pRootId = (CCString *)pDict->objectForKey("rootId");
	CCAssert(pRootId, "rootId not found!");
	char * pszRootId = (char *)(pHeadInfo->info.rootId);
	snprintf(pszRootId, sizeof(pHeadInfo->info.rootId), "%s", pRootId->getCString());
	
	// copy symbol buffer
	unsigned int iLibraryBufferOffset = iHeadOffset + iHeadLength;
	memcpy(&m_pDataBuffer[iLibraryBufferOffset], pLibraryBuffer, iLibraryBufferLength);
	pHeadInfo->info.libraryBuffer.offset = iLibraryBufferOffset;
	pHeadInfo->info.libraryBuffer.length = iLibraryBufferLength;
	
	// copy frame buffer
	unsigned int iFrameBufferOffset = iLibraryBufferOffset + iLibraryBufferLength;
	memcpy(&m_pDataBuffer[iFrameBufferOffset], pFrameBuffer, iFrameBufferLength);
	pHeadInfo->info.framesBuffer.offset = iFrameBufferOffset;
	pHeadInfo->info.framesBuffer.length = iFrameBufferLength;
	
	// copy name buffer
	unsigned int iNameBufferOffset = iFrameBufferOffset + iFrameBufferLength;
	memcpy(&m_pDataBuffer[iNameBufferOffset], pNameBuffer, iNameBufferLength);
	pHeadInfo->info.namesBuffer.offset = iNameBufferOffset;
	pHeadInfo->info.namesBuffer.length = iNameBufferLength;
	
	// copy texture buffer
	unsigned int iTextureBufferOffset = iNameBufferOffset + iNameBufferLength;
	memcpy(&m_pDataBuffer[iTextureBufferOffset], pTextureBuffer, iTextureBufferLength);
	pHeadInfo->info.texturesBuffer.offset = iTextureBufferOffset;
	pHeadInfo->info.texturesBuffer.length = iTextureBufferLength;
	
	CC_SAFE_FREE(pTextureBuffer);
	CC_SAFE_FREE(pNameBuffer);
	CC_SAFE_FREE(pFrameBuffer);
	CC_SAFE_FREE(pLibraryBuffer);
	
	m_bIsDataValid = checkDataFormat();
	return m_bIsDataValid;
}

NS_FC_END
