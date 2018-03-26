
#include <list>


template<typename T>
struct logging_allocator
{
	using value_type = T;
	using pointer = T*;
	using const_pointer = const T*;
	using reference = T&;
	using const_reference = const T&;

	template<typename U>
	struct rebind
	{
		using other = logging_allocator<U>;
	};

	class _bs
	{
		int value;
	public:
		_bs():value(1){}
		~_bs(){}
		int & operator=(const int &i)
		{
			if((i>0) && (i<20 /*max i*/))
			{
				value = i;
			}
		}
		operator int () {return value;}
	} block_size;


	std::list<T*> alloc_mem;
	int free_ptr;

	logging_allocator(){}
	~logging_allocator()
	{
		for(auto it: alloc_mem)
		{
			std::free(it);
		}
	}


	T *allocate(std::size_t n)
	{
		std::cout << __PRETTY_FUNCTION__ << "[n = " << n << "]" << std::endl;
		auto p = std::malloc(n * sizeof(T));
		if(!p) throw std::bad_alloc();
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


