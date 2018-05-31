//
//  ChapterManager.cpp
//  ALABS0010
//
//  Created by houhuang on 16/3/21.
//
//

#include "ChapterManager.hpp"
#include "CSVParse.h"
#include "UITools.h"

static ChapterManager* _instance = NULL;

ChapterManager* ChapterManager::getInstance()
{
    if (!_instance)
    {
        _instance = new ChapterManager();
    }
    
    return _instance;
}

ChapterManager::ChapterManager()
{
    _tableOffset = 0;
    readChapterCsvInfo();
}

ChapterManager::~ChapterManager()
{
    
}

void ChapterManager::readChapterCsvInfo()
{
    m_vChapterInfo.clear();
    
    CSVParse* lCsv = CSVParse::create("chapter.csv");
    const unsigned int row = lCsv->getRows();
    
    for (int i = 1; i < row; ++i)
    {
        string lID = lCsv->getDatas(i, "id");
        string lName = lCsv->getDatas(i, "name");
        string lFrame = lCsv->getDatas(i, "frame");
        
        if (lID.empty()) break;
        
        Chapter lChapterInfo;
        lChapterInfo.id = std::stoi(lID);
        lChapterInfo.name = lName;
        lChapterInfo.image = lCsv->getDatas(i, "img");
        
        int rIndex = lFrame.find(".");
        int rValue = std::stoi(lFrame.substr(0,rIndex));
        
        int gIndex = lFrame.find(".", rIndex + 1);
        int gValue = std::stoi(lFrame.substr(rIndex + 1, gIndex));
        int bValue = std::stoi(lFrame.substr(gIndex + 1, lFrame.length()));
    
        lChapterInfo.frame = Color3B(rValue, gValue, bValue);
        
        m_vChapterInfo.push_back(lChapterInfo);
    }
    
    auto graphs = xGraph->getData();
    for (auto graph : graphs) {
        vector<Graph> &vGraphInfo = getChapterData(graph.chapter_id).vGraphInfo;
        vGraphInfo.push_back(graph);
    }
}

void ChapterManager::readChapterCsvInfoDownload()
{
    CSVParse* lCsv = CSVParse::create("_chapter.csv");
    const unsigned int row = lCsv->getRows();
    
    for (int i = 1; i < row; ++i)
    {
        string lID = lCsv->getDatas(i, "id");
        if (lID.empty()) break;
        
        bool found = false;
        for (auto chapter : m_vChapterInfo) {
            if (chapter.id == std::stoi(lID)) {
                found = true;
                break;
            }
        }
        
        if (found)
        {
            continue;
        }
        else
        {
            string lName = lCsv->getDatas(i, "name");
            string lFrame = lCsv->getDatas(i, "frame");
            
            Chapter lChapterInfo;
            lChapterInfo.id = std::stoi(lID);
            lChapterInfo.name = lName;
            lChapterInfo.image = lCsv->getDatas(i, "img");
            lChapterInfo.isNewChapter = true;
            
            int rIndex = lFrame.find(".");
            int rValue = std::stoi(lFrame.substr(0,rIndex));
            
            int gIndex = lFrame.find(".", rIndex + 1);
            int gValue = std::stoi(lFrame.substr(rIndex + 1, gIndex));
            int bValue = std::stoi(lFrame.substr(gIndex + 1, lFrame.length()));
            
            lChapterInfo.frame = Color3B(rValue, gValue, bValue);
            
            m_vChapterInfo.push_back(lChapterInfo);
        }
    }
}

void ChapterManager::updateChapterInfo(int id)
{
    Chapter & chapter = getChapterData(id);
    chapter.vGraphInfo.clear();
    
    auto graphs = xGraph->getDataDownload();
    for (auto graph : graphs) {
        if (graph.chapter_id == id) {
            chapter.vGraphInfo.push_back(graph);
        }
    }
}

void ChapterManager::updateProgress(int id)
{
    //downloaded++;
//    for (auto chaper : m_vChapterInfo)
//    {
//        if (id == chaper.id)
//        {
//            chaper.downloaded ++;
//        }
//    }
    
    for (int i = 0 ; i < m_vChapterInfo.size(); ++i)
    {
        Chapter *lInfo = &m_vChapterInfo.at(i);
        if (lInfo->id == id)
        {
            lInfo->downloaded ++;
            if (lInfo->downloaded >= lInfo->total)
            {
                this->updateChapterInfo(lInfo->id);
            }
        }
    }
    
    
    NotificationCenter::getInstance()->postNotification(st_updateProgress_message);
    
    
}

void ChapterManager::addTotal(int id)
{
    getChapterData(id).total += 2;
}

vector<Chapter> & ChapterManager::getData()
{
    return m_vChapterInfo;
}

Chapter & ChapterManager::getChapterData(int id)
{
    for (int i = 0; i < m_vChapterInfo.size(); i++)
    {
        if (m_vChapterInfo.at(i).id == id)
        {
            return m_vChapterInfo.at(i);
        }
    }
    
    CCASSERT(false, "error");
    return m_vChapterInfo.at(0);
}

void ChapterManager::enterChapter(int index)
{
    _current = index;
    quickFlurry("chapter_open_" + m_vChapterInfo.at(index).name);
}

void ChapterManager::enterGraph(const string &name)
{
    for (int chapteri = 0; chapteri < m_vChapterInfo.size(); chapteri++) {
        
        auto & graphs = m_vChapterInfo.at(chapteri).vGraphInfo;
        for (int graphi = 0; graphi < graphs.size(); graphi++) {
            
            if (graphs.at(graphi).getPngName() == name) {
                _current = chapteri;
                getCurrentChapter().enterGraph(graphi);
                return;
            }
        }
    }
    
    CCASSERT(false, "error");
}

Chapter & ChapterManager::getCurrentChapter()
{
    return m_vChapterInfo.at(_current);
}

void Chapter::enterGraph(int index)
{
    _current = index;
    quickFlurry("img_open_" + vGraphInfo.at(index).name);
}

bool Chapter::isDownloading()
{
    if (getPercent() == 100)
        return false;
    else
        return true;
}

Graph & Chapter::getCurrentGraph()
{
    return vGraphInfo.at(_current);
}

int Chapter::getPercent() const
{
    if (total == 0) {
        return 100;
    }
    else
    {
        return downloaded * 100 / total;
    }
}

Graph & ChapterManager::getGraph(const string &name)
{
    for (int chapteri = 0; chapteri < m_vChapterInfo.size(); chapteri++) {
        vector<Graph> & graphs = m_vChapterInfo.at(chapteri).vGraphInfo;
        for (int graphi = 0; graphi < graphs.size(); graphi++) {
            if (graphs.at(graphi).getPngName() == name) {
                return graphs.at(graphi);
            }
        }
    }
    
    CCASSERT(false, "error");
    return m_vChapterInfo.at(0).vGraphInfo.at(0);
}
