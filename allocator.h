


#include <list>
#include <boost/log/trivial.hpp>


namespace my {

template<typename T, size_t P>
class logging_allocator
{
    static_assert(P != 0, "no zero capacity!");
    
private:
    std::vector<std::vector<std::array<unsigned char,sizeof(T)>>> pool;
    std::size_t index = 0;

public:
	using value_type = T;
	using pointer = T*;
	using const_pointer = const T*;
	using reference = T&;
	using const_reference = const T&;

	static int BlockSize;

	template<class U>
	struct rebind
	{
		using other = logging_allocator<U, P>;
	};

	logging_allocator() = default;

	template<class U>
	logging_allocator(const logging_allocator<U, P>& other)
	{
		// BOOST_LOG_TRIVIAL(trace) << __PRETTY_FUNCTION__;
	}


	~logging_allocator()
	{
		// BOOST_LOG_TRIVIAL(trace) << __PRETTY_FUNCTION__ << "[as = " << alloc_mem.size() << "]";
	}



	T *allocate(std::size_t n)
	{
		// BOOST_LOG_TRIVIAL(trace) << __PRETTY_FUNCTION__ << "[n = " << n << "]";
        if(n != 1)
            throw std::invalid_argument( "Invalid  number of elements for allocate()" );

        pool.resize(1 + index/P);
        pool[index/P].resize(P);
        T* p = reinterpret_cast<T*>(&pool[index/P][index%P]);
        index += n;
        return p;
	}

	void deallocate(T *p, std::size_t n)
	{
		// BOOST_LOG_TRIVIAL(trace) << __PRETTY_FUNCTION__ << "[n = " << n << "]";
	}

	template<typename U, typename ...Args>
	void construct(U *p, Args &&...args)
	{
		// BOOST_LOG_TRIVIAL(trace) << __PRETTY_FUNCTION__;
		new(p) U(std::forward<Args>(args)...);
	}

	void destroy(T *p)
	{
		// BOOST_LOG_TRIVIAL(trace) << __PRETTY_FUNCTION__;
		p->~T();
	}
};

} // namespace my
