//
//  ImageNode.cpp
//  ColorGarden
//
//  Created by lidong on 15/11/23.
//
//

#include "ImageNode.h"

#include "../define/GlobalMacro.h"
#include "../define/ResoureMacro.h"
#include "../game/DataManager.h"
#include "ImageBox.h"
ImageNode::ImageNode()
{
    _disSprite=NULL;
    _newSprite=NULL;
    
    _isShowingNew=true;
}

ImageNode::~ImageNode()
{
   
}

ImageNode* ImageNode::create(std::string imagename,int index)
{
    ImageNode* lImageNode=new ImageNode();
    if(lImageNode && lImageNode->init(imagename,index))
    {
        lImageNode->autorelease();
        return lImageNode;
    }
    
    CC_SAFE_DELETE(lImageNode);
    return NULL;
    
}

void ImageNode::onEnter()
{
    Node::onEnter();
    
    NotificationCenter::getInstance()->removeObserver(this, st_texture_load_finish_notification);
    NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(ImageNode::loadTextureFinish),st_texture_load_finish_notification,NULL);

}


void ImageNode::onExit()
{
    NotificationCenter::getInstance()->removeObserver(this, st_texture_load_finish_notification);
    Node::onExit();
}



bool ImageNode::init(std::string imagename,int index)
{
    if(!Node::init()) return false;
    
    this->setContentSize(Size(Vec2(1024, 1024)));
    
    this->setAnchorPoint(Vec2(0.5f, 0.5f));
    
    _imageName=imagename;
    _index=index;
    

    return true;
}

void ImageNode::loadTextureFinish(Ref* pSender)
{

    Texture2D* lTexture =  Director::getInstance()->getTextureCache()->getTextureForKey(_imageName);
    
    if(_disSprite==NULL && lTexture!=NULL)
    {
        Color3B lColor=DataManager::getInstance()->getSaveBackColor(_index);
        LayerColor* layerColor=LayerColor::create(Color4B(lColor.r, lColor.g, lColor.b, 255));
        layerColor->setContentSize(this->getContentSize());
//        this->addChild(layerColor);
        
        _disSprite=Sprite::create(_imageName);
        layerColor->addChild(_disSprite);
        _disSprite->setPosition(Vec2(layerColor->getContentSize().width/2, layerColor->getContentSize().height/2));
        
        ImageBox* lImageBox=dynamic_cast<ImageBox*>(this->getParent());
        if(lImageBox!=NULL)
        {
            lImageBox->removeRotateAniamtion();
        }
        
        _newSprite = Sprite::create("new.png");
        this -> addChild(_newSprite,1);
        _newSprite ->setVisible(true);
        
        _newSprite->setPosition(Vec2(this->getContentSize().width - _newSprite->getContentSize().width/2,this->getContentSize().height - _newSprite->getContentSize().height/2));

         _newSprite->setVisible(_isShowingNew);

        Sprite* lSprite = Sprite::create("paper_1.png");
        lSprite->setScale(1);
        ClippingNode* clippingNode = ClippingNode::create();
        clippingNode->setStencil(lSprite);
        

        clippingNode->setInverted(false);        //设置底板不可见
        clippingNode->setAlphaThreshold(0);         //设置绘制底板的Alpha值为0
        this->addChild(clippingNode);
        clippingNode->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height/2));
        clippingNode->addChild(layerColor);
        layerColor->ignoreAnchorPointForPosition(false);
        layerColor->setAnchorPoint(Vec2(0.5, 0.5));
        layerColor->setPosition(Vec2(clippingNode->getContentSize().width/2, clippingNode->getContentSize().height/2));
        
    }
}


void ImageNode::display()
{
    Texture2D* lTexture =  Director::getInstance()->getTextureCache()->getTextureForKey(_imageName);
    if(lTexture!=NULL)
    {
        this->loadTextureFinish(NULL);
    }
    else
    {
        DataManager::getInstance()->ansyLoadTexture(_imageName);
    }
}

void ImageNode::setIsShowingNew(bool pBool)
{
    _isShowingNew=pBool;
    if(_newSprite!=NULL)
    {
        _newSprite->setVisible(_isShowingNew);
    }
}
