//
//  FCAnimationData.cpp
//  FlashAnimation-X
//
//  Created by Moky on 12-12-12.
//  Copyright 2012 Slanissue Inc. All rights reserved.
//

#include "FCAnimationData.h"

NS_FC_BEGIN

#define FC_HEAD_VERSION 0x01
#define FC_VERSION      0x01

USING_NS_CC;

FCAnimationData::FCAnimationData(void)
: m_pDataBuffer(NULL)
, m_iBufferLength(0)
, m_bIsDataValid(false)
{
	
}

FCAnimationData::~FCAnimationData(void)
{
	CC_SAFE_FREE(m_pDataBuffer);
}

bool FCAnimationData::init(void)
{
	CC_SAFE_FREE(m_pDataBuffer);
	m_pDataBuffer = NULL;
	m_iBufferLength = 0;
	m_bIsDataValid = false;
	return true;
}

bool FCAnimationData::init(const unsigned long iBufferLength)
{
	if (iBufferLength <= sizeof(FCAnimationDataHead))
	{
		FCLog("not enough buffer length: %ld", iBufferLength);
		return false;
	}
	
	if (!init())
	{
		return false;
	}
	
	m_pDataBuffer = (unsigned char *)malloc(iBufferLength);
	if (!m_pDataBuffer)
	{
		FCLog("not enough memory");
		return NULL;
	}
	m_iBufferLength = iBufferLength;
	
	// init the buffer
	memset(m_pDataBuffer, 0, m_iBufferLength);
	
	FCAnimationDataHead * pHeadInfo = (FCAnimationDataHead *)m_pDataBuffer;
	
	// protocol
	pHeadInfo->format[0] = 'F';
	pHeadInfo->format[1] = '2';
	pHeadInfo->format[2] = 'C';
	pHeadInfo->format[3] = '\0';
	
	pHeadInfo->headVersion = FC_HEAD_VERSION;
	pHeadInfo->version = FC_VERSION;
	
	pHeadInfo->fileLength = m_iBufferLength;
	
	// descriptions
	time_t now = time(NULL);
	tm * tm = gmtime(&now);
	
	snprintf((char *)pHeadInfo->description, sizeof(pHeadInfo->description),
			 "Data file of FlashToCocos2d. Generated at %d-%d-%d %d:%d:%d",
			 tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);
	
	snprintf((char *)pHeadInfo->copyright, sizeof(pHeadInfo->copyright),
			 "Copyright %d Slanissue Inc.", tm->tm_year + 1900);
	
	snprintf((char *)pHeadInfo->author, sizeof(pHeadInfo->author),
			 "Author: Moky@Beva.com, %d-%d-%d", 12, 12, 12);
	
	return true;
}

bool FCAnimationData::init(const std::string & sPath)
{
	if (sPath.empty() || !init())
	{
		return false;
	}
	
	CCFileData data(sPath.c_str(), "rb");
	unsigned char * pBuffer = data.getBuffer();
	unsigned long lSize = data.getSize();
	if (!pBuffer || lSize <= sizeof(FCAnimationDataHead))
	{
		FCLog("invalid data file: %s, size: %ld", sPath.c_str(), lSize);
		return false;
	}
	FCLog("read %ld bytes from file: %s", lSize, sPath.c_str());
	
	m_pDataBuffer = (unsigned char *)malloc(lSize);
	if (!m_pDataBuffer)
	{
		FCLog("not enough memory, size: %ld", lSize);
		return false;
	}
	m_iBufferLength = lSize;
	memset(m_pDataBuffer, 0, m_iBufferLength);
	memcpy(m_pDataBuffer, pBuffer, m_iBufferLength);
	
	m_bIsDataValid = checkDataFormat();
	return m_bIsDataValid;
}

