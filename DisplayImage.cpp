// Main header for GStreamer
#include <gst/gst.h>
// Used for several funcions of opencv for image processing
#include "opencv2/imgproc.hpp"

// For saving the images with imwrite for debugging 
#include "opencv2/highgui.hpp"
// For writing x and y values to a file for debugging
#include <fstream>

using namespace std;
using namespace cv;

int Hue_Low= 4;//lower range of hue//
int Hue_high = 84;//upper range of hue//
int Sat_Low =136;//lower range of saturation//
int Sat_high = 255;//upper range of saturation//
int Val_Low = 150;//lower range of value//
int Val_high = 255;//upper range of value// 
int Horizontal_Last = -1;//initial horizontal position//
int vertical_Last = -1;//initial vertical position//

typedef struct _AllElements 
{
  GstElement *pipeline, *source, *filter, *decoder, *conv, *sink;
} AllElements;

static gboolean bus_call (GstBus *bus, GstMessage *msg, gpointer data)
{
  GMainLoop *loop = (GMainLoop *) data;

  switch (GST_MESSAGE_TYPE (msg)) {

    case GST_MESSAGE_EOS:
      g_print ("End of stream\n");
      g_main_loop_quit (loop);
      break;

    case GST_MESSAGE_ERROR: {
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

static GstFlowReturn new_sample (GstElement *sink, AllElements *element)  
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
     g_print("Check frame \n");
     if (sample) 
     {
        buffer = gst_sample_get_buffer(sample);
        if (gst_buffer_map(buffer, &map, GST_MAP_READ)) 
        {
            	g_print("size: %ld \n", map.size);
            	Mat frameYUV(240+240/2, 320, CV_8UC1 , map.data, Mat::AUTO_STEP);
            	Mat frameRGB(240, 320, CV_8UC4);
            	cvtColor(frameYUV, frameRGB, COLOR_YUV2BGRA_I420);
	    	imwrite("checkRGB.jpg", frameRGB);
	    	
		Mat track_motion = Mat::zeros(frameRGB.size(), CV_8UC4);//creating black matrix for detection//
		Mat converted_to_HSV;//declaring a matrix to store converted image//
      		cvtColor(frameRGB, converted_to_HSV, COLOR_BGR2HSV);//converting BGR image to HSV//
		imwrite("checkHSV.jpg", converted_to_HSV);
		
      		Mat adjusted_frame;//declaring a matrix to detected color//
      		inRange(converted_to_HSV, Scalar(Hue_Low, Sat_Low, Val_Low), Scalar(Hue_high, Sat_high, Val_high), adjusted_frame);//applying change of values of track-bars//      
		imwrite("checkDetect.jpg", adjusted_frame);
		
		Moments detecting_object = moments(adjusted_frame);//creating an object from detected color frame//
	      	double vertical_moment = detecting_object.m01;//getting value of vertical position//
	      	double horizontal_moment = detecting_object.m10;//getting value of horizontal position//
	      	double tracking_area = detecting_object.m00;//getting area of the object//
		if (tracking_area > 10000){ //when area of the object is greater than 10000 pixels//
			 int posX = horizontal_moment / tracking_area;//calculate the horizontal position of the object//
			 int posY = vertical_moment / tracking_area;//calculate the vertical position of the object//
			 Horizontal_Last = posX;//getting new horizontal position//
			 vertical_Last = posY;// getting new vertical position value//
	      	}
	      	frameRGB = frameRGB + track_motion;//drawing continuous line in original video frames//
            	g_print("Position of the object is: %d, %d \n", Horizontal_Last, vertical_Last);//showing tracked co-ordinated values//
	      
	      	// Write location data to file
	      	fstream file;
	      	
 		file.open("PosX.txt", ios::out | ios::app);
		//taking input from frame to write to PosX.txt file
		string message = to_string(Horizontal_Last);
		file<< message << "," << endl;
		file.close();
		
		file.open("PosY.txt", ios::out | ios::app);
		//taking input from frame to write to PosY.txt file
		message = to_string(vertical_Last);
		file<< message << "," << endl;
		file.close();
	      
        }
        g_print("Found frame \n \n");
        return GST_FLOW_OK;
    }
    gst_buffer_unmap ((buffer), &map); 
    gst_sample_unref (sample);
    return GST_FLOW_ERROR;
}

int main (int   argc, char *argv[])
{
  GMainLoop *loop;
  AllElements element;
  GstCaps *caps;
  GstBus *bus;
  guint bus_watch_id;

  /* Initialisation */
  gst_init (&argc, &argv);

  loop = g_main_loop_new (NULL, FALSE);

  /* Create gstreamer elements */
  element.pipeline =          gst_pipeline_new ("video-player");
  element.source   =          gst_element_factory_make ("v4l2src",       "cam-source");
  element.filter   = 	       gst_element_factory_make ("capsfilter", "video-filter");
  element.decoder  =          gst_element_factory_make ("jpegdec",     "jpeg-decoder");
  element.sink     =          gst_element_factory_make ("appsink", "appsink");

  if (!element.pipeline || !element.source || !element.filter || !element.decoder || !element.sink) {
    g_printerr ("One element could not be created. Exiting.\n");
    return -1;
  }

  /* Set up the pipeline */

  /* we set the input filename to the source element */
  g_object_set (G_OBJECT (element.source), "device", "/dev/video0", NULL);
  caps = gst_caps_new_simple("image/jpeg", "format", G_TYPE_STRING, "RGB", "framerate", GST_TYPE_FRACTION, 30, 1, "width", G_TYPE_INT, 320, "height", G_TYPE_INT, 240, NULL);
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

  return 0;
}

