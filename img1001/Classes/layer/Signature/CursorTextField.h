//
//  CursorTextField.h
//  TestProject
//
//  Created by houhuang on 15/10/10.
//
//

#ifndef __TestProject__CursorTextField__
#define __TestProject__CursorTextField__

#include <stdio.h>
#include "cocos2d.h"
USING_NS_CC;
using namespace std;

class CursorTextField : public TextFieldTTF, TextFieldDelegate
{
public:
    static CursorTextField* create(Size size, const string& placeHolder, const string& fontName, int fontSize);
    bool init(int fontSize);
    CursorTextField();
    ~CursorTextField();

public:
    
    void openIME();
    void closeIME();

    virtual bool onTextFieldAttachWithIME(TextFieldTTF * sender);
    virtual bool onTextFieldDetachWithIME(TextFieldTTF * sender);
    virtual bool onTextFieldInsertText(TextFieldTTF * sender, const char * text, size_t nLen);
    virtual bool onTextFieldDeleteBackward(TextFieldTTF * sender, const char * delText, size_t nLen);

private:
    string  m_pInputStr;
    Sprite* m_pCursorSprite;
};

#endif /* defined(__TestProject__CursorTextField__) */
