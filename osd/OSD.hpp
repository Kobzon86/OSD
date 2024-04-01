
#ifndef OSD_HPP_
#define OSD_HPP_

/***********************************************************
 *                          INCLUDES                       *
 ***********************************************************/
#include "graphics_lib/Graphics_lib.hpp"

/***********************************************************
 *                       CLASS DEFINITION                  *
 ***********************************************************/

class OSD {
	/**************
	 * Properties *
	 **************/
	private:
		int width;
		int height;
		long screen_pixel_count;
	public:
		Graphics graphic_layer_1;

	/***********
	 * Methods *
	 ***********/
	public:
		OSD(int osd_width, int osd_height); // Constructor
		~OSD(void); // Destructor

};

#endif /* OSD_HPP_ */
