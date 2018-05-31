#import "AppController.h"
#import "EAGLView.h"
#import "cocos2d.h"
#import "AppDelegate.h"
#import "RootViewController.h"
#import "AudioToolbox/AudioServices.h"
#import "AnalyticsTool.h"
//#import "VersionChecker.h"
#import "NewsBlast.h"

@implementation AppController

@synthesize window;
@synthesize viewController;

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
    
    // Init the EAGLView
    EAGLView *__glView = [EAGLView viewWithFrame: [window bounds]
                                     pixelFormat: kEAGLColorFormatRGB565
                                     depthFormat: GL_DEPTH24_STENCIL8_OES
                              preserveBackbuffer: NO
                                      sharegroup: nil
                                   multiSampling: NO
                                 numberOfSamples: 0];
    
    [__glView setMultipleTouchEnabled:true];
    
    // Use RootViewController manage EAGLView
    viewController = [[RootViewController alloc] initWithNibName:nil bundle:nil];
    viewController.wantsFullScreenLayout = YES;
    viewController.view = __glView;
    
    // Set RootViewController to window
    if ( [[UIDevice currentDevice].systemVersion floatValue] < 6.0)
    {
        // warning: addSubView doesn't work on iOS6
        [window addSubview: viewController.view];
    }
    else
    {
        // use this method on ios6
        [window setRootViewController:viewController];
    }
    
    [window makeKeyAndVisible];
    
    [[UIApplication sharedApplication] setStatusBarHidden:true];
    
    cocos2d::CCApplication::sharedApplication()->run();
    
    //版本检查
   // [[VersionChecker sharedVersionChecker] startChecking:self];
    //公司业务 newsblast的弹出放在版本检查的回调中执行，不提示版本更新的情况下再弹newsblast
    //[self performSelector:@selector(beginSession) withObject:nil afterDelay:3.0];
    
    // 注册接收远程通
    [[UIApplication sharedApplication] registerForRemoteNotificationTypes:(UIRemoteNotificationTypeAlert | UIRemoteNotificationTypeBadge | UIRemoteNotificationTypeSound)];
    
    [[AnalyticsTool sharedTool] StartSessionWithSentFile:NO];
    
    return YES;
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


- (void)applicationWillResignActive:(UIApplication *)application {
    /*
     Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
     Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
     */
    cocos2d::CCDirector::sharedDirector()->pause();
}

- (void)applicationDidBecomeActive:(UIApplication *)application {
    /*
     Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
     */
    cocos2d::CCDirector::sharedDirector()->resume();
}

- (void)applicationDidEnterBackground:(UIApplication *)application {
    /*
     Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later.
     If your application supports background execution, called instead of applicationWillTerminate: when the user quits.
     */
    cocos2d::CCApplication::sharedApplication()->applicationDidEnterBackground();
    
    // clear icon numbers after recive push info and start game
    [[UIApplication sharedApplication] setApplicationIconBadgeNumber:0];
}

- (void)applicationWillEnterForeground:(UIApplication *)application {
    /*
     Called as part of  transition from the background to the inactive state: here you can undo many of the changes made on entering the background.
     */
    cocos2d::CCApplication::sharedApplication()->applicationWillEnterForeground();
    
    //if you need request NewsBlast when app enter to foreground everytime.
    [self beginSession];
}

- (void)applicationWillTerminate:(UIApplication *)application {
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
	return UIInterfaceOrientationMaskLandscape;
}

#endif


- (void)dealloc {
    [window release];
    [super dealloc];
}


@end
