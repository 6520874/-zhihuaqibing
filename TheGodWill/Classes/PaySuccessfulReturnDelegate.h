//
//  PaySuccessfulReturnDelegate.h
//  eyesightprotectionvideoplayer
//
//  Created by user on 13-4-17.
//
//

#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	#import <Foundation/Foundation.h>
#endif

@protocol PaySuccessfulReturnDelegate <NSObject>


@required

-(void) PaySuccessfulReturn:(BOOL)isSuccess;

@end
