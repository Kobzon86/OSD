/*
 * Xilinx ZYNQ-VDMA-OSD project
 * Autor: Maksim Ananev
 */

#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xil_io.h"

#include "osd/OSD.hpp"
#include "osd/fonts/fonts.h"

//#include "iic_utils.h"
#include "xil_cache.h"

#define PATTERN_GREY
#define PATTERN_COL_BARS

#define RED   0x00e00000
#define GREEN 0x0000e000
#define BLUE  0x000000e0

#define RED_HALF   0x00600000
#define GREEN_HALF 0x00006000
#define BLUE_HALF  0x00000060

#define RED_EIGHTS   0x00200000
#define GREEN_EIGHTS 0x00002000
#define BLUE_EIGHTS  0x00000020



#define BLUE_YUYV		0x6B1DFF1D
#define RED_YUYV		0xFF4C544C
#define GREEN_YUYV		0x15952B95
#define MAGENTA_YUYV	0xEA69D469
#define YELLOW_YUYV		0X94FF00FF
#define CYAN_YUYV		0x00B2ABB2
#define WHITE_YUYV		0x80FF80FF
#define BLACK_YUYV		0x80008000

#define WIDTH 1024
#define HEIGHT 768

#define CLK_WIZ_BASEADR 0x43C00000U
#define FRAME_BUFFER_ADDR 0x10000000


