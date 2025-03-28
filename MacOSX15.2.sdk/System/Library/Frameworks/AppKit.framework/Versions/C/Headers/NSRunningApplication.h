/*
	NSRunningApplication.h
	Application Kit
	Copyright (c) 1994-2024, Apple Inc.
	All rights reserved.
*/

#import <Foundation/NSObject.h>
#import <Foundation/NSArray.h>
#import <AppKit/NSWorkspace.h>
#import <AppKit/AppKitDefines.h>

NS_HEADER_AUDIT_BEGIN(nullability, sendability)
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

#pragma mark - NSApplicationActivationOptions

/// The following flags are for `-activateWithOptions:` and equivalent.
typedef NS_OPTIONS(NSUInteger, NSApplicationActivationOptions) {
    /// By default, activation brings only the main and key
    /// windows forward. If you specify `activateAllWindows`,
    /// all of the application's windows are brought forward.
    NSApplicationActivateAllWindows = 1 << 0,
    
    /// The application is activated regardless of the currently
    /// active app.
    NSApplicationActivateIgnoringOtherApps API_DEPRECATED("ignoringOtherApps is deprecated in macOS 14 and will have no effect.", macos(10.6, 14.0)) = 1 << 1
};

#pragma mark - NSApplicationActivationPolicy

/// The following activation policies control whether and how an application may be activated.
/// They are determined by the `Info.plist`.
typedef NS_ENUM(NSInteger, NSApplicationActivationPolicy) {
    /* The application is an ordinary app that appears in the Dock and may have a user interface.  This is the default for bundled apps, unless overridden in the Info.plist. */
    NSApplicationActivationPolicyRegular, 
    
    /* The application does not appear in the Dock and does not have a menu bar, but it may be activated programmatically or by clicking on one of its windows.  This corresponds to LSUIElement=1 in the Info.plist. */
    NSApplicationActivationPolicyAccessory,
    
    /* The application does not appear in the Dock and may not create windows or be activated.  This corresponds to LSBackgroundOnly=1 in the Info.plist.  This is also the default for unbundled executables that do not have Info.plists. */
    NSApplicationActivationPolicyProhibited
};

@class NSLock, NSDate, NSImage, NSURL;

#pragma mark - NSRunningApplication

/**
 `NSRunningApplication` is a class to manipulate and provide information for a single instance of an application.  Only user applications are tracked; this does not provide information about every process on the system.
 
 Some properties of an application are fixed, such as the bundle identifier.  Other properties may vary over time, such as whether the app is hidden.  Properties that vary can be observed with KVO, in which case the description comment for the method will mention it.
 
 Properties that vary over time are inherently race-prone.  For example, a hidden app may unhide itself at any time.  To ameliorate this, properties persist until the next turn of the main run loop in a common mode.  For example, if you repeatedly poll an unhidden app for its hidden property without allowing the run loop to run, it will continue to return `NO`, even if the app hides, until the next turn of the run loop.
 
 `NSRunningApplication` is thread safe, in that its properties are returned atomically.  However, it is still subject to the main run loop policy described above.  If you access an instance of `NSRunningApplication` from a background thread, be aware that its time-varying properties may change from under you as the main run loop runs (or not).
 
 An `NSRunningApplication` instance remains valid after the application exits.  However, most properties lose their significance, and some properties may not be available on a terminated application.
 
 To access the list of all running applications, use the `-runningApplications` method on `NSWorkspace`.
*/
API_AVAILABLE(macos(10.6)) NS_SWIFT_SENDABLE
@interface NSRunningApplication : NSObject

/// Indicates that the process is an exited application.
/// This is observable through KVO.
@property (readonly, getter=isTerminated) BOOL terminated;

/// Indicates that the process is finished launching, which corresponds to the @c NSApplicationDidFinishLaunching internal notification.
///  This is observable through KVO.
///  Some applications do not post this notification and so are never reported as finished launching.
@property (readonly, getter=isFinishedLaunching) BOOL finishedLaunching;

/// Indicates whether the application is currently hidden.
/// This is observable through KVO.
@property (readonly, getter=isHidden) BOOL hidden;

/// Indicates whether the application is currently frontmost.
/// This is observable through KVO.
@property (readonly, getter=isActive) BOOL active;

/// Indicates whether the application currently owns the menu bar.
/// This is observable through KVO.
@property (readonly) BOOL ownsMenuBar API_AVAILABLE(macos(10.7));

/// Indicates the activation policy of the application.
/// This is observable through KVO (the type is usually fixed, but may be changed through a call to `-[NSApplication setActivationPolicy:]`).
@property (readonly) NSApplicationActivationPolicy activationPolicy;

/// Indicates the name of the application.
/// This is dependent on the current localization of the referenced app, and is suitable for presentation to the user.
@property (nullable, readonly, copy) NSString *localizedName;

