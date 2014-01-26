LOCAL_PATH := $(call my-dir)


include $(CLEAR_VARS)
LOCAL_MODULE := libglob
LOCAL_SRC_FILES := libglob.a
include $(PREBUILT_STATIC_LIBRARY)


include $(CLEAR_VARS)
LOCAL_MODULE := libxml2
LOCAL_SRC_FILES := libxml2.a
include $(PREBUILT_STATIC_LIBRARY)


include $(CLEAR_VARS)
LOCAL_MODULE := libpcre
LOCAL_SRC_FILES := libpcre.a
include $(PREBUILT_STATIC_LIBRARY)


include $(CLEAR_VARS)
LOCAL_MODULE := libraptor2
LOCAL_SRC_FILES := libraptor2.a
include $(PREBUILT_STATIC_LIBRARY)


include $(CLEAR_VARS)
LOCAL_MODULE := librasqal
LOCAL_SRC_FILES := librasqal.a
include $(PREBUILT_STATIC_LIBRARY)


include $(CLEAR_VARS)
LOCAL_MODULE := librdf
LOCAL_SRC_FILES := librdf.a
include $(PREBUILT_STATIC_LIBRARY)


include $(CLEAR_VARS)
LOCAL_MODULE    := raptortest
LOCAL_SRC_FILES := raptortest.c rasqal_native.c
LOCAL_CFLAGS    := -I.
LOCAL_STATIC_LIBRARIES := librdf librasqal libraptor2 libpcre libxml2 libglob
include $(BUILD_SHARED_LIBRARY)
