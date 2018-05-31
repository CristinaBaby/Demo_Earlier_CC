//
//  EncryptDataHelper.h
//  LVUP002
//
//  Created by Steven.Xc.Tian on 13-10-17.
//
//

#ifndef __LVUP002__EncryptDataHelper__
#define __LVUP002__EncryptDataHelper__

#include <cocos2d.h>

class EncryptDataHelper : public cocos2d::CCObject
{
public:
//    static EncryptDataHelper* getInstance();
//    static void destroy();
    
    /**
     *  for game finish layer to update the new high scores
     *
     *  @return the highest score
     */
    static int  updateHighScore(const std::string &key, int scores);
    
    static bool getPurchaseFlag(const std::string &key);
    static void setPurchaseFlag(const std::string &key, bool value);
    
    static long getRateUsTimestamp(const std::string &key);
    static void setRateUsTimestamp(const std::string &key, long value);
    
    static unsigned getHintQuantity(const std::string &key);
    static void     setHintQuantity(const std::string &key, unsigned quantity);
};



#endif /* defined(__LVUP002__EncryptDataHelper__) */
