/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                 //
//                     _               _           _                                          _                                    //
//   _ __ ___    __ _ | | __ ___      | |  ___  __| |      ___   __ _ __   __ ___  _ __      (_) _ __    ___   __ _  ___     ___   //
//  | '_ ` _ \  / _` || |/ // _ \     | | / __|/ _` |     / __| / _` |\ \ / // _ \| '__|     | || '_ \  / _ \ / _` |/ __|   / __|  //
//  | | | | | || (_| ||   <|  __/     | || (__| (_| |     \__ \| (_| | \ V /|  __/| |        | || |_) ||  __/| (_| |\__ \ _| (__   //
//  |_| |_| |_| \__,_||_|\_\\___|_____|_| \___|\__,_|_____|___/ \__,_|  \_/  \___||_|_____  _/ || .__/  \___| \__, ||___/(_)\___|  //
//                              |_____|             |_____|                         |_____||__/ |_|           |___/                //
//                                                                                                                                 //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                 //
//                                     Copyright (c) 2020 by S.F.T. Inc. - All rights reserved                                     //
//                              Use, copying, and distribution of this software are licensed according                             //
//                                    to the GPLv2, LGPLv2, or BSD-like license, as appropriate.                                   //
//                            (just give me at least some credit for having written the thing, k-thanks)                           //
//                                                                                                                                 //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// What this is:  It is a simple program that creates a pile of sequenced jpeg files in a directory
//                'jpeg_output' off of the current directory.  You'll have to check for errors and
//                clean out directories yourself.
//
// The JPEG files, when turned into a movie file at 8 frames per second, create an "LCD Exercise" video
// that SHOULD help to repair pixels that are 'burned' or 'stuck'.  There are no guarantees, but the
// principle is very similar to what's already "out there" (including an X11 screen saver).
//
// The theory is that a charge is accumulating between LCD pixels, and when monitors stay on for long
// periods of time, it can cause the equivalent of "screen burn".  Turning off the monitor, or allowing
// it to go into "blank" mode, helps prevent this.  But over time you'll see ghosty images and bright or
// dark lines where pixels ought not to be doing that.
//
// You will need to create a video that matches the resolution of your monitor, which is why a "universal"
// video isn't going to cut it.  Sure it may "work" but not as well.
//
// So let's say you have a Raspberry Pi and the touch screen (800x480 in this case) has a bad burn mark on
// it because it's been used for a kiosk for a year of continuous operation.  So you run this video a few
// times to try and correct it.  [you probably do have to rinse/repeat with it].  Then most of the ghostiness
// flattens out and you can continue using the screen for a while longer.
//
// To create that video you would probably do this:
// a) build this program
// b) run:  ./make_lcd_saver_jpegs 800 480     <-- makes the video 800x480 pixels in size
// c) run:  mencoder "mf://jpeg_output/*" -idx -ovc x264 -nosound -mf type=jpeg:fps=8 -o saver.avi
//    (this will create th video 'saver.avi' from the jpegs)
// d) rm -rf jpeg_output    (to get rid of all of the JPG files, there are thousands!)
//
// Then, run 'saver.avi' in fullscreen mode using your favorite video player.  Loop it for a few hours for best results.


// NOTE:  the cc command to build the program may need to have
//        '-I/usr/local/include -L/usr/local/lib' and '-ljpeg' on the command line, i.e.
//
//   cc -o make_lcd_saver_jpegs make_lcd_saver_jpegs.c -ljpeg
//  - or -
//   cc -o make_lcd_saver_jpegs make_lcd_saver_jpegs.c -I/usr/local/include -L/usr/local/lib -ljpeg
//
// Your Mileage May Vary depending on whether you build for FreeBSD, Linux, Cygwin, Mac, etc.
//
// A lot of Linux distributions will require you to load a 'developer' package for libjpeg, something like
//   (on debian-based systems)  apt-get install libjpeg-dev
//
// typical command to create video
//   mencoder "mf://jpeg_output/*" -idx -ovc x264 -nosound -mf type=jpeg:fps=8 -o saver.avi



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>

#include <jpeglib.h>


void make_jpeg_mono(unsigned char *pBits, int nWidth, int nHeight, const char *szFileName);
void make_jpeg_RGB(unsigned char *pBits, int nWidth, int nHeight, const char *szFileName);


