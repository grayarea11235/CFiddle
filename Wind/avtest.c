#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>


int main(void)
{
  char *filename = "C:\\aaa\\HitOrMyth.mp3";
  
  AVFormatContext * fCtx = NULL;
  if(avformat_open_input(&fCtx, filename, NULL, NULL) < 0)
  {
    exit(1);
  }
  
  avformat_find_stream_info(fCtx, NULL);
  AVCodec * codec = NULL;
  int strm = av_find_best_stream(fCtx, AVMEDIA_TYPE_AUDIO, -1, -1, &codec, 0);
  AVCodecContext codecCtx = tx->streams[strm]->codec;

  if (avcodec_open2(codecCtx, codec, NULL) < 0) 
  {
    exit(1);
  }
  
  for (;;)
  {
    int err = 0;
    AVPacket pkt;
    err = av_read_frame(fCtx , &pkt);
    if (AVERROR_EOF == err)
    {
      break;
    }
    
    if (pkt.stream_index != strm)
    {
      continue;   
    }
    
    AVFrame frame;
    int gotFrame = 0;
    int len = avcodec_decode_audio4(codecCtx, &frame, &gotFrame, &pkt);
      // or avcodec_decode_audio3 in previous version of ffmpeg, as it is written you.
      /// check result and use frame (or buffer in case of avcodec_decode_audio3)
  }
}
