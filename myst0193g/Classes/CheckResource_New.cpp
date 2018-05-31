//
//  CheckResource_New.c
//  Card
//
//  Created by lidong on 15-1-6.
//
//

#include "CheckResource_New.h"
#include <math.h>
#include <string.h>
#include "cocos2d.h"
USING_NS_CC;

#ifdef VERSION_SUPPORT_215
   #define IsExist_func(filename) CCFileUtils::sharedFileUtils()->isFileExist(filename)
#else
   #define IsExist_func(filename) FileUtils::getInstance()->isFileExist(filename)
#endif



using namespace std;

void checkResource(int mapLevel)
{
    
    printf("--------------------------------------------------------\n");
    printf("----------------开始扫描---------------------------------\n");
    printf("\n");
    
    //一般的资源检查
    string scanfilename[]={
        //字体文件
        "fonts/Lucida Bright.ttf",
        
        //声音文件
        "sounds/game_complete.mp3",
        "sounds/game_over.mp3",
        "sounds/hint.mp3",
        "sounds/in_game.mp3",
        "sounds/popup.mp3",
        "sounds/skill_bonus.mp3",
        "sounds/tap_button.mp3",
        "sounds/tap_correct.mp3",
        "sounds/tap_error.mp3",
        "sounds/turn_page.mp3",
    };
    int scanLength = sizeof(scanfilename) / sizeof(scanfilename[0]);
    for (int i=0; i<scanLength; i++)
    {
        string filename=scanfilename[i];
        if (filename.empty() || IsExist_func(filename)==false)
        {
            printf("file:%s 不存在\n",filename.c_str());
        }
    }
    
    checkResourceUI_1024("ui-1024X768",mapLevel);
    checkResourceUI_2048("ui-2048X1536",mapLevel);
    
    
    printf("----------------扫描结束---------------------------------\n");
    
}




void scanPlistContentFile_Func(vector<string> plistVector,string mPlistFilename,string dir)
{
    vector<string>::iterator iter;
    for(iter=plistVector.begin();iter!=plistVector.end();iter++)
    {
        string filename=*iter;                    //要检查的plist内部文件名
        bool lIsFind=false;                      //判断是否能够从所有plist文件中找到该文件
        string plistFileName=mPlistFilename;
        
#ifdef VERSION_SUPPORT_215
        
        if(IsExist_func(plistFileName)==true)
        {
            std::string fullPath = CCFileUtils::sharedFileUtils()->fullPathForFilename(plistFileName.c_str());
            CCDictionary* lDict=CCDictionary::createWithContentsOfFile(fullPath.c_str());
            CCDictionary* lFrames=(CCDictionary*)lDict->objectForKey("frames");
            CCArray* lArray=lFrames->allKeys();
            for (int i=0; i<lArray->count(); i++) {
                CCString* lFileName=(CCString*)lArray->objectAtIndex(i);
                if(lFileName->m_sString==filename)
                {
                    lIsFind=true;
                    break;
                }
            }
        }
        
#else
        
        if(IsExist_func(plistFileName)==true)
        {
            std::string fullPath = FileUtils::getInstance()->fullPathForFilename(plistFileName);
            ValueMap dict = FileUtils::getInstance()->getValueMapFromFile(fullPath);
            ValueMap& framesDict = dict["frames"].asValueMap();
            ValueMap::iterator iter;
            
            for (iter=framesDict.begin(); iter!=framesDict.end(); iter++) {
                string str=iter->first;
                if(str==filename)
                {
                    lIsFind=true;
                    break;
                }
            }
        }
#endif
        if(!lIsFind)
        {
            printf("dir=%s file:%s 不能在在所有的plist文件中找到 \n",dir.c_str(),filename.c_str());
        }
    }
}



