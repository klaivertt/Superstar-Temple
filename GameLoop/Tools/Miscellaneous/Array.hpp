#pragma once

// An Array is like a std::vector use what you prefer
template <typename T>
class Array
{
public:
	Array(void);
	~Array(void);

	void Add(T _value);
	void Remove(int _index);
	void RemoveLast(void);
	void Clear(void);
	T Get(int _index);

	int Size(void);
	void SetCapacity(int _capacity);

	T& operator[] (int _index)
	{
		return data[_index];
	}

	Array<T> operator = (Array<T> _other)
	{
		return _other;
	}
private:
	T* data = nullptr;
	unsigned int capacity = 0;
	unsigned int size = 0;
};

template<typename T>
inline Array<T>::Array(void)
{
	data = new T[1];
	capacity = 1;
	size = 0;
}

template<typename T>
inline Array<T>::~Array(void)
{
	if (data != nullptr)
	{
		//delete[] data;
	}
}

template<typename T>
inline void Array<T>::Add(T _value)
{
	if (size < capacity)
	{
		data[size] = _value;
		size++;
	}
	else
	{
		T* oldTab = data;

		capacity *= 2;
		data = new T[capacity];

		for (unsigned int i = 0; i < size; i++)
		{
			data[i] = oldTab[i];
		}

		data[size] = _value;
		size++;
	}
}

template<typename T>
inline void Array<T>::Remove(int _index)
{
	if (size > 0)
	{
		for (unsigned int i = _index; i < size - 1; i++)
		{
			data[i] = data[i + 1];
		}
		size--;

		if (size < capacity / 2)
		{
			T* oldTab = data;

			capacity /= 2;
			data = new T[capacity];

			for (unsigned int i = 0; i < size; i++)
			{
				data[i] = oldTab[i];
			}
		}
	}
}

template<typename T>
inline void Array<T>::RemoveLast(void)
{
	size--;
}

template<typename T>
inline void Array<T>::Clear(void)
{
	delete[] data;

	capacity = 1;
	size = 0;
	data = new T[capacity];
}

template<typename T>
inline T Array<T>::Get(int _index)
{
	return data[_index];
}

template<typename T>
inline int Array<T>::Size(void)
{
	return size;
}

template<typename T>
inline void Array<T>::SetCapacity(int _capacity)
{
	capacity = _capacity;

	T* oldTab = data;

	data = new T[capacity];

	for (int i = 0; i < size; i++)
	{
		data[i] = oldTab[i];
	}
}
