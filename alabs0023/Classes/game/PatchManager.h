//
//  PatchManager.h
//  ColorBook
//
//  Created by maxiang on 4/24/15.
//
//

#ifndef __ColorBook__PatchManager__
#define __ColorBook__PatchManager__

#include "UIHeader.h"
#include <deque>

#define xPatch  (PatchManager::getInstance())

typedef std::function<void(int, void*)> ccPatchCallback;

class PatchManager : public Node
{
    struct Patch
    {
        int fromVersion;
        int toVersion;
        ccPatchCallback callback;
    };
    
public:
    static PatchManager* getInstance();
    void destroyInstance();
    void patchApp(int iData, void* vData);
    void patchChoose(int iData, void* vData);
    void patchFinish();
    
    
protected:
    PatchManager();
    void findPatch();
    void app_2(int iData, void* vData);
    void choose_2(int iData, void* vData);

protected:
    int _fromVersion = 1;
    int _toVersion = 2;
    string _writablePath;
    
    vector<Patch> _patchsApp;
    vector<Patch> _patchsChoose;
    
    ccPatchCallback _patchApp = nullptr;
    ccPatchCallback _patchChoose = nullptr;
};

#endif /* defined(__ColorBook__PatchManager__) */
