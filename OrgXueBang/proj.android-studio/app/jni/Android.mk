LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/cocos)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/cocos/audio/include)

LOCAL_MODULE := MyGame_shared
LOCAL_MODULE_FILENAME := libMyGame

define walk  
    $(wildcard $(1)) $(foreach e, $(wildcard $(1)/*), $(call walk, $(e)))  
endef  
ANDROIDFILES = $(call walk, $(LOCAL_PATH)/../../../android_common)
CLASSFILES = $(call walk, $(LOCAL_PATH)/../../../Classes)

FILE_LIST := hellocpp/main.cpp
FILE_LIST += $(filter %.cpp, $(ANDROIDFILES))
FILE_LIST += $(filter %.c, $(ANDROIDFILES))
FILE_LIST += $(filter %.cpp, $(CLASSFILES))
FILE_LIST += $(filter %.c, $(CLASSFILES))


LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)

LOCAL_CFLAGS = -DFIXED_POINT -DUSE_KISS_FFT -DEXPORT="" -UHAVE_CONFIG_H  
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../Classes
LOCAL_C_INCLUDES += $(shell ls -FR $(LOCAL_C_INCLUDES) | grep $(LOCAL_PATH)/$ )
LOCAL_C_INCLUDES := $(LOCAL_C_INCLUDES:$(LOCAL_PATH)/%:=$(LOCAL_PATH)/%)
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../cocos2d/external/curl/include/android
LOCAL_C_INCLUDES += $(LOCAL_PATH)/include


# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cocos2dx_static
LOCAL_STATIC_LIBRARIES += cocos_curl_static

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)
$(call import-module,audio/android)
# $(call import-module,Box2D)
# $(call import-module,editor-support/cocosbuilder)
$(call import-module,editor-support/spine)
$(call import-module,editor-support/cocostudio)
$(call import-module,network)
$(call import-module,extensions)
$(call import-module,curl/prebuilt/Android)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
