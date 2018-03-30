
#include <list>
#include <boost/log/trivial.hpp>


template<typename T, int P>
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
		using other = logging_allocator<U, P>;
	};


	logging_allocator()
	{
		BOOST_LOG_TRIVIAL(trace) << __PRETTY_FUNCTION__;
		// block_size = 3;
		set_block_size(P);
		free_ptr = 0;
	}


	~logging_allocator()
	{
		BOOST_LOG_TRIVIAL(trace) << __PRETTY_FUNCTION__ << "[as = " << alloc_mem.size() << "]";
		for(auto it: alloc_mem)
		{
			BOOST_LOG_TRIVIAL(trace) << it;
			std::free(it);
		}
	}

	void set_block_size(int val)
	{
		BOOST_LOG_TRIVIAL(trace) << __PRETTY_FUNCTION__ << "[bs = " << val << "]";

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
		BOOST_LOG_TRIVIAL(trace) << __PRETTY_FUNCTION__ << "[n = " << n << "]";
		void *p;
		if(n > block_size)
		{ // Просто выделяем новую память запрашиваемого размера
			p = std::malloc(n * sizeof(T));
			alloc_mem.push_back(static_cast<T*>(p));
			BOOST_LOG_TRIVIAL(trace) << "1";
		}
		else
		{
			if(free_ptr == 0)
			{ // Выделенная память закончилась. Нужно выделять новую.
				p = std::malloc(block_size * sizeof(T));
				if(!p) throw std::bad_alloc();
				alloc_mem.push_back(static_cast<T*>(p));
				wrk_ptr = static_cast<T*>(p);
				free_ptr = block_size - n;
				BOOST_LOG_TRIVIAL(trace) << "2";
			}
			else if(n > free_ptr)
			{ // Оставшаяся выделенная память меньше требуемой
				// Не заморачиваемся и выделяем сколько требуется.
				p = std::malloc(n * sizeof(T));
				if(!p) throw std::bad_alloc();
				alloc_mem.push_back(static_cast<T*>(p));
				BOOST_LOG_TRIVIAL(trace) << "3";
			}
			else /* n <= free_ptr */
			{
				p = wrk_ptr + block_size - free_ptr;
				free_ptr -= n;
				BOOST_LOG_TRIVIAL(trace) << "4";
			}
		}
		// if(!p) throw std::bad_alloc();
		BOOST_LOG_TRIVIAL(trace) << p << " ~ " << wrk_ptr << " ~ " << free_ptr << " ~ " << alloc_mem.size() << " ~ ";
		return reinterpret_cast<T *>(p);
	}

	void deallocate(T *p, std::size_t n)
	{
		BOOST_LOG_TRIVIAL(trace) << __PRETTY_FUNCTION__ << "[n = " << n << "]";
		// std::free(p);
	}

	template<typename U, typename ...Args>
	void construct(U *p, Args &&...args)
	{
		BOOST_LOG_TRIVIAL(trace) << __PRETTY_FUNCTION__;
		new(p) U(std::forward<Args>(args)...);
	}

	void destroy(T *p)
	{
		BOOST_LOG_TRIVIAL(trace) << __PRETTY_FUNCTION__;
		p->~T();
	}
};


