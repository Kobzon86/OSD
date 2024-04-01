/***********************************************************
 *                          INCLUDES                       *
 ***********************************************************/
//#include <io.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "xil_io.h"
#include "Graphics_lib.hpp"

/***********************************************************
 *                     CLASS IMPLEMENTATION                *
 ***********************************************************/

/*
 * Constructor
 */
Graphics::Graphics(){
}

/*
 * Destructor
 */
Graphics::~Graphics(){
}

/*
 * get_Graphic_Base_Address
 */
void* Graphics::get_Graphic_Base_Address(void){
	return(get_Buffer());
}

/*
 * Set_Pixel
 *
 * Purpose:
 * 		Sets the specified pixel to the specified color. Sets one pixel
 * 		although frame buffer consists of two-pixel words. Therefore
 * 		this function is not efficient when painting large areas of
 * 		the screen.
 */
void Graphics::Set_Pixel(int horiz, int vert, unsigned int color){
	int addr;

	if(vd_color_depth == 32){
		addr = ((int)(vd_buffer_ptrs[vd_buffer_being_written]->buffer) + (vert * (vd_width * 4)) + horiz * 4);
		Xil_Out32(addr, (unsigned int)(color));
	}
	else if(vd_color_depth == 24){
		addr = ((int)(vd_buffer_ptrs[vd_buffer_being_written]->buffer) + (vert * (vd_width * 3)) + horiz * 3);
		Xil_Out8(addr, (unsigned char)(color));
		Xil_Out8(addr+1, (unsigned char)(color >> 8));
		Xil_Out8(addr+2, (unsigned char)(color >> 16));
	}
	else if(vd_color_depth == 16 ){
		addr = ((int)(vd_buffer_ptrs[vd_buffer_being_written]->buffer) + (vert * (vd_width * 2)) + horiz * 2);
		Xil_Out16(addr, (int)(color));
	}
	else if(vd_color_depth == 8 ){
		addr = ((int)(vd_buffer_ptrs[vd_buffer_being_written]->buffer) + (vert * (vd_width * 1)) + horiz * 1);
		Xil_Out16(addr, (int)(color));
	}
}

/*
 * Set_Round_Corner_Points
 *
 * Purpose:
 * 		Called by vid_draw_round_corner_box() and vid_draw_circle()
 * 		to plot the actual points of the round corners. Draws
 * 		horizontal lines to fill the shape.
 */
void Graphics::Set_Round_Corner_Points(int cx, int cy, int x, int y, int straight_width, int straight_height, int color, char fill){
	// If we're directly above, below, left and right of center (0 degrees), plot those 4 pixels
	if(x == 0){
		// bottom
		Set_Pixel(cx, cy + y + straight_height, color);
		Set_Pixel(cx + straight_width, cy + y + straight_height, color);
		// top
		Set_Pixel(cx, cy - y, color);
		Set_Pixel(cx + straight_width, cy - y, color);

		if(fill){
			Draw_Line(cx - y, cy, cx + y + straight_width, cy, 1, color);
			Draw_Line(cx - y, cy + straight_height, cx + y + straight_width, cy + straight_height, 1, color);
		}
		else{
			//right
			Set_Pixel(cx + y + straight_width, cy, color);
			Set_Pixel(cx + y + straight_width, cy + straight_height, color);
			//left
			Set_Pixel(cx - y, cy, color);
			Set_Pixel(cx - y, cy + straight_height, color);
		}
	}
	else{
		// If we've reached the 45 degree points (x=y), plot those 4 pixels
		if(x == y){
			if(fill){
				Draw_Line(cx - x, cy + y + straight_height, cx + x + straight_width, cy + y + straight_height, 1, color); // lower
				Draw_Line(cx - x, cy - y, cx + x + straight_width, cy - y, 1, color); // upper
			}
			else{
				Set_Pixel(cx + x + straight_width, cy + y + straight_height, color); // bottom right
				Set_Pixel(cx - x, cy + y + straight_height, color); // bottom left
				Set_Pixel(cx + x + straight_width, cy - y, color); // top right
				Set_Pixel(cx - x, cy - y, color); // top left
			}
		}
		else{
			// If we're between 0 and 45 degrees plot 8 pixels.
			if(x < y){
				if(fill){
					Draw_Line(cx - x, cy + y + straight_height, cx + x + straight_width, cy + y + straight_height, 1, color);
					Draw_Line(cx - y, cy + x + straight_height, cx + y + straight_width, cy + x + straight_height, 1, color);
					Draw_Line(cx - y, cy - x, cx + y + straight_width, cy - x, 1, color);
					Draw_Line(cx - x, cy - y, cx + x + straight_width, cy - y, 1, color);
				}
				else{
					Set_Pixel(cx + x + straight_width, cy + y + straight_height, color);
					Set_Pixel(cx - x, cy + y + straight_height, color);
					Set_Pixel(cx + x + straight_width, cy - y, color);
					Set_Pixel(cx - x, cy - y, color);
					Set_Pixel(cx + y + straight_width, cy + x + straight_height, color);
					Set_Pixel(cx - y, cy + x + straight_height, color);
					Set_Pixel(cx + y + straight_width, cy - x, color);
					Set_Pixel(cx - y, cy - x, color);
				}
			}
		}
	}
}

