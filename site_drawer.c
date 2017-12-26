/*
/*
 * Copyright (c) 2018 [n/a] info@embeddora.com All rights reserved
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *        * Redistributions of source code must retain the above copyright
 *          notice, this list of conditions and the following disclaimer.
 *        * Redistributions in binary form must reproduce the above copyright
 *          notice, this list of conditions and the following disclaimer in the
 *          documentation and/or other materials provided with the distribution.
 *        * Neither the name of The Linux Foundation nor
 *          the names of its contributors may be used to endorse or promote
 *          products derived from this software without specific prior written
 *          permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NON-INFRINGEMENT ARE DISCLAIMED.    IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Hint
 * ~~~~
 * Do the <sudo ldconfig -v> to fix standalone <libpng> installation error which
 * results in following error  message: <error while loading shared libraries:
 * libpng16.so.16: cannot open shared object file: No such file or directory">.
 *
 */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

/* Declaration of <libpng> types and functions */
#include <png.h>

/* Own interface, some defines, etc */
#include "site_drawer.h"

/* Put the complex string on STDERR and exit the program */
void terminate(const char * s, ...)
{
/* Variadic list placeholder */
va_list args;

	/* Start parsing arguments */
        va_start(args, s);

	/* Put on STDERR (attention!) all of them */
        fprintf(stderr, s, args);

	/* Put on newline on STDERR (attention!) */
        fprintf(stderr, "\n");

	/* Finish parsing arguments */
        va_end(args);

	/* Exit the program */
        abort();
}

