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
 */

#ifndef _SITE_DRAWER_H_
#define _SITE_DRAWER_H_

/* Amount of bytes needed to represent a CLOR/OPCTY vbalue */
#define DEFAULT_BIT_DEPTH	8

/* Image width to show single timepoint. */
#define OX_COEFF		20

/* To make up[per end of vertical line visible */
#define PEAK_HEIGHT		10

/* 2.3s - max imageallowed 'height' of value to be drawn. Those above will be casted. */
#define MAX_VERT_SIZE		 2300 // do 2170 for action 

/* See <GAUGE_HTTP_THRESHOLD> in <site_crawler.rlda.h> */
#define REPRODUCED_SIZE		29500

/* PNG image height. Acually PEAK_HEIGHT is not strictly needed */
#define PNG_HEIGHT		(MAX_VERT_SIZE + PEAK_HEIGHT)


/* Put the complex string on STDERR and exit the program */
void terminate(const char * s, ...);

/* Create an PNG image named <file_name> of size <_WIDTH x _HEIGHT> according to mask <cpMask> */
void CreateMaskPng(char* file_name, unsigned short _WIDTH, unsigned short  _HEIGHT, char * cpMask);

/* Create stretched by factor <Coeff> PNG image named <file_name> of size <_WIDTH x _HEIGHT> according to mask <cpMask> */
void CreateMaskPngCoeff(char* file_name, unsigned short _WIDTH, unsigned short  _HEIGHT, char * cpMask, int Coeff);

#endif /* _SITE_DRAWER_H_ */
