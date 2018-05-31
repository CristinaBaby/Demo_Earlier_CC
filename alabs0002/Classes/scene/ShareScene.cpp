//
//  ShareScene.cpp
//  BabyPlay
//
//  Created by luoxp on 3/18/15.
//
//

#include "ShareScene.h"
#include "UICpp.h"
#include "GraphManager.hpp"
#include "STSystemFunction.h"
#include "ColorAdsManager.h"
#include "ChapterManager.hpp"
#include "STSystemFunction.h"
#include "STInstagramFunction.h"
#include <sys/stat.h>
#include "DownLoadPage.h"
#include "MyArtDataManager.hpp"
#include "RuntimePermissionManager.h"


ShareScene::~ShareScene(){
    NotificationCenter::getInstance()->removeObserver(this, st_myPost_failed);
    NotificationCenter::getInstance()->removeObserver(this, st_myPost_success);
}

ShareScene::ShareScene():_graph(xChapter->getCurrentChapter().getCurrentGraph()){
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(ShareScene::waitPostSuccessful), st_myPost_success, NULL);
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(ShareScene::waitPostFaild), st_myPost_failed, NULL);
}

bool ShareScene::init(){
    
    if(Scene::init())
    {
        auto _root = (Layout*)(cocostudio::GUIReader::getInstance()->widgetFromJsonFile("share.json"));
        addChild(_root);
        _root->cocos2d::Node::visit();
        
        _back = quickLayout("back", _root, CC_CALLBACK_2(ShareScene::onButton, this));
        quickLayout("home", _root, CC_CALLBACK_2(ShareScene::onButton, this));
        quickButton("save", _root, CC_CALLBACK_2(ShareScene::onButton, this));
        quickButton("share", _root, CC_CALLBACK_2(ShareScene::onButton, this));
        _inspiration = quickText("TXT_post_inspiration", _root, CC_CALLBACK_2(ShareScene::onButton, this));

        //修正 canvas_bg的坐标
        auto canvas_bg = (Layout*)Helper::seekNodeByName(_root, "canvas_bg");
        canvas_bg->setAnchorPoint(Vec2(0.5, 0.5));
        canvas_bg->ignoreAnchorPointForPosition(false);
        
        auto up_ui = Helper::seekNodeByName(_root, "up_ui");
        auto bottom_ui = Helper::seekNodeByName(_root, "bottom_ui");
        
        auto height = up_ui->getPositionY() - bottom_ui->getPositionY() - bottom_ui->getContentSize().height;
        auto offset = Vec2(canvas_bg->getContentSize().width - getContentSize().width, canvas_bg->getContentSize().height - height);
        canvas_bg->setContentSize(Size(getContentSize().width, height));
        canvas_bg->setPosition(canvas_bg->getPosition() + offset);
        
        auto paper = canvas_bg->getChildren().at(0);
        paper->setPosition(paper->getPosition() - offset/2);
        
        _canvas = Helper::seekNodeByName(_root, "canvas");
        auto preview = Sprite::create(_graph.getHistoryName());
        preview->setScale(quickAdaptScale(preview, _canvas));
        preview->setPosition(_canvas->getContentSize()/2);
        _canvas->addChild(preview);
        
        m_sPostImage = _graph.getHistoryName();
        
        //translate and color
        xTrans("TXT_post_inspiration");
        
        if (xMyArtData->isShared(_graph.getPreviewName()))
        {
            _back->setVisible(false);
            _inspiration->setVisible(false);
            ((Node*)_inspiration->getUserData())->setVisible(false);
        }
        
        xAnalytics->sendScreenEvent("ShareScene");
        
        return true;
    }

    return false;
}

void ShareScene::onButton(Ref* pSender, Widget::TouchEventType type)
{
    if (type != Widget::TouchEventType::ENDED) return;
    
    string name = ((Node*)pSender)->getName();
    
    if(name == "back")
    {
        _graph.setNew(false);
        xScene->back();
        xColorAds->changeScene();
    }
    else if(name == "home")
    {
        xGraph->destorySaveImage();
        xScene->back(SceneManager::Tag::CHAPTER);
        xColorAds->changeScene();
        quickFlurry("btn_backtohome");
    }
    else if(name == "save")
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        STSystemFunction function = STSystemFunction();
        function.saveLocalImage(xGraph->getSaveImage());
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        RuntimePermissionManager::getInstance()->onPermissionGrantedResult = [&](int requestcode,bool bgranted){
            onPermissionGrantedResult(requestcode, bgranted);
        };
        RuntimePermissionManager::getInstance()->requestRuntimePermissions(1, PERMISSION::kWriteExternalStorage);
#endif
        
        xColorAds->share();
        quickFlurry("share_save");
    }
    else if (name == "share")
    {
        auto saveImage = xGraph->getSaveImage();
        auto texture = new Texture2D();
        texture->initWithImage(saveImage);
        texture->autorelease();
        auto image = Sprite::createWithTexture(texture);
        image->setPosition(512, 512);
        
        Vec2 lOrginPos = _canvas->getPosition();
        RenderTexture* lRenderTexture = RenderTexture::create(1024, 1024);
        lRenderTexture->begin();
        image->visit();
        lRenderTexture->end();
        
        Director::getInstance()->getRenderer()->render();
        
        STSystemFunction sf;
        Image* lImage = lRenderTexture->newImage();
        string lPath;
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        lPath = FileUtils::getInstance()->getWritablePath() + "share_alabs0002.png";
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        lPath = sf.getSdCardPath()+"/"+"share_alabs0002.png";
#endif
        lImage->saveToFile(lPath, false);
        lImage->autorelease();
        
        Director::getInstance()->getTextureCache()->removeTextureForKey(lPath.c_str());
        _canvas->setPosition(lOrginPos);

        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        sf.sendPictureToMore(lPath.c_str());
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        STInstagramFunction st;
        st.sendImageToInstagram(lPath.c_str());
#endif
        
        xColorAds->share();
        quickFlurry("share_share");
    }else if (name == "TXT_post_inspiration")
    {
        xDialog->show("post to insprision", "later", "post", CC_CALLBACK_1(ShareScene::onDialog, this));
    }
}

