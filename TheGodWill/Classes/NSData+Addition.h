//
//  NSData+Addition.h
//  PurchaseTest
//
//  Created by 欧然 Wu on 12-2-20.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	#import <Foundation/Foundation.h>
#endif

@interface NSData (Addition)
- (NSString*)base64String;

@end
