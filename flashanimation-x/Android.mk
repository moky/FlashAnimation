LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := flashanimationx_static

LOCAL_MODULE_FILENAME := libflashanimationx

LOCAL_SRC_FILES := flashanimation.cpp \
					FCAnimation.cpp \
					\
					library/FCLibrarySymbol.cpp \
					library/FCLibrarySymbolBuilder.cpp \
					library/FCLibrary.cpp \
					\
					sprite/FCSprite.cpp \
					sprite/FCShapeSprite.cpp \
					sprite/FCInstanceSprite.cpp \
					sprite/FCInstanceSpriteInternal.cpp \
					\
					timeline/FCTimelineFrame.cpp \
					timeline/FCTimelineSprite.cpp \
					timeline/FCTimelineLayer.cpp \
					timeline/FCTimeline.cpp \
					\
					data/FCAnimationData.cpp \
					data/FCAnimationDataReader.cpp \
					data/FCAnimationDataTransformer.cpp 

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH) 

LOCAL_C_INCLUDES := $(LOCAL_PATH) 

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static coconut2dx_static

include $(BUILD_STATIC_LIBRARY)
