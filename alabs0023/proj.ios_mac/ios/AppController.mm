/****************************************************************************
 Copyright (c) 2010 cocos2d-x.org

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#import "AppController.h"
#import "CCEAGLView.h"
#import "cocos2d.h"
#import "AppDelegate.h"
#import "RootViewController.h"
//#include <AnalyticsTool/AnalyticsTool.h>
#include <AnalyticsSDK/AnalyticsSDK.h>
//#import "VersionChecker.h"
//#import "AnalyticsTool.h"
#import "AudioToolbox/AudioServices.h"
#import "NewsBlast.h"
@implementation AppController
@synthesize window;
@synthesize viewController=_viewController;
@synthesize myConnection=_myConnection;
@synthesize isOpenWebsite=_isOpenWebsite;
#pragma mark -
#pragma mark Application lifecycle

// cocos2d application instance
static AppDelegate s_sharedApplication;

+(AppController*)sharedAppController
{
    return (AppController*)[UIApplication sharedApplication].delegate;
}

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {    

    // Override point for customization after application launch.

    // Add the view controller's view to the window and display.
    window = [[UIWindow alloc] initWithFrame: [[UIScreen mainScreen] bounds]];

    // Init the CCEAGLView
    CCEAGLView *eaglView = [CCEAGLView viewWithFrame: [window bounds]
                                     pixelFormat: kEAGLColorFormatRGBA8
                                     depthFormat: GL_DEPTH24_STENCIL8_OES
                              preserveBackbuffer: NO
                                      sharegroup: nil
                                   multiSampling: NO
                                 numberOfSamples: 0];
    // Use RootViewController manage CCEAGLView 
    _viewController = [[RootViewController alloc] initWithNibName:nil bundle:nil];
    _viewController.wantsFullScreenLayout = YES;
    _viewController.view = eaglView;

    [eaglView setMultipleTouchEnabled:TRUE];
    
    // Set RootViewController to window
    if ( [[UIDevice currentDevice].systemVersion floatValue] < 6.0)
    {
        // warning: addSubView doesn't work on iOS6
        [window addSubview: _viewController.view];
    }
    else
    {
        // use this method on ios6
        [window setRootViewController:_viewController];
    }

    [window makeKeyAndVisible];

    [[UIApplication sharedApplication] setStatusBarHidden:true];

    // IMPORTANT: Setting the GLView should be done after creating the RootViewController
    cocos2d::GLView *glview = cocos2d::GLView::createWithEAGLView(eaglView);
    cocos2d::Director::getInstance()->setOpenGLView(glview);

    cocos2d::Application::getInstance()->run();

    
    //版本检查
    //[[VersionChecker sharedVersionChecker] startChecking:self];
    //公司业务 newsblast的弹出放在版本检查的回调中执行，不提示版本更新的情况下再弹newsblast
    //[self performSelector:@selector(beginSession) withObject:nil afterDelay:3.0];
    
    // 注册接收远程通知
    [[UIApplication sharedApplication] registerForRemoteNotificationTypes:(UIRemoteNotificationTypeAlert | UIRemoteNotificationTypeBadge | UIRemoteNotificationTypeSound)];
    
//    [[AnalyticsTool getInstance] startLoggingWithSendData:NO];
    
    [self beginSession];
    
//    [[Analytics getInstance] setDebugMode:true];
    
//    [[Analytics getInstance] startSession:AT_FLURRY|AT_NATIVE|AT_GOOGLE];
    
    NSString* lsTR=[NSString stringWithUTF8String:cocos2d::FileUtils::getInstance()->getWritablePath().c_str()];
    
    NSURL* lUrl=[[NSURL alloc] initFileURLWithPath:lsTR];
    [self addSkipBackupAttributeToItemAtURL:lUrl];
    
    [super launchingWorkOnDebugMode:YES];
    return YES;
}

- (BOOL)addSkipBackupAttributeToItemAtURL:(NSURL *)URL
{
    assert([[NSFileManager defaultManager] fileExistsAtPath: [URL path]]);
    
    NSError *error = nil;
    BOOL success = [URL setResourceValue: [NSNumber numberWithBool: YES]
                                  forKey: NSURLIsExcludedFromBackupKey error: &error];
    if(!success){
        NSLog(@"Error excluding %@ from backup %@", [URL lastPathComponent], error);
    }
    return success;
}

/*
- (void)versionCheckerDidShow:(VersionChecker *)versionChecker
{
    NSLog(@"versionCheckerDidShow");
}

- (void)versionCheckerDidNotShow:(VersionChecker *)versionChecker
{
    NSLog(@"versionCheckerDidNotShow");
    //公司业务 newsblast的弹出放在版本检查的回调中执行，不提示版本更新的情况下再弹newsblast
    [self beginSession];
}

- (void)versionCheckerDidDismiss:(VersionChecker *)versionChecker
{
    NSLog(@"versionCheckerDidDismiss");
}
*/
//公司业务
-(void)beginSession
{
    [NewsBlast startNews];
    [[NewsBlast shareNews] setInvokeMode:InvokeModeResume];
}


