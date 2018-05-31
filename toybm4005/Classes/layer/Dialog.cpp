//
//  Dialog.cpp
//  HIDD
//
//  Created by lidong on 15-4-7.
//
//

#include "Dialog.h"
#include "../define/ResoureMacro.h"
#include "../define/GlobalMacro.h"
#include "../helper/ScreenAdapterHelper.h"
#include "../helper/ControlHelper.h"
#include "../helper/AudioHelper.h"
#include "../helper/PurchaseHelper.h"
#include "../modules/STSystemFunction.h"
//#include "../helper/AdHelper.h"
#include "../utilities/STUtility.h"
#pragma mark - Dialog

enum
{
   eSettingLayer_Close_Item_Tag=10,
   eSettingLayer_TellFriends_Item_Tag,
   eSettingLayer_DeleteData_Item_Tag,
   eSettingLayer_Slider_Music_Tag,
   eSettingLayer_Slider_Sound_Tag,
   eAlertDialog_Left_Item_Tag,
   eAlertDialog_Right_Item_Tag,
   eDeluxe_Close_Item_Tag,
   eDeluxe_Restore_Tag,
   eDeluxe_Buy_Restore_Tag,
   eDeluxe_Buy_NoRestore_Tag,
   eGameOver_Home_Item_Tag,
   eGameOver_Replay_Item_Tag,
   eGameComplete_Home_Item_Tag,
   eGameComplete_Replay_Item_Tag,
   eGameComplete_Next_Item_Tag,
    
   K_DIALOG_TAG=999
};

Dialog* Dialog::create()
{
    Dialog* lDialog = new Dialog();
    if (lDialog && lDialog->cocos2d::Layer::init())
    {
        lDialog->autorelease();
        return lDialog;
    }
    
    CC_SAFE_DELETE(lDialog);
    return NULL;
}

bool Dialog::init()
{
    if(!Layer::init()) return false;
    
    LayerColor* lColorLayer=LayerColor::create(Color4B::BLACK);
    this->addChild(lColorLayer);
    lColorLayer->setOpacity(180);
    _layerColor=lColorLayer;
    
//    FadeTo* lFadeTo=FadeTo::create(1.0f, 180);
//    lColorLayer->runAction(lFadeTo);
    
    this->setToucEnable(true);
    
    
    
    
    return true;
}
void Dialog::setToucEnable(bool pBool)
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

void Dialog::removeTocuhListener()
{
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->removeEventListenersForTarget(this);
}
void Dialog::addTouchListener()
{
    
    
    this->removeTocuhListener();
    
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener=EventListenerTouchOneByOne::create();
    listener->onTouchBegan=CC_CALLBACK_2(Dialog::onTouchBegan,this);
    listener->setSwallowTouches(true);
    dispatcher->addEventListenerWithSceneGraphPriority(listener,this);

}

bool Dialog::onTouchBegan(Touch *touch, Event *unused_event)
{
    Vec2 lPosition=touch->getLocationInView();
    lPosition=CCDirector::getInstance()->convertToGL(lPosition);

    
    return true;
}

void Dialog::doAction(cocos2d::Ref *pSender)
{
  //  AudioHelper::getInstance()->playSound(st_tap_button_sound);
}

Dialog::~Dialog()
{
    this->removeTocuhListener();
}

#pragma mark - TargetDialog

TargetDialog::TargetDialog():_leftTarget(NULL),_left_selector(NULL),_rightTarget(NULL),_right_selector(NULL)
{
 
}

bool TargetDialog::init()
{
    if(!Dialog::init()) return false;
    
    this->startListener();
    
    return true;
}

void TargetDialog::setLeftListener(Ref* pLeftTarget,SEL_CallFuncN pLeftSelector)
{
    _leftTarget=pLeftTarget;
    _left_selector=pLeftSelector;
}
void TargetDialog::setRightListener(Ref* pRightTraget,SEL_CallFuncN pRightSelector)
{
    _rightTarget=pRightTraget;
    _right_selector=pRightSelector;
}
void TargetDialog::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    if(keyCode== EventKeyboard::KeyCode::KEY_BACK)
    {
        Dialog* lDialog=(Dialog*)this->getChildByTag(K_DIALOG_TAG);
        if(lDialog)
        {
            return;
        }
        if(_rightTarget && _right_selector)
        {
            (_rightTarget->*_right_selector)(this);
        }
    }
}

void TargetDialog::removeDialog(Node* pNode)
{
    pNode->removeFromParentAndCleanup(true);
    Dialog* lDialog=(Dialog*)this->getChildByTag(K_DIALOG_TAG);
    if(lDialog)
    {
        return;
    }
    this->startListener();
}

void TargetDialog::startListener()
{
    this->stopListener();
    auto listener = EventListenerKeyboard::create();
    listener->onKeyReleased = CC_CALLBACK_2(TargetDialog::onKeyPressed,this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener,this);
    _mListener=listener;
}

void TargetDialog::stopListener()
{
    if(_mListener!=NULL)
    {
        _eventDispatcher->removeEventListener(_mListener);
        _mListener=NULL;
    }

}

#pragma mark - STAlertDialog

STAlertDialog::STAlertDialog()
{
}

