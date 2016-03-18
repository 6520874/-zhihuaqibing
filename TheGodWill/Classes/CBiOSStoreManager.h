/*
 *  CBiOSStoreManager.h
 *  CloudBox Cross-Platform Framework Project
 *
 *  Created by Cloud on 2012/10/30.
 *  Copyright 2011 Cloud Hsu. All rights reserved.
 *
 */

#import <UIKit/UIKit.h>
#import <StoreKit/StoreKit.h>
#import <StoreKit/SKPaymentTransaction.h>
#ifndef NDEBUG
#define kReceiptValidationURL @"https://sandbox.itunes.apple.com/verifyReceipt"
#else
#define kReceiptValidationURL @"https://buy.itunes.apple.com/verifyReceipt"
#endif
#import "PaySuccessfulReturnDelegate.h"

#include "chargeInfoLayer.h"

//#import "Bridge.h"
@interface CBiOSStoreManager : NSObject<SKProductsRequestDelegate,SKPaymentTransactionObserver,UIAlertViewDelegate  >
{
    int buyType;
    
    UIAlertView *SuccessalerView;
    id<PaySuccessfulReturnDelegate>PaySuccess;
    
    BOOL buyMonthCard;
    
    chargeInfoLayer *charLayer;
}
@property (nonatomic, retain) id<PaySuccessfulReturnDelegate>PaySuccess;


+ (CBiOSStoreManager*) sharedInstance;

- (void) buy:(NSString*)buyProductIDTag;
- (void) restoreBuy;
- (bool) CanMakePay;
- (void) initialStore: (chargeInfoLayer *)layer;
- (void) releaseStore;
- (void) requestProductData:(NSString*)buyProductIDTag;
- (void) provideContent:(NSString *)product;
- (void) recordTransaction:(NSString *)product;

- (void) requestProUpgradeProductData:(NSString*)buyProductIDTag;
- (void) paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions;
- (void) purchasedTransaction: (SKPaymentTransaction *)transaction;
- (void) completeTransaction: (SKPaymentTransaction *)transaction;
- (void) failedTransaction: (SKPaymentTransaction *)transaction;
- (void) paymentQueueRestoreCompletedTransactionsFinished: (SKPaymentTransaction *)transaction;
//-(void)paymentQueueRestoreCompletedTransactionsFinished:(SKPaymentQueue *)queue;
- (void) paymentQueue:(SKPaymentQueue *) paymentQueue restoreCompletedTransactionsFailedWithError:(NSError *)error;
- (void) restoreTransaction: (SKPaymentTransaction *)transaction;
@end