bool FCAnimationData::saveToFile(const std::string & sPath) const
{
	if (sPath.empty())
	{
		return false;
	}
	if (!checkDataFormat())
	{
		FCLog("data format error");
		return false;
	}
	
	FCLog("saving to binary file: %s", sPath.c_str());
	
	FILE * fp = fopen(sPath.c_str(), "wb");
	if (!fp)
	{
		FCLog("failed to open file for write: %s", sPath.c_str());
		return false;
	}
	unsigned long length = fwrite(m_pDataBuffer, sizeof(unsigned char), m_iBufferLength, fp);
	fclose(fp);
	FCLog("buffer length: %ld, written length: %ld", m_iBufferLength, length);
	return length == m_iBufferLength;
}

bool FCAnimationData::checkDataFormat(void) const
{
	if (m_bIsDataValid)
	{
		// the data had been checked before
		// quick return here
		return true;
	}
	
	if (!m_pDataBuffer || m_iBufferLength <= sizeof(FCAnimationDataHead))
	{
		FCLog("data buffer not init");
		return false;
	}
	
	// check head
	FCAnimationDataHead * pHead = (FCAnimationDataHead *)&m_pDataBuffer[0];
	if (pHead->format[0] != 'F' || pHead->format[1] != '2' || pHead->format[2] != 'C' || pHead->format[3] != '\0')
	{
		FCLog("format sign error: [%c%c%c%c]", pHead->format[0], pHead->format[1], pHead->format[2], pHead->format[3]);
		return false;
	}
	
	if (pHead->headVersion != FC_HEAD_VERSION || pHead->version != FC_VERSION)
	{
		FCLog("versions error: %d.%d", pHead->headVersion, pHead->version);
		return false;
	}
	
	if (pHead->fileLength != m_iBufferLength)
	{
		FCLog("file length error: %ld != %ld", pHead->fileLength, m_iBufferLength);
		return false;
	}
	
	// check index info
	FCAnimationDataInfo bodyIndex = pHead->info;
	
	if (bodyIndex.libraryBuffer.offset != sizeof(FCAnimationDataHead) ||
		bodyIndex.libraryBuffer.length == 0)
	{
		FCLog("symbol buffer info error");
		return false;
	}
	
	if (bodyIndex.framesBuffer.offset != bodyIndex.libraryBuffer.offset + bodyIndex.libraryBuffer.length ||
		bodyIndex.framesBuffer.length == 0)
	{
		FCLog("frame buffer info error");
		return false;
	}
	
	if (bodyIndex.namesBuffer.offset != bodyIndex.framesBuffer.offset + bodyIndex.framesBuffer.length ||
		bodyIndex.namesBuffer.length == 0)
	{
		FCLog("name buffer info error");
		return false;
	}
	
	if (bodyIndex.texturesBuffer.offset != bodyIndex.namesBuffer.offset + bodyIndex.namesBuffer.length ||
		bodyIndex.texturesBuffer.length == 0)
	{
		FCLog("texture buffer info error");
		return false;
	}
	
	unsigned long fileLength = sizeof(FCAnimationDataHead)
	+ bodyIndex.libraryBuffer.length
	+ bodyIndex.framesBuffer.length
	+ bodyIndex.namesBuffer.length
	+ bodyIndex.texturesBuffer.length;
	
	if (fileLength != m_iBufferLength)
	{
		FCLog("file length error");
		return false;
	}
	
	// extra: check library
	FCLibraryBufferHead * pLib = (FCLibraryBufferHead *)&m_pDataBuffer[bodyIndex.libraryBuffer.offset];
	
	if (pLib->symbolsBuffer.offset != sizeof(FCLibraryBufferHead) ||
		pLib->symbolsBuffer.length == 0)
	{
		FCLog("library.symbols buffer info error");
		return false;
	}
	
	if (pLib->timelineLayersBuffer.offset != pLib->symbolsBuffer.offset + pLib->symbolsBuffer.length ||
		pLib->timelineLayersBuffer.length == 0)
	{
		FCLog("library.layers buffer info error");
		return false;
	}
	
	if (pLib->timelineFramesBuffer.offset != pLib->timelineLayersBuffer.offset + pLib->timelineLayersBuffer.length ||
		pLib->timelineFramesBuffer.length == 0)
	{
		FCLog("library.frames buffer info error");
		return false;
	}
	
	if (pLib->timelineSpritesBuffer.offset != pLib->timelineFramesBuffer.offset + pLib->timelineFramesBuffer.length ||
		pLib->timelineSpritesBuffer.length == 0)
	{
		FCLog("library.sprites buffer info error");
		return false;
	}
	
	return true;
}

