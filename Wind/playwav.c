#include <stdio.h>
#include <stdlib.h>
#include <portaudio.h>

#include "wav.h"


typedef struct
{
  float left_phase;
  float right_phase;
} paTestData;


/* This routine will be called by the PortAudio engine when audio is needed.
   It may called at interrupt level on some machines so don't do anything
   that could mess up the system like calling malloc() or free().
*/ 
static int patestCallback( const void *inputBuffer,
                           void *outputBuffer,
                           unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo* timeInfo,
                           PaStreamCallbackFlags statusFlags,
                           void *userData)
{
  /* Cast data passed through stream to our structure. */
  paTestData *data = (paTestData*)userData; 
  float *out = (float*)outputBuffer;
  unsigned int i;
  (void) inputBuffer; /* Prevent unused variable warning. */
  
  for( i=0; i<framesPerBuffer; i++ )
  {
    *out++ = data->left_phase;  /* left */
    *out++ = data->right_phase;  /* right */

    // Generate simple sawtooth phaser that ranges between -1.0 and 1.0.
    data->left_phase += 0.01f;
    
    // When signal reaches top, drop back down.
    if (data->left_phase >= 1.0f)
    {
      data->left_phase -= 2.0f;
    }
    
    // higher pitch so we can distinguish left and right.
    data->right_phase += 0.03f;

    if(data->right_phase >= 1.0f)
    {
      data->right_phase -= 2.0f;
    }
  }

  return 0;
}

int load_wav(const char *filename)
{
  wav_header wav_head;

  FILE *f = fopen(filename, "rb");
  if (f)
  {
    fread(&wav_head, sizeof(wav_header), 1, f);
  }

  fclose(f);

  printf("%c%c%c%c\n", wav_head.riff_header[0], wav_head.riff_header[1],
         wav_head.riff_header[2], wav_head.riff_header[3]);
  printf("data_bytes = %d\n", wav_head.data_bytes);

  wav_head.bytes = malloc(sizeof(uint8_t) * wav_head.data_bytes);
  fread(wav_head.bytes, sizeof(uint8_t), wav_head.data_bytes, f);
  
  
  return wav_head.data_bytes;
}

#define 	SAMPLE_RATE   (44100)

void play_wav(wav_header wav_head)
{
  PaStream *stream;
  PaError err;
  char data[256];
  
  err = Pa_Initialize();
  if(err != paNoError)
  {
    printf("PortAudio error: %s\n", Pa_GetErrorText(err));
    exit(0);
  }

  /* Open an audio I/O stream. */
  err = Pa_OpenDefaultStream(&stream,
                             0,          /* no input channels */
                             2,          /* stereo output */
                             paFloat32,  /* 32 bit floating point output */
                             SAMPLE_RATE,
                             256,        /* frames per buffer, i.e. the number
                                            of sample frames that PortAudio will
                                            request from the callback. Many apps
                                            may want to use
                                            paFramesPerBufferUnspecified, which
                                            tells PortAudio to pick the best,
                                            possibly changing, buffer size.*/
                             patestCallback, /* this is your callback function */
                             &data); /* This is a pointer that will be passed to
                                        your callback */
  if (err != paNoError)
  {
    printf("PortAudio error: %s\n", Pa_GetErrorText(err));
    exit(0);
  }

  err = Pa_Terminate();
  if (err != paNoError)
  {
    printf("PortAudio error: %s\n", Pa_GetErrorText(err));
    exit(0);
  }
  
}

int main(void)
{
  wav_header wav_head;
  
  load_wav("C:\\aaa\\miss.wav");

  
  return 0;
}
