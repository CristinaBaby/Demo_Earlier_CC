//
//  KeyLayer.h
//  ColorGarden
//
//  Created by lidong on 15/8/19.
//
//

#ifndef __ColorGarden__KeyLayer__
#define __ColorGarden__KeyLayer__

#include <stdio.h>
#include "cocos2d.h"

USING_NS_CC;

class KeyLayer : public Layer {
    
        EventListenerKeyboard* _listener;
public:
    bool init();
    CREATE_FUNC(KeyLayer);
    
    void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
    void startListener();
    void stopListener();
    
    void quit(Node* pNode);
    void removeDialog(Node* pNode);
};

#endif /* defined(__ColorGarden__KeyLayer__) */
