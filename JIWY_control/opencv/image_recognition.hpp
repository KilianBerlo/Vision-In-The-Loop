#ifndef JIWY_CONTROL_IMAGE_RECOGNITION_HPP
#define JIWY_CONTROL_IMAGE_RECOGNITION_HPP

#include <gst/gst.h>	//Main header for GStreamer
#include "opencv2/imgproc.hpp"	//Used for several funcions of opencv for image processing
#include "opencv2/highgui.hpp"	//For saving the images with imwrite for debugging
#include <fstream>	//For writing x and y values to a file for debugging
#include <functional>

using namespace cv;

#include "../motor/motor.hpp"

class ImageRecognition
{

public:

    ImageRecognition();

    static void initialize(std::function<void (float, float)> func);

    static void run ();

private:

    typedef struct _AllElements
    {
        GstElement *pipeline, *source, *filter, *decoder, *conv, *sink;
    } AllElements;

    static gboolean bus_call (GstBus *bus, GstMessage *msg, gpointer data);
    static GstFlowReturn new_sample (GstElement *sink, AllElements *element);

    static int res_w;	//Resolution width
    static int res_h;	//Resolution height
    static int hue_low;	//Lower range of hue
    static int hue_high;	//Upper range of hue
    static int sat_low;	//Lower range of saturation
    static int sat_high;	//Upper range of saturation
    static int val_low;	//Lower range of value
    static int val_high;	//Upper range of value
    static float distX;
    static float distY;
    static int horizontal_last;	//Horizontal position
    static int vertical_last;	//Vertical position

    static AllElements element;
    static GMainLoop *loop;
    static guint bus_watch_id;

    static std::function<void (float, float)> callback;
};


#endif //JIWY_CONTROL_IMAGE_RECOGNITION_HPP
