//
//  STSystemFunction.h
//  MysteryGuardians
//
//  Created by Steven.Xc.Tian on 13-7-9.
//
//

#ifndef __MysteryGuardians__SystemFunction__
#define __MysteryGuardians__SystemFunction__
#include <string>



class STSystemFunction
{
public:
	STSystemFunction();
	~STSystemFunction();
public:
    /**
     *  @brief Send Email by system default.
     *
     *  @param subject email subject.
     *  @param content email subject.
     */
    virtual void sendEmail(const char* subject, const char* body);
    
    /**
     *
     */
    virtual void popAlertDialog(const char* message);
    virtual void makeToast(const char* message);
    
    /**
     *
     */
    virtual void showMoreGame();
	virtual void rating(const char* appleID, const char* message);
    virtual void rating();
	virtual void go2MarketDetail(std::string packagename);
	virtual void contactUs();
	virtual void sendEmailAndAssetPic(std::string subject, std::string message, std::string assetName);
	virtual void sendEmailAndFilePic(std::string subject, std::string message, std::string fileName);
    virtual void sendPictureToInstagram(std::string fileName);
    virtual void sendPictureToMore(std::string fileName);
	std::string getSdCardPath();
	virtual bool checkNetworkAvailable();
	virtual bool isTabletAvailable();
	virtual float densityScale();
    
    virtual void go2PrivacyPage();
    virtual void openURL(std::string url);
    virtual void endSession();
    
    virtual void saveLocalImage(void* image);
    
    void refreshDCIM(std::string filename);
    
    //删除Document下的文件及其子文件
    void deleteDocumentFile(std::string filename);

    
};

#endif /* defined(__MysteryGuardians__SystemFunction__) */