#pragma mark Interfaces

// head info: frameRate
unsigned int FCAnimationData::getFrameRate(void) const
{
	if (!checkDataFormat())
	{
		return 0;
	}
	FCAnimationDataHead * pHead = (FCAnimationDataHead *)m_pDataBuffer;
	return pHead->info.frameRate;
}

// head info: rootId
const char * FCAnimationData::getRootId(void) const
{
	if (!checkDataFormat())
	{
		return NULL;
	}
	
	FCAnimationDataHead * pHead = (FCAnimationDataHead *)m_pDataBuffer;
	return (const char *)pHead->info.rootId;
}

// buffers
unsigned char * FCAnimationData::getTexturesBuffer(unsigned long * pBufferLength) const
{
	if (!checkDataFormat())
	{
		return NULL;
	}
	FCAnimationDataHead * pHead = (FCAnimationDataHead *)m_pDataBuffer;
	*pBufferLength = pHead->info.texturesBuffer.length;
	return &m_pDataBuffer[pHead->info.texturesBuffer.offset];
}

unsigned char * FCAnimationData::getLibraryBuffer(unsigned long * pBufferLength) const
{
	if (!checkDataFormat())
	{
		return NULL;
	}
	FCAnimationDataHead * pHead = (FCAnimationDataHead *)m_pDataBuffer;
	*pBufferLength = pHead->info.libraryBuffer.length;
	return &m_pDataBuffer[pHead->info.libraryBuffer.offset];
}

unsigned char * FCAnimationData::getFramesBuffer(unsigned long * pBufferLength) const
{
	if (!checkDataFormat())
	{
		return NULL;
	}
	FCAnimationDataHead * pHead = (FCAnimationDataHead *)m_pDataBuffer;
	*pBufferLength = pHead->info.framesBuffer.length;
	return &m_pDataBuffer[pHead->info.framesBuffer.offset];
}

unsigned char * FCAnimationData::getNamesBuffer(unsigned long * pBufferLength) const
{
	if (!checkDataFormat())
	{
		return NULL;
	}
	FCAnimationDataHead * pHead = (FCAnimationDataHead *)m_pDataBuffer;
	*pBufferLength = pHead->info.namesBuffer.length;
	return &m_pDataBuffer[pHead->info.namesBuffer.offset];
}

// library buffers
unsigned char * FCAnimationData::getLibrarySymbolsBuffer(unsigned long * pBufferLength) const
{
	if (!checkDataFormat())
	{
		return NULL;
	}
	FCAnimationDataHead * pHead = (FCAnimationDataHead *)m_pDataBuffer;
	FCLibraryBufferHead * pLibHead = (FCLibraryBufferHead *)&m_pDataBuffer[pHead->info.libraryBuffer.offset];
	*pBufferLength = pLibHead->symbolsBuffer.length;
	return &m_pDataBuffer[pHead->info.libraryBuffer.offset + pLibHead->symbolsBuffer.offset];
}

unsigned char * FCAnimationData::getLibraryTimelineLayersBuffer(unsigned long * pBufferLength) const
{
	if (!checkDataFormat())
	{
		return NULL;
	}
	FCAnimationDataHead * pHead = (FCAnimationDataHead *)m_pDataBuffer;
	FCLibraryBufferHead * pLibHead = (FCLibraryBufferHead *)&m_pDataBuffer[pHead->info.libraryBuffer.offset];
	*pBufferLength = pLibHead->timelineLayersBuffer.length;
	return &m_pDataBuffer[pHead->info.libraryBuffer.offset + pLibHead->timelineLayersBuffer.offset];
}

