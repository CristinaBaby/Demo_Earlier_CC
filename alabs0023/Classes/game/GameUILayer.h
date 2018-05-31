//
//  GameUILayer.h
//  ColorGarden
//
//  Created by lidong on 15/8/3.
//
//

#ifndef __ColorGarden__GameUILayer__
#define __ColorGarden__GameUILayer__

#include <stdio.h>
#include "cocos2d.h"
#include "../define/GlobalMacro.h"
USING_NS_CC;
class GameUILayer : public Layer {
protected:
    Menu* _menu;
    LayerColor* _shadeLayer;
    bool  _isShowUndo;

public:
    bool init(int pIndex);
    static GameUILayer* create(int pIndex);

    void loadUI();
    void doAction(Ref* pSender);
    void loadScrollView();
    
    void removeUndoSprite(Node* pNode);
    
public:
    st_property(int,_index,Index);
    void removeTutorial();
    void removeDialog(Node* pRef);
    void resetGame(Node* pRef);
    void reset(float dt);
    void undoEndPrompt();
    void enableMenu(float dt);
    void enableFillNode();
};


#endif /* defined(__ColorGarden__GameUILayer__) */
