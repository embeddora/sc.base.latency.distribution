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
 * Abstract
 * ~~~~~~~~
 * Program to create simple PNG file to visualize some of data a graphic
 * representation of which telkls much more than conventional ones - numeric,
 * logical, textual, usw.
 *
 * Howto
 * ~~~~~
 * This is to compile:
 * gcc ./create_png.c -I/usr/local/include -L/usr/local/lib -lpng -o ./create_png
 *
 * This is to this fix standalone <libpng> installation error which results in
 * following error  message: <error while loading shared libraries: libpng16.so.16:
 * cannot open shared object file: No such file or directory">.
 * sudo ldconfig -v
 *
 */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

/* Declaration of <libpng> types and functions */
#include <png.h>

/* Picture height */
#define _HEIGHT			250

/* Picture width */
#define _WIDTH			260

/* Amount of bytes needed to represent a CLOR/OPCTY vbalue */
#define DEFAULT_BIT_DEPTH	8

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
        exit;
}

/* Create an image in Portable Network Graphics format */
void CreatePng(char* file_name)
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

			ptr[0] = ptr[1] = ptr[2] = 0; ptr[3] = 255;

			if (iX == iY)
			{
				if (iX < 64)
					ptr[0] = (unsigned char)iX;
				else if (iX < 2*64)
					ptr[1] = (unsigned char)iX;
				else
					ptr[2] = (unsigned char)iX;
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

} /* CreatePng */

/* Sample to */
int main(int argc, char **argv)
{
	/* Command line check */
        if (argc != 2)

		/* Leave (not silently) if command line was not composed correctly */
                terminate("Usage: program_name <out_file>");

	/* Create PNG file named <argv[1]> */
        CreatePng(argv[1]);

	/* Exit with no error */
        return 0;

} /* int main( . . . ) */

