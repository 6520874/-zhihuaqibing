/*
 *  CBiOSStoreManager.mm
 *  CloudBox Cross-Platform Framework Project
 *
 *  Created by Cloud on 2012/10/30.
 *  Copyright 2011 Cloud Hsu. All rights reserved.
 *
 */

#import "CBiOSStoreManager.h"
#include "NSData+Base64.h"

//TalkingData统计
#include "TDCCVirtualCurrency.h"
#include "TDCCTalkingDataGA.h"


//**********************新的价格体系***********************************************************

#define ProductID_LargeNumberGemstone @"LargeNumberGemstone"   //648元购买一大车宝石
#define ProductID_ALittleStone  @"ALittleStone"   //1元购买一小袋宝石
#define ProductID_PreciousStones @"PreciousStones"   //198元购买一箱宝石
#define ProductID_BagOfStones @"BagOfStones"   //98元购买一袋宝石
#define ProductID_CartfulGemstone @"CartfulGemstone"   //328购买一车宝石
#define ProductID_halfStone @"halfStone"   //30元购买半袋宝石

@implementation CBiOSStoreManager
@synthesize PaySuccess;
static CBiOSStoreManager* _sharedInstance = nil;

+(CBiOSStoreManager*)sharedInstance
{
	@synchronized([CBiOSStoreManager class])
	{
		if (!_sharedInstance)
		_sharedInstance=	[[self alloc] init];
        
		return _sharedInstance;
	}
	return nil;
}

+(id)alloc
{
	@synchronized([CBiOSStoreManager class])
	{
		NSAssert(_sharedInstance == nil, @"Attempted to allocate a second instance of a singleton.\n");
		_sharedInstance = [super alloc];
		return _sharedInstance;
	}
	return nil;
}

-(id)init {
	self = [super init];
	if (self != nil) {
		// initialize stuff here
	}
	return self;
}

-(void)initialStore: (chargeInfoLayer *)layer
{
    [[SKPaymentQueue defaultQueue] addTransactionObserver:self];
    charLayer = layer;
}
-(void)releaseStore
{
    NSLog(@"releaseStore");
    [[SKPaymentQueue defaultQueue] removeTransactionObserver:self];
}

-(void)buy:(NSString*)productIDTag
{
    [self requestProductData:productIDTag];
}

-(void) restoreBuy
{
    
    
    [[SKPaymentQueue defaultQueue] addTransactionObserver:self];
    [[SKPaymentQueue defaultQueue] restoreCompletedTransactions];
}

-(bool)CanMakePay
{
    return [SKPaymentQueue canMakePayments];
}

-(void)requestProductData:(NSString*)productIDTag
{
    NSLog(@"---------请求产品信息------------\n");
    NSLog(@"productIDTag:%@",productIDTag);
    NSArray *product = [[NSArray alloc] initWithObjects:productIDTag,nil];
    NSSet *nsset = [NSSet setWithArray:product];
    SKProductsRequest *request=[[SKProductsRequest alloc] initWithProductIdentifiers: nsset];
    request.delegate=self;
    [request start];
}

- (void)productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response{
    
    NSLog(@"-----------获取产品信息--------------\n");
    
    
    NSArray *myProduct = response.products;
    
    NSLog(@"Product ID:%@\n",response.invalidProductIdentifiers);
    NSLog(@"Product count: %ld\n", [myProduct count]);
    
    
    if (myProduct.count == 0) {
        NSLog(@"无法获取产品信息，购买失败。");
        return;
    }
    
    NSLog(@"Product ID:%@\n",response.invalidProductIdentifiers);
    NSLog(@"Product count: %ld\n", [myProduct count]);
    // populate UI
    for(SKProduct *product in myProduct){
        NSLog(@"Detail product info\n");
        NSLog(@"SKProduct description: %@\n", [product description]);
        NSLog(@"Product localized title: %@\n" , product.localizedTitle);
        NSLog(@"Product localized descitption: %@\n" , product.localizedDescription);
        NSLog(@"Product price: %@\n" , product.price);
        NSLog(@"Product identifier: %@\n" , product.productIdentifier);
    }
    SKPayment *payment = nil;
    
    payment = [SKPayment paymentWithProduct:[response.products objectAtIndex:0]];
    NSLog(@"---------要求付款------------\n");
    [[SKPaymentQueue defaultQueue] addPayment:payment];
    
}
- (void)requestProUpgradeProductData:(NSString*)productIDTag
{
    NSLog(@"------要求升级产品数据---------\n");
    NSSet *productIdentifiers = [NSSet setWithObject:productIDTag];
    SKProductsRequest* productsRequest = [[SKProductsRequest alloc] initWithProductIdentifiers:productIdentifiers];
    productsRequest.delegate = self;
    [productsRequest start];
    
}

