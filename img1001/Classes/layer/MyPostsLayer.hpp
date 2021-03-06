//
//  MyArtworksLayer.hpp
//  IMG1001
//
//  Created by houhuang on 15/11/17.
//
//

#ifndef MyArtworksLayer_hpp
#define MyArtworksLayer_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "../data/DataManager.h"
#include "../compoment/MyArtworksSprite.hpp"
#include "../compoment/FavoriteSprite.hpp"

USING_NS_CC;
USING_NS_CC_EXT;

class MyPostsLayer : public LayerColor, public ScrollViewDelegate
{
public:
    CREATE_FUNC(MyPostsLayer);
    bool init();
    
    MyPostsLayer();
    ~MyPostsLayer();
    
    void onExit();

public:

    void doAction(Ref* ref);
    void removeDialog(Node* pSender);
    
    void hideMapDeleteBtn();
    void showMapDeleteBtn();
    void deleteMapImage(Node* node);

    void initContentLayer();
    void reponseMapSprite(Node* ref);

    void onEnterTransitionDidFinish();
    
    void deleteSaveImage(const string& imageName);
    
    void setScrollViewOffset();
//public:
//    Size cellSizeForTable(TableView* table);
//    TableViewCell* tableCellAtIndex(TableView* table, ssize_t idex);
//    ssize_t numberOfCellsInTableView(TableView* table);
//    void tableCellTouched(TableView* table, TableViewCell* cell);
    
public:
    void scrollViewDidScroll(ScrollView* view);

    void scrollViewDidZoom(ScrollView* view);
    
protected:
    int         m_iImageNumber;
    string      m_sImageName;
    
    TableView*  m_pTableView;
    ScrollView* m_pScrollView;
//    std::vector<GalleryInfo>* m_vGalleryInfo;
//    std::vector<MapSprite_New*> m_vMapSprite;
    
    bool        m_bIsEdit;
    
    std::vector<FavoriteSprite*>  m_vMyArtworksSprite;
    std::vector<Menu*>  m_vdeleteMenu;
    FavoriteSprite*   m_pClickDeleteSprite;
    
};

#endif /* MyArtworksLayer_hpp */
