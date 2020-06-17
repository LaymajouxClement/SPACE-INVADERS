/*
 * container.h
 *
 *  Created on: 3 mars 2020
 *      Author: Laymajoux Gilles Pascual
 */

struct s_container;
typedef struct s_container  s_container;

typedef void*(*t_ptfV)(void*);
typedef void*(*t_ptfVV)(void*, void*);
typedef void*(*t_ptfVVV)(void*, void*, void*);

s_container* ContainerNew(t_ptfV pDelElemFunc);
s_container* ContainerDel(s_container* pContainer);

void*ContainerPushback(s_container* pContainer, void*pElem);
void*ContainerPushfront(s_container* pContainer, void*pElem);
void*ContainerPushat(s_container* pContainer, void*pElem, int nAt);

void*ContainerGetback(s_container* pContainer);
void*ContainerGetfront(s_container* pContainer);
void*ContainerGetat(s_container* pContainer, int nAt);

void*ContainerPopback(s_container* pContainer);
void*ContainerPopfront(s_container* pContainer);
void*ContainerPopat(s_container* pContainer, int nAt);

void*ContainerParse(s_container* pContainer, t_ptfVV pParseFunc, void* pParam);

void*ContainerParseDelIf(s_container* pContainer, t_ptfVV pParseFunc, void* pParam);

void*ContainerSort(s_container* pContainer, t_ptfVVV pSortFunc, void* pParam);

void*ContainerIntersectDelElemsIf(s_container* pContA, s_container* pContB, t_ptfVVV pIntersecFunc, void* pParam);

void*ContainerDeleteByElem(s_container* pContainer, void*pElem);

void*ContainerIsElem(s_container* pContainer, void*pElem);

int ContainerCard(s_container* pContainer);

void*ContainerFlush(s_container*pContainer);

