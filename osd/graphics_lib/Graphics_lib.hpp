#ifndef GRAPHICS_LIB_HPP_
#define GRAPHICS_LIB_HPP_

/***********************************************************
 *                          INCLUDES                       *
 ***********************************************************/
#include "../fonts/fonts.h"
#include "../video_display/Video_Display.hpp"

/***********************************************************
 *                          DEFINES                        *
 ***********************************************************/


// Color Definitions
// Use these colours when in RGB format
	#define ALICEBLUE_24    				0xF0F8FF
	#define ANTIQUEWHITE_24   			    0xFAEBD7
	#define AQUA_24   						0x00FFFF
	#define AQUAMARINE_24  				 	0x7FFFD4
	#define AZURE_24    					0xF0FFFF
	#define BEIGE_24    					0xF5F5DC
	#define BISQUE_24   					0xFFE4C4
	#define BLACK_24    					0x000000
	#define BLANCHEDALMOND_24   		    0xFFEBCD
	#define BLUE_24   						0x0000FF
	#define BLUEVIOLET_24   				0x8A2BE2
	#define BROWN_24    					0xA52A2A
	#define BURLYWOOD_24    				0xDEB887
	#define CADETBLUE_24    				0x5F9EA0
	#define CHARTREUSE_24   				0x7FFF00
	#define CHOCOLATE_24    				0xD2691E
	#define CORAL_24    					0xFF7F50
	#define CORNFLOWERBLUE_24   		    0x6495ED
	#define CORNSILK_24   					0xFFF8DC
	#define CRIMSON_24    					0xDC143C
	#define CYAN_24   						0x00FFFF
	#define DARKBLUE_24   					0x00008B
	#define DARKCYAN_24   					0x008B8B
	#define DARKGOLDENROD_24   			    0xB8860B
	#define DARKGRAY_24   					0xA9A9A9
	#define DARKGREY_24   					0xA9A9A9
	#define DARKGREEN_24    				0x006400
	#define DARKKHAKI_24    				0xBDB76B
	#define DARKMAGENTA_24    			    0x8B008B
	#define DARKOLIVEGREEN_24   		    0x556B2F
	#define DARKORANGE_24   				0xFF8C00
	#define DARKORCHID_24   				0x9932CC
	#define DARKRED_24    					0x8B0000
	#define DARKSALMON_24   				0xE9967A
	#define DARKSEAGREEN_24   			    0x8FBC8F
	#define DARKSLATEBLUE_24    		    0x483D8B
	#define DARKSLATEGRAY_24    		    0x2F4F4F
	#define DARKSLATEGREY_24    		    0x2F4F4F
	#define DARKTURQUOISE_24    		    0x00CED1
	#define DARKVIOLET_24   				0x9400D3
	#define DEEPPINK_24   					0xFF1493
	#define DEEPSKYBLUE_24    			    0x00BFFF
	#define DIMGRAY_24    					0x696969
	#define DIMGREY_24    					0x696969
	#define DODGERBLUE_24   				0x1E90FF
	#define FIREBRICK_24    				0xB22222
	#define FLORALWHITE_24    			0xFFFAF0
	#define FORESTGREEN_24    			0x228B22
	#define FUCHSIA_24  						0xFF00FF
	#define GAINSBORO_24    				0xDCDCDC
	#define GHOSTWHITE_24   				0xF8F8FF
	#define GOLD_24   							0xFFD700
	#define GOLDENROD_24    				0xDAA520
	#define GRAY_24   							0x808080
	#define GREY_24   							0x808080
	#define GREEN_24    						0x00FF00
	#define GREENYELLOW_24    			0xADFF2F
	#define HONEYDEW_24   					0xF0FFF0
	#define HOTPINK_24    					0xFF69B4
	#define INDIANRED_24    				0xCD5C5C
	#define INDIGO_24     					0x4B0082
	#define IVORY_24    						0xFFFFF0
	#define KHAKI_24    						0xF0E68C
	#define LAVENDER_24   					0xE6E6FA
	#define LAVENDERBLUSH_24    		0xFFF0F5
	#define LAWNGREEN_24    				0x7CFC00
	#define LEMONCHIFFON_24   			0xFFFACD
	#define LIGHTBLUE_24    				0xADD8E6
	#define LIGHTCORAL_24   				0xF08080
	#define LIGHTCYAN_24    				0xE0FFFF
	#define LIGHTGOLDENRODYELLOW_24 0xFAFAD2
	#define LIGHTGRAY_24    				0xD3D3D3
	#define LIGHTGREY_24    				0xD3D3D3
	#define LIGHTGREEN_24   				0x90EE90
	#define LIGHTPINK_24    				0xFFB6C1
	#define LIGHTSALMON_24    			0xFFA07A
	#define LIGHTSEAGREEN_24    		0x20B2AA
	#define LIGHTSKYBLUE_24   			0x87CEFA
	#define LIGHTSLATEGRAY_24   		0x778899
	#define LIGHTSLATEGREY_24   		0x778899
	#define LIGHTSTEELBLUE_24   		0xB0C4DE
	#define LIGHTYELLOW_24    			0xFFFFE0
	#define LIME_24   							0x00FF00
	#define LIMEGREEN_24    				0x32CD32
	#define LINEN_24    						0xFAF0E6
	#define MAGENTA_24    					0xFF00FF
	#define MAROON_24   						0x800000
	#define MEDIUMAQUAMARINE_24   	0x66CDAA
	#define MEDIUMBLUE_24   				0x0000CD
	#define MEDIUMORCHID_24   			0xBA55D3
	#define MEDIUMPURPLE_24   			0x9370D8
	#define MEDIUMSEAGREEN_24_24    0x3CB371
	#define MEDIUMSLATEBLUE_24    	0x7B68EE
	#define MEDIUMSPRINGGREEN_24    0x00FA9A
	#define MEDIUMTURQUOISE_24    	0x48D1CC
	#define MEDIUMVIOLETRED_24    	0xC71585
	#define MIDNIGHTBLUE_24   			0x191970
	#define MINTCREAM_24    				0xF5FFFA
	#define MISTYROSE_24    				0xFFE4E1
	#define MOCCASIN_24   					0xFFE4B5
	#define NAVAJOWHITE_24    			0xFFDEAD
	#define NAVY_24   							0x000080
	#define OLDLACE_24    					0xFDF5E6
	#define OLIVE_24    						0x808000
	#define OLIVEDRAB_24    				0x6B8E23
	#define ORANGE_24   						0xFFA500
	#define ORANGERED_24    				0xFF4500
	#define ORCHID_24   						0xDA70D6
	#define PALEGOLDENROD_24    		0xEEE8AA
	#define PALEGREEN_24    				0x98FB98
	#define PALETURQUOISE_24    		0xAFEEEE
	#define PALEVIOLETRED_24    		0xD87093
	#define PAPAYAWHIP_24   				0xFFEFD5
	#define PEACHPUFF_24    				0xFFDAB9
	#define PERU_24   							0xCD853F
	#define PINK_24   							0xFFC0CB
	#define PLUM_24   							0xDDA0DD
	#define POWDERBLUE_24   				0xB0E0E6
	#define PURPLE_24   						0x800080
	#define RED_24    							0xFF0000
	#define ROSYBROWN_24    				0xBC8F8F
	#define ROYALBLUE_24    				0x4169E1
	#define SADDLEBROWN_24    			0x8B4513
	#define SALMON_24   						0xFA8072
	#define SANDYBROWN_24   				0xF4A460
	#define SEAGREEN_24   					0x2E8B57
	#define SEASHELL_24   					0xFFF5EE
	#define SIENNA_24   						0xA0522D
	#define SILVER_24   						0xC0C0C0
	#define SKYBLUE_24    					0x87CEEB
	#define SLATEBLUE_24    				0x6A5ACD
	#define SLATEGRAY_24    				0x708090
	#define SLATEGREY_24    				0x708090
	#define SNOW_24   							0xFFFAFA
	#define SPRINGGREEN_24    			0x00FF7F
	#define STEELBLUE_24    				0x4682B4
	#define TAN_24    							0xD2B48C
	#define TEAL_24   							0x008080
	#define THISTLE_24    					0xD8BFD8
	#define TOMATO_24   						0xFF6347
	#define TURQUOISE_24    				0x40E0D0
	#define VIOLET_24   						0xEE82EE
	#define WHEAT_24    						0xF5DEB3
	#define WHITE_24    						0xFFFFFF
	#define WHITESMOKE_24   				0xF5F5F5
	#define YELLOW_24   						0xFFFF00
	#define YELLOWGREEN_24    			0x9ACD32
	#define BLUE_DARK								0x29242B
	#define	VIOLET_DARK							0x373D57

