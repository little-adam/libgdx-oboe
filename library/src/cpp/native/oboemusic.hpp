#pragma once
#include "jni.h"

#define OBOEMUSIC_METHOD(result, method) \
    JNIEXPORT result JNICALL \
    Java_barsoosayque_libgdxoboe_OboeMusic ## _ ## method

extern "C" {

OBOEMUSIC_METHOD(bool, isPlaying) (JNIEnv*, jobject);
OBOEMUSIC_METHOD(bool, isLooping) (JNIEnv*, jobject);
OBOEMUSIC_METHOD(void, pause) (JNIEnv*, jobject);
OBOEMUSIC_METHOD(void, setPan) (JNIEnv*, jobject, jfloat, jfloat);
OBOEMUSIC_METHOD(jfloat, getPosition) (JNIEnv*, jobject);
OBOEMUSIC_METHOD(void, setLooping) (JNIEnv*, jobject, jfloat);
OBOEMUSIC_METHOD(jfloat, getVolume) (JNIEnv*, jobject);
OBOEMUSIC_METHOD(void, play) (JNIEnv*, jobject);
OBOEMUSIC_METHOD(void, stop) (JNIEnv*, jobject);
OBOEMUSIC_METHOD(void, setVolume) (JNIEnv*, jobject, jfloat);
OBOEMUSIC_METHOD(void, setPosition) (JNIEnv*, jobject, jfloat);
OBOEMUSIC_METHOD(void, dispose) (JNIEnv*, jobject);

}