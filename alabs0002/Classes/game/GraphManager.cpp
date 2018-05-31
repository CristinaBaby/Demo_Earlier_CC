//
//  GraphManager.cpp
//  ALABS0010
//
//  Created by houhuang on 16/3/22.
//
//

#include "GraphManager.hpp"
#include "CSVParse.h"
#include "UndoManager.h"

static GraphManager* _instance = NULL;

GraphManager* GraphManager::getInstance()
{
    if (!_instance)
    {
        _instance = new GraphManager();
    }
    
    return _instance;
}

GraphManager::GraphManager()
{
    _saveImage = nullptr;
    readGraphCsvInfo();
}

GraphManager::~GraphManager()
{

}

void GraphManager::readGraphCsvInfo()
{
    readGraphCsvInfo("graph.csv", m_vGraphInfo);
}

void GraphManager::readGraphCsvInfo(const string& filename, vector<Graph> & graphs)
{
    graphs.clear();
    
    CSVParse* csv = CSVParse::create(filename.c_str());
    const unsigned int row = csv->getRows();
    for (int i = 1; i < row; ++i)
    {
        Graph graph;
        graph.name = csv->getDatas(i, "name");
        
        if (graph.name.empty()) break;
        graph.chapter_id = stoi(csv->getDatas(i, "chapter_id"));
        
        string lIslock = csv->getDatas(i, "islock");
        if (lIslock == "TRUE")
        {
            graph.isLock = true;
        }
        else
        {
            graph.isLock = false;
        }
        
        //读取BGColor
        auto color = xUser->getStringForKey(string(graph.name + "_bgColor").c_str(), "255.255.255");           //默认白色
        
        int rIndex = color.find(".");
        int rValue = std::stoi(color.substr(0,rIndex));
        
        int gIndex = color.find(".", rIndex + 1);
        int gValue = std::stoi(color.substr(rIndex + 1, gIndex));
        int bValue = std::stoi(color.substr(gIndex + 1, color.length()));
        
        graph.bgColor = Color3B(rValue, gValue, bValue);
        
        //读取History
        graph.historyIndex = xUser->getIntegerForKey(string(graph.name + "_hIndex").c_str(), 0);
        graph.historyCount = xUser->getIntegerForKey(string(graph.name + "_hCount").c_str(), 1);
        
        graphs.push_back(graph);
    }
}

void GraphManager::destorySaveImage()
{
    CC_SAFE_DELETE(_saveImage);
    xUndo->reset();
}

vector<Graph> & GraphManager::getData()
{
    return m_vGraphInfo;
}

vector<Graph> & GraphManager::getDataDownload()
{
    if (m_vGrapInfoDownload.empty()) {
        readGraphCsvInfo("_graph.csv", m_vGrapInfoDownload);
    }
    
    return m_vGrapInfoDownload;
}

string Graph::getHistoryName()
{
    return std::move(string(name + "_" + std::to_string(historyIndex) + "_save.png"));
}

string Graph::getPreviewName()
{
    return std::move(string(name + "_" + std::to_string(historyIndex) + "_preview.png"));
}

string Graph::getThumbsName()
{
    return std::move(string(name + "_" + std::to_string(historyIndex) + "_save.png"));
}

string Graph::getPngName()
{
    return std::move(string(name + ".png"));
}

string Graph::getMaskName()
{
    return std::move(string(name + "_mask.png"));
}

void Graph::setBGColor(const Color3B& color)
{
    if (bgColor != color) {
        bgColor = color;
        
        auto value = std::to_string(bgColor.r) + "." + std::to_string(bgColor.g) + "." + std::to_string(bgColor.b);
        xUser->setStringForKey(string(name + "_bgColor").c_str(), value);
        xUser->flush();
    }
}

void Graph::deleteHistory(const string& previewName)
{
    auto end = previewName.find_last_of("_") - 1;
    auto start = previewName.find_last_of("_", end);
    auto number = previewName.substr(start + 1, end - start);
    
    int historyIndexBackup = historyIndex;
    historyIndex = stoi(number);
    remove((FileUtils::getInstance()->getWritablePath() + getPreviewName()).c_str());
    remove((FileUtils::getInstance()->getWritablePath() + getHistoryName()).c_str());
    Director::getInstance()->getTextureCache()->removeTextureForKey(getPreviewName());
    Director::getInstance()->getTextureCache()->removeTextureForKey(getHistoryName());
    historyIndex = historyIndexBackup;
    
    setBGColor(Color3B::WHITE);
}

void Graph::setNew(bool iNew)
{
    isNew = iNew;
    
    if (isNew)
    {
        historyCount++;
        historyIndex = historyCount - 1;
        
        xUser->setIntegerForKey(string(name + "_hIndex").c_str(), historyIndex);
        xUser->setIntegerForKey(string(name + "_hCount").c_str(), historyCount);
        xUser->flush();
        
        setBGColor(Color3B::WHITE);
    }
}

void Graph::setIndex(const string& previewName)
{
    auto end = previewName.find_last_of("_") - 1;
    auto start = previewName.find_last_of("_", end);
    auto number = previewName.substr(start + 1, end - start);
    
    isNew = false;
    historyIndex = stoi(number);
    xUser->setIntegerForKey(string(name + "_hIndex").c_str(), historyIndex);
    xUser->flush();
}