/*
 * Paint_Block
 *
 *  Purpose:
 *  	Paints a block of the screen the specified color.
 *    Note: works with two pixels at a time to maintain high
 *    bandwidth.  Therefore, corner points must be even
 *    numbered coordinates.  Use Draw_solid_box() for
 *    areas with odd-numbered corner points.
 *    The color parameter must contain two pixel's worth
 *    (32 bits).
 */
void Graphics::Paint_Block(int horiz_start, int vert_start, int horiz_end, int vert_end, int color, char transparency){
  int i;
  int addr;
  int bytes_per_line, bytes_per_pixel;
  char* line;

  bytes_per_pixel = (vd_color_depth / 8);
  bytes_per_line = ((horiz_end - horiz_start) * bytes_per_pixel);

  line = (char*)calloc(bytes_per_line + 12, sizeof(char));
  if (line==NULL)
	  return;

	if(vd_color_depth == 16){
    for(i=0; i<bytes_per_line; i+=2){
    	*(line + i) = (unsigned char)color;
      *(line + i + 1) = (unsigned char)(color >> 8);
    }
  }
  else if(vd_color_depth == 24){
    for(i=0; i<bytes_per_line; i+=3){
      *(line + i) = (unsigned char)color;
      *(line + i + 1) = (unsigned char)(color >> 8);
      *(line + i + 2) = (unsigned char)(color >> 16);
    }
  }
  else if(vd_color_depth == 32){
    for(i=0; i<bytes_per_line; i+=4){
      // Does the right hand side of this assignment determine the size?
      *(int*)(line + i) = (unsigned int)color;
      *(line + i + 3) = (unsigned char)(transparency);
    }
  }

  /* Initial Address */
  addr = (int)(vd_buffer_ptrs[vd_buffer_being_written]->buffer) + ((vert_start * (vd_width * bytes_per_pixel)) + (horiz_start * bytes_per_pixel));

  for(i=vert_start; i<vert_end; i++){
    memcpy((void*)addr, line, bytes_per_line);
    addr += (vd_width * bytes_per_pixel);
  }
  free(line);
}

/*
 * Draw_Horiz_Line
 *
 * Purpose:
 * 		Draws a horizontal line on the screen quickly.
 *    Good for filling stuff.
 */
