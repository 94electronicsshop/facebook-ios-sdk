// Copyright (c) 2014-present, Facebook, Inc. All rights reserved.
//
// You are hereby granted a non-exclusive, worldwide, royalty-free license to use,
// copy, modify, and distribute this software in source code or binary form for use
// in connection with the web services and APIs provided by Facebook.
//
// As with any software that integrates with the Facebook platform, your use of
// this software is subject to the Facebook Developer Principles and Policies
// [http://developers.facebook.com/policy/]. This copyright notice shall be
// included in all copies or substantial portions of the software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
// IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#import <FBSDKCoreKit/FBSDKCoreKit.h>

#import "FBSDKAppEventsConfigurationFixtures.h"
#import "FBSDKBridgeAPI+Testing.h"
#import "FBSDKCloseIcon.h"
#import "FBSDKEventDeactivationManager.h"
#import "FBSDKEventBinding.h"
#import "FBSDKEventBindingManager.h"
#import "FBSDKMath.h"
#import "FBSDKSKAdNetworkEvent.h"
#import "FBSDKSKAdNetworkRule.h"
#import "FBSDKServerConfigurationFixtures.h"
#import "FBSDKSettingsProtocol.h"
#import "FBSDKTestCase.h"
#import "FBSDKTestCoder.h"
#import "FBSDKViewHierarchy.h"
#import "UserDefaultsSpy.h"
#import "FakeBundle.h"
// URLSession Abstraction
#import "FBSDKURLSessionProxyProviding.h"
#import "FBSDKURLSessionProxyFactory.h"
// GraphRequestConnection Abstraction
#import "FBSDKGraphRequestConnecting.h"
#import "FBSDKGraphRequestConnectionFactory.h"
#import "FBSDKGraphRequestConnectionProviding.h"
// ErrorConfiguration Abstractions
#import "FBSDKErrorConfigurationProviding.h"
#import "FBSDKErrorConfigurationProvider.h"
// GraphRequestPiggybackManager Abstractions
#import "FBSDKGraphRequestPiggybackManaging.h"
#import "FBSDKGraphRequestPiggybackManagerProviding.h"
#import "FBSDKGraphRequestPiggybackManagerProvider.h"
// AppEvents Abstractions
#import "FBSDKEventLogger.h"
// GraphRequest Abstraction
#import "FBSDKGraphRequestProviding.h"
#import "FBSDKGraphRequestFactory.h"
// Data Persistance
#import "FBSDKDataPersisting.h"
#import "NSUserDefaults+FBSDKDataPersisting.h"
// Swizzling
#import "FBSDKSwizzling.h"
// AppLinkUtility method
#import "FBSDKAppLinkUtility+Internal.h"
// AppEventsConfiguration
#import "FBSDKAppEventsConfigurationProtocol.h"
#import "FBSDKAppEventsConfigurationProviding.h"
#import "FBSDKAppEventsConfiguration+AppEventsConfigurationProtocol.h"

NS_ASSUME_NONNULL_BEGIN

// Interfaces for Swift extensions on Objective-C Test classes
@interface FBSDKAppEventsUtilityTests : FBSDKTestCase
@end

// Categories needed to expose private methods to Swift
@interface FBSDKAppEventsConfigurationManager (Testing)

+ (void)_processResponse:(id)response error:(nullable NSError *)error;

@end

@interface FBSDKCloseIcon (Testing)

- (nullable UIImage *)imageWithSize:(CGSize)size
                       primaryColor:(UIColor *)primaryColor
                     secondaryColor:(UIColor *)secondaryColor
                              scale:(CGFloat)scale;

@end

NS_SWIFT_NAME(FBProfilePictureViewState)
@interface FBSDKProfilePictureViewState
@end

@interface FBSDKProfilePictureView (Testing)

- (void)_accessTokenDidChangeNotification:(NSNotification *)notification;
- (void)_profileDidChangeNotification:(NSNotification *)notification;
- (void)_updateImageWithProfile;
- (void)_updateImageWithAccessToken;
- (void)_updateImage;
- (void)_fetchAndSetImageWithURL:(NSURL *)imageURL state:(FBSDKProfilePictureViewState *)state;
- (nullable FBSDKProfilePictureViewState *)lastState;

@end

@interface FBSDKAccessToken (Testing)

+ (void)setCurrentAccessToken:(nullable FBSDKAccessToken *)token
          shouldDispatchNotif:(BOOL)shouldDispatchNotif;

@end

@interface FBSDKProfile (Testing)
@property (class, nonatomic, nullable) id<FBSDKDataPersisting> store;

