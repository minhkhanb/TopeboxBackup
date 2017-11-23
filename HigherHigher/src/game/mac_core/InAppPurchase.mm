// for MD5 hash

#import "URLConnection.h"
#import "PineGameDelegate.h"
#import "Defines.h"
#import "CGame.h"
#import "InAppPurchase.h"
extern CGame* g_pGame;

@implementation InAppPurchase
-(void) RefeshStore
{
    [[SKPaymentQueue defaultQueue] addTransactionObserver:self];
    [[SKPaymentQueue defaultQueue] restoreCompletedTransactions]; 
}
- (void) purchase:(SKProduct *)product{
    SKPayment *payment = [SKPayment paymentWithProduct:product];
    [[SKPaymentQueue defaultQueue] addTransactionObserver:self];
    [[SKPaymentQueue defaultQueue] addPayment:payment];
}


- (void) paymentQueueRestoreCompletedTransactionsFinished:(SKPaymentQueue *)queue
{
    //queue.transactions.count
    PURCHASE_FUNC(0, enumIAPResponseStatus::IAP_RESPONSE_STATUS_END_RESTORE);
}

- (void)paymentQueue:(SKPaymentQueue *)queue restoreCompletedTransactionsFailedWithError:(NSError *)error
{
    PURCHASE_FUNC(0, enumIAPResponseStatus::IAP_RESPONSE_STATUS_FAIL_RESTORE);
}

// InAppPurchaseManager.m
-(bool) CanPayment
{
    if ([SKPaymentQueue canMakePayments])
    {
        return true;
    }
    return false;
}
- (void)requestAllIAPInformation
{
#if USE_PINESTORE
    int num = CGame::_this->_pineStore.GetNumIAP();
    if(num > 0)
    {
        NSMutableArray * tmpary = [[NSMutableArray alloc] initWithCapacity: num];
        for (int i = 0; i < num; i++)
        {
            [tmpary addObject: [NSString stringWithCString: CGame::_this->_pineStore.GetProductIdAt(i) encoding:NSASCIIStringEncoding]];
        }
        
        //NSSet *productList = [NSSet a
        productInformations= [[SKProductsRequest alloc] initWithProductIdentifiers: [NSSet setWithArray:tmpary]];
        productInformations.delegate = self;
        [productInformations start];
    }
#endif
    
}
- (void)requestProUpgradeProductData
{
    
    productsRequest= [[SKProductsRequest alloc] initWithProductIdentifiers: [NSSet setWithObject: @"NULL"]];
    productsRequest.delegate = self;
    [productsRequest start];
    
       
    // we will release the request object in the delegate callback
}
-(bool)GetContent:(NSString *)productId
{
    [[NSUserDefaults standardUserDefaults] boolForKey:productId];
    return true;
}
-(void) PurchaseProduct:(NSString *)productID
{
    productsRequest= [[SKProductsRequest alloc] initWithProductIdentifiers: [NSSet setWithObject: productID]];
    productsRequest.delegate = self;
    [productsRequest start];
    //[self RefeshStore];
//dispatch_sync(dispatch_get_main_queue(), ^{
    //SKPayment *payment = [SKPayment paymentWithProductIdentifier:productID];
    //[[SKPaymentQueue defaultQueue] addPayment:payment];
    //});
}
#pragma mark -
#pragma mark SKProductsRequestDelegate methods

- (void)productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response
{
   
#if USE_PINESTORE
    if(productInformations == request)
    {
        //NSLog(@"Product reponse %@", response);
        if ([SKPaymentQueue canMakePayments] && [response.products count]>0) {
            
        }
        int count= 0;
         for (SKProduct* good in response.products)
         {
             count++;
             NSLog(@"Product %d", count);
             NSLog(@"Product title: %@" , good.localizedTitle);
             NSLog(@"Product description: %@" , good.localizedDescription);
             NSLog(@"Product price: %@" , good.price);
             
             NSNumberFormatter *formatter = [[NSNumberFormatter alloc] init];
             [formatter setFormatterBehavior:NSNumberFormatterBehavior10_4];
             [formatter setNumberStyle:NSNumberFormatterCurrencyStyle];
             [formatter setLocale: good.priceLocale];
             NSString *str_local = [NSString stringWithFormat:@"%@ %@",good.price,[formatter currencyCode]];
             //[formatter stringFromNumber:good.price];
             [formatter release];
              NSLog(@"Product local price: %@" , str_local);
             NSLog(@"Product id: %@" , good.productIdentifier);
             
            
             
             const char *proID=[good.productIdentifier UTF8String];
             const char *ProPrice=[[NSString stringWithFormat:@"%@",str_local] UTF8String];
             CGame::_this->_pineStore.SynPriceForProduct((char*)proID, (char*)ProPrice);
             
         }
        [productInformations release];
        productInformations = nil;
        return;
    }
#endif
    
    NSArray *products = response.products;
    proUpgradeProduct = [products count] == 1 ? [[products firstObject] retain] : nil;
    if (proUpgradeProduct)
    {
        NSLog(@"Product title: %@" , proUpgradeProduct.localizedTitle);
        NSLog(@"Product description: %@" , proUpgradeProduct.localizedDescription);
        NSLog(@"Product price: %@" , proUpgradeProduct.price);
        NSLog(@"Product id: %@" , proUpgradeProduct.productIdentifier);
        NSLog(@"request = %@",request);
        if(productsRequest == request)
        {
            [self purchase:proUpgradeProduct];
            
        }
    }
    
    
    //NSLog(@"log %@",response);
    for (NSString *invalidProductId in response.invalidProductIdentifiers)
    {
        NSLog(@"Invalid product id: %@" , invalidProductId);
    }
    
    // finally release the reqest we alloc/init’ed in requestProUpgradeProductData
    [productsRequest release];
    
    [[NSNotificationCenter defaultCenter] postNotificationName:kInAppPurchaseManagerProductsFetchedNotification object:self userInfo:nil];
}
#pragma -
#pragma Public methods