- (void)request:(SKRequest *)request didFailWithError:(NSError *)error
{
    
    NSLog(@"-------请求失败的信息----------\n");
    UIAlertView *alerView =  [[[UIAlertView alloc] initWithTitle:nil message:[error localizedDescription]
                                                       delegate:nil cancelButtonTitle:@"关闭" otherButtonTitles:nil]autorelease];
    [alerView show];
}

-(void) requestDidFinish:(SKRequest *)request
{
    NSLog(@"----------请求完成--------------\n");
//    [Bridge removeAnimationBackWaitView];
    
}




-(void) purchasedTransaction: (SKPaymentTransaction *)transaction
{
    NSLog(@"-----Purchased Transaction购买交易----\n");
    NSArray *transactions =[[[NSArray alloc] initWithObjects:transaction, nil]autorelease];
    [self paymentQueue:[SKPaymentQueue defaultQueue] updatedTransactions:transactions];
}


- (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions//交易结果
{
    NSLog(@"[transactions count]:%d",[transactions count]);
    //CCLOG(@"-----paymentQueue--------");
    for (SKPaymentTransaction *transaction in transactions)
    {
        switch (transaction.transactionState)
        {
                
            case SKPaymentTransactionStatePurchased:
            {
                
                //购买完成
                [self completeTransaction:transaction];
                NSLog(@"-----交易完成 --------");
                
                
                break;
            }
            case SKPaymentTransactionStateFailed:
            {
                //交易失败
                [self failedTransaction:transaction];
                NSLog(@"-----交易失败 --------");
                
                
                break;
            }
            case SKPaymentTransactionStateRestored:
            {
                //恢复购买
                [self restoreTransaction:transaction];
                NSLog(@"-----已经购买过该商品 --------");
                break;
            }
            case SKPaymentTransactionStatePurchasing:
            {
                //商品添加进列表
                NSLog(@"-----商品添加进列表 --------");
                break;
            }
            default:
                break;
        }
    }
}


- (void) completeTransaction: (SKPaymentTransaction *)transaction
{
    
    NSLog(@"Transaction　complete");

    NSLog(@"-----completeTransaction--------\n");
    // Your application should implement these two methods.
    NSString *product = transaction.payment.productIdentifier;
    if ([product length] > 0) {
        
        NSArray *tt = [product componentsSeparatedByString:@"."];
        NSString *bookid = [tt lastObject];
        if ([bookid length] > 0) {
            [self recordTransaction:bookid];
            NSLog(@"product::%@",product);
            [self ChangeLingShiCount:product];
            
        }
    }
    
    // Remove the transaction from the payment queue.
    [[SKPaymentQueue defaultQueue] finishTransaction: transaction];
    
    
}
- (void)alertView:(UIAlertView *)alertView didDismissWithButtonIndex:(NSInteger)buttonIndex{
    if (alertView==SuccessalerView) {
        //[PaySuccess PaySuccessfulReturn:YES];
    }
}

-(void)ChangeLingShiCount:(NSString *)productID{
    
    int lingshiCount=0;
    int count =0;
    int  money=0;
    int ConsumerRecords=0;//消费记录
    buyMonthCard = false;
    NSUserDefaults *userDefaults = [NSUserDefaults standardUserDefaults];

    if ([productID isEqualToString:ProductID_LargeNumberGemstone]) {
        lingshiCount=8100;//648元购买一大车宝石
//        ConsumerRecords=60;
//        count=MY_BASIC_INFO->getUserMagicStone()+lingshiCount;
//        if (_Statistics)
//        {
//            umeng::MobClickCpp::pay(6, 1, (double)lingshiCount);//通过App Store 花6元买了6个魔法石
//        }
        money=648;
        
    }
    else if([productID isEqualToString:ProductID_ALittleStone]){
        lingshiCount=100;//1元购买一小袋宝石
//        ConsumerRecords=300;
//        count=MY_BASIC_INFO->getUserMagicStone()+lingshiCount;
////        MY_BASIC_INFO->setMagicStone(count,0,lingshiCount);
//        if (_Statistics)
//        {
//            umeng::MobClickCpp::pay(30, 1, (double)lingshiCount);//通过App Store 花30元买了300个魔法石
//        }
       money=1;
        
    }
    else if([productID isEqualToString:ProductID_PreciousStones]){
        lingshiCount=2200;
//         ConsumerRecords=980;
//        count=MY_BASIC_INFO->getUserMagicStone()+lingshiCount;
////        MY_BASIC_INFO->setMagicStone(count,0,lingshiCount);
//        if (_Statistics)
//        {
//            umeng::MobClickCpp::pay(98, 1, (double)lingshiCount);//通过App Store 花98元买了980个魔法石
//        }
       money=198;
       
    }
    else if([productID isEqualToString:ProductID_BagOfStones]){
        lingshiCount=1031;  //98元购买一箱宝石
//        ConsumerRecords=1980;
//        count=MY_BASIC_INFO->getUserMagicStone()+lingshiCount;
////        MY_BASIC_INFO->setMagicStone(count,0,lingshiCount);
//        if (_Statistics)
//        {
//            umeng::MobClickCpp::pay(198, 1, (double)lingshiCount);//通过App Store 花198元买了1980个魔法石
//        }
        money=98;
    }
    else if([productID isEqualToString:ProductID_CartfulGemstone]){
        lingshiCount=3858;  //328元购买一车宝石
//        ConsumerRecords=3280;
//        count=MY_BASIC_INFO->getUserMagicStone()+lingshiCount;
////        MY_BASIC_INFO->setMagicStone(count,0,lingshiCount);
//        if (_Statistics)
//        {
//            umeng::MobClickCpp::pay(328, 1, (double)lingshiCount);//通过App Store 花328元买了3280个魔法石
//        }
        money=328;
    }
    else if([productID isEqualToString:ProductID_halfStone]){
        lingshiCount=300;   //30元购买半袋宝石
//        ConsumerRecords=6480;
//        count=MY_BASIC_INFO->getUserMagicStone()+lingshiCount;
////        MY_BASIC_INFO->setMagicStone(count,0,lingshiCount);
//        if (_Statistics)
//        {
//            umeng::MobClickCpp::pay(648, 1, (double)lingshiCount);//通过App Store 花648元买了6480个魔法石
//        }
       money=30;
    }
    
    charLayer->iapSendMessage();

//    NSDate* dat = [NSDate dateWithTimeIntervalSinceNow:0];
//    NSTimeInterval a=[dat timeIntervalSince1970]*1000;
//    NSString *timeString = [NSString stringWithFormat:@"%f", a];//转为字符型
//    
//    NSString * billNumString=[NSString stringWithFormat:@"%d#%@",MY_BASIC_INFO->m_strUserInfo.m_nUserID,timeString];
//    [self sendBuySuccessToServer:MY_BASIC_INFO->m_strUserInfo.m_nUserID andaddDiamond:lingshiCount andauthcode:[NSString stringWithCString:MY_BASIC_INFO->m_strUserInfo.m_sAuthCode.c_str() encoding:[NSString defaultCStringEncoding]] andmoney:money andbillNumber:billNumString];
//    
//    MY_BASIC_INFO->SaveUserMCF(NULL, NULL, count ,0, ConsumerRecords,0, 0, 0, -1);
//    NSLog(@"购买：%d 个魔法石成功",lingshiCount);
//    NSLog(@"%d", MY_BASIC_INFO->m_strUserInfo.m_iUserVipExp);
//    MY_BASIC_INFO->updateUserVipExp(MY_BASIC_INFO->m_strUserInfo.m_iUserVipExp+money*10);  //首充 魔法石翻倍  VIP经验不翻倍
//    NSString *Order=[userDefaults objectForKey:@"appProductOrder"];
//    CCLOG("%s",[Order UTF8String]);
//    if(_Statistics){
//        TDCCVirtualCurrency::onChargeSuccess([Order UTF8String]);
//    }
    [PaySuccess PaySuccessfulReturn:YES];
}

//-(void)sendBuySuccessToServer:(int)userid andaddDiamond:(int)addDiamond andauthcode:(NSString *)authcode andmoney:(int)money andbillNumber:(NSString *)billNumber{
//    
//    NSString *url=[NSString stringWithFormat:@"%@CardGameProgram/saveIosBill?userId=%d&addDiamond=%d&authcode=%@&goodId=%d&billNo=%@",URL_HTTP_ADDRESS,userid,addDiamond,authcode,money,billNumber];
//    NSLog(@"url：%@",url);
//    NSString *urlString=[url stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
//  ASIHTTPRequest*  messagesRequest = [[ASIHTTPRequest alloc] initWithURL:[NSURL URLWithString:urlString]];
//    [messagesRequest setDelegate:self];
//    [messagesRequest setDidFinishSelector : @selector (requestFinished:)];
//    [messagesRequest setDidFailSelector: @selector (requestFailed:)];
//    [messagesRequest startAsynchronous ];
//
//    
//}
//-(void)requestFinished:(ASIHTTPRequest *)request
//{
//    //NSLog(@"111111111111");
//    //NSLog(@"t%d",t);
//    NSData *responseData=[request responseData];
//    NSString *responseString22 = [[NSString alloc] initWithData:responseData encoding:NSUTF8StringEncoding];
//    NSLog(@"myRequestString:%@",responseString22);
//    
//    if (buyMonthCard) {
//        MY_BASIC_INFO->BuySuccess();
//    }
//}
//-(void)requestFailed:(ASIHTTPRequest *)request
//{
//    
//}
//#define ProductID_100_Lingshi  @"cn.xinpin.100_Lingshi"
-(void)recordTransaction:(NSString *)product
{
    NSLog(@"-----Record transcation--------\n");
    // Todo: Maybe you want to save transaction result into plist.
    NSLog(@"product：%@",product);
    
}

-(void)provideContent:(NSString *)product
{
    NSLog(@"-----Download product content--------\n");
    NSLog(@"product：%@",product);
}

- (void) failedTransaction: (SKPaymentTransaction *)transaction
{
    NSLog(@"Failed\n");
    if (transaction.error.code != SKErrorPaymentCancelled)
    {
        UIAlertView *alerView2 =  [[[UIAlertView alloc] initWithTitle:nil
                                                             message:@"购买失败，请再次尝试。"
                                                            delegate:nil cancelButtonTitle:@"关闭" otherButtonTitles:nil]autorelease];
        
        [alerView2 show];
    }
    [[SKPaymentQueue defaultQueue] finishTransaction: transaction];
    
}
-(void) paymentQueueRestoreCompletedTransactionsFinished: (SKPaymentTransaction *)transaction
{   NSLog(@"恢复购买完成");
    
}

- (void) restoreTransaction: (SKPaymentTransaction *)transaction
{
    NSLog(@"恢复");
    NSLog(@"恢复购买成功");
    // [[MovieStore defaultStore] addPayInfo:@"4"];
    //购买成功
    UIAlertView *alerView =  [[[UIAlertView alloc] initWithTitle:nil
                                                        message:@"恢复购买成功。"
                                                       delegate:nil cancelButtonTitle:@"关闭" otherButtonTitles:nil]autorelease];
    
    [alerView show];
    
    SKPayment *payment = [transaction payment];
    
    [self recordTransaction:[payment productIdentifier]];
    
    [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
    
}

-(void) paymentQueue:(SKPaymentQueue *) paymentQueue restoreCompletedTransactionsFailedWithError:(NSError *)error
{
    NSLog(@"恢复失败");
    //购买成功
    UIAlertView *alerView =  [[[UIAlertView alloc] initWithTitle:nil
                                                        message:@"恢复失败。"
                                                       delegate:nil cancelButtonTitle:@"关闭" otherButtonTitles:nil]autorelease];
    [alerView show];
}

#pragma mark connection delegate
- (void)connection:(NSURLConnection *)connection didReceiveData:(NSData *)data
{
    NSLog(@"%@\n",  [[[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding]autorelease]);
}
- (void)connectionDidFinishLoading:(NSURLConnection *)connection
{
    
}

- (void)connection:(NSURLConnection *)connection didReceiveResponse:(NSURLResponse *)response
{
    switch([(NSHTTPURLResponse *)response statusCode]) {
        case 200:
        case 206:
            break;
        case 304:
            break;
        case 400:
            break;
        case 404:
            break;
        case 416:
            break;
        case 403:
            break;
        case 401:
        case 500:
            break;
        default:
            break;
    }
}

- (void)connection:(NSURLConnection *)connection didFailWithError:(NSError *)error
{
    NSLog(@"test\n");
}

@end