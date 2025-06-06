/*******************************************************************************
The content of this file includes portions of the AUDIOKINETIC Wwise Technology
released in source code form as part of the SDK installer package.

Commercial License Usage

Licensees holding valid commercial licenses to the AUDIOKINETIC Wwise Technology
may use this file in accordance with the end user license agreement provided
with the software or, alternatively, in accordance with the terms contained in a
written agreement between you and Audiokinetic Inc.

Apache License Usage

Alternatively, this file may be used under the Apache License, Version 2.0 (the
"Apache License"); you may not use this file except in compliance with the
Apache License. You may obtain a copy of the Apache License at
http://www.apache.org/licenses/LICENSE-2.0.

Unless required by applicable law or agreed to in writing, software distributed
under the Apache License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES
OR CONDITIONS OF ANY KIND, either express or implied. See the Apache License for
the specific language governing permissions and limitations under the License.

  Copyright (c) 2025 Audiokinetic Inc.
*******************************************************************************/

#ifndef _KEYARRAY_H_
#define _KEYARRAY_H_

#include <AK/Tools/Common/AkArray.h>
#include <AK/Tools/Common/AkKeyDef.h>

// The Key list is simply a list that may be referenced using a key
// NOTE : 
template <class T_KEY, class T_ITEM, class U_POOL = ArrayPoolDefault, class TGrowBy = AkGrowByPolicy_DEFAULT, class TMovePolicy = AkAssignmentMovePolicy<MapStruct<T_KEY, T_ITEM> > >
class CAkKeyArray : public AkArray< MapStruct<T_KEY, T_ITEM>, const MapStruct<T_KEY, T_ITEM>&, U_POOL, TGrowBy, TMovePolicy>
{
public:
	//====================================================================================================
	// Return NULL if the Key does not exisis
	// Return T_ITEM* otherwise
	//====================================================================================================
	T_ITEM* Exists(T_KEY in_Key) const
	{
		typename CAkKeyArray< T_KEY, T_ITEM, U_POOL, TGrowBy, TMovePolicy >::Iterator it = this->FindEx(in_Key);
		return (it != this->End()) ? &(it.pItem->item) : NULL;
	}

public:
	//====================================================================================================
	// Sets the item referenced by the specified key and item
	// Return AK_Fail if the list max size was exceeded
	//====================================================================================================
	T_ITEM * Set(T_KEY in_Key, const T_ITEM & in_Item)
	{
		T_ITEM* pSearchedItem = Exists(in_Key);
		if (pSearchedItem)
		{
			*pSearchedItem = in_Item;
		}
		else
		{
			MapStruct<T_KEY, T_ITEM> * pStruct = this->AddLast();
			if (pStruct)
			{
				pStruct->key = in_Key;
				pStruct->item = in_Item;
				pSearchedItem = &(pStruct->item);
			}
		}
		return pSearchedItem;
	}

	T_ITEM * SetFirst(T_KEY in_Key, const T_ITEM & in_Item)
	{
		T_ITEM* pSearchedItem = Exists(in_Key);
		if (pSearchedItem)
		{
			*pSearchedItem = in_Item;
		}
		else
		{
			MapStruct<T_KEY, T_ITEM> * pStruct = this->Insert(0); //insert at index 0 is AddFirst.
			if (pStruct)
			{
				pStruct->key = in_Key;
				pStruct->item = in_Item;
				pSearchedItem = &(pStruct->item);
			}
		}
		return pSearchedItem;
	}

	T_ITEM * Set(T_KEY in_Key)
	{
		T_ITEM* pSearchedItem = Exists(in_Key);
		if (!pSearchedItem)
		{
			MapStruct<T_KEY, T_ITEM> * pStruct = this->AddLast();
			if (pStruct)
			{
				pStruct->key = in_Key;
				pSearchedItem = &(pStruct->item);
			}
		}
		return pSearchedItem;
	}

