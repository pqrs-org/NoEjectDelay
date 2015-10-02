#import "Sparkle/SUUpdater.h"
#import "Updater.h"

@interface Updater () {
  SUUpdater* suupdater_;
}
@end

@implementation Updater

- (id)init {
  self = [super init];

  if (self) {
    suupdater_ = [SUUpdater new];
  }

  return self;
}

- (NSString*)getFeedURL:(BOOL)isStableOnly {
  if (isStableOnly) {
    return @"https://pqrs.org/macosx/karabiner/files/noejectdelay-appcast.xml";
  } else {
    return @"https://pqrs.org/macosx/karabiner/files/noejectdelay-appcast-devel.xml";
  }
}

- (void)check:(BOOL)isBackground {
  NSString* url = [self getFeedURL:YES];
  [suupdater_ setFeedURL:[NSURL URLWithString:url]];

  NSLog(@"checkForUpdates %@", url);
  if (isBackground) {
    [suupdater_ checkForUpdatesInBackground];
  } else {
    [suupdater_ checkForUpdates:nil];
  }
}

- (IBAction)checkForUpdates:(id)sender {
  [self check:NO];
}

- (IBAction)checkForUpdatesInBackground:(id)sender {
  [self check:YES];
}

- (IBAction)checkForUpdatesStableOnly:(id)sender {
  NSString* url = [self getFeedURL:YES];
  [suupdater_ setFeedURL:[NSURL URLWithString:url]];
  NSLog(@"checkForUpdates %@", url);
  [suupdater_ checkForUpdates:nil];
}

- (IBAction)checkForUpdatesWithBetaVersion:(id)sender {
  NSString* url = [self getFeedURL:NO];
  [suupdater_ setFeedURL:[NSURL URLWithString:url]];
  NSLog(@"checkForUpdates %@", url);
  [suupdater_ checkForUpdates:nil];
}

@end
