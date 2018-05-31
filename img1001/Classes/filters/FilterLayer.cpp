//
//  FilterLayer.cpp
//  TestShader
//
//  Created by lidong on 15/9/8.
//
//

#include "FilterLayer.h"

enum
{
    eOverLay_Fliter_Tag=1,
    eRouGuang_Filter_Tag,
    eKelvin_Filter_Tag,
    e1977_Filter_Tag,
    eNashville_Filter_Tag,
    eValencial_Filter_Tag,
   // eWalden_Filter_Tag,
    //eHefe_Filter_Tag=2,
    eOverLay2_Fliter_Tag

};


bool FilterLayer::init()
{
    if(!Layer::init()) return false;
    
    LayerColor* layerColor=LayerColor::create(Color4B::WHITE);
    this->addChild(layerColor);
    
    _names.push_back("原图");
    
    _names.push_back("LOMO");
    _names.push_back("Test2");
    _names.push_back("复古");
    _names.push_back("梦幻");
    
    _names.push_back("OverLay");
    _names.push_back("酒红");
    _names.push_back("清宁");
    
    _names.push_back("哥特");
    _names.push_back("浪漫");
     _names.push_back("淡雅");
    
//    _names.push_back("原图");
//    _names.push_back("LOMO");
//    _names.push_back("黑白");
//    _names.push_back("Test2");
//    _names.push_back("复古");
//    _names.push_back("复古2");
//    _names.push_back("哥特");
//    _names.push_back("锐化");
//    _names.push_back("淡雅");
//    _names.push_back("酒红");
//    _names.push_back("清宁");
//    _names.push_back("浪漫");
//    _names.push_back("光晕");
//    _names.push_back("蓝调");
//    _names.push_back("梦幻");
//    _names.push_back("夜色");
//    _names.push_back("UNKNOWN模式");
//    _names.push_back("对比度");
//    _names.push_back("饱和度");
//    _names.push_back("边缘检测");
//    _names.push_back("浮雕");
//    _names.push_back("Motion模糊");
//    _names.push_back("1977");
//    _names.push_back("Kelvin");
//    _names.push_back("Nashvile");
//    _names.push_back("Valencial");
////    _names.push_back("Walden");
////    _names.push_back("Hefe");
////    _names.push_back("InKWell");
//    _names.push_back("OverLay");
//    _names.push_back("柔光");
//    _names.push_back("OverLay");
    
    Sprite* lSprite=Sprite::create("flower.png");
    lSprite->setScale(0.5f);
    lSprite->setPosition(Vec2(512, 550));
    this->addChild(lSprite);
    _targetSprite=lSprite;
    
    TableView *tableView=TableView::create(this, Size(1024,250));
    tableView->setDirection(ScrollView::Direction::HORIZONTAL);
    tableView->setAnchorPoint(Vec2(0, 0));
    tableView->setPosition(Vec2(0, 100));
    tableView->setVerticalFillOrder(cocos2d::extension::TableView::VerticalFillOrder::TOP_DOWN);
    tableView->setDelegate(this);
    this->addChild(tableView);
    tableView->reloadData();

    
    return true;
}

void FilterLayer::changeSprite(Sprite* pSprite,const GLchar* pVsh,const GLchar* pFsh, Texture2D* pTexture,GLuint pIndex,Texture2D* pTexture2,GLuint pIndex2)
{
    GLProgram* p= GLProgram::createWithByteArrays(pVsh, pFsh);
    if(pTexture!=NULL)
    {
        GL::bindTexture2D(0);
        
        GLuint lTexture1 =  p->getUniformLocation("CC_Texture1");
       // GLuint lTexture1_id = Director::getInstance()->getTextureCache()->addImage(pTextureName)->getName();
        GLuint lTexture1_id = pTexture->getName();
        GL::bindTexture2DN(pIndex, lTexture1_id);
        p->setUniformLocationWith1i(lTexture1, pIndex);
      
    }
    if(pTexture2!=NULL)
    {
        GLuint lTexture1 =  p->getUniformLocation("CC_Texture2");
        // GLuint lTexture1_id = Director::getInstance()->getTextureCache()->addImage(pTextureName)->getName();
        GLuint lTexture1_id = pTexture->getName();
        GL::bindTexture2DN(pIndex2, lTexture1_id);
        p->setUniformLocationWith1i(lTexture1, pIndex2);
    }
    //GLProgram* p= GLProgram::createWithFilenames("gray.vsh", "test3.fsh");
    auto glProgramState = GLProgramState::getOrCreateWithGLProgram( p );
    pSprite->setGLProgramState( glProgramState );
    
}