void Graphics::Draw_Horiz_Line(short horiz_start, short horiz_end, int vert, int color){
	int i;
	int addr;
	int bytes_per_line;

	char *fast_buffer = (char*) calloc(1024 * 3, sizeof(char));
	if (fast_buffer==NULL)
		return;

	if(horiz_start > horiz_end){
		short temp = horiz_start;
		horiz_start = horiz_end;
		horiz_end = temp;
	}

	if(vd_color_depth == 32){
		addr = (int)(vd_buffer_ptrs[vd_buffer_being_written]->buffer) + ((vert * (vd_width * 4)) + (horiz_start * 4));
	  bytes_per_line = ((horiz_end - horiz_start) * 4);
	  for(i=0; i<bytes_per_line; i+=4){
	  	// Does the right hand side of this assignment determine the size?
	    *(int*)(fast_buffer + i) = (unsigned int)color;
	  }
	  memcpy((void*)addr, fast_buffer, bytes_per_line);
	}
	else if(vd_color_depth == 24){
		addr = (int)(vd_buffer_ptrs[vd_buffer_being_written]->buffer) + ((vert * (vd_width * 3)) + (horiz_start * 3));
		bytes_per_line = ((horiz_end - horiz_start) * 3);
	  for(i=0; i<bytes_per_line; i+=3){
	  	*(fast_buffer + i) = (unsigned char)color;
	    *(fast_buffer + i + 1) = (unsigned char)(color >> 8);
	    *(fast_buffer + i + 2) = (unsigned char)(color >> 16);
	  }
	  memcpy( (void*)addr, fast_buffer, bytes_per_line );
	}
	else if(vd_color_depth == 16){
		addr = (int)(vd_buffer_ptrs[vd_buffer_being_written]->buffer) + ((vert * (vd_width * 2)) + (horiz_start * 2));
		bytes_per_line = ((horiz_end - horiz_start) * 2);
	  for(i=0; i<bytes_per_line; i+=2){
	  	*(fast_buffer + i) = (unsigned char)color;
	    *(fast_buffer + i + 1) = (unsigned char)(color >> 8);
	  }
	  memcpy( (void*)addr, fast_buffer, bytes_per_line );
	}

	free(fast_buffer);
}

/*
 * Draw_Sloped_Line
 *
 * Purpose:
 * 		Draws a line between two end points using Bresenham's line drawing algorithm.
 */
void Graphics::Draw_Sloped_Line(unsigned short horiz_start, unsigned short vert_start, unsigned short horiz_end, unsigned short vert_end, int color){
	// Find the vertical and horizontal distance between the two points
	int horiz_delta = abs(horiz_end - horiz_start);
	int vert_delta = abs(vert_end - vert_start);

	// Find out what direction we are going
	int horiz_incr, vert_incr;
	if(horiz_start > horiz_end){
		horiz_incr=-1;
	}
	else{
		horiz_incr=1;
	}

	if(vert_start > vert_end){
		vert_incr=-1;
	}
	else{
		vert_incr=1;
	}

	// Find out which axis is always incremented when drawing the line

	// If it's the horizontal axis
	if(horiz_delta >= vert_delta){
		int dPr = vert_delta << 1;
	  int dPru = dPr - (horiz_delta << 1);
	  int P = dPr - horiz_delta;

	  // Process the line, one horizontal point at at time
	  for(; horiz_delta>= 0; horiz_delta--){
	  	// plot the pixel
	    Set_Pixel(horiz_start, vert_start, color);
	    // If we're moving both up and right
	    if(P > 0){
	    	horiz_start += horiz_incr;
	      vert_start += vert_incr;
	      P += dPru;
	    }
	    else{
	    	horiz_start += horiz_incr;
	    	P += dPr;
	    }
	  }
	}
	// If it's the vertical axis
	else{
		int dPr = horiz_delta << 1;
	  int dPru = dPr - (vert_delta << 1);
	  int P = dPr - vert_delta;

	  // Process the line, one vertical point at at time
	  for(; vert_delta>=0; vert_delta--){
	  	// plot the pixel
	    Set_Pixel(horiz_start, vert_start, color);
	    // If we're moving both up and right
	    if(P > 0){
				horiz_start += horiz_incr;
				vert_start += vert_incr;
				P += dPru;
			}
	    else{
				vert_start += vert_incr;
				P += dPr;
			}
	  }
	}
}

/*
 * CopyImageToBuffer
 *
 * Purpose:
 * 		Copies an image to a video buffer of a different width.  The source and
 *    destination do not have to be contained in the same buffer, and can be
 *    differend widths.  This function is useful for copying images stored in
 *    RAM to the active frame buffer to be displayed.
 */
