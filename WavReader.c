
#include "inc/tm4c123gh6pm.h"
#include "PLL.h"
//#include "DAC.h"
#include "MAX5353.h"
//#include "Timer0A.h"
//#include "Timer1A.h"
#include "Timer2A.h"
#include <stdint.h>
#include <stdlib.h>
#include "diskio.h"
#include "ff.h"
#include "integer.h"
#include <stdio.h>
#include <string.h>

//this file was adapted from code from this webpage 
//http://stackoverflow.com/questions/16075233/reading-and-processing-wav-file-data-in-c-c
/*
this file will read and output 8-bit PCM uncompressed, unsigned wav files.
*/
static FATFS g_sFatFs;
FIL Handle,Handle2;
FRESULT MountFresult;
FRESULT Fresult;
unsigned char buffer[512];
#define MAXBLOCKS 100
#define MAXSONGBLOCKS 95
#define FALSE 0
#define TRUE 1
#define OPENING "opening.wav"
#define WAKA "waka.wav"
#define GHOST "ghost.wav"
#define FRUIT "cherry.wav"
#define DEATH "die.wav"

//volume is the define statement that sets the volume
#define VOLUME 4

enum WavChunks {
    RiffHeader = 0x46464952,
    WavRiff = 0x54651475,
    Format = 0x020746d66,
    LabeledText = 0x478747C6,
    Instrumentation = 0x478747C6,
    Sample = 0x6C706D73,
    Fact = 0x47361666,
    Data = 0x61746164,
    Junk = 0x4b4e554a,
};

enum WavFormat {
    PulseCodeModulation = 0x01,
    IEEEFloatingPoint = 0x03,
    ALaw = 0x06,
    MuLaw = 0x07,
    IMAADPCM = 0x11,
    YamahaITUG723ADPCM = 0x16,
    GSM610 = 0x31,
    ITUG721ADPCM = 0x40,
    MPEG = 0x50,
    Extensible = 0xFFFE
};



void music_init(void){
		 MountFresult = f_mount(&g_sFatFs, "", 0);
		 DAC_Init(0);
}


uint32_t chunkid;
uint32_t formatsize;
uint16_t format;
uint16_t channels;
int channelcount;
uint32_t samplerate;
uint32_t bitspersecond;
uint16_t formatblockalign;
uint16_t bitdepth;
uint32_t headerid;
uint32_t memsize;
uint32_t riffstyle;
uint32_t datasize;
unsigned char buffer[512];
unsigned char secondbuffer[512];
UINT successfulreads, successfulwrites;

//index will be my index in the buffer
uint32_t index;
//samplenum is the sample I am outputting
uint32_t sampleNum;
//needMore is a global flag that says we need to load into the next buffer
uint8_t needMore;
//whichBuffer = 1, we are currently outputting the samples in buffer, otherwise we are in secondbuffer
//i am doing an alternating buffer system, one buffer will have the samples currently being outputted, the other buffer will have the next samples to output
uint8_t whichBuffer;
void handler(void);
void handler2(void);

//char* f_name;
char* current_song;
char* data_start = 0;
uint8_t looped;
uint8_t start_play;
uint8_t song_playing;
uint8_t temp_count = 0;
uint8_t debug = 0;

void music_stop(void){
	Fresult = f_close(&Handle);
	TIMER2_CTL_R = 0x00000000;
	//if (!looped) free(f_name);
	song_playing = FALSE;
}