unsigned char * FCAnimationData::getLibraryTimelineFramesBuffer(unsigned long * pBufferLength) const
{
	if (!checkDataFormat())
	{
		return NULL;
	}
	FCAnimationDataHead * pHead = (FCAnimationDataHead *)m_pDataBuffer;
	FCLibraryBufferHead * pLibHead = (FCLibraryBufferHead *)&m_pDataBuffer[pHead->info.libraryBuffer.offset];
	*pBufferLength = pLibHead->timelineFramesBuffer.length;
	return &m_pDataBuffer[pHead->info.libraryBuffer.offset + pLibHead->timelineFramesBuffer.offset];
}

unsigned char * FCAnimationData::getLibraryTimelineSpritesBuffer(unsigned long * pBufferLength) const
{
	if (!checkDataFormat())
	{
		return NULL;
	}
	FCAnimationDataHead * pHead = (FCAnimationDataHead *)m_pDataBuffer;
	FCLibraryBufferHead * pLibHead = (FCLibraryBufferHead *)&m_pDataBuffer[pHead->info.libraryBuffer.offset];
	*pBufferLength = pLibHead->timelineSpritesBuffer.length;
	return &m_pDataBuffer[pHead->info.libraryBuffer.offset + pLibHead->timelineSpritesBuffer.offset];
}

// library arrays
FCSymbolInfo * FCAnimationData::getSymbol(const unsigned int iLocation) const
{
	unsigned long iBufferLength = 0;
	FCSymbolInfo * pArray = (FCSymbolInfo *)getLibrarySymbolsBuffer(&iBufferLength);
	if (!pArray || iBufferLength < sizeof(FCSymbolInfo) * (iLocation + 1))
	{
		FCLog("failed to get Symbol, location: %d, element size: %ld, buffer length: %ld", iLocation, sizeof(FCSymbolInfo), iBufferLength);
		return NULL;
	}
	return &pArray[iLocation];
}

FCTimelineLayerInfo * FCAnimationData::getTimelineLayer(const unsigned int iLocation) const
{
	unsigned long iBufferLength = 0;
	FCTimelineLayerInfo * pArray = (FCTimelineLayerInfo *)getLibraryTimelineLayersBuffer(&iBufferLength);
	if (!pArray || iBufferLength < sizeof(FCTimelineLayerInfo) * (iLocation + 1))
	{
		FCLog("failed to get TimelineLayer, location: %d, element size: %ld, buffer length: %ld", iLocation, sizeof(FCTimelineLayerInfo), iBufferLength);
		return NULL;
	}
	return &pArray[iLocation];
}

FCTimelineFrameInfo * FCAnimationData::getTimelineFrame(const unsigned int iLocation) const
{
	unsigned long iBufferLength = 0;
	FCTimelineFrameInfo * pArray = (FCTimelineFrameInfo *)getLibraryTimelineFramesBuffer(&iBufferLength);
	if (!pArray || iBufferLength < sizeof(FCTimelineFrameInfo) * (iLocation + 1))
	{
		FCLog("failed to get TimelineFrame, location: %d, element size: %ld, buffer length: %ld", iLocation, sizeof(FCTimelineFrameInfo), iBufferLength);
		return NULL;
	}
	return &pArray[iLocation];
}

FCTimelineSpriteInfo * FCAnimationData::getTimelineSprite(const unsigned int iLocation) const
{
	unsigned long iBufferLength = 0;
	FCTimelineSpriteInfo * pArray = (FCTimelineSpriteInfo *)getLibraryTimelineSpritesBuffer(&iBufferLength);
	if (!pArray || iBufferLength < sizeof(FCTimelineSpriteInfo) * (iLocation + 1))
	{
		FCLog("failed to get TimelineSprite, location: %d, element size: %ld, buffer length: %ld", iLocation, sizeof(FCTimelineSpriteInfo), iBufferLength);
		return NULL;
	}
	return &pArray[iLocation];
}

NS_FC_END