//
// call this method once on startup
//
- (void)loadStore
{
    //[self RefeshStore];
    [self requestAllIAPInformation];
    // restarts any purchases if they were interrupted last time the app was open
    [[SKPaymentQueue defaultQueue] addTransactionObserver:self];
    //[self RefeshStore];
    // get the product description (defined in early sections)
    //printf("\ncall request product");
    //[self requestProUpgradeProductData];
   //printf("\nLoad Store Completed");
}

//
// call this before making a purchase
//
- (BOOL)canMakePurchases
{
    return [SKPaymentQueue canMakePayments];
}

//
// kick off the upgrade transaction
//


#pragma -
#pragma Purchase helpers

//
// saves a record of the transaction by storing the receipt to disk
//
- (void)recordTransaction:(SKPaymentTransaction *)transaction
{
    if(transaction.payment.productIdentifier)
    {
        // save the transaction receipt to disk
        [[NSUserDefaults standardUserDefaults] setValue:transaction.transactionReceipt forKey:@"proUpgradeTransactionReceipt" ];
        [[NSUserDefaults standardUserDefaults] synchronize];
    }
}

//
// enable pro features
//
- (void)provideContent:(NSString *)productId
{
    if(productId)
    {
        // enable the pro features
        [[NSUserDefaults standardUserDefaults] setBool:YES forKey:productId ];
        [[NSUserDefaults standardUserDefaults] synchronize];
    }
}

//
// removes the transaction from the queue and posts a notification with the transaction result
//
- (void)finishTransaction:(SKPaymentTransaction *)transaction wasSuccessful:(BOOL)wasSuccessful
{
    // remove the transaction from the payment queue.
    [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
    
    NSDictionary *userInfo = [NSDictionary dictionaryWithObjectsAndKeys:transaction, @"transaction" , nil];
    if (wasSuccessful)
    {
        // send out a notification that we’ve finished the transaction
        [[NSNotificationCenter defaultCenter] postNotificationName:kInAppPurchaseManagerTransactionSucceededNotification object:self userInfo:userInfo];
    }
    else
    {
        // send out a notification for the failed transaction
        [[NSNotificationCenter defaultCenter] postNotificationName:kInAppPurchaseManagerTransactionFailedNotification object:self userInfo:userInfo];
    }
}

//
// called when the transaction was successful
//
- (void)completeTransaction:(SKPaymentTransaction *)transaction
{
    [self recordTransaction:transaction];
    [self provideContent:transaction.payment.productIdentifier];
    [self finishTransaction:transaction wasSuccessful:YES];
}

//
// called when a transaction has been restored and and successfully completed
//
- (void)restoreTransaction:(SKPaymentTransaction *)transaction
{
    [self recordTransaction:transaction.originalTransaction];
    [self provideContent:transaction.originalTransaction.payment.productIdentifier];
    [self finishTransaction:transaction wasSuccessful:YES];
}

//
// called when a transaction has failed
//
- (void)failedTransaction:(SKPaymentTransaction *)transaction
{
    
    if (transaction.error.code != SKErrorPaymentCancelled)
    {
        printf("\n error Transaction");
        // error!
        [self finishTransaction:transaction wasSuccessful:NO];
    }
    else
    {
        printf("\n cancel Transaction");
        // this is fine, the user just cancelled, so don’t notify
        [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
    }
}

#pragma mark -
#pragma mark SKPaymentTransactionObserver methods
- (BOOL)paymentQueue:(SKPaymentQueue *)queue
shouldAddStorePayment:(SKPayment *)payment
          forProduct:(SKProduct *)product
{
    return true;
}

//
// called when the transaction status is updated
//
- (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions
{
#if ADD_IAP
    for (SKPaymentTransaction *transaction in transactions)
    {
        NSInteger ii = transaction.transactionState;
        switch (ii)
        {
            case SKPaymentTransactionStatePurchased:
            {
                 printf("\nstate purchased");
                [self completeTransaction:transaction];

                	//NSString *productID = [[NSString alloc] initWithUTF8String:IAP_ID];
                    //const char* str_id = [[transaction.payment.productIdentifier retain] UTF8String];
                
                PURCHASE_FUNC([[transaction.payment.productIdentifier retain] UTF8String], enumIAPResponseStatus::IAP_RESPONSE_STATUS_SUCCESS);
                
                
                break;
            }
            case SKPaymentTransactionStateFailed:
                printf("\nstate failed");
                NSLog(@"\n payment error = %@ \n", [[transaction error] debugDescription]);
                [self failedTransaction:transaction];
                PURCHASE_FUNC(0, enumIAPResponseStatus::IAP_RESPONSE_STATUS_FAIL);
                break;
            case SKPaymentTransactionStateRestored:
            {
                printf("\nstate restore");
                [self restoreTransaction:transaction];
                //const char* str_id = [[transaction.payment.productIdentifier retain] UTF8String];
                
                PURCHASE_FUNC([[transaction.payment.productIdentifier retain] UTF8String], enumIAPResponseStatus::IAP_RESPONSE_STATUS_SUCCESS_RESTORE);
            }
                break;
            case SKPaymentTransactionStatePurchasing:
                printf("\nstate processing");
                break;
        }
    }
#endif
}

-(void) SetTimeOut:(int) time
{
    _time_request = time;
}
-(void) UpdateTimeOut:(int) dt
{
    _time_request -= dt;
}
-(int) GetTimeOut
{
    return _time_request;
}

@end
