//
//  FavoriteLayer.hpp
//  IMG1001
//
//  Created by houhuang on 15/12/10.
//
//

#ifndef FavoriteLayer_hpp
#define FavoriteLayer_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "../data/DataManager.h"
#include "../compoment/FavoriteSprite.hpp"

USING_NS_CC;
USING_NS_CC_EXT;

class FavoriteLayer : public LayerColor, public ScrollViewDelegate
{
public:
    CREATE_FUNC(FavoriteLayer);
    bool init();
    
    FavoriteLayer();
    ~FavoriteLayer();
    
    void onExit();
    
public:
    
    void doAction(Ref* ref);
    void removeDialog(Node* pSender);
    
    void hideMapDeleteBtn();
    void showMapDeleteBtn();
    void deleteMapImage(Node* node);
    
    void initContentLayer();
    
    void onEnterTransitionDidFinish();
    
    void deleteSaveImage(const string& imageName);
    
    void setScrollViewOffset();

public:
    void scrollViewDidScroll(ScrollView* view);
    
    void scrollViewDidZoom(ScrollView* view);
    
protected:
    int         m_iImageNumber;
    string      m_sImageName;
    
    TableView*  m_pTableView;
    ScrollView* m_pScrollView;
    
    bool        m_bIsEdit;
    
    std::vector<FavoriteSprite*>  m_vMyArtworksSprite;
//    std::vector<Menu*>  m_vdeleteMenu;
    FavoriteSprite*   m_pClickDeleteSprite;
    
};

#endif /* FavoriteLayer_hpp */
