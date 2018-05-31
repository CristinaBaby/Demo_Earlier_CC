//
//  SceneManager.h
//  SingleEye
//
//  Created by luoxiaopeng on 1/21/14.
//
//

#pragma once

#include "UIHeader.h"

#define xScene      (SceneManager::getInstance())

class SceneManager : public Node{
    
public:
    enum class Tag{
        NONE,
        LOADING,
        CHAPTER,
        GRAPH,
        GAME,
        MY_ARTS,
        SHARE,
        SETTING,
        INSPIRATION,
    };
    
    struct SceneData {
        Tag aTag;
        int iData;
        void* pData;
        bool isForward;
    };
    
public:
    static SceneManager* getInstance();
    
public:
    ~SceneManager();    
    void forward(Tag aTag);
    void forward(Tag aTag, int iData, void* pData);
    
    void back();
    void back(Tag aTag);
    Tag getRunningSceneTag();
    Tag getLastSceneTag();
    void updateCSV();
    
    void dumpAll();
        
private:
    SceneManager();    
    bool isOldScene();
    void replaceScene(SceneData &data);
    
    Tag                     m_eLastScene;
    Scene*                  m_pOldScene;
    SceneData               m_eBackup;
    std::vector<SceneData>  m_vData;
    
    bool _updateCSV = false;
};
