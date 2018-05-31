//
//  Dialog.h
//  HIDD
//
//  Created by lidong on 15-4-7.
//
//

#ifndef __HIDD__Dialog__
#define __HIDD__Dialog__

#include <stdio.h>
#include "cocos2d.h"
#include "../define/GlobalMacro.h"
#include "../commpoent/STSlider.h"
//#include "../modules/STInAppBilling.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;
class Dialog : public Layer{
    
    
protected:
    bool _isTouchEnable;
    LayerColor* _layerColor;
public:
    static Dialog* create();
    bool init();
    virtual void setToucEnable(bool pBool);
    virtual void addTouchListener();
    virtual void removeTocuhListener();
    
    virtual bool onTouchBegan(Touch *touch, Event *unused_event);
    void doAction(Ref* pSender);
    ~Dialog();

};

class TargetDialog : public Dialog {
protected:
    Ref* _leftTarget;
    SEL_CallFuncN _left_selector;
    Ref* _rightTarget;
    SEL_CallFuncN _right_selector;
    EventListenerKeyboard* _mListener;
public:
     bool init();
    TargetDialog();
    void removeDialog(Node* pNode);
public:
    void setLeftListener(Ref* pLeftTarget,SEL_CallFuncN pLeftSelector);
    void setRightListener(Ref* pRightTraget,SEL_CallFuncN pRightSelector);
    void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
    void startListener();
    void stopListener();
};

class STAlertDialog : public TargetDialog {
    
protected:
    Menu* _menu;
public:
    STAlertDialog();
    bool init(string pText,string pLeftBtnName,string pRightBtnName);
    static STAlertDialog* create(string pText,string pLeftBtnName,string pRightBtnName);
    void doAction(Ref* pSender);
};


class DeluxeDialog : public TargetDialog,public TableViewDataSource,public TableViewDelegate {
protected:
    std::map<string,name_vec> _nameVecMaps;
    Menu* _menu;
    TableView* _tableView;
    Size _cellSize;
    
//    STInAppBilling  *_stInAppBilling;
    
public:
    bool init();
    CREATE_FUNC(DeluxeDialog);
    DeluxeDialog();
    ~DeluxeDialog();
    void loadResource();
    void loadUI();
    void doAction(Ref* pSender);
    Vec2 getPlayPosition();
    void excuteAnimation();
    
public:
    TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(TableView *table);
    Size cellSizeForTable(TableView *table);
    
    void tableCellTouched(TableView* table, TableViewCell* cell);
};




class SettingDialog : public TargetDialog , public STSliderDelegate {

protected:
    std::map<string,name_vec> _nameVecMaps;
    Menu* _menu;
    Sprite* _backgroundSprite;
    std::vector<STSlider*> _sliders;
    STSlider* _targetSlider;
    Vec2 _startPoint;
    Vec2 _orginPoint;
    EventListenerTouchOneByOne* _listener;
    
public:
    ~SettingDialog();
    bool init();
    CREATE_FUNC(SettingDialog);
    void loadResource();
    void loadUI();
    void doAction(Ref* pSender);
    virtual void setToucEnable(bool pBool);
    void setDeleteDataItemVisible(bool pBool);
public:
    void sliderChangeValue(STSlider* pSlider);
public:
    
    virtual void addTouchListener();
    virtual void removeTocuhListener();
    virtual bool onTouchBegan(Touch *touch, Event *unused_event);
    virtual void onTouchMoved(Touch *touch, Event *unused_event);
    virtual void onTouchEnded(Touch *touch, Event *unused_event);
    virtual void onTouchCancelled(Touch *touch, Event *unused_event);
public:
    void deleteAllSavedData(Node* pRef);
    void removeDialog(Node* pRef);
};

class GameOverDialog : public TargetDialog {

    
    
protected:
    std::map<string,name_vec> _nameVecMaps;
    Menu* _menu;
    GameInfo* _gameInfo;
    Label* _scoreLabel;
   
public:
    bool init();
    CREATE_FUNC(GameOverDialog);
    void loadResource();
    void loadUI();
    void setGameData(int pScore);
    void doAction(Ref* pSender);
    
};

class GameCompleteDialog : public TargetDialog {
    
protected:
    std::map<string,name_vec> _nameVecMaps;
    Menu* _menu;
    Label* _timeLabel;
    Label* _scoreLabel;
    Label* _highestLabel;
    bool _isMayRateUs;
     bool _isResponseTouchEvent;
    
    int _time;
    int _score;
    int _highestScore;
    
public:
    void deleyTimeLoad(float dt);
    void stopResponseTouch(Ref* pSender);
    void resumeResponseTouch(Ref* pSender);
    
    bool init();
    CREATE_FUNC(GameCompleteDialog);
    ~GameCompleteDialog();
    void recordFinishInfo();
    void loadRateUsInfo();
    void loadResource();
    void loadUI();
    void showData(int pTime,int pScore,int pHighestScore);
    string getTimeStrWithTime(int pTime);
    void doAction(Ref* pSender);
    
};


#endif /* defined(__HIDD__Dialog__) */
