/*
 * ST_SystemFunction_Android.cpp
 *
 *  Created on: 2013-9-2
 *      Author: Steven.Xc.Tian
 */
#include "ST_InstagramFunction_Android.h"

static InstagramFunctionManager* sp_InstagramFunctionManager = 0;

InstagramFunctionManager* InstagramFunctionManager::getInstance()
{
	if (!sp_InstagramFunctionManager)
	{
		sp_InstagramFunctionManager = new InstagramFunctionManager();
	}
    
	return sp_InstagramFunctionManager;
}

void InstagramFunctionManager::destroy()
{
	if (sp_InstagramFunctionManager)
	{
		delete sp_InstagramFunctionManager;
        
		sp_InstagramFunctionManager = 0;
	}
}

bool InstagramFunctionManager::init(JNIEnv * pEnv, jobject pJava)
{
	// get class
	ClassSF = pEnv->GetObjectClass(pJava);
	if (!ClassSF)
	{
		LOGE("get STSystemFunction class Failed!");
		return false;
	}
	ClassSF = (jclass) ST_JNI_Helper::makeGlobalRef(pEnv, ClassSF);
    

    MethodSendImageToInstagram=pEnv->GetMethodID(ClassSF, "sendImageToInstagram", "(Ljava/lang/String;)V");
    if(!MethodSendImageToInstagram)
    {
        LOGE("get senderImageToInstagram id Failed!");
        return false;
    }
    
    // Caches objects.
    stSFJava = ST_JNI_Helper::makeGlobalRef(pEnv, pJava);
    if (!stSFJava)
    {
        LOGE("Cache stSFJava Failed!");
        return false;
    }
    
	return true;
}


void InstagramFunctionManager::sendImageToInstagram(const char* path)
{
    if (!stSFJava)
    {
        LOGE("InstagramFunctionManager::senderImageToInstagram failed!");
        return;
    }
    
    JNIEnv* lEnv = ST_JNI_Helper::getJNIEnv();
    
    // Convert the C++ string to C-string, then to JNI String (jstring) for method args
    jstring msg = lEnv->NewStringUTF(path);
    
    lEnv->CallVoidMethod(stSFJava, MethodSendImageToInstagram, msg);
    
    lEnv->DeleteLocalRef(msg);
}

InstagramFunctionManager::~InstagramFunctionManager()
{
	ST_JNI_Helper::deleteGlobalRef(ClassSF);
	ST_JNI_Helper::deleteGlobalRef(stSFJava);
}

