#include <gst/gst.h>
#include <stdio.h>

//target: query time

int main(int argc, char *argv[]) {
    GstElement *pipeline;
    GstBus *bus;
    GstMessage *msg;

    gst_init(&argc, &argv);

    pipeline = gst_parse_launch("playbin uri=https://file-examples.com/wp-content/uploads/2017/11/file_example_OOG_1MG.ogg", NULL);

    gst_element_set_state(pipeline, GST_STATE_PLAYING);

    bus = gst_element_get_bus(pipeline);
    
    char is_playing = 0;
    gint64 duration = -1;
    gint64 position = -1;

    do {
        msg = gst_bus_timed_pop_filtered(bus, 2 * GST_SECOND, 
            GST_MESSAGE_EOS | GST_MESSAGE_STATE_CHANGED);

        if (msg == NULL) {
            if (is_playing) {                
                gst_element_query_position(pipeline, GST_FORMAT_TIME, &position);

                g_print("current: %" GST_TIME_FORMAT "\n", GST_TIME_ARGS(position));  
            }
        }
        else {
            if (msg->src == pipeline) {
                if (msg->type == GST_MESSAGE_EOS) {
                    printf("End of Sound\n");
                    break;
                }
                else if (msg->type == GST_MESSAGE_STATE_CHANGED) {
                    
                    GstState new_state;
                    gst_message_parse_state_changed(msg, NULL, &new_state, NULL);
                    printf("new state %s\n", gst_element_state_get_name(new_state));

                    if (new_state == GST_STATE_PLAYING) {
                        printf("aaa\n");
                        is_playing = 1;
                        gst_element_query_duration(pipeline, GST_FORMAT_TIME, &duration);

                        g_print("duration: %" GST_TIME_FORMAT "\n", GST_TIME_ARGS(duration));  
                    }

                }
            }
        }
    }
    while (1);

    gst_object_unref(bus);
    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);

    return 0;
}