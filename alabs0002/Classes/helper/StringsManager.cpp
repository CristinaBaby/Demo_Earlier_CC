//
//  StringsManager.cpp
//  ALABS0010
//
//  Created by luoxiaopeng on 16/3/21.
//
//

#include "StringsManager.h"
#include "UICpp.h"


static StringsManager *_sharedString = nullptr;

StringsManager* StringsManager::getInstance()
{
    if (! _sharedString)
    {
        _sharedString = new StringsManager();
    }
    
    return _sharedString;
}

void StringsManager::destroyInstance()
{
    CC_SAFE_DELETE(_sharedString);
}

StringsManager::StringsManager()
{
    LanguageType currentLanguageType = Application::getInstance()->getCurrentLanguage();
    switch (currentLanguageType)
    {
        case LanguageType::ENGLISH:
            _dialogFontName = "fonts/verdana_0.ttf";
            _normalFontName = "fonts/Seravek-ExtraLight.ttf";
            _boldFontName = "fonts/ArbutusSlab-Regular.ttf";
            break;
        case LanguageType::CHINESE:  // only for test
            _dialogFontName = "fonts/verdana_0.ttf";
            _normalFontName = "fonts/Seravek-ExtraLight.ttf";
            _boldFontName = "fonts/ArbutusSlab-Regular.ttf";
            break;
        default:
            CCASSERT(false, "Invalid language type.");
            break;
    }
    
    _strings = FileUtils::getInstance()->getValueMapFromFile("strings.xml");
}

void StringsManager::translate(Widget* root, const string &name)
{
    Text* text = (Text*)Helper::seekNodeByName(root, name);
    CCASSERT(_strings.find(name) != _strings.end(), "error");
    text->setString(_strings[name].asString());
    text->setColor(UI_COLOR);
}

void StringsManager::translateNormal(Widget* root, const string &name)
{
    Text* text = (Text*)Helper::seekNodeByName(root, name);
    CCASSERT(_strings.find(name) != _strings.end(), "error");
    text->setFontName(_normalFontName);
    text->setString(_strings[name].asString());
    text->setColor(UI_COLOR);
    
}

void StringsManager::translateBold(Widget* root, const string &name)
{
    Text* text = (Text*)Helper::seekNodeByName(root, name);
    CCASSERT(_strings.find(name) != _strings.end(), "error");
    text->setFontName(_boldFontName);
    text->setString(_strings[name].asString());
    text->setColor(UI_COLOR);
}

string StringsManager::getString(const string& name)
{
    return std::move(_strings[name].asString());
}

string StringsManager::getStringDefault(const string& name)
{
    string key = "TXT_" + name;
    if (_strings.find(key) != _strings.end()) {
        return std::move(_strings[key].asString());
    }
    else
    {
        return name;
    }
}

const string & StringsManager::getDialogFontName()
{
    return _dialogFontName;
}

const string & StringsManager::getBoldFontName()
{
    return _boldFontName;
}

