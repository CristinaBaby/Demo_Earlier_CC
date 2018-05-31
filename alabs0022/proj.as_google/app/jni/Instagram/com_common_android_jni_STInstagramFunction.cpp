/*
 * com_common_android_jni_STSystemFunction.cpp
 *
 *  Created on: 2013-9-2
 *      Author: Steven.Xc.Tian
 */
#include "com_common_android_jni_STInstagramFunction.h"
#include "ST_InstagramFunction_Android.h"

JNIEXPORT void JNICALL Java_com_common_android_jni_STInstagramFunction_nativeInit
(JNIEnv * pEnv, jobject thisObj)
{
	InstagramFunctionManager::getInstance()->init(pEnv, thisObj);
}

JNIEXPORT void JNICALL Java_com_common_android_jni_STInstagramFunction_nativeDestroy
(JNIEnv * pEnv, jobject jobjSf)
{
	InstagramFunctionManager::getInstance()->destroy();
}

