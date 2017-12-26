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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Own interface */
#include "structs.h"

/* Macro <MAX_VERT_SIZE> */
#include "site_drawer.h"

/* Append an entry in signle binded list or create it (once does not exist)  */
void _AddTarget(pTgtStructType * ppbThisTarget, pDtaStructType pDta)
{
	/* If heading element of the struct */
	if (NULL == *ppbThisTarget)
	{
		/* Create only one chain, for breginning */
		*ppbThisTarget = (pTgtStructType) malloc ( sizeof (TgtStructType) );

		/* Check if successful */
		if (NULL == *ppbThisTarget)
		{
			DSTRUCT("[%s] %s: ERROR: failure on creation \n", __FILE__, __func__);
		}

		if (NULL != pDta)
		{
			(*ppbThisTarget)->pDta = (pDtaStructType) malloc( sizeof (struct _DtaStructType) );

			if (NULL != (*ppbThisTarget)->pDta)
			{

				(*ppbThisTarget)->pDta->cpHttpRequest = (char*) malloc (strlen (pDta->cpHttpRequest) +1 );
				strcpy((*ppbThisTarget)->pDta->cpHttpRequest, pDta->cpHttpRequest);

				memcpy(&(*ppbThisTarget)->pDta->tmHttpRequest, &pDta->tmHttpRequest, sizeof(struct timeval) );

				memcpy(&(*ppbThisTarget)->pDta->tmHttpResponce, &pDta->tmHttpResponce, sizeof(struct timeval) );
			}
			else
				DSTRUCT("[%s] %s: ERROR: can't allocate mem. while copying initialization data\n", __FILE__, __func__);

		}
		else
			DSTRUCT("[%s] %s: ERROR: bad initialization data\n", __FILE__, __func__);


		/* Initialize. In successors we won't update them. */
		(*ppbThisTarget)->iExAxisLength = 0;

		/* Initialize here. Values in successors vill be ignored. TODO: needed? */
		(*ppbThisTarget)->iMax = 0;

		/* Initialize here. Values in successors vill be ignored. TODO: needed? */
		(*ppbThisTarget)->dbMax = 0.0;

		/* Lock-up  */
		(*ppbThisTarget)->pNext = NULL;
	
	}
	/* Once exixts then append new element on a tail of it */
	else
	{
	/* introduce two temporary variables of type 'pTgtStructType' */
	pTgtStructType pbChild, pbTempTgtStructType;

		/* point with first temporary element to head of Target */
		pbChild = *ppbThisTarget;

		/* allocate a space for new record in Behorde */
		pbTempTgtStructType = (pTgtStructType) malloc ( sizeof (TgtStructType));

		/* Skip everything .. */
		while (NULL != pbChild->pNext )

			/* .. except the tail */
			pbChild = pbChild->pNext;
		
		/* if previous memory allocation was successful */
		if(pbTempTgtStructType != NULL)
		{
			if (NULL != pDta)
			{
				pbTempTgtStructType->pDta = (pDtaStructType) malloc( sizeof (struct _DtaStructType) );

				if (NULL != pbTempTgtStructType->pDta)
				{

					pbTempTgtStructType->pDta->cpHttpRequest = (char*) malloc (strlen (pDta->cpHttpRequest) +1 );
					strcpy(pbTempTgtStructType->pDta->cpHttpRequest, pDta->cpHttpRequest);

					memcpy(&pbTempTgtStructType->pDta->tmHttpRequest, &pDta->tmHttpRequest, sizeof(struct timeval));

					memcpy(&pbTempTgtStructType->pDta->tmHttpResponce, &pDta->tmHttpResponce, sizeof(struct timeval));

					/* Prevent seeing garbage values in the tailing element of the chain */
					pbTempTgtStructType->pDta->dbOpRespInt = 0;

					(*ppbThisTarget)->dbMax = max(pDta->dbOpRespInt, (*ppbThisTarget)->dbMax);

					/* Comment this memcheck - needed, not? */
					if (NULL != pbChild->pDta)
					{
						/* This will be <pbTempTgtStructType->pPrev->pDta->dbOpRespInt, see code below  */
						pbChild->pDta->dbOpRespInt = pDta->dbOpRespInt;

					}  /* So <dbOpRespInt> is being assigned for _previous chain. */
				}
				else
					DSTRUCT("[%s] %s: ERROR: can't allocate mem. while copying initialization data\n",
					 __FILE__, __func__);

			}
			else
				DSTRUCT("[%s] %s: ERROR: bad initialization data\n", __FILE__, __func__);
		
			/* Set a sucessor */
			pbTempTgtStructType->pNext = NULL;

			/* Set a parent */
			pbTempTgtStructType->pPrev = pbChild;

			/* append a new Target entry to the end of existing Target */
			pbChild->pNext = pbTempTgtStructType;
		}
		else
			/* memory for new recored hasn't been allocated */
			return; 
	}
} /* void _AddTarget(pTgtStructType * ppbThisTarget, pDtaStructType pDta) */