// Fill
#define NO_FILL	0
#define FILL		1

// Use this background colour when you don't want a filled in box behind the alpha blended text
#define CLEAR_BACKGROUND -1

// Special Characters
#define TAB_SPACING	2


/***********************************************************
 *                           TYPES                         *
 ***********************************************************/

typedef struct{
	int x;
	int y;
} t_point;

/***********************************************************
 *                       CLASS DEFINITION                  *
 ***********************************************************/

class Graphics : public Video_Display {
	/**************
	 * Properties *
	 **************/

	/***********
	 * Methods *
	 ***********/
	public:
		Graphics(); // Constructor
		~Graphics(); // Destructor

		void* get_Graphic_Base_Address(void);

		/* Graphical */
	private:
		void Set_Pixel(int horiz, int vert, unsigned int color);
		void Set_Round_Corner_Points(int cx, int cy, int x, int y, int straight_width, int straight_height, int color, char fill);
		void Paint_Block(int horiz_start, int vert_start, int horiz_end, int vert_end, int color, char transparency);
		void Draw_Horiz_Line(short horiz_start, short horiz_end, int vert, int color);
		void Draw_Sloped_Line(unsigned short horiz_start, unsigned short vert_start, unsigned short horiz_end, unsigned short vert_end, int color);
		void CopyImageToBuffer(char* dest, char* src, int src_width, int src_height);

