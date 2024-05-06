#include "pch.h"
#include "LockFreeStack.h"

/*
// []
// Header[ next ]
void InitializeHead(SListHeader* header)
{
	header->next = nullptr;
}

void PushEntrySList(SListHeader* header, SListEntry* entry)
{
	entry->next = header->next;
	header->next = entry;
}

SListEntry* PopEntrySList(SListHeader* header)
{
	SListEntry* first = header->next;

	if (first != nullptr)
		header->next = first->next;

	return first;
}
*/

// 싱글 쓰레드 환경에서는 잘 작동 되지만 멀티쓰레드 환경에서는 안 됨.
// 아래는 멀티쓰레드 환경에서도 잘 작동되도록 바꾼 것.

/*
void InitializeHead(SListHeader* header)
{
	header->next = nullptr;
}

void PushEntrySList(SListHeader* header, SListEntry* entry)
{
	entry->next = header->next;
	// 호출하는 순간에 누군가 끼어드는 것을 방지
	// 실패를 할 동안 계속 뺑뺑이를 돌아라
	while (::InterlockedCompareExchange64((int64*)&header->next, (int64)entry, (int64)entry->next) == 0)
	{

	}
}

// [][]
// Header[ next ]
SListEntry* PopEntrySList(SListHeader* header)
{
	SListEntry* expected = header->next;

	// 호출하는 순간에 누군가 끼어드는 것을 방지
	// ABA Problem -> A라는 상태에서 B라는 상태가 되었다가 다시 A가 되는 것.
	while (expected && ::InterlockedCompareExchange64((int64*)&header->next, (int64)expected->next, (int64)expected) == 0)
	{

	}

	return expected;
}
*/

void InitializeHead(SListHeader* header)
{
	header->alignment = 0;
	header->region = 0;
}

void PushEntrySList(SListHeader* header, SListEntry* entry)
{
	SListHeader expected = {};
	SListHeader desired = {};
	
	// 16 바이트 정렬
	desired.HeaderX64.next = (((uint64)entry) >> 4);

	while (true)
	{
		expected = *header;

		// 이 사이에 변경될 수 있다

		entry->next = (SListEntry*)(((uint64)expected.HeaderX64.next) << 4);
		desired.HeaderX64.depth = expected.HeaderX64.depth + 1;
		desired.HeaderX64.sequence = expected.HeaderX64.sequence + 1;

		if (::InterlockedCompareExchange128((int64*)header, desired.region, desired.alignment, (int64*)&expected) == 1)
			break;
	}
}

SListEntry* PopEntrySList(SListHeader* header)
{
	SListHeader expected = {};
	SListHeader desired = {};
	SListEntry* entry = nullptr;

	while (true)
	{
		expected = *header;

		entry = (SListEntry*)(((uint64)expected.HeaderX64.next) << 4);
		if (entry == nullptr)
			break;

		// Use-After-Free
		desired.HeaderX64.next = ((uint64)entry->next) >> 4;
		desired.HeaderX64.depth = expected.HeaderX64.depth - 1;
		desired.HeaderX64.sequence = expected.HeaderX64.sequence + 1;

		if (::InterlockedCompareExchange128((int64*)header, desired.region, desired.alignment, (int64*)&expected) == 1)
			break;
	}

	return entry;
}