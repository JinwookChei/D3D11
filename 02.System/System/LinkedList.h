#pragma once

struct LINK_ITEM {
	LINK_ITEM* prev_;
	LINK_ITEM* next_;
	void* item_;
};

SYSTEM_API void LinkToLinkedList(LINK_ITEM** ppHead, LINK_ITEM** ppTail, LINK_ITEM* pNew);
SYSTEM_API void LinkToLinkedListFIFO(LINK_ITEM** ppHead, LINK_ITEM** ppTail, LINK_ITEM* pNew);
SYSTEM_API void UnLinkFromLinkedList(LINK_ITEM** ppHead, LINK_ITEM** ppTail, LINK_ITEM* pDel);