	public:
		void Draw_Line(int horiz_start, int vert_start, int horiz_end, int vert_end, int width, int color);
		void Draw_Box(int horiz_start, int vert_start, int horiz_end, int vert_end, int color, int fill, char transparency);
		void Draw_Rounded_Box(int horiz_start, int vert_start, int horiz_end, int vert_end, int radius, int color, int fill, char transparency);
		void Draw_Circle(int Hcenter, int Vcenter, int radius, int color, char fill);


	/* Text */
	private:
		__inline__ void Seperate_Color_Channels(unsigned char * color, unsigned char * red, unsigned char * green, unsigned char * blue);
		__inline__ void Read_From_Frame(int horiz, int vert, unsigned char *red, unsigned char *green, unsigned char *blue);
		__inline__ void Alpha_Blending(int horiz_offset, int vert_offset, int background_color, unsigned char alpha, unsigned char *red, unsigned char *green, unsigned char *blue);
		__inline__ void Merge_Color_Channels(unsigned char red, unsigned char green, unsigned char blue, unsigned char * color);

	public:
		void Print_Char_Alpha(int horiz_offset, int vert_offset, int color, char character, int background_color, font_struct font[]);
		void Print_String_Alpha(int horiz_offset, int vert_offset, int color, int background_color, font_struct font[], const char string[]);
		int Get_String_Pixel_Length_Alpha(font_struct font[], char string[]);
};


#endif /* GRAPHICS_LIB_HPP_ */
