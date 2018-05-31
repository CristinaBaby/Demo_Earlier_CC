//
//  BaseLayer.hpp
//  ColorGarden
//
//  Created by lidong on 15/11/20.
//
//

#ifndef BaseLayer_hpp
#define BaseLayer_hpp

#include <stdio.h>
#include "cocos2d.h"
USING_NS_CC;

class BaseLayer : public Layer {
    
protected:
    bool _isAlreadyShowScene;
    
public:
    BaseLayer();
    ~BaseLayer();
    bool init();
    void responseIsDownloadNotification(Ref* pSender);
    void removeDialog(Node* pSender);
    void startDownload(Node* pSender);
    void downloadFinish(Ref *pSender);
    void downloadFinishRemoveDialog(Node* pSender);
    void noNewVersionReadCsvFinish(Ref *pSender);
    virtual void refresh(){}
};

#endif /* BaseLayer_hpp */
