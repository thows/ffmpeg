#include <jni.h>
#include <string>



extern "C" {
#include <libavutil/time.h>
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libavutil/imgutils.h"
#include "libavutil/avutil.h"
};
using namespace std;

extern "C" {
jstring
Java_com_ndk_1ffmpeg_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {

    string hello = "Hello from C++aaa";
    return env->NewStringUTF(hello.c_str());
}


JNIEXPORT void JNICALL
Java_com_ndk_1ffmpeg_MainActivity_push(JNIEnv *env, jobject instance, jstring in_, jstring out_) {


    av_register_all();
//    const char *in_filename = env->GetStringUTFChars(in_, 0);
//    const char *out_filename = env->GetStringUTFChars(out_, 0);
//
//    AVOutputFormat *ofmt = NULL;
//    //输入对应一个AVFormatContext，输出对应一个AVFormatContext
//    //（Input AVFormatContext and Output AVFormatContext）
//    AVFormatContext *ifmt_ctx = NULL, *ofmt_ctx = NULL;
//    AVPacket pkt;
//    int ret, i;
//    int videoindex = -1;
//    int frame_index = 0;
//    int64_t start_time = 0;
//
//    // in_filename  = "cuc_ieschool.flv";//输入URL（Input file URL）
//    //in_filename  = "shanghai03_p.h264";
//
//    //  out_filename = "rtmp://localhost/publishlive/livestream";//输出 URL（Output URL）[RTMP]
//    //out_filename = "rtp://233.233.233.233:6666";//输出 URL（Output URL）[UDP]
//
//    av_register_all();
//    //Network
//    avformat_network_init();
//    //输入（Input）
//    if ((ret = avformat_open_input(&ifmt_ctx, in_filename, 0, 0)) < 0) {
//        printf("Could not open input file.");
//        goto end;
//    }
//    if ((ret = avformat_find_stream_info(ifmt_ctx, 0)) < 0) {
//        printf("Failed to retrieve input stream information");
//        goto end;
//    }
//
//    for (i = 0; i < ifmt_ctx->nb_streams; i++)
//        if (ifmt_ctx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
//            videoindex = i;
//            break;
//        }
//
//    av_dump_format(ifmt_ctx, 0, in_filename, 0);
//
//    //输出（Output）
//
//    avformat_alloc_output_context2(&ofmt_ctx, NULL, "flv", out_filename); //RTMP
//    //avformat_alloc_output_context2(&ofmt_ctx, NULL, "mpegts", out_filename);//UDP
//
//    if (!ofmt_ctx) {
//        printf("Could not create output context\n");
//        ret = AVERROR_UNKNOWN;
//        goto end;
//    }
//    ofmt = ofmt_ctx->oformat;
//    for (i = 0; i < ifmt_ctx->nb_streams; i++) {
//        //根据输入流创建输出流（Create output AVStream according to input AVStream）
//        AVStream *in_stream = ifmt_ctx->streams[i];
//        AVStream *out_stream = avformat_new_stream(ofmt_ctx, in_stream->codec->codec);
//        if (!out_stream) {
//            printf("Failed allocating output stream\n");
//            ret = AVERROR_UNKNOWN;
//            goto end;
//        }
//        //复制AVCodecContext的设置（Copy the settings of AVCodecContext）
//        ret = avcodec_copy_context(out_stream->codec, in_stream->codec);
//        if (ret < 0) {
//            printf("Failed to copy context from input to output stream codec context\n");
//            goto end;
//        }
//        out_stream->codec->codec_tag = 0;
//        if (ofmt_ctx->oformat->flags & AVFMT_GLOBALHEADER)
//            out_stream->codec->flags |= CODEC_FLAG_GLOBAL_HEADER;
//    }
//    //Dump Format------------------
//    av_dump_format(ofmt_ctx, 0, out_filename, 1);
//    //打开输出URL（Open output URL）
//    if (!(ofmt->flags & AVFMT_NOFILE)) {
//        ret = avio_open(&ofmt_ctx->pb, out_filename, AVIO_FLAG_WRITE);
//        if (ret < 0) {
//            printf("Could not open output URL '%s'", out_filename);
//            goto end;
//        }
//    }
//    //写文件头（Write file header）
//    ret = avformat_write_header(ofmt_ctx, NULL);
//    if (ret < 0) {
//        printf("Error occurred when opening output URL\n");
//        goto end;
//    }
//
//    start_time = av_gettime();
//    while (1) {
//        AVStream *in_stream, *out_stream;
//        //获取一个AVPacket（Get an AVPacket）
//        ret = av_read_frame(ifmt_ctx, &pkt);
//        if (ret < 0)
//            break;
//
//        if (pkt.pts == AV_NOPTS_VALUE) {
//            //Write PTS
//            AVRational time_base1 = ifmt_ctx->streams[videoindex]->time_base;
//            //Duration between 2 frames (us)
//            int64_t calc_duration =
//                    (double) AV_TIME_BASE / av_q2d(ifmt_ctx->streams[videoindex]->r_frame_rate);
//            //Parameters
//            pkt.pts = (double) (frame_index * calc_duration) /
//                      (double) (av_q2d(time_base1) * AV_TIME_BASE);
//            pkt.dts = pkt.pts;
//            pkt.duration = (double) calc_duration / (double) (av_q2d(time_base1) * AV_TIME_BASE);
//        }
//        //Important:Delay
//        if (pkt.stream_index == videoindex) {
//            AVRational time_base = ifmt_ctx->streams[videoindex]->time_base;
//            AVRational time_base_q = {1, AV_TIME_BASE};
//            int64_t pts_time = av_rescale_q(pkt.dts, time_base, time_base_q);
//            int64_t now_time = av_gettime() - start_time;
//            if (pts_time > now_time)
//                av_usleep(pts_time - now_time);
//
//        }
//
//        in_stream = ifmt_ctx->streams[pkt.stream_index];
//        out_stream = ofmt_ctx->streams[pkt.stream_index];
//        /* copy packet */
//        //转换PTS/DTS（Convert PTS/DTS）
//        pkt.pts = av_rescale_q_rnd(pkt.pts, in_stream->time_base, out_stream->time_base,
//                                   (AVRounding) (AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
//        pkt.dts = av_rescale_q_rnd(pkt.dts, in_stream->time_base, out_stream->time_base,
//                                   (AVRounding) (AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
//        pkt.duration = av_rescale_q(pkt.duration, in_stream->time_base, out_stream->time_base);
//        pkt.pos = -1;
//        //Print to Screen
//        if (pkt.stream_index == videoindex) {
//            printf("Send %8d video frames to output URL\n", frame_index);
//            frame_index++;
//        }
//        //ret = av_write_frame(ofmt_ctx, &pkt);
//        ret = av_interleaved_write_frame(ofmt_ctx, &pkt);
//
//        if (ret < 0) {
//            printf("Error muxing packet\n");
//            break;
//        }
//
//        av_free_packet(&pkt);
//
//    }
//    //写文件尾（Write file trailer）
//    av_write_trailer(ofmt_ctx);
//
//    end:
//
//    env->ReleaseStringUTFChars(in_, in_filename);
//    env->ReleaseStringUTFChars(out_, out_filename);
}
}