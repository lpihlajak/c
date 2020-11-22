#include <dos.h>
#include <stdlib.h>

#define WIDTH 320
#define HEIGHT 200
#define TEXSIZE 32
#define PI 3.1415926

#include <math.h>

float distance(float x, float y)
{
  return sqrt(x*x + y*y);
}

void main()
{
    char texture[1024];

    typedef unsigned char byte;
    byte far *VGA = (byte far*)0xA0000000L;
    unsigned short texoffset = 0;
    unsigned short vgaoffset;
    unsigned short lutoffset;
    int x;
    int y;
    int u;
    int v;

    int counter1 = 0;

    byte a;

    byte r;

    unsigned char *atanLUT;
    byte *distLUT;
    unsigned char *double_buffer;

    byte color;

    union REGS regs;
    regs.h.ah = 0x00;
    regs.h.al = 0x13;
    int86(0x10,&regs,&regs);

    printf("\nprecalc\n");
/*DOUBLEBUFFER*/
double_buffer = (unsigned char *) malloc(320*200);
if (double_buffer==NULL)
{
  printf("Not enough memory for double buffer.\n");
  exit(1);
}
atanLUT = (byte *) malloc(320*200);
if(atanLUT==NULL)
{
   printf("No memory for LUT"); exit(1);
}
distLUT = (byte *) malloc(WIDTH*HEIGHT);
if(distLUT==NULL)
{
  printf("No memory for distLUT"); exit(1);
}

    for(x=0;x<WIDTH;x++) {
      for(y=0;y<HEIGHT;y++) {

    u = x-WIDTH/2;
    v = y-HEIGHT/2;

    lutoffset = WIDTH*y + x;
    atanLUT[lutoffset] = (byte)(128 * (atan2((float)(y - HEIGHT / 2), (float)(x - WIDTH / 2)) / PI + 1.0));
    distLUT[lutoffset] = (byte)distance(u,v);
      }
    }
    printf("\n.. lut done");

    for(x=0;x<TEXSIZE;x++) {
       for(y=0;y<TEXSIZE;y++) {
       color = x^y;
       texoffset = TEXSIZE*y + x;
       texture[texoffset] = color;
       }
   }
   printf("\n.. texture done");

while(counter1 < 240)
{
   texoffset = 0;
   for(x=0;x<WIDTH;x++) {
     for(y=0;y<HEIGHT;y++) {
    vgaoffset = WIDTH*y+x;

    r = distLUT[vgaoffset];
    a = atanLUT[vgaoffset];

    u = a;
    v = r+counter1;

    u = u%TEXSIZE;
    v = v%TEXSIZE;

    texoffset = v * TEXSIZE + u;
    double_buffer[vgaoffset] = texture[texoffset];

     }
   }

while ((inp(0x3DA) & 0x08));
while (!(inp(0x3DA) & 0x08));
memcpy(VGA,double_buffer,320*200);

   counter1++;

}



}


