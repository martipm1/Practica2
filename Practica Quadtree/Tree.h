#ifndef _Tree_H_
#define _Tree_H_

#include <stdlib.h>
#include "p2List.h"
#include "Stack.h"

template <class TYPE>
class NodeTree
{
public:
	
	TYPE data;
	NodeTree* parent;
	p2List<NodeTree*> siblings;

	//Constructor
	NodeTree(const TYPE& newData)
	{
		data = newData;
		parent = NULL;
	}

	//Destructor
	~NodeTree()
	{}

	void AddSibling(NodeTree* node)
	{
		siblings.add(node);
		node->parent = this;
	}

	void RemoveSibling(NodeTree* node)
	{
		p2List_item<NodeTree*>* tmp = sibling.getFirst();

		for (; tmp != NULL; tmp = tmp->next)
		{
			NodeTree* sibling = tmp->data;

			if (node == sibling)
			{
				siblings.del(tmp);
				node->parent = NULL;
				break;
			}
		}
	}

	//Find nodes inside the tree
	NodeTree<TYPE>* Find(const TYPE& node)
	{
		if (node == data)
		{
			return this;
		}

		NodeTree<TYPE>* father = NULL;
		p2List_item<NodeTree*>* tmp = siblings.getFirst()

		for (; tmp != NULL; tmp = tmp->next)
		{
			NodeTree* sibling = tmp->data;
			father = sibling->Find(node);

			if (father != NULL)
				break;
		}

		return father;
	}

	void GetAllNodes(p2List<NodeTree<TYPE>*>* list)
	{
		list->add(this);

		p2List_item<NodeTree*>* tmp = siblings.getFirst();

		for (; tmp != NULL; tmp = tmp->next)
		{
			NodeTree* sibling = tmp->data;
			sibling->GetAllNodes(list);
		}
	}

	//Visit tree Recursives
	void PreOrderRecursive(p2List<NodeTree<TYPE>*>* list)
	{
		list->add(this);
		p2List_item<NodeTree*>* tmp = siblings.getFirst();

		for (; tmp != NULL; tmp = tmp->next)
		{
			tmp->data->PreOrderRecursive(list);
		}

		list->add(this);
	}

	void PostOrderRecursive(p2List<NodeTree<TYPE>*>* list)
	{
		p2List_item<NodeTree*>* tmp = siblings.getFirst();

		for (; tmp != NULL; tmp = tmp->next)
		{
			tmp->data->PostOrderRecursive(list);
		}

		list->add(this);
	}

	void InOrderRecursive(p2List<NodeTree<TYPE>*>* list)
	{
		p2List_item<NodeTree*>* tmp = siblings.getFirst();
		unsigned int half = siblings.Count() / 2;

		for (int i = 0; i < half; i ++, tmp = tmp->next)
		{
			tmp->data->InOrderRecursive(list);
		}

		list->add(this);

		for (; tmp != NULL; tmp = tmp->next)
		{
			tmp->data->InOrderRecursive(list);
		}
	}
};

template <class TYPE>
class Tree
{
public:
	//Attribute(s)
	NodeTree<TYPE> root;

	//Constructor
	Tree(const TYPE& newData) : root(newData)
	{}

	//Destructor
	virtual ~Tree()
	{}

	//Add nodes
	NodeTree<TYPE>* Add(const TYPE& data)
	{
		NodeTree<TYPE>* newSibling = new NodeTree<TYPE>(data);
		root->AddSibling(newSibling);

		return newSibling;
	}

	
	NodeTree<TYPE>* Add(const TYPE& newData, const TYPE& position)
	{
		NodeTree<TYPE>* father = root.Find(position);
		NodeTree<TYPE>* newSibling = new NodeTree<TYPE>(newData);
		father->AddSibling(newSibling);

		return newSibling;
	}
	
	//Clear tree
	void Clear()
	{
		p2List<NodeTree<TYPE>*> results;
		root.GetAllNodes(&results);
		p2List_item<NodeTree<TYPE>*>* tmp = results.getFirst();

		for (; tmp != NULL; tmp = tmp->next)
		{
			NodeTree<TYPE>* sibling = tmp->data;
			if (sibling->parent)
			{
				sibling->parent->RemoveSibling(sibling);
			}
		}
	}

	//Visit all recursives
	void PreOrderRecursive(p2List<NodeTree<TYPE>*>* list)
	{
		root.PreOrderRecursive(list);
	}

	void PostOrderRecursive(p2List<NodeTree<TYPE>*>* list)
	{
		root.PostOrderRecursive(list);
	}

	void InOrderRecursive(p2List<NodeTree<TYPE>*>* list)
	{
		root.InOrderRecursive(list);
	}

	//Visit all iteratives
	void PreOrderIterative(p2List<NodeTree<TYPE>*>* list)
	{
		Stack<NodeTree<TYPE>*> stack;
		NodeTree<TYPE>* node = &root;

		while (node != NULL || stack.Pop(node))
		{
			list->add(node);
			p2List_item<NodeTree<TYPE>*> tmp = node->siblings.getLast();

			for (; tmp != node->siblings.getFirst(); tmp = tmp->prev)
			{
				stack.Push(tmp.data);
			}
			
			if (tmp != NULL)
			{
				node = tmp.data;
			}

			else
			{
				node = NULL;
			}
		}
	}

	void PostOrderIterative(p2List<NodeTree<TYPE>*>* list)
	{
		Stack<NodeTree<TYPE>*> stack;
		NodeTree<TYPE>* node = &root;

		while (node != NULL || stack.Pop(node))
		{
			p2List_item<TYPE>* tmp = node->siblings->getLast();

			if (tmp != NULL && list->find(tmp->data) == -1)
			{
				stack.Push(node);
				for (; tmp != node->siblings.getFirst(); tmp = tmp->prev)
					stack.Push(tmp->data);

				node = tmp->data;
			}

			else
			{
				list->add(node);
				node = NULL;
			}
		}
	}
	
	void InOrderIterative(p2List<NodeTree<TYPE>*>* list) const
	{
		Stack<NodeTree<TYPE>*> stack;
		NodeTree<TYPE>* node = &root;
		bool notFinished = true;

		while (notFinished)
		{
			if (node != NULL)
			{
				if (list->find(node) == -1)
				{
					stack.Push(node);
				}
				if (node->siblings.count() != 0)
				{
					node->siblings.getFirst()->data
				}
				else
				{
					node = NULL;
				}
			}

			else if (stack.Count() > 0)
			{
				stack.Pop(node);
				list->add(node);
				if (node->siblings.getElements() != 0)
				{
					node = node->siblings.getLast()->data;
				}
				else
				{
					node = NULL;
				}
			}
			else
			{
				notFinished = false;
			}
		}
	}
};

#endif _Tree_H_