bool STAlertDialog::init(string pText,string pLeftBtnName,string pRightBtnName)
{
    if(!TargetDialog::init()) return false;

    this->setToucEnable(true);
    
    Sprite* lBgSprite;
    if(pRightBtnName.empty())
    {
        lBgSprite=ControlHelper::AddSprite("pop01.png", this);
        check_assert(lBgSprite);
        ScreenAdapterHelper::setPosition(lBgSprite, Vec2(384, 600));
    }
    else
    {
        lBgSprite=ControlHelper::AddSprite("pop02.png", this);
        check_assert(lBgSprite);
        ScreenAdapterHelper::setPosition(lBgSprite, Vec2(384, 600));
    }
    
    Label* label=ControlHelper::getLabel(st_font_futumd, pText.c_str(),st_dialog_text_font_size,Size(lBgSprite->getContentSize().width*0.9, lBgSprite->getContentSize().height*0.9f),TextHAlignment::CENTER,TextVAlignment::CENTER);
    label->setColor(Color3B::BLACK);
    lBgSprite->addChild(label);
    label->setPosition(lBgSprite->getContentSize().width/2,lBgSprite->getContentSize().height*2.0f/3);
    
    if(pRightBtnName.empty())
    {
        MenuItemSprite* lLeftMenuItem=ControlHelper::getMenuItem(st_dialog_button_image, eAlertDialog_Left_Item_Tag, CC_CALLBACK_1(STAlertDialog::doAction, this));
        lLeftMenuItem->setPosition(lBgSprite->getContentSize().width/2,ScreenAdapterHelper::getValue(40));
       
    
        
        Label* leftLabel=ControlHelper::getLabel(st_font_futumd, pLeftBtnName.c_str(), st_dialog_text_font_size);
        leftLabel->setColor(Color3B(0, 0, 0));
        leftLabel->setPosition(Vec2(lLeftMenuItem->getContentSize().width/2,lLeftMenuItem->getContentSize().height/2));
        lLeftMenuItem->addChild(leftLabel);
        
        _menu=Menu::create(lLeftMenuItem, NULL);
        lBgSprite->addChild(_menu);
        _menu->setPosition(Vec2(0, 0));
    }
    else
    {
        MenuItemSprite* lLeftMenuItem=ControlHelper::getMenuItem(st_dialog_button_image, eAlertDialog_Left_Item_Tag, CC_CALLBACK_1(STAlertDialog::doAction, this));
        lLeftMenuItem->setPosition(lBgSprite->getContentSize().width/4,ScreenAdapterHelper::getValue(40));
         lLeftMenuItem->setColor(Color3B::RED);
        Label* leftLabel=ControlHelper::getLabel(st_font_futumd, pLeftBtnName.c_str(), st_dialog_text_font_size);
        leftLabel->setColor(Color3B(0, 0, 0));
        leftLabel->setPosition(Vec2(lLeftMenuItem->getContentSize().width/2,lLeftMenuItem->getContentSize().height/2));
          lLeftMenuItem->addChild(leftLabel);
        
        MenuItemSprite* lRightMenuItem=ControlHelper::getMenuItem(st_dialog_button_image, eAlertDialog_Right_Item_Tag, CC_CALLBACK_1(STAlertDialog::doAction, this));
        lRightMenuItem->setPosition(lBgSprite->getContentSize().width*3.0f/4,ScreenAdapterHelper::getValue(40));
        lRightMenuItem->setColor(Color3B::BLUE);
        Label* rightLabel=ControlHelper::getLabel(st_font_futumd, pRightBtnName.c_str(), st_dialog_text_font_size);
        rightLabel->setColor(Color3B(0, 0, 0));
        rightLabel->setPosition(Vec2(lRightMenuItem->getContentSize().width/2, lRightMenuItem->getContentSize().height/2));
        lRightMenuItem->addChild(rightLabel);
        
        _menu=Menu::create(lLeftMenuItem,lRightMenuItem, NULL);
        lBgSprite->addChild(_menu);
        _menu->setPosition(Vec2(0, 0));
    }
    
    
    
    return true;
}



void STAlertDialog::doAction(cocos2d::Ref *pSender)
{
    _menu->setEnabled(false);
    
    Dialog::doAction(pSender);
    MenuItemSprite* lMenuItem=(MenuItemSprite*)pSender;
    switch (lMenuItem->getTag()) {
        case eAlertDialog_Left_Item_Tag:
        {
            if(_leftTarget && _left_selector)
            {
                (_leftTarget->*_left_selector)(this);
            }
        }
            break;
        case eAlertDialog_Right_Item_Tag:
        {
            if(_rightTarget && _right_selector)
            {
                (_rightTarget->*_right_selector)(this);
            }
        }
            break;
        default:
            break;
    }
}

STAlertDialog* STAlertDialog::create(string pImage, string pLeftBtnName, string pRightBtnName)
{
    STAlertDialog* lDialog=new STAlertDialog();
    if(lDialog && lDialog->init(pImage, pLeftBtnName, pRightBtnName))
    {
        lDialog->autorelease();
        return lDialog;
    }
    
    CC_SAFE_DELETE(lDialog);
    return NULL;
}

#pragma mark - DeluxeDialog

DeluxeDialog::DeluxeDialog()
{
//    _stInAppBilling =new STInAppBilling();
//    _stInAppBilling->setIABDelegate(PurchaseHelper::getInstance());
}

DeluxeDialog::~DeluxeDialog()
{
//    CC_SAFE_DELETE(_stInAppBilling);
    
}

bool DeluxeDialog::init()
{
    if(!TargetDialog::init()) return false;
    
    this->loadResource();
    this->loadUI();
    
    

    
    return true;
}

void DeluxeDialog::loadResource()
{
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(st_deluxe_plist);
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(st_pop_window_plist);
    //读取地图坐标信息
  //  _nameVecMaps=DataManager::getInstance()->deluxe_NameVecMaps;
}

void DeluxeDialog::loadUI()
{
    
    
}

void DeluxeDialog::excuteAnimation()
{
    
}

Vec2 DeluxeDialog::getPlayPosition()
{
    Sprite* lHintSprite=ControlHelper::addSpriteWithFrame(st_deluxe_hint_image, this);
    ScreenAdapterHelper::setPosition(lHintSprite, _nameVecMaps[st_deluxe_hint_image].position);
    Sprite* lPlaySprite=ControlHelper::addSpriteWithFrame(st_deluxe_btn_buy_image, this);
    ScreenAdapterHelper::setPosition(lPlaySprite, _nameVecMaps[st_deluxe_btn_buy_image].position);
    Vec2 lPoint=lHintSprite->convertToNodeSpace(lPlaySprite->getPosition());
    lHintSprite->removeFromParentAndCleanup(true);
    lPlaySprite->removeFromParentAndCleanup(true);
    return lPoint;
}

