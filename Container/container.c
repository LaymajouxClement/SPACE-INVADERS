/*
 * container.c
 *
 *  Created on: 3 mars 2020
 *      Author: Laymajoux Gilles Pascual
 */

#include <assert.h>

#include <stdio.h>
#include <stdlib.h>
#include "container.h"
#include <stdbool.h>


//////---------NODE MODULE--------------------------------------------------------------------------------///////

enum {
	NOT_DEL_ELEM = false,
	DEL_ELEM = true
};

typedef struct t_node{
	struct t_node*	pPrev;
	struct t_node*	pNext;
	void*			pElem;
}t_node;

t_node*NodeNew(t_node*pPrev, t_node*pNext, void*pElem){
	t_node*pNewNode = (t_node*)malloc(sizeof(t_node));
	pNewNode->pPrev = pPrev;
	pNewNode->pNext = pNext;
	pNewNode->pElem = pElem;
	if(pPrev) pPrev->pNext = pNewNode;
	if(pNext) pNext->pPrev = pNewNode;
	return pNewNode;
}

t_node*NodeDelReturnNext(t_node*pNode, t_ptfV pDelElemFunc, bool delElem){

	t_node*pNext = pNode->pNext;
	if(delElem){
		if(pDelElemFunc) pDelElemFunc(pNode->pElem);
		else free(pNode->pElem);
	}

	if(pNode->pPrev) pNode->pPrev->pNext = pNode->pNext;
	if(pNode->pNext) pNode->pNext->pPrev = pNode->pPrev;

	free(pNode);

	return pNext;
}


t_node*NodeDelReturnPrev(t_node*pNode, t_ptfV pDelElemFunc, bool delElem){

	t_node*pPrev = pNode->pPrev;

	if(delElem){
		if(pDelElemFunc) pDelElemFunc(pNode->pElem);
		else free(pNode->pElem);
	}

	if(pNode->pPrev) pNode->pPrev->pNext = pNode->pNext;
	if(pNode->pNext) pNode->pNext->pPrev = pNode->pPrev;

	free(pNode);

	return pPrev;
}



//////------------CONTAINER MODULE----------------------------------------------------------------------------////


struct s_container{
	int		nCard;
	t_node*	pHead;
	t_node*	pTail;
	t_ptfV	pDelElemFunc;
};


s_container* ContainerNew(t_ptfV pDelElemFunc){
	s_container* pNewContainer = (s_container* )malloc(sizeof(struct s_container));

	pNewContainer->nCard = 0;
	pNewContainer->pHead = NULL;
	pNewContainer->pTail = NULL;
	pNewContainer->pDelElemFunc = pDelElemFunc;

	return pNewContainer;
}

s_container* ContainerDel(s_container* pContainer){

	if(pContainer==NULL) return NULL;

	while(pContainer->pHead){
		pContainer->pHead = NodeDelReturnNext(pContainer->pHead, pContainer->pDelElemFunc, true);
		pContainer->nCard--;
	}

	assert(pContainer->nCard==0);
	free(pContainer);

	return NULL;
}

void*ContainerPushback(s_container* pContainer, void*pElem){

	assert(pContainer!=NULL);

	if(pContainer->nCard==0){
		assert((pContainer->pHead==NULL)&&(pContainer->pTail==NULL));
		pContainer->pHead=pContainer->pTail=NodeNew(NULL, NULL, pElem);
	}
	else{
		pContainer->pTail = NodeNew(pContainer->pTail, NULL, pElem);
	}

	pContainer->nCard++;
	assert(pContainer->pTail->pElem==pElem);

	return pContainer->pTail->pElem;
}

void*ContainerPushfront(s_container* pContainer, void*pElem){

	assert(pContainer!=NULL);

	if(pContainer->nCard==0){
		assert((pContainer->pHead==NULL)&&(pContainer->pTail==NULL));
		pContainer->pHead=pContainer->pTail=NodeNew(NULL, NULL, pElem);
	}
	else{
		pContainer->pHead = NodeNew(NULL, pContainer->pHead, pElem);
	}

	pContainer->nCard++;
	assert(pContainer->pHead->pElem==pElem);

	return pContainer->pHead->pElem;
}

