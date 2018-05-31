//
//  STSystemFunction.cpp
//  MysteryGuardians
//
//  Created by Steven.Tian on 13-7-9.
//
//

#include "STSystemFunction.h"
#include "SystemFunctionIOSWrapper.h"

STSystemFunction::STSystemFunction()
{
}

STSystemFunction::~STSystemFunction()
{
}

void STSystemFunction::sendEmail(const char* subject, const char* body)
{
    [[SystemFunctionManager sharedManager]sendEmail : [NSString stringWithCString:subject encoding:NSUTF8StringEncoding]
                                             Content:[NSString stringWithCString:body encoding:NSUTF8StringEncoding]];
}

void STSystemFunction::popAlertDialog(const char *message)
{
    [[SystemFunctionManager sharedManager]popAlertDialog:[NSString stringWithCString:message encoding:NSUTF8StringEncoding]];
}

void STSystemFunction::showMoreGame()
{
    [[SystemFunctionManager sharedManager]showMorePage];
}

void STSystemFunction::go2MarketDetail(std::string packagename)
{
    [[SystemFunctionManager sharedManager]go2Market:[NSString stringWithCString:packagename.c_str() encoding:NSUTF8StringEncoding]];
}

void STSystemFunction::makeToast(const char* message)
{
}

void STSystemFunction::rating(const char* appleID, const char* message)
{
    [[SystemFunctionManager sharedManager]rateUs:[NSString stringWithCString:appleID encoding:NSUTF8StringEncoding]
                                         Message:[NSString stringWithCString:message encoding:NSUTF8StringEncoding]];
}

void STSystemFunction::rating()
{
    // only for Android
}

void STSystemFunction::contactUs()
{
}

void STSystemFunction::sendEmailAndAssetPic(std::string subject, std::string message, std::string assetName)
{}

void STSystemFunction::sendEmailAndFilePic(std::string subject, std::string message, std::string fileName)
{
    NSString* lSubStr=[NSString stringWithUTF8String:subject.c_str()];
    NSString* lMessageStr=[NSString stringWithUTF8String:message.c_str()];
    NSString* lPath=[NSString stringWithUTF8String:fileName.c_str()];
   [[SystemFunctionManager sharedManager] sendEmailAndFilePic:lSubStr Message:lMessageStr ImagePath:lPath];
}

void STSystemFunction::sendPictureToInstagram(std::string fileName)
{
     NSString* lImageName=[NSString stringWithUTF8String:fileName.c_str()];
    [[SystemFunctionManager sharedManager] sendPictureToInstagram:lImageName];
}

std::string STSystemFunction::getSdCardPath()
{
    return NULL;
}

bool STSystemFunction::checkNetworkAvailable()
{
    return true;
}

bool STSystemFunction::isTabletAvailable()
{
    return true;
}

float STSystemFunction::densityScale()
{
    return 0.0f;
}

void STSystemFunction::go2PrivacyPage()
{
    [[SystemFunctionManager sharedManager] go2PrivacyPage];
}

void STSystemFunction::openURL(std::string url)
{
    [[SystemFunctionManager sharedManager] openURL:[NSString stringWithUTF8String:url.c_str()]];
}

void STSystemFunction::endSession()
{
}

void STSystemFunction::saveLocalImage(void *image)
{
    [[SystemFunctionManager sharedManager] saveLocalImage:(cocos2d::Image*)image];
}

void STSystemFunction::refreshDCIM(std::string filename)
{
    
}
