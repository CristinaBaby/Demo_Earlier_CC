//
//  GraphManager.hpp
//  ALABS0010
//
//  Created by houhuang on 16/3/22.
//
//

#ifndef GraphManager_hpp
#define GraphManager_hpp

#include <stdio.h>
#include "cocos2d.h"
USING_NS_CC;
using namespace std;

#define GphM_getInstance    GraphManager::getInstance()
#define xGraph              GraphManager::getInstance()

class Graph;

class GraphManager
{
public:
    GraphManager();
    ~GraphManager();
    
    static GraphManager* getInstance();
    void readGraphCsvInfo();
    void readGraphCsvInfo(const string& filename, vector<Graph> & graphs);
    
    vector<Graph> & getData();                    //后面直接将数据全部存入Chapter中, 不在保留一份数据
    vector<Graph> & getDataDownload();             
    
    void destorySaveImage();
    
private:
    vector<Graph> m_vGraphInfo;
    vector<Graph> m_vGrapInfoDownload;
    CC_SYNTHESIZE(Image *, _saveImage, SaveImage)
};

class Graph
{
public:
    string getHistoryName();
    string getPreviewName();
    string getThumbsName();
    string getPngName();
    string getMaskName();
    
    void setBGColor(const Color3B& color);
    
    void deleteHistory(const string& previewName);
    void setNew(bool isNew);
    void setIndex(const string& previewName);
    
public:
    string  name;
    int     chapter_id;
    bool    isLock;
    Color3B bgColor;
    int     historyIndex = 0;
    int     historyCount = 1;
    
    bool    isNew = false;
    bool    reDownload = false;
};

#endif /* GraphManager_hpp */