void*ContainerPushat(s_container* pContainer, void*pElem, int nAt){

	assert(pContainer!=NULL);

	assert(nAt>=0); assert(nAt<=pContainer->nCard);

	if(nAt==0) return ContainerPushfront(pContainer, pElem);
	if(nAt==pContainer->nCard) return ContainerPushback(pContainer, pElem);

	t_node*pScan;
	if(nAt<=pContainer->nCard/2){
		pScan=pContainer->pHead;
		while(nAt--){
			pScan = pScan->pNext;
		}
		pScan = NodeNew(pScan->pPrev, pScan, pElem);
	}
	else{
		pScan=pContainer->pTail;
		nAt=pContainer->nCard-nAt;
		while(nAt--){
			pScan = pScan->pPrev;
		}
		pScan = NodeNew(pScan, pScan->pNext, pElem);
	}
	pContainer->nCard++;
	assert(pScan->pElem==pElem);
	return pScan->pElem;
}

void*ContainerGetback(s_container* pContainer){
	assert(pContainer!=NULL);
	assert(pContainer->nCard!=0);
	return pContainer->pTail->pElem;
}

void*ContainerGetfront(s_container* pContainer){
	assert(pContainer!=NULL);
	assert(pContainer->nCard!=0);
	return pContainer->pHead->pElem;
}

void*ContainerGetat(s_container* pContainer, int nAt){
	assert(pContainer!=NULL);
	assert(pContainer->nCard!=0);
	assert(nAt>=0); assert(nAt<pContainer->nCard);

	if(nAt==0) return ContainerGetfront(pContainer);
	if(nAt==pContainer->nCard-1) return ContainerGetback(pContainer);

	t_node*pScan;
	if(nAt<=pContainer->nCard/2){
		pScan=pContainer->pHead;
		while(nAt--){
			pScan = pScan->pNext;
		}
	}
	else{
		pScan=pContainer->pTail;
		nAt=pContainer->nCard-nAt-1;
		while(nAt--){
			pScan = pScan->pPrev;
		}
	}
	return pScan->pElem;
}

void*ContainerPopback(s_container* pContainer){
	assert(pContainer!=NULL);
	assert(pContainer->nCard!=0);
	void*pElem = pContainer->pTail->pElem;
	pContainer->pTail = NodeDelReturnPrev(pContainer->pTail, pContainer->pDelElemFunc, false);
	pContainer->nCard--;
	return pElem;
}

void*ContainerPopfront(s_container* pContainer){
	assert(pContainer!=NULL);
	assert(pContainer->nCard!=0);
	void*pElem = pContainer->pHead->pElem;
	pContainer->pHead = NodeDelReturnNext(pContainer->pHead, pContainer->pDelElemFunc, false);
	pContainer->nCard--;
	return pElem;
}

void*ContainerPopat(s_container* pContainer, int nAt){
	assert(pContainer!=NULL);
	assert(pContainer->nCard!=0);
	assert(nAt>=0); assert(nAt<pContainer->nCard);

	void*pElem;

	if(nAt==0) return ContainerPopfront(pContainer);
	if(nAt==pContainer->nCard-1) return ContainerPopback(pContainer);

	t_node*pScan;
	if(nAt<=pContainer->nCard/2){
		pScan=pContainer->pHead;
		while(nAt--){
			pScan = pScan->pNext;
		}
	}
	else{
		pScan=pContainer->pTail;
		nAt=pContainer->nCard-nAt-1;
		while(nAt--){
			pScan = pScan->pPrev;
		}
	}
	pElem = pScan->pElem;
	NodeDelReturnNext(pScan, pContainer->pDelElemFunc, false);
	pContainer->nCard--;
	return pElem;
}

void*ContainerParse(s_container* pContainer, t_ptfVV pParseFunc, void* pParam){
	assert(pContainer!=NULL);
	assert(pParseFunc!=NULL);
	t_node*pScan = pContainer->pHead;
	while(pScan){
		if(!pParseFunc(pScan->pElem, pParam)) return pScan->pElem;
		pScan = pScan->pNext;
	}
	return NULL;
}

