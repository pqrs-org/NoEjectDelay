import Cocoa

@NSApplicationMain
class AppDelegate: NSObject, NSApplicationDelegate {

    @IBOutlet weak var window: NSWindow!
    @IBOutlet weak var version: NSTextField!

    func applicationDidFinishLaunching(aNotification: NSNotification) {
        version.stringValue = NSBundle.mainBundle().infoDictionary?["CFBundleVersion"] as! String;
    }

    func applicationWillTerminate(aNotification: NSNotification) {
    }
}