void DeluxeDialog::doAction(cocos2d::Ref *pSender)
{
//           CCLOG("========button action=========");
//    
//    Dialog::doAction(pSender);
//    MenuItemSprite* lMenuItem=(MenuItemSprite*)pSender;
//    switch (lMenuItem->getTag()) {
//        case eDeluxe_Close_Item_Tag:
//        {
//            if(_rightTarget && _right_selector)
//            {
//                (_rightTarget->*_right_selector)(this);
//            }
//        }
//            break;
//        case eDeluxe_Restore_Tag:
//        {
//            if(_leftTarget && _left_selector)
//            {
//                (_leftTarget->*_left_selector)(this);
//            }
//        }
//            break;
//        case eDeluxe_Buy_Restore_Tag:
//        {
//            SkuInfo* lInfo=(SkuInfo*)lMenuItem->getUserData();
//           
//     
//            string sku;
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//            sku=lInfo->str_ios_iap;
//#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//            sku=lInfo->str_android_iap;
//#endif
//        CCLOG("＝＝＝＝＝IAP SKU: %s", sku.c_str());
//            
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//            // if already unlocked all levels, pop up the dialog to prevent purchase again.
//            if(strcmp(sku.c_str(), st_iap_sku_unlock_all)==0)
//            {
//                // if already unlocked all levels, pop up the dialog to prevent purchase again.
//               
//               // if(DataManager::getInstance()->isUnlockAllLevel())
//                {
//                    string lMessageStr = DataManager::getInstance()->getMessages()[st_string_already_purchased_key];
//                    STSystemFunction* sf = new STSystemFunction();
//                    sf->popAlertDialog(lMessageStr.c_str());
//                    CC_SAFE_DELETE(sf);
//                    return;
//                }
//            }
//            else if(strcmp(sku.c_str(), st_iap_sku_ad)==0)
//            {
//                // if already unlocked all levels, pop up the dialog to prevent purchase again.
//              //  SaveGameData& lSaveGameData=DataManager::getInstance()->shareSaveGameData();
//               // if(DataManager::getInstance()->isUnlockAllLevel() && lSaveGameData.isRemoveAd)
//                {
//                    string lMessageStr = DataManager::getInstance()->getMessages()[st_string_already_purchased_key];
//                    STSystemFunction* sf = new STSystemFunction();
//                    sf->popAlertDialog(lMessageStr.c_str());
//                    CC_SAFE_DELETE(sf);
//                    return;
//                }
//            }
//
//#endif
//
//            
//#ifdef DEBUG_IAP
//            PurchaseHelper::getInstance()->purchaseSuccessful(sku->getCString());
//#else
//            _stInAppBilling->purchase(sku.c_str());
//#endif
//         
// 
//            
////            SaveGameData& lSaveGameData=DataManager::getInstance()->loadSaveGameData();
////            lSaveGameData.hintNumber=lSaveGameData.hintNumber+st_init_set_hint_number_value;
////            DataManager::getInstance()->excuteSaveGameData();
////            NotificationCenter::getInstance()->postNotification(st_refresh_hint_notification);
//        }
//            break;
//        case eDeluxe_Buy_NoRestore_Tag:
//        {
//            SkuInfo* lInfo=(SkuInfo*)lMenuItem->getUserData();
//            string sku;
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//            sku=lInfo->str_ios_iap;
//#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//            sku=lInfo->str_android_iap;
//#endif
//             CCLOG("＝＝＝＝＝IAP SKU: %s", sku.c_str());
//            
//#ifdef DEBUG_IAP
//             PurchaseHelper::getInstance()->purchaseSuccessful(sku->getCString());
//#else
//
//            _stInAppBilling->purchaseUnmanaged(sku.c_str());
//            
//#endif
//            
////            DataManager::getInstance()->unlockAllLockedLevel();
////            NotificationCenter::getInstance()->postNotification(st_refresh_choose_map_box_locked_notification);
//            
//               CCLOG("IAP SKU: %s", sku.c_str());
//            
//        }
//            break;
//        default:
//            break;
//    }
}

TableViewCell* DeluxeDialog::tableCellAtIndex(TableView *table, ssize_t idx)
{
    TableViewCell *cell = table->dequeueCell();
    if (!cell) {
        cell = new TableViewCell();
        cell->autorelease();
    }
    else
    {
        cell->removeAllChildren();
    }
    

    
//    Sprite* lBuySprite=Sprite::createWithSpriteFrameName(st_deluxe_btn_buy_image);
//    
//    SkuInfo info=DataManager::getInstance()->getSkuInfos()[idx];
//    Sprite* lBgNameSprite=ControlHelper::addSpriteWithFrame(info.str_bg_name.c_str(), cell);
//    lBgNameSprite->setPosition(_cellSize.width/2-lBuySprite->getContentSize().width/4,_cellSize.height/2);
//    
//    bool lIsStore=false;
//    if(strcmp(info.str_store.c_str(), "TRUE")==0)
//    {
//        lIsStore=true;
//    }
//    
//    MenuItemSprite* lBuyMenuItem=ControlHelper::getMenuItemWithFrameName_NoSameSize(st_deluxe_btn_buy_image, st_deluxe_btn_buy_h_image, lIsStore?eDeluxe_Buy_Restore_Tag:eDeluxe_Buy_NoRestore_Tag, CC_CALLBACK_1(DeluxeDialog::doAction, this));
//    lBuyMenuItem->setPosition(this->getPlayPosition());
//    lBuyMenuItem->setUserData(&DataManager::getInstance()->getSkuInfos()[idx]);
//    Menu* lMenu=Menu::create(lBuyMenuItem, NULL);
//    lBgNameSprite->addChild(lMenu);
//    lMenu->setPosition(0,0);
    
    return cell;
    
}
ssize_t DeluxeDialog::numberOfCellsInTableView(TableView *table)
{
    //return DataManager::getInstance()->getSkuInfos().size();
    return 1;
}
Size DeluxeDialog::cellSizeForTable(TableView *table)
{
    return _cellSize;
}

void DeluxeDialog::tableCellTouched(TableView* table, TableViewCell* cell)
{
    
}

#pragma mark -SettingDialog

bool SettingDialog::init()
{
    if(!TargetDialog::init()) return false;
    
    _listener=NULL;
    
    this->loadResource();
    this->loadUI();
    
    this->setToucEnable(true);
    
    return true;
}

SettingDialog::~SettingDialog()
{
    this->setToucEnable(false);
}

void SettingDialog::setToucEnable(bool pBool)
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
void SettingDialog::removeTocuhListener()
{
    if(_listener!=NULL)
    {
        auto dispatcher = Director::getInstance()->getEventDispatcher();
        dispatcher->removeEventListener(_listener);
    }
}

void SettingDialog::loadResource()
{
  //  SpriteFrameCache::getInstance()->addSpriteFramesWithFile(st_pop_window_plist);
    //读取地图坐标信息
   // _nameVecMaps=DataManager::getInstance()->setting_NameVecMaps;
}