void scanPlistContentFile_Func(string* pScanPlistContentFiles,int scanPlistContentLength,string mPlistFilename,string dir)
{
    for (int i=0; i<scanPlistContentLength; i++) {
        
        string filename=pScanPlistContentFiles[i]; //要检查的plist内部文件名
        bool lIsFind=false;                      //判断是否能够从所有plist文件中找到该文件
        
        string plistFileName=dir+"/"+mPlistFilename;
        
#ifdef VERSION_SUPPORT_215
        
        if(IsExist_func(plistFileName)==true)
        {
            std::string fullPath = CCFileUtils::sharedFileUtils()->fullPathForFilename(plistFileName.c_str());
            CCDictionary* lDict=CCDictionary::createWithContentsOfFile(fullPath.c_str());
            CCDictionary* lFrames=(CCDictionary*)lDict->objectForKey("frames");
            CCArray* lArray=lFrames->allKeys();
            for (int i=0; i<lArray->count(); i++) {
                CCString* lFileName=(CCString*)lArray->objectAtIndex(i);
                if(lFileName->m_sString==filename)
                {
                    lIsFind=true;
                    break;
                }
            }
        }
        
#else
        
        if(IsExist_func(plistFileName)==true)
        {
            std::string fullPath = FileUtils::getInstance()->fullPathForFilename(plistFileName);
            ValueMap dict = FileUtils::getInstance()->getValueMapFromFile(fullPath);
            ValueMap& framesDict = dict["frames"].asValueMap();
            ValueMap::iterator iter;
            
            for (iter=framesDict.begin(); iter!=framesDict.end(); iter++) {
                string str=iter->first;
                if(str==filename)
                {
                    lIsFind=true;
                    break;
                }
            }
        }
        
#endif
        
        if(!lIsFind)
        {
            printf("dir=%s file:%s 不能在在所有的plist文件中找到 \n",dir.c_str(),filename.c_str());
        }
    }
}


