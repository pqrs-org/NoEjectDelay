#import "AppDelegate.h"
#import "Updater.h"

@interface AppDelegate ()
@property(weak) IBOutlet NSWindow* window;
@property(weak) IBOutlet NSTextField* label_version_;
@property(weak) IBOutlet Updater* updater_;

- (IBAction)launchUninstaller:(id)sender;
@end

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification*)aNotification {
  NSString* version = [[NSBundle mainBundle] infoDictionary][@"CFBundleVersion"];
  [self.label_version_ setStringValue:version];

  [self.updater_ checkForUpdatesInBackground:self];
}

- (void)applicationWillTerminate:(NSNotification*)aNotification {
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication*)theApplication {
  return YES;
}

- (IBAction)launchUninstaller:(id)sender {
  NSString* path = @"/Library/Application Support/org.pqrs/NoEjectDelay/uninstaller.applescript";
  [[[NSAppleScript alloc] initWithContentsOfURL:[NSURL fileURLWithPath:path] error:nil] executeAndReturnError:nil];
}

@end