void SettingDialog::loadUI()
{
    //load Box   //可能是图片不一样 程序中用的资源和地图用的资源
    Sprite* lSettingBg=ControlHelper::AddSprite(st_setting_box_image, this);
    check_assert(lSettingBg);
    ScreenAdapterHelper::setPosition(lSettingBg, _nameVecMaps[st_setting_box_image].position);
    _backgroundSprite=lSettingBg;
    
    //text
    Sprite* lTextSprite=ControlHelper::addSpriteWithFrame(st_setting_text_image, lSettingBg);
    check_assert(lTextSprite);
    ScreenAdapterHelper::setPosition(lTextSprite, _nameVecMaps[st_setting_text_image].position);
    lTextSprite->setPosition(lSettingBg->convertToNodeSpace(lTextSprite->getPosition()));
    //Music
    Sprite* lMusicSprite=ControlHelper::addSpriteWithFrame(st_setting_music_image, lSettingBg);
    check_assert(lMusicSprite);
    ScreenAdapterHelper::setPosition(lMusicSprite, _nameVecMaps[st_setting_music_image].position);
    lMusicSprite->setPosition(lSettingBg->convertToNodeSpace(lMusicSprite->getPosition()));
    //Sound
    Sprite* lSoundSprite=ControlHelper::addSpriteWithFrame(st_setting_sound_image, lSettingBg);
    check_assert(lSoundSprite);
    ScreenAdapterHelper::setPosition(lSoundSprite, _nameVecMaps[st_setting_sound_image].position);
    lSoundSprite->setPosition(lSettingBg->convertToNodeSpace(lSoundSprite->getPosition()));

    //Slider
    STSlider* lSliderMusic=STSlider::create(st_setting_bar_bg_image, st_setting_progress_image, st_setting_bar_btn_image);
    lSliderMusic->setDelegate(this);
    lSliderMusic->setTag(eSettingLayer_Slider_Music_Tag);
    lSliderMusic->setMinValue(0.0f);
    lSliderMusic->setMaxValue(1.0f);
    lSettingBg->addChild(lSliderMusic);
    ScreenAdapterHelper::setPosition(lSliderMusic, _nameVecMaps[st_setting_bar_bg_image].position);
    lSliderMusic->setPosition(lSettingBg->convertToNodeSpace(lSliderMusic->getPosition()));
    
    Node* lProgressNode=Node::create();
    ScreenAdapterHelper::setPosition(lProgressNode, _nameVecMaps[st_setting_progress_image].position);
    
    Node* lBarNode=Node::create();
    ScreenAdapterHelper::setPosition(lBarNode,  _nameVecMaps[st_setting_bar_btn_image].position);
    
    lSliderMusic->changePosition(lProgressNode->getPosition(),lBarNode->getPosition());
    lSliderMusic->changeValue(AudioHelper::getMusicValue());
    _sliders.push_back(lSliderMusic);
    
    STSlider* lSliderSound=STSlider::create(st_setting_bar_bg_image, st_setting_progress_image, st_setting_bar_btn_image);
    lSliderSound->setDelegate(this);
    lSliderSound->setTag(eSettingLayer_Slider_Sound_Tag);
    lSliderSound->setMinValue(0.0f);
    lSliderSound->setMaxValue(1.0f);
    lSettingBg->addChild(lSliderSound);
    ScreenAdapterHelper::setPosition(lSliderSound, _nameVecMaps[st_setting_bar_bg_2_image].position);
    lSliderSound->setPosition(lSettingBg->convertToNodeSpace(lSliderSound->getPosition()));
    
    ScreenAdapterHelper::setPosition(lProgressNode, _nameVecMaps[st_setting_progress_2_image].position);
    ScreenAdapterHelper::setPosition(lBarNode, _nameVecMaps[st_setting_bar_btn_2_image].position);
    
    lSliderSound->changePosition(lProgressNode->getPosition(), lBarNode->getPosition());
    lSliderSound->changeValue(AudioHelper::getSoundValue());
    
    _sliders.push_back(lSliderSound);
    
    //Buttons
    MenuItemSprite* lMenuItemClose=ControlHelper::getMenuItemWithFrameName_NoSameSize(st_setting_close_image, st_setting_close_h_image, eSettingLayer_Close_Item_Tag, CC_CALLBACK_1(SettingDialog::doAction, this));
    ScreenAdapterHelper::setPosition(lMenuItemClose, _nameVecMaps[st_setting_close_image].position);
    lMenuItemClose->setPosition(lSettingBg->convertToNodeSpace(lMenuItemClose->getPosition()));

    MenuItemSprite* lMenuItemDeleteData=ControlHelper::getMenuItemWithFrameName_NoSameSize(st_setting_delete_image, st_setting_delete_h_image, eSettingLayer_DeleteData_Item_Tag, CC_CALLBACK_1(SettingDialog::doAction, this));
    ScreenAdapterHelper::setPosition(lMenuItemDeleteData, _nameVecMaps[st_setting_delete_image].position);
    lMenuItemDeleteData->setPosition(lSettingBg->convertToNodeSpace(lMenuItemDeleteData->getPosition()));

    MenuItemSprite* lMenuItemTellFriends=ControlHelper::getMenuItemWithFrameName_NoSameSize(st_setting_tell_friend_image, st_setting_tell_friend_h_image, eSettingLayer_TellFriends_Item_Tag, CC_CALLBACK_1(SettingDialog::doAction, this));
    ScreenAdapterHelper::setPosition(lMenuItemTellFriends, _nameVecMaps[st_setting_tell_friend_image].position);
    lMenuItemTellFriends->setPosition(lSettingBg->convertToNodeSpace(lMenuItemTellFriends->getPosition()));
    _menu=Menu::create(lMenuItemClose,lMenuItemDeleteData,lMenuItemTellFriends, NULL);
    lSettingBg->addChild(_menu);
    _menu->setPosition(Vec2(0.0f, 0.0f));

    
}

