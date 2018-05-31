/*
 * STSystemFunction.cpp
 *
 *  Created on: 2013-9-2
 *      Author: Steven.Xc.Tian
 */
#include "../../Classes/modules/STSystemFunction.h"
#include "ST_SystemFunction_Android.h"
#include "platform/android/jni/JniHelper.h"

using namespace cocos2d;

STSystemFunction::STSystemFunction()
{
}
STSystemFunction::~STSystemFunction()
{
}

void STSystemFunction::sendEmail(const char* subject, const char* body)
{
	SystemFunctionManager::getInstance()->sendEmail(subject, body);
}

void STSystemFunction::popAlertDialog(const char* message)
{
	SystemFunctionManager::getInstance()->popAlertDialog(message);
}

void STSystemFunction::makeToast(const char* message)
{
	SystemFunctionManager::getInstance()->makeToast(message);
}

void STSystemFunction::showMoreGame()
{
	SystemFunctionManager::getInstance()->showMoreGame();
}

void STSystemFunction::rating()
{
//	SystemFunctionManager::getInstance()->rating();
    if (!SystemFunctionManager::getInstance()->isRate()) {
        SystemFunctionManager::getInstance()->rating();
    }
}

void STSystemFunction::go2MarketDetail(std::string packagename)
{
	SystemFunctionManager::getInstance()->go2MarketDetail(packagename);
}

void STSystemFunction::contactUs()
{
	SystemFunctionManager::getInstance()->contactUs();
}

void STSystemFunction::sendEmailAndAssetPic(std::string subject, std::string message, std::string assetName)
{
	SystemFunctionManager::getInstance()->sendEmailAndAssetPic(subject, message, assetName);
}

void STSystemFunction::sendEmailAndFilePic(std::string subject, std::string message, std::string fileName)
{
	SystemFunctionManager::getInstance()->sendEmailAndFilePic(subject, message, fileName);

}

void STSystemFunction::sendPictureToInstagram(std::string fileName)
{
   
}

void STSystemFunction::sendPictureToMore(std::string fileName)
{

}

std::string STSystemFunction::getSdCardPath()
{
	return SystemFunctionManager::getInstance()->getSdCardPath();
}

bool STSystemFunction::checkNetworkAvailable()
{
	return SystemFunctionManager::getInstance()->checkNetworkAvailable();

}

bool STSystemFunction::isTabletAvailable()
{
	return SystemFunctionManager::getInstance()->isTabletAvailable();

}
float STSystemFunction::densityScale()
{
	return SystemFunctionManager::getInstance()->densityScale();
}
void STSystemFunction::rating(const char* appleID, const char* message)
{
	// just for iOS platform.
}

void STSystemFunction::go2PrivacyPage()
{
	// NOT implement yet.
    openURL("https://www.imagineappsltd.com/privacy/");
}

void STSystemFunction::openURL(std::string url)
{
	// NOT implement yet.
    JniMethodInfo methodInfo;
    
    bool isHave = JniHelper::getStaticMethodInfo(methodInfo,
                                                 "com.imagineappsltd.android_colorfree/AppActivity", "openURL", "(Ljava/lang/String;)V");
    
    if (isHave)
    {
        jstring jurl = methodInfo.env->NewStringUTF(url.c_str());
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, jurl);
    }
}

void STSystemFunction::endSession()
{
	SystemFunctionManager::getInstance()->endSession();
}

void STSystemFunction::saveLocalImage(void* image)
{
   
    
}

void STSystemFunction::deleteDocumentFile(std::string filename)
{

}

void STSystemFunction::refreshDCIM(std::string filename)
{
    SystemFunctionManager::getInstance()->refreshDCIM(filename);
}