	// NOTE: The real definition should be 
	// typename CAkKeyArray<T_KEY,T_ITEM,TGrowBy, TMovePolicy>::Iterator FindEx( T_KEY in_Item ) const
	// Typenaming the base class is a workaround for bug MTWX33123 in the new Freescale CodeWarrior.
	typename AkArray< MapStruct<T_KEY, T_ITEM>, const MapStruct<T_KEY, T_ITEM>&, U_POOL, TGrowBy, TMovePolicy>::Iterator FindEx(T_KEY in_Item) const
	{
		typename CAkKeyArray< T_KEY, T_ITEM, U_POOL, TGrowBy, TMovePolicy >::Iterator it = this->Begin();

		typename CAkKeyArray< T_KEY, T_ITEM, U_POOL, TGrowBy, TMovePolicy >::Iterator itEnd = this->End();
		for (; it != itEnd; ++it)
		{
			if ((*it).key == in_Item)
				break;
		}

		return it;
	}

	//====================================================================================================
	//	Remove the item referenced by the specified key
	//====================================================================================================

	void Unset(T_KEY in_Key)
	{
		typename CAkKeyArray< T_KEY, T_ITEM, U_POOL, TGrowBy, TMovePolicy >::Iterator it = FindEx(in_Key);
		if (it != this->End())
		{
			this->Erase(it);
		}
	}

	//====================================================================================================
	//	More efficient version of Unset when order is unimportant
	//====================================================================================================

	void UnsetSwap(T_KEY in_Key)
	{
		typename CAkKeyArray< T_KEY, T_ITEM, U_POOL, TGrowBy, TMovePolicy >::Iterator it = FindEx(in_Key);
		if (it != this->End())
		{
			this->EraseSwap(it);
		}
	}
};

/// Key policy for AkSortedKeyArray.
template <class T_KEY, class T_ITEM> struct AkGetArrayKey
{
	/// Default policy.
	static AkForceInline T_KEY & Get(T_ITEM & in_item)
	{
		return in_item.key;
	}
};

template <class T_KEY, class T_ITEM> struct AkGetArrayKeyFunc
{
	/// Default policy.
	static AkForceInline T_KEY& Get(T_ITEM& in_item)
	{
		return in_item.Key();
	}
};

/// Trivial key policy for AkSortedKeyArray, when T_KEY is T_ITEM.
struct AkGetArrayKeyTrivial
{
	/// Default policy.
	template <class T_KEY>
	static AkForceInline T_KEY& Get(T_KEY& in_item)
	{
		return in_item;
	}
};

//Default comparison policy for AkSortedKeyArray.
template <class T_KEY> struct AkDefaultSortedKeyCompare
{
public:
	template<class THIS_CLASS>
	static AkForceInline bool Lesser(THIS_CLASS*, const T_KEY &a, const T_KEY &b)
	{
		return a < b;
	}

	template<class THIS_CLASS>
	static AkForceInline bool Equal(THIS_CLASS*, const T_KEY &a, const T_KEY &b)
	{
		return a == b;
	}
};

/// Array of items, sorted by key. Uses binary search for lookups. BEWARE WHEN
/// MODIFYING THE ARRAY USING BASE CLASS METHODS.
template <class T_KEY, class T_ITEM, class U_POOL = ArrayPoolDefault, class U_KEY = AkGetArrayKey< T_KEY, T_ITEM >, class TGrowBy = AkGrowByPolicy_DEFAULT, class TMovePolicy = AkAssignmentMovePolicy<T_ITEM>, class TComparePolicy = AkDefaultSortedKeyCompare<T_KEY> >
class AkSortedKeyArray : public AkArray< T_ITEM, const T_ITEM &, U_POOL, TGrowBy, TMovePolicy >
{
public:	
	using base = AkArray<T_ITEM, const T_ITEM&, U_POOL, TGrowBy, TMovePolicy>;
	using Iterator = typename base::Iterator;

	AkForceInline bool Lesser(const T_KEY &a, const T_KEY &b) const
	{
		return TComparePolicy::Lesser((void*)this, a, b);
	}

	AkForceInline bool Equal(const T_KEY &a, const T_KEY &b) const
	{
		return TComparePolicy::Equal((void*)this, a, b);
	}

	AkUInt32 GetIndex(T_ITEM* in_pItem) const
	{
		if (this->m_pItems && this->m_uLength > 0)
		{
			AkUInt32 index = (AkUInt32)(in_pItem - this->m_pItems);
			if ((this->m_pItems + index) == in_pItem)
				return index;
		}
		return this->m_uLength;
	}

