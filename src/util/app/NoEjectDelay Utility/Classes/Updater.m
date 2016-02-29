#import "Sparkle/SUUpdater.h"
#import "Updater.h"

@interface Updater ()

@property SUUpdater* suupdater;

@end

@implementation Updater

- (id)init {
  self = [super init];

  if (self) {
    self.suupdater = [SUUpdater new];
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
  [self.suupdater setFeedURL:[NSURL URLWithString:url]];

  NSLog(@"checkForUpdates %@", url);
  if (isBackground) {
    [self.suupdater checkForUpdatesInBackground];
  } else {
    [self.suupdater checkForUpdates:nil];
  }
}

- (void)checkForUpdatesInBackground {
  [self check:YES];
}

- (void)checkForUpdatesStableOnly {
  NSString* url = [self getFeedURL:YES];
  [self.suupdater setFeedURL:[NSURL URLWithString:url]];
  NSLog(@"checkForUpdates %@", url);
  [self.suupdater checkForUpdates:nil];
}

- (void)checkForUpdatesWithBetaVersion {
  NSString* url = [self getFeedURL:NO];
  [self.suupdater setFeedURL:[NSURL URLWithString:url]];
  NSLog(@"checkForUpdates %@", url);
  [self.suupdater checkForUpdates:nil];
}

@end
