/*

    File: MyOpenALSupport.c
Abstract: OpenAL-related support functions
 Version: 1.7

Disclaimer: IMPORTANT:  This Apple software is supplied to you by Apple
Inc. ("Apple") in consideration of your agreement to the following
terms, and your use, installation, modification or redistribution of
this Apple software constitutes acceptance of these terms.  If you do
not agree with these terms, please do not use, install, modify or
redistribute this Apple software.

In consideration of your agreement to abide by the following terms, and
subject to these terms, Apple grants you a personal, non-exclusive
license, under Apple's copyrights in this original Apple software (the
"Apple Software"), to use, reproduce, modify and redistribute the Apple
Software, with or without modifications, in source and/or binary forms;
provided that if you redistribute the Apple Software in its entirety and
without modifications, you must retain this notice and the following
text and disclaimers in all such redistributions of the Apple Software.
Neither the name, trademarks, service marks or logos of Apple Inc. may
be used to endorse or promote products derived from the Apple Software
without specific prior written permission from Apple.  Except as
expressly stated in this notice, no other rights or licenses, express or
implied, are granted by Apple herein, including but not limited to any
patent rights that may be infringed by your derivative works or by other
works in which the Apple Software may be incorporated.

The Apple Software is provided by Apple on an "AS IS" basis.  APPLE
MAKES NO WARRANTIES, EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION
THE IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS
FOR A PARTICULAR PURPOSE, REGARDING THE APPLE SOFTWARE OR ITS USE AND
OPERATION ALONE OR IN COMBINATION WITH YOUR PRODUCTS.

IN NO EVENT SHALL APPLE BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL
OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) ARISING IN ANY WAY OUT OF THE USE, REPRODUCTION,
MODIFICATION AND/OR DISTRIBUTION OF THE APPLE SOFTWARE, HOWEVER CAUSED
AND WHETHER UNDER THEORY OF CONTRACT, TORT (INCLUDING NEGLIGENCE),
STRICT LIABILITY OR OTHERWISE, EVEN IF APPLE HAS BEEN ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.

Copyright (C) 2009 Apple Inc. All Rights Reserved.


*/

#include "MyOpenALSupport.h"
//#include "BufferStream.h"

ALvoid  alBufferDataStaticProc(const ALint bid, ALenum format, ALvoid* data, ALsizei size, ALsizei freq)
{
	static	alBufferDataStaticProcPtr	proc = NULL;
    
    if (proc == NULL) 
	{
        proc = (alBufferDataStaticProcPtr) alcGetProcAddress(NULL, (const ALCchar*) "alBufferDataStatic");
    }
    
    if (proc)
	{
		proc(bid, format, data, size, freq);
	}
	
    return;
}

