//
//  FCAnimationData.h
//  FlashAnimation-X
//
//  Created by Moky on 12-12-12.
//  Copyright 2012 Slanissue Inc. All rights reserved.
//

#ifndef FlashAnimation_X_FCData_h
#define FlashAnimation_X_FCData_h

#include "../fcMacros.h"
#include "../timeline/FCTimelineSprite.h"
#include "../timeline/FCTimelineFrame.h"
#include "../timeline/FCTimelineLayer.h"
#include "../library/FCLibrarySymbol.h"

NS_FC_BEGIN

#pragma pack(1)

typedef struct {
	unsigned int   location; // start position in array buffer
	unsigned short count;    // count of the array (0 - 65535)
} FCArrayRange;

typedef struct {
	unsigned long  offset; // offset in memory buffer
	unsigned long  length; // length of the sub buffer
} FCBufferRange;

typedef unsigned short FCSymbolID; // 0 - 65535

typedef struct {
	float a;
	float b;
	float c;
	float d;
	float tx;
	float ty;
} FCMatrixInfo;

typedef struct {
	FCSymbolID id;
	FCTimelineSpriteType type;
	unsigned short firstFrame;
	FCTimelineSpriteLoopType loop;
	FCMatrixInfo matrix;
	float x;
	float y;
	float anchorX;
	float anchorY;
	float scaleX;
	float scaleY;
	float skewX;
	float skewY;
	float rotation;
	float alpha;
} FCTimelineSpriteInfo;

typedef struct {
	FCTimelineFrameTweenType type;
	unsigned short startFrame;
	unsigned short duration;
	FCArrayRange sprites; // sub array in timelineSpritesBuffer
} FCTimelineFrameInfo;

typedef struct {
//	char name[32]; // no use
	FCTimelineLayerType type;
	FCArrayRange frames; // sub array in timelineFramesBuffer (only key frame, not total frame!)
} FCTimelineLayerInfo;

typedef struct {
	FCSymbolID id;
	float width;
	float height;
	FCLibrarySymbolType type;
	union {
		unsigned short fileId; // index for filename (array saved in framesBuffer)
		FCArrayRange layers; // sub array in timelineLayersBuffer
	};
} FCSymbolInfo;

typedef struct {
	FCBufferRange symbolsBuffer;
	FCBufferRange timelineLayersBuffer;
	FCBufferRange timelineFramesBuffer;
	FCBufferRange timelineSpritesBuffer;
} FCLibraryBufferHead;

// FlashToCocos2d Data File Protocol
typedef struct {
	// frame rate
	unsigned char frameRate; // 0 - 255
	// root id
	unsigned char rootId[63];
	// buffers
	FCBufferRange libraryBuffer; // symbols library (symbols + layers + frames + sprites)
	FCBufferRange framesBuffer; // sprite frames (file id-name array)
	FCBufferRange namesBuffer; // symbol names (symbol name-offset map dictionary)
	FCBufferRange texturesBuffer; // textures array
} FCAnimationDataInfo;

typedef struct {
	// protocol
	unsigned char format[4]; // "F2C"
	unsigned char headVersion;
	unsigned char version;
	unsigned char reserved[10]; // reserved & for bytes alignment
	unsigned long fileLength;
	// descriptions
	unsigned char description[64];
	unsigned char copyright[32];
	unsigned char author[32];
	// index info
	FCAnimationDataInfo info;
} FCAnimationDataHead;

#pragma pack()

class FCAnimationData
{
protected:
	unsigned char * m_pDataBuffer;
	unsigned long m_iBufferLength;
	
	bool m_bIsDataValid;
	
public:
	FCAnimationData(void);
	virtual ~FCAnimationData(void);
	virtual bool init(void);
	
	virtual bool init(const unsigned long iBufferLength); // create an initialized buffer
	virtual bool init(const std::string & sDataFile); // load data from a 'f2c' file
	
	bool checkDataFormat(void) const;
	
	bool saveToFile(const std::string & sDataFile) const; // safe buffer into a 'f2c' file
	
#pragma mark Interfaces
	
public: // head info
	unsigned int getFrameRate(void) const;
	const char * getRootId(void) const;
protected: // buffers
	unsigned char * getTexturesBuffer(unsigned long * pBufferLength) const;
	unsigned char * getLibraryBuffer(unsigned long * pBufferLength) const;
	unsigned char * getFramesBuffer(unsigned long * pBufferLength) const;
	unsigned char * getNamesBuffer(unsigned long * pBufferLength) const;
protected: // library buffers
	unsigned char * getLibrarySymbolsBuffer(unsigned long * pBufferLength) const;
	unsigned char * getLibraryTimelineLayersBuffer(unsigned long * pBufferLength) const;
	unsigned char * getLibraryTimelineFramesBuffer(unsigned long * pBufferLength) const;
	unsigned char * getLibraryTimelineSpritesBuffer(unsigned long * pBufferLength) const;
public: // library arrays
	FCSymbolInfo * getSymbol(const unsigned int iLocation) const;
	FCTimelineLayerInfo * getTimelineLayer(const unsigned int iLocation) const;
	FCTimelineFrameInfo * getTimelineFrame(const unsigned int iLocation) const;
	FCTimelineSpriteInfo * getTimelineSprite(const unsigned int iLocation) const;
};

NS_FC_END

#endif