volatile int readed;
int main()
{
	int line, column;
	UINTPTR Addr1=FRAME_BUFFER_ADDR;

    init_platform();

    print("VDMA application on ZC702\n\r");
    print("---------------------------------------------\n\n\r");

    print("Initialize frame buffer data...");
    //Disable the data cache to initialize the frame buffer with a blue color
    Xil_DCacheDisable();

	//Write grey bar pattern in memory
#ifdef PATTERN_GREY
    for(line=0; line < HEIGHT; line++)
    	{
    		for(column=0; column < WIDTH; column++)
    		{

    			if(column<WIDTH/8)
    			{
    				Xil_Out32(Addr1, 0x00e0e0e0);
    			}
    			else if(column<2*WIDTH/8)
    			{
    				Xil_Out32(Addr1, 0x00c0c0c0);
    			}
    			else if(column<3*WIDTH/8)
    			{
    				Xil_Out32(Addr1, 0x00a0a0a0);
    			}
    			else if(column<4*WIDTH/8)
    			{
    				Xil_Out32(Addr1, 0x00808080);
    			}
    			else if(column<5*WIDTH/8)
    			{
    				Xil_Out32(Addr1, 0x00606060);
    			}
    			else if(column<6*WIDTH/8)
    			{
    				Xil_Out32(Addr1, 0x00404040);
    			}
    			else if(column<7*WIDTH/8)
    			{
    				Xil_Out32(Addr1, 0x00202020);
    			}
    			else if(column<WIDTH-5)
    			{
    				Xil_Out32(Addr1, 0);
    			}
    			else
    			{
    				Xil_Out32(Addr1, 0);
    			}

    			Addr1+=4;
    		}

        }
#else
	#ifdef PATTERN_COL_BARS
    	//Write a color bar pattern in memory
    	for(line=0; line < HEIGHT; line++)
    	{
    		for(column=0; column < WIDTH; column++)
    		{

    			if(column<WIDTH/8)
    			{
    				Xil_Out32(Addr1, RED);
    			}
    			else if(column<2*WIDTH/8)
    			{
    				Xil_Out32(Addr1, GREEN);
    			}
    			else if(column<3*WIDTH/8)
    			{
    				Xil_Out32(Addr1, BLUE);
    			}
    			else if(column<4*WIDTH/8)
    			{
    				Xil_Out32(Addr1, RED_HALF);
    			}
    			else if(column<5*WIDTH/8)
    			{
    				Xil_Out32(Addr1, GREEN_HALF);
    			}
    			else if(column<6*WIDTH/8)
    			{
    				Xil_Out32(Addr1, BLUE_HALF);
    			}
    			else if(column<7*WIDTH/8)
    			{
    				Xil_Out32(Addr1, RED_EIGHTS);
    			}
    			else if(column<WIDTH-5)
    			{
    				Xil_Out32(Addr1, GREEN_EIGHTS);
    			}
    			else
    			{
    				Xil_Out32(Addr1, 0);
    			}

    			Addr1+=4;
    		}

        }
	#endif
#endif

	OSD osd(WIDTH, HEIGHT,FRAME_BUFFER_ADDR);

	osd.graphic_layer_1.Draw_Box(11,11,630,470,0x20FF40,0,0);
	osd.graphic_layer_1.Draw_Box(12,12,629,469,0x20FF40,0,0);
	osd.graphic_layer_1.Draw_Box(13,13,628,468,0x20FF40,0,0);

	osd.graphic_layer_1.Print_String_Alpha(100, 100, WHITE_24, BLUE_DARK, arialbold_34, "Synopsys");
	osd.graphic_layer_1.Print_String_Alpha(300, 200, BLACK_24, WHITE_24, segoescriptbold_42, "Yes, if...");

	osd.graphic_layer_1.Draw_Box(30,290,540,410,0xFF0000,0,0);
	osd.graphic_layer_1.Draw_Box(29,289,541,411,0xFF0000,0,0);
	osd.graphic_layer_1.Draw_Box(28,288,542,412,0xFF0000,0,0);

	osd.graphic_layer_1.Draw_Rounded_Box(330,340,620,400,15,DARKRED_24,1,0);
	osd.graphic_layer_1.Print_String_Alpha(350, 340, SILVER_24, DARKRED_24, arialbold_34, "Hello");

	osd.graphic_layer_1.Print_String_Alpha(700, 500, WHITE_24, BLUE_DARK, arialbold_34, "Yerevan 2023");


	//Re-enable the data cache
    Xil_DCacheEnable();
    print("Done\n\r");

	/* Start of VDMA Configuration */
    xil_printf("VDMA Setup...");
    readed =  Xil_In32(XPAR_AXI_VDMA_0_BASEADDR + 0x2c);
    /* Configure the Read interface (MM2S)*/
    // MM2S Control Register
    Xil_Out32(XPAR_AXI_VDMA_0_BASEADDR + 0x00, 0x8B);
    // MM2S Start Address 1
    Xil_Out32(XPAR_AXI_VDMA_0_BASEADDR + 0x5C, FRAME_BUFFER_ADDR);
    Xil_Out32(XPAR_AXI_VDMA_0_BASEADDR + 0x60, FRAME_BUFFER_ADDR);
    Xil_Out32(XPAR_AXI_VDMA_0_BASEADDR + 0x64, FRAME_BUFFER_ADDR);
    Xil_Out32(XPAR_AXI_VDMA_0_BASEADDR + 0x68, FRAME_BUFFER_ADDR);

    // MM2S Frame delay / Stride register
    Xil_Out32(XPAR_AXI_VDMA_0_BASEADDR + 0x58, WIDTH*4);
    // MM2S HSIZE register
    Xil_Out32(XPAR_AXI_VDMA_0_BASEADDR + 0x54, WIDTH*4);
    // MM2S VSIZE register
    Xil_Out32(XPAR_AXI_VDMA_0_BASEADDR + 0x50, HEIGHT);


    readed =  Xil_In32(XPAR_AXI_VDMA_0_BASEADDR + 0x5C);
    readed =  Xil_In32(XPAR_AXI_VDMA_0_BASEADDR + 0x58);
    readed =  Xil_In32(XPAR_AXI_VDMA_0_BASEADDR + 0x54);
    readed =  Xil_In32(XPAR_AXI_VDMA_0_BASEADDR + 0x50);

    xil_printf("Done\r\n");


	/* End of VDMA Configuration */

	while(1)
	{
	}


    cleanup_platform();
    return 0;
}