int main(int argc, char *argv[])
{
int iR, iC, ii, iFrame, iScene, iWidth, iHeight;
unsigned char *pBuf;
char fn[256];

  if(argc < 2)
  {
    fprintf(stderr, "usage:  xxxx\n");
    return 1;
  }

  if(argc >= 2)
    iWidth = atoi(argv[1]);

  if(argc >= 3)
    iHeight = atoi(argv[2]);
  else
    iHeight = iWidth;

  pBuf = malloc(iWidth * iHeight * 3);
  if(!pBuf)
  {
    fprintf(stderr, "Unable to alloc buffer, errno=%d\n", errno);
    return 2;
  }

  mkdir("jpeg_output", 0777);

  for(iScene=0; iScene < 13; iScene++)
  {
    // frames 0 through 960 - 8 frames per second, 2 minutes [120 seconds]
    // with a total of 13 "scenes", it's a 26 minute video
    for(iFrame=0; iFrame < 960; iFrame++)
    {
      int iMod = iFrame % 8;
      int iMod5 = ((iScene - 8) + (iFrame / 128)) % 5; // flip colors every ~16 sec

      snprintf(fn, sizeof(fn), "jpeg_output/frame%06d.jpg\n", iFrame + iScene * 960);

      memset(pBuf, 0, iHeight * iWidth);

      if(iScene == 0 || iScene == 1 || iScene == 4 || iScene == 5)
      {
        // horizontal and vertical
        int iB=0, iW=255; // white on black

        if(iScene == 4 || iScene == 5)
        {
          // black on white
          iB=255;
          iW=0;
        }

        for(iR=0; iR < iHeight; iR++)
        {
          unsigned char *pR = pBuf + iR * iWidth;

          for(iC=0; iC < iWidth; iC++)
          {
            if(iScene == 0 || iScene == 4)
            {
              // horizontal lines
              if((iR % 8) == iMod)
                pR[iC] = iW;
              else
                pR[iC] = iB;
            }
            else if(iScene == 1 || iScene == 5)
            {
              // vertical lines
              if((iC % 8) == iMod)
                pR[iC] = iW;
              else
                pR[iC] = iB;
            }
          }
        }
      }
      else if(iScene == 2 || iScene == 6)
      {
        // uphill diagonal

        int iB=0, iW=255;

        if(iScene == 6)
        {
          iB=255;
          iW=0;
        }

        for(iR=0; iR < iHeight; iR++)
        {
          for(iC=0; iC < iR; iC++)
          {
            unsigned char *pR;

            ii = iR - iC; // the row I'll be "poking" too
            if(ii < 0)
              break; // done (it went negative)

            pR = pBuf + ii * iWidth;

            if((iR % 8) == iMod)
              pR[iC] = iW;
            else
              pR[iC] = iB;
          }
        }

        // we're on the bottom row so now I scan by column

        for(iC=0; iC < iWidth; iC++)
        {
          for(iR = iHeight - 1; iR >= 0; iR--)
          {
            unsigned char *pR = pBuf + iR * iWidth;

            ii = iC + ((iHeight - 1) - iR); // the column I'll be "poking" too

            if(ii < 0 || ii >= iWidth) // over the edge??
              continue; // skip it

            if(((iHeight - 1 + iC) % 8) == iMod)
              pR[ii] = iW;
            else
              pR[ii] = iB;
          }
        }
      }
      else if(iScene == 3 || iScene == 7)
      {
        // downhill diagonal

        int iB=0, iW=255;

        if(iScene == 7)
        {
          iB=255;
          iW=0;
        }

        for(iR=0; iR < iHeight; iR++)
        {
          for(iC=0; iC < iWidth; iC++)
          {
            unsigned char *pR;

            ii = iR + iC; // the row I'll be "poking" too
            if(ii < 0 || ii >= iWidth)
              continue; // skip (out of bounds)

            pR = pBuf + ii * iWidth;

            if(((iHeight - 1 - iR) % 8) == iMod)
              pR[iC] = iW;
            else
              pR[iC] = iB;
          }
        }

        // we're on the bottom row so now I scan by column
        for(iC=0; iC < iWidth; iC++)
        {
          for(iR=0; iR < iHeight; iR++)
          {
            unsigned char *pR = pBuf + iR * iWidth;

            ii = iC + iR; // the column I'll be "poking" too

            if(ii < 0 || ii >= iWidth) // over the edge??
              continue; // skip it

            if(((iHeight - 1 + iC) % 8) == iMod)
              pR[ii] = iW;
            else
              pR[ii] = iB;
          }
        }
      }
      else if(iScene >= 8 && iScene < 13)
      {
        // solid color

        for(iR=0; iR < iHeight; iR++)
        {
          int ii;
          unsigned char *pR = pBuf + iR * iWidth * 3;

          for(ii=0, iC=0; iC < iWidth; iC++, ii += 3)
          {
            if(iMod5 == 0)
            {
              pR[ii] = 255;
              pR[ii + 1] = 0;
              pR[ii + 2] = 0;
            }
            else if(iMod5 == 1)
            {
              pR[ii] = 0;
              pR[ii + 1] = 255;
              pR[ii + 2] = 0;
            }
            else if(iMod5 == 2)
            {
              pR[ii] = 0;
              pR[ii + 1] = 0;
              pR[ii + 2] = 255;
            }
            else if(iMod5 == 3)
            {
              pR[ii] = 0;
              pR[ii + 1] = 0;
              pR[ii + 2] = 0;
            }
            else if(iMod5 == 4)
            {
              pR[ii] = 255;
              pR[ii + 1] = 255;
              pR[ii + 2] = 255;
            }
          }
        }
      }

      if(iScene >=0 && iScene <= 7) // B&W images
        make_jpeg_mono(pBuf, iWidth, iHeight, fn);
      else                          // 24-bit RGB color images
        make_jpeg_RGB(pBuf, iWidth, iHeight, fn);

      fprintf(stderr, "%02d: %d\r", iScene, iFrame); // progress
    }

    fputs("\n", stderr);
  }


  free(pBuf);

  return 0;
}


