//
//  CursorTextField.cpp
//  TestProject
//
//  Created by houhuang on 15/10/10.
//
//

#include "CursorTextField.h"
#include "../../helper/ScreenAdapterHelper.h"
#include "TextFieldNode.h"

CursorTextField* CursorTextField::create(Size size, const string& placeHolder, const string& fontName, int fontSize)
{
    CursorTextField* lCursorTextField = new CursorTextField();
    if (lCursorTextField && lCursorTextField->initWithPlaceHolder(placeHolder, size, TextHAlignment::CENTER, fontName, fontSize) &&lCursorTextField->init(fontSize)  )
    {
        lCursorTextField->autorelease();
        
        if (placeHolder != "")
        {
            lCursorTextField->setPlaceHolder(placeHolder);
        }
        
        return lCursorTextField;
    }
    
    CC_SAFE_DELETE(lCursorTextField);
    return NULL;
}

CursorTextField::CursorTextField()
{
    m_pInputStr = "";
    m_pCursorSprite = NULL;
}

CursorTextField::~CursorTextField()
{

}

bool CursorTextField::init(int fontSize)
{
    if (!TextFieldTTF::init()) return false;
    
    
//    Sprite* lSprite = Sprite::create("cursor.png");
//    lSprite->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height/2));
//    this->addChild(lSprite);
//    m_pCursorSprite = lSprite;
    
    this->setDelegate(this);

    return true;
}


void CursorTextField::openIME()
{
    this->attachWithIME();
}

void CursorTextField::closeIME()
{
    this->detachWithIME();
}

bool CursorTextField::onTextFieldAttachWithIME(TextFieldTTF * sender)
{
    return false;
}

bool CursorTextField::onTextFieldDetachWithIME(TextFieldTTF * sender)
{
    return false;
}

bool CursorTextField::onTextFieldInsertText(TextFieldTTF * sender, const char * text, size_t nLen)
{
    int textASCII = *text;


    string ss;
    ss = sender->getString();
    sender->setString(sender->getString() + '\n' + 'm');

    if (textASCII == 10 )
    {
        if (sender->getContentSize().height > ScreenAdapterHelper::getValue(250.0))
        {
            sender->setString(ss);
            return true;
        }
        
        sender->setString(ss);
        sender->setString(sender->getString() + '\n');
        return true;
    }
    
    sender->setString(ss);
    
    
    string ss2;
//    log("---%f",sender->getContentSize().height);
    ss2 = sender->getString();

    sender->setString(sender->getString() + text);

    
//    log("-------------%f",sender->getContentSize().width);
    
    if (sender->getContentSize().width > ScreenAdapterHelper::getValue(245.0))
    {
        sender->setString(ss2);
        return true;
    }
    
    
    if (sender->getContentSize().height > ScreenAdapterHelper::getValue(250.0))
    {
        sender->setString(ss2);
        return true;
    }
    sender->setString(ss2);
    
    
    
//    if (textASCII == 10 )
//    {
//        sender->setString(sender->getString() + '\n');
//        
//    }else
//    {
//
//        
//        return false;
//    }

    
//    if ( (textASCII >= 65 && textASCII <= 90) || (textASCII >= 97 && textASCII <= 122) || (textASCII == 32))
//    {
//        return false;
//    }
    

    
        
    
    

    
    return false;
}

bool CursorTextField::onTextFieldDeleteBackward(TextFieldTTF * sender, const char * delText, size_t nLen)
{
    string ss1;
    ss1 = sender->getString();
    
    string ss2;
    ss2 = sender->getString();
    ss2.pop_back();
    sender->setString(ss2);

    if (sender->getContentSize().height / ScreenAdapterHelper::getValue(250.0) > 1.0)
    {
        this->setScale(ScreenAdapterHelper::getValue(250.0)/sender->getContentSize().height);
    }else
    {
        this->setScale(1.0);
    }
    
    sender->setString(ss1);
    return false;
}