void checkMapLevel(string dir,int currentLevel,bool checkIsIpadHD)
{
    vector<string> resource;
    vector<string> plistVector;
    
    string mDir=dir+"/"+"maps/";
    
    char buffer_displayname[100];
    char buffer_level[20];
    char buffer_thumb[100];
    char buffer_bg[100];
    char buffer_picture[100];
    char buffer_picture_pvr[100];
    char buffer_silhouette[100];
    char buffer_silhouette_pvr[100];
    char buffer_word[100];
    char buffer_word_pvr[100];
    char buffer_tmx[100];
    
    int i=currentLevel;
    
    if(currentLevel>=10)
    {
        sprintf(buffer_level, "L%d",i);
        
        sprintf(buffer_displayname,"DisplayName.csv");
        sprintf(buffer_thumb, "L%d_bg_thumb.jpg",i);
        sprintf(buffer_bg, "L%d_bg.jpg",i);
        sprintf(buffer_picture, "L%d_picture.plist",i);
        sprintf(buffer_picture_pvr, "L%d_picture.pvr.ccz",i);
        
        sprintf(buffer_silhouette, "L%d_silhouette.plist",i);
        sprintf(buffer_silhouette_pvr, "L%d_silhouette.pvr.ccz",i);
        sprintf(buffer_word, "L%d_word.plist",i);
        sprintf(buffer_word_pvr, "L%d_word.pvr.ccz",i);
        
        sprintf(buffer_tmx, "L%d.tmx",i);
    }
    
    else
    {
        sprintf(buffer_level, "L0%d",i);
        
        sprintf(buffer_displayname,"DisplayName.csv");
        sprintf(buffer_thumb, "L0%d_bg_thumb.jpg",i);
        sprintf(buffer_bg, "L0%d_bg.jpg",i);
        sprintf(buffer_picture, "L0%d_picture.plist",i);
        sprintf(buffer_picture_pvr, "L0%d_picture.pvr.ccz",i);
        
        sprintf(buffer_silhouette, "L0%d_silhouette.plist",i);
        sprintf(buffer_silhouette_pvr, "L0%d_silhouette.pvr.ccz",i);
        sprintf(buffer_word, "L0%d_word.plist",i);
        sprintf(buffer_word_pvr, "L0%d_word.pvr.ccz",i);
        
        sprintf(buffer_tmx, "L0%d.tmx",i);
    }
    
    string str_level=buffer_level;
    
    string str_displayname      =mDir    +   str_level      +"/"+   buffer_displayname;
    string str_thumb            =mDir    +   str_level      +"/"+   buffer_thumb;
    string str_bg               =mDir    +   str_level      +"/"+   buffer_bg;
    string str_picture          =mDir    +   str_level      +"/"+   buffer_picture;
    string str_picture_pvr      =mDir    +   str_level      +"/"+   buffer_picture_pvr;
    string str_silhouette       =mDir    +   str_level      +"/"+   buffer_silhouette;
    string str_silhouette_pvr   =mDir    +   str_level      +"/"+   buffer_silhouette_pvr;
    string str_word             =mDir    +   str_level      +"/"+   buffer_word;
    string str_word_pvr         =mDir    +   str_level      +"/"+   buffer_word_pvr;
    string str_tmx              =mDir    +   str_level      +"/"+   buffer_tmx;
    
    if(checkIsIpadHD)
    {
        resource.push_back(str_displayname);
    }
    resource.push_back(str_thumb);
    resource.push_back(str_bg);
    resource.push_back(str_picture);
    resource.push_back(str_picture_pvr);
    resource.push_back(str_silhouette);
    resource.push_back(str_silhouette_pvr);
    resource.push_back(str_word);
    resource.push_back(str_word_pvr);
    
    if(!checkIsIpadHD) resource.push_back(str_tmx);
    
    plistVector.push_back(str_picture);
    plistVector.push_back(str_silhouette);
    plistVector.push_back(str_word);
    
    vector<string>::iterator iter;
    for (iter=resource.begin(); iter!=resource.end(); iter++) {
        string filename=*iter;
        if(IsExist_func(filename)==false)
        {
            printf("file:%s 不存在\n",filename.c_str());
        }
    }
    
    
    vector<string> filenames_p;
    vector<string> filenames_s;
    vector<string> filenames_w;
    
    for (int j=1; j<=32; j++)
    {
        char buffer_picture_content[100];
        char buffer_silhouette_content[100];
        char buffer_word_content[100];
        
        if(i>=10)
        {
            if(j>=10)
            {
                sprintf(buffer_picture_content, "L%d_%d_p.png",i,j);
                sprintf(buffer_silhouette_content, "L%d_%d_s.png",i,j);
                sprintf(buffer_word_content, "L%d_%d_w.png",i,j);
            }
            else
            {
                sprintf(buffer_picture_content, "L%d_0%d_p.png",i,j);
                sprintf(buffer_silhouette_content, "L%d_0%d_s.png",i,j);
                sprintf(buffer_word_content, "L%d_0%d_w.png",i,j);
            }
        }
        else
        {
            if(j>=10)
            {
                sprintf(buffer_picture_content, "L0%d_%d_p.png",i,j);
                sprintf(buffer_silhouette_content, "L0%d_%d_s.png",i,j);
                sprintf(buffer_word_content, "L0%d_%d_w.png",i,j);
            }
            else
            {
                sprintf(buffer_picture_content, "L0%d_0%d_p.png",i,j);
                sprintf(buffer_silhouette_content, "L0%d_0%d_s.png",i,j);
                sprintf(buffer_word_content, "L0%d_0%d_w.png",i,j);
            }
        }
        
        string str_picture=buffer_picture_content;
        string str_silhouette=buffer_silhouette_content;
        string str_word=buffer_word_content;
        
        filenames_p.push_back(str_picture);
        filenames_s.push_back(str_silhouette);
        filenames_w.push_back(str_word);
    }
    
    
    scanPlistContentFile_Func(filenames_p,plistVector[0],mDir);
    scanPlistContentFile_Func(filenames_s,plistVector[1],mDir);
    scanPlistContentFile_Func(filenames_w,plistVector[2],mDir);
    
}

void checkMap(string dir,int mapLevel,bool checkIsIpadHD)
{
    for (int i=1; i<=mapLevel; i++) {
        
        checkMapLevel(dir,i,checkIsIpadHD);
    }
    
}