	T_ITEM* Exists(T_KEY in_key) const	
	{
		bool bFound;
		T_ITEM* pItem = BinarySearch(in_key, bFound);
		return bFound ? pItem : NULL;
	}

	// Add an item to the list (allowing duplicate keys)

	T_ITEM * Add(T_KEY in_key)
	{
		T_ITEM * pItem = AddNoSetKey(in_key);

		// Then set the key
		if (pItem)
			U_KEY::Get(*pItem) = in_key;

		return pItem;
	}

	// Add an item to the list (allowing duplicate keys)

	T_ITEM * AddNoSetKey(T_KEY in_key)
	{
		bool bFound;
		return AddNoSetKey(in_key, bFound);
	}

	T_ITEM * AddNoSetKey(T_KEY in_key, bool& out_bFound)
	{
		T_ITEM * pItem = BinarySearch(in_key, out_bFound);
		if (pItem)
		{
			unsigned int uIdx = (unsigned int)(pItem - this->m_pItems);
			pItem = this->Insert(uIdx);
		}
		else
		{
			pItem = this->AddLast();
		}

		return pItem;
	}

	// Set an item in the list (returning existing item if present)

	T_ITEM * Set(T_KEY in_key)
	{
		bool bFound;
		return Set(in_key, bFound);
	}

	T_ITEM * Set(T_KEY in_key, bool & out_bExists)
	{
		T_ITEM * pItem = BinarySearch(in_key, out_bExists);
		if (!out_bExists)
		{
			if (pItem)
			{
				unsigned int uIdx = (unsigned int)(pItem - this->m_pItems);
				pItem = this->Insert(uIdx);
			}
			else
			{
				pItem = this->AddLast();
			}

			if (pItem)
				U_KEY::Get(*pItem) = in_key;
		}

		return pItem;
	}

	bool Unset(T_KEY in_key)
	{
		T_ITEM * pItem = Exists(in_key);
		if (pItem)
		{
			Iterator it;
			it.pItem = pItem;
			this->Erase(it);
			return true;
		}

		return false;
	}

	// Replace current sorted items with a list of sorted updates.
	// - Walks both arrays (current and updates), and calls user provided functions on each comparison.
	// - Returns false on any error. Always completes walks of both arrays.
	// - fnExists = void (T_ITEM& in_item, const tUpdate& in_update);
	// - fnNew = void (T_ITEM& in_item, const tUpdate& in_update);
	// - fnOld = bool (T_ITEM& in_item); // return value indicates if entry should be deleted.
	template <typename T_UPDATE>
	struct GetUpdateKey
	{
		static const T_KEY& Get(const T_UPDATE& in) { return in; }
	};