void*ContainerParseDelIf(s_container* pContainer, t_ptfVV pParseFunc, void* pParam){
	assert(pContainer!=NULL);
	assert(pParseFunc!=NULL);
	t_node*pScan = pContainer->pHead;

	while(pScan){
		if(!pParseFunc(pScan->pElem, pParam)) ContainerDeleteByElem(pContainer, pScan->pElem);
		pScan = pScan->pNext;
	}

	return NULL;
}

void*ContainerSort(s_container* pContainer, t_ptfVVV pSortFunc, void* pParam){
	assert(pContainer!=NULL);
	assert(pSortFunc!=NULL);

	struct t_node*pScan, *pNext;
	void*pTmp;
	bool restart;

	do{
		pScan = pContainer->pHead;
		restart = false;
		while(pScan){
			pNext=pScan->pNext;
			if(pNext){
				if(pSortFunc(pScan->pElem, pNext->pElem, pParam)){
					pTmp = pScan->pElem;
					pScan->pElem = pNext->pElem;
					pNext->pElem = pTmp;
					restart = true;
				}
			}
			pScan = pScan->pNext;
		}
	}while(restart);
	return NULL;
}

void*ContainerIntersectDelElemsIf(s_container* pContA, s_container* pContB, t_ptfVVV pIntersecFunc, void* pParam){
	assert(pContA!=NULL);
	assert(pContB!=NULL);
	assert(pIntersecFunc!=NULL);

	t_node *pScanA, *pScanB;
	bool rescan;
	do{


		pScanA = pContA->pHead;
		pScanB = pContB->pHead;

		while(pScanA)
		{
			while(pScanB)
			{
				if(pIntersecFunc(pScanA->pElem, pScanB->pElem, pParam) == pScanB->pElem)
				{
					ContainerDeleteByElem(pContA, pScanA->pElem);
					ContainerDeleteByElem(pContB, pScanB->pElem);
				}
				pScanB = pScanB->pNext;
			}
			pScanA = pScanA->pNext;
		}

		rescan = false;

	}while(rescan);
	return NULL;
}

void*ContainerDeleteByElem(s_container* pContainer, void*pElem){
	assert(pContainer!=NULL);
	assert(pElem!=NULL);
	t_node*pScan;
	pScan =  pContainer->pHead;
	while((pScan!=NULL)&&(pScan->pElem!=pElem)){
		pScan = pScan->pNext;
	}
	if(pScan!=NULL){
		if(pScan==pContainer->pHead){
			pScan = pContainer->pHead = NodeDelReturnNext(pScan, pContainer->pDelElemFunc, DEL_ELEM);
			if(pContainer->nCard==1){
				pContainer->pTail = pContainer->pHead;
				assert(pContainer->pHead==NULL);
			}
		}
		else if(pScan==pContainer->pTail){
			pContainer->pTail = NodeDelReturnPrev(pScan, pContainer->pDelElemFunc, DEL_ELEM);
			pScan = NULL;
		}
		else{
			pScan = NodeDelReturnNext(pScan, pContainer->pDelElemFunc, DEL_ELEM);
		}
		pContainer->nCard--;
	}
	return pScan;
}

void*ContainerIsElem(s_container* pContainer, void*pElem){
	assert(pContainer!=NULL);
	t_node*pScan = pContainer->pHead;
	while(pScan){
		if(pScan->pElem == pElem) return pElem;
		pScan = pScan->pNext;
	}
	return NULL;
}


int ContainerCard(s_container* pContainer) { return pContainer->nCard; }

void*ContainerFlush(s_container*pContainer) {
	assert(pContainer != NULL);
	while (pContainer->pHead) {
		pContainer->pHead = NodeDelReturnNext(pContainer->pHead, pContainer->pDelElemFunc, true);
		pContainer->nCard--;
	}
	assert(pContainer->nCard == 0);
	pContainer->pTail = NULL;
	return NULL;
}