void Graphics::CopyImageToBuffer(char* dest, char* src, int src_width, int src_height){
  int i;

  //Copy one line at a time from top to bottom
  for(i=0; i<src_height; i++){
    memcpy(dest, src, (src_width * 4));
    src += (src_width * 4);
    dest += (vd_width * 4);
  }
}

/*
 * Draw_Line
 *
 * Purpose:
 * 		Draws a line between two end points. First checks
 *    to see if the line is horizontal. If it is, it calls
 *    Draw_Horiz_Line(), which is much faster than
 *    Draw_Sloped_Line.
 */
__inline__ void Graphics::Draw_Line(int horiz_start, int vert_start, int horiz_end, int vert_end, int width, int color){
	if(vert_start == vert_end){
		Draw_Horiz_Line((unsigned short)horiz_start, (unsigned short)horiz_end, (unsigned short)vert_start, color);
	}
	else{
		Draw_Sloped_Line((unsigned short)horiz_start, (unsigned short)vert_start, (unsigned short)horiz_end, (unsigned short)vert_end, color);
	}
}

/*
 * Draw_Box
 *
 *  Purpose:
 *		Draws a box on the screen with the specified corner
 *  	points. The fill parameter tells the function whether
 *  	or not to fill in the box. 1 = fill, 0 = do not fill.
 */
void Graphics::Draw_Box(int horiz_start, int vert_start, int horiz_end, int vert_end, int color, int fill, char transparency){
	// If we want to fill in our box
	if(fill){
		Paint_Block(horiz_start, vert_start, horiz_end, vert_end, color, transparency);
	// If we're not filling in the box, just draw four lines.
	}
	else{
		Draw_Line(horiz_start, vert_start, horiz_start, vert_end-1, 1, color);
		Draw_Line(horiz_end-1, vert_start, horiz_end-1, vert_end-1, 1, color);
		Draw_Line(horiz_start, vert_start, horiz_end-1, vert_start, 1, color);
		Draw_Line(horiz_start, vert_end-1, horiz_end-1, vert_end-1, 1, color);
	}
}

/*
 * Draw_Rounded_Box
 *
 * Purpose:
 * 		Draws a box on the screen with the specified corner
 *  	points.  The fill parameter tells the function whether or not
 *  	to fill in the box.  1 = fill, 0 = do not fill.
 */
void Graphics::Draw_Rounded_Box(int horiz_start, int vert_start, int horiz_end, int vert_end, int radius, int color, int fill, char transparency){
	unsigned int x, y;
	int p;
	int diameter;
	int temp;
	int width, height, straight_width, straight_height;

	// Make sure the start point us up and left of the end point
	if(horiz_start > horiz_end){
		temp = horiz_end;
	  horiz_end = horiz_start;
	  horiz_start = temp;
	}

	if(vert_start > vert_end){
		temp = vert_end;
		vert_end = vert_start;
		vert_start = temp;
	}

	// These are the overall dimensions of the box
	width = horiz_end - horiz_start;
	height = vert_end - vert_start;

	// Make sure our radius isnt more than the shortest dimension
	// of the box, or it'll screw us all up
	if(radius > (width / 2))
		radius = width / 2;

	if( radius > (height / 2))
		radius = height / 2;

	// We use the diameter for some calculations, so we'll pre calculate it here.
	diameter = (radius * 2);

	// These are the lengths of the straight portions of the box edges.
	straight_width = width - diameter;
	straight_height = height - diameter;

	x = 0;
	y = radius;
	p = (5 - radius*4)/4;

	// Start the corners with the top, bottom, left, and right pixels.
	Set_Round_Corner_Points(horiz_start + radius, vert_start + radius, x, y, straight_width, straight_height, color, fill);

	// Now start moving out from those points until the lines meet
	while (x < y) {
		x++;
		if(p < 0){
			p += 2*x+1;
		}
		else{
			y--;
			p += 2*(x-y)+1;
		}
		Set_Round_Corner_Points(horiz_start + radius, vert_start + radius, x, y, straight_width, straight_height, color, fill);
	}

	// If we want to fill in our box
	if(fill){
		Paint_Block(horiz_start, vert_start + radius, horiz_end, vert_end - radius, color, transparency);
	// If we're not filling in the box, just draw four lines.
	}
	else{
		Draw_Line(horiz_start, vert_start + radius, horiz_start, vert_end - radius , 1, color); //left
		Draw_Line(horiz_end, vert_start + radius, horiz_end, vert_end - radius , 1, color); //right
		Draw_Line(horiz_start + radius, vert_start, horiz_end - radius , vert_start, 1, color); //top
		Draw_Line(horiz_start + radius, vert_end, horiz_end - radius , vert_end, 1, color); //bottom
	}
}


