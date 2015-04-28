#ifndef _Stack_H_
#define _Stack_H_

#define STACK_BLOCK_SIZE 16 //Liked the idea of being able to add memory blocks its size is 16

template <class TYPE>

class Stack
{
private:

	TYPE* data;
	unsigned int memAlloc;
	unsigned int nElements;

public:

	//CONSTRUCTORS
	Stack() : memAlloc(0), nElements(0), data(NULL)
	{
		ReAllocate(STACK_BLOCK_SIZE);
	}

	Stack(unsigned int newMem) : memAlloc(0), nElements(0), data(NULL)
	{
		ReAllocate(newMem);
	}

	//DESTRUCTORS
	~Stack()
	{
		if (data != NULL){
			delete[]data;
		}
	}	

	//STACK MANAGEMENT
	void Push(const TYPE& value)
	{
		if (nElements >= memAlloc)
		{
			ReAllocate(memAlloc + STACK_BLOCK_SIZE)
		}
		data[nElements++] = value;
	}

	TYPE Pop()
	{
		if (data != NULL && nElements > 0)
		{
			unsigned int i = nElements - 1;
			TYPE& ret = data[i];
			nElements--;
			return ret;
		}
		else
		{
			return NULL;
		}
	}

	bool Pop(TYPE& value)
	{
		if (nElements > 0)
		{
			value = data[nElements--];
			return true
		}
		return false;
	}

	//GETTERS
	const TYPE* Peek(unsigned int position) const
	{
		if (nElements > position)
		{
			return &data[position];
		}

		else
		{
			return NULL;
		}
	}
	
	int getElements() const
	{
		return nElements;
	}

	int getMemory() const
	{
		return memAlloc;
	}

	//RESIZE || CLEAR ARRAY
	void ReAllocate(const unsigned int newMemory)
	{
		TYPE* tmp = data;
		memAlloc = newMemory;
		data = new TYPE[memAlloc];

		if (nElements > memAlloc)
		{
			nElements = memAlloc;
		}

		if (tmp != NULL)
		{
			for (int i = 0; i < nElements; ++i)
				data[i] = tmp[i];

			delete[] tmp;
		}
	}

	void Clear()
	{
		nElements = 0;
	}
};

#endif _Stack_H_