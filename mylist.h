


template< typename T, typename Allocator=std::allocator<T>() >
class mylist {
private:

    // Структура узла односвязного списка
    template<typename _T>
    struct Node {
        Node() : m_next( NULL ) { }

        Node( const _T& t ) : m_t( t ), m_next( NULL ) { }

        // Значение узла
        _T m_t;
        
        // Указатель на следующий узел
        Node<_T>* m_next;
    };


    typedef typename __gnu_cxx::__alloc_traits<Allocator>::template  rebind<T>::other _Tp_alloc_type;
    typedef __gnu_cxx::__alloc_traits<_Tp_alloc_type>   _Tp_alloc_traits;
    typedef typename _Tp_alloc_traits::template  rebind<Node<T> >::other _Node_alloc_type;
    typedef __gnu_cxx::__alloc_traits<_Node_alloc_type> _Node_alloc_traits;

    _Node_alloc_type alloc;
    // Голова односвязного списка
    Node<T>* m_head;

public:
    // Класс итератора односвязного списка
    template<typename _T>
    class Iterator {
    public:
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
    : alloc(), m_head()
    { }

    mylist(const _Node_alloc_type& __a) noexcept
    : alloc(__a), m_head()
    { }

    mylist(_Node_alloc_type&& __a) noexcept
    : alloc(std::move(__a)), m_head()
    { }




    ~mylist()
    {
        while( m_head ) 
        {
            remove();
        }
    }

    // Добавление узла в список
    void append( const T& t )
    {
        // Создаем новый узел для значения
        // Не забудем проверить, что память удалось выделить
        if( Node<T>* node = alloc.allocate(sizeof(_Node_alloc_type))) 
        {
            node->m_t = t;
            // Новый узел привязывается к старому головному элементу
            node->m_next = m_head;
            // Новый узел сам становится головным элементом
            m_head = node;
        }
    }
    
    // Удаление последнего добавленного узла из списка
    void remove()
    {
        if( m_head ) 
        { // Если список не пуст:
            // Сохраняем указатель на второй узел, который станет новым головным элементом
            Node<T>* newHead = m_head->m_next;

            // Освобождаем память, выделенную для удаляемого головного элемента
            alloc.deallocate(m_head, sizeof(_Node_alloc_type));
            // delete m_head;

            // Назначаем новый головной элемент
            m_head = newHead;
        } // Иначе могли бы возбудить исключение
    }
    
    // Получить головной элемент списка
    T head() const{return m_head;}

    // Получить итератор на начало списка
    Iterator<T> begin() const
    {
        // Итератор пойдет от головного элемента...
        return Iterator<T>( m_head );
    }

    
    // Получить итератор на конец списка
    Iterator<T> end() const
    {
        // ... и до упора, т.е. NULL
        return Iterator<T>( NULL );
    }

    // Получить размер списка
    size_t size() const
    {
        size_t s = 0;
        for( Iterator<T> it = begin(); it != end(); ++it ) {
            ++s;
        }
        /* 
        Но можно и без итераторов
        for( Node* n = m_head; n != NULL; n = n->m_next ) {
            ++s;
        }
        */

        return s;
    }

private:
};



