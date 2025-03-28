//
//  ASCredentialIdentityStore.h
//  AuthenticationServices Framework
//
//  Copyright © 2018 Apple Inc. All rights reserved.
//

#import <AuthenticationServices/ASFoundation.h>
#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@protocol ASCredentialIdentity;

@class ASCredentialIdentityStoreState;
@class ASPasswordCredentialIdentity;

@class ASCredentialServiceIdentifier;

AS_EXTERN API_AVAILABLE(ios(12.0), macCatalyst(14.0), macos(11.0)) API_UNAVAILABLE(tvos, watchos)
NSErrorDomain const ASCredentialIdentityStoreErrorDomain;

/*! @enum ASCredentialIdentityStoreErrorCode
 @constant ASCredentialIdentityStoreErrorCodeInternalError The operation failed due to an internal error.
 @constant ASCredentialIdentityStoreErrorCodeStoreDisabled The operation failed because the credential identity store is disabled.
 @constant ASCredentialIdentityStoreErrorCodeStoreBusy The operation failed because the credential identity store is busy. Attempt the operation again at a later time.
 */
typedef NS_ERROR_ENUM(ASCredentialIdentityStoreErrorDomain, ASCredentialIdentityStoreErrorCode) {
    ASCredentialIdentityStoreErrorCodeInternalError = 0,
    ASCredentialIdentityStoreErrorCodeStoreDisabled = 1,
    ASCredentialIdentityStoreErrorCodeStoreBusy = 2,
} API_AVAILABLE(ios(12.0), macCatalyst(14.0), macos(11.0)) API_UNAVAILABLE(tvos, watchos);

typedef NS_OPTIONS(NSUInteger, ASCredentialIdentityTypes) {
    ASCredentialIdentityTypesAll = 0,
    ASCredentialIdentityTypesPassword = 1,
    ASCredentialIdentityTypesPasskey = 1 << 1,
    ASCredentialIdentityTypesOneTimeCode = 1 << 2,
} NS_SWIFT_NAME(ASCredentialIdentityStore.IdentityTypes) API_AVAILABLE(ios(17.4), macos(14.4), visionos(1.1)) API_UNAVAILABLE(tvos, watchos);

AS_EXTERN API_AVAILABLE(ios(12.0), macCatalyst(14.0), macos(11.0)) API_UNAVAILABLE(tvos, watchos)
@interface ASCredentialIdentityStore : NSObject

@property (class, nonatomic, readonly) ASCredentialIdentityStore *sharedStore;

- (instancetype)init NS_UNAVAILABLE;

/*! @abstract Get the state of the credential identity store.
 @param completion completion handler to be called with the current state of the store.
 @discussion Call this method to find out the current state of the store before attempting to call other store methods.
 Use the provided ASCredentialIdentityStoreState to find out if the store is enabled and whether it supports incremental
 updates.
 */
- (void)getCredentialIdentityStoreStateWithCompletion:(void (^)(ASCredentialIdentityStoreState *state))completion
    NS_SWIFT_ASYNC_NAME(state())
    ;

/*! @abstract List the currently saved credential identities.
 @param serviceIdentifier Specify a service identifier to get only credential identities for that service.
 Pass nil to get credential identities for all services.
 @param credentialIdentityTypes Specify one or more types to get only credential identities of those types.
 Pass ASCredentialIdentityTypesAll to get credential identities of all types.
 @discussion Call this method to get a list of all credential identities saved in the store for your extension.
 */
- (void)getCredentialIdentitiesForService:(nullable ASCredentialServiceIdentifier *)serviceIdentifier credentialIdentityTypes:(ASCredentialIdentityTypes)credentialIdentityTypes completionHandler:(void (^)(NSArray<id <ASCredentialIdentity>> *))completionHandler API_AVAILABLE(ios(17.4), macos(14.4), visionos(1.1)) API_UNAVAILABLE(tvos, watchos) NS_REFINED_FOR_SWIFT;

/*! @abstract Save the given credential identities to the store.
 @param credentialIdentities array of ASPasswordCredentialIdentity objects to save to the store.
 @param completion optional completion handler to be called after adding the credential identities.
 If the operation fails, an error with domain ASCredentialIdentityStoreErrorDomain will be provided
 and none of the objects in credentialIdentities will be saved to the store.
 @discussion If the store supports incremental updates, call this method to add new credential
 identities since the last time the store was updated. Otherwise, call this method to pass all credential
 identities.
 If some credential identities in credentialIdentities already exist in the store, they will be replaced by
 those from credentialIdentities.
 */
- (void)saveCredentialIdentities:(NSArray<ASPasswordCredentialIdentity *> *)credentialIdentities completion:(void (^ _Nullable)(BOOL success, NSError * _Nullable error))completion
    NS_SWIFT_ASYNC_THROWS_ON_FALSE(1)
    API_DEPRECATED_WITH_REPLACEMENT("saveCredentialIdentityEntries:completion:", ios(12.0, 17.0), macos(11.0, 14.0))
    ;

