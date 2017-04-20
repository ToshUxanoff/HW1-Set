#pragma once
#include <iostream>
#include <exception>

template<class Key, class Compare = std::less<Key>>
class SetClass
{
public:
	using key_type = Key;
	using value_type = Key;
	using size_type = size_t;
	using reference = value_type&;
	using const_reference = const value_type&;

private:
	struct TNode
	{
		value_type Value;
		TNode* Right;
		TNode* Left;
		TNode* Up;

		TNode(const key_type& value)
			: Value(value)
			, Right(nullptr)
			, Left(nullptr)
			, Up(nullptr)
		{ }
		TNode()
			: Right(nullptr)
			, Left(nullptr)
			, Up(nullptr)
		{ }
		~TNode()
		{
			delete Left;
			delete Right;
		}
	};
	struct TSetIterator
	{
		TNode* Ptr = nullptr;

		TSetIterator() : Ptr(nullptr)
		{ }
		TSetIterator(const TSetIterator & iterator) : Ptr(iterator.Ptr)
		{ }
		TSetIterator(TNode * Tree) : Ptr(Tree)
		{ }
		~TSetIterator() = default;


		TSetIterator& operator=(const TSetIterator &iterator)
		{
			Ptr = iterator.Ptr;
			return *this;
		}
		bool operator !=(const TSetIterator& iterator)
		{
			return Ptr != iterator.Ptr;
		}
		bool operator ==(const TSetIterator& iterator)
		{
			return Ptr == iterator.Ptr;
		}
		TSetIterator& operator++()
		{
			if (Ptr == nullptr)
			{
				throw std::exception("Ptr = nullptr");
			}
			if (Ptr->Right != nullptr)
			{
				Ptr = Ptr->Right;
				while (Ptr->Left != nullptr)
				{
					Ptr = Ptr->Left;
				}
				return *this;
			}
			else if(Ptr->Up != nullptr)
			{
				TNode *prevPtr = Ptr;
				Ptr = Ptr->Up;
				while (prevPtr == Ptr->Right)
				{
					prevPtr = Ptr;
					if (Ptr->Up != nullptr)
					{
						Ptr = Ptr->Up;
					}
				}			
			}
			return *this;
		}
		TSetIterator& operator--()
		{
			if (Ptr == nullptr)
			{
				throw std::exception("Ptr = nullptr");
			}
			if (Ptr->Left != nullptr)
			{
				Ptr = Ptr->Left;
				while (Ptr->Right != nullptr)
				{
					Ptr = Ptr->Right;
				}
				return *this;
			}
			else
			{
				TNode *prevPtr = Ptr;
				Ptr = Ptr->Up;
				while (prevPtr == Ptr->Left)
				{
					prevPtr = Ptr;
					if (Ptr->Up != nullptr)
					{
						Ptr = Ptr->Up;
					}
				}
			}
			return *this;
		}
		
		TSetIterator& operator++(int)
		{
			TSetIterator it(*this);
			++*this;
			return it;
		}
		TSetIterator& operator--(int)
		{
			TSetIterator it(*this);
			--*this;
			return it;
		}
		
		reference operator*()
		{
			if (Ptr == nullptr)
			{
				throw std::exception("Ptr = nullptr");
			}
			return Ptr->Value;
		}
		const_reference operator*() const
		{
			if (Ptr == nullptr)
			{
				throw std::exception("Ptr = nullptr");
			}
			return Ptr->Value;
		}
		value_type* operator->()
		{
			if (Ptr == nullptr)
			{
				throw std::exception("Ptr = nullptr");
			}
			return &(Ptr->Value);
		}
	};
private:
	TNode* Tree;
	TNode* End;
	Compare less; 
	using iterator = TSetIterator;
	using const_iterator = const TSetIterator;
	void Insert_Branch(iterator it)// for Erase
	{
		if (it != nullptr)
		{
			iterator pos(it);
			if (pos.Ptr == nullptr)
			{
				return;
			}
			if (pos.Ptr->Right != End)
			{
				Insert_Branch(pos.Ptr->Right);
			}
			if (pos.Ptr->Left != nullptr)
			{
				Insert_Branch(pos.Ptr->Left);
			}
			insert(pos.Ptr->Value);
		}
	} 
	void EraseRoot() //for erase
	{
		if (Tree->Left == nullptr)
		{
			Tree = Tree->Right;
		}
		else if (Tree->Right == nullptr)
		{
			Tree = Tree->Left;
		}
		else
		{
			iterator LeftBranch = Tree->Left;
			iterator RightBranch = Tree->Right;
			Tree = nullptr;
			Insert_Branch(RightBranch);
			Insert_Branch(LeftBranch);
		}
		return;
	}
	void SearchEnd()
	{
		TNode* tmp = Tree;
		while (tmp->Right)
		{
			tmp = tmp->Right;
		}
		tmp->Right = End;
		End->Up = tmp;		
	}	
	
public:

