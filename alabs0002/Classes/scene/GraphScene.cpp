//
//  GraphScene.cpp
//  BabyPlay
//
//  Created by luoxp on 3/18/15.
//
//

#include "GraphScene.h"
#include "UICpp.h"
#include "GraphSprite.hpp"
#include "ChapterManager.hpp"
#include "LockManager.h"
#include "DownloadManager.hpp"
#include "MyArtDataManager.hpp"

GraphScene::~GraphScene(){
    NotificationCenter::getInstance()->removeObserver(this, st_download_finish);
}

GraphScene::GraphScene()
{

    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(GraphScene::waitDownloadFinish), st_download_finish, NULL);
}

void GraphScene::waitDownloadFinish(Ref* ref)
{
    _tableView->reloadData();
}

bool GraphScene::init(){
    
    if(Scene::init())
    {
        setName("GraphScene");
        
        _root = (Layout*)(cocostudio::GUIReader::getInstance()->widgetFromJsonFile("graph.json"));
        addChild(_root);
        _root->cocos2d::Node::visit();
        
        Director::getInstance()->getTextureCache()->addImage("cocos/other/graph_bg.png");
        
        quickLayoutColor("back", _root, CC_CALLBACK_2(GraphScene::onButton, this));
        quickText("TXT_my_artwork", _root, CC_CALLBACK_2(GraphScene::onButton, this));
        quickText("TXT_inspiration", _root, CC_CALLBACK_2(GraphScene::onButton, this));
        
        _slider = (Slider*)Helper::seekNodeByName(_root, "slider");
        _slider->addEventListener(CC_CALLBACK_2(GraphScene::onSlider, this));
        _text = (Text*)Helper::seekNodeByName(_root, "slider_text");
        
        createTableView();
        
        auto tilte = (Text*)Helper::seekNodeByName(_root, "TXT_flower");
        tilte->setString(xStr->getStringDefault(xChapter->getCurrentChapter().name));
        tilte->setFontName(xStr->getBoldFontName());
        
        //translate and color
        xTrans("TXT_my_artwork");
        xTrans("TXT_inspiration");
        
//        scheduleOnce(schedule_selector(GraphScene::checkUpdate), 3);
        
        xAnalytics->sendScreenEvent("GraphScene");
        
        return true;
    }

    return false;
}

void GraphScene::checkUpdate(float dt)
{
    if (Director::getInstance()->getRunningScene() == this) {
        xDownload->excuteFunc(DownloadState::CHECK_NEW_VERSION_STATE);
    }
}

void GraphScene::onButton(Ref* pSender, Widget::TouchEventType type)
{
    if (type != Widget::TouchEventType::ENDED) return;
    
    string name = ((Node*)pSender)->getName();
    
    if(name.compare("back") == 0)
    {
        xChapter->getCurrentChapter().tableOffset = _tableView->getContentOffset().x;
        xScene->back();
    }
    else if(name == "TXT_my_artwork"){
        xMyArtData->m_bIsChapterEnterMyart = true;
        xScene->forward(SceneManager::Tag::MY_ARTS);
    }
    else if(name == "TXT_inspiration"){
        xScene->forward(SceneManager::Tag::INSPIRATION);
    }
}

void GraphScene::createTableView()
{
    _placeholder = (ListView*)Helper::seekNodeByName(_root, "listview");
    _tableView = TableView::create(this, Size(getContentSize().width, _placeholder->getContentSize().height));
    _tableView->setDirection(extension::ScrollView::Direction::HORIZONTAL);
    
    auto tablePos = _placeholder->getParent()->convertToNodeSpace(Vec2::ZERO);
    _tableView->setPosition(Vec2(tablePos.x, _placeholder->getPosition().y));
    _tableView->setDelegate(this);
    _placeholder->getParent()->addChild(_tableView);
    _tableView->reloadData();
    _placeholder->removeFromParent();
    _tableView->setContentOffset(Vec2(xChapter->getCurrentChapter().tableOffset, _tableView->getContentOffset().y));
}

void GraphScene::reloadData()
{
    xChapter->getCurrentChapter().tableOffset = _tableView->getContentOffset().x;
    xChapter->getCurrentChapter().enterGraph(_graphIndex);
    xChapter->getCurrentChapter().getCurrentGraph().setNew(false);
    xScene->forward(SceneManager::Tag::GAME);
}

void GraphScene::onDialog(const string& name)
{
    log("%s", xDialog->getUserString().c_str());
    if(name == "watch_ad"){
        xLock->unlock(xDialog->getUserString(), LockManager::Type::GRAPH);
        quickFlurry("watchAds_" + xDialog->getUserString());
    }
    else if (name == "new"){
        xChapter->getCurrentChapter().getCurrentGraph().setNew(true);
        xScene->forward(SceneManager::Tag::GAME);
    }
    else if (name == "continue"){
        xChapter->getCurrentChapter().getCurrentGraph().setNew(false);
        xScene->forward(SceneManager::Tag::GAME);
    }
}

