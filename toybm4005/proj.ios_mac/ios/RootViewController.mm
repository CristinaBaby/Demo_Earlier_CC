/****************************************************************************
 Copyright (c) 2013      cocos2d-x.org
 Copyright (c) 2013-2014 Chukong Technologies Inc.

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

#import "RootViewController.h"
#import "cocos2d.h"
#import "CCEAGLView.h"
//#import <QuartzCore/QuartzCore.h>

@interface RootViewController ()
@property(nonatomic, strong)     UIDocumentInteractionController* docController;
@end

@implementation RootViewController

#define IS_IPAD (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad)

- (BOOL)checkInstagramInstalled
{
    return [[UIApplication sharedApplication] canOpenURL:[NSURL URLWithString:@"instagram://app"]];
}

-(void)sendPictureToInstagram:(NSString*)pimagePath
{
  //UIImage* image = [UIImage imageNamed:imagePath];
    
    if ( ![self checkInstagramInstalled] ) {
        
        UIAlertView *alter = [[UIAlertView alloc] initWithTitle:@"" message:@"No Instagram detected. Go to download now?" delegate:self cancelButtonTitle:@"No, thanks" otherButtonTitles:@"Yes, Get it",nil];
        [alter show];
        [alter release];
        
        return;
    }
    
    //imagePath =  [[[NSBundle mainBundle] bundlePath] stringByAppendingPathComponent:@"default.png"];
    
   // UIImage* instaImage = [UIImage imageNamed:@"Default.png"];
    
    
    UIImage* instaImage =[UIImage imageWithContentsOfFile:pimagePath];
    
    NSString* imagePath = [NSString stringWithFormat:@"%@/image.igo", [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) lastObject]];
    [[NSFileManager defaultManager] removeItemAtPath:imagePath error:nil];
    [UIImagePNGRepresentation(instaImage) writeToFile:imagePath atomically:YES];

    
    
//    NSLog(@"path=%@",imagePath);
//    
//    NSURL* lUrl =  [NSURL fileURLWithPath:imagePath];
//
//    NSLog(@"lUrl=%@",lUrl);;
    
    
    self.docController = [UIDocumentInteractionController interactionControllerWithURL:[NSURL fileURLWithPath:imagePath]];
    self.docController.delegate=self;
    self.docController.UTI = @"com.instagram.exclusivegram";
    
    //注意必须设置具体的屏幕宽高度
    //[self.docController presentOpenInMenuFromRect:CGRectMake(0, 0, 320, 568) inView:self.view animated:YES];
//    if(IS_IPAD)
//    {
//        [self.docController presentOpenInMenuFromRect:CGRectMake(0, 0, 768, 1024) inView:self.view animated:YES];
//    }
//    else
//    {
//        [self.docController presentOpenInMenuFromRect:self.view.frame inView:self.view animated:YES];
//    }
    
    
     [self.docController presentOpenInMenuFromRect:CGRectZero inView:self.view animated:YES];

    //  [_docController presentOptionsMenuFromRect:CGRectZero inView:self.view animated:YES];
}

- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex
{
    if(buttonIndex==1)
    {
        [[UIApplication sharedApplication] openURL:[NSURL URLWithString:@"http://itunes.apple.com/app/id389801252"]];
    }
}


/*
 // The designated initializer.  Override if you create the controller programmatically and want to perform customization that is not appropriate for viewDidLoad.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if ((self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil])) {
        // Custom initialization
    }
    return self;
}
*/

/*
// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (void)loadView {
}
*/


// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
 // Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
 - (void)viewDidLoad {
     [super viewDidLoad];
     if ([self respondsToSelector:@selector(setNeedsStatusBarAppearanceUpdate)]) {
         // iOS 7
         [self prefersStatusBarHidden];
         [self performSelector:@selector(setNeedsStatusBarAppearanceUpdate)];
     }
     

     
 }
 
-(BOOL)prefersStatusBarHidden
{
   return YES;//隐藏为YES，显示为NO
}

// Override to allow orientations other than the default portrait orientation.
// This method is deprecated on ios6
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    return UIInterfaceOrientationIsPortrait( interfaceOrientation );
}

// For ios6, use supportedInterfaceOrientations & shouldAutorotate instead
- (NSUInteger) supportedInterfaceOrientations{
#ifdef __IPHONE_6_0
    return UIInterfaceOrientationMaskAllButUpsideDown;
#endif
}

- (BOOL) shouldAutorotate {
    return TRUE;
}

- (void)didRotateFromInterfaceOrientation:(UIInterfaceOrientation)fromInterfaceOrientation {
    [super didRotateFromInterfaceOrientation:fromInterfaceOrientation];

    cocos2d::GLView *glview = cocos2d::Director::getInstance()->getOpenGLView();

    if (glview)
    {
        CCEAGLView *eaglview = (CCEAGLView*) glview->getEAGLView();

        if (eaglview)
        {
            CGSize s = CGSizeMake([eaglview getWidth], [eaglview getHeight]);
            cocos2d::Application::getInstance()->applicationScreenSizeChanged((int) s.width, (int) s.height);
        }
    }
}


- (void)didReceiveMemoryWarning {
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];

    // Release any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload {
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}


- (void)dealloc {
    [super dealloc];
}


@end
