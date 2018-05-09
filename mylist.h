
#pragma once

#include <memory>
#include <iterator>



namespace my 
{

/// Шаблон класса односвязного списка
template< typename T, typename Allocator=std::allocator<T>>
class mylist 
{
private:
	template<typename U>
	struct Node;

public:

	template<typename U>
	class Iterator;


	using value_type = T;
	using pointer = T*;
	using const_pointer = const T*;
	using reference = T&;
	using const_reference = const T&;
	typedef Iterator<T> iterator;
	

	typedef typename Allocator::template rebind<Node<T>>::other Node_alloc_type;

private:

	class del_alloc
	{
	private:
		Node_alloc_type *alloc;
		del_alloc(){};
	public:
		del_alloc(Node_alloc_type *a): alloc(a) {}

		void operator()(Node<T> *ptr)
		{
			alloc->destroy(ptr);
			alloc->deallocate(ptr, 1);
		}
	};

	class del_nop
	{
	public:
		void operator()(Node<T> *ptr){}
	};


	/// Структура узла односвязного списка
	template<typename U>
	struct Node 
	{
	private:
		Node_alloc_type *alloc;

		Node() {}
	public:
		Node(Node_alloc_type *a): alloc(a), m_next( nullptr, a ) { }
		Node( const U& t ) : m_t( t ), m_next( nullptr, alloc ) { }
		U m_t;                                                         ///< Значение узла
		std::unique_ptr<Node<U>, del_alloc> m_next {nullptr, alloc};   ///< Указатель на следующий узел
	};


	Node_alloc_type alloc;                                             ///< Управление памятью

	std::unique_ptr<Node<T>, del_alloc> m_head {nullptr, &alloc};      ///< Первый элемент списка
	std::unique_ptr<Node<T>, del_nop> m_tail {nullptr};                ///< Последний элемент списка

public:
	/// Класс итератора односвязного списка
	template<typename U>
	class Iterator 
	{
	public:

		using value_type = T;
		using pointer = T*;
		using const_pointer = const T*;
		using reference = T&;
		using const_reference = const T&;

		Iterator( Node<U>* node ) : m_node( node ) { }

		// Проверка на равенство
		bool operator==( const Iterator& other ) const 
		{
			if( this == &other ) 
			{
				return true;
			}
			return m_node == other.m_node;
		}

		// Проверка на неравенство
		bool operator!=( const Iterator& other ) const 
		{
			return !operator==( other );
		}

		// Получение значения текущего узла
		T operator*() const 
		{
			if( m_node ) 
			{
				return m_node->m_t;
			} // Иначе достигнут конец списка. Уместно возбудить исключение
			return T();
		}

		// Переход к следующему узлу
		void operator++() 
		{
			if( m_node ) 
			{
				m_node = m_node->m_next.get();
			} // Иначе достигнут конец списка. Уместно возбудить исключение
		}

	private:
		Node<T>* m_node;
	};


public:

	mylist() noexcept
	: alloc()
	{ }

	~mylist()
	{
		while( m_head ) 
		{
			remove();
		}
	}

	/// Добавление узла в список
	void append( const T& t )
	{
		if( Node<T>* node = alloc.allocate(1)) 
		{
			alloc.construct(node, t);
			if(m_head.get() == nullptr)
			{
				m_head.reset(node);
				m_tail.reset(node);
			}
			else
			{
				m_tail->m_next.reset(node);
				m_tail.release();
				m_tail.reset(node);
			}

		}
	}

	/**
	 * Функция для std::generate_n
	 */
	Iterator<T> insert(const Iterator<T> pos, T&& value )
	{
		append(value);
		return Iterator<T>(m_tail.get());
	}
	
	/// Удаление первого узла из списка
	void remove()
	{
		if(m_head.get() != nullptr) 
		{ // Если список не пуст:
			// Сохраняем указатель на второй узел, который станет новым головным элементом
			Node<T>* newHead = m_tail->m_next.get();
			// Назначаем новый головной элемент
			m_head.reset(newHead);
		}
	}
	
	/// Получить головной элемент списка
	T head() const{return m_head;}

	/// Получить итератор на начало списка
	Iterator<T> begin() const
	{
		// Итератор пойдет от головного элемента...
		return Iterator<T>( m_head.get() );
	}

	/// Получить итератор на конец списка
	Iterator<T> end() const
	{
		return Iterator<T>( nullptr );
	}

};


} // namespace my
