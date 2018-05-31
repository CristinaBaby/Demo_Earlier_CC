//
//  ChapterScene.cpp
//  BabyPlay
//
//  Created by luoxp on 3/18/15.
//
//

#include "ChapterScene.h"
#include "UICpp.h"
#include "ChapterManager.hpp"
#include "MyArtDataManager.hpp"
#include "DownloadManager.hpp"
#include "UIListViewEx.h"
#include "STSystemFunction.h"
#include "LockManager.h"
#include "DownLoadPage.h"

#define OFF_SET                 (Vec2(0, -3))

ChapterScene::~ChapterScene(){
    NotificationCenter::getInstance()->removeObserver(this, st_updateProgress_message);
    NotificationCenter::getInstance()->removeObserver(this, st_download_finish);
}

ChapterScene::ChapterScene()
{
    m_vProgress.clear();
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(ChapterScene::updateProgress), st_updateProgress_message, NULL);
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(ChapterScene::waitDownloadFinish), st_download_finish, NULL);
}

bool ChapterScene::init(){
    
    if(Scene::init())
    {
        setName("ChapterScene");
        
        _root = (Layout*)(cocostudio::GUIReader::getInstance()->widgetFromJsonFile("chapter.json"));
        addChild(_root);
        _root->cocos2d::Node::visit();
        
        quickText("TXT_my_artwork", _root, CC_CALLBACK_2(ChapterScene::onButton, this));
        quickText("TXT_inspiration", _root, CC_CALLBACK_2(ChapterScene::onButton, this));
        
        _slider = (Slider*)Helper::seekNodeByName(_root, "slider");
        _slider->addEventListener(CC_CALLBACK_2(ChapterScene::onSlider, this));
        _text = (Text*)Helper::seekNodeByName(_root, "slider_text");
        
        //create list view
        _listview = (ListView*)Helper::seekNodeByName(_root, "listview");
        _listview->setClippingEnabled(false);
        _listview->setPositionX(-getContentSize().width/2);
        _listview->setContentSize(Size(getContentSize().width, _listview->getContentSize().height));
        createListView();
        
        auto listener = EventListenerKeyboard::create();
        listener->onKeyReleased = [this](EventKeyboard::KeyCode code, Event* event)
        {
            if(code == EventKeyboard::KeyCode::KEY_BACK)
            {
                xDialog->show("quit", "cancel", "ok", CC_CALLBACK_1(ChapterScene::onDialog, this));
            }
        };
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
        
        //translate and color
        xTrans("TXT_gallery");
        xTrans("TXT_my_artwork");
        xTrans("TXT_inspiration");
        
        checkTableOffset();
        
        xAnalytics->sendScreenEvent("ChapterScene");
        
        this->scheduleOnce(schedule_selector(ChapterScene::checkUpdate), 2.0);
        
//        auto eventRateUs = EventListenerCustom::create("chapter_rateUs", CC_CALLBACK_1(ChapterScene::showRateUs, this));
//        _eventDispatcher->addEventListenerWithSceneGraphPriority(eventRateUs, this);
        
        
        
        return true;
    }

    return false;
}

void ChapterScene::showRateUs(EventCustom* event)
{
    STSystemFunction sf;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    sf.rating("1040798228", "Enjoyed our app? We are looking forward to your encouragements!^_^");
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    sf.rating();
#endif
}

void ChapterScene::checkUpdate(float dt)
{
    
    if (Director::getInstance()->getRunningScene() == this) {
        if (xLock->_isShowRateUs)
        {
            xLock->_isShowRateUs = false;
            this->showRateUs(NULL);
        }
        xDownload->excuteFunc(DownloadState::CHECK_NEW_VERSION_STATE);
    }
}

void ChapterScene::onDialog(const string& name)
{
    if(name == "ok"){
        
        xMyArtData->saveMyArtData();
        DownLoadPage::getInstance()->saveClickLikeInfo();
        xAudio->stopMusic();
        Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        exit(0);
#endif
    }
}

void ChapterScene::onButton(Ref* pSender, Widget::TouchEventType type)
{
    if (type != Widget::TouchEventType::ENDED) return;
    
    if (((Node*)pSender)->getTag() == 1) return;

    string name = ((Node*)pSender)->getName();
    
    if (name == "chapter") {
        xChapter->enterChapter(_listview->getCurSelectedIndex() - 1);
        
        xUser->setBoolForKey(xChapter->getCurrentChapter().name.c_str(), false);
        xScene->forward(SceneManager::Tag::GRAPH);
    }
    else if(name == "TXT_my_artwork"){
        xMyArtData->m_bIsChapterEnterMyart = true;
        xScene->forward(SceneManager::Tag::MY_ARTS);
    }
    else if(name == "TXT_inspiration"){
        xScene->forward(SceneManager::Tag::INSPIRATION);
    }
    
    xChapter->setTableOffset(_listview->getInnerContainerPosition().x);
}

