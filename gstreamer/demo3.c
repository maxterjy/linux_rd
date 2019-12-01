#include <gst/gst.h>
#include <stdio.h>
#include <unistd.h>

//target: pad-added handler

typedef struct {
    GstElement *pipeline;
    GstElement *source;
    GstElement *convert;
    GstElement *sink;
} CustomData;

void pad_added_handler(GstElement* src, GstPad* pad, CustomData *data);

int main(int argc, char *argv[]) {
    CustomData data;
    GstBus *bus;
    GstMessage *msg;
    GstStateChangeReturn ret;
    gboolean terminate = FALSE;

    gst_init(&argc, &argv);

    data.source = gst_element_factory_make("uridecodebin", "mysource");
    data.convert = gst_element_factory_make("audioconvert", "myconvert");
    data.sink = gst_element_factory_make("autoaudiosink", "mysink");

    data.pipeline = gst_pipeline_new("test-pipeline");

    gst_bin_add_many(GST_BIN(data.pipeline), data.source, data.convert, data.sink, NULL);
    gst_element_link(data.convert, data.sink);
    

    g_object_set(data.source, "uri", "https://file-examples.com/wp-content/uploads/2017/11/file_example_OOG_1MG.ogg", NULL);

    g_signal_connect(data.source, "pad-added", G_CALLBACK(pad_added_handler), &data);

    gst_element_set_state(data.pipeline, GST_STATE_PLAYING);

    bus = gst_element_get_bus(data.pipeline);

    do {
        msg = gst_bus_timed_pop(bus, GST_CLOCK_TIME_NONE);

        switch (msg->type)
        {
        case GST_MESSAGE_ERROR:
            
            break;
        
        case GST_MESSAGE_EOS:
            printf("end of sound\n");
            terminate = TRUE;
            break;

        case GST_MESSAGE_STATE_CHANGED:

            //only care message from pipeline
            if (msg->src == data.pipeline) {
                GstState old_state, new_state;
                gst_message_parse_state_changed(msg, &old_state, &new_state, NULL);
                printf("state change: %s --> %s\n", 
                    gst_element_state_get_name(old_state),
                    gst_element_state_get_name(new_state));
            }

            break;
        
        default:
            break;
        }

        gst_message_unref(msg);

    }
    while (!terminate);
    

    gst_object_unref(bus);
    gst_element_set_state(data.pipeline, GST_STATE_NULL);
    gst_object_unref(data.pipeline);

    return 0;
}

//pad-added called when source element has enough information to start
void pad_added_handler(GstElement* src, GstPad* src_pad, CustomData *data) {
    printf("pad_added_handler\n");

    GstPad *sink_pad = gst_element_get_static_pad(data->convert, "sink");
    gst_pad_link(src_pad, sink_pad);

    gst_object_unref(sink_pad);
}