/// Indicates the `CFBundleIdentifier` of the application, or nil if the application does not have an `Info.plist`.
@property (nullable, readonly, copy) NSString *bundleIdentifier;

/// Indicates the URL to the application's bundle, or nil if the application does not have a bundle.
@property (nullable, readonly, copy) NSURL *bundleURL;

/// Indicates the URL to the application's executable.
@property (nullable, readonly, copy) NSURL *executableURL;

/// Indicates the process identifier (pid) of the application.
/// Do not rely on this for comparing processes.  Use `-isEqual:` instead.
/// @note Not all applications have a pid.  Applications without a pid return -1 from this method.
/// This is observable through KVO (an application's pid may change if it is automatically terminated).
@property (readonly) pid_t processIdentifier;

/// Indicates the date when the application was launched.
///  This property is not available for all applications.
///  Specifically, it is not available for applications that were launched without going through `LaunchServices`.
@property (nullable, readonly, copy) NSDate *launchDate;

/// @return The icon of the application.
@property (nullable, readonly, strong) NSImage *icon;

/// Indicates the executing processor architecture for the application, as an @c NSBundleExecutableArchitecture from `NSBundle.h`.
@property (readonly) NSInteger executableArchitecture;

/// Attempts to hide the receiver.
/// @return `YES` if the request to hide or unhide was successfully sent, `NO` if not (for example, if the application has quit, or is of a type that cannot be unhidden).
- (BOOL)hide;

/// Attempts to unhide the receiver.
/// @return `YES` if the request to hide or unhide was successfully sent, `NO` if not (for example, if the application has quit, or is of a type that cannot be unhidden).
- (BOOL)unhide;

/**
 Attempts to activate the application using the specified options.
 
 You shouldn’t assume the app will be active immediately
 after sending this message. The framework also does not
 guarantee that the app will be activated at all.
 
 Additionally allows specifying another application to take
 the active status from, which can be used for coordinated
 or cooperative activation. The other application should
 call `-yieldActivationToApplication:` or equivalent prior to this
 request being sent.
 
 @return `YES` if the request has been allowed by the system,
 otherwise `NO`.
 */
- (BOOL)activateFromApplication:(NSRunningApplication *)application options:(NSApplicationActivationOptions)options API_AVAILABLE(macos(14.0)) NS_SWIFT_NAME(activate(from:options:));

/// Attempts to activate the receiver.
/// @return `YES` if the request to activate was successfully sent, `NO` if not (for example, if the application has quit, or is of a type that cannot be activated).
- (BOOL)activateWithOptions:(NSApplicationActivationOptions)options;

/// Attempts to quit the receiver normally.
/// @return `YES` if the request was successfully sent, `NO` if not (for example, if the application is no longer running).
/// This method may return before the receiver exits; you should observe the terminated property or listen for the notification to detect when the app has exited.
- (BOOL)terminate;

/// Attempts to force the receiver to quit.
/// @return `YES` if the request was successfully sent, `NO` if not (for example, if the application is no longer running).
/// This method may return before the receiver exits; you should observe the terminated property or listen for the notification to detect when the app has exited.
- (BOOL)forceTerminate;

/// @return An array of currently running applications with the given bundle identifier, or an empty array if no apps match.
+ (NSArray<NSRunningApplication *> *)runningApplicationsWithBundleIdentifier:(NSString *)bundleIdentifier;

/// @return The running application with the given process identifier, or nil if no application has that pid.
/// Applications that do not have PIDs cannot be returned from this method.
+ (nullable instancetype)runningApplicationWithProcessIdentifier:(pid_t)pid;

/// @return An @c NSRunningApplication representing this application.
@property (class, readonly, strong) __kindof NSRunningApplication *currentApplication;

/**
 *  Cause any applications that are invisibly still running (see `NSProcessInfo.h` automatic termination methods and docs) to terminate as if triggered by system memory pressure.
 *  This is intended for installer apps and the like to make sure that nothing is unexpectedly relying on the files they're replacing.
 */
+ (void) terminateAutomaticallyTerminableApplications;

@end

@interface NSWorkspace (NSWorkspaceRunningApplications)

/// @return An array of `NSRunningApplication`s representing currently running applications.
/// The order of the array is unspecified, but it is stable, meaning that the relative order of particular applications will not change across multiple calls to `runningApplications`.
/// Similar to `NSRunningApplication`'s properties, this property will only change when the main run loop is run in a common mode.  Instead of polling, use key-value observing to be notified of changes to this array property.
///This property is thread safe, in that it may be called from background threads and the result is returned atomically.  This property is observable through KVO.
@property (readonly, copy) NSArray<NSRunningApplication *> *runningApplications API_AVAILABLE(macos(10.6));

@end

API_UNAVAILABLE_END
NS_HEADER_AUDIT_END(nullability, sendability)
