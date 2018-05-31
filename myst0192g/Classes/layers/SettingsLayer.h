//
//  SettingsLayer.h
//  LVUP002
//
//  Created by Steven.Xc.Tian on 13-9-25.
//
//

#ifndef __LVUP002__SettingsLayer__
#define __LVUP002__SettingsLayer__

#include "cocos2d.h"
#include "../widgets/CoverLayer.h"
#include "../widgets/STButton.h"
#include "../widgets/STSlider.h"


class SettingsLayer : public CoverLayer
{
    bool isOneButton;
    
    STSlider *pMusicSlider, *pSoundSlider;
    STButton *pBtnClose, *pBtnShare, *pBtnDelete;
    
public:
    static SettingsLayer* create(bool onlyOneButton);
    static SettingsLayer* createWithColor(cocos2d::ccColor4B color, bool onlyOneButton);
        
    SettingsLayer(bool onlyOneButton);
    virtual ~SettingsLayer();
    
    virtual bool initWithColor(const cocos2d::ccColor4B& color);
    
private:
    // return true add a subclass of CCNode, this node contains settings layer's contents.
    bool createFrame();
    
    //
    void onShareClicked(cocos2d::CCNode*);
    //
    void onDeleteClicked(cocos2d::CCNode*);
    //
    void onCloseClicked(cocos2d::CCNode*);
    //
    void onSliderChanged(CCNode* slider);
    
    void deleteUserData(CCObject*);
    
    void onAlertDialogClosed(cocos2d::CCObject*);
    
    void setButtonsEnable(bool boo);
    
};

#endif /* defined(__LVUP002__SettingsLayer__) */
