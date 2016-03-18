#import "MicBlowViewController.h"

@implementation MicBlowViewController

- (void)viewDidLoad {
    [super viewDidLoad];

    
    AVAudioSession *session = [AVAudioSession sharedInstance];

    NSError *sessionError;
    [session setCategory:AVAudioSessionCategoryPlayAndRecord error:&sessionError];

    if(session == nil)
        NSLog(@"Error creating session: %@", [sessionError description]);
    else
        [session setActive:YES error:nil];
    
    
    
	NSURL *url = [NSURL fileURLWithPath:@"/dev/null"];
    
	NSDictionary *settings = [NSDictionary dictionaryWithObjectsAndKeys:
							  [NSNumber numberWithFloat: 44100.0],                 AVSampleRateKey,
							  [NSNumber numberWithInt: kAudioFormatAppleLossless], AVFormatIDKey,
							  [NSNumber numberWithInt: 1],                         AVNumberOfChannelsKey,
							  [NSNumber numberWithInt: AVAudioQualityMax],         AVEncoderAudioQualityKey,
							  nil];
		
	NSError *error;
		
	recorder = [[AVAudioRecorder alloc] initWithURL:url settings:settings error:&error];
		
	if (recorder) {
		[recorder prepareToRecord];
		recorder.meteringEnabled = YES;
		[recorder record];
		levelTimer = [NSTimer scheduledTimerWithTimeInterval: 0.03 target: self selector: @selector(levelTimerCallback:) userInfo: nil repeats: YES];

	} else
		NSLog([error description]);
    
    
//    AVAudioSession *session = [AVAudioSession sharedInstance];
//    
//    NSError *sessionError;
//    [session setCategory:AVAudioSessionCategoryPlayAndRecord error:&sessionError];
//    
//    if(session == nil)
//        NSLog(@"Error creating session: %@", [sessionError description]);
//    else
//        [session setActive:YES error:nil];
//    
//    
//    
//    NSURL *recordedFile = [NSURL fileURLWithPath:[NSTemporaryDirectory() stringByAppendingString:@"RecordedFile"]];
//    recorder = [[AVAudioRecorder alloc] initWithURL:recordedFile settings:nil error:nil];
//    
//    if (recorder) {
//        [recorder prepareToRecord];
//        [recorder record];
//        
////        [NSTimer scheduledTimerWithTimeInterval:3 target:self selector:@selector(PlayTheRecord:) userInfo:nil repeats:NO];
//        
//        levelTimer = [NSTimer scheduledTimerWithTimeInterval: 0.03 target: self selector: @selector(levelTimerCallback:) userInfo: nil repeats: YES];
//    }
    
    
    
    
    
}

- (void)setThe:(CFightScene*)callback{
    
    m_pcallBack = callback;
    
}


- (void)levelTimerCallback:(NSTimer *)timer {
	[recorder updateMeters];

	const double ALPHA = 0.05;
	double peakPowerForChannel = pow(10, (0.05 * [recorder peakPowerForChannel:0]));
	lowPassResults = ALPHA * peakPowerForChannel + (1.0 - ALPHA) * lowPassResults;	
	
    
    
//    if (lowPassResults>0.001) {
//        NSLog(@"value = %f", lowPassResults);
//    }
    
    
    if((lowPassResults > 0.32) && (lowPassResults < 0.95))
    {
        NSLog(@"value = %lf", peakPowerForChannel);
        m_pcallBack->m_bValid = true;
        m_pcallBack->m_volume = lowPassResults;
    }
}

- (void)PlayTheRecord:(NSTimer *)timer{
    
    [recorder stop];
    recorder = nil;
    
    NSURL *recordedFile = [NSURL fileURLWithPath:[NSTemporaryDirectory() stringByAppendingString:@"RecordedFile"]];
    
    
    
    
    NSError *playerError;
    
    AVAudioPlayer* player = [[AVAudioPlayer alloc] initWithContentsOfURL:recordedFile error:&playerError];
    
    
    if (player == nil)
    {
        NSLog(@"ERror creating player: %@", [playerError description]);
    }
    player.delegate = self;
    
    
    [player play];
    
}

- (void)RecordOverCallBack{
    
    [recorder stop];
    
    [levelTimer invalidate];
}





- (void)dealloc {
	[levelTimer release];
	[recorder release];
    [super dealloc];
}

@end
