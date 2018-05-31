//
//  STResManager.cpp
//  LVUP002
//
//  Created by Steven.Xc.Tian on 13-9-24.
//
//

#include "STResManager.h"
#include "../AppConfig.h"
#include "../AppGlobal.h"

USING_NS_ST;
USING_NS_CC;
using std::vector;
using std::string;

static STResManager* s_p_STResManager = NULL;

STResManager* STResManager::getInstance()
{
    if (!s_p_STResManager)
    {
        s_p_STResManager = new STResManager();
    }
    
    return s_p_STResManager;
}

void STResManager::destroy()
{
    if (s_p_STResManager)
    {
        delete s_p_STResManager;
        s_p_STResManager = NULL;
    }
}

vector<string> STResManager::getFileSearchPath()
{
    vector<string> searchpath;
    string languageSuffix = getCurrentLanguageSuffix();
    if (!languageSuffix.empty())
    {
        languageSuffix = "/ui-" + languageSuffix;
    }
    
    if (AppGlobal::IS_2048)
    {
        searchpath.push_back(UI_ROOT_2048 + languageSuffix);
        searchpath.push_back(UI_ROOT_2048);
        searchpath.push_back(UI_ROOT_1024 + languageSuffix);
        searchpath.push_back(UI_ROOT_1024);
    }
    else
    {
        searchpath.push_back(UI_ROOT_1024 + languageSuffix);
        searchpath.push_back(UI_ROOT_1024);
        searchpath.push_back(UI_ROOT_2048 + languageSuffix);
        searchpath.push_back(UI_ROOT_2048);
    }
    
    return searchpath;
}

const char* STResManager::getCurrentLanguageSuffix()
{
    switch (CCApplication::sharedApplication()->getCurrentLanguage())
    {
        case kLanguageChinese:
            return "zh";
        case kLanguageFrench:
            return "fr";
        case kLanguageItalian:
            return "it";
        case kLanguageGerman:
            return "de";
        case kLanguageSpanish:
            return "es";
        case kLanguageRussian:
            return "ru";
        case kLanguageKorean:
            return "ko";
        case kLanguageJapanese:
            return "ja";
        case kLanguageHungarian:
            return "hu";
        case kLanguagePortuguese:
            return "pt";
        case kLanguageArabic:
            return "ar";
        default:
            // kLanguageEnglish
            return "en";
    }
}