void* GetOpenALAudioData(CFURLRef inFileURL, ALsizei *outDataSize, ALenum *outDataFormat, ALsizei*	outSampleRate)
{
	OSStatus						err = noErr;	
	SInt64							theFileLengthInFrames = 0;
	AudioStreamBasicDescription		theFileFormat;
	UInt32							thePropertySize = sizeof(theFileFormat);
	ExtAudioFileRef					extRef = NULL;
	void*							theData = NULL;
	AudioStreamBasicDescription		theOutputFormat;

	// Open a file with ExtAudioFileOpen()
	err = ExtAudioFileOpenURL(inFileURL, &extRef);
	if (err) 
	{
		printf("GetOpenALAudioData: ExtAudioFileOpenURL FAILED, Error = %ld\n", err); 
		return NULL; 
	}
	
	// Get the audio data format
	err = ExtAudioFileGetProperty(extRef, kExtAudioFileProperty_FileDataFormat, &thePropertySize, &theFileFormat);
	if (err) 
	{ 
		printf("GetOpenALAudioData: ExtAudioFileGetProperty(kExtAudioFileProperty_FileDataFormat) FAILED, Error = %ld\n", err); 
		if (extRef)
		{
			ExtAudioFileDispose(extRef);
		}
		
		return NULL; 
	}
	
	if (theFileFormat.mChannelsPerFrame > 2)  
	{
		printf("GetOpenALAudioData - Unsupported Format, channel count is greater than stereo\n"); 
		if (extRef)
		{
			ExtAudioFileDispose(extRef);
		}
		
		return NULL;
	}

	// Set the client format to 16 bit signed integer (native-endian) data
	// Maintain the channel count and sample rate of the original source format
	theOutputFormat.mSampleRate = theFileFormat.mSampleRate;
	theOutputFormat.mChannelsPerFrame = theFileFormat.mChannelsPerFrame;

	theOutputFormat.mFormatID = kAudioFormatLinearPCM;
	theOutputFormat.mBytesPerPacket = 2 * theOutputFormat.mChannelsPerFrame;
	theOutputFormat.mFramesPerPacket = 1;
	theOutputFormat.mBytesPerFrame = 2 * theOutputFormat.mChannelsPerFrame;
	theOutputFormat.mBitsPerChannel = 16;
	theOutputFormat.mFormatFlags = kAudioFormatFlagsNativeEndian | kAudioFormatFlagIsPacked | kAudioFormatFlagIsSignedInteger;
	
	// Set the desired client (output) data format
	err = ExtAudioFileSetProperty(extRef, kExtAudioFileProperty_ClientDataFormat, sizeof(theOutputFormat), &theOutputFormat);
	if (err) 
	{
		printf("GetOpenALAudioData: ExtAudioFileSetProperty(kExtAudioFileProperty_ClientDataFormat) FAILED, Error = %ld\n", err);
		if (extRef)
		{
			ExtAudioFileDispose(extRef);
		}
		
		return NULL;
	}
	
	// Get the total frame count
	thePropertySize = sizeof(theFileLengthInFrames);
	err = ExtAudioFileGetProperty(extRef, kExtAudioFileProperty_FileLengthFrames, &thePropertySize, &theFileLengthInFrames);
	if (err) 
	{
		printf("GetOpenALAudioData: ExtAudioFileGetProperty(kExtAudioFileProperty_FileLengthFrames) FAILED, Error = %ld\n", err);
		if (extRef)
		{
			ExtAudioFileDispose(extRef);
		}
		
		return NULL;
	}
	
	// Read all the data into memory
	UInt32		dataSize = theFileLengthInFrames * theOutputFormat.mBytesPerFrame;;
	theData = malloc(dataSize);
	if (theData)
	{
		AudioBufferList		theDataBuffer;
		theDataBuffer.mNumberBuffers = 1;
		theDataBuffer.mBuffers[0].mDataByteSize = dataSize;
		theDataBuffer.mBuffers[0].mNumberChannels = theOutputFormat.mChannelsPerFrame;
		theDataBuffer.mBuffers[0].mData = theData;
		
		// Read the data into an AudioBufferList
		err = ExtAudioFileRead(extRef, (UInt32*)&theFileLengthInFrames, &theDataBuffer);
		if (err == noErr)
		{
			// success
			*outDataSize = (ALsizei)dataSize;
			*outDataFormat = (theOutputFormat.mChannelsPerFrame > 1) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;
			*outSampleRate = (ALsizei)theOutputFormat.mSampleRate;
		}
		else 
		{ 
			// failure
			free(theData);
			printf("GetOpenALAudioData: ExtAudioFileRead FAILED, Error = %ld\n", err);
			
			if (extRef)
			{
				ExtAudioFileDispose(extRef);
			}
			
			return NULL;
		}	
	}
	
	// Dispose the ExtAudioFileRef, it is no longer needed
	if (extRef)
	{
		ExtAudioFileDispose(extRef);
	}
	
	return theData;
}

