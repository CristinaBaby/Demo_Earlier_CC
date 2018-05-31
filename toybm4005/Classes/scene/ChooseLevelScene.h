//
//  ChooseLevelScene.h
//  ColorGarden
//
//  Created by lidong on 15/7/31.
//
//

#ifndef __ColorGarden__ChooseLevelScene__
#define __ColorGarden__ChooseLevelScene__

#include <stdio.h>
#include "../define/GlobalMacro.h"
#include "cocos2d.h"
#include "cocos-ext.h"
#include "../commpoent/ImageBox.h"
#include "BaseLayer.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;
class ImageBox;
class ChooseLevelScene :  public BaseLayer, public ImageBoxDelegate {
    
protected:
    std::vector<ImageBox*> _imageBoxs;
    ScrollView* _scrollView;
    bool _isTouchEnable;
    Vec2 _startPoint;
    Vec2 _orginPoint;
    
    bool _isMayExcuteTouchMove;
    bool _isTouchMoving;
    int _currentPage;
    
    std::vector<ImageBox*> _showingBoxs;
    
    EventListenerKeyboard* _listener;
    
public:
    ~ChooseLevelScene();
    bool init(int pIndex);
    static cocos2d::Scene* scene(int pIndex);
    //SCENE_FUNC(ChooseLevelScene);
    void loadMap();
    void showGame();
    void setToucEnable(bool pBool);
    int getPageNum();
    ImageBox* getImageBox(int pIndex);
    int getRealIndex(int pIndex);
    
    void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);


    void quit(Node* pNode);
    void removeDialog(Node* pNode);
public:
    
    void onEnter();
    
    void startListener();
    void stopListener();
    
    void addTouchListener();
    void removeTocuhListener();
    virtual bool onTouchBegan(Touch *touch, Event *unused_event);
    virtual void onTouchMoved(Touch *touch, Event *unused_event);
    virtual void onTouchEnded(Touch *touch, Event *unused_event);
    virtual void onTouchCancelled(Touch *touch, Event *unused_event);
    
    void refreshScroll(float dx);
    void excuteMoveEnd();
    void normalExcuteMoveEnd();
    void refreshDots();
public:
    void reponseTouchEvent(ImageBox* pImageBox);
    void refresh();
    void changeScene(float dt);
public:
    st_property(int, _index, Index);
};


#endif /* defined(__ColorGarden__ChooseLevelScene__) */
