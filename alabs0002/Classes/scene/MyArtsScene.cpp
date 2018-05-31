//
//  MyArtsScene.cpp
//  BabyPlay
//
//  Created by luoxp on 3/18/15.
//
//

#include "MyArtsScene.h"
#include "UICpp.h"
#include "MyArtSprite.hpp"
#include "MyArtDataManager.hpp"
#include "ChapterManager.hpp"
#include "STSystemFunction.h"
#include "DownLoadPage.h"

enum{
    st_tableCell_leftChild_tag = 11,
    st_tableCell_rightChild_tag,

};

MyArtsScene::~MyArtsScene(){
    xMyArtData->saveMyArtData();
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
    NotificationCenter::getInstance()->removeObserver(this, st_deleteMyArtSprite);
}

MyArtsScene::MyArtsScene()
{
    _offset = 0;
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(MyArtsScene::reponseDeleteMyArt), st_deleteMyArtSprite, NULL);
}

bool MyArtsScene::init(){
    
    if(Scene::init())
    {
        _root = (Layout*)(cocostudio::GUIReader::getInstance()->widgetFromJsonFile("my_arts.json"));
        addChild(_root);
        _root->cocos2d::Node::visit();
        
        _placeholder = (Layout*)Helper::seekNodeByName(_root, "listview");
        _placeholder->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
        _placeholder->setBackGroundColor(Color3B::GRAY);
        
        quickLayoutColor("back", _root, CC_CALLBACK_2(MyArtsScene::onButton, this));
        
        Director::getInstance()->getTextureCache()->addImage("cocos/my_arts/paper.png");
        
        createTableView();
        
        xMyArtData->m_bIsChapterEnterMyart = false;
        
        xAnalytics->sendScreenEvent("MyArtsScene");
        
        return true;
        
    }

    return false;
}

void MyArtsScene::onButton(Ref* pSender, Widget::TouchEventType type)
{
    if (type != Widget::TouchEventType::ENDED) return;
    
    string name = ((Node*)pSender)->getName();
    
    if (name == "back") {
        
        if (xScene->getLastSceneTag() == SceneManager::Tag::SHARE) {
            auto graph = xChapter->getCurrentChapter().getCurrentGraph();
            auto path = FileUtils::getInstance()->getWritablePath() + graph.getPreviewName().c_str();
            if (!FileUtils::getInstance()->isFileExist(path)) {
                xScene->back(SceneManager::Tag::CHAPTER);
                return;
            }
        }
        
        xScene->back();
    }
}

void MyArtsScene::createTableView()
{
    auto up_ui = Helper::seekNodeByName(_root, "up_ui");
    _tableView = STTableView::create(this, Size(getContentSize().width, up_ui->getPositionY()));
    _tableView->setDirection(extension::ScrollView::Direction::VERTICAL);
    
    _tableView->setDelegate(this);
    _tableView->setDirection(cocos2d::extension::ScrollView::Direction::VERTICAL);
    _placeholder->getParent()->addChild(_tableView);
    _tableView->reloadData();
    _placeholder->removeFromParent();
    _tableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    _tableView->setColor(Color3B::RED);
}

void MyArtsScene::reponseDeleteMyArt(Ref* ref)
{
    float offset = _tableView->getContentOffset().x;
    _tableView->reloadData();
    if (offset < _tableView->minContainerOffset().x)
    {
        _tableView->setContentOffset(_tableView->minContainerOffset());
    }else if (offset > _tableView->maxContainerOffset().x)
    {
        _tableView->setContentOffset(_tableView->maxContainerOffset());
    }else
    {
        _tableView->setContentOffset(Vec2(offset, _tableView->getContentOffset().y));
    }
}

void MyArtsScene::tableCellTouched(TableView* table, TableViewCell* cell)
{
    
//    MyArtSprite* lSprite = dynamic_cast<MyArtSprite*>(cell->getChildren().at(0));
//    if (lSprite)
//    {
//        xChapter->enterGraph(lSprite->getSpriteName());
//        xScene->forward(SceneManager::Tag::GAME);
//    }
 
}

void MyArtsScene::reponseSprite(Node* node)
{
    MyArtSprite* lSprite = dynamic_cast<MyArtSprite*>(node);
    m_pMyartSprite = lSprite;
    if (lSprite)
    {
        if (xMyArtData->isShared(lSprite->getPreName()))
        {
            xDialog->show("shared", "cancel", "new", CC_CALLBACK_1(MyArtsScene::onDialog, this));
        }else
        {
            xChapter->enterGraph(lSprite->getSpriteName());
            Graph & graph = xChapter->getCurrentChapter().getCurrentGraph();
            graph.setBGColor(lSprite->getBGColor());
            
            log("%d,%d,%d",lSprite->getBGColor().r,lSprite->getBGColor().g,lSprite->getBGColor().b);
            graph.setIndex(lSprite->getPreName());
            xScene->forward(SceneManager::Tag::GAME);
        }
        
    }
}