/* Deploy information about all current targets */
void _ProcessTargets(pTgtStructType pbThisTarget, char * pbMask)
{
/* a storage for number under which the item is registerd in Targets */
unsigned char _the_number;

/* TODO: add comment */
int iX = 0, iY = MAX_VERT_SIZE/*pbThisTarget->iMax*/, iAxisLen = pbThisTarget->iExAxisLength;

    /* process each Target's entry */
    while (pbThisTarget != NULL )
    {
#if 0
	DSTRUCT("[%s] %s:  dbOpRespInt<%9.3f>\tcpHttpRequest<%s>  \n", __FILE__, __func__,
		pbThisTarget->pDta->dbOpRespInt/1000, pbThisTarget->pDta->cpHttpRequest   );
#endif /* 0 */

	/* If duration was longer than 29.5 s */
	if (REPRODUCED_SIZE * 1000 < pbThisTarget->pDta->dbOpRespInt)

		/* Make 'reproduce' dots. Idx 3 for white stripe */
		pbMask[iX] = 3;

	/* If duratino was longer than longer than 2.3s and less than 29s  */
	else if (MAX_VERT_SIZE * 1000 < pbThisTarget->pDta->dbOpRespInt)

		/* Make dots those don't fit the picture. Idx 2 for yellow stripe */
		pbMask[iX] = 2;

	/* Regular processing onlyfor those like 0.002s */
	else
		/* Make 'normal' dots. Idx 1 for greep stripe with red peak */
		pbMask[iX + iAxisLen*(iY  - (int) (pbThisTarget->pDta->dbOpRespInt/1000) ) ] = 1;


	/* Go to next record of Target */
	pbThisTarget =  pbThisTarget->pNext;

	/* Prepare to depict next HTTP-request - shift one point on a left */
	iX++;
    };

} /* void _ProcessTargets(pTgtStructType pbThisTarget) */


/* Show in console, useful for debug */
void _DisplayTargets(pTgtStructType pbThisTarget, char * cpFileName)
{
/* a storage for number under which the item is registerd in Targets */
unsigned char _the_number;

    /* Create an output file */
    FILE *fp = fopen(cpFileName, "w");

    /* process each Target's entry */
    while (pbThisTarget != NULL )
    {
#if 0
	/* Put it on the screen first */
	DSTRUCT(" %9.3f\t%s\n", 
		pbThisTarget->pDta->dbOpRespInt/1000, 	pbThisTarget->pDta->cpHttpRequest   );

	/* TODO: really needed?  */
	fflush(stdout);
#endif /* 0 */

	/* Assure it has been created */
	if ( NULL != fp )

		/* fflush() is not necessary, since we've got the fclose() right after cycle */
		fprintf(fp," %9.3f\t%s\n", pbThisTarget->pDta->dbOpRespInt/1000,
			pbThisTarget->pDta->cpHttpRequest );

	/* Go to next record of Target */
	pbThisTarget =  pbThisTarget->pNext;
    };

    /* On this the data are flushed in and ready for preview */
    if ( NULL != fp ) fclose(fp);   

} /* void _ProcessTargets(pTgtStructType pbThisTarget) */


/* Erase an entire signle binded list */
void _DeleteTarget(pTgtStructType pbThisTarget)
{
/* a tomparary variable of type 'pTgtStructType' */
pTgtStructType pbChild;

	/* Walk through entire list and delete each chain */
	while (NULL != pbThisTarget)
	{
		if (NULL != pbThisTarget->pDta) 
		{
			free(pbThisTarget->pDta->cpHttpRequest);
		}
		else
			DSTRUCT("[%s] %s: ERROR: nothing to release\n", __FILE__, __func__);
		    
		/* preserve a pointer to next record */
		pbChild = pbThisTarget->pNext;
		
		/* free space occupied by current record */
		free (pbThisTarget);
		
		/* Go to next record */
		pbThisTarget = pbChild;
	}
	
	/* Done */
	return;

} /* void _DeleteTarget(pTgtStructType pbThisTarget) */
