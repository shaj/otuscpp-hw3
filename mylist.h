
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
	template<typename _T>
	struct Node;

public:

	template<typename _T>
	class Iterator;


	using value_type = T;
	using pointer = T*;
	using const_pointer = const T*;
	using reference = T&;
	using const_reference = const T&;
	typedef Iterator<T> iterator;
	

	typedef typename Allocator::template rebind<Node<T>>::other Node_alloc_type;

private:

	/// Структура узла односвязного списка
	template<typename _T>
	struct Node 
	{
		Node() : m_next( nullptr ) { }
		Node( const _T& t ) : m_t( t ), m_next( nullptr ) { }
		_T m_t;             ///< Значение узла
		std::unique_ptr<Node<T>> m_next;   ///< Указатель на следующий узел
	};


	Node_alloc_type alloc;  ///< Управление памятью

	std::unique_ptr<Node<T>> m_head;        ///< Первый элемент списка
	std::unique_ptr<Node<T>> m_last;        ///< Последний элемент списка

public:
	/// Класс итератора односвязного списка
	template<typename _T>
	class Iterator 
	{
	public:

		using value_type = T;
		using pointer = T*;
		using const_pointer = const T*;
		using reference = T&;
		using const_reference = const T&;

		Iterator( Node<_T>* node ) : m_node( node ) { }

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
				m_node = m_node->m_next;
			} // Иначе достигнут конец списка. Уместно возбудить исключение
		}

	private:
		Node<T>* m_node;
	};


public:

	mylist() noexcept
	: alloc(), m_head(), m_last()
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
		// Создаем новый узел для значения
		// Не забудем проверить, что память удалось выделить
		if( Node<T>* node = alloc.allocate(1)) 
		{
			alloc.construct(node, t);
			node->m_next = nullptr;
			if(m_head == nullptr)
			{
				m_head = node;
				m_last = node;
			}
			else
			{
				m_last->m_next = node;
				m_last = node;
			}
		}
	}

	/**
	 *
	 */
	Iterator<T> insert(const Iterator<T> pos, T&& value )
	{
		if( Node<T>* node = alloc.allocate(1)) 
		{
			alloc.construct(node, std::move(value));
			node->m_next = nullptr;
			if(m_head == nullptr)
			{
				m_head = node;
				m_last = node;
			}
			else
			{
				m_last->m_next = node;
				m_last = node;
			}
			return Iterator<T>(node);
		}
		return Iterator<T>(nullptr);
	}
	
	// Удаление первого узла из списка
	void remove()
	{
		if(m_head) 
		{ // Если список не пуст:
			// Сохраняем указатель на второй узел, который станет новым головным элементом
			Node<T>* newHead = m_head->m_next;
			// Освобождаем память, выделенную для удаляемого головного элемента
			alloc.destroy(m_head);
			alloc.deallocate(m_head, 1);
			// Назначаем новый головной элемент
			m_head = newHead;
		}
	}
	
	/// Получить головной элемент списка
	T head() const{return m_head;}

	/// Получить итератор на начало списка
	Iterator<T> begin() const
	{
		// Итератор пойдет от головного элемента...
		return Iterator<T>( m_head );
	}

	/// Получить итератор на конец списка
	Iterator<T> end() const
	{
		return Iterator<T>( nullptr );
	}

};


} // namespace my
