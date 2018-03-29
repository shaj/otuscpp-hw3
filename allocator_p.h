
#include <list>


template<typename T>
class logging_allocator
{
private:
	std::list<T*> alloc_mem;
	T *wrk_ptr;
	int free_ptr;
	int block_size;

public:
	using value_type = T;
	using pointer = T*;
	using const_pointer = const T*;
	using reference = T&;
	using const_reference = const T&;

	static int BlockSize;

	template<typename U>
	struct rebind
	{
		using other = logging_allocator<U>;
	};


	logging_allocator()
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
		block_size = 3;
		free_ptr = 0;
	}


	~logging_allocator()
	{
		std::cout << __PRETTY_FUNCTION__ << "[as = " << alloc_mem.size() << "]" << std::endl;
		for(auto it: alloc_mem)
		{
			std::cout << it << std::endl;
			std::free(it);
		}
	}

	void set_block_size(int val)
	{
		std::cout << __PRETTY_FUNCTION__ << "[bs = " << val << "]" << std::endl;

		if(val <= 0)
		{
			block_size = 1;
		}
		else if(val > 20 /*max block size*/)
		{
			block_size = 20;
		}
		else
		{
			block_size = val;
		}

	}

	T *allocate(std::size_t n)
	{
		std::cout << __PRETTY_FUNCTION__ << "[n = " << n << "]" << std::endl;
		void *p;
		if(n > block_size)
		{ // Просто выделяем новую память запрашиваемого размера
			p = std::malloc(n * sizeof(T));
			alloc_mem.push_back(static_cast<T*>(p));
			std::cout << "1";
		}
		else
		{
			if(free_ptr == 0)
			{ // Выделенная память закончилась. Нужно выделять новую.
				p = std::malloc(block_size * sizeof(T));
				if(!p) throw std::bad_alloc();
				wrk_ptr = static_cast<T*>(p);
				free_ptr = block_size - n;
				std::cout << "2";
			}
			else if(n > free_ptr)
			{ // Оставшаяся выделенная память меньше требуемой
				// Не заморачиваемся и выделяем сколько требуется.
				p = std::malloc(n * sizeof(T));
				if(!p) throw std::bad_alloc();
				alloc_mem.push_back(static_cast<T*>(p));
				std::cout << "3";
			}
			else /* n <= free_ptr */
			{
				p = wrk_ptr + block_size - free_ptr;
				free_ptr -= n;
				std::cout << "4";
			}
		}
		// if(!p) throw std::bad_alloc();
		std::cout << std::endl << p << " ~ " << wrk_ptr << " ~ " << free_ptr << " ~ " << alloc_mem.size() << " ~ " << std::endl;
		return reinterpret_cast<T *>(p);
	}

	void deallocate(T *p, std::size_t n)
	{
		std::cout << __PRETTY_FUNCTION__ << "[n = " << n << "]" << std::endl;
		std::free(p);
	}

	template<typename U, typename ...Args>
	void construct(U *p, Args &&...args)
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
		new(p) U(std::forward<Args>(args)...);
	}

	void destroy(T *p)
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
		p->~T();
	}
};