void checkResourceUI_2048(string dir,int mapLevel)
{
    //一般的资源检查
    string scanfilename[]={
        //字体文件
        "choose-map.pvr.ccz",
        "in-game.pvr.ccz",
        "load-home.pvr.ccz",
        "pop-window.pvr.ccz",
        
        "ui01_menu_bg.jpg",
        "ui03_bg.jpg",
        "ui05_box.png",
        
        "tutorial/tutorial_1.jpg",
        "tutorial/tutorial_2.jpg",
        "tutorial/tutorial_3.jpg",
        "tutorial/tutorial_4.jpg",
        "tutorial/tutorial_5.jpg",
        "tutorial/tutorial_skip.png",
        
        "particles/hint.plist",
        "particles/particleTexture.png",
        "particles/skill_bonus.plist",
        "particles/tap_correct.plist",
        
        "sku/sku.csv",
        "sku/sku.pvr.ccz",
        
    };
    
    
    int scanLength = sizeof(scanfilename) / sizeof(scanfilename[0]);
    for (int i=0; i<scanLength; i++)
    {
        string filename=scanfilename[i];
        filename=dir+"/"+filename;
        if (filename.empty() || IsExist_func(filename)==false)
        {
            printf("file:%s 不存在\n",filename.c_str());
        }
    }
    
    //Plist文件检查
    string scanPlistFilename[]={
        "choose-map.plist",
        "in-game.plist",
        "load-home.plist",
        "pop-window.plist",
        "sku/sku.plist",
    };
    
    int scanPlistLength=sizeof(scanPlistFilename)/sizeof(scanPlistFilename[0]);
    for (int i=0; i<scanPlistLength; i++) {
        string filename=scanPlistFilename[i];
        filename=dir+"/"+filename;
        if (filename.empty() || IsExist_func(filename)==false)
        {
            printf("file:%s 不存在\n",filename.c_str());
        }
    }
    
    //Plist里面的纹理文件检查
    string scanPlistContentFile1[]={
        
        //choose-map.plist
        "dot_1.png",
        "dot_2.png",
        "ui02_button.png",
        "ui02_button_h.png",
        "ui02_choose_mode.png",
        "ui02_picture.png",
        "ui02_silhouette.png",
        "ui02_word.png",
        "ui03_back.png",
        "ui03_back_h.png",
        "ui03_frame.png",
        "ui03_lock.png",
        "ui03_star.png",
        "ui03_star_u.png",
    };
    
    int scanPlistContentFile1_length=sizeof(scanPlistContentFile1)/sizeof(scanPlistContentFile1[0]);
    for (int i=0; i<scanPlistContentFile1_length; i++) {
        string filename=scanPlistContentFile1[i];
        filename=dir+"/choose-map"+"/"+filename;
        if (filename.empty() || IsExist_func(filename)==false)
        {
            printf("file:%s 不存在\n",filename.c_str());
        }
    }
    
    
    string scanPlistContentFile2[]={
        //in-game.plist
        "ui04_bar.png",
        "ui04_frame.png",
        "ui04_frame_h.png",
        "ui04_hint.png",
        "ui04_hint_u.png",
        "ui04_next.png",
        "ui04_next_h.png",
        "ui04_number.png",
        "ui04_pause.png",
        "ui04_plus.png",
        "ui04_previous.png",
        "ui04_previous_h.png",
        "ui04_wrong.png",
        "ui05_game_complete.png",
        "ui05_game_over.png",
        "ui05_high_score.png",
        "ui05_next.png",
        "ui05_next_h.png",
        "ui05_replay.png",
        "ui05_replay_h.png",
        "ui05_score.png",
        "ui05_text_bg.png",
        "ui05_time.png",
        "ui06_menu.png",
        "ui06_menu_h.png",
        "ui06_pause_bg.png",
        "ui06_play.png",
        "ui06_play_h.png",
        "ui06_replay.png",
        "ui06_replay_h.png",
        "ui06_settings.png",
        "ui06_settings_h.png",
    };
    int scanPlistContentFile2_length=sizeof(scanPlistContentFile2)/sizeof(scanPlistContentFile2[0]);
    for (int i=0; i<scanPlistContentFile2_length; i++) {
        string filename=scanPlistContentFile2[i];
        filename=dir+"/in-game"+"/"+filename;
        if (filename.empty() || IsExist_func(filename)==false)
        {
            printf("file:%s 不存在\n",filename.c_str());
        }
    }
    
    
    string scanPlistContentFile3[]={
        //load-home.plist
        "ui01_btn_deluxe.png",
        "ui01_btn_deluxe_h.png",
        "ui01_btn_more.png",
        "ui01_btn_more_h.png",
        "ui01_btn_new_game.png",
        "ui01_btn_new_game_h.png",
        "ui01_btn_play.png",
        "ui01_btn_play_h.png",
        "ui01_btn_resume.png",
        "ui01_btn_resume_h.png",
        "ui01_btn_settings.png",
        "ui01_btn_settings_h.png",
        "ui01_logo.png",
        "ui09_bar.png",
        "ui09_bar_bg.png",
        "ui09_loading.png",
        "ui10_popup.png",
        "ui10_popup_button.png",
        "ui10_popup_button_h.png",
    };
    
    int scanPlistContentFile3_length=sizeof(scanPlistContentFile3)/sizeof(scanPlistContentFile3[0]);
    for (int i=0; i<scanPlistContentFile3_length; i++) {
        string filename=scanPlistContentFile3[i];
        filename=dir+"/load-home"+"/"+filename;
        if (filename.empty() || IsExist_func(filename)==false)
        {
            printf("file:%s 不存在\n",filename.c_str());
        }
    }
    
    
    string scanPlistContentFile4[]={
        //pop-window.plist
        "ui05_box.png",
        "ui07_bar.png",
        "ui07_bar_bg.png",
        "ui07_button.png",
        "ui07_close.png",
        "ui07_close_h.png",
        "ui07_delete.png",
        "ui07_delete_h.png",
        "ui07_line.png",
        "ui07_music.png",
        "ui07_settings.png",
        "ui07_sound.png",
        "ui07_tell_a_friend.png",
        "ui07_tell_a_friend_h.png",
        "ui08_btn_buy.png",
        "ui08_btn_buy_h.png",
        "ui08_btn_restore.png",
        "ui08_btn_restore_h.png",
        "ui08_deluxe.png",
//        "ui08_deluxe_h.png",
//        "ui08_hint.png",
//        "ui08_pictures.png",
    };
    
    int scanPlistContentFile4_length=sizeof(scanPlistContentFile4)/sizeof(scanPlistContentFile4[0]);
    for (int i=0; i<scanPlistContentFile4_length; i++) {
        string filename=scanPlistContentFile4[i];
        filename=dir+"/pop-window"+"/"+filename;
        if (filename.empty() || IsExist_func(filename)==false)
        {
            printf("file:%s 不存在\n",filename.c_str());
        }
    }
    
    string scanPlistContentFile5[]={
        //sku.plist
//        "ui08_hint.png",
        "ui08_noad.png",
//        "ui08_pictures.png",
    };
    int scanPlistContentFile5_length=sizeof(scanPlistContentFile5)/sizeof(scanPlistContentFile5[0]);
    
    
    
    scanPlistContentFile_Func(scanPlistContentFile1,scanPlistContentFile1_length, scanPlistFilename[0], dir);
    scanPlistContentFile_Func(scanPlistContentFile2,scanPlistContentFile2_length, scanPlistFilename[1], dir);
    scanPlistContentFile_Func(scanPlistContentFile3,scanPlistContentFile3_length, scanPlistFilename[2], dir);
    scanPlistContentFile_Func(scanPlistContentFile4,scanPlistContentFile4_length, scanPlistFilename[3], dir);
    
    scanPlistContentFile_Func(scanPlistContentFile5,scanPlistContentFile5_length, scanPlistFilename[4], dir);
    
    checkMap(dir, mapLevel,true);
    
    
}



