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

#ifndef _STRUCTS_H_
#define _STRUCTS_H_

/* General purpose output */
#define DGENERAL(format, ...) fprintf (stdout, format, __VA_ARGS__)

/* Verbose XML-related stuff. TODO: delme!!! */
#define DXML(format, ...) fprintf (stdout, format, __VA_ARGS__)

/* Verbose single binded list related stuff: records of this list and how they're being processed */
#define DSTRUCT(format, ...) fprintf (stdout, format, __VA_ARGS__)



/* Structure to keep HTTP-request identifier (pointer to <pcAddrStr> field of <struct _GP>) and structures with this request' send- and receive times */
typedef struct _DtaStructType
{
	/* A HTTP-requst as string */
	char * cpHttpRequest;

	/* Structures to keep value of time when request has been issued. TODO: get rid of */
	struct timeval tmHttpRequest;

	/* Structures to keep value of time when responce has issued. TODO: get rid of */
	struct timeval tmHttpResponce;

	/* Previous operation responce interval */
	double dbOpRespInt;

	/* Group identifier to tell which WebUI procedure has been executed by this HTTP-request */


	/* Time (corr.: diff. time) transformed to form approproate for visualizing */

} DtaStructType, *pDtaStructType;


/* Single binded list */
typedef struct _TgtStructType
{
	/* Structure with effective stuff */
	struct _DtaStructType * pDta;

	/* A pointer to a successor */
	struct _TgtStructType * pNext;

	/* A pointer to a parent */
	struct _TgtStructType * pPrev;

	/* Amount of points to visualize on <Ox> - HTTP-requests plus service data */
	int iExAxisLength;

	/* Maximal value in the chain */
	double dbMax;

	/* Amount of points to visualize on <Oy> - depends on Maximal element in the chain */
	int iMax;

	/* Minimal element in the whole chain. TODO: get rid of */
	int iMin;

} TgtStructType, *pTgtStructType;


/* Append an entry in signle binded list or create it (once does not exist)  */
void _AddTarget(pTgtStructType * ppbThisTarget, pDtaStructType pDta);

/* Deploy information of an entire signle binded list */
void _ProcessTargets(pTgtStructType pbThisTarget, char * pcPool);

/* Erase an entire signle binded list */
void _DeleteTarget(pTgtStructType pbThisTarget);

/* Of two scalars regardless float integer, byte or word take a minimal one */
#define min(a,b) ((a<b)?a:b)

/* Of two scalars regardless float integer, byte or word take a maximal one */
#define max(a,b) ((a>b)?a:b)


#endif /* _STRUCTS_H_ */
