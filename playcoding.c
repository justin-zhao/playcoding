#include <stdlib.h>
#include <vga.h>
 
 int main(void)
 {
    vga_init();
    vga_setmode(G1024x768x256);
	vga_clear();

    vga_setcolor(4);
	vga_drawpixel(20,10);
	vga_drawline(10,10,100,100);
               
	vga_getch();
    vga_setmode(TEXT);

    return EXIT_SUCCESS;
 }