void make_jpeg_RGB(unsigned char *pBits, int iWidth, int iHeight, const char *szFileName)
{
struct jpeg_compress_struct cinfo;
struct jpeg_error_mgr jerr;
FILE *pOut;

      cinfo.err = jpeg_std_error(&jerr);
      jpeg_create_compress(&cinfo);

      cinfo.image_height = iHeight;
      cinfo.image_width = iWidth;
      cinfo.input_components = 3;
      cinfo.in_color_space = JCS_RGB; // 3 bytes per pixel

      jpeg_set_defaults(&cinfo);

      pOut = fopen(szFileName, "w");

      if(pOut)
      {
        jpeg_stdio_dest(&cinfo, pOut);

        jpeg_set_quality(&cinfo, 75, FALSE); // default quality of 75
        jpeg_start_compress(&cinfo, TRUE);

        while(/*!bThreadQuitFlag &&*/ cinfo.next_scanline < cinfo.image_height)
        {
          JSAMPROW aRow[1];

          aRow[0] = (JSAMPROW)&(pBits[cinfo.next_scanline * 3 * iWidth]);
          jpeg_write_scanlines(&cinfo, aRow, 1);
        }

//        if(error)
//        {
//          jpeg_abort_compress(&cinfo);
//        }
//        else
//        {
        jpeg_finish_compress(&cinfo);
//        }

        fclose(pOut);
      }

      jpeg_destroy_compress(&cinfo);

}

void make_jpeg_mono(unsigned char *pBits, int iWidth, int iHeight, const char *szFileName)
{
struct jpeg_compress_struct cinfo;
struct jpeg_error_mgr jerr;
FILE *pOut;

      cinfo.err = jpeg_std_error(&jerr);
      jpeg_create_compress(&cinfo);

      cinfo.image_height = iHeight;
      cinfo.image_width = iWidth;
      cinfo.input_components = 1;
      cinfo.in_color_space = JCS_GRAYSCALE; // 1 byte per pixel

      jpeg_set_defaults(&cinfo);

      pOut = fopen(szFileName, "w");

      if(pOut)
      {
        jpeg_stdio_dest(&cinfo, pOut);

        jpeg_set_quality(&cinfo, 75, FALSE); // default quality of 75
        jpeg_start_compress(&cinfo, TRUE);

        while(/*!bThreadQuitFlag &&*/ cinfo.next_scanline < cinfo.image_height)
        {
          JSAMPROW aRow[1];

          aRow[0] = (JSAMPROW)&(pBits[cinfo.next_scanline * iWidth]);
          jpeg_write_scanlines(&cinfo, aRow, 1);
        }

//        if(error)
//        {
//          jpeg_abort_compress(&cinfo);
//        }
//        else
//        {
        jpeg_finish_compress(&cinfo);
//        }

        fclose(pOut);
      }

      jpeg_destroy_compress(&cinfo);

}


