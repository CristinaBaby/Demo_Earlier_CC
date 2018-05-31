//
//  ShareScene.h
//
//
//  Created by luoxp on 3/18/15.
//
//

#pragma once

#include "UITools.h"
#include "SwallowListenerLayer.hpp"

class Graph;

class  ShareScene : public Scene
{
public :
    virtual ~ShareScene();
    ShareScene();
    
    CREATE_FUNC(ShareScene);
    
    virtual bool init();
    void onButton(Ref* pSender, Widget::TouchEventType type);
    
    void onDialog(const string& name);
    
    //post
    void waitPostSuccessful(Ref* ref);
    void waitPostFaild(Ref* ref);
    void onPermissionGrantedResult(int requestCode,bool bGranted);
private:
    Node* _canvas;
    std::string m_sPostImage;
    SwallowListenerLayer*   m_pPostingLayer;
    Graph &     _graph;
    Layout *    _back;
    Text *      _inspiration;
};