/*
 * Draw_Circle
 *
 * Purpose:
 * 		Draws a circle on the screen with the specified center and radius.
 * 		Draws symetric circles only. The fill parameter tells the function
 * 		whether or not to fill in the box.  1 = fill, 0 = do not fill.
 */
void Graphics::Draw_Circle(int Hcenter, int Vcenter, int radius, int color, char fill){
	int x = 0;
	int y = radius;
	int p = (5 - radius*4)/4;

	// Start the circle with the top, bottom, left, and right pixels.
	Set_Round_Corner_Points(Hcenter, Vcenter, x, y, 0, 0, color, fill);

	// Now start moving out from those points until the lines meet
	while(x < y){
		x++;
		if(p < 0){
			p += 2*x+1;
		}
		else{
			y--;
			p += 2*(x-y)+1;
		}
		Set_Round_Corner_Points(Hcenter, Vcenter, x, y, 0, 0, color, fill);
	}
}


/*
 * Seperate_Color_Channels
 *
 * Purpose:
 * 		Takes a single colour pointer and break the value into
 *    seperate channels. It supports 16/24(packed/unpacked)
 *    formats as inputs. The results are passed back by
 *    the function working on pointers.
 */
__inline__ void Graphics::Seperate_Color_Channels(unsigned char * color, unsigned char * red, unsigned char * green, unsigned char * blue){
	unsigned short temp_color = 0;

	if(vd_color_depth == 16){	// R(5):G(6):B(5)
		temp_color = *color++;        // Get the first half of the 16 bit color
	  temp_color += (*color << 8);  // Get the second half of the 16 bit color
	  *blue = temp_color & 0x1F;
	  *green = (temp_color >> 5) & 0x3F;
	  *red = (temp_color >> 11) & 0x1F;
	}
	else if((vd_color_depth == 24) || (vd_color_depth == 32)){	// R(8):G(8):B(8) packed/unpacked
		*blue = *color++;
	  *green = *color++;
	  *red = *color;
	}
	else{	// color_depth == 8
		*red = *color;
	  *green = 0;
	  *blue = 0;
	}
}

/*
 * Read_From_Frame
 *
 * Purpose:
 * 		Reads a pixel value from an existing frame buffer and
 *    separates the color channels.  This is useful for alpha
 *    blending where the previously rendored pixel value is
 *    needed to calculate the new one.
 */
__inline__ void Graphics::Read_From_Frame(int horiz, int vert, unsigned char *red, unsigned char *green, unsigned char *blue){
	unsigned int temp_color = 0;
	unsigned int addr = 0;

	if(vd_color_depth == 32){
		addr = ((unsigned int)vd_buffer_ptrs[vd_buffer_being_written]->buffer) + ((unsigned int)((vert * vd_width * 4) + (horiz * 4)));
	  temp_color = Xil_In32(addr);
	  *blue = (unsigned char)(temp_color & 0xFF);
	  *green = (unsigned char)((temp_color >> 8) & 0xFF);
	  *red = (unsigned char)((temp_color >> 16) & 0xFF);
	}
	else if(vd_color_depth == 24){
		addr = ((unsigned int)vd_buffer_ptrs[vd_buffer_being_written]->buffer) + ((unsigned int)((vert * vd_width * 3) + (horiz * 3)));
	  *blue = Xil_In8(addr + 0);
	  *green = Xil_In8(addr + 1);
	  *red = Xil_In8(addr + 2);
	}
	else if(vd_color_depth == 16){
		addr = ((unsigned int)vd_buffer_ptrs[vd_buffer_being_written]->buffer) + ((unsigned int)((vert * vd_width * 2) + (horiz * 2)));
	  temp_color = (unsigned int)Xil_In16(addr);
	  *blue = (unsigned char)(temp_color & 0x1F);
	  *green = (unsigned char)((temp_color >> 5) & 0x3F);
	  *red = (unsigned char)((temp_color >> 11) & 0x1F);
	}
	else{
		*blue = 0;
	  *red = Xil_In8(addr);;
	  *green = 0;
  }
}