void FilterLayer::changeSprite(Sprite*pSprite,int idx)
{
    Sprite* lSprite=pSprite;
    lSprite->removeAllChildrenWithCleanup(true);
    switch (idx) {
        case 0:
        {
            this->changeSprite(lSprite, str_VSH, strFSH_ORGIN);
        }
            break;
        case 1:
        {
            this->changeSprite(lSprite, str_VSH, strFSH_LOMO);
        }
            break;
        case 2:
        {
            this->changeSprite(lSprite, str_VSH, strFSH_HEIBAI2);
        }
            break;
        case 3:
        {
            this->changeSprite(lSprite, str_VSH, strFSH_FUGU);
        }
            break;
        case 4:
        {
            this->changeSprite(lSprite, str_VSH, strFSH_MENGHUAN);
        }
            break;
            
           
       //1
        case 5:
        {
            this->changeSprite(lSprite, str_VSH, strFSH_ORGIN);
            
//            Sprite* lResSprite=Sprite::create("noise.png");
//            lSprite->addChild(lResSprite);
//            lResSprite->setPosition(Vec2(lSprite->getContentSize().width/2, lSprite->getContentSize().height/2));
//            this->changeSprite(lResSprite, str_VSH, strFSH_OVERLAY,lSprite->getTexture(),eOverLay_Fliter_Tag);
        }
            break;
            
        case 6:
        {
            this->changeSprite(lSprite, str_VSH, strFSH_JIUHONG);
        }
            break;
        case 7:
        {
            this->changeSprite(lSprite, str_VSH, strFSH_QINGNING);
        }
            break;
            
            
       //2
        case 8:
        {
            this->changeSprite(lSprite, str_VSH, strFSH_GETE);
        }
            break;
        case 9:
        {
            this->changeSprite(lSprite, str_VSH, strFSH_LANGMAN);
        }
            break;
        case 10:
        {
            this->changeSprite(lSprite, str_VSH, strFSH_DANYA);
        }
            break;
        default:
            break;
    }
    
}

