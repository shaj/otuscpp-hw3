# otuscpp-hw3
Homework 3

## Умные указатели
```
вызов foo(1, 2, 3);
внутри
(std::max<int>(2, args))...
развернется в
std::max<int>(2, 1), std::max<int>(2, 2),
std::max<int>(2, 3)
```

### почитать
- https://github.com/AnthonyCalandra/modern-cppfeatures/blob/master/CPP11.md
- https://github.com/AnthonyCalandra/modern-cppfeatures/blob/master/CPP14.md
- http://www.ozon.ru/context/detail/id/34747131/
- http://cppreference.com/


## Умные указатели
### Область видимости
В принципе все понятно
### Время жизни
В общем тоже все понятно
### Сырые/нативные указатели
Наша цель обернуть динамическое время жизни

### unique_ptr
```
  std::unique_ptr<int> p{new int{42}};
  auto p = std::make_unique<int>(12);
  *, ->, get()
```

### *Handler*
- С-шная обёртка для указателя с конструктором и деструктором.

###smart_ptr Зато своё, и еще лучше
Тоже в принципе все понятно.
### почитать
- http://en.cppreference.com/w/cpp/memory/unique_ptr
- http://en.cppreference.com/w/cpp/memory/shared_ptr
- http://en.cppreference.com/w/cpp/memory/weak_ptr
- http://www.ozon.ru/context/detail/id/34747131/

## Аллокатор

```
template <typename U , typename . . . Args >
void construct (U* p , Args&& . . . a rgs ) {
    new( p ) U ( s td : : forward <Args > ( a rgs ) . . . ) ;
} ;
```
```
void destroy ( T* p ) {
    p−>~T ( ) ;
}
```
###почитать
- http://en.cppreference.com/w/cpp/memory/allocator
- http://www.ozon.ru/context/detail/id/3829080/