/*
 * Alpha_Blending
 *
 * Purpose:
 * 		Takes in the alpha value (0-255) and uses that to mix
 *    the font color with the background color.
 */
__inline__ void Graphics::Alpha_Blending(int horiz_offset, int vert_offset, int background_color, unsigned char alpha, unsigned char *red, unsigned char *green, unsigned char *blue){
	unsigned char background_red, background_green, background_blue;
	unsigned int blended_red, blended_green, blended_blue;  // Short since there may be overflow to be clipped

	// Set red, green, and blue of the background color
	if(background_color == CLEAR_BACKGROUND){
		Read_From_Frame(horiz_offset, vert_offset, &background_red, &background_green, &background_blue);
	}
	else{
		Seperate_Color_Channels((unsigned char *)&background_color, &background_red, &background_green, &background_blue);
	}

	// These blended colors may need to be clipped to the maximum amounts the color depth supports
	blended_red = (((*red) * alpha) + (background_red * (255 - alpha)))/255;
	blended_green = (((*green) * alpha) + (background_green * (255 - alpha)))/255;
	blended_blue = (((*blue) * alpha) + (background_blue * (255 - alpha)))/255;

	if(vd_color_depth == 16){
		*red = (blended_red > 0x1F)? 0x1F: blended_red;
		*green = (blended_green > 0x3F)? 0x3F: blended_green;
		*blue = (blended_blue > 0x1F)? 0x1F: blended_blue;
	}
	else if((vd_color_depth == 24) || (vd_color_depth == 32)){
		*red = (blended_red > 0xFF)? 0xFF: blended_red;
		*green = (blended_green > 0xFF)? 0xFF: blended_green;
		*blue = (blended_blue > 0xFF)? 0xFF: blended_blue;
	}
	else{
		*red = (blended_red > 0xFF)? 0xFF: blended_red;
	}
}

/*
 * Merge_Color_Channels
 *
 * Purpose:
 * 		Takes the seperate RGB channels and merges them into
 *    a single 16/24/32 bit location.  The caller must
 *    use an unsigned char * pointer to the destination
 *    color allowing the same function to be used for
 *    different color formats and packing.
 */
__inline__ void Graphics::Merge_Color_Channels(unsigned char red, unsigned char green, unsigned char blue, unsigned char * color){
	unsigned short temp_color;

	if(vd_color_depth == 16){  // R(5):G(6):B(5)
		temp_color = (blue & 0x1F) | ((green & 0x3F) << 5) | ((red & 0x1F) << 11);  // 16 bit color format
	  *color++ = temp_color & 0xFF;
	  *color = (temp_color >> 8) & 0xFF;
	}
	else if((vd_color_depth == 24) || (vd_color_depth == 32)){  // R(8):G(8):B(8) packed/unpacked
		*color++ = blue;
	  *color++ = green;
	  *color = red;
	}
	else{  // 8bit // not a valid color depth for this function
		*color = red;
	}
}


/*
 * Print_Char_Alpha
 *
 * Purpose:
 * 		Prints a character to the specified location of the
 *    screen using the specified font and color.
 */
