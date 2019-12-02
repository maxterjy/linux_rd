#include <gst/gst.h>

//target: using tee, queue

int main(int argc, char *argv[]) {
    GstElement *pipeline;
    GstElement *audio_source, *tee;
    GstElement *audio_queue, *audio_convert, *audio_resample, *audio_sink;
    GstElement *video_queue, *video_visual, *video_convert, *video_sink;
    GstBus *bus;
    GstMessage *msg;
    GstPad *tee_audio_pad, *tee_video_pad;
    GstPad *queue_audio_pad, *queue_video_pad;

    gst_init(&argc, &argv);

    audio_source = gst_element_factory_make("audiotestsrc", "audio_source");
    tee = gst_element_factory_make("tee", "tee");
    
    audio_queue = gst_element_factory_make("queue", "audio_queue");
    audio_convert = gst_element_factory_make("audioconvert", "audio_convert");
    audio_resample = gst_element_factory_make("audioresample", "audio_resample");
    audio_sink = gst_element_factory_make("autoaudiosink", "audio_sink");
            
    video_queue = gst_element_factory_make("queue", "video_queue");
    video_visual = gst_element_factory_make("wavescope", "video_visual");
    video_convert = gst_element_factory_make("videoconvert", "video_convert");
    video_sink = gst_element_factory_make("autovideosink", "video_sink");

    pipeline = gst_pipeline_new("test-pipeline");

    g_object_set(audio_source, "freq", 215.0f, NULL);
    g_object_set(video_visual, "shader", 0, "style", 1, NULL);

    gst_bin_add_many(GST_BIN(pipeline), audio_source, tee,
        audio_queue, audio_convert, audio_resample, audio_sink,
        video_queue, video_visual, video_convert, video_sink,
        NULL);

    gst_element_link_many(audio_source, tee, NULL);

    gst_element_link_many(audio_queue, audio_convert, audio_resample, audio_sink, NULL);
    gst_element_link_many(video_queue, video_visual, video_convert, video_sink, NULL);

    tee_audio_pad = gst_element_get_request_pad(tee, "src_%u");
    tee_video_pad = gst_element_get_request_pad(tee, "src_%u");

    queue_audio_pad = gst_element_get_static_pad(audio_queue, "sink");
    queue_video_pad = gst_element_get_static_pad(video_queue, "sink");

    gst_pad_link(tee_audio_pad, queue_audio_pad);
    gst_pad_link(tee_video_pad, queue_video_pad);
    
    gst_element_set_state(pipeline, GST_STATE_PLAYING);

    bus = gst_element_get_bus(pipeline);

    msg = gst_bus_timed_pop_filtered(bus, GST_CLOCK_TIME_NONE,
        GST_MESSAGE_ERROR | GST_MESSAGE_EOS);

    if (msg != NULL) {
        if (msg->type == GST_MESSAGE_ERROR) {
            GError *err;
            gchar *debug_info;

            gst_message_parse_error(msg, &err, &debug_info);
            g_print("%s, %s\n", err->message, debug_info);
        }
    }

    return 0;
}