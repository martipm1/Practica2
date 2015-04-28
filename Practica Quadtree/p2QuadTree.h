// ----------------------------------------------------
// Quadtree implementation --
// ----------------------------------------------------

#ifndef __P2QUADTREE_H__
#define __P2QUADTREE_H__

#include "Collider.h"
#include "p2DynArray.h"

#define QUADTREE_MAX_ITEMS 2

// Helper function to check if one rectangle complately contains another
bool Contains(const SDL_Rect& a, const SDL_Rect& b);
bool Intersects(const SDL_Rect& a, const SDL_Rect& b);

// Tree node -------------------------------------------------------
class p2QuadTreeNode
{

public:

	SDL_Rect				rect;
	p2DynArray<Collider*>	objects;
	p2QuadTreeNode*			parent;
	p2QuadTreeNode*			childs[4];

public:

	p2QuadTreeNode(SDL_Rect r) :
		rect(r),
		objects(QUADTREE_MAX_ITEMS)
	{
		parent = childs[0] = childs[1] = childs[2] = childs[3] = NULL;
	}

	~p2QuadTreeNode()
	{
		for (int i = 0; i < 4; ++i)
		if (childs[i] != NULL) delete childs[i];
	}

	void Insert(Collider* col)
	{
		// TODO: Insertar un nou Collider al quadtree
		if (childs[0] == NULL)
		{
			if (objects.Count() >= QUADTREE_MAX_ITEMS - 1)
			{
				childs[0] = new p2QuadTreeNode(SDL_Rect{ rect.x, rect.y, rect.w / 2, rect.h / 2 });
				childs[1] = new p2QuadTreeNode(SDL_Rect{ rect.x + (rect.w / 2), rect.y, rect.w / 2, rect.h / 2 });
				childs[2] = new p2QuadTreeNode(SDL_Rect{ rect.x, rect.y + (rect.h / 2), rect.w / 2, rect.h / 2 });
				childs[3] = new p2QuadTreeNode(SDL_Rect{ rect.x + (rect.w / 2), rect.y + (rect.h / 2), rect.w / 2, rect.h / 2 });

				for (unsigned int i = 0; i <= 3; i++)
				{
					childs[i]->parent = this;
				}

				p2DynArray<Collider*> tmp_darray = objects;
				objects.Clear();

				for (int i = 0; i < tmp_darray.Count(); i++)
				{
					Insert(tmp_darray[i]);
				}
				Insert(col);
			}

			else
			{
				objects.PushBack(col);
			}
		}

		else
		{
			if (colisionsChilds(col))
			{
				objects.PushBack(col);
			}

			else
			{
				for (int i = 0; i < 4; i++)
				{
					if (Intersects(col->rect, childs[i]->rect))
					{
						childs[i]->Insert(col);
					}
				}
			}
		}
	}

	int CollectCandidates(p2DynArray<Collider*>& nodes, const SDL_Rect& r)
	{
		int intersections = 0;

		for (int i = 0; i < objects.Count(); i++)
		{
			nodes.PushBack(objects[i]);
			intersections++;
		}

		if (childs[0] != NULL)
		{

			for (int i = 0; i < 4; i++)
			{
				if (Intersects(childs[i]->rect, r))
				{
					intersections += childs[i]->CollectCandidates(nodes, r);
				}
			}
		}

		return intersections;
	}

	void CollectRects(p2DynArray<p2QuadTreeNode*>& nodes)
	{
		nodes.PushBack(this);

		for (int i = 0; i < 4; ++i)
		if (childs[i] != NULL) childs[i]->CollectRects(nodes);
	}

	bool colisionsChilds(Collider* col)const
	{
		int colisions_num = 0;

		for (int i = 0; i < 4; i++)
		{
			if (Intersects(col->rect, childs[i]->rect))
			{
				++colisions_num;
			}
		}
		if (colisions_num == 4)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
};

// Tree class -------------------------------------------------------
class p2QuadTree
{
public:

	// Constructor
	p2QuadTree() : root(NULL)
	{}

	// Destructor
	virtual ~p2QuadTree()
	{
		Clear();
	}

	void SetBoundaries(const SDL_Rect& r)
	{
		if(root != NULL)
			delete root;

		root = new p2QuadTreeNode(r);
	}

	void Insert(Collider* col)
	{
		if(root != NULL)
		{
			if(Intersects(root->rect, col->rect))
				root->Insert(col);
		}
	}

	void Clear()
	{
		if(root != NULL)
		{
			delete root;
			root = NULL;
		}
	}

	int CollectCandidates(p2DynArray<Collider*>& nodes, const SDL_Rect& r) const
	{
		int tests = 1;
		if(root != NULL && Intersects(root->rect, r))
			tests = root->CollectCandidates(nodes, r);
		return tests;
	}

	void CollectRects(p2DynArray<p2QuadTreeNode*>& nodes) const
	{
		if(root != NULL)
			root->CollectRects(nodes);
	}

public:

	p2QuadTreeNode*	root;

};

#endif // __p2QuadTree_H__