void SettingDialog::doAction(cocos2d::Ref *pSender)
{
//    Dialog::doAction(pSender);
//    MenuItemSprite* lMenuItem=(MenuItemSprite*)pSender;
//    switch (lMenuItem->getTag()) {
//        case eSettingLayer_Close_Item_Tag:
//        {
//            if(_rightTarget && _right_selector)
//            {
//                (_rightTarget->*_right_selector)(this);
//            }
//        }
//            break;
//        case eSettingLayer_TellFriends_Item_Tag:
//        {
//
//            string subject = DataManager::getInstance()->getMessages()[st_string_email_subject_key];
//
//            std::string body;
//            
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//            body =  DataManager::getInstance()->getMessages()[st_string_email_body_key];
//            
//#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//            
//             body.append("Hey!\n")
//            .append("\nI am playing this awesome hidden object game - ")
//            .append(project_name)
//            .append("!\nI think you will like this also!\n")
//            .append("Get it NOW!\n");
//            
////#if (ST_TARGET_MARKET == ST_TARGET_MARKET_AMAZON)
////            body.append("http://www.amazon.com/gp/mas/dl/android?p=")
////            .append(st_android_package_name).append("\n");
////#else
//            body.append("https://play.google.com/store/apps/details?id=")
//            .append(st_android_package_name).append("\n");
////#endif
////            
//#endif
//            
//            
//            STSystemFunction* sf = new STSystemFunction();
//            sf->sendEmail(subject.c_str(), body.c_str());
//            
//            CC_SAFE_DELETE(sf);
//        }
//            break;
//        case eSettingLayer_DeleteData_Item_Tag:
//        {
//            this->stopListener();
//            
//            std::map<string,string> lMessages=DataManager::getInstance()->getMessages();
//            STAlertDialog* lAlertDialog=STAlertDialog::create(lMessages[st_string_delete_content_key], lMessages[st_string_button_yes_key], lMessages[st_string_button_no_key]);
//            lAlertDialog->setLeftListener(this,callfuncN_selector(SettingDialog::deleteAllSavedData));
//            lAlertDialog->setRightListener(this, callfuncN_selector(TargetDialog::removeDialog));
//            this->addChild(lAlertDialog);
//            
//        }
//            break;
//        default:
//            break;
//    }
    
  
}

void SettingDialog::sliderChangeValue(STSlider *pSlider)
{
//    switch (pSlider->getTag()) {
//        case eSettingLayer_Slider_Sound_Tag:
//        {
//            AudioHelper::setSoundValue(pSlider->getValue());
//        }
//            break;
//        case eSettingLayer_Slider_Music_Tag:
//        {
//            AudioHelper::setMusicValue(pSlider->getValue());
//        }
//            break;
//        default:
//            break;
//    }
}

void SettingDialog::setDeleteDataItemVisible(bool pBool)
{
   if(!pBool)
   {
       MenuItemSprite* lDeleteDataItem=(MenuItemSprite*)_menu->getChildByTag(eSettingLayer_DeleteData_Item_Tag);
       lDeleteDataItem->setVisible(false);
       MenuItemSprite* lTellFriendItem=(MenuItemSprite*)_menu->getChildByTag(eSettingLayer_TellFriends_Item_Tag);
       lTellFriendItem->setPosition((lDeleteDataItem->getPosition()+lTellFriendItem->getPosition())/2);
   }
}

void SettingDialog::addTouchListener()
{
//    this->removeTocuhListener();
//    
//    auto dispatcher = Director::getInstance()->getEventDispatcher();
//    auto listener=EventListenerTouchOneByOne::create();
//    listener->onTouchBegan=CC_CALLBACK_2(SettingDialog::onTouchBegan,this);
//    listener->onTouchMoved=CC_CALLBACK_2(SettingDialog::onTouchMoved,this);
//    listener->onTouchEnded=CC_CALLBACK_2(SettingDialog::onTouchEnded,this);
//    listener->onTouchCancelled=CC_CALLBACK_2(SettingDialog::onTouchCancelled,this);
//    listener->setSwallowTouches(true);
//    dispatcher->addEventListenerWithSceneGraphPriority(listener,this);
//
//    _listener=listener;

}

bool SettingDialog::onTouchBegan(Touch *touch, Event *unused_event)
{
    Vec2 lPosition=touch->getLocationInView();
    lPosition=CCDirector::getInstance()->convertToGL(lPosition);
    
    _targetSlider=NULL;
    for (std::vector<STSlider*>::iterator iter=_sliders.begin(); iter!=_sliders.end(); iter++) {
        STSlider* lSlider=*iter;
        if(lSlider->isContainPosition(lPosition))
        {
            _targetSlider=lSlider;
            break;
        }
    }
    
    _startPoint=lPosition;
    _orginPoint=lPosition;
    return true;
}
void SettingDialog::onTouchMoved(Touch *touch, Event *unused_event)
{
    Vec2 lPosition=touch->getLocationInView();
    lPosition=CCDirector::getInstance()->convertToGL(lPosition);
    if(_targetSlider!=NULL)
    {
        float lDX=lPosition.x-_startPoint.x;
        _targetSlider->moveX(lDX);
        _startPoint=lPosition;
    }
}
void SettingDialog::onTouchEnded(Touch *touch, Event *unused_event)
{
    Vec2 lPosition=touch->getLocationInView();
    lPosition=CCDirector::getInstance()->convertToGL(lPosition);
    if(_targetSlider!=NULL)
    {
        _targetSlider->excuteBackData();
    }
}
void SettingDialog::onTouchCancelled(Touch *touch, Event *unused_event)
{

}

void SettingDialog::deleteAllSavedData(Node* pRef)
{
//    DataManager::getInstance()->deleteAllSavedData();
//    AudioHelper::reset(1.0f, 1.0f);
//    Director::getInstance()->replaceScene(HomeScene::scene());
}

void SettingDialog::removeDialog(Node* pRef)
{
    pRef->removeFromParentAndCleanup(true);
}

#pragma mark  - GameOverDioalog

bool GameOverDialog::init()
{
    if(!Dialog::init()) return false;
    
//    //清除ResumeGameInfo 数据
//    DataManager::getInstance()->clearResumeGameInfo();
//    
//    SaveGameData& lSaveGameData = DataManager::getInstance()->shareSaveGameData();
//    lSaveGameData.isAlreadySaveGame=false;
//    DataManager::getInstance()->excuteSaveGameData(st_savedata_isHaveSavedGame_bool, lSaveGameData.isAlreadySaveGame);
//
//    
//    _gameInfo=DataManager::getInstance()->shareGameInfo();
//    
//    AudioHelper::getInstance()->playSound(st_game_over_sound);
//    
//    this->loadResource();
//    this->loadUI();
    
    return true;
}

