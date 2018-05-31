//
//  LoadingScene.cpp
//  HIDD
//
//  Created by lidong on 15-3-12.
//
//

#include "LoadingScene.h"

#include "../helper/ControlHelper.h"
#include "../helper/ScreenAdapterHelper.h"
#include "../define/ResoureMacro.h"
#include "../define/GlobalMacro.h"
#include "../scene/GameScene.h"
#include "../scene/ChooseLevelScene.h"
#include "../game/DataManager.h"
#include "../helper/ImageDownload.h"
//#include "../helper/AdHelper.h"
#include "../helper/DeviceHelper.h"

using namespace ui;

bool LoadingScene::init()
{
    if(!BaseLayer::init()) return false;
    
    DataManager::getInstance()->initSetting();
    
    this->loadUI();
    
    return true;
}

//加载loading界面
void LoadingScene::loadUI()
{
    //加载loading界面的背景图片
    Sprite* lBgSprite=ControlHelper::AddSprite(st_loading_bg_image, this, 0, 0);
    ScreenAdapterHelper::setPosition(lBgSprite, st_screen_center_position);
    lBgSprite->setScale(ScreenAdapterHelper::getRealDesignScale());
    this->scheduleOnce(schedule_selector(LoadingScene::gotoNextScene), 1.0f);
}

void LoadingScene::gotoNextScene(float dt)
{

    
    
    Director::getInstance()->replaceScene(ChooseLevelScene::scene(1));
   

}