+ (void)setCurrentProfile:(nullable FBSDKProfile *)profile
   shouldPostNotification:(BOOL)shouldPostNotification;

+ (void)configureWithStore:(id<FBSDKDataPersisting>)store
NS_SWIFT_NAME(configure(store:));

+ (void)reset;

@end

@interface FBSDKAuthenticationToken (Testing)

- (instancetype)initWithTokenString:(NSString *)tokenString
                              nonce:(NSString *)nonce;

+ (void)setCurrentAuthenticationToken:(nullable FBSDKAuthenticationToken *)token
               shouldPostNotification:(BOOL)shouldPostNotification;

@end

@interface FBSDKGateKeeperManager (Testing)

@property (class, nonatomic, readonly) BOOL canLoadGateKeepers;
@property (class, nonatomic, nullable) FBSDKLogger *logger;
@property (class, nonatomic, nullable) Class<FBSDKSettings> settings;
@property (class, nonatomic, nullable) id<FBSDKGraphRequestProviding> requestProvider;
@property (class, nonatomic, nullable) id<FBSDKGraphRequestConnectionProviding> connectionProvider;
@property (class, nonatomic, nullable) id<FBSDKDataPersisting> store;

@property (class, nonatomic, nullable) NSDictionary *gateKeepers;
@property (class, nonatomic) BOOL requeryFinishedForAppStart;
@property (class, nonatomic, nullable) NSDate *timestamp;
@property (class, nonatomic) BOOL isLoadingGateKeepers;

+ (void)configureWithSettings:(Class<FBSDKSettings>)settings
              requestProvider:(id<FBSDKGraphRequestProviding>)requestProvider
           connectionProvider:(nonnull id<FBSDKGraphRequestConnectionProviding>)connectionProvider
                        store:(id<FBSDKDataPersisting>)store
NS_SWIFT_NAME(configure(settings:requestProvider:connectionProvider:store:));
+ (id<FBSDKGraphRequest>)requestToLoadGateKeepers;
+ (void)processLoadRequestResponse:(nullable id)result error:(nullable NSError *)error
NS_SWIFT_NAME(parse(result:error:));
+ (BOOL)_gateKeeperIsValid;
+ (void)reset;
+ (id<FBSDKGraphRequestProviding>)requestProvider;

@end

@interface FBSDKAppLinkUtility (Testing)
+ (id<FBSDKGraphRequestProviding>)requestProvider;

@end

@interface FBSDKEventBindingManager (Testing)

@property (nonatomic) BOOL isStarted;
@property (nonatomic, nullable) NSMutableDictionary *reactBindings;
@property (nonatomic, readonly) NSSet *validClasses;
@property (nonatomic) BOOL hasReactNative;
@property (nonatomic, nullable) NSArray<FBSDKEventBinding *> *eventBindings;
@property (nonatomic, nullable, readonly) Class<FBSDKSwizzling> swizzler;

- (instancetype)initWithSwizzler:(Class<FBSDKSwizzling>)swizzling;
- (instancetype)initWithSwizzler:(Class<FBSDKSwizzling>)swizzling
                            json:(NSDictionary *)dict;
- (void)start;
- (void)handleReactNativeTouchesWithHandler:(nullable id)handler
                                    command:(nullable SEL)command
                                    touches:(id)touches
                                  eventName:(id)eventName;
- (void)handleDidSelectRowWithBindings:(NSArray<FBSDKEventBinding *> *)bindings
                                target:(nullable id)target
                               command:(nullable SEL)command
                             tableView:(UITableView *)tableView
                             indexPath:(NSIndexPath *)indexPath;
- (void)handleDidSelectItemWithBindings:(NSArray<FBSDKEventBinding *> *)bindings
                                 target:(nullable id)target
                                command:(nullable SEL)command
                         collectionView:(UICollectionView *)collectionView
                              indexPath:(NSIndexPath *)indexPath;
- (void)matchView:(UIView *)view
         delegate:(id)delegate;

@end

@interface FBSDKSettings (Testing)

@property (class, nonatomic, nullable, readonly) id<FBSDKDataPersisting> store;
@property (class, nonatomic, nullable, readonly) id<FBSDKAppEventsConfigurationProviding> appEventsConfigurationProvider;

+ (void)configureWithStore:(id<FBSDKDataPersisting>)store
appEventsConfigurationProvider:(Class<FBSDKAppEventsConfigurationProviding>)provider
NS_SWIFT_NAME(configure(store:appEventsConfigurationProvider:));

+ (void)reset;

@end

NS_ASSUME_NONNULL_END