void GameOverDialog::loadResource()
{
//    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(st_ingame_plist);
//    //读取地图坐标信息
//    _nameVecMaps=DataManager::getInstance()->gameover_NameVecMaps;
}

void GameOverDialog::loadUI()
{
//    Sprite* lBoxSprite=ControlHelper::AddSprite(st_gameover_box_image, this);
//    ScreenAdapterHelper::setPosition(lBoxSprite, _nameVecMaps[st_gameover_box_image].position);
//    
//    Sprite* lTitleSprite=ControlHelper::addSpriteWithFrame(st_gameover_title_image, lBoxSprite);
//    ScreenAdapterHelper::setPosition(lTitleSprite, _nameVecMaps[st_gameover_title_image].position);
//    lTitleSprite->setPosition(lBoxSprite->convertToNodeSpace(lTitleSprite->getPosition()));
//    
//    Sprite* lTextBgSprite=ControlHelper::addSpriteWithFrame(st_gameover_textbg_image, lBoxSprite);
//    ScreenAdapterHelper::setPosition(lTextBgSprite, _nameVecMaps[st_gameover_textbg_image].position);
//    lTextBgSprite->setPosition(lBoxSprite->convertToNodeSpace(lTextBgSprite->getPosition()));
//    
//    Sprite* lScoreSprite=ControlHelper::addSpriteWithFrame(st_gameover_score_image, lBoxSprite);
//    ScreenAdapterHelper::setPosition(lScoreSprite, _nameVecMaps[st_gameover_score_image].position);
//    lScoreSprite->setPosition(lBoxSprite->convertToNodeSpace(lScoreSprite->getPosition()));
//    
//    MenuItemSprite* lMenuItemHome=ControlHelper::getMenuItemWithFrameName_NoSameSize(st_gameover_home_image, st_gameover_home_h_image, eGameOver_Home_Item_Tag, CC_CALLBACK_1(GameOverDialog::doAction, this));
//    ScreenAdapterHelper::setPosition(lMenuItemHome,_nameVecMaps[st_gameover_home_image].position);
//    lMenuItemHome->setPosition(lBoxSprite->convertToNodeSpace(lMenuItemHome->getPosition()));
//    
//    MenuItemSprite* lMenuItemReplay=ControlHelper::getMenuItemWithFrameName_NoSameSize(st_gameover_replay_image, st_gameover_replay_h_image, eGameOver_Replay_Item_Tag, CC_CALLBACK_1(GameOverDialog::doAction, this));
//    ScreenAdapterHelper::setPosition(lMenuItemReplay, _nameVecMaps[st_gameover_replay_image].position);
//    lMenuItemReplay->setPosition(lBoxSprite->convertToNodeSpace(lMenuItemReplay->getPosition()));
//    
//    _menu=Menu::create(lMenuItemHome,lMenuItemReplay, NULL);
//    lBoxSprite->addChild(_menu);
//    _menu->setPosition(Vec2(0, 0));
//    
//    Label* label=ControlHelper::getLabel(st_font_Helvetica_Neue, "", st_gameover_text_font_size);
//    lScoreSprite->addChild(label);
//    ScreenAdapterHelper::setPosition(label, _nameVecMaps[st_gameover_score_value_image].position);
//    label->setPosition(lScoreSprite->convertToNodeSpace(label->getPosition()));
//    _scoreLabel=label;
}

void GameOverDialog::setGameData(int pScore)
{
    stringstream ss;
    ss<<"";
    ss<<pScore;
    _scoreLabel->setString(ss.str());
}

void GameOverDialog::doAction(cocos2d::Ref *pSender)
{
//        _menu->setEnabled(false);
//    
//    Dialog::doAction(pSender);
//    MenuItemSprite* lMenuItem=(MenuItemSprite*)pSender;
//    switch (lMenuItem->getTag()) {
//        case eGameOver_Home_Item_Tag:
//        {
//            GameScene* lGameScene=(GameScene*)this->getParent();
//            lGameScene->releasePlist();
//            Director::getInstance()->replaceScene(HomeScene::scene());
//        }
//            break;
//        case eGameOver_Replay_Item_Tag:
//        {
//            GameInfo* lGameInfo=DataManager::getInstance()->shareGameInfo();
//            lGameInfo->m_time=st_level_time_orgin_data;
//  
//            
//            
//            //清除ResumeGameInfo 数据
//            DataManager::getInstance()->clearResumeGameInfo();
//            
//            GameScene* lGameScene=(GameScene*)this->getParent();
//            lGameScene->resetGame();
//            this->removeFromParentAndCleanup(true);
//            
//            
//           // Director::getInstance()->replaceScene(GameScene::scene());
//        }
//            break;
//
//        default:
//            break;
//    }
}

#pragma mark - GameComplete 
bool GameCompleteDialog::init()
{
    if(!Dialog::init()) return false;
    
//    _layerColor->stopAllActions();
//    _layerColor->setOpacity(0);
//    
//    _isResponseTouchEvent=true;
//    
//    NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(GameCompleteDialog::stopResponseTouch),st_stop_response_menu_touch_notification,NULL);
//    
//    NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(GameCompleteDialog::resumeResponseTouch),st_resume_response_menu_touch_notification,NULL);
//    
//    SaveGameData& lSaveGameData = DataManager::getInstance()->shareSaveGameData();
//    lSaveGameData.isAlreadySaveGame=false;
//    DataManager::getInstance()->excuteSaveGameData(st_savedata_isHaveSavedGame_bool, lSaveGameData.isAlreadySaveGame);
//    
//    //清除ResumeGameInfo 数据
//    DataManager::getInstance()->clearResumeGameInfo();
//    
//    AudioHelper::getInstance()->playSound(st_game_complete_sound);
//
//    this->recordFinishInfo();
//    this->loadResource();
//    
//    this->scheduleOnce(schedule_selector(GameCompleteDialog::deleyTimeLoad),0.5f);
    
    return true;
}