-(void)requestWebsite
{
    NSString *urlString=@"http://www.facebook.com";
    NSURL * url = [NSURL URLWithString:urlString];
    NSURLRequest *request = [[NSURLRequest alloc] initWithURL:url];//通过url，获得request;
    self.myConnection = [[NSURLConnection alloc] initWithRequest:request
                                                        delegate:self];//根据request得到connection;
    [request release];
}

-(void)connection:(NSURLConnection *)connection didReceiveData:(NSData *)data {
    
    self.isOpenWebsite=TRUE;
    
}

- (void)connectionDidFinishLoading:(NSURLConnection*)connection {
    
    //NSLog(@"finish: %d",[connection retainCount]);
    //NSLog(@"%@",[[NSString alloc] initWithData:_data encoding:NSUTF8StringEncoding]);
    self.myConnection = nil;
    self.isOpenWebsite=TRUE;
    
    
}


- (void)applicationWillResignActive:(UIApplication *)application {
    [super applicationWillResignActive:application];
    /*
     Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
     Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
     */
     //We don't need to call this method any more. It will interupt user defined game pause&resume logic
    /* cocos2d::Director::getInstance()->pause(); */
}

- (void)applicationDidBecomeActive:(UIApplication *)application {
    [super applicationDidBecomeActive:application];
    /*
     Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
     */
     //We don't need to call this method any more. It will interupt user defined game pause&resume logic
    /* cocos2d::Director::getInstance()->resume(); */
    
      [self requestWebsite];
    
}

- (void)applicationDidEnterBackground:(UIApplication *)application {
    [super applicationDidEnterBackground:application];
    /*
     Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later. 
     If your application supports background execution, called instead of applicationWillTerminate: when the user quits.
     */
    cocos2d::Application::getInstance()->applicationDidEnterBackground();
}

- (void)applicationWillEnterForeground:(UIApplication *)application {
    /*
     Called as part of  transition from the background to the inactive state: here you can undo many of the changes made on entering the background.
     */
    cocos2d::Application::getInstance()->applicationWillEnterForeground();
}

- (void)applicationWillTerminate:(UIApplication *)application {
    [super applicationWillTerminate:application];
    /*
     Called when the application is about to terminate.
     See also applicationDidEnterBackground:.
     */
}


#pragma mark -
#pragma mark Memory management

- (void)applicationDidReceiveMemoryWarning:(UIApplication *)application {
    /*
     Free up as much memory as possible by purging cached data objects that can be recreated (or reloaded from disk) later.
     */
}

#pragma mark 远程通知
// Retrieve the device token
- (void)application:(UIApplication *)application didRegisterForRemoteNotificationsWithDeviceToken:(NSData *)devToken
{
    //set APNS Token
    NSString *token= [[[[devToken description] stringByReplacingOccurrencesOfString: @"<" withString: @""]
                       stringByReplacingOccurrencesOfString: @">" withString: @""]
                      stringByReplacingOccurrencesOfString: @" " withString: @""];
    
    
    NSString *bundleID = [[NSBundle mainBundle] bundleIdentifier];
    NSArray *bundleIDArray = [bundleID componentsSeparatedByString:@"."];
    
    NSString *urlString = [NSString stringWithFormat:@"http://services.%@.%@/push_gateway/register?token=%@&app=%@",
                           [bundleIDArray objectAtIndex:1], [bundleIDArray objectAtIndex:0], token, [bundleIDArray lastObject]];
    
    NSURL *url = [NSURL URLWithString:urlString];
    [NSURLConnection connectionWithRequest:[NSURLRequest requestWithURL:url] delegate:nil];
}

// Handle an actual notification
- (void)application:(UIApplication *)application didReceiveRemoteNotification:(NSDictionary *)userInfo
{
    //	NSString *status = [NSString stringWithFormat:@"Notification received:\n%@",[userInfo description]];
    //接收到push  打开程序以后设置badge的值
    NSString *badgeStr = [[userInfo objectForKey:@"aps"] objectForKey:@"badge"];
    if (badgeStr != nil)
        [UIApplication sharedApplication].applicationIconBadgeNumber = [badgeStr intValue];
    //接收到push  打开程序以后会震动
    AudioServicesPlaySystemSound(kSystemSoundID_Vibrate);
    
}

-(void)application:(UIApplication *)app didFailToRegisterForRemoteNotificationsWithError:(NSError *)err
{
    //
    NSLog(@"Error in registration. Error: %@", err);
    //
}

#if __IPHONE_OS_VERSION_MAX_ALLOWED >= __IPHONE_6_0

- (NSUInteger)application:(UIApplication *)application supportedInterfaceOrientationsForWindow:(UIWindow *)window
{
    return UIInterfaceOrientationMaskPortrait;
}

#endif

- (void)dealloc {
    [window release];
    [super dealloc];
}


@end
