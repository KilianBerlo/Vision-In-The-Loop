#include "image_recognition.hpp"

int ImageRecognition::res_w = 320;	//Resolution width
int ImageRecognition::res_h = 240;	//Resolution height
int ImageRecognition::hue_low= 4;	//Lower range of hue
int ImageRecognition::hue_high = 84;	//Upper range of hue
int ImageRecognition::sat_low =136;	//Lower range of saturation
int ImageRecognition::sat_high = 255;	//Upper range of saturation
int ImageRecognition::val_low = 150;	//Lower range of value
int ImageRecognition::val_high = 255;	//Upper range of value
float ImageRecognition::distX = 0.0;
float ImageRecognition::distY = 0.0;
int ImageRecognition::horizontal_last = -1;	//Horizontal position
int ImageRecognition::vertical_last = -1;	//Vertical position
ImageRecognition::AllElements ImageRecognition::element;
GMainLoop *ImageRecognition::loop;
guint ImageRecognition::bus_watch_id;
std::function<void (float, float )> ImageRecognition::callback;

ImageRecognition::ImageRecognition()
{ }

void ImageRecognition::initialize(std::function<void (float, float)> func)
{
    callback = func;

    GstCaps *caps;
    GstBus *bus;

    /* Initialisation */
    GError error;
    GError *ptr = &error;
    gboolean init = gst_init_check (NULL, NULL, &ptr);

    loop = g_main_loop_new (NULL, FALSE);

    /* Create gstreamer elements */
    element.pipeline	=	gst_pipeline_new ("video-player");
    element.source	=	gst_element_factory_make ("v4l2src",       "cam-source");
    element.filter	=	gst_element_factory_make ("capsfilter", "video-filter");
    element.decoder	=	gst_element_factory_make ("jpegdec",     "jpeg-decoder");
    element.sink	=	gst_element_factory_make ("appsink", "appsink");

    if (!element.pipeline || !element.source || !element.filter || !element.decoder || !element.sink) {
        g_printerr ("One element could not be created. Exiting.\n");
    }

    /* Set up the pipeline */

    /* we set the input filename to the source element */
    g_object_set (G_OBJECT (element.source), "device", "/dev/video0", NULL);
    caps = gst_caps_new_simple("image/jpeg", "framerate", GST_TYPE_FRACTION, 30, 1, "width", G_TYPE_INT, res_w, "height", G_TYPE_INT, res_h, NULL);
    g_object_set(G_OBJECT(element.filter), "caps", caps, NULL);

    /* we add a message handler */
    bus = gst_pipeline_get_bus (GST_PIPELINE (element.pipeline));
    bus_watch_id = gst_bus_add_watch (bus, bus_call, loop);
    gst_object_unref (bus);

    /* we add all elements into the pipeline */
    gst_bin_add_many (GST_BIN (element.pipeline), element.source, element.filter, element.decoder, element.sink, NULL);

    /* we link the elements together */
    gst_element_link_many (element.source, element.filter, element.decoder, element.sink, NULL);

    g_object_set (element.sink, "emit-signals", TRUE, NULL);
    g_signal_connect (element.sink, "new-sample", G_CALLBACK (new_sample), &element);
}

gboolean ImageRecognition::bus_call (GstBus *bus, GstMessage *msg, gpointer data)
{
    GMainLoop *loop = (GMainLoop *) data;

    switch (GST_MESSAGE_TYPE (msg))
    {
        case GST_MESSAGE_EOS:
        {
            g_print ("End of stream\n");
            g_main_loop_quit (loop);
            break;
        }

        case GST_MESSAGE_ERROR:
        {
            gchar  *debug;
            GError *error;

            gst_message_parse_error (msg, &error, &debug);
            g_free (debug);

            g_printerr ("Error: %s\n", error->message);
            g_error_free (error);

            g_main_loop_quit (loop);
            break;
        }

        default:
            break;
    }

    return TRUE;
}

