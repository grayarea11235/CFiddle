#include <stdio.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>

static void logging(const char *fmt, ...)
{
    va_list args;
    fprintf( stderr, "LOG: " );
    va_start( args, fmt );
    vfprintf( stderr, fmt, args );
    va_end( args );
    fprintf( stderr, "\n" );
}

void error(int code)
{
  logging("ERROR : %s", av_err2str(code));
}

// https://github.com/leandromoreira/ffmpeg-libav-tutorial/blob/master/0_hello_world.c
int main(int argc, char *argv[]) {
  av_register_all();
  avcodec_register_all();

  AVFormatContext *pFormatContext = avformat_alloc_context();
  char *test_file ="/home/cpd/Devel/CFiddle/ffmpeg/sample-6s.mp3";
  
  if (!pFormatContext) {
    logging("ERROR could not allocate memory for Format Context");
    
    return -1;
  }

  int r = avformat_open_input(&pFormatContext, "sample-6s.mp3", NULL, NULL);
  if (r != 0) {
    logging("ERROR could not open the file. Code is %ud (%s)\n", r, av_err2str(r));
    return -1;
  }

  printf("pFormatContext = %p\n", pFormatContext);
  printf("pFormatContext = %p\n", pFormatContext->iformat);


  logging("format %s, duration %lld us, bit_rate %lld", pFormatContext->iformat->name, 
      pFormatContext->duration, pFormatContext->bit_rate);

  logging("finding stream info from format"); 

  r = avformat_find_stream_info(pFormatContext,  NULL);
  if (r < 0) {
    logging("ERROR could not get the stream info");

    return -1;
  }
 
  AVCodec *pCodec = NULL;
  AVCodecParameters *pCodecParameters =  NULL;
  int video_stream_index = -1;

  logging("Stream count %d\n", pFormatContext->nb_streams);
  // loop though all the streams and print its main information
  for (int i = 0; i < pFormatContext->nb_streams; i++)
  {
    printf("i = %d\n", i);

    AVCodecParameters *pLocalCodecParameters =  NULL;
    pLocalCodecParameters = pFormatContext->streams[i]->codecpar;
    logging("AVStream->time_base before open coded %d/%d", pFormatContext->streams[i]->time_base.num, pFormatContext->streams[i]->time_base.den);
    logging("AVStream->r_frame_rate before open coded %d/%d", pFormatContext->streams[i]->r_frame_rate.num, pFormatContext->streams[i]->r_frame_rate.den);
    logging("AVStream->start_time %" PRId64, pFormatContext->streams[i]->start_time);
    logging("AVStream->duration %" PRId64, pFormatContext->streams[i]->duration);

    logging("finding the proper decoder (CODEC)");

    AVCodec *pLocalCodec = NULL;

    // finds the registered decoder for a codec ID
    // https://ffmpeg.org/doxygen/trunk/group__lavc__decoding.html#ga19a0ca553277f019dd5b0fec6e1f9dca
    pLocalCodec = avcodec_find_decoder(pLocalCodecParameters->codec_id);

    if (pLocalCodec==NULL) {
      logging("ERROR unsupported codec!");
      // In this example if the codec is not found we just skip it
      continue;
    }
 
    // print its name, id and bitrate
    logging("\tCodec %s ID %d bit_rate %lld", pLocalCodec->name, pLocalCodec->id, 
        pLocalCodecParameters->bit_rate);
  }

  // https://ffmpeg.org/doxygen/trunk/structAVCodecContext.html
  AVCodecContext *pCodecContext = avcodec_alloc_context3(pCodec);
  if (!pCodecContext)
  {
    logging("failed to allocated memory for AVCodecContext");
    return -1;
  }

  return 0;
}