void Graphics::Print_Char_Alpha(int horiz_offset, int vert_offset, int color, char character, int background_color, font_struct font[]){
	unsigned long i, j;
	unsigned char * alpha;
	unsigned char original_red, original_blue, original_green;
	unsigned char red, green, blue;
	int new_color;

	// Assign the pointer of the font bitmap
	alpha = font[character-33].char_alpha_map;

	// Set red, green, and blue of the font color
	Seperate_Color_Channels((unsigned char *)&color, &original_red, &original_green, &original_blue);

	for(i=0; i<font[character-33].bounds_height; i++){
		for(j=0; j<font[character-33].bounds_width; j++){
			red = original_red;
	    green = original_green;
	    blue = original_blue;

	    // Send in the font alpha channel and the colour channels for the blending to occur
	    Alpha_Blending((horiz_offset + j), (vert_offset + i), background_color, *alpha, &red, &green, &blue);

	    // Take the blended pixel and merge the color channels back into a single pixel value
	    Merge_Color_Channels(red, green, blue, (unsigned char *)&new_color);

	    // Write the new pixel value out to memory overwriting the current background
	    Set_Pixel((horiz_offset + j), (vert_offset + i), new_color);

	    alpha++;
		}
	}
}

/*
 *  Print_String_Alpha
 *
 *  Purpose:
 *  		Prints a string to the specified location of the screen
 *      using the specified font and color. Calls Print_Char_Alpha.
 *
 *      The color formats are as follows:
 *      	-> 16 bit - R(5):G(6):B(5)  (packed)
 *        -> 24 bit - R(8):G(8):B(8)  (packed)
 *        -> 32 bit - 0 : R(8):G(8):B(8) (unpacked)
 */
void Graphics::Print_String_Alpha(int horiz_offset, int vert_offset, int color, int background_color, font_struct font[], const char string[]){
	int i = 0;
	int tab;
	int original_horiz_offset;

	original_horiz_offset = horiz_offset;

	// Print until we hit the '\0' char.
	while(string[i]){
		// Handle newline char here.
		if(string[i] == '\n'){
			horiz_offset = original_horiz_offset;
	    vert_offset += font['|' - 33].bounds_height;  // we'll use "|" to add the line to line spacing
	    i++;
	    continue;
		}
	  // Lay down that character and increment our offsets.
	  if(string[i] == 32){  // this is a space in ASCII
	  	if(background_color != CLEAR_BACKGROUND){  // need to fill in this spot (setting foreground to background color so that it looks like a drawn in box)
	  		Print_Char_Alpha(horiz_offset, vert_offset, background_color, '-', background_color, font);
	  	}
	    horiz_offset += font[45 - 33].bounds_width;  // using the width of the '-' character for the space width since it's not present in the font
	  }
	  else if(string[i] == '\t'){  // this is a tab
	  	for(tab=0; tab<TAB_SPACING; tab++){
	  		if(background_color != CLEAR_BACKGROUND){  // need to fill in this spot (setting foreground to background color so that it looks like a drawn in box)
	  			Print_Char_Alpha (horiz_offset, vert_offset, background_color, '-', background_color, font);
	  		}
	      horiz_offset += font[45 - 33].bounds_width;  // using the width of the '-' character for the space width since it's not present in the font
	  	}
	  }
	  else{
	  	Print_Char_Alpha(horiz_offset, vert_offset, color, string[i], background_color, font);
	  	horiz_offset += font[string[i] - 33].bounds_width;
	  }
	  i++;
	}
}


/*
 * Get_String_Pixel_Length_Alpha
 *
 * Purpose:
 * 		Gets the length in pixels of a string in the specified
 *    font. Useful for centering text, and determining the
 *    appropriate size of a text box or button.
 *
 */
int Graphics::Get_String_Pixel_Length_Alpha(font_struct font[], char string[]){
  int i = 0;
  int length = 0;

  // Count the length of the string
  for(i=0; string[i]!= '\0'; i++){
    if(string[i] == ' '){
      // Here we just add the length of the '-' character
      length += font[45 - 33].bounds_width;
    }
    else{
      length += font[string[i] - 33].bounds_width;
    }
  }
  return(length);
}
