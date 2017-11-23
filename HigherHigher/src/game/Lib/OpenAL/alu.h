#ifndef __alu_h_
#define __alu_h_

#define ALUAPI
#define ALUAPIENTRY __cdecl

#define OUTPUTCHANNELS 2
#define MIXBUFFERSIZE 48000

#include "altypes.h"

#ifdef __cplusplus
extern "C" {
#endif

ALUAPI ALint	ALUAPIENTRY aluF2L(ALfloat value);
ALUAPI ALshort	ALUAPIENTRY aluF2S(ALfloat value);
ALUAPI ALvoid	ALUAPIENTRY aluCrossproduct(ALfloat *inVector1,ALfloat *inVector2,ALfloat *outVector);
ALUAPI ALfloat	ALUAPIENTRY aluDotproduct(ALfloat *inVector1,ALfloat *inVector2);
ALUAPI ALvoid	ALUAPIENTRY aluNormalize(ALfloat *inVector);
ALUAPI ALvoid	ALUAPIENTRY aluMatrixVector(ALfloat matrix[3][3],ALfloat *vector);
ALUAPI ALvoid	ALUAPIENTRY aluCalculateSourceParameters(ALuint source,ALuint channels,ALfloat *pitch,ALfloat *volume);
ALUAPI ALvoid	ALUAPIENTRY aluMixData(ALuint context,ALvoid *buffer,ALsizei size,ALenum format);

#ifdef __cplusplus
};
#endif

#endif

