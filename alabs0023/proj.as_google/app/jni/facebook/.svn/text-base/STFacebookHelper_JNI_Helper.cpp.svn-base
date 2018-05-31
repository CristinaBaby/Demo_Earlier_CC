/*
 *  STFacebookHelper_JNI_Helper.cpp
 *
 *  Created on: 2013-7-8
 *      Author: liuajian
 */
 
#include "STFacebookHelper_JNI_Helper.h"

STFacebookHelper::STFacebookHelper()
	: STFacebookBridgeJava(0)
	, ClassSTFacebookBridge(0)
	, MethodNativePostStatusUpdate(0)
	, MethodNativePostPhotoFromAsset(0)
	, MethodNativePostPhotoFromDrawable(0)
	, MethodNativePostPhotoFromFile(0)
	, MethodNativePostRichContentAsset(0)
	, MethodNativePostRichContentDrawable(0)
	, MethodNativePostRichContentFile(0)
{

}
	
void STFacebookHelper::init(JNIEnv* pEnv, jobject pFacebookBridgeJava)
{
	LOGI("STFacebookHelper::init() start!");

	// get class
	ClassSTFacebookBridge = pEnv->GetObjectClass(pFacebookBridgeJava);
	if (!ClassSTFacebookBridge)
	{
		LOGE("get FacebookBridge class Failed!");
		return;
	}
	ClassSTFacebookBridge = (jclass)ST_JNI_Helper::makeGlobalRef(pEnv, ClassSTFacebookBridge);

	// get method id --- void NativePostStatusUpdate(final String text)
	MethodNativePostStatusUpdate = pEnv->GetMethodID(ClassSTFacebookBridge, "NativePostStatusUpdate", "(ILjava/lang/String;)V");
	if (!MethodNativePostStatusUpdate)
	{
		LOGE("get MethodNativePostStatusUpdate id Failed!");
		return;
	}
	// get method id --- void NativePostPhotoFromAsset(final String text)
	MethodNativePostPhotoFromAsset = pEnv->GetMethodID(ClassSTFacebookBridge, "NativePostPhotoFromAsset", "(ILjava/lang/String;)V");
	if (!MethodNativePostPhotoFromAsset)
	{
		LOGE("get MethodNativePostPhotoFromAsset id Failed!");
		return;
	}
	// get method id --- void NativePostPhotoFromDrawable(final int imgId)
	MethodNativePostPhotoFromDrawable = pEnv->GetMethodID(ClassSTFacebookBridge, "NativePostPhotoFromDrawable", "(II)V");
	if (!MethodNativePostPhotoFromDrawable)
	{
		LOGE("get MethodNativePostPhotoFromDrawable id Failed!");
		return;
	}
	// get method id --- void NativePostPhotoFromFile(final String fileName)
	MethodNativePostPhotoFromFile = pEnv->GetMethodID(ClassSTFacebookBridge, "NativePostPhotoFromFile", "(ILjava/lang/String;)V");
	if (!MethodNativePostPhotoFromFile)
	{
		LOGE("get MethodNativePostPhotoFromFile id Failed!");
		return;
	}
	// get method id --- void NativePostRichContentDrawable(final String  message, final String  link, final int imgId)
	MethodNativePostRichContentDrawable = pEnv->GetMethodID(ClassSTFacebookBridge, "NativePostRichContentDrawable", "(ILjava/lang/String;Ljava/lang/String;I)V");
	if (!MethodNativePostRichContentDrawable)
	{
		LOGE("get MethodNativePostRichContentDrawable id Failed!");
		return;
	}
	// get method id --- void NativePostRichContentFile(final String  message, final String  link, final String fileName)
	MethodNativePostRichContentFile = pEnv->GetMethodID(ClassSTFacebookBridge, "NativePostRichContentFile", "(ILjava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
	if (!MethodNativePostRichContentFile)
	{
		LOGE("get MethodNativePostRichContentFile id Failed!");
		return;
	}
	// get method id --- void NativePostRichContentAsset(final String  message, final String  link, final String assetFileName)
	MethodNativePostRichContentAsset = pEnv->GetMethodID(ClassSTFacebookBridge, "NativePostRichContentAsset", "(ILjava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
	if (!MethodNativePostRichContentAsset)
	{
		LOGE("get MethodNativePostRichContentAsset id Failed!");
		return;
	}

	// Caches objects.
	STFacebookBridgeJava = ST_JNI_Helper::makeGlobalRef(pEnv, pFacebookBridgeJava);
	if (!STFacebookBridgeJava)
	{
		LOGE("Cache STFacebookBridgeJava Failed!");
		return;
	}
}

static STFacebookHelper* sp_STFacebookHelper = 0;
STFacebookHelper* STFacebookHelper::getInstance()
{
	if (!sp_STFacebookHelper)
	{
		sp_STFacebookHelper = new STFacebookHelper();
	}

	return sp_STFacebookHelper;
}

void STFacebookHelper::finalize()
{
	if (sp_STFacebookHelper)
	{
		delete sp_STFacebookHelper;
		sp_STFacebookHelper = 0;
	}
}


STFacebookHelper::~STFacebookHelper()
{
	ST_JNI_Helper::deleteGlobalRef(STFacebookBridgeJava);
	ST_JNI_Helper::deleteGlobalRef(ClassSTFacebookBridge);
}