/*! @abstract Save the given credential identities to the store.
 @param credentialIdentities array of ASCredentialIdentity objects to save to the store.
 @param completion optional completion handler to be called after adding the credential identities.
 If the operation fails, an error with domain ASCredentialIdentityStoreErrorDomain will be provided
 and none of the objects in credentialIdentities will be saved to the store.
 @discussion If the store supports incremental updates, call this method to add new credential
 identities since the last time the store was updated. Otherwise, call this method to pass all credential
 identities.
 If some credential identities in credentialIdentities already exist in the store, they will be replaced by
 those from credentialIdentities.
 */
-(void)saveCredentialIdentityEntries:(NSArray<id <ASCredentialIdentity>> *)credentialIdentities completion:(void (^ _Nullable)(BOOL success, NSError * _Nullable error))completion
    NS_SWIFT_ASYNC_THROWS_ON_FALSE(1)
    NS_SWIFT_NAME(saveCredentialIdentities(_:completion:)) API_AVAILABLE(ios(17.0), macos(14.0));

/*! @abstract Remove the given credential identities from the store.
 @param credentialIdentities array of ASPasswordCredentialIdentity objects to remove from the store.
 @param completion optional completion handler to be called after removing the credential identities.
 If the operation fails, an error with domain ASCredentialIdentityStoreErrorDomain will be provided
 and none of the objects in credentialIdentities will be removed from the store.
 @discussion Use this method only if the store supports incremental updates to remove previously added
 credentials to the store.
 */
- (void)removeCredentialIdentities:(NSArray<ASPasswordCredentialIdentity *> *)credentialIdentities completion:(void (^ _Nullable)(BOOL success, NSError * _Nullable error))completion
    NS_SWIFT_ASYNC_THROWS_ON_FALSE(1)
    API_DEPRECATED_WITH_REPLACEMENT("removeCredentialIdentityEntries:completion:", ios(12.0, 17.0), macos(11.0, 14.0))
    ;

/*! @abstract Remove the given credential identities from the store.
 @param credentialIdentities array of ASCredentialIdentity objects to remove from the store.
 @param completion optional completion handler to be called after removing the credential identities.
 If the operation fails, an error with domain ASCredentialIdentityStoreErrorDomain will be provided
 and none of the objects in credentialIdentities will be removed from the store.
 @discussion Use this method only if the store supports incremental updates to remove previously added
 credentials to the store.
 */
- (void)removeCredentialIdentityEntries:(NSArray<id <ASCredentialIdentity>> *)credentialIdentities completion:(void (^ _Nullable)(BOOL success, NSError * _Nullable error))completion
    NS_SWIFT_ASYNC_THROWS_ON_FALSE(1)
    NS_SWIFT_NAME(removeCredentialIdentities(_:completion:)) API_AVAILABLE(ios(17.0), macos(14.0));

/*! @abstract Remove all existing credential identities from the store.
 @param completion optional completion handler to be called after removing all existing credential identities.
 If the operation fails, an error with domain ASCredentialIdentityStoreErrorDomain will be provided and none of
 the existing credential identities will be removed from the store.
 */
- (void)removeAllCredentialIdentitiesWithCompletion:(void (^ _Nullable)(BOOL success, NSError * _Nullable error))completion
    NS_SWIFT_ASYNC_THROWS_ON_FALSE(1)
    ;

/*! @abstract Replace existing credential identities with new credential identities.
 @param newCredentialIdentities array of new credential identity objects to replace the old ones.
 @param completion an optional completion block to be called after the operation is finished.
 @discussion This method will delete all existing credential identities that are persisted in the
 store and replace them with the provided array of credential identities. If the operation fails, an
 error with domain ASCredentialIdentityStoreErrorDomain will be provided and none of the new credential
 identities will be saved.
 */
- (void)replaceCredentialIdentitiesWithIdentities:(NSArray<ASPasswordCredentialIdentity *> *)newCredentialIdentities completion:(void (^ _Nullable)(BOOL success, NSError * _Nullable error))completion
    NS_SWIFT_ASYNC_THROWS_ON_FALSE(1)
    API_DEPRECATED_WITH_REPLACEMENT("replaceCredentialIdentityEntries:completion:", ios(12.0, 17.0), macos(11.0, 14.0))
    ;

/*! @abstract Replace existing credential identities with new credential identities.
 @param newCredentialIdentities array of new credential identity objects to replace the old ones.
 @param completion an optional completion block to be called after the operation is finished.
 @discussion This method will delete all existing credential identities that are persisted in the
 store and replace them with the provided array of credential identities. If the operation fails, an
 error with domain ASCredentialIdentityStoreErrorDomain will be provided and none of the new credential
 identities will be saved.
 */
- (void)replaceCredentialIdentityEntries:(NSArray<id <ASCredentialIdentity>> *)newCredentialIdentities completion:(void (^ _Nullable)(BOOL success, NSError * _Nullable error))completion
    NS_SWIFT_ASYNC_THROWS_ON_FALSE(1)
    NS_SWIFT_NAME(replaceCredentialIdentities(_:completion:)) API_AVAILABLE(ios(17.0), macos(14.0));

@end

NS_ASSUME_NONNULL_END