void* GetOpenALAudioDataWithData(CFURLRef inFileURL,int offset, int length, ALsizei *outDataSize, ALenum *outDataFormat, ALsizei*	outSampleRate)
{
	OSStatus						err = noErr;
	SInt64							theFileLengthInFrames = 0;
	AudioStreamBasicDescription		theFileFormat;
	UInt32							thePropertySize = sizeof(theFileFormat);
	ExtAudioFileRef					extRef = NULL;
	void*							theData = NULL;
	AudioStreamBasicDescription		theOutputFormat;
    
	// Open a file with ExtAudioFileOpen()
	err = ExtAudioFileOpenURL(inFileURL, &extRef);
	if (err)
	{
		printf("GetOpenALAudioData: ExtAudioFileOpenURL FAILED, Error = %ld\n", err);
		return NULL;
	}
	
	// Get the audio data format
	err = ExtAudioFileGetProperty(extRef, kExtAudioFileProperty_FileDataFormat, &thePropertySize, &theFileFormat);
	if (err)
	{
		printf("GetOpenALAudioData: ExtAudioFileGetProperty(kExtAudioFileProperty_FileDataFormat) FAILED, Error = %ld\n", err);
		if (extRef)
		{
			ExtAudioFileDispose(extRef);
		}
		
		return NULL;
	}
	
	if (theFileFormat.mChannelsPerFrame > 2)
	{
		printf("GetOpenALAudioData - Unsupported Format, channel count is greater than stereo\n");
		if (extRef)
		{
			ExtAudioFileDispose(extRef);
		}
		
		return NULL;
	}
    
	// Set the client format to 16 bit signed integer (native-endian) data
	// Maintain the channel count and sample rate of the original source format
	theOutputFormat.mSampleRate = theFileFormat.mSampleRate;
	theOutputFormat.mChannelsPerFrame = theFileFormat.mChannelsPerFrame;
    
	theOutputFormat.mFormatID = kAudioFormatLinearPCM;
	theOutputFormat.mBytesPerPacket = 2 * theOutputFormat.mChannelsPerFrame;
	theOutputFormat.mFramesPerPacket = 1;
	theOutputFormat.mBytesPerFrame = 2 * theOutputFormat.mChannelsPerFrame;
	theOutputFormat.mBitsPerChannel = 16;
	theOutputFormat.mFormatFlags = kAudioFormatFlagsNativeEndian | kAudioFormatFlagIsPacked | kAudioFormatFlagIsSignedInteger;
	
	// Set the desired client (output) data format
	err = ExtAudioFileSetProperty(extRef, kExtAudioFileProperty_ClientDataFormat, sizeof(theOutputFormat), &theOutputFormat);
	if (err)
	{
		printf("GetOpenALAudioData: ExtAudioFileSetProperty(kExtAudioFileProperty_ClientDataFormat) FAILED, Error = %ld\n", err);
		if (extRef)
		{
			ExtAudioFileDispose(extRef);
		}
		
		return NULL;
	}
	
	// Get the total frame count
	thePropertySize = sizeof(theFileLengthInFrames);
	err = ExtAudioFileGetProperty(extRef, kExtAudioFileProperty_FileLengthFrames, &thePropertySize, &theFileLengthInFrames);
	if (err)
	{
		printf("GetOpenALAudioData: ExtAudioFileGetProperty(kExtAudioFileProperty_FileLengthFrames) FAILED, Error = %ld\n", err);
		if (extRef)
		{
			ExtAudioFileDispose(extRef);
		}
		
		return NULL;
	}
	
	// Read all the data into memory
	UInt32		dataSize = theFileLengthInFrames * theOutputFormat.mBytesPerFrame;;
	theData = malloc(dataSize);
	if (theData)
	{
		AudioBufferList		theDataBuffer;
		theDataBuffer.mNumberBuffers = 1;
		theDataBuffer.mBuffers[0].mDataByteSize = dataSize;
		theDataBuffer.mBuffers[0].mNumberChannels = theOutputFormat.mChannelsPerFrame;
		theDataBuffer.mBuffers[0].mData = theData;
		
		// Read the data into an AudioBufferList
		err = ExtAudioFileRead(extRef, (UInt32*)&theFileLengthInFrames, &theDataBuffer);
		if (err == noErr)
		{
			// success
			*outDataSize = (ALsizei)dataSize;
			*outDataFormat = (theOutputFormat.mChannelsPerFrame > 1) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;
			*outSampleRate = (ALsizei)theOutputFormat.mSampleRate;
		}
		else
		{
			// failure
			free(theData);
			printf("GetOpenALAudioData: ExtAudioFileRead FAILED, Error = %ld\n", err);
			
			if (extRef)
			{
				ExtAudioFileDispose(extRef);
			}
			
			return NULL;
		}
	}
	
	// Dispose the ExtAudioFileRef, it is no longer needed
	if (extRef)
	{
		ExtAudioFileDispose(extRef);
	}
	
	return theData;
}
void* MyGetOpenALAudioDataWithBuffer(const char* buff,int buff_len, ALsizei *outDataSize, ALenum *outDataFormat, ALsizei *outSampleRate)
{
    //detected format
    int offset = 0;
    char tmp[5];
    //RIFF
    for(int i = 0; i<4; i++){
        tmp[i] = buff[offset + i];
    }
    tmp[4] = '\0';
    //printf("%s\n",tmp);
    offset+=4;
    
    //ChunkSize
    /*for(int i = 0; i<4; i++){
        tmp[i] = buff[offset + i];
    }
    tmp[4] = '\0';
    int chunksize = *(int*)tmp;
    printf(" chunksize:%d\n", chunksize);*/
    offset+=4;
   
    //WAVE
    /*for(int i = 0; i<4; i++){
        tmp[i] = buff[offset + i];
    }
    printf("%s\n",tmp);*/
    offset+=4;
    
    //////////////////////////////////
    //fmt chunk
    /*for(int i = 0; i<4; i++){
        tmp[i] = buff[offset + i];
    }
    printf("subchunk:%s\n",tmp); //fmt*/
    offset+=4;
    
    //subchunk1size
    for(int i = 0; i<4; i++){
        tmp[i] = buff[offset + i];
    }
    int subchunksize = *(int*)tmp;
    //printf(" subchunksize:%d\n", subchunksize);
    offset+=4;
    int indexBeginTrunkCount = offset;
    
    //audio format
    /*for(int i = 0; i<2; i++){
        tmp[i] = buff[offset + i];
    }
    short audioformat = *(short*) tmp;
    printf(" audio format:%d\n", audioformat);*/
    offset +=2;
    
    //numchannel
    for(int i = 0; i<2; i++){
        tmp[i] = buff[offset + i];
    }
    short numchannel = *(short*) tmp;
    //printf(" num channel:%d\n", numchannel);
    offset +=2;
    
    //sample rate
    for(int i = 0; i<4; i++){
        tmp[i] = buff[offset + i];
    }
    int samplerate = *(int*)tmp;
    //printf(" sample rate:%d\n", samplerate);
    offset+=4;
    
    //byte rate
    /*for(int i = 0; i<4; i++){
        tmp[i] = buff[offset + i];
    }
    int byterate = *(int*)tmp;
    printf(" byte rate:%d\n", byterate);*/
    offset+=4;
    
    //blockalign
    /*for(int i = 0; i<2; i++){
        tmp[i] = buff[offset + i];
    }
    short blockalign = *(short*) tmp;
    printf(" blockalign:%d\n", blockalign);*/
    offset +=2;
    
    
    //bitpersample
    /*for(int i = 0; i<2; i++){
        tmp[i] = buff[offset + i];
    }
    short bitpersample = *(short*) tmp;
    printf(" bitpersample:%d\n", bitpersample);*/
    offset +=2;
    
    offset =indexBeginTrunkCount +subchunksize;

    while (offset < buff_len)
    {
        if (buff[offset] == 0x64 && buff[offset+1] == 0x61 && buff[offset+2] == 0x74 && buff[offset+3] == 0x61)
        {
            break;
        }
        else
        {
            for(int i = 0; i<4; i++)
            {
                tmp[i] = buff[offset + 4 + i];
            }
            subchunksize = *(int*)tmp;
            offset += 8;
            offset += subchunksize;
        }
    }
    
    //data
    /*for(int i = 0; i<4; i++){
        tmp[i] = buff[offset + i];
    }
    printf("%s\n %d ",tmp, offset);*/
    offset+=4;
    
    //data size;
    for(int i = 0; i<4; i++){
        tmp[i] = buff[offset + i];
    }
    int datasize = *(int*)tmp;
    //printf(" subchunk2size:%d\n", datasize);
    offset+=4;
    
    void*							theData = NULL;
	AudioStreamBasicDescription		theOutputFormat;
    
    if (numchannel > 2)
	{
		printf("GetOpenALAudioData - Unsupported Format, channel count is greater than stereo\n");
		return NULL;
	}
    
	// Set the client format to 16 bit signed integer (native-endian) data
	// Maintain the channel count and sample rate of the original source format
	theOutputFormat.mSampleRate = samplerate;
	theOutputFormat.mChannelsPerFrame = numchannel;
    
	theOutputFormat.mFormatID = kAudioFormatLinearPCM;
	theOutputFormat.mBytesPerPacket = 2 * numchannel;
	theOutputFormat.mFramesPerPacket = 1;
	theOutputFormat.mBytesPerFrame = 2 * numchannel;
	theOutputFormat.mBitsPerChannel = 16;
	theOutputFormat.mFormatFlags = kAudioFormatFlagsNativeEndian | kAudioFormatFlagIsPacked | kAudioFormatFlagIsSignedInteger;
	
	// Set the desired client (output) data format

	// Read all the data into memory
	theData = malloc(datasize);
	if (theData)
	{
        char* tmp_data = (char*)theData;
        for(int i = 0; i< datasize; i++){
            *tmp_data++ = buff[offset + i];
        }
		AudioBufferList		theDataBuffer;
		theDataBuffer.mNumberBuffers = 1;
		theDataBuffer.mBuffers[0].mDataByteSize = datasize;
		theDataBuffer.mBuffers[0].mNumberChannels = theOutputFormat.mChannelsPerFrame;
		theDataBuffer.mBuffers[0].mData = theData;
		
		// Read the data into an AudioBufferList

			// success
			*outDataSize = (ALsizei)datasize;
			*outDataFormat = (theOutputFormat.mChannelsPerFrame > 1) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;
			*outSampleRate = (ALsizei)theOutputFormat.mSampleRate;
		
	}
	
		
	return theData;
}