void ChapterScene::createListView()
{
    auto & data = xChapter->getData();

    //Indent
    auto before = Layout::create();
    before->setContentSize(Size(0, _listview->getContentSize().height));
    _listview->pushBackCustomItem(before);
    
    for(int i = 0; i < data.size(); i ++)
    {
        auto layout = Layout::create();
        auto vGraphInfo = data.at(i).vGraphInfo;
        
        auto background = LayerColor::create(Color4B::WHITE, 440, 440);
        auto frontground = ImageView::create("other/frame2.png");
        frontground->setPosition(Vec2(background->getContentSize()/2) + OFF_SET);
        frontground->setScaleY(1.01);
        layout->addChild(frontground, 1);
        
        frontground->setColor(data.at(i).frame);
        _listview->pushBackCustomItem(layout);
        
        if (data.at(i).isDownloading())
        {
            Sprite* progress_bg = Sprite::create("other/down-37.png");
            progress_bg->setPosition(Vec2(background->getContentSize().width/2, background->getContentSize().height / 2));
            background->addChild(progress_bg, 10);
            
            Sprite* lSprite = Sprite::create("other/down-gray.png");
            lSprite->setPosition(Vec2(progress_bg->getContentSize().width/2 + 0.5, progress_bg->getContentSize().height/2 + 3.2));
            progress_bg->addChild(lSprite);
            
            ProgressTimer* lProgress = ProgressTimer::create(Sprite::create("other/down-black.png"));
            lProgress->setPosition(Vec2(progress_bg->getContentSize().width/2 + 0.5, progress_bg->getContentSize().height/2 + 3.2));
            lProgress->setType(cocos2d::ProgressTimer::Type::RADIAL);
            progress_bg->addChild(lProgress, 10);
//            lProgress->setMidpoint(Vec2(0, 0.5));
//            lProgress->setBarChangeRate(Vec2(1, 0));
            lProgress->setReverseDirection(false);
            lProgress->setPercentage(data.at(i).getPercent());
            lProgress->setTag(i);
            
            m_vProgress.push_back(lProgress);
        }
        

        //添加图片
        //graph->setScale((background->getContentSize().width - OFF_SET.x) / graph->getContentSize().width * 0.9);
        auto size = Size(Vec2(background->getContentSize()) - OFF_SET);
        m_sLayoutSize = size;
        
        layout->setContentSize(size);
        layout->addChild(background);
        
        if (!data.at(i).isDownloading() && !vGraphInfo.empty()) {

            auto graph = ImageView::create(data.at(i).image);
            graph->setName("thumb");
            graph->setPosition(Vec2(size / 2) + OFF_SET);
            graph->setScale(quickAdaptScale(graph, background));
            layout->addChild(graph);
        }
        
        //添加Text
        string textContent = xStr->getStringDefault(data.at(i).name);
        auto title = Text::create(textContent, FONT_NAME, 47);
        title->setPosition(Vec2(frontground->getContentSize().width * 0.5, frontground->getContentSize().height * 0.28));
        title->setFontName(xStr->getBoldFontName());
        frontground->addChild(title, 1);
        
        //添加New
        
//        string userData = data.at(i).name + "_white.png";
        
        if (UserDefault::getInstance()->getBoolForKey(data.at(i).name.c_str(), false))
        {
            Sprite* lSprite = Sprite::create("other/new.png");
            lSprite->setAnchorPoint(Vec2(1, 1));
            lSprite->setPosition(layout->getContentSize() + Size(-11, -14));
            layout->addChild(lSprite, 2);
        }
        
        if (data.at(i).isNewChapter)
        {
            layout->setTag(1);
        }else
        {
            layout->setTag(0);
        }
        layout->setName("chapter");
        layout->setTouchEnabled(true);
        layout->addTouchEventListener(CC_CALLBACK_2(ChapterScene::onButton, this));
    }
    
    //Indent
    auto after = Layout::create();
    after->setContentSize(Size(0, _listview->getContentSize().height));
    _listview->pushBackCustomItem(after);
    _listview->setScrollBarEnabled(false);
    _listview->addEventListener((ui::ListView::ccScrollViewCallback)CC_CALLBACK_2(ChapterScene::onListView,this));
}

void ChapterScene::updateProgress(Ref* ref)
{
    for (int i = 0; i < m_vProgress.size(); ++i)
    {
        ProgressTimer* lTimer = m_vProgress.at(i);
        lTimer->setPercentage(xChapter->getData().at(lTimer->getTag()).getPercent());
        log("%d",lTimer->getTag());
        if (xChapter->getData().at(lTimer->getTag()).getPercent() >= 100)
        {
            xChapter->getData().at(lTimer->getTag()).isNewChapter = false;
            this->waitDownloadFinish(NULL);
        }
    }
}

void ChapterScene::waitDownloadFinish(Ref* ref)
{
    m_vProgress.clear();
    _listview->removeAllChildren();
    createListView();
}
void ChapterScene::onListView(Ref* pSender, ui::ScrollView::EventType type)
{
    if (type == ui::ScrollView::EventType::CONTAINER_MOVED){
        auto percent = _listview->getInnerContainerPosition().x / (_listview->getContentSize().width - _listview->getInnerContainerSize().width) * 100;
        if (percent < 0) {
            percent = 0;
        }
        else if (percent > 100) {
            percent = 100;
        }
        _slider->setPercent(percent);
        
        auto count = _listview->getItems().size() - 2;
        int current = round((count - 1) * percent / 100) + 1;
        _text->setString(std::to_string(current) + "/" + std::to_string(count));
    }
}

void ChapterScene::onSlider(Ref *pSender, Slider::EventType type)
{
    if (type == Slider::EventType::ON_PERCENTAGE_CHANGED)
    {
        Slider* slider = dynamic_cast<Slider*>(pSender);
        int percent = slider->getPercent();
        _listview->jumpToPercentHorizontal(percent);
        
        auto count = _listview->getItems().size() - 2;
        int current = round((count - 1) * percent / 100) + 1;
        _text->setString(std::to_string(current) + "/" + std::to_string(count));
    }
}

void ChapterScene::checkTableOffset()
{
    _oldX = _listview->getPositionX();
    _listview->setPositionX(_oldX + xChapter->getTableOffset());
    scheduleOnce(schedule_selector(ChapterScene::refresh), 0);
}

void ChapterScene::refresh(float dt)
{
    _listview->setPositionX(_oldX);
    
    //这个类只是为了使用protected的方法
    ListViewEx listViewEx(_listview);
    listViewEx.jumpToDestinationEx(Vec2(xChapter->getTableOffset(), 0));
}