void MyArtsScene::enterNextScene()
{
    xChapter->enterGraph(m_pMyartSprite->getSpriteName());
    Graph & graph = xChapter->getCurrentChapter().getCurrentGraph();
    graph.setNew(true);
    xScene->forward(SceneManager::Tag::GAME);
}

void MyArtsScene::onDialog(const string& name)
{
    if(name == "new"){
        this->enterNextScene();
    }
    else if (name == "cancel"){
        
    }
}

Size MyArtsScene::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    log("%lu",xMyArtData->m_vMyArtDatas.size());
    
    if ((xMyArtData->m_vMyArtDatas.size() + 1) / 2 < 3)
    {
        return Size(600, 330);
    }else
    {
        _offset = 1;
    }
    
    if ((((xMyArtData->m_vMyArtDatas.size()+1) / 2) + _offset) - 1 == idx)
    {
        return Size(600, 150);
    }else
    {
        return Size(600, 330);
    }
    
}

TableViewCell* MyArtsScene::tableCellAtIndex(TableView *table, ssize_t idx)
{
    TableViewCell* cell = table->dequeueCell();
    if (!cell)
    {
        cell = new TableViewCell();
        cell->autorelease();
    }
    
    
    cell->removeAllChildrenWithCleanup(true);
    
    
    int index = (int)((xMyArtData->m_vMyArtDatas.size()+1) / 2 + _offset - 1);
    
    if (idx == index && (_offset == 1))
    {
        return cell;
    }
    
    Color3B color;
    color.r = xMyArtData->m_vMyArtDatas.at(xMyArtData->m_vMyArtDatas.size() - 2*idx - 1).bgColor_R;
    color.g = xMyArtData->m_vMyArtDatas.at(xMyArtData->m_vMyArtDatas.size() - 2*idx - 1).bgColor_G;
    color.b = xMyArtData->m_vMyArtDatas.at(xMyArtData->m_vMyArtDatas.size() - 2*idx - 1).bgColor_B;
    
    MyArtSprite* lSprite = MyArtSprite::create(xMyArtData->m_vMyArtDatas.at(xMyArtData->m_vMyArtDatas.size() - 2*idx - 1).name, color, xMyArtData->m_vMyArtDatas.at(xMyArtData->m_vMyArtDatas.size() - 2*idx - 1).serveId);
    lSprite->setPreName(xMyArtData->m_vMyArtDatas.at(xMyArtData->m_vMyArtDatas.size() - 2*idx - 1).previewName);
    lSprite->setPosition(Vec2(this->getContentSize().width / 4 + 20, 160));
    lSprite->setScale(0.55f);
    lSprite->display();
    lSprite->setTag(st_tableCell_leftChild_tag);
    cell->addChild(lSprite);
    lSprite->addTouchListener(this, callfuncN_selector(MyArtsScene::reponseSprite), false);


    int range = (int)(xMyArtData->m_vMyArtDatas.size() - 2*idx - 2);
    
    if (range >= 0)
    {
        Color3B color;
        color.r = xMyArtData->m_vMyArtDatas.at(xMyArtData->m_vMyArtDatas.size() - 2*idx - 2).bgColor_R;
        color.g = xMyArtData->m_vMyArtDatas.at(xMyArtData->m_vMyArtDatas.size() - 2*idx - 2).bgColor_G;
        color.b = xMyArtData->m_vMyArtDatas.at(xMyArtData->m_vMyArtDatas.size() - 2*idx - 2).bgColor_B;
        
        MyArtSprite* lSprite2 = MyArtSprite::create(xMyArtData->m_vMyArtDatas.at(xMyArtData->m_vMyArtDatas.size() - 2*idx - 2).name, color, xMyArtData->m_vMyArtDatas.at(xMyArtData->m_vMyArtDatas.size() - 2*idx - 2).serveId);
        lSprite2->setPreName(xMyArtData->m_vMyArtDatas.at(xMyArtData->m_vMyArtDatas.size() - 2*idx - 2).previewName);
        lSprite2->setPosition(Vec2(this->getContentSize().width * 3/4 - 20, 160));
        lSprite2->setScale(0.55f);
        lSprite2->display();
        lSprite2->setTag(st_tableCell_rightChild_tag);
        cell->addChild(lSprite2);
        lSprite2->addTouchListener(this, callfuncN_selector(MyArtsScene::reponseSprite), false);
    }

    return cell;
}

ssize_t MyArtsScene::numberOfCellsInTableView(TableView *table)
{
    return (int)(((xMyArtData->m_vMyArtDatas.size()+1) / 2) + _offset);
}

void MyArtsScene::tableCellRemoveTexture(TableView* table, TableViewCell* cell)
{
    MyArtSprite* lSprite1 = dynamic_cast<MyArtSprite*>(cell->getChildByTag(st_tableCell_leftChild_tag));
    if (lSprite1)
    {
        lSprite1->removeTexture();
    }
    
    MyArtSprite* lSprite2 = dynamic_cast<MyArtSprite*>(cell->getChildByTag(st_tableCell_rightChild_tag));
    if (lSprite2)
    {
        lSprite2->removeTexture();
    }
}