GstFlowReturn ImageRecognition::new_sample (GstElement *sink, AllElements *element)
{
    GstSample *sample = NULL;
    GstBuffer *buffer;
    GstMemory *mem;
    GstMapInfo map;
    Mat frame;
    mem = gst_allocator_alloc(NULL, 1000000, NULL);
    gst_memory_map(mem, &map, GST_MAP_WRITE);

    /* Retrieve the buffer */
    g_signal_emit_by_name (element->sink, "pull-sample", &sample, NULL);
    //g_print("Check frame \n");
    if (sample)
    {
        buffer = gst_sample_get_buffer(sample);	//Get the buffer associated with sample
        if (gst_buffer_map(buffer, &map, GST_MAP_READ))
        {
            //g_print("size: %ld \n", map.size);
            Mat frameYUV(res_h + res_h/2, res_w, CV_8UC1 , map.data, Mat::AUTO_STEP); //Converting mapping of sample to openCV Mat
            Mat frameRGB(res_h, res_w, CV_8UC4);	//Creating a matrix for RGB values
            cvtColor(frameYUV, frameRGB, COLOR_YUV2BGRA_I420);	//Converting YUV image to RGB
            //imwrite("checkRGB.jpg", frameRGB);

            Mat track_motion = Mat::zeros(frameRGB.size(), CV_8UC4);	//Creating a matrix with zeros (black) for detection
            Mat converted_to_HSV;	//Declaring a matrix to store BGR to HSV converted image
            cvtColor(frameRGB, converted_to_HSV, COLOR_BGR2HSV);	//Converting BGR image to HSV
            //imwrite("checkHSV.jpg", converted_to_HSV);

            Mat adjusted_frame;	//Declaring a matrix for detected object color
            inRange(converted_to_HSV, Scalar(hue_low, sat_low, val_low), Scalar(hue_high, sat_high, val_high), adjusted_frame);	//Applying range of HSV values to detect
            //imwrite("checkDetect.jpg", adjusted_frame);

            Moments detecting_object = moments(adjusted_frame);	//Creating an object from the detected color frame
            double vertical_moment = detecting_object.m01;	//Getting value of vertical position of the object
            double horizontal_moment = detecting_object.m10;	//Getting value of horizontal position of the object
            double tracking_area = detecting_object.m00;	//Getting area of the object
            if (tracking_area > 10000){	//When the area of the object is greater than 10000 pixels
                int posX = horizontal_moment / tracking_area;	//Calculate the horizontal position of the object
                int posY = vertical_moment / tracking_area;	//Calculate the vertical position of the object
                distX = (160 - posX) * 0.00284925;
                distY = (120 - posY) * 0.00293652;

                callback(distX, distY);

                horizontal_last = posX;	//Getting a new horizontal position
                vertical_last = posY;	//Getting a new vertical position value
            }

            //g_print("Position of the object is: %d, %d \n", horizontal_last, vertical_last);	//Showing tracked co-ordinated values
            //g_print("Radians of the object from center is: %f, %f \n", distX, distY);	//Showing distance from object center to frame center, needed for motor controller

            /* Write location data to files */
            /*std::fstream file;

            file.open("PosX.txt", std::ios::out | std::ios::app);
            std::string message = std::to_string(horizontal_last); //Taking input from each frame to write to PosX.txt file
            file<< message << "," << std::endl;
            file.close();

            file.open("PosY.txt", std::ios::out | std::ios::app);
            message = std::to_string(vertical_last); //Taking input from each frame to write to PosY.txt file
            file<< message << "," << std::endl;
            file.close();
            */

        }
        //g_print("Found frame \n \n");
        return GST_FLOW_OK;
    }
    gst_buffer_unmap ((buffer), &map);
    gst_sample_unref (sample);
    return GST_FLOW_ERROR;
}

void ImageRecognition::run ()
{
    /* Set the pipeline to "playing" state*/
    g_print ("Now playing!");
    gst_element_set_state (element.pipeline, GST_STATE_PLAYING);

    /* Iterate */
    g_print ("Running...\n");
    g_main_loop_run (loop);

    /* Out of the main loop, clean up nicely */
    g_print ("Returned, stopping playback\n");
    gst_element_set_state (element.pipeline, GST_STATE_NULL);

    g_print ("Deleting pipeline\n");
    gst_object_unref (GST_OBJECT (element.pipeline));
    g_source_remove (bus_watch_id);
    g_main_loop_unref (loop);
}
