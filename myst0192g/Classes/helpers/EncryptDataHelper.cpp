//
//  EncryptDataHelper.cpp
//  LVUP002
//
//  Created by Steven.Xc.Tian on 13-10-17.
//
//

#include "EncryptDataHelper.h"
#include "../FileModifiers.h"
#include "../utilities/STUserDefault.h"
#include "../utilities/base64.h"

using namespaceST::STUserDefault;
using std::string;
USING_NS_CC;


int EncryptDataHelper::updateHighScore(const std::string &key, int scores)
{
    // encode the key value
//    string encodeKey = base64_encode(reinterpret_cast<const unsigned char*>(key.c_str()), key.length());
    
    CCString* highScoreString = NULL;
    
    // first load value from file if exists.
    string  temphighScoreString = STLoadStringFromUD(key.c_str(), "0");
    if (temphighScoreString == "0")
    {
        // not exsit this value
        highScoreString = CCString::create(temphighScoreString);
    }
    else
    {
        // decode from base64
        highScoreString = CCString::create(base64_decode(temphighScoreString));
    }
    
    
    int oldHighScore = highScoreString->intValue();
    
    // have a new record
    if (scores > oldHighScore)
    {
        CCString* newHighScore = CCString::createWithFormat("%d", scores);
        CCLOG(" new record is %s", newHighScore->getCString());
        
        // save the highest score to STUserDefault, and encode with base64
        string encoded = base64_encode(reinterpret_cast<const unsigned char*>(newHighScore->getCString()),
                                       newHighScore->m_sString.length());
        
        STSaveStringToUD(key.c_str(), encoded);
        
        return scores;
    }
    
    return oldHighScore;
}

bool EncryptDataHelper::getPurchaseFlag(const string &key)
{
    // encode the key value
    string encodeKey = base64_encode(reinterpret_cast<const unsigned char*>(key.c_str()), key.length());
    
    // because this flag no need to delete, so we store it in CCUserDefault
    return CCUserDefault::sharedUserDefault()->getBoolForKey(encodeKey.c_str(), false);
}

void EncryptDataHelper::setPurchaseFlag(const std::string &key, bool value)
{
    // encode the key value
    string encodeKey = base64_encode(reinterpret_cast<const unsigned char*>(key.c_str()), key.length());
    
    CCUserDefault::sharedUserDefault()->setBoolForKey(encodeKey.c_str(), value);
}

long EncryptDataHelper::getRateUsTimestamp(const std::string &key)
{
    // encode the key value
    string encodeKey = base64_encode(reinterpret_cast<const unsigned char*>(key.c_str()), key.length());
    
    string temp = CCUserDefault::sharedUserDefault()->getStringForKey(encodeKey.c_str());
    
    string tv = base64_decode(temp);
    
    if (!tv.empty())
    {
        return atol(tv.c_str());
    }
    else
    {
        return 0;
    }
}

void EncryptDataHelper::setRateUsTimestamp(const string &key, long value)
{
    // check key
    if (key.empty())
        return;
    
    // format the value
    char tmp[50];
    memset(tmp, 0, 50);
    sprintf(tmp, "%ld", value);
    
    // encode the key value
    string encodeKey = base64_encode(reinterpret_cast<const unsigned char*>(key.c_str()), key.length());
    
    string encodeValue = base64_encode(reinterpret_cast<const unsigned char*>(tmp), strlen(tmp));
    
    CCUserDefault::sharedUserDefault()->setStringForKey(encodeKey.c_str(), encodeValue);
}

void EncryptDataHelper::setHintQuantity(const std::string &key, unsigned quantity)
{
    // save the highest score to STUserDefault, and encode with base64
    string encodedKey = base64_encode(reinterpret_cast<const unsigned char*>(key.c_str()), key.length());
    CCUserDefault::sharedUserDefault()->setIntegerForKey(encodedKey.c_str(), quantity);
}

unsigned EncryptDataHelper::getHintQuantity(const std::string &key)
{
    // encode the key value
    string encodedKey = base64_encode(reinterpret_cast<const unsigned char*>(key.c_str()), key.length());
    
    // because this flag no need to delete, so we store it in CCUserDefault
    return CCUserDefault::sharedUserDefault()->getIntegerForKey(encodedKey.c_str(), 5);
}