	template <
		typename T_UPDATE,
		typename U_UPDATEKEY = GetUpdateKey<T_UPDATE>,
		typename FN_EXISTS = void (*)(T_ITEM&, const T_UPDATE&),
		typename FN_NEW = void (*)(T_ITEM&, const T_UPDATE&),
		typename FN_OLD = void (*)(T_ITEM&)
	>
	bool SortedUpdate(AkUInt32 in_numUpdates, const T_UPDATE* in_pUpdates, FN_EXISTS in_fnExists, FN_NEW in_fnNew, FN_OLD in_fnOld)
	{
		#ifndef AK_OPTIMIZED
			// Updates must be sorted.
			for (AkUInt32 i = 1; i < in_numUpdates; ++i)
			{
				const T_KEY& a = U_UPDATEKEY::Get(in_pUpdates[i-1]);
				const T_KEY& b = U_UPDATEKEY::Get(in_pUpdates[i]);
				if (!Lesser(a, b) && !Equal(a, b))
				{
					AKASSERT(!"AkSortedKeyArray::SortedUpdate, input not sorted!");
					return false;
				}
			}

			// Array must be sorted.
			for (AkUInt32 i = 1; i < this->Length(); ++i)
			{
				T_KEY& a = U_KEY::Get(this->m_pItems[i-1]);
				T_KEY& b = U_KEY::Get(this->m_pItems[i]);
				if (!Lesser(a, b) && !Equal(a, b))
				{
					AKASSERT(!"AkSortedKeyArray::SortedUpdate, array not sorted!");
					return false;
				}
			}
		#endif

		bool bResult = true;

		// Walk through both arrays while not having reached the end of either.
		AkUInt32 oldIdx = 0, newIdx = 0;
		while (oldIdx < this->Length() && newIdx < in_numUpdates)
		{
			T_ITEM& oldItem = this->m_pItems[oldIdx];
			T_KEY& oldKey = U_KEY::Get(oldItem);

			const T_UPDATE& newItem = in_pUpdates[newIdx];
			const T_KEY& newKey = U_UPDATEKEY::Get(newItem);

			if (Equal(oldKey, newKey))
			{
				// Common item. Call user function, then move one.
				in_fnExists(oldItem, newItem);
				++oldIdx;
				++newIdx;
			}
			else if (Lesser(oldKey, newKey))
			{
				// Call user function; return value determines if we delete the entry.
				if (in_fnOld(oldItem))
					this->Erase(oldIdx);
				else
					++oldIdx;

				// Don't increment oldIdx!
			}
			else // if (oldKey > newKey)
			{
				// New entry. Insert, then call user function.
				T_ITEM* pNew = this->Insert(oldIdx);
				if (pNew)
				{
					U_KEY::Get(*pNew) = newKey;
					in_fnNew(*pNew, newItem);
					++oldIdx;
				}
				else
					bResult = false;

				++newIdx;
			}
		}

		// Remove any leftover old items.
		while (oldIdx < this->Length())
		{
			if (in_fnOld(this->m_pItems[oldIdx]))
				this->Erase(oldIdx);
			else
				++oldIdx;
		}

		// Add any leftover new items.
		for (; newIdx < in_numUpdates; ++newIdx)
		{
			const T_UPDATE& newItem = in_pUpdates[newIdx];
			const T_KEY& newKey = U_UPDATEKEY::Get(newItem);

			T_ITEM* pNew = this->AddLast();
			if (pNew)
			{
				U_KEY::Get(*pNew) = newKey;
				in_fnNew(*pNew, newItem);
			}
			else
				bResult = false;
		}

		return bResult;
	}

	// WARNING: Do not use on types that need constructors or destructor called on Item objects at each creation.

	void Reorder(T_KEY in_OldKey, T_KEY in_NewKey, const T_ITEM & in_item)
	{
		bool bFound;
		T_ITEM * pItem = BinarySearch(in_OldKey, bFound);

		//AKASSERT( bFound );
		if (!bFound) return;// cannot be an assert for now.(WG-19496)

		unsigned int uIdx = (unsigned int)(pItem - this->m_pItems);
		unsigned int uLastIdx = this->Length() - 1;

		AKASSERT(*pItem == in_item);

		bool bNeedReordering = false;
		if (uIdx > 0) // if not first
		{
			T_ITEM * pPrevItem = this->m_pItems + (uIdx - 1);
			if (Lesser(in_NewKey, U_KEY::Get(*pPrevItem)))
			{
				// Check one step further
				if (uIdx > 1)
				{
					T_ITEM * pSecondPrevItem = this->m_pItems + (uIdx - 2);
					if (Lesser(U_KEY::Get(*pSecondPrevItem), in_NewKey))
					{
						return Swap(pPrevItem, pItem);
					}
					else
					{
						bNeedReordering = true;
					}
				}
				else
				{
					return Swap(pPrevItem, pItem);
				}
			}
		}
		if (!bNeedReordering && uIdx < uLastIdx)
		{
			T_ITEM * pNextItem = this->m_pItems + (uIdx + 1);
			if (Lesser(U_KEY::Get(*pNextItem), in_NewKey))
			{
				// Check one step further
				if (uIdx < (uLastIdx - 1))
				{
					T_ITEM * pSecondNextItem = this->m_pItems + (uIdx + 2);
					if (Lesser(in_NewKey, U_KEY::Get(*pSecondNextItem)))
					{
						return Swap(pNextItem, pItem);
					}
					else
					{
						bNeedReordering = true;
					}
				}
				else
				{
					return Swap(pNextItem, pItem);
				}
			}
		}

		if (bNeedReordering)
		{
			/////////////////////////////////////////////////////////
			// Faster implementation, moving only what is required.
			/////////////////////////////////////////////////////////
			unsigned int uIdxToInsert; // non initialized
			T_ITEM * pTargetItem = BinarySearch(in_NewKey, bFound);
			if (pTargetItem)
			{
				uIdxToInsert = (unsigned int)(pTargetItem - this->m_pItems);
				if (uIdxToInsert > uIdx)
				{
					--uIdxToInsert;// we are still in the list, don't count the item to be moved.
				}
			}
			else
			{
				uIdxToInsert = uLastIdx;
			}

			T_ITEM * pStartItem = this->m_pItems + uIdx;
			T_ITEM * pEndItem = this->m_pItems + uIdxToInsert;
			if (uIdxToInsert < uIdx)
			{
				// Slide backward.
				while (pStartItem != pEndItem)
				{
					--pStartItem;
					pStartItem[1] = pStartItem[0];
				}
			}
			else
			{
				// Slide forward.
				while (pStartItem != pEndItem)
				{
					pStartItem[0] = pStartItem[1];
					++pStartItem;
				}
			}
			pEndItem[0] = in_item;
			///////////////////////////////////////////////
		}
	}

