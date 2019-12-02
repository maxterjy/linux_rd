#include <gst/gst.h>
#include <stdio.h>

//target: query pad

void print_pad_templates_information(GstElementFactory *factory);
void print_pad_capability(GstElement *element, gchar *pad_name);
gboolean print_field(GQuark field_id, const GValue *value, gpointer userdata);


int main(int argc, char *argv[]){
    GstElement *pipeline, *source, *sink;
    GstElementFactory *source_factory, *sink_factory;
    GstBus *bus;
    GstMessage *msg;
    GstStateChangeReturn ret;
    gboolean terminate = FALSE;

    gst_init(&argc, &argv);

    source_factory = gst_element_factory_find("audiotestsrc");
    sink_factory = gst_element_factory_find("autoaudiosink");

    print_pad_templates_information(source_factory);
    print_pad_templates_information(sink_factory);

    source = gst_element_factory_create(source_factory, "source");
    sink = gst_element_factory_create(sink_factory, "sink");

    pipeline = gst_pipeline_new("test-pipeline");

    gst_bin_add_many(GST_BIN(pipeline), source, sink, NULL);
    gst_element_link(source, sink);

    print_pad_capability(sink, "sink");
    print_pad_capability(source, "source");

    gst_element_set_state(pipeline, GST_STATE_PLAYING);

    bus = gst_element_get_bus(pipeline);

    
    do {
        msg = gst_bus_timed_pop_filtered(bus, GST_CLOCK_TIME_NONE, 
            GST_MESSAGE_ERROR | GST_MESSAGE_EOS | GST_MESSAGE_STATE_CHANGED);

        if (msg != NULL) {
            switch (msg->type)
            {
            case GST_MESSAGE_ERROR:
                terminate = TRUE;
                break;
            
            case GST_MESSAGE_EOS:
                terminate = TRUE;
                 break;
            
            case GST_MESSAGE_STATE_CHANGED:
                if (msg->src == pipeline) {
                    // print_pad_capability(sink, "sink");          
                    // print_pad_capability(source, "source");    
                    print_pad_capability(sink, "sink");
                    print_pad_capability(source, "source");      
                }
                break;
            

            default:
                break;
            }
           
        }
    }
    while (!terminate);

    return 0;
}

void print_pad_templates_information(GstElementFactory *factory) {   
    g_print("print_pad_templates_information\n");
    const GList *pads = gst_element_factory_get_static_pad_templates(factory);

    GstStaticPadTemplate *pad_template;

    while (pads) {
        pad_template = pads->data;
        g_print("------------------------\n");
        g_print("%s\n", pad_template->name_template);
        g_print("%d\n", pad_template->direction);
        g_print("%d\n", pad_template->presence);
        g_print("%s\n", pad_template->static_caps.string);
        g_print("------------------------\n");

        pads = pads->next;
    }
}

void print_pad_capability(GstElement *element, gchar *pad_name) {
    g_print("print_pad_capability\n");

    GstPad *pad = gst_element_get_static_pad(element, pad_name);
    
    if (!pad) return;

    GstCaps *caps =  gst_pad_get_current_caps(pad);

    if (caps) {
        g_print("pad_name: %s\n", pad_name);

        if (gst_caps_is_any(caps)) {
            printf("is any\n");
            return;
        }

        if (gst_caps_is_empty(caps)) {
            printf("is empty\n");
            return;
        }

        int n = gst_caps_get_size(caps);
        for(int i = 0; i < n; i++) {
            GstStructure *structure = gst_caps_get_structure(caps, i);
                        
            g_print("------------------------\n");
            g_print("structure name: %s\n", gst_structure_get_name(structure));
            gst_structure_foreach(structure, print_field, NULL);
            g_print("------------------------\n");
        }
    }
}

gboolean print_field(GQuark field_id, const GValue *value, gpointer userdata) {
    g_print("print_field\n");
    gchar *str = gst_value_serialize(value);
    g_print("%s: %s\n", g_quark_to_string(field_id), str);
    return TRUE;
}