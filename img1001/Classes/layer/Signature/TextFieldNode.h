//
//  TextFieldNode.h
//  TestProject
//
//  Created by houhuang on 15/10/13.
//
//

#ifndef __TestProject__TextFieldNode__
#define __TestProject__TextFieldNode__

#include <stdio.h>
#include "cocos2d.h"
#include "CursorTextField.h"
USING_NS_CC;
using namespace std;

class TextFieldNode : public Node
{
public:
    static TextFieldNode* create(const string& bgName, const string& scaleIcon, const string& deleteIcon);
    bool init(const string& bgName, const string& scaleIcon, const string& deleteIcon);

    TextFieldNode();
    ~TextFieldNode();
    void onEnterTransitionDidFinish();
public:
    void setTouchEnable(bool pBool);
    void addListener();
    void removeListener();
    
    void openIME();
    void closeIME();
    void setWhiteBGVisible(bool pBool);
    
    void updateTextColor(Color3B& color);
    void updareTextFont(const string& fontName);
    
    void showWhiteBG();
    void hideWhiteBG(float dt);
    void hideWhiteBG();
    
    void update(float dt);
    
    void reset();
    
    void onExit();
public:
//    virtual bool onTouchBegan(Touch *touch, Event *unused_event);
//    virtual void onTouchMoved(Touch *touch, Event *unused_event);
//    virtual void onTouchEnded(Touch *touch, Event *unused_event);
//    virtual void onTouchCancelled(Touch *touch, Event *unused_event);
    
private:
    bool    m_bIsMoveTextField;
    bool    m_bIsClickIcon;
    bool    m_bIsShowWhiteBG;
    
    bool    m_bIsClickThis;
    
    Vec2    m_vStartPoint;
    Vec2    m_vOffsetVecThis;
    Vec2    m_vOffsetVecScaleIcon;
    Vec2    m_vOrginalPositon;
    
    Sprite*             m_pBgSprite;
    CursorTextField*    m_pCursorTextField;
    Sprite*             m_pScaleIcon;
    
    Sprite*             m_pDeleteIcon;
    Sprite*             m_pBgWhite;
    LayerColor*         m_pLayerColor;
    
public:
    static bool m_bIsOpenKeyboard;
    
    EventListenerTouchAllAtOnce*    m_eListener;
    
    bool _isResonseSingleTouch;
    bool m_bIsTouchEnable;
    
    Vec2    m_vOrignalPoint;
    Vec2 _startPoint;
    
    bool  m_bIsPlayAnimation;
};

#endif /* defined(__TestProject__TextFieldNode__) */