void checkResourceUI_1024(string dir,int mapLevel)
{
    //一般的资源检查
    string scanfilename[]={
        //字体文件
        "choose-map.pvr.ccz",
        "in-game.pvr.ccz",
        "load-home.pvr.ccz",
        "pop-window.pvr.ccz",
        
        "ui01_menu_bg.jpg",
        "ui03_bg.jpg",
        "ui05_box.png",
        
        "tutorial/tutorial_1.jpg",
        "tutorial/tutorial_2.jpg",
        "tutorial/tutorial_3.jpg",
        "tutorial/tutorial_4.jpg",
        "tutorial/tutorial_5.jpg",
        "tutorial/tutorial_skip.png",
        
        "strings/DisplayName.csv",
        "strings/strings.csv",
        
        "particles/hint.plist",
        "particles/particleTexture.png",
        "particles/skill_bonus.plist",
        "particles/tap_correct.plist",
        
        "maps/Description.csv",
        
        "sku/sku.csv",
        "sku/sku.pvr.ccz",
    };
    
    
    int scanLength = sizeof(scanfilename) / sizeof(scanfilename[0]);
    for (int i=0; i<scanLength; i++)
    {
        string filename=scanfilename[i];
        filename=dir+"/"+filename;
        if (filename.empty() || IsExist_func(filename)==false)
        {
            printf("file:%s 不存在\n",filename.c_str());
        }
    }
    
    //Plist文件检查
    string scanPlistFilename[]={
        "choose-map.plist",
        "in-game.plist",
        "load-home.plist",
        "pop-window.plist",
        "sku/sku.plist",
    };
    
    int scanPlistLength=sizeof(scanPlistFilename)/sizeof(scanPlistFilename[0]);
    for (int i=0; i<scanPlistLength; i++) {
        string filename=scanPlistFilename[i];
        filename=dir+"/"+filename;
        if (filename.empty() || IsExist_func(filename)==false)
        {
            printf("file:%s 不存在\n",filename.c_str());
        }
    }
    
    //Plist里面的纹理文件检查
    string scanPlistContentFile1[]={
        
        //choose-map.plist
        "dot_1.png",
        "dot_2.png",
        "ui02_button.png",
        "ui02_button_h.png",
        "ui02_choose_mode.png",
        "ui02_picture.png",
        "ui02_silhouette.png",
        "ui02_word.png",
        "ui03_back.png",
        "ui03_back_h.png",
        "ui03_frame.png",
        "ui03_lock.png",
        "ui03_star.png",
        "ui03_star_u.png",
    };
    
    int scanPlistContentFile1_length=sizeof(scanPlistContentFile1)/sizeof(scanPlistContentFile1[0]);
    for (int i=0; i<scanPlistContentFile1_length; i++) {
        string filename=scanPlistContentFile1[i];
        filename=dir+"/choose-map"+"/"+filename;
        if (filename.empty() || IsExist_func(filename)==false)
        {
            printf("file:%s 不存在\n",filename.c_str());
        }
    }
    
    
    string scanPlistContentFile2[]={
        //in-game.plist
        "ui04_bar.png",
        "ui04_frame.png",
        "ui04_frame_h.png",
        "ui04_hint.png",
        "ui04_hint_u.png",
        "ui04_next.png",
        "ui04_next_h.png",
        "ui04_number.png",
        "ui04_pause.png",
        "ui04_plus.png",
        "ui04_previous.png",
        "ui04_previous_h.png",
        "ui04_wrong.png",
        "ui05_game_complete.png",
        "ui05_game_over.png",
        "ui05_high_score.png",
        "ui05_next.png",
        "ui05_next_h.png",
        "ui05_replay.png",
        "ui05_replay_h.png",
        "ui05_score.png",
        "ui05_text_bg.png",
        "ui05_time.png",
        "ui06_menu.png",
        "ui06_menu_h.png",
        "ui06_pause_bg.png",
        "ui06_play.png",
        "ui06_play_h.png",
        "ui06_replay.png",
        "ui06_replay_h.png",
        "ui06_settings.png",
        "ui06_settings_h.png",
    };
    
    int scanPlistContentFile2_length=sizeof(scanPlistContentFile2)/sizeof(scanPlistContentFile2[0]);
    for (int i=0; i<scanPlistContentFile2_length; i++) {
        string filename=scanPlistContentFile2[i];
        filename=dir+"/in-game"+"/"+filename;
        if (filename.empty() || IsExist_func(filename)==false)
        {
            printf("file:%s 不存在\n",filename.c_str());
        }
    }
    
    
    string scanPlistContentFile3[]={
        //load-home.plist
        "ui01_btn_deluxe.png",
        "ui01_btn_deluxe_h.png",
        "ui01_btn_more.png",
        "ui01_btn_more_h.png",
        "ui01_btn_new_game.png",
        "ui01_btn_new_game_h.png",
        "ui01_btn_play.png",
        "ui01_btn_play_h.png",
        "ui01_btn_resume.png",
        "ui01_btn_resume_h.png",
        "ui01_btn_settings.png",
        "ui01_btn_settings_h.png",
        "ui01_logo.png",
        "ui09_bar.png",
        "ui09_bar_bg.png",
        "ui09_loading.png",
        "ui10_popup.png",
        "ui10_popup_button.png",
        "ui10_popup_button_h.png",
    };
    
    int scanPlistContentFile3_length=sizeof(scanPlistContentFile3)/sizeof(scanPlistContentFile3[0]);
    for (int i=0; i<scanPlistContentFile3_length; i++) {
        string filename=scanPlistContentFile3[i];
        filename=dir+"/load-home"+"/"+filename;
        if (filename.empty() || IsExist_func(filename)==false)
        {
            printf("file:%s 不存在\n",filename.c_str());
        }
    }
    
    string scanPlistContentFile4[]={
        //pop-window.plist
        "ui05_box.png",
        "ui07_bar.png",
        "ui07_bar_bg.png",
        "ui07_button.png",
        "ui07_close.png",
        "ui07_close_h.png",
        "ui07_delete.png",
        "ui07_delete_h.png",
        "ui07_line.png",
        "ui07_music.png",
        "ui07_settings.png",
        "ui07_sound.png",
        "ui07_tell_a_friend.png",
        "ui07_tell_a_friend_h.png",
        "ui08_btn_buy.png",
        "ui08_btn_buy_h.png",
        "ui08_btn_restore.png",
        "ui08_btn_restore_h.png",
        "ui08_deluxe.png",
//        "ui08_deluxe_h.png",
//        "ui08_hint.png",
//        "ui08_pictures.png",
    };
    
    int scanPlistContentFile4_length=sizeof(scanPlistContentFile4)/sizeof(scanPlistContentFile4[0]);
    for (int i=0; i<scanPlistContentFile4_length; i++) {
        string filename=scanPlistContentFile4[i];
        filename=dir+"/pop-window"+"/"+filename;
        if (filename.empty() || IsExist_func(filename)==false)
        {
            printf("file:%s 不存在\n",filename.c_str());
        }
    }
    
    string scanPlistContentFile5[]={
        //sku.plist
//        "ui08_hint.png",
        "ui08_noad.png",
//        "ui08_pictures.png",
    };
    int scanPlistContentFile5_length=sizeof(scanPlistContentFile5)/sizeof(scanPlistContentFile5[0]);
    
    
    scanPlistContentFile_Func(scanPlistContentFile1,scanPlistContentFile1_length, scanPlistFilename[0], dir);
    scanPlistContentFile_Func(scanPlistContentFile2,scanPlistContentFile2_length, scanPlistFilename[1], dir);
    scanPlistContentFile_Func(scanPlistContentFile3,scanPlistContentFile3_length, scanPlistFilename[2], dir);
    scanPlistContentFile_Func(scanPlistContentFile4,scanPlistContentFile4_length, scanPlistFilename[3], dir);
    
    scanPlistContentFile_Func(scanPlistContentFile5,scanPlistContentFile5_length, scanPlistFilename[4], dir);
    
    checkMap(dir, mapLevel,false);
    
}