void ShareScene::onDialog(const string& name)
{
    if (name == "post"){
        _back->setVisible(false);
        _inspiration->setVisible(false);
        ((Node*)_inspiration->getUserData())->setVisible(false);
        
        SwallowListenerLayer* lSwallowLayer = SwallowListenerLayer::create(Color4B(0, 0, 0, 200));
        this->addChild(lSwallowLayer, 100);
        lSwallowLayer->setTouchEnable(true);
        m_pPostingLayer = lSwallowLayer;
        
        Label* label = Label::createWithTTF("Posting...", "fonts/Seravek.ttf", 50);
        label->setPosition(Vec2(lSwallowLayer->getContentSize().width/2, lSwallowLayer->getContentSize().height/2));
        lSwallowLayer->addChild(label);
        
        runAction(Sequence::create(DelayTime::create(0.1), CallFunc::create([&](){
            DownLoadPage::getInstance()->postImage(m_sPostImage);
            
        }), nullptr));
    }else if (name == "later")
    {
        if (m_pPostingLayer)
        {
            m_pPostingLayer->removeFromParentAndCleanup(true);
            m_pPostingLayer = NULL;
        }
    }else if (name == "tryAgain")
    {
        SwallowListenerLayer* lSwallowLayer = SwallowListenerLayer::create(Color4B(0, 0, 0, 200));
        this->addChild(lSwallowLayer, 100);
        lSwallowLayer->setTouchEnable(true);
        m_pPostingLayer = lSwallowLayer;
        
        Label* label = Label::createWithTTF("Posting...", "fonts/Seravek.ttf", 50);
        label->setPosition(Vec2(lSwallowLayer->getContentSize().width/2, lSwallowLayer->getContentSize().height/2));
        lSwallowLayer->addChild(label);
        
        runAction(Sequence::create(DelayTime::create(0.1), CallFunc::create([&](){
            
            DownLoadPage::getInstance()->postImage(m_sPostImage);
            
        }), nullptr));
    }else if (name == "go_ins")
    {
        xScene->forward(SceneManager::Tag::INSPIRATION);
    }
}

void ShareScene::waitPostFaild(Ref* ref)
{
    
    if (m_pPostingLayer)
    {
        m_pPostingLayer->removeFromParentAndCleanup(true);
        m_pPostingLayer = NULL;
    }
    
    xDialog->show("post fail", "later", "tryAgain", CC_CALLBACK_1(ShareScene::onDialog, this));
    
}

void ShareScene::waitPostSuccessful(Ref* ref)
{
    if (m_pPostingLayer)
    {
        m_pPostingLayer->removeFromParentAndCleanup(true);
        m_pPostingLayer = NULL;
    }
    
    xDialog->show("post to insprision", "later", "go_ins", CC_CALLBACK_1(ShareScene::onDialog, this));
    
    quickFlurry(string("insprision_") + _graph.name);
}


void ShareScene::onPermissionGrantedResult(int requestCode,bool bGranted) {
#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
    if (requestCode == 1) {
        if (bGranted) {
            //add your code....
            log("-------->anroid runtime permisson was granted,requestcode = %d",requestCode);
            STSystemFunction function = STSystemFunction();
            auto writePath = function.getSdCardPath()+"/Color Book/";
            
            if(!FileUtils::sharedFileUtils()->isFileExist(writePath))
            {
                mode_t processMask = umask(0);
                int ret = mkdir(writePath.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
                umask(processMask);
                if (ret != 0 && (errno != EEXIST))
                    return;
            }
            
            struct timeval now;
            gettimeofday(&now, nullptr);
            std::string _time = StringUtils::format("%ld", (now.tv_sec * 1000 + now.tv_usec / 1000));
            std::string fullPath = writePath + _time + ".jpg";
            
            bool isSaveSuccess = xGraph->getSaveImage()->saveToFile(fullPath.c_str());
            if(isSaveSuccess)
                function.makeToast("Saved to Camera Roll!");
            else
                function.makeToast("Allow access to your album in \"Setting\"->\"Privacy\"->\"Photos\"");
            function.refreshDCIM(fullPath);
        }else{
            //add your code....
            log("-------->anroid runtime permisson was not  granted,requestcode = %d",requestCode);
        }
    }
#endif
}

