//
//  ImageBox.cpp
//  ColorGarden
//
//  Created by lidong on 15/8/3.
//
//

#include "ImageBox.h"
#include "../game/DataManager.h"
#include "../helper/ImageDownload.h"
#include "ImageNode.h"
ImageBox::ImageBox()
{
    _imageNode=NULL;
}

ImageBox::~ImageBox()
{

}



bool ImageBox::init()
{
    if(!Node::init()) return false;
    
    this->setContentSize(Size(Vec2(1024, 1024)));
    
    this->setAnchorPoint(Vec2(0.5f, 0.5f));
    _index=0;
    _disSprite=NULL;

    _delegate=NULL;
    _rotateSprite=NULL;
    m_pRotate = NULL;
    
    this->loadBackColor();
    
    //load_rotate_icon.png
    
    return true;
}

void ImageBox::loadBackColor()
{
    LayerColor* layerColor2=LayerColor::create(Color4B(255,255,255, 255));
    layerColor2->setContentSize(this->getContentSize());
    this->addChild(layerColor2);
}

void ImageBox::removeNodes()
{
   if(_rotateSprite!=NULL)
   {
       this->removeRotateAniamtion();
   }
    
   if(_imageNode!=NULL)
   {
       _imageNode->removeFromParentAndCleanup(true);
       _imageNode=NULL;
   }
}

void ImageBox::display()
{
    _rotateSprite=NULL;
    _disSprite=NULL;
    
    _rotateSprite=Sprite::create("load_rotate_icon.png");
    _rotateSprite->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height/2));
    this->addChild(_rotateSprite,5);
    
    RotateBy* lRotateBy=RotateBy::create(1.0f, 360.0f);
    RepeatForever* lRepeatForever=RepeatForever::create(lRotateBy);
    _rotateSprite->runAction(lRepeatForever);

    _imageNode=ImageNode::create(this->getImageName(), this->getIndex());
    _imageNode->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height/2));
    this->addChild(_imageNode);
    
    _imageNode->display();
    
}

void ImageBox::downloadingImage()
{
    if (_isMask)
    {
        return;
    }else if (DataManager::m_bDownloadingImage)
    {
        m_pRotate=Sprite::create("load_rotate_icon.png");
        m_pRotate->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height/2));
        this->addChild(m_pRotate,5);
        
        RotateBy* lRotateBy=RotateBy::create(1.0f, 360.0f);
        RepeatForever* lRepeatForever=RepeatForever::create(lRotateBy);
        m_pRotate->runAction(lRepeatForever);
    }
    
}


string ImageBox::getImageName()
{
    ResourceInfo* lInfo;
    if(this->getIndex()<=DataManager::getInstance()->_resourceInfos.size())
    {
        lInfo=&DataManager::getInstance()->_resourceInfos.at(this->getIndex()-1);
    }
    else
    {
        lInfo=&ImageDownload::getInstance()->_downloadFinishResourceInfo.at(this->getIndex()-DataManager::getInstance()->_resourceInfos.size()-1);
    }

    
    string lPath = FileUtils::getInstance()->getWritablePath() + lInfo->image_name_w;
    
    _isMask = true;
    if (!FileUtils::getInstance()->isFileExist(lInfo->image_name_w) && !FileUtils::getInstance()->isFileExist(lPath))
    {
        _isMask = false;
    }
    
    string filename = lInfo->image_name;
    filename = filename.substr(0, filename.find_last_of(".")) + "_save.png";
    
    bool isExsit = FileUtils::getInstance()->isFileExist(filename);
    if (isExsit)
    {
    return filename;
}
    else
    {
        return lInfo->image_name;
    }
}

string ImageBox::getUserNewTag()
{
    ResourceInfo* lInfo;
    if(this->getIndex()<=DataManager::getInstance()->_resourceInfos.size())
    {
        lInfo=&DataManager::getInstance()->_resourceInfos.at(this->getIndex()-1);
    }
    else
    {
        lInfo=&ImageDownload::getInstance()->_downloadFinishResourceInfo.at(this->getIndex()-DataManager::getInstance()->_resourceInfos.size()-1);
    }
    
    stringstream ss;
    ss<<"";
    ss<<"map";
    ss<<lInfo->image_index;
    ss<<"_white.png";
    
//    log("%s",ss.str().c_str());
    return ss.str();
}

void ImageBox::responseTouchEvent(Vec2 pPoint)
{
    Vec2 lPoint=this->getParent()->convertToNodeSpace(pPoint);
    
    if(this->getBoundingBox().containsPoint(lPoint))
    {
        if(this->getDelegate()!=NULL)
        {
            this->getDelegate()->reponseTouchEvent(this);
        }
    }
}

void ImageBox::removeRotateAniamtion()
{
    
    if(_rotateSprite!=NULL)
    {
        _rotateSprite->stopAllActions();
        _rotateSprite->removeFromParentAndCleanup(true);
        _rotateSprite=NULL;
    }
    
}

void ImageBox::setIsShowingNew(bool pBool)
{
   if(_imageNode!=NULL)
   {
       _imageNode->setIsShowingNew(pBool);
   }
}


