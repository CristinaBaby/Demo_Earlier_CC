/*
 *  com_common_android_facebook_FacebookBridge.cpp
 *
 *  Created on: 2013-7-19
 *  Author: liujian
 */
 
#include "com_common_android_facebook_FacebookBridge.h"
#include "STFacebookHelper_JNI_Helper.h"
#include "../../Classes/modules/STFacebook.h"

JNIEXPORT void JNICALL Java_com_common_android_facebook_FacebookBridge_nativeInit
(JNIEnv *pEnv, jobject javaObj)
{
	LOGI("Java_common_jni_facebook_FacebookBridge_nativeInit");
	STFacebookHelper::getInstance()->init(pEnv, javaObj);
}

JNIEXPORT void JNICALL Java_com_common_android_facebook_FacebookBridge_nativeFinalize
(JNIEnv *pEnv, jobject javaObj)
{
	STFacebookHelper::getInstance()->finalize();
}

JNIEXPORT void JNICALL Java_com_common_android_facebook_FacebookBridge_nativeOnPostStatus
  (JNIEnv *pEnv, jobject javaObj, jint tag, jboolean bresult)
{
	bool resultC = (bresult == JNI_TRUE ? true: false);
	STFacebook::getInstance()->onPostStatus(tag, resultC);
}

JNIEXPORT void JNICALL Java_com_common_android_facebook_FacebookBridge_nativeOnPostPhoto
  (JNIEnv *pEnv, jobject javaObj, jint tag, jboolean bresult)
{
	bool resultC = (bresult == JNI_TRUE ? true: false);
	STFacebook::getInstance()->onPostPhoto(tag, resultC);
}

JNIEXPORT void JNICALL Java_com_common_android_facebook_FacebookBridge_nativeOnPostRichContent
  (JNIEnv *pEnv, jobject javaObj, jint tag, jboolean bresult)
{
	bool resultC = (bresult == JNI_TRUE ? true: false);
	STFacebook::getInstance()->onPostRichContent(tag, resultC);
}

JNIEXPORT void JNICALL Java_com_common_android_facebook_FacebookBridge_nativeOnPostTimeOut
  (JNIEnv *pEnv, jobject javaObj, jint tag)
{
	STFacebook::getInstance()->onPostTimeOut(tag);	
}
