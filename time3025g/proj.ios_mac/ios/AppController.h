#import <UIKit/UIKit.h>

@class RootViewController;

@interface AppController : NSObject <UIApplicationDelegate> {
    UIWindow *window;

}
@property (nonatomic, retain) UIWindow *window;
@property (nonatomic, retain) RootViewController *viewController;


+(AppController*)sharedAppController;
@end

