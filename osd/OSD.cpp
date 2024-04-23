/***********************************************************
 *                          INCLUDES                       *
 ***********************************************************/
#include "OSD.hpp"
#include "fonts/fonts.h"
#include <math.h>
#include <stdio.h>

/***********************************************************
 *                     CLASS IMPLEMENTATION                *
 ***********************************************************/
/*
 * Constructor
 */
OSD::OSD(int osd_width, int osd_height, int frame_buffer_addr){

	width = osd_width;
	height = osd_height;

	if(graphic_layer_1.Init(width, height, VIDEO_DISPLAY_COLOR_DEPTH, frame_buffer_addr, 1)){ //0x08000000
		printf("Memory allocation error (graphic_layer_1)!");
		while(1);
	}

	screen_pixel_count = width * height;

	// Clear the frame buffer to initial content
	// We set black color and full transparency
	graphic_layer_1.Draw_Box(0,0,width,height,0x000000,1,0xFF);
}

/*
 * Destructor
 */
OSD::~OSD(void){
}