void GraphScene::tableCellTouched(TableView* table, TableViewCell* cell)
{
    GraphSprite* lGraphSprite = dynamic_cast<GraphSprite*>(cell->getChildren().at(0));

    if (strcmp(lGraphSprite->m_gGraphInfo.name.substr(0, 3).c_str(), "map") != 0)
    {
        string userData = lGraphSprite->m_gGraphInfo.name;
        string aa = userData.substr(0, 4);
        int index = stoi(aa);
        
        stringstream ss;
        ss<<"";
        ss<<"map";
        ss<<index + 20;
        ss<<"_white.png";
        
        UserDefault::getInstance()->setBoolForKey(ss.str().c_str(), true);
        UserDefault::getInstance()->flush();
    }
    
    
    if (lGraphSprite->getCurrentGraphIslock())
    {
        _graphIndex = lGraphSprite->getTag();
        xDialog->show("watch_ad", "cancel", "watch_ad", CC_CALLBACK_1(GraphScene::onDialog, this));
        xDialog->setUserString(lGraphSprite->m_gGraphInfo.name);
    }
    else
    {
        xChapter->getCurrentChapter().tableOffset = _tableView->getContentOffset().x;
        xChapter->getCurrentChapter().enterGraph(lGraphSprite->getTag());
        
        auto & graph = xChapter->getCurrentChapter().getCurrentGraph();
        UserDefault::getInstance()->setBoolForKey(graph.name.c_str(), true);
        
        auto path = FileUtils::getInstance()->getWritablePath();
        bool isExsit = FileUtils::getInstance()->isFileExist(FileUtils::getInstance()->getWritablePath() + graph.getHistoryName());
        if (isExsit) {
            if (xMyArtData->isShared(graph.getPreviewName())) {
                xDialog->show("shared", "cancel", "new", CC_CALLBACK_1(GraphScene::onDialog, this));
            }
            else {
                xDialog->show("continue", "new", "continue", CC_CALLBACK_1(GraphScene::onDialog, this));
            }
        }
        else
        {
            graph.setNew(false);
            xScene->forward(SceneManager::Tag::GAME);
        }
    }
}

Size GraphScene::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    if (idx == 0)
    {
        return Size(24, 450);
    }else
    {
        return Size(500, 450);
    }
    
    
}

TableViewCell* GraphScene::tableCellAtIndex(TableView *table, ssize_t idx)
{
    TableViewCell* cell = table->dequeueCell();
    if (!cell)
    {
        cell = new TableViewCell();
        cell->autorelease();
    }
    cell->removeAllChildrenWithCleanup(true);
    
    if (idx == 0)
    {
        return cell;
    }
    
    Chapter & lChapterInfo = xChapter->getCurrentChapter();

    GraphSprite* lGraphSprite = GraphSprite::create(lChapterInfo.vGraphInfo.at(idx - 1));
    lGraphSprite->setTag(idx - 1);
    lGraphSprite->setPosition(Vec2(250, 235));
    lGraphSprite->display();
    cell->addChild(lGraphSprite);
    
    return cell;
}

ssize_t GraphScene::numberOfCellsInTableView(TableView *table)
{
    return xChapter->getCurrentChapter().vGraphInfo.size() + 1;
}

void GraphScene::scrollViewDidScroll(extension::ScrollView* view)
{
    auto percent = _tableView->getContentOffset().x / (_tableView->getViewSize().width - _tableView->getContainer()->getContentSize().width) * 100;
    if (percent < 0) {
        percent = 0;
    }
    else if (percent > 100) {
        percent = 100;
    }
    _slider->setPercent(percent);
    
    auto count = numberOfCellsInTableView(_tableView) - 1;
    int current = round((count - 1) * percent / 100) + 1;
    _text->setString(std::to_string(current) + "/" + std::to_string(count));
}

void GraphScene::onSlider(Ref *pSender, Slider::EventType type)
{
    if (type == Slider::EventType::ON_PERCENTAGE_CHANGED)
    {
        Slider* slider = dynamic_cast<Slider*>(pSender);
        int percent = slider->getPercent();
        auto offsetx = (_tableView->getViewSize().width - _tableView->getContainer()->getContentSize().width) * percent / 100;
        _tableView->setContentOffset(Vec2(offsetx, _tableView->getContentOffset().y));
        
        auto count = numberOfCellsInTableView(_tableView) - 1;
        int current = round((count - 1) * percent / 100) + 1;
        _text->setString(std::to_string(current) + "/" + std::to_string(count));
    }
}
