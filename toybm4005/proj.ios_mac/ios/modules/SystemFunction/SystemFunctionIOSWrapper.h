//
//  SystemFunctionIOSWrapper.h
//  MysteryGuardians
//
//  Created by Steven.Tian on 13-7-22.
//
//

#import <Foundation/Foundation.h>
#import <MessageUI/MessageUI.h>

@interface SystemFunctionManager : NSObject<MFMailComposeViewControllerDelegate>
{
}

+(SystemFunctionManager*)sharedManager;
+(void)purgeManager;

/**
 *  @brief Send Email by system default.
 *
 *  @param subject email subject.
 *  @param content email subject.
 */
-(void)sendEmail:(NSString*)subject Content:(NSString*) content;

-(void)popAlertDialog:(NSString *)message;

-(void)showMorePage;

-(void)go2Market:(NSString *)url;

-(void)rateUs:(NSString*)appleID Message:(NSString*) message;

-(void)go2PrivacyPage;

-(void)openURL:(NSString *)url;

-(void)sendEmailAndFilePic:(NSString*)subject Message:(NSString*)message ImagePath:(NSString*)imagePath;

-(void)sendPictureToInstagram:(NSString*)imagePath;

-(void)saveLocalImage:(cocos2d::Image *)image;
-(UIImage*)covertLocalImage:(cocos2d::Image *)image;

@end