/*
void FilterLayer::changeSprite(Sprite*pSprite,int idx)
{
    Sprite* lSprite=pSprite;
    lSprite->removeAllChildrenWithCleanup(true);
    switch (idx) {
        case 0:
        {
            this->changeSprite(lSprite, str_VSH, strFSH_ORGIN);
        }
            break;
        case 1:
        {
            this->changeSprite(lSprite, str_VSH, strFSH_LOMO);
        }
            break;
        case 2:
        {
            this->changeSprite(lSprite, str_VSH, strFSH_HEIBAI);
        }
            break;
        case 3:
        {
            this->changeSprite(lSprite, str_VSH, strFSH_HEIBAI2);
        }
            break;
        case 4:
        {
            this->changeSprite(lSprite, str_VSH, strFSH_FUGU);
        }
            break;
        case 5:
        {
            this->changeSprite(lSprite, str_VSH, strFSH_FUGU2);
        }
            break;
        case 6:
        {
            this->changeSprite(lSprite, str_VSH, strFSH_GETE);
        }
            break;
        case 7:
        {
            this->changeSprite(lSprite, str_VSH, strFSH_RUISE);
        }
            break;
        case 8:
        {
            this->changeSprite(lSprite, str_VSH, strFSH_DANYA);
        }
            break;
        case 9:
        {
            this->changeSprite(lSprite, str_VSH, strFSH_JIUHONG);
        }
            break;
        case 10:
        {
            this->changeSprite(lSprite, str_VSH, strFSH_QINGNING);
        }
            break;
        case 11:
        {
            this->changeSprite(lSprite, str_VSH, strFSH_LANGMAN);
        }
            break;
        case 12:
        {
            this->changeSprite(lSprite, str_VSH, strFSH_GUANGYUN);
        }
            break;
        case 13:
        {
            this->changeSprite(lSprite, str_VSH, strFSH_LANDIAO);
        }
            break;
        case 14:
        {
            this->changeSprite(lSprite, str_VSH, strFSH_MENGHUAN);
        }
            break;
        case 15:
        {
            this->changeSprite(lSprite, str_VSH, strFSH_YESE);
        }
            break;
        case 16:
        {
            this->changeSprite(lSprite, str_VSH, strFSH_UKNOWN);
        }
            break;
        case 17:
        {
            this->changeSprite(lSprite, str_VSH, strFSH_DUIBIDU);
        }
            break;
        case 18:
        {
            this->changeSprite(lSprite, str_VSH, strFSH_BAOHEDU);
        }
            break;
        case 19:
        {
            this->changeSprite(lSprite, str_VSH, strFSH_BIANYUANJIANCHE);
        }
            break;
        case 20:
        {
            this->changeSprite(lSprite, str_VSH, strFSH_FUDIAO);
        }
            break;
        case 21:
        {
            this->changeSprite(lSprite, str_VSH, strFSH_MOTION_BLUR);
        }
            break;
        case 22:
        {
            this->changeSprite(lSprite, str_VSH, strFSH_1977,Director::getInstance()->getTextureCache()->addImage("1977map.png"),e1977_Filter_Tag,NULL,0);
        }
            break;
        case 23:
        {
            this->changeSprite(lSprite, str_VSH, strFSH_KELVIN,Director::getInstance()->getTextureCache()->addImage("kelvinMap.png"),eKelvin_Filter_Tag,NULL,0);
        }
            break;
        case 24:
        {
            this->changeSprite(lSprite, str_VSH, strFSH_NASHVILLE,Director::getInstance()->getTextureCache()->addImage("nashvilleMap.png"),eNashville_Filter_Tag,NULL,0);
        }
            break;
        case 25:
        {

          //  this->changeSprite(lSprite, str_VSH, strFSH_NASHVILLE,Director::getInstance()->getTextureCache()->addImage("valenciaMap.png"),eValencial_Filter_Tag,Director::getInstance()->getTextureCache()->addImage("valenciaGradientMap.png"),eValencial_Filter2_Tag);
            
             this->changeSprite(lSprite, str_VSH, strFSH_NASHVILLE,Director::getInstance()->getTextureCache()->addImage("valenciaMap.png"),eValencial_Filter_Tag,NULL,0);
        }
            break;
//        case 26:
//        {
//            this->changeSprite(lSprite, str_VSH, strFSH_WALDEN,Director::getInstance()->getTextureCache()->addImage("valenciaMap.png"),eWalden_Filter_Tag,NULL,0);
//        }
//            break;
//        case 27:
//        {
//            this->changeSprite(lSprite, str_VSH, strFSH_HEFE,Director::getInstance()->getTextureCache()->addImage("hefeMap.png"),eHefe_Filter_Tag,NULL,0);
//        }
//            break;
//        case 28:
//        {
//            this->changeSprite(lSprite, str_VSH, strFSH_INKWELL,Director::getInstance()->getTextureCache()->addImage("inkwellMap.png"),2,NULL,0);
//        }
            break;
        case 26:
        {
            this->changeSprite(lSprite, str_VSH, strFSH_ORGIN);
            
            Sprite* lResSprite=Sprite::create("noise.png");
            lSprite->addChild(lResSprite);
            lResSprite->setPosition(Vec2(lSprite->getContentSize().width/2, lSprite->getContentSize().height/2));
            this->changeSprite(lResSprite, str_VSH, strFSH_OVERLAY,lSprite->getTexture(),eOverLay_Fliter_Tag);
        }
            break;
        case 27:
        {
            this->changeSprite(lSprite, str_VSH, strFSH_ROUGUANG, Director::getInstance()->getTextureCache()->addImage("water_glass.jpg"),eRouGuang_Filter_Tag);
        }
            break;
        case 28:
        {
             this->changeSprite(lSprite, str_VSH, strFSH_ORGIN);
            
            Sprite* lResSprite=Sprite::create("filter04.png");
            lSprite->addChild(lResSprite);
            lResSprite->setPosition(Vec2(lSprite->getContentSize().width/2, lSprite->getContentSize().height/2));
            this->changeSprite(lResSprite, str_VSH, strFSH_OVERLAY2,lSprite->getTexture(),eOverLay2_Fliter_Tag,NULL,0);
            
//            this->changeSprite(lSprite, str_VSH, strFSH_OVERLAY2,Director::getInstance()->getTextureCache()->addImage("filter04-768.png"),eRouGuang_Filter_Tag,NULL,0);
//            
            
        //   this->changeSprite(lSprite, str_VSH, strFSH_ROUGUANG, Director::getInstance()->getTextureCache()->addImage("filter04.png"),eRouGuang_Filter_Tag);
        }
            break;
        default:
            break;
    }

}
*/
#pragma mark - TableView

Size FilterLayer::cellSizeForTable(TableView *table) {
    return  Size(220,250);
};

TableViewCell* FilterLayer::tableCellAtIndex(TableView *table, ssize_t idx)
{
    
    TableViewCell *cell = table->dequeueCell();
    if (!cell) {
        cell = new TableViewCell();
        cell->autorelease();
    }
    
    cell->removeAllChildrenWithCleanup(true);
    
    Sprite* lSprite=Sprite::create("flower.png");
    lSprite->setScale(0.26f);
    lSprite->setPosition(Vec2(110, 125));
    cell->addChild(lSprite);
    
    
    
    this->changeSprite(lSprite, idx);
    
    auto label = LabelTTF::create(_names.at(idx).c_str(), "Arial", 30);
    label->setColor(Color3B::RED);
    label->setPosition(Vec2(110, 15));
    cell->addChild(label);
    
    
    return cell;
}


ssize_t FilterLayer::numberOfCellsInTableView(TableView *table)
{
    return _names.size();
}

void FilterLayer::tableCellTouched(TableView* table, TableViewCell* cell)
{
    this->changeSprite(_targetSprite, cell->getIdx());
}
