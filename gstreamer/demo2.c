#include <gst/gst.h>

//target: pad source-sink

int main(int argc, char *argv[]) {
    GstElement *pipeline, *source, *sink;
    GstBus *bus;
    GstMessage *msg;
    GstStateChangeReturn *ret;

    gst_init(&argc, &argv);

    source = gst_element_factory_make("videotestsrc", "mysource");
    sink = gst_element_factory_make("autovideosink", "mysink");

    pipeline = gst_pipeline_new(NULL);

    gst_bin_add_many(GST_BIN(pipeline), source, sink, NULL);
    gst_element_link(source, sink);

    g_object_set(source, "pattern", 0, NULL);

    gst_element_set_state(pipeline, GST_STATE_PLAYING);

    bus = gst_element_get_bus(pipeline);

    msg = gst_bus_timed_pop_filtered(bus, GST_CLOCK_TIME_NONE,
        GST_MESSAGE_ERROR | GST_MESSAGE_EOS);

    if (msg != NULL) {
        if (GST_MESSAGE_TYPE(msg) == GST_MESSAGE_ERROR) {
            GError *err;
            gchar *debug_info;

            gst_message_parse_error(msg, &err, &debug_info);

            g_printerr("%s %s\n", GST_OBJECT_NAME(msg->src), err->message);
            g_printerr("%s\n", debug_info);

            g_clear_error(&err);
            g_free(debug_info);
        }
    }
}