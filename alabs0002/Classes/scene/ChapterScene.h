//
//  ChapterScene.h
//
//
//  Created by luoxp on 3/18/15.
//
//

#pragma once

#include "UITools.h"
#include "UIScrollView.h"
#include "UISlider.h"


class  ChapterScene : public Scene
{
public :
    virtual ~ChapterScene();
    ChapterScene();
    
    CREATE_FUNC(ChapterScene);
    
    virtual bool init();
    void onButton(Ref* pSender, Widget::TouchEventType type);
    void onDialog(const string& name);
    void createListView();
    
    void updateProgress(Ref* ref);
    void waitDownloadFinish(Ref* ref);
    void onListView(Ref* pSender, ui::ScrollView::EventType type);
    void onSlider(Ref *pSender, Slider::EventType type);
    void checkTableOffset();
    void refresh(float dt);
    
    void checkUpdate(float dt);
    void showRateUs(EventCustom* event);
private:
    ListView * _listview;
    Layout * _root;
    std::vector<ProgressTimer*> m_vProgress;
    Size m_sLayoutSize;
    Slider * _slider;
    Text *   _text;
    float    _oldX;
};