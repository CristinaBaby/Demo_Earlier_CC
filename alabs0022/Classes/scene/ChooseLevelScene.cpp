//
//  ChooseLevelScene.cpp
//  ColorGarden
//
//  Created by lidong on 15/7/31.
//
//

#include "ChooseLevelScene.h"
#include "../helper/ControlHelper.h"
#include "../helper/ScreenAdapterHelper.h"
#include "../define/ResoureMacro.h"
#include "../scene/GameScene.h"
#include "../layer/Dialog.h"
#include "../modules/STSystemFunction.h"
#include "Analytics.h"
#include "../layer/KeyLayer.h"
//#include "../helper/AdHelper.h"
#include "AdsManager.h"

#include "../game/DataManager.h"
#include "../helper/ImageDownload.h"
#include "../layer/ChooseMapLayer.hpp"
#include "PatchManager.h"


#define st_move_distance 150

#define st_orgin_version_image_num 20


enum
{
    K_DIALOG_TAG=999
};

 cocos2d::Scene* ChooseLevelScene::scene(int pIndex)
{
    cocos2d::Scene* lScene=cocos2d::Scene::create();
    ChooseLevelScene *pRet = new ChooseLevelScene();
    if (pRet && pRet->init(pIndex))
    {
        pRet->autorelease();
        lScene->addChild(pRet);
        return lScene;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool ChooseLevelScene::init(int pIndex)
{
    if(!BaseLayer::init()) return false;
    
    //    if (DeviceHelper::getInstance()->getCurrentDeviceType() == kType_Device_Pad)
    //    {
    //        DataManager::getInstance()->readCsvData("ColorRGB_pad.csv");
    //    }else
    //    {
    //        DataManager::getInstance()->readCsvData("ColorRGB_phone.csv");
    //    }
    DataManager::getInstance()->readCsvData("colorRGB_new.csv");
    
    
    if(ImageDownload::getInstance()->getCurrentState()==STRAT_CHECK_VERSION_STATE)
    {
       ImageDownload::getInstance()->excuteFunc(STRAT_CHECK_VERSION_STATE);
    }
    
    this->setIndex(pIndex);
    
    DataManager::getInstance()->loadColorScvData();
    
    xPatch->patchChoose(0, nullptr);
    xPatch->patchFinish();
    
    LayerColor* layerColor=LayerColor::create(Color4B(49, 49, 49, 255));
    this->addChild(layerColor);
    
    Label* label=ControlHelper::getLabel(st_font_futumd, "Select an Artwork", 60);
    this->addChild(label);
    ScreenAdapterHelper::setPosition(label, Vec2(384, 900),true,false,false,false);

//    this->loadMap();
    
//    this->setToucEnable(true);
    
    ChooseMapLayer* layer = ChooseMapLayer::create();
    this->addChild(layer);
    
    //this->startListener();
 
    KeyLayer* lKeyLayer=KeyLayer::create();
    this->addChild(lKeyLayer);
    
//    AdHelper::requestAds();
    
//    AdsManager::getInstance()->showAds(kTypeBannerAds);
    AdsManager::getInstance()->setVisiable(kTypeBannerAds, true);
    
    MenuItemImage* privacy = MenuItemImage::create("Privacy Policy.png", "Privacy Policy.png", CC_CALLBACK_1(ChooseLevelScene::responsePrivacy, this));
    ScreenAdapterHelper::setPosition(privacy, Vec2(710, 895),true,false,false,false);
    
    Menu* lMenu = Menu::create(privacy, NULL);
    lMenu->setPosition(Vec2::ZERO);
    this->addChild(lMenu, 100);
    
    return true;
}

void ChooseLevelScene::responsePrivacy(Ref* ref)
{
    STSystemFunction sf;
    sf.go2PrivacyPage();
}

void ChooseLevelScene::onEnter()
{
    Layer::onEnter();
    _isAlreadyShowScene=true;
}

void ChooseLevelScene::loadMap()
{
    _imageBoxs.clear();
    //每多一个版本加一个Firstload
    if(!UserDefault::getInstance() -> getBoolForKey("firstLoad") ||!UserDefault::getInstance() -> getBoolForKey("firstLoad2") ||!UserDefault::getInstance() -> getBoolForKey("firstLoad3"))
    {

        for (int i=0; i<this->getPageNum(); i++) {
            ImageBox* lImageBox=ImageBox::create();
            lImageBox->setIndex(i+1);
            
//            //处理v1.1升级到v1.2时的特殊情况。。。
//            if (UserDefault::getInstance() -> getBoolForKey(lImageBox->getImageName().c_str()) == false && lImageBox->getIndex() > 10 && lImageBox -> getIndex() < 31 && UserDefault::getInstance() -> getBoolForKey("firstLoad2") == false && UserDefault::getInstance() -> getBoolForKey("firstLoad3") == false) {
//                
//                
//                
//            }
//            else
//            {
//                UserDefault::getInstance()->setBoolForKey(lImageBox->getImageName().c_str(), true);
//            }


            lImageBox->setScale(0.5f);
            lImageBox->setDelegate(this);
            lImageBox->setVisible(false);
            this->addChild(lImageBox);
            _imageBoxs.push_back(lImageBox);
 
        }
        
        UserDefault::getInstance() -> setBoolForKey("firstLoad", true);
        UserDefault::getInstance() -> setBoolForKey("firstLoad2", true);
        this->showGame();

    }
    else{
    
        for (int i=0; i<this->getPageNum(); i++) {
           ImageBox* lImageBox=ImageBox::create();
           lImageBox->setIndex(i+1);
           lImageBox->setScale(0.5f);
           lImageBox->setDelegate(this);
           lImageBox->setVisible(false);
           this->addChild(lImageBox);
           _imageBoxs.push_back(lImageBox);
//        if (i < ST_LAST_VERSION_MAP_NUM) {
//            lImageBox->_newSprite->setVisible(false);
//        }

    }
    
    
    this->showGame();
    }

}

int ChooseLevelScene::getRealIndex(int pIndex)
{
    if(pIndex<=0)
    {
        pIndex=pIndex+this->getPageNum();
    }
    else if(pIndex>this->getPageNum())
    {
        pIndex=pIndex-this->getPageNum();
    }
    return pIndex;
}

void ChooseLevelScene::showGame()
{
    for(int i=0;i<_imageBoxs.size();i++)
    {
        ImageBox* lImageBox=_imageBoxs.at(i);
        lImageBox->setVisible(false);
        lImageBox->removeNodes();

    }
    _showingBoxs.clear();
    int lCount=0;
    
    for (int i=this->getIndex()-2; i<=this->getIndex()+2; i++) {
        lCount++;
        ImageBox* lImageBox=this->getImageBox(this->getRealIndex(i));

        lImageBox->display();
        
        //是前几关 且 userD 是true
        if (this->getRealIndex(i) <= st_orgin_version_image_num || UserDefault::getInstance()->getBoolForKey(lImageBox->getImageName().c_str(), true) == false) {
            
            lImageBox->setIsShowingNew(false);
        }

        
        _showingBoxs.push_back(lImageBox);
        lImageBox->setVisible(true);
        ScreenAdapterHelper::setPosition(lImageBox, Vec2(384+((i-this->getIndex())*600), 512));
        lImageBox->setPriviousPos(lImageBox->getPosition());

    }
    
    for(int i=0;i<_imageBoxs.size();i++)
    {
        ImageBox* lImageBox=_imageBoxs.at(i);
        if(!lImageBox->isVisible())
        {
            Director::getInstance()->getTextureCache()->removeTextureForKey(lImageBox->getImageName());
        }
        
    }
    
}

ImageBox* ChooseLevelScene::getImageBox(int pIndex)
{
    return _imageBoxs.at(pIndex-1);
}


//得到Page数量
int ChooseLevelScene::getPageNum()
{
    return (int)DataManager::getInstance()->_resourceInfos.size()+ImageDownload::getInstance()->_downloadFinishResourceInfo.size();
}

void ChooseLevelScene::startListener()
{
    this->stopListener();
    EventListenerKeyboard* listener = EventListenerKeyboard::create();
    listener->onKeyReleased = CC_CALLBACK_2(ChooseLevelScene::onKeyPressed,this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener,this);
    _listener=listener;
}
void ChooseLevelScene::stopListener()
{
    if(_listener!=NULL)
    {
        _eventDispatcher->removeEventListener(_listener);
        _listener=NULL;
    }
}


void ChooseLevelScene::setToucEnable(bool pBool)
{
    _isTouchEnable=pBool;
    if(_isTouchEnable)
    {
        this->addTouchListener();
    }
    else
    {
        this->removeTocuhListener();
    }
}


void ChooseLevelScene::addTouchListener()
{
    this->removeTocuhListener();
    
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(ChooseLevelScene::onTouchBegan,this);
    listener->onTouchMoved = CC_CALLBACK_2(ChooseLevelScene::onTouchMoved,this);
    listener->onTouchEnded = CC_CALLBACK_2(ChooseLevelScene::onTouchEnded,this);
    listener->onTouchCancelled=CC_CALLBACK_2(ChooseLevelScene::onTouchCancelled,this);
    listener->setSwallowTouches(false);  //true不向下传递触摸
    dispatcher->addEventListenerWithSceneGraphPriority(listener,this);
    
    

}

void ChooseLevelScene::removeTocuhListener()
{
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->removeEventListenersForTarget(this);
}

bool ChooseLevelScene::onTouchBegan(Touch *touch, Event *unused_event)
{
    Vec2 lPosition=touch->getLocationInView();
    lPosition=CCDirector::getInstance()->convertToGL(lPosition);
    lPosition=this->convertToNodeSpace(lPosition);
    
    _startPoint=lPosition;
    _orginPoint=lPosition;
    
    _isMayExcuteTouchMove=true;
    _isTouchMoving=false;
    
    if(lPosition.y<200)
    {
        return false;
    }
    
    return true;
}
void ChooseLevelScene::onTouchMoved(Touch *touch, Event *unused_event)
{
    Vec2 lPosition=touch->getLocationInView();
    lPosition=CCDirector::getInstance()->convertToGL(lPosition);
     lPosition=this->convertToNodeSpace(lPosition);
    
    
    if(_isMayExcuteTouchMove)
    {
        //  _isMayExcuteTouchMove=false;
        _isTouchMoving=true;
    }
    
    if(!_isTouchMoving)
    {
        return;
    }
    
    
    float ldX=lPosition.x-_startPoint.x;
    
    float lDistanceX=lPosition.x-_orginPoint.x;
    if(lDistanceX>=10)
    {
        _isMayExcuteTouchMove=false;
    }
    if(fabs(lDistanceX)>=ScreenAdapterHelper::getValue(st_move_distance))
    {
        _isTouchMoving=false;
        
        this->refreshScroll(lDistanceX);
    }
    else
    {
        for (int i=0; i<_showingBoxs.size(); i++) {
            ImageBox* lImageBox=_showingBoxs[i];
            lImageBox->setPosition(lImageBox->getPosition()+Vec2(ldX,0));
        }
        _startPoint=lPosition;
    }
}
void ChooseLevelScene::onTouchEnded(Touch *touch, Event *unused_event)
{
    Vec2 lPosition=touch->getLocationInView();
    lPosition=CCDirector::getInstance()->convertToGL(lPosition);
     lPosition=this->convertToNodeSpace(lPosition);
    float lDistanceX=lPosition.x-_orginPoint.x;
    
    CCLOG("lDistanceX=%f",lDistanceX);
    
    if(_isTouchEnable)
    {
        this->refreshScroll(lDistanceX);
    }
    
    if(_isMayExcuteTouchMove)//android 有些设备会相应touch moved
    {
        for(int i=0;i<_imageBoxs.size();i++)
        {
            ImageBox* lImageBox=(ImageBox*)_imageBoxs[i];
            Vec2 lPosition=touch->getLocationInView();
            lPosition=CCDirector::getInstance()->convertToGL(lPosition);
            lImageBox->responseTouchEvent(lPosition);
        }
    }

}
void ChooseLevelScene::onTouchCancelled(Touch *touch, Event *unused_event)
{
    
}


void ChooseLevelScene::refreshScroll(float dx)
{
    
    if(dx!=0)
    {
        this->setToucEnable(false);
    }
    
    int lIndex=this->getIndex();
    if(dx>ScreenAdapterHelper::getValue(st_move_distance))
    {
        lIndex--;
       
    }
    else if(dx<-ScreenAdapterHelper::getValue(st_move_distance))
    {
        lIndex++;
    }
    

    if(this->getRealIndex(lIndex)==this->getIndex())
    {
        float lTime=0.1f;
        
        for (int i=0; i<_showingBoxs.size(); i++) {
            ImageBox* lImageBox=_showingBoxs[i];
            lImageBox->stopAllActions();
            
            MoveBy* lMoveBy=MoveBy::create(lTime,Vec2(-dx,0));
            
            if(i==_showingBoxs.size()-1)
            {
                CallFunc* lFunc=CallFunc::create(CC_CALLBACK_0(ChooseLevelScene::normalExcuteMoveEnd,this));
                Sequence* lAction=Sequence::create(lMoveBy,lFunc, NULL);
                lImageBox->runAction(lAction);
            }
            else
            {
                lImageBox->runAction(lMoveBy);
            }
        }

    }
    else
    {
        this->setIndex(this->getRealIndex(lIndex));

        float lTime=0.2f;
        
        for (int i=0; i<_showingBoxs.size(); i++) {
            ImageBox* lImageBox=_showingBoxs[i];
            lImageBox->stopAllActions();
            
            MoveTo* lMoveTo;
            if(dx>0)
            {
                lMoveTo=MoveTo::create(lTime,Vec2(ScreenAdapterHelper::getValue(600)+lImageBox->getPriviousPos().x,lImageBox->getPosition().y));
            }
            else
            {
                lMoveTo=MoveTo::create(lTime,Vec2(-ScreenAdapterHelper::getValue(600)+lImageBox->getPriviousPos().x,lImageBox->getPosition().y));
            }
            
            if(i==_showingBoxs.size()-1)
            {
                CallFunc* lFunc=CallFunc::create(CC_CALLBACK_0(ChooseLevelScene::excuteMoveEnd,this));
                CallFunc* lRefreshFunc=CallFunc::create(CC_CALLBACK_0(ChooseLevelScene::refreshDots, this));
                Sequence* lAction=Sequence::create(lMoveTo,lFunc,lRefreshFunc, NULL);
                lImageBox->runAction(lAction);
            }
            else
            {
                lImageBox->runAction(lMoveTo);
            }
        }

    }
    
}

void ChooseLevelScene::normalExcuteMoveEnd()
{
    this->setToucEnable(true);
}

void ChooseLevelScene::excuteMoveEnd()
{
    this->setToucEnable(true);
    this->showGame();
}

void ChooseLevelScene::refreshDots()
{
//    ChooseMapScene* lChooseMapScene=(ChooseMapScene*)this->getParent();
//    lChooseMapScene->refreshDots(_currentPage);
}

void ChooseLevelScene::reponseTouchEvent(ImageBox* pImageBox)
{
    int lIndex=pImageBox->getIndex();
    ImageBox* lImageBox=this->getImageBox(this->getRealIndex(lIndex));
    UserDefault::getInstance()->setBoolForKey(lImageBox->getUserNewTag().c_str(), false);
    Director::getInstance()->replaceScene(GameScene::createScene(lIndex));
}

void ChooseLevelScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
   // CCLOG("keyCode=%d",keyCode);
    
    if(keyCode== EventKeyboard::KeyCode::KEY_BACK)
    {
        Dialog* lDialog=(Dialog*)this->getChildByTag(K_DIALOG_TAG);
        if(lDialog)
        {
            return;
        }
        STAlertDialog*lAlertDialog=STAlertDialog::create("Are you sure you want to quit the game?","Yes", "No");
        lAlertDialog->setLeftListener(this,callfuncN_selector(ChooseLevelScene::quit));
        lAlertDialog->setRightListener(this, callfuncN_selector(ChooseLevelScene::removeDialog));
        this->addChild(lAlertDialog,0,K_DIALOG_TAG);
    }
}



void ChooseLevelScene::quit(Node* pRef)
{

    STSystemFunction* sf = new STSystemFunction();
    sf->endSession();
    CC_SAFE_DELETE(sf);
    
    Director::getInstance()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
void ChooseLevelScene::removeDialog(Node* pRef)
{
    pRef->removeFromParentAndCleanup(true);

}

ChooseLevelScene::~ChooseLevelScene()
{
    this->stopListener();
    NotificationCenter::getInstance()->removeObserver(this, st_begin_download_image);
}

ChooseLevelScene::ChooseLevelScene()
{
    NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(ChooseLevelScene::firstRefresh),st_begin_download_image,NULL);
}

void ChooseLevelScene::refresh()
{
    this->setToucEnable(false);
    
    this->scheduleOnce(schedule_selector(ChooseLevelScene::changeScene), 0.3f);
    
}

void ChooseLevelScene::firstRefresh(Ref* pSender)
{
    this->setToucEnable(false);
    
    this->scheduleOnce(schedule_selector(ChooseLevelScene::changeScene), 0.3f);
}

void ChooseLevelScene::changeScene(float dt)
{
   Director::getInstance()->replaceScene(ChooseLevelScene::scene(this->getIndex()));
}

