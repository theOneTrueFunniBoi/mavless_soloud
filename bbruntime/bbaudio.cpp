
#include "std.h"
#include "bbaudio.h"

gxAudio *gx_audio;

static inline bool debugSound( gxSound *s,const char* a ){
	if( debug ){
		if( !gx_audio->verifySound( s ) ) {
			RTEX( "Sound does not exist" );
			return false;
		}
	} else {
		if ( !gx_audio->verifySound( s ) ) {
			errorLog.push_back(std::string(a)+std::string(": Sound does not exist"));
			return false;
		}
	}
	return true;
}

static gxSound *loadSound( BBStr *f,bool use_3d ){
	string t=*f;delete f;
	return gx_audio ? gx_audio->loadSound( t,use_3d ) : 0;
}

static gxChannel *playMusic( BBStr *f,bool use_3d ){
	string t=*f;delete f;
	return gx_audio ? gx_audio->playFile( t,use_3d ) : 0;
}

gxSound *bbLoadSound( BBStr *f ){
	return loadSound( f,false );
}

void bbFreeSound( gxSound *sound ){
	if( !sound ) return;
	if (!debugSound( sound, "FreeSound" )) return;
	gx_audio->freeSound( sound );
}

void bbLoopSound( gxSound *sound ){
	if( !sound ) return;
	if (!debugSound( sound, "LoopSound" )) return;
	sound->setLoop( true );
}

void bbSoundPitch( gxSound *sound,int pitch ){
	if( !sound ) return;
	if (!debugSound( sound, "SoundPitch" )) return;
	sound->setPitch( pitch );
}

void bbSoundVolume( gxSound *sound,float volume ){
	if( !sound ) return;
	if (!debugSound( sound, "SoundVolume" )) return;
	sound->setVolume( volume );
}

void bbSoundPan( gxSound *sound,float pan ){
	if( !sound ) return;
	if (!debugSound( sound, "SoundPan" )) return;
	sound->setPan( pan );
}

gxChannel *bbPlaySound( gxSound *sound ){
	if( !sound ) return 0;
	if (!debugSound( sound, "PlaySound" )) return 0;
	return sound->play();
}

gxChannel *bbPlayMusic( BBStr *f ){
	return playMusic( f,false );
}

gxChannel *bbPlayCDTrack( int track,int mode ){
	return gx_audio ? gx_audio->playCDTrack( track,mode ) : 0;
}

void bbStopChannel( gxChannel *channel ){
	if( !channel ) return;
	channel->stop();
}

void bbPauseChannel( gxChannel *channel ){
	if( !channel ) return;
	channel->setPaused( true );
}

void bbResumeChannel( gxChannel *channel ){
	if( !channel ) return;
	channel->setPaused( false );
}

void bbChannelPitch( gxChannel *channel,int pitch ){
	if( !channel ) return;
	channel->setPitch( pitch );
}

void bbChannelVolume( gxChannel *channel,float volume ){
	if( !channel ) return;
	channel->setVolume( volume );
}

void bbChannelPan( gxChannel *channel,float pan ){
	if( !channel ) return;
	channel->setPan( pan );
}

int bbChannelPlaying( gxChannel *channel ){
	return channel ? channel->isPlaying() : 0;
}

gxSound *bbLoad3DSound( BBStr *f ){
	return loadSound( f,true );
}

bool audio_create(){
	gx_audio=gx_runtime->openAudio( 0 );
	return true;
}

bool audio_destroy(){
	if( gx_audio ) gx_runtime->closeAudio( gx_audio );
	gx_audio=0;
	return true;
}

void audio_link( void(*rtSym)(const char*,void*) ){
	rtSym( "%LoadSound$filename",bbLoadSound );
	rtSym( "FreeSound%sound",bbFreeSound );
	rtSym( "LoopSound%sound",bbLoopSound );
	rtSym( "SoundPitch%sound%pitch",bbSoundPitch );
	rtSym( "SoundVolume%sound#volume",bbSoundVolume );
	rtSym( "SoundPan%sound#pan",bbSoundPan );
	rtSym( "%PlaySound%sound",bbPlaySound );
	rtSym( "%PlayMusic$midifile",bbPlayMusic );
	rtSym( "%PlayCDTrack%track%mode=1",bbPlayCDTrack );
	rtSym( "StopChannel%channel",bbStopChannel );
	rtSym( "PauseChannel%channel",bbPauseChannel );
	rtSym( "ResumeChannel%channel",bbResumeChannel );
	rtSym( "ChannelPitch%channel%pitch",bbChannelPitch );
	rtSym( "ChannelVolume%channel#volume",bbChannelVolume );
	rtSym( "ChannelPan%channel#pan",bbChannelPan );
	rtSym( "%ChannelPlaying%channel",bbChannelPlaying );
	rtSym( "%Load3DSound$filename",bbLoad3DSound );
}
