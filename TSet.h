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
		key_type Value;
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
			: Value(0)
			, Right(nullptr)
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


		bool operator=(const TSetIterator &iterator)
		{
			Ptr = iterator.Ptr;
			return Ptr;
		}
		bool operator !=(const TSetIterator& iterator)
		{
			return Ptr != iterator.Ptr;
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
			else 
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
	size_t Size = 0; 
	TNode* Tree;
	TNode* End;
	using iterator = TSetIterator;
	using const_iterator = const TSetIterator;
	void Insert_Branch(iterator it)// for Erase
	{
		iterator pos(it);
		if (pos.Ptr == nullptr)
		{
			return;
		}
		if (pos.Ptr->Right != nullptr)
		{
			Insert_Branch(pos.Ptr->Right);
		}
		if (pos.Ptr->Left != nullptr)
		{
			Insert_Branch(pos.Ptr->Left);
		}	
		insert(pos.Ptr->Value);
		Size--; //Т.к. вставляем старые. Удаляемый элемент уберет Erase.
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
	void DeleteTree(TNode **For_delete)
	{
		if ((*For_delete) == nullptr)
		{
			return;
		}
		if ((*For_delete)->Left != nullptr)
		{
			DeleteTree(&((*For_delete)->Left));
		}
		if ((*For_delete)->Right != nullptr)
		{
			DeleteTree(&((*For_delete)->Right));
		}
		(*For_delete) = nullptr;
		
		Size = 0;
	}
	
public:

	SetClass()
	{
		Tree = nullptr;
	}
	SetClass(std::initializer_list<value_type> list) 
	{
		Tree =  nullptr;
		for (value_type item : list) 
		{
			insert(item);
		}
	}
	~SetClass()
	{
		DeleteTree(&Tree);
	}
	iterator begin()
	{
		TNode* tmp = Tree;
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
		DeleteTree(&Tree);
	}
	void erase(const key_type& key)
	{
		Size --;
		if (Size == 0) 
		{
			Tree = nullptr;
			delete Tree;	
			return;
		}
		if (Tree->Value == key)
		{
			DeleteTree(&Tree);
			return;
		}
		iterator Position = find(key);
	
		if (Position.Ptr == End->Up) //Если крайний правый - смещаем end.
		{
			End = Position.Ptr;
			End->Up = Position.Ptr->Up;
			return;
		}
		else if (Position.Ptr->Left == nullptr && Position.Ptr->Right == nullptr) // Если удаляемый - последний.
		{
		
			if (Position.Ptr->Value < Position.Ptr->Up->Value) //Могут стоять другие элементы слева/справа. Это - чтобы не потерять связи
			{
				Position.Ptr->Up->Left = nullptr;
			}
			else
			{
				Position.Ptr->Up->Right = nullptr;
			}
		}
		else if (Position.Ptr->Left != nullptr)
		{
			iterator for_insert_Left = Position.Ptr->Left;	
			Position.Ptr->Up->Left = nullptr;
			Insert_Branch(for_insert_Left);
		}
		else if (Position.Ptr->Right != nullptr)
		{
			iterator for_insert_Right = Position.Ptr->Right;
			Position.Ptr->Up->Right = nullptr;
			Insert_Branch(for_insert_Right);
			
		}	
		Position.Ptr = nullptr;
		return;
	}
	iterator find(const Key& key)
	{
		TNode * currNode = Tree;
		do
		{
			if (currNode->Value == key)
			{
				iterator it(currNode);
				return it;
			}
			if (currNode->Value < key)
			{
				currNode = currNode->Right;
			}
			else
			{
				currNode = currNode->Left;
			}
		} while (currNode);

		return end();
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
			Size++;
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

			if (endNode->Value == value)
			{
				return;
			}
			if (endNode->Value < value)
			{
				endNode = endNode->Right;
			}
			else
			{
				endNode = endNode->Left;
			}
		} while (endNode);

		TNode * newNode = new TNode(value);

		(prevNode->Value < value) ? prevNode->Right = newNode : prevNode->Left = newNode;
		Size++;
		newNode->Up = prevNode;
		SearchEnd();
		return ;

	}
	size_type size() const
	{
		return Size;
	}
	iterator lower_bound(const Key& key)
	{
		iterator it = begin();
		while (*it < key && it!= end())
		{
			++it;
		}
		return it;
	}
	iterator upper_bound(const Key& key)
	{
		iterator it = begin();
		while (*it <= key && it != end())
		{
			++it;
		}
		return it;
	}
	bool operator ==(SetClass& obj)
	{
		if (Size != obj.Size)
		{
			return false;
		}
		if (Tree == nullptr && obj.Tree == nullptr)
		{
			return true;
		}
		else if (Tree != nullptr && obj.Tree != nullptr)
		{
			iterator tree1 = this->begin();
			iterator tree2 = obj.begin();
			obj.Tree;
			for (size_t i = 0; i < Size; ++i, ++tree1, ++tree2)
			{
				if ((*tree1) != (*tree2))
				{
					return false;
				}
			}
			return true;
		}
		return false;
	}
};
