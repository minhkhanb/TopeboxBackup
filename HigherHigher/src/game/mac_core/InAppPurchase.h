#if !_IN_APP_PURCHASE_H
#define _IN_APP_PURCHASE_H 1

#import <Foundation/NSObject.h>
#import <UIKit/UIKit.h>
#import <Foundation/NSURLConnection.h>
#import "CGame.h"
#import <StoreKit/StoreKit.h>

#define kInAppPurchaseManagerProductsFetchedNotification @"kInAppPurchaseManagerProductsFetchedNotification"
#define kInAppPurchaseManagerTransactionFailedNotification @"kInAppPurchaseManagerTransactionFailedNotification"
#define kInAppPurchaseManagerTransactionSucceededNotification @"kInAppPurchaseManagerTransactionSucceededNotification"
#define kInAppPurchaseProUpgradeProductId @"topebox.default"
#define TIME_OUT_REQUEST 5000


@interface InAppPurchase : NSObject <SKProductsRequestDelegate, SKPaymentTransactionObserver>
{
    SKProduct *proUpgradeProduct;
    SKProductsRequest *productsRequest;
    SKProductsRequest *productInformations;
    
    SK_EXTERN_CLASS_AVAILABLE(11_0) SKProductStorePromotionController* _promotion;
    int _time_request;
}
// public methods
- (void)loadStore;
- (BOOL)canMakePurchases;
- (void)purchaseProUpgrade;
- (void)Restore;
- (bool)CanPayment;
- (void) PurchaseProduct:(NSString *)productID;
- (void) RefeshStore;

-(void) SetTimeOut:(int) time;
-(void) UpdateTimeOut:(int) dt;
-(int) GetTimeOut;

@end

#endif
