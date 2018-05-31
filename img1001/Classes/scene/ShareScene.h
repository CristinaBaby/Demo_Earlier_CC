//
//  ShareScene.h
//  ColorFree
//
//  Created by houhuang on 15/9/9.
//
//

#ifndef __ColorFree__ShareScene__
#define __ColorFree__ShareScene__

#include <stdio.h>
#include "cocos2d.h"
#include "../define/Globalmacro.h"
#include "../compoment/UIButton.h"
#include "../base/BaseScene.hpp"
#include "../base/SwallowListenerLayer.hpp"
USING_NS_CC;
using namespace std;

class ShareScene : public BaseScene
{
public:
//    CREATE_FUNC(ShareScene);
//    SCENE_FUNC(ShareScene);
    static Scene* scene(Sprite* pSprite);
    bool init(Sprite* pSprite);
    
    void initUI();
    
    void onEnterTransitionDidFinish();
    
    ShareScene();
    ~ShareScene();
public:
    void doAction(Node* pNode);
    void reponsePostBtn(Ref* ref);
    
    void saveShareImage();
    bool createDirectory(const char *path);
    
    //post
    void waitPostSuccessful(Ref* ref);
    void waitPostFaild(Ref* ref);
    
    void reponsePostSuccessful(Node* node);
    void reponsePostFaild(Node* node);
    
    void removePostDialog(Node* node);
    
    string getSaveImagePath();
    
    void delayePostImage(float dt);
    
    void threadSaveDatas();

protected:
    string  m_sImagePath;
    Sprite* m_pShareSprite;

    UIButton*   m_pBackBtn;
    UIButton*   m_pHomeBtn;
    SwallowListenerLayer*   m_pPostingLayer;
    
    string  m_sMyPostImageName;
    Menu*   m_pPostMenu;
};

#endif /* defined(__ColorFree__ShareScene__) */