	// WARNING: Do not use on types that need constructors or destructor called on Item objects at each creation.

	void ReSortArray() //To be used when the < > operator changed meaning.
	{
		AkInt32 NumItemsToReInsert = this->Length();
		if (NumItemsToReInsert != 0)
		{
			// Do a re-insertion sort.
			// Fool the table by faking it is empty, then re-insert one by one.
			T_ITEM * pReinsertionItem = this->m_pItems;
			this->m_uLength = 0; // Faking the Array Is Empty.
			for (AkInt32 idx = 0; idx < NumItemsToReInsert; ++idx)
			{
				T_ITEM ItemtoReinsert = pReinsertionItem[idx]; // make a copy as the source is about to be overriden.

				T_KEY keyToReinsert = U_KEY::Get(ItemtoReinsert);

				T_ITEM* pInsertionEmplacement = AddNoSetKey(keyToReinsert);

				AKASSERT(pInsertionEmplacement);
				*pInsertionEmplacement = ItemtoReinsert;
			}
		}
	}	
	
	// If found, returns the first item it encounters, if not, returns the insertion point.
	T_ITEM * BinarySearch( T_KEY in_key, bool & out_bFound ) const
	{
		AkUInt32 uNumToSearch = this->Length();
		AkInt32 iBase = 0;
		AkInt32 iPivot = 0;

		while ( uNumToSearch > 0 )
		{
			iPivot = iBase + ( uNumToSearch >> 1 );
			T_KEY pivotKey = U_KEY::Get( this->m_pItems[ iPivot ] );
			if ( Equal( pivotKey, in_key ) )
			{
				out_bFound = true;
				return this->m_pItems + iPivot;
			}

			if ( Lesser( pivotKey, in_key ) )
			{
				iBase = iPivot + 1;
				uNumToSearch--;
			}
			uNumToSearch >>= 1;
		}

		out_bFound = false;
		return this->m_pItems + iBase;
	}

	T_ITEM* LowerBounds(T_KEY in_key) const
	{
		return LowerBounds(in_key, this->Begin(), this->End());
	}

	// Returns the first item in the array that is not less than the key,
	// or the insertion point, if no key is less then in_key.
	T_ITEM* LowerBounds(T_KEY in_key, Iterator in_from, Iterator in_to) const
	{
		AKASSERT(in_to.pItem >= in_from.pItem);
		AkUInt32 uBase = (AkUInt32)(in_from.pItem - this->m_pItems);
		AkInt64 uNumToSearch = (AkInt64)(in_to.pItem - in_from.pItem);
		AkUInt32 uPivot;

		while (uNumToSearch > 0)
		{
			uPivot = uBase + (AkUInt32)(uNumToSearch >> 1);
			T_KEY pivotKey = U_KEY::Get(this->m_pItems[uPivot]);
			if (Lesser(pivotKey, in_key))
			{
				uBase = uPivot + 1;
				uNumToSearch--;
			}
			uNumToSearch >>= 1;
		}

		return this->m_pItems + uBase;
	}

	AkForceInline void Swap(T_ITEM * in_ItemA, T_ITEM * in_ItemB)
	{
		T_ITEM ItemTemp = *in_ItemA;
		*in_ItemA = *in_ItemB;
		*in_ItemB = ItemTemp;
	}
};


#endif //_KEYARRAY_H_
