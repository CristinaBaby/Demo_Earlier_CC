#include "FacebookTool.h"


#import "AppController.h"

#import <Social/Social.h>


static void showMessage(NSString* title,NSString* msg)
{
    UIAlertView *alertView = [[UIAlertView alloc]
                              initWithTitle:title
                              message:msg
                              delegate:nil
                              cancelButtonTitle:@"OK"
                              otherButtonTitles:nil];
    [alertView show];
    [alertView release];

}

void FacebookTool::postToFacebook(const char* msg, const char* imagePath, const char* url)
{

    if([[AppController sharedAppController] isOpenWebsite]==FALSE)
    {
        showMessage(@"Sorry!", @"Facebook connection is currently not available, please try again later.");
        return;
    }
    
    if ([SLComposeViewController isAvailableForServiceType:SLServiceTypeFacebook])
    {
        SLComposeViewController *sheet = [SLComposeViewController composeViewControllerForServiceType:SLServiceTypeFacebook];
        [sheet setInitialText:[NSString stringWithUTF8String:msg]];
        if (imagePath)
        {
            [sheet addImage:[UIImage imageWithContentsOfFile:[NSString stringWithUTF8String:imagePath]]];
        }
        
        if (url)
        {
            [sheet addURL:[NSURL URLWithString:[NSString stringWithUTF8String:url]]];
        }
        
        UIViewController *myViewController = [[UIViewController alloc] init];
        UIWindow *window = [[UIApplication sharedApplication] keyWindow];
        [window addSubview:myViewController.view];
        
        sheet.completionHandler = ^(SLComposeViewControllerResult result)
        {
            [myViewController dismissViewControllerAnimated:true completion:^{}];
            [myViewController.view removeFromSuperview];
            [myViewController release];
            if ( result == SLComposeViewControllerResultDone ) {
                showMessage(@"", @"Post succeed!");
            }
        };
        
        
		[myViewController presentViewController:sheet animated:YES completion:nil];
    }
    else
    {
        showMessage(@"Sorry",@"You can't send a facebook right now, make sure your device has an internet connection and you have at least one facebook account setup");
    }
}