/* Create an PNG image named <file_name> of size <_WIDTH x _HEIGHT> according to mask <cpMask> */
void CreateMaskPng(char* file_name, unsigned short _WIDTH, unsigned short  _HEIGHT, char * cpMask)
{
/* Array counters in raster graphics are usually coordinates, as well */
int iX, iY;

/* Main PNG structure */
png_structp png_ptr;

/* Info PGN structure */
png_infop info_ptr;

/* Array to inclure pointers onto PNG imnage rows */
png_bytep * ppbImageRows;


        /* Create output file */
        FILE *fp = fopen(file_name, "wb");

	/* Check if it has been created */
        if (NULL == fp)

		/* and leave (not silently) if not */
                terminate("[write_png_file] File %s could not be opened for writing", file_name);

        /* Initialize internal PNG main/write structure */
        png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

	/* Check if it has been initialized */
        if (!png_ptr)

		/* and leave (not silently) if not */
                terminate("[write_png_file] png_create_write_struct failed");

	/* Initialize internal PNG info structure */
        info_ptr = png_create_info_struct(png_ptr);

	/* Check if was initialized */
        if (!info_ptr)

		/* and leave (not silently) if not */
                terminate("[write_png_file] png_create_info_struct failed");

	/* Create array of pointers to point onto image rows */
	ppbImageRows = (png_bytep*) malloc(sizeof(png_bytep) * _HEIGHT);

	/* For each image row */ 
	for (iY=0; iY<_HEIGHT; iY++)

		/* allocate space to include RGBA information: [0] - Red, [1] - Green, [2] - Blue, [3] - opacity */
		ppbImageRows[iY] = (png_byte*) malloc( _WIDTH * 4);

	/* Try to assiciate buffer with internal PNG structure */
        if (setjmp(png_jmpbuf(png_ptr)))

		/* leave (not silently) if not successful */
                terminate("[write_png_file] Error during init_io");

	/* Initialize the output/write function (and other IO functions) with PNG file */
        png_init_io(png_ptr, fp);

        /* Try to write header of PNG  */
        if (setjmp(png_jmpbuf(png_ptr)))

		/* leave (not silently) if not successful */
                terminate("[write_png_file] Error during writing header");

	/* Copy basic PNG parameters into <info_ptr> */
        png_set_IHDR(png_ptr, info_ptr,
		_WIDTH, _HEIGHT, DEFAULT_BIT_DEPTH, PNG_COLOR_TYPE_RGBA,
		PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

	/* Write info structure <info_ptr> into main PNG structure */
        png_write_info(png_ptr, info_ptr);

        /* Try to write bytes */
        if (setjmp(png_jmpbuf(png_ptr)))

		/* leave (not silently) if not successful */
                terminate("[write_png_file] Error during writing bytes");

	/* Make an image in the memory. Sorry for I'm not gifted in explaning very simple things */
        for (iY=0; iY<_HEIGHT; iY++)
	{
        png_byte* row = ppbImageRows[iY];

	        for (iX=0; iX<_WIDTH; iX++)	
		{
		png_byte* ptr = &(row[iX*4]);

			/* Seventy seven is easteregg, in fact any different value is equally relevant */
			ptr[0] = ptr[1] = ptr[2] = 0x77;

			/* No transparency, no opacity */
			ptr[3] = 255;

			/* 'Debug' colored diagonal just ensure the videomemory geometry is as expected */
			if (iX == iY)
			{
				/* Diagonal is composed of 3 fragments */
				if (iX < 64)

					/* Dark colours lie in lowest quarted of 0x100 */
					ptr[0] = (unsigned char)iX;

				else if (iX < 2*64)

					/* Lighter colours lie in second quarted of 0x100 */
					ptr[1] = (unsigned char)iX;

				else
					/* More Lighter colours lie in third quarted of 0x100 */
					ptr[2] = (unsigned char)iX;
			}

			/* Once current pixel is marked in the <cpMask> as pixel-to-draw */
			if ( 1 == cpMask[ iY*_WIDTH + iX ] )

				/* Put red peak in this pixel */
				ptr[0] = 255;

		}

	}


	/*  */
        for (iY=0; iY<_HEIGHT; iY++)
	{
	int iT;
                png_byte* row = ppbImageRows[iY];

	        for (iX=0; iX<_WIDTH; iX++)	
		{
			png_byte* ptr = &(row[iX*4]);

			/* Once current pixel is marked in the <cpMask> as pixel-to-draw */
			if ( 1 == cpMask[ iY*_WIDTH + iX ] )
			{
				/* Put red peak in this pixel */
				ptr[0] = 255;

				/* Draw green stalk under red peak */
				for (iT = iY; iT < _HEIGHT-1; ++iT)
				{
				png_byte* rowT;

					/* */
					rowT = ppbImageRows[iT];

					/* */
					ptr = &(rowT[iX*4]);

					/* Put pixel into stalk */
					ptr[1] = 255;

				}
			}
		}
	}

	/* Write pucture into struct <png_ptr> */
        png_write_image(png_ptr, ppbImageRows);

        /* Try to finalize writing */
        if (setjmp(png_jmpbuf(png_ptr)))

		/* leave (not silently) if not successful */
                terminate("[write_png_file] Error during end of write");

	/* Complete the file writing */
        png_write_end(png_ptr, NULL);

        /* Cleanup heap .. */
        for (iY=0; iY<_HEIGHT; iY++)

		/*  .. allocated for each image row */
                free(ppbImageRows[iY]);

	/* Cleanup heap occupied by array of pointers onto image rows */
        free(ppbImageRows);

	/* PGN file is ready */
        fclose(fp);

} /* CreateMaskPng */

/* Create stretched by factor <iCoeff> PNG image named <file_name> of size <_WIDTH x _HEIGHT> according to mask <cpMask> */
void CreateMaskPngCoeff(char* file_name, unsigned short _WIDTH, unsigned short  _HEIGHT, char * cpMask, int iCoeff)
{
/* Array counters in raster graphics are usually coordinates, as well */
int iX, iY;

/* Zwischenstoff, eigentlich eben die koordinaten */
int iYt, iXr, iXq;

/* Main PNG structure */
png_structp png_ptr;

/* Info PGN structure */
png_infop info_ptr;

/* Array to inclure pointers onto PNG imnage rows */
png_bytep * ppbImageRows;

        /* Create output file */
        FILE *fp = fopen(file_name, "wb");

	/* Check if it has been created */
        if (NULL == fp)

		/* and leave (not silently) if not */
                terminate("[write_png_file] File %s could not be opened for writing", file_name);

        /* Initialize internal PNG main/write structure */
        png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

	/* Check if it has been initialized */
        if (!png_ptr)

		/* and leave (not silently) if not */
                terminate("[write_png_file] png_create_write_struct failed");

	/* Initialize internal PNG info structure */
        info_ptr = png_create_info_struct(png_ptr);

	/* Check if was initialized */
        if (!info_ptr)

		/* and leave (not silently) if not */
                terminate("[write_png_file] png_create_info_struct failed");

	/* Create array of pointers to point onto image rows */
	ppbImageRows = (png_bytep*) malloc(sizeof(png_bytep) * _HEIGHT);

	/* For each image row */ 
	for (iY=0; iY<_HEIGHT; iY++)

		/* allocate space to include RGBA information: [0] - Red, [1] - Green, [2] - Blue, [3] - opacity */
		ppbImageRows[iY] = (png_byte*) malloc( _WIDTH * 4 * iCoeff);

	/* Try to assiciate buffer with internal PNG structure */
        if (setjmp(png_jmpbuf(png_ptr)))

		/* leave (not silently) if not successful */
                terminate("[write_png_file] Error during init_io");

	/* Initialize the output/write function (and other IO functions) with PNG file */
        png_init_io(png_ptr, fp);

        /* Try to write header of PNG  */
        if (setjmp(png_jmpbuf(png_ptr)))

		/* leave (not silently) if not successful */
                terminate("[write_png_file] Error during writing header");

	/* Copy basic PNG parameters into <info_ptr> */
        png_set_IHDR(png_ptr, info_ptr,
		_WIDTH * iCoeff, _HEIGHT, DEFAULT_BIT_DEPTH, PNG_COLOR_TYPE_RGBA,
		PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

	/* Write info structure <info_ptr> into main PNG structure */
        png_write_info(png_ptr, info_ptr);

        /* Try to write bytes */
        if (setjmp(png_jmpbuf(png_ptr)))

		/* leave (not silently) if not successful */
                terminate("[write_png_file] Error during writing bytes");


	/* Make an image in the memory. Sorry for I'm not gifted in explaning very simple things */
        for (iY=0; iY<_HEIGHT; iY++)
	{
                png_byte* row = ppbImageRows[iY];

	        for (iXr=0; iXr<_WIDTH; iXr++)	
		{
			for (iXq=0; iXq<iCoeff; iXq++)	
			{
				png_byte* ptr = &(row[(iXr*iCoeff + iXq)*4]);

				/* Background colour - black */
				ptr[0] = ptr[1] = ptr[2] = 0;

				/* No transparency, no opacity */
				ptr[3] = 255;

				/* Once current pixel is marked in the <cpMask> as pixel-to-draw */
				if ( 1 == cpMask[ iY*_WIDTH + iXr ] )

					/* Put red peak in this pixel */
					ptr[0] = 255;
			}
		}
	}

/* It's a Hack. And it can be reworked, for optimization concrns at least */
#if (1)
	/* Hack. Spoil the <cpMask>. */
	for (iY=0; iY<_HEIGHT; iY++)
	        for (iX=0; iX<_WIDTH; iX++)	
			if ( 1 == cpMask[ iY*_WIDTH + iX ] )
				for (iYt=iY; iYt<_HEIGHT-1; ++iYt)
					cpMask[ iYt*_WIDTH + iX ] = 1;

	/* Hack. Draw the red stuff which patially will be overdrawn by green stuff */
        for (iY=0; iY<_HEIGHT; iY++)
	{
                png_byte* row = ppbImageRows[iY];

	        for (iXr=0; iXr<_WIDTH; iXr++)	
		{
			for (iXq=0; iXq<iCoeff; iXq++)	
			{
				png_byte* ptr = &(row[(iXr*iCoeff + iXq)*4]);

				/* Once current pixel is marked in the <cpMask> as pixel-to-draw */
				if ( 1 == cpMask[ iY*_WIDTH + iXr ] )
				{
					/* Put red peak in this pixel */
					ptr[0] = 255;
					ptr[1] = 0;
					ptr[2] = 0;
				}
			}
		}
	}

	/* Hack. Draw the green stuff - overdraw the read stuff except of PEAK_HEIGHT-high peaks */
        for (iY=PEAK_HEIGHT; iY<_HEIGHT-1; iY++)
	{
                png_byte* row = ppbImageRows[iY];

	        for (iXr=0; iXr<_WIDTH; iXr++)	
		{
			for (iXq=0; iXq<iCoeff; iXq++)	
			{
				png_byte* ptr = &(row[(iXr*iCoeff + iXq)*4]);

				/* Once current pixel is marked in the <cpMask> as pixel-to-draw */
				if ( 1 == cpMask[ (iY-PEAK_HEIGHT)*_WIDTH + iXr ] )
				{
					/* Put green peak in this pixel */
					ptr[0] = 0;
					ptr[1] = 255;
					ptr[2] = 0;
				}
			}
		}
	}
#endif /* (0) */

	/* Yellow stripe - those don't find in the image */
	for (iX=0; iX<_WIDTH; iX++)

		if ( 2 == cpMask[ iX ] )
		{
		int iT;

			for (iT = 0; iT < _HEIGHT; iT++)
			{
			png_byte* rowT = ppbImageRows[iT];

				for (iXq = 0; iXq<iCoeff; iXq++)	
				{
					png_byte* ptr1 = &(rowT[(iX*iCoeff + iXq)*4]);

					/* Put yellow pixel */
					ptr1[0] = 255; ptr1[1] = 255; ptr1[2] = 0;

				}
			}
		}

	/* Yellow stripe - those HTTP-requests which 'reproduce' the TMO bug */
	for (iX=0; iX<_WIDTH; iX++)

		if ( 3 == cpMask[ iX ] )
		{
		int iT;
			for (iT = 0; iT < _HEIGHT; iT++)
			{
			png_byte* rowT = ppbImageRows[iT];

				for (iXq = 0; iXq<iCoeff; iXq++)	
				{
					png_byte* ptr1 = &(rowT[(iX*iCoeff + iXq)*4]);

					/* Put white pixel */
					ptr1[0] = 255; ptr1[1] = 255; ptr1[2] = 255;
				}
			}
		}
		

	/* Write pucture into struct <png_ptr> */
        png_write_image(png_ptr, ppbImageRows);

        /* TYry to finalize writing */
        if (setjmp(png_jmpbuf(png_ptr)))

		/* leave (not silently) if not successful */
                terminate("[write_png_file] Error during end of write");

	/* Complete the file writing */
        png_write_end(png_ptr, NULL);

        /* Cleanup heap .. */
        for (iY=0; iY<_HEIGHT; iY++)

		/*  .. allocated for each image row */
                free(ppbImageRows[iY]);

	/* Cleanup heap occupied by array of pointers onto image rows */
        free(ppbImageRows);

	/* PGN file is ready */
        fclose(fp);

} /* CreateMaskPngCoeff */
