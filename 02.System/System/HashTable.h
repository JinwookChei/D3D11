#pragma once

struct BUCKET_DESC {
	LINK_ITEM* bucketHead_;
	LINK_ITEM* bucketTail_;
	unsigned int linkCount_;
};

struct BUCKET_ITEM {
	const void* item_;         // value
	BUCKET_DESC* bucketDesc_;  // 버킷 아이템을 가지고 있는 버킷
	LINK_ITEM link_;           // 버킷의 링크드리스트 링크
	unsigned int keySize_;     // key size
	char keyData_[1];
};

class HashTableIterator {
	friend class HashTable;

public:
	SYSTEM_API HashTableIterator(unsigned int maxBucketCount, unsigned int currentBucketCount, BUCKET_DESC* bucketDescTable, LINK_ITEM* current);

	SYSTEM_API HashTableIterator& operator++();  // ++전위

	SYSTEM_API HashTableIterator operator++(int);  // 후위++

	SYSTEM_API bool operator==(const HashTableIterator& other);

	SYSTEM_API bool operator!=(const HashTableIterator& other);

	SYSTEM_API void* operator*();

private:
	unsigned int maxBucketCount_;

	unsigned int currentBucketCount_;

	BUCKET_DESC* bucketDescTable_;

	LINK_ITEM* current_;
};

// 중복키 허용
class HashTable final {
public:
	SYSTEM_API HashTable();

	SYSTEM_API ~HashTable();

	SYSTEM_API bool Initialize(unsigned int maxBucketCount, unsigned int maxKeySize);

	// Player* selectPlayer[10] = {nullptr, };
	// 8 = Select((void**)selectPlayer, 10, "A", 1);
	// for

	// Player* selectPlayer = nullptr;
	// 1 = Select((void**)&selectPlayer, 1, "A", 1);

	SYSTEM_API unsigned int Select(void** ppOutItemList, unsigned int maxItemCount, const void* keyData, unsigned int keySize, unsigned int startIndex = 0);

	// pItem == Player* / pKeyData == int
	SYSTEM_API void* Insert(const void* pItem, const void* pKeyData, unsigned int keySize);

	SYSTEM_API void Delete(const void* searchHandle);

	SYSTEM_API void DeleteAll();

	SYSTEM_API unsigned int GetAllItem(void** ppOutItemList, unsigned int maxItemCount, unsigned int startIndex, bool* pOutInsufficient) const;

	SYSTEM_API void Cleanup();

	SYSTEM_API HashTableIterator begin();

	SYSTEM_API HashTableIterator erase(HashTableIterator& iter);

	SYSTEM_API HashTableIterator end();

private:
	// 리턴은 해시테이블의 인덱스를 리턴한다.
	unsigned int CreateKey(const void* pData, unsigned int keySize, unsigned int bucketCount);

	BUCKET_DESC* bucketDescTable_;

	unsigned int maxBucketCount_;

	unsigned int maxKeySize_;

	unsigned int currentItemCount_;
};