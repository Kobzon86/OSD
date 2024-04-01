
#ifndef VIDEO_DISPLAY_HPP_
#define VIDEO_DISPLAY_HPP_

/***********************************************************
 *                          DEFINES                        *
 ***********************************************************/

#define VIDEO_DISPLAY_COLOR_DEPTH 32
#define VIDEO_DISPLAY_USE_HEAP 		-1
#define VIDEO_DISPLAY_COLOR	 			0xFF000000

/* Maximum number of display buffers the driver will accept */
#define VIDEO_DISPLAY_MAX_BUFFERS 2

/***********************************************************
 *                           TYPES                         *
 ***********************************************************/
typedef struct {
  void *desc_base; /* Pointer to SGDMA descriptor chain */
  void *buffer;    /* Pointer to video data buffer */
} video_frame;

/***********************************************************
 *                       CLASS DEFINITION                  *
 ***********************************************************/

class Video_Display {
	/**************
	 * Properties *
	 **************/
	protected:
		void *vd_sgdma;
		int vd_width;
		int vd_height;
		int vd_color_depth;
		int vd_bytes_per_pixel;
		int vd_bytes_per_frame;
		int vd_num_buffers;
		int vd_buffer_location;
		video_frame* vd_buffer_ptrs[VIDEO_DISPLAY_MAX_BUFFERS];
		int vd_buffer_being_displayed;
		int vd_buffer_being_written;
		int vd_screen_base_address;
		int vd_screen_pixel_count;

	/***********
	 * Methods *
	 ***********/
	public:
		Video_Display(); // Constructor
		~Video_Display(); // Destructor

		int Init(int width, int height, int color_depth, int buffer_location, int num_buffers);
		void Init(int width, int height, int color_depth, int num_buffers);

	protected:
		void * get_Buffer();

	private:
		int Allocate_Buffers(void);
};


#endif /* VIDEO_DISPLAY_HPP_ */
