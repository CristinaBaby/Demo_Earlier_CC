#import <UIKit/UIKit.h>
#import "AppRootController.h"

@class RootViewController;

@interface AppController : AppRootController <UIApplicationDelegate> {
    UIWindow *window;
    NSURLConnection* _myConnection;
    BOOL _isOpenWebsite;
}
@property (nonatomic, retain) UIWindow *window;
@property (nonatomic, retain) RootViewController *viewController;
@property(nonatomic,assign)BOOL isOpenWebsite;
@property(nonatomic,retain)  NSURLConnection* myConnection;
+(AppController*)sharedAppController;
@end