	SetClass(const Compare& comp = Compare())
	{
		Tree = nullptr;
		End = nullptr; 
		less = comp;
	}
	SetClass(const SetClass & obj)
	{
		for (iterator it = obj.cbegin(); it != obj.cend(); ++it)
		{
			insert(*it);
		}
	}
	SetClass(std::initializer_list<value_type> list, const Compare& comp = Compare())
	{
		Tree =  nullptr;
		less = comp;
		for (const auto& item : list) 
		{
			insert(item);
		}
	}
	~SetClass()
	{
		delete Tree;
	}
	SetClass& operator=(const SetClass& obj)
	{
		if (Tree == obj.Tree)
		{
			return *this;
		}
		delete Tree;
		for (iterator it = obj.cbegin(); it != obj.cend(); ++it)
		{
			insert(*it);
		}
		return *this;
	}
	iterator begin()
	{ 
		if (Tree == nullptr)
		{
			throw std::exception("Tree is nullptr");
		}
		TNode* tmp = Tree;
		if (tmp->Left == nullptr || tmp->Right == nullptr)
		{
			iterator it(tmp);
			return it;
		}

		do
		{
			if (tmp->Left == nullptr && tmp->Right == nullptr)
			{
				iterator it(tmp);
				return it;
			}
			else if (tmp->Left != nullptr)
			{
				tmp = tmp->Left;
			}
			else
			{
				tmp = tmp->Right;
			}
		} while (1);
	}
	const_iterator cbegin() const
	{
		if (Tree == nullptr)
		{
			throw std::exception("Tree is nullptr");
		}
		TNode* tmp = Tree;
		do
		{
			if (tmp->Left == nullptr && tmp->Right == nullptr)
			{
				const_iterator it(tmp);
				return it;
			}
			else if (tmp->Left != nullptr)
			{
				tmp = tmp->Left;
			}
			else
			{
				tmp = tmp->Right;
			}
		} while (1);
	}
	iterator end()
	{	
		iterator ret(End);		
		return ret;
	}
	const_iterator cend() const
	{
		const_iterator ret(End);
		return ret;
	}
	void clear()
	{
		delete Tree;
		Tree = nullptr;
	}
	bool empty()
	{
		return (size() == 0);
	}

	void erase(const key_type& key)
	{
		iterator Position = find(key);
		erase(Position);
		return;
	}
	void erase(iterator Position) 
	{
		if (Tree != nullptr)
		{
			if(Position == Tree)
			{
				EraseRoot(); 
				return;
			}
			if (Position.Ptr == End->Up)	//Если удаляемый - крайний правый: смещаем end на узел вверх.
			{
				End = Position.Ptr;
				End->Up = Position.Ptr->Up;
				return;
			}
			else if (Position.Ptr->Left == nullptr && Position.Ptr->Right == nullptr) // Если удаляемый - "лист" дерева.
			{
				//У элемента выше удаляемого, могут стоять другие элементы слева/справа(не 0). Это - чтобы не потерять связи из-за возможного присвоения nullptr.
				less(Position.Ptr->Value, Position.Ptr->Up->Value) ? (Position.Ptr->Up->Left = nullptr) 
					: (Position.Ptr->Up->Right = nullptr);
				return;
			}
			
			iterator LeftBranch = Position.Ptr->Left;
			Position.Ptr->Up->Left = nullptr;
		
			iterator RightBranch = Position.Ptr->Right;
			Position.Ptr->Up->Right = nullptr;

			Insert_Branch(RightBranch);
			Insert_Branch(LeftBranch);

			delete Position.Ptr;

		}
		return;
	}

	void swap( SetClass& obj)
	{
		TNode* BufEnd = End;
		TNode* Buf = Tree;
		
		End = obj.End;
		Tree = obj.Tree;
		
		obj.Tree = Buf;
		obj.End = BufEnd;
		return;
	}
	iterator find(const Key& key)
	{	
		if (Tree == nullptr)
		{
			return End;
		}
		TNode * currNode = Tree;
		do
		{
			if (currNode == End || currNode == nullptr)
			{
				return end();
			}
			if (!less(currNode->Value,key)&&!less(key, currNode->Value)) //== here
			{
				iterator it(currNode);
				return it;
			}
			if (less(currNode->Value, key))
			{
				currNode = currNode->Right;
			}
			else
			{
				currNode = currNode->Left;
			}

		} while (currNode);		
	}
	size_type count(const Key& key)
	{
		if (find(key)!= end())
		{
			return 1;
		}
		return 0;
	}
	void insert(const value_type& value)
	{
		if (Tree == nullptr)
		{
			Tree = new TNode(value);
			End = new TNode;
			SearchEnd();		
			return;
		}
		TNode * endNode = Tree;
		TNode * prevNode = endNode;
		End->Up->Right = 0;
		do
		{
			prevNode = endNode;

			if (!less(endNode->Value, value) && !less(value, endNode->Value)) //== here
			{
				return;
			}
			if (less(endNode->Value, value))
			{
				endNode = endNode->Right;
			}
			else
			{
				endNode = endNode->Left;
			}
		} while (endNode);

		TNode * newNode = new TNode(value);

		(less(prevNode->Value, value)) ? prevNode->Right = newNode : prevNode->Left = newNode;
		newNode->Up = prevNode;
		SearchEnd();
		return ;

	}
	size_type size() 
	{
		if (Tree != nullptr)
		{
			size_t Size = 0;
			for (iterator it = begin(); it != end(); ++it)
			{
				Size++;
			}
			return Size;
		}
		return 0;
	}
	iterator lower_bound(const Key& key)
	{
		iterator it = begin();
		while (less(*it, key) && it!= end())
		{
			++it;
		}
		return it;
	}
	iterator upper_bound(const Key& key)
	{
		iterator it = begin();
		while (!less(key, *it) && it != end())
		{
			++it;
		}
		return it;
	}
	bool operator ==(const SetClass& obj)
	{
		if (size() != obj.size())
		{
			return false;
		}	
		else if (Tree != nullptr && obj.Tree != nullptr)
		{		
			iterator tree2 = obj.cbegin();
			for (iterator tree1 = cbegin(); tree1 != cend(); ++tree1, ++tree2)
			{
				if ((*tree1) != (*tree2))
				{
					return false;
				}
			}		
		}
		return true;
	}
};
template <class T, class Compare>
void swap(SetClass<T, Compare>& lhs, SetClass <T, Compare>& rhs)
{
	lhs.swap(rhs);
}