void GameCompleteDialog::deleyTimeLoad(float dt)
{
//    FadeTo* lFadeTo=FadeTo::create(1.0f, 180);
//    _layerColor->runAction(lFadeTo);
//    
//    this->loadUI();
//    this->loadRateUsInfo();
//    static int lFinishNumber=0;
//    lFinishNumber++;
//    
//    if(_isMayRateUs && lFinishNumber==5)
//    {
//        _isMayRateUs=false;
//        string message1 = DataManager::getInstance()->getMessages()[st_string_rate_us1_key];
//        string message2 = DataManager::getInstance()->getMessages()[st_string_rate_us2_key];
//        message1.append(", ").append(message2);
//        
//        STSystemFunction* sf = new STSystemFunction();
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//        sf->rating(apple_id, message1.c_str());
//#else
//        sf->rating();
//#endif
//        CC_SAFE_DELETE(sf);
//        
//    }
//    if(lFinishNumber==5)
//    {
//        lFinishNumber=0;
//        AdHelper::requestInterstitialAds();
//    }
//    
//    SaveGameData& lSaveGameData2=DataManager::getInstance()->shareSaveGameData();
//    long timestamp = STUtility::getCurrentTimeSeconds();
//    lSaveGameData2.recordGameStartTime=timestamp;
//    DataManager::getInstance()->excuteSaveGameData();
//
//    
//    _timeLabel->setString(this->getTimeStrWithTime(st_level_time_orgin_data-_time));
//    
//    stringstream ss;
//    ss.clear();
//    ss<<_score;
//    _scoreLabel->setString(ss.str());
//    
//    stringstream ss2;
//    ss2.clear();
//    ss2<<_highestScore;
//    _highestLabel->setString(ss2.str());
}

void GameCompleteDialog::stopResponseTouch(Ref* pSender)
{
    _isResponseTouchEvent=false;
    
}

void GameCompleteDialog::resumeResponseTouch(Ref* pSender)
{
    _isResponseTouchEvent=true;
}

void GameCompleteDialog::loadRateUsInfo()
{
//    SaveGameData& lSaveGameData=DataManager::getInstance()->shareSaveGameData();
//    long nowTimestamp = STUtility::getCurrentTimeSeconds();
//    long beforeTimestamp=lSaveGameData.recordGameStartTime;
//    int during = STUtility::getHoursByTimeInterval(beforeTimestamp, nowTimestamp);
//    if(lSaveGameData.isNeverRateUs)
//    {
//       if(during>=st_rateus_day_1)
//       {
//           _isMayRateUs=true;
//       }
//    }
//    else
//    {
//        if(during>=st_rateus_day_2)
//        {
//            _isMayRateUs=true;
//        }
//    }
}

void GameCompleteDialog::recordFinishInfo()
{
//    //开始下一关的Enable
//    GameInfo* lGameInfo =  DataManager::getInstance()->shareGameInfo();
//    levelInfo* info=lGameInfo->m_levelInfo;
//    if(info->nextLevelInfoInfo!=NULL)
//    {
//        info->nextLevelInfoInfo->levelData.isEnabled=true;
//    }
//    
//    switch (lGameInfo->m_gameMode) {
//        case K_GAME_MODE_PICTURE:
//        {
//            info->levelData.isFinishPictureMode=true;
//        }
//            break;
//        case K_GAME_MODE_SILHOUETTE:
//        {
//            info->levelData.isFinishSilhoutetteMode=true;
//        }
//            break;
//        case K_GAME_MODE_NAME:
//        {
//            info->levelData.isFinishWordMode=true;
//        }
//            break;
//        default:
//            break;
//    }
//    
//    info->levelData.starStarNum=0;
//    info->levelData.starStarNum+=info->levelData.isFinishPictureMode?1:0;
//    info->levelData.starStarNum+=info->levelData.isFinishSilhoutetteMode?1:0;
//    info->levelData.starStarNum+=info->levelData.isFinishWordMode?1:0;
    
}


void GameCompleteDialog::loadResource()
{
//    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(st_ingame_plist);
//    //读取地图坐标信息
//    _nameVecMaps=DataManager::getInstance()->gamecomplete_NameVecMaps;

}