void music_play(const char* fileName){
	uint8_t datachunk = 0; 	//this variable will act as a boolean
	uint8_t gotFormat = 0;
	uint16_t skipsize;
	uint16_t extradata;
	char* temp_name;
	chunkid = 0;
	// save filename for later.
	/*
	if (f_name != NULL && !strcmp(f_name, fileName)){
		music_stop();
	}
	f_name = (char*)malloc(strlen(fileName) + 1);
	memcpy(f_name, fileName, strlen(fileName));
	f_name[strlen(fileName)] = 0;
	*/
	if (song_playing) music_stop();
	if (!strcmp(fileName, OPENING)){
		current_song = OPENING;
	}
	else if (!strcmp(fileName, WAKA)){
		current_song = WAKA;
	}
	else if (!strcmp(fileName, GHOST)){
		current_song = GHOST;
	}
	else if (!strcmp(fileName, FRUIT)){
		current_song = FRUIT;
	}
	else {
		current_song = DEATH;
	}
	
	index = 0;
	sampleNum = 0;
	needMore = 0;
	whichBuffer = 1;
	if (temp_count >= 39){
		debug = 1;
	}
	Fresult = f_open(&Handle, fileName, FA_READ);
	Fresult = f_read(&Handle, buffer, 512, &successfulreads);
	if(Fresult == FR_OK){
		temp_count++;
		while( !datachunk ) {
				//chunkid = reader.ReadInt32( );
				start_play = FALSE;
				if(index >= 512){
					Fresult = f_read(&Handle, buffer, 512, &successfulreads);
					index = index - 512;
				}
				//chunkid = (int) buffer[index];
				if(!gotFormat){
					memcpy(&chunkid, &buffer[index], sizeof chunkid);
					index +=4;
				}
				else{
					memcpy(&chunkid, &buffer[index], sizeof chunkid);
					index +=1;
				}
				
				switch ( chunkid ) {
				case Format:
						gotFormat = 1;
						//formatsize = reader.ReadInt32( );
						memcpy(&formatsize, &buffer[index], sizeof formatsize);
						index+=4;
				
						//format = (WavFormat)reader.ReadInt16( );
						memcpy(&format, &buffer[index], sizeof format);
						index+=2;
				
						//channels = (Channels)reader.ReadInt16( );
						memcpy(&channels, &buffer[index], sizeof channels);
						index +=2;
				
						channelcount = (int)channels;
				
						//samplerate = reader.ReadInt32( );
						memcpy(&samplerate, &buffer[index], sizeof samplerate);
						index+=4;
				
						//bitspersecond = reader.ReadInt32( );
						memcpy(&bitspersecond, &buffer[index], sizeof bitspersecond);
						index+=4;
						
						//formatblockalign = reader.ReadInt16( );
						memcpy(&formatblockalign, &buffer[index], sizeof formatblockalign);
						index+=2;
						
						//bitdepth = reader.ReadInt16( );
						memcpy(&bitdepth, &buffer[index], sizeof bitdepth);
						index+=2;
						
						if ( formatsize == 18 ) {
								//int32_t extradata = reader.ReadInt16( );
							
								memcpy(&extradata, &buffer[index], sizeof extradata);
								index+=2;
								index+=extradata;
								//reader.Seek( extradata, SeekOrigin::Current );
						}
						break;
				case RiffHeader:
						headerid = chunkid;
						//memsize = reader.ReadInt32( );
						memcpy(&memsize, &buffer[index], sizeof memsize);
						index+=4;
				
						//riffstyle = reader.ReadInt32( );
						memcpy(&riffstyle, &buffer[index], sizeof riffstyle);
						index+=4;
						break;
				case Data:
						datachunk = 1;
						//datasize = reader.ReadInt32( );
						memcpy(&datasize, &buffer[index], sizeof datasize);
						index+=4;
						data_start = &buffer[index];
						break;
				default:
						//int32_t skipsize = reader.ReadInt32( );
						//reader.Seek( skipsize, SeekOrigin::Current );
						/*
						memcpy(&skipsize, &buffer[index], sizeof skipsize);
						index+=2;
						index+=skipsize;
						*/
						break;
				}
		}
		//if 8-bit PCM uncomment this and use handler
		Timer2A_Init(handler, 80000000/samplerate);
		//if larger than 8-bit PCM use this
		//Timer2A_Init(handler2, 80000000/samplerate);
	}
}

void load_more(void){
	needMore = 0;
	if(whichBuffer){
		Fresult = f_read(&Handle, secondbuffer, 512, &successfulreads);
	}
	else{
		Fresult = f_read(&Handle, buffer, 512, &successfulreads);
	}
	if (!start_play){
		int i, count;
		count = 0;
		for (i=0; i<512; i++){
			char current = buffer[i];
			if (current == 0x80) ++count;
		}
		if (count < 128){
			start_play = TRUE;
			song_playing = TRUE;
		}
	}
	if (successfulreads < 512){
		if (looped) {
			music_play(current_song);
		} else {
			music_stop();
		}
	}
}


//this handler is verified to work very well with 8-bit PCM wav files
void handler(void){
		//i am going to read the sample into a 32 bit integer and then convert it down
		uint64_t sample = 0;
		uint16_t truncated_sample = 0;
		if (!start_play){
			sampleNum += (bitdepth/8);
			index+= (bitdepth/8);
			if(index >= 512){
				index = 0;
				whichBuffer ^= 0x01;
				needMore = 1;
			}
			return;
		}
		if(sampleNum > datasize){
			music_stop();
		}
		if(index >= 512){
			index = 0;
			whichBuffer ^= 0x01;
			needMore = 1;
		}
		
		if(whichBuffer){
			memcpy(&sample, &buffer[index], bitdepth/8);
		}
		else{
			memcpy(&sample, &secondbuffer[index], bitdepth/8);
		}
		if(bitdepth >= 12){
			sample = sample >> (bitdepth - 12);
		}
		truncated_sample = (uint16_t) sample;
		DAC_Out(VOLUME*truncated_sample);
		
		sampleNum += (bitdepth/8);
		index+= (bitdepth/8);
		
}

//This is a handler I am experimenting witH to try and support 16-bit and up PCM
void handler2(void){
		//i am going to read the sample into a 32 bit integer and then convert it down
		uint64_t sample = 0;
		uint16_t truncated_sample = 0;
		if(sampleNum > datasize){
			music_stop();
		}
		if(index >= 512){
			index = 0;
			whichBuffer ^= 0x01;
			needMore = 1;
		}
		
		if(whichBuffer){
			memcpy(&sample, &buffer[index], bitdepth/8);
		}
		else{
			memcpy(&sample, &secondbuffer[index], bitdepth/8);
		}
		if(bitdepth >= 12){
			sample = sample >> (bitdepth - 12);
		}
		truncated_sample = (uint16_t) sample;
		DAC_Out(VOLUME*truncated_sample);
		
		sampleNum += (bitdepth/8);
		index+= (bitdepth/8);
		
}