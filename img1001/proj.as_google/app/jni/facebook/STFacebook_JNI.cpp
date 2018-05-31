/*
 * 	STFacebook_JNI.cpp
 *
 *  Created on: 2013-7-19
 *  Author: liujian
 */

#include "STFacebookHelper_JNI_Helper.h"
#include "../../Classes/modules/STFacebook.h"

void STFacebook::NativePostStatusUpdate(int tag, std::string text)
{
	LOGI("NativePostStatusUpdate start!");

	STFacebookHelper* helper = STFacebookHelper::getInstance();
	if (!helper->MethodNativePostStatusUpdate)
	{
		LOGE("FROM STFacebook, Can't get method NativePostStatusUpdate()");
		return;
	}
	
	JNIEnv* lEnv = ST_JNI_Helper::getJNIEnv();
	jstring jniText = lEnv->NewStringUTF(text.c_str());
	lEnv->CallVoidMethod(helper->STFacebookBridgeJava, helper->MethodNativePostStatusUpdate, tag, jniText);
}

void STFacebook::NativePostPhotoFromAsset(int tag, std::string filename)
{
	LOGI("NativePostPhotoFromAsset start!");

	STFacebookHelper* helper = STFacebookHelper::getInstance();
	if (!helper->MethodNativePostPhotoFromAsset)
	{
		LOGE("FROM STFacebook, Can't get method NativePostPhotoFromAsset()");
		return;
	}
	
	JNIEnv* lEnv = ST_JNI_Helper::getJNIEnv();
	jstring jniFileName = lEnv->NewStringUTF(filename.c_str());
	lEnv->CallVoidMethod(helper->STFacebookBridgeJava, helper->MethodNativePostPhotoFromAsset, tag, jniFileName);
}

void STFacebook::NativePostPhotoFromDrawable(int tag, int drawableId)
{
	LOGI("NativePostPhotoFromDrawable start!");

	STFacebookHelper* helper = STFacebookHelper::getInstance();
	if (!helper->MethodNativePostPhotoFromDrawable)
	{
		LOGE("FROM STFacebook, Can't get method NativePostPhotoFromDrawable()");
		return;
	}

	JNIEnv* lEnv = ST_JNI_Helper::getJNIEnv();
	lEnv->CallVoidMethod(helper->STFacebookBridgeJava, helper->MethodNativePostPhotoFromDrawable, tag, drawableId);
}

void STFacebook::NativePostPhotoFromFile(int tag, std::string filename)
{
	LOGI("NativePostPhotoFromFile start!");

	STFacebookHelper* helper = STFacebookHelper::getInstance();
	if (!helper->MethodNativePostPhotoFromFile)
	{
		LOGE("FROM STFacebook, Can't get method NativePostPhotoFromFile()");
		return;
	}
	
	JNIEnv* lEnv = ST_JNI_Helper::getJNIEnv();
	jstring jniFileName = lEnv->NewStringUTF(filename.c_str());
	lEnv->CallVoidMethod(helper->STFacebookBridgeJava, helper->MethodNativePostPhotoFromFile, tag, jniFileName);
}

void STFacebook::NativePostRichContentAsset(int tag, std::string message, std::string link, std::string assetFileName)
{
	LOGI("NativePostRichContentAsset start!");

	STFacebookHelper* helper = STFacebookHelper::getInstance();
	if (!helper->MethodNativePostRichContentAsset)
	{
		LOGE("FROM STFacebook, Can't get method NativePostRichContentAsset()");
		return;
	}
	
	JNIEnv* lEnv = ST_JNI_Helper::getJNIEnv();
	jstring jniMessage = lEnv->NewStringUTF(message.c_str());
	jstring jniLink = lEnv->NewStringUTF(link.c_str());
	jstring jniAssetFileName = lEnv->NewStringUTF(assetFileName.c_str());
	lEnv->CallVoidMethod(helper->STFacebookBridgeJava, helper->MethodNativePostRichContentAsset, tag, jniMessage, jniLink, jniAssetFileName);
}

void STFacebook::NativePostRichContentDrawable(int tag, std::string message, std::string link, int imgId)
{
	LOGI("NativePostRichContentDrawable start!");

	STFacebookHelper* helper = STFacebookHelper::getInstance();
	if (!helper->MethodNativePostRichContentDrawable)
	{
		LOGE("FROM STFacebook, Can't get method NativePostRichContentDrawable()");
		return;
	}
	
	JNIEnv* lEnv = ST_JNI_Helper::getJNIEnv();
	jstring jniMessage = lEnv->NewStringUTF(message.c_str());
	jstring jniLink = lEnv->NewStringUTF(link.c_str());
	lEnv->CallVoidMethod(helper->STFacebookBridgeJava, helper->MethodNativePostRichContentDrawable, tag, jniMessage, jniLink, imgId);
}

void STFacebook::NativePostRichContentFile(int tag, std::string message, std::string link, std::string filename)
{
	LOGI("NativePostRichContentFile start!");

	STFacebookHelper* helper = STFacebookHelper::getInstance();
	if (!helper->MethodNativePostRichContentFile)
	{
		LOGE("FROM STFacebook, Can't get method NativePostRichContentFile()");
		return;
	}
	
	JNIEnv* lEnv = ST_JNI_Helper::getJNIEnv();
	jstring jniMessage = lEnv->NewStringUTF(message.c_str());
	jstring jniLink = lEnv->NewStringUTF(link.c_str());
	jstring jniFileName = lEnv->NewStringUTF(filename.c_str());
	lEnv->CallVoidMethod(helper->STFacebookBridgeJava, helper->MethodNativePostRichContentFile, tag, jniMessage, jniLink, jniFileName);
}
