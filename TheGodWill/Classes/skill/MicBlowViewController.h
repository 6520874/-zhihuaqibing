#import <UIKit/UIKit.h>
#import <AVFoundation/AVFoundation.h>
#import <CoreAudio/CoreAudioTypes.h>

#include "fightScene.h"

@interface MicBlowViewController : UIViewController <AVAudioPlayerDelegate> {
	AVAudioRecorder *recorder;
	NSTimer *levelTimer;
	double lowPassResults;
    CFightScene* m_pcallBack;
}

- (void)levelTimerCallback:(NSTimer *)timer;
- (void)setThe:(CFightScene*)callback;

- (void)RecordOverCallBack;

- (void)PlayTheRecord:(NSTimer*)timer;

@end

