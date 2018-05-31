//
//  STTableView.cpp
//  ColorGarden
//
//  Created by lidong on 15/12/29.
//
//

#include "STTableView.h"
STTableView* STTableView::create(TableViewDataSource* dataSource, Size size)
{
     return STTableView::create(dataSource, size, nullptr);
}
STTableView* STTableView::create(TableViewDataSource* dataSource, Size size, Node *container)
{
    STTableView *table = new STTableView();
    table->initWithViewSize(size, container);
    table->autorelease();
    table->setDataSource(dataSource);
    table->_updateCellPositions();
    table->_updateContentSize();
    return table;
}

void STTableView::scrollViewDidScroll(ScrollView* view)
{
    long countOfItems = _dataSource->numberOfCellsInTableView(this);
    if (0 == countOfItems)
    {
        return;
    }
    
    if (_isUsedCellsDirty)
    {
        _isUsedCellsDirty = false;
        std::sort(_cellsUsed.begin(), _cellsUsed.end(), [](TableViewCell *a, TableViewCell *b) -> bool{
            return a->getIdx() < b->getIdx();
        });
    }
    
    if(_tableViewDelegate != nullptr) {
        _tableViewDelegate->scrollViewDidScroll(this);
    }
    
    ssize_t startIdx = 0, endIdx = 0, idx = 0, maxIdx = 0;
    Vec2 offset = this->getContentOffset() * -1;
    maxIdx = MAX(countOfItems-1, 0);
    
    if (_vordering == VerticalFillOrder::TOP_DOWN)
    {
        offset.y = offset.y + _viewSize.height/this->getContainer()->getScaleY();
    }
    startIdx = this->_indexFromOffset(offset);
    if (startIdx == CC_INVALID_INDEX)
    {
        startIdx = countOfItems - 1;
    }
    
    if (_vordering == VerticalFillOrder::TOP_DOWN)
    {
        offset.y -= _viewSize.height/this->getContainer()->getScaleY();
    }
    else
    {
        offset.y += _viewSize.height/this->getContainer()->getScaleY();
    }
    offset.x += _viewSize.width/this->getContainer()->getScaleX();
    
    endIdx   = this->_indexFromOffset(offset);
    if (endIdx == CC_INVALID_INDEX)
    {
        endIdx = countOfItems - 1;
    }
    
#if 0 // For Testing.
    Ref* pObj;
    int i = 0;
    CCARRAY_FOREACH(_cellsUsed, pObj)
    {
        TableViewCell* pCell = static_cast<TableViewCell*>(pObj);
        log("cells Used index %d, value = %d", i, pCell->getIdx());
        i++;
    }
    log("---------------------------------------");
    i = 0;
    CCARRAY_FOREACH(_cellsFreed, pObj)
    {
        TableViewCell* pCell = static_cast<TableViewCell*>(pObj);
        log("cells freed index %d, value = %d", i, pCell->getIdx());
        i++;
    }
    log("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
#endif
    
    if (!_cellsUsed.empty())
    {
        auto cell = _cellsUsed.at(0);
        idx = cell->getIdx();
        
        ssize_t lChangeStartIdx=startIdx-5;
        if(lChangeStartIdx<0)
        {
            lChangeStartIdx=0;
        }
        
        while(idx < lChangeStartIdx)
        {
            STTableViewDelegate* lTableViewDelegate=dynamic_cast<STTableViewDelegate*>(_tableViewDelegate);
            if(lTableViewDelegate!=NULL)
            {
                lTableViewDelegate->tableCellRemoveTexture(this, cell);
            }
            this->_moveCellOutOfSight(cell);
            if (!_cellsUsed.empty())
            {
                cell = _cellsUsed.at(0);
                idx = cell->getIdx();
            }
            else
            {
                break;
            }
        }
        
//        while(idx < startIdx)
//        {
//            this->_moveCellOutOfSight(cell);
//            if (!_cellsUsed.empty())
//            {
//                cell = _cellsUsed.at(0);
//                idx = cell->getIdx();
//            }
//            else
//            {
//                break;
//            }
//        }
    }
    if (!_cellsUsed.empty())
    {
        auto cell = _cellsUsed.back();
        idx = cell->getIdx();
        
        ssize_t lChangeEndIdx=endIdx+5;
        if(lChangeEndIdx>maxIdx)
        {
            lChangeEndIdx=maxIdx;
        }
        while(idx <= maxIdx && idx > lChangeEndIdx)
        {
            STTableViewDelegate* lTableViewDelegate=dynamic_cast<STTableViewDelegate*>(_tableViewDelegate);
            if(lTableViewDelegate!=NULL)
            {
                lTableViewDelegate->tableCellRemoveTexture(this, cell);
            }
            this->_moveCellOutOfSight(cell);
            if (!_cellsUsed.empty())
            {
                cell = _cellsUsed.back();
                idx = cell->getIdx();
            }
            else
            {
                break;
            }
        }
//        while(idx <= maxIdx && idx > endIdx)
//        {
//            this->_moveCellOutOfSight(cell);
//            if (!_cellsUsed.empty())
//            {
//                cell = _cellsUsed.back();
//                idx = cell->getIdx();
//            }
//            else
//            {
//                break;
//            }
//        }
    }
    
    
    
    for (long i = MAX(startIdx-5,0); i <= MIN(endIdx+5,maxIdx); i++)
    {
        if (_indices->find(i) != _indices->end())
        {
            continue;
        }
        this->updateCellAtIndex(i);
    }
    
    
    
//    if(endIdx+5>maxIdx)
//    {
//        STTableViewDelegate* lTableViewDelegate=dynamic_cast<STTableViewDelegate*>(_tableViewDelegate);
//        if(lTableViewDelegate!=NULL)
//        {
//            lTableViewDelegate->tableRequestNextPage(this,endIdx+5);
//        }
//    }
}


void STTableView::onTouchEnded(Touch *pTouch, Event *pEvent)
{
    if (!this->isVisible()) {
        return;
    }
    
    if (_touchedCell){
        Rect bb = this->getBoundingBox();
        bb.origin = _parent->convertToWorldSpace(bb.origin);
        
        if (bb.containsPoint(pTouch->getLocation()) && _tableViewDelegate != nullptr)
        {
            STTableViewDelegate* lTableViewDelegate=dynamic_cast<STTableViewDelegate*>(_tableViewDelegate);
            
            _tableViewDelegate->tableCellUnhighlight(this, _touchedCell);
           // _tableViewDelegate->tableCellTouched(this, _touchedCell);
            if(lTableViewDelegate!=NULL)
            {
              lTableViewDelegate->tableCellTouched(this, _touchedCell, pTouch);
            }
            else
            {
                _tableViewDelegate->tableCellTouched(this, _touchedCell);
            }
        }
        
        _touchedCell = nullptr;
    }
    
    ScrollView::onTouchEnded(pTouch, pEvent);
}

void STTableView::updateContentSize()
{
    Vec2 offset=this->minContainerOffset();
    

    _oldDirection = Direction::NONE;
    
    for(const auto &cell : _cellsUsed) {
        if(_tableViewDelegate != nullptr) {
            _tableViewDelegate->tableCellWillRecycle(this, cell);
        }
        
        _cellsFreed.pushBack(cell);
        
        cell->reset();
        if (cell->getParent() == this->getContainer())
        {
            this->getContainer()->removeChild(cell, true);
        }
    }
    
    _indices->clear();
    _cellsUsed.clear();
    
    this->_updateCellPositions();
    this->_updateContentSize();
    

    this->setContentOffset(offset+Vec2(0, _viewSize.height));
    
    if (_dataSource->numberOfCellsInTableView(this) > 0)
    {
        this->scrollViewDidScroll(this);
    }

    
}

void STTableView::setIsUpdateShow(bool pBool)
{
    _isUpdateShow=pBool;
    if(_isUpdateShow)
    {
        this->scheduleUpdate();
    }
    else
    {
        this->unscheduleUpdate();
    }
}

void STTableView::update(float dt)
{
    if(!_cellsUsed.empty())
    {
        for (int i=0; i<_cellsUsed.size(); i++) {
            TableViewCell* lTableViewCell=_cellsUsed.at(i);
            if(lTableViewCell!=NULL)
            {
                STTableViewDelegate* lTableViewDelegate=dynamic_cast<STTableViewDelegate*>(_tableViewDelegate);
                if(lTableViewDelegate != nullptr) {
                    lTableViewDelegate->tableUpdateFunc(this, lTableViewCell);
                }
            }
        }
    }
}

