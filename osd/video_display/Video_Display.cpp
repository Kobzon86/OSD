/***********************************************************
 *                          INCLUDES                       *
 ***********************************************************/
#include <stdlib.h>
#include <string.h>
//#include <sys/alt_cache.h>
//#include "system.h"

#include "Video_Display.hpp"

/***********************************************************
 *                     CLASS IMPLEMENTATION                *
 ***********************************************************/

/*
 * Constructor
 */
Video_Display::Video_Display(){
}

/*
 * Destructor
 */
Video_Display::~Video_Display(){
	free(vd_buffer_ptrs[0]);
}

/*
 * Init
 */
int Video_Display::Init(int width, int height, int color_depth, int buffer_location, int num_buffers){
	int i, error;

	// We'll need these values more than once, so let's pre-calculate them.
	vd_bytes_per_pixel = color_depth >> 3; // same as /8
	vd_bytes_per_frame = ((width * height) * vd_bytes_per_pixel);

	// Fill out the display structure
	vd_width = width;
	vd_height = height;
	vd_color_depth = color_depth;
	vd_num_buffers = num_buffers;
	vd_buffer_location = buffer_location;
	vd_buffer_being_displayed = 0;
	vd_buffer_being_written = 0;

	// Allocate our frame and descriptor buffers
	error = Allocate_Buffers();

	// Clear all frame buffers to initial content
	//for(i=0; i<this->vd_num_buffers; i++) {
		//memset((void*)(vd_buffer_ptrs[i]->buffer), VIDEO_DISPLAY_COLOR, vd_bytes_per_frame);
		//memset((void*)(vd_buffer_ptrs[i]->buffer), 0, vd_bytes_per_frame);
	//}

	vd_screen_base_address = ((int)(vd_buffer_ptrs[vd_buffer_being_written]->buffer));

	return(error);
}

/*
 * Init without location
 */
void Video_Display::Init(int width, int height, int color_depth, int num_buffers){
	// We'll need these values more than once, so let's pre-calculate them.
	vd_bytes_per_pixel = color_depth >> 3; // same as /8
	vd_bytes_per_frame = ((width * height) * vd_bytes_per_pixel);

	// Fill out the display structure
	vd_width = width;
	vd_height = height;
	vd_color_depth = color_depth;
	vd_num_buffers = num_buffers;
	vd_buffer_being_displayed = 0;
	vd_buffer_being_written = 0;

	vd_buffer_ptrs[0] = (video_frame*) malloc(sizeof(video_frame));
	vd_buffer_ptrs[0]->buffer = (void*) malloc(vd_bytes_per_frame);
}

/*
 * Allocate_Buffers
 *
 * Purpose:
 * 		Allocates memory for both the frame buffers and the descriptor chains.
 *
 *  	If ALT_VIDEO_DISPLAY_USE_HEAP is specified for either the buffer_location
 *  	or descriptor_location, the memory is allocated from the heap. Otherwise,
 *  	buffer_location and descriptor_locaiton are presumed to contain the
 *  	base address of memory sufficient to hold the requested number of
 *  	descriptors and video frames. This memory space may be quite large.
 *
 *  Returns:
 *  	0  - Success.
 *    -1 - Error allocating memory.
 */
int Video_Display::Allocate_Buffers(void){
	 int i, ret_code = 0;

	/* Allocate our frame buffers and descriptor buffers */
	for(i=0; i<vd_num_buffers; i++){
		vd_buffer_ptrs[i] = (video_frame*) malloc(sizeof(video_frame));
		//vd_buffer_ptrs[i] = (video_frame*) alt_uncached_malloc(sizeof(video_frame));

		if(vd_buffer_ptrs[i] == NULL){
			ret_code = -1;
		}

		if(vd_buffer_location == VIDEO_DISPLAY_USE_HEAP ) {
			vd_buffer_ptrs[i]->buffer = (void*) malloc((vd_bytes_per_frame));

			if(vd_buffer_ptrs[i]->buffer == NULL)
				ret_code = -1;
		}
	    else{
	    	//vd_buffer_ptrs[i]->buffer = (void*)alt_remap_uncached((void*)(vd_buffer_location + (i * vd_bytes_per_frame)), vd_bytes_per_frame);
	    	vd_buffer_ptrs[i]->buffer = (void*)(vd_buffer_location + (i * vd_bytes_per_frame));
	    }

		vd_buffer_ptrs[i]->desc_base = ((void*) 0);
	}

	return ret_code;
}

/*
 * get_Buffer()
 */
void * Video_Display::get_Buffer(){
	return(vd_buffer_ptrs[vd_buffer_being_written]->buffer);
}