void GameCompleteDialog::loadUI()
{
//    Sprite* lBoxSprite=ControlHelper::AddSprite(st_gamecomplete_box_image, this);
//    ScreenAdapterHelper::setPosition(lBoxSprite, _nameVecMaps[st_gamecomplete_box_image].position);
//    
//    Sprite* lTitleSprite=ControlHelper::addSpriteWithFrame(st_gamecomplete_title_image, lBoxSprite);
//    ScreenAdapterHelper::setPosition(lTitleSprite, _nameVecMaps[st_gamecomplete_title_image].position);
//    lTitleSprite->setPosition(lBoxSprite->convertToNodeSpace(lTitleSprite->getPosition()));
//    
//    Sprite* lTimeSprite=ControlHelper::addSpriteWithFrame(st_gamecomplete_time_image, lBoxSprite);
//    ScreenAdapterHelper::setPosition(lTimeSprite, _nameVecMaps[st_gamecomplete_time_image].position);
//    lTimeSprite->setPosition(lBoxSprite->convertToNodeSpace(lTimeSprite->getPosition()));
//    
//    Sprite* lScoreSprite=ControlHelper::addSpriteWithFrame(st_gamecomplete_score_image, lBoxSprite);
//    ScreenAdapterHelper::setPosition(lScoreSprite, _nameVecMaps[st_gamecomplete_score_image].position);
//    lScoreSprite->setPosition(lBoxSprite->convertToNodeSpace(lScoreSprite->getPosition()));
//    
//    Sprite* lHighScoreSprite=ControlHelper::addSpriteWithFrame(st_gamecomplete_highscore_image, lBoxSprite);
//    ScreenAdapterHelper::setPosition(lHighScoreSprite, _nameVecMaps[st_gamecomplete_highscore_image].position);
//    lHighScoreSprite->setPosition(lBoxSprite->convertToNodeSpace(lHighScoreSprite->getPosition()));
//    
//    Label* lTimeLabel=ControlHelper::getLabel(st_font_Helvetica_Neue, "", st_gamecomplete_text_font_size);
//    lBoxSprite->addChild(lTimeLabel);
//    ScreenAdapterHelper::setPosition(lTimeLabel, _nameVecMaps[st_gamecomplete_time_value_image].position);
//    lTimeLabel->setPosition(lBoxSprite->convertToNodeSpace(lTimeLabel->getPosition()));
//    _timeLabel=lTimeLabel;
//    
//    Label* lScoreLabel=ControlHelper::getLabel(st_font_Helvetica_Neue, "", st_gamecomplete_text_font_size);
//    ScreenAdapterHelper::setPosition(lScoreLabel, _nameVecMaps[st_gamecomplete_score_value_image].position);
//    lScoreLabel->setPosition(lBoxSprite->convertToNodeSpace(lScoreLabel->getPosition()));
//    lBoxSprite->addChild(lScoreLabel);
//    _scoreLabel=lScoreLabel;
//    
//    Label* lHighScoreLabel=ControlHelper::getLabel(st_font_Helvetica_Neue, "", st_gamecomplete_text2_font_size);
//    ScreenAdapterHelper::setPosition(lHighScoreLabel, _nameVecMaps[st_gamecomplete_highscore_value_image].position);
//    lHighScoreLabel->setPosition(lBoxSprite->convertToNodeSpace(lHighScoreLabel->getPosition()));
//    lBoxSprite->addChild(lHighScoreLabel);
//    _highestLabel=lHighScoreLabel;
//    
//    MenuItemSprite* lMenuItemHome=ControlHelper::getMenuItemWithFrameName_NoSameSize(st_gamecomplete_home_image, st_gamecomplete_home_h_image, eGameComplete_Home_Item_Tag, CC_CALLBACK_1(GameCompleteDialog::doAction, this));
//    ScreenAdapterHelper::setPosition(lMenuItemHome, _nameVecMaps[st_gamecomplete_home_image].position);
//    lMenuItemHome->setPosition(lBoxSprite->convertToNodeSpace(lMenuItemHome->getPosition()));
//    
//    MenuItemSprite* lMenuItemReplay=ControlHelper::getMenuItemWithFrameName_NoSameSize(st_gamecomplete_replay_image, st_gamecomplete_replay_h_image, eGameComplete_Replay_Item_Tag, CC_CALLBACK_1(GameCompleteDialog::doAction, this));
//    ScreenAdapterHelper::setPosition(lMenuItemReplay, _nameVecMaps[st_gamecomplete_replay_image].position);
//    lMenuItemReplay->setPosition(lBoxSprite->convertToNodeSpace(lMenuItemReplay->getPosition()));
//    
//    MenuItemSprite* lMenuItemNext=ControlHelper::getMenuItemWithFrameName_NoSameSize(st_gamecomplete_next_image, st_gamecomplete_next_h_image, eGameComplete_Next_Item_Tag, CC_CALLBACK_1(GameCompleteDialog::doAction, this));
//    ScreenAdapterHelper::setPosition(lMenuItemNext, _nameVecMaps[st_gamecomplete_next_image].position);
//    lMenuItemNext->setPosition(lBoxSprite->convertToNodeSpace(lMenuItemNext->getPosition()));
//    
//    _menu=Menu::create(lMenuItemHome,lMenuItemReplay,lMenuItemNext, NULL);
//    lBoxSprite->addChild(_menu);
//    _menu->setPosition(Vec2(0, 0));
//    
//    if(DataManager::getInstance()->shareGameInfo()->m_levelInfo->nextLevelInfoInfo==NULL)
//    {
//        lMenuItemNext->setVisible(false);
//        lMenuItemReplay->setPosition(lMenuItemNext->getPosition());
//    }
//    
    
}

void GameCompleteDialog::showData(int pTime,int pScore,int pHighestScore)
{
    _time=pTime;
    _score=pScore;
    _highestScore=pHighestScore;
    

}

string GameCompleteDialog::getTimeStrWithTime(int pTime)
{
    int lMinites=pTime/60;
    int lSecond=pTime%60;
    stringstream ss;
    if(lMinites>=10)
    {
        ss<<lMinites;
    }
    else
    {
        ss<<"0";
        ss<<lMinites;
    }
    ss<<":";
    if(lSecond>=10)
    {
        ss<<lSecond;
    }
    else
    {
        ss<<"0";
        ss<<lSecond;
    }
    return ss.str();
}


void GameCompleteDialog::doAction(cocos2d::Ref *pSender)
{
//    if(!_isResponseTouchEvent)
//    {
//        _isResponseTouchEvent=true;
//        return;
//    }
//    
//    _menu->setEnabled(false);
//    
//
//    Dialog::doAction(pSender);
//    MenuItemSprite* lMenuItem=(MenuItemSprite*)pSender;
//    switch (lMenuItem->getTag()) {
//        case eGameComplete_Home_Item_Tag:
//        {
//            GameScene* lGameScene=(GameScene*)this->getParent();
//            lGameScene->releasePlist();
//            Director::getInstance()->replaceScene(HomeScene::scene());
//        }
//            break;
//        case eGameComplete_Replay_Item_Tag:
//        {
//            GameInfo* lGameInfo=DataManager::getInstance()->shareGameInfo();
//            lGameInfo->m_time=st_level_time_orgin_data;
//            
//            //清除ResumeGameInfo 数据
//            DataManager::getInstance()->clearResumeGameInfo();
//            
//            GameScene* lGameScene=(GameScene*)this->getParent();
//            lGameScene->resetGame();
//            
//            this->removeFromParentAndCleanup(true);
//            
//            //Director::getInstance()->replaceScene(GameScene::scene());
//        }
//            break;
//        case eGameComplete_Next_Item_Tag:
//        {
//            
//            GameInfo* lGameInfo=DataManager::getInstance()->shareGameInfo();
//            levelInfo* info = lGameInfo->m_levelInfo->nextLevelInfoInfo;
//        
//            bool isLocked=info->levelData.isLocked;
//            if(isLocked)
//            {
//                GameScene* lGameScene=(GameScene*)this->getParent();
//                lGameScene->loadDulexDialog();
//            }
//            else
//            {
//                GameScene* lGameScene=(GameScene*)this->getParent();
//                lGameScene->releasePlist();
//
//                GameInfo* lGameInfo=DataManager::getInstance()->shareGameInfo();
//                //将下一关设置当前游戏关卡信息
//                lGameInfo->m_time=st_level_time_orgin_data;
//                lGameInfo->m_levelInfo=info;
//
//                
//                lGameScene->nextGame();
//                
//                this->removeFromParentAndCleanup(true);
//                
//                //Director::getInstance()->replaceScene(GameScene::scene());
//            }
//        }
//            break;
//        default:
//            break;
//    }
}

GameCompleteDialog::~GameCompleteDialog()
{
    NotificationCenter::getInstance()->removeObserver(this, st_stop_response_menu_touch_notification);
    NotificationCenter::getInstance()->removeObserver(this, st_resume_response_menu_touch_notification);
}
