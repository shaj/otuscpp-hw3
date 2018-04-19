
/// Работа std::unique_ptr с std::allocator
/// Отсюда: https://stackoverflow.com/a/33845812
template<typename T>
std::unique_ptr<T[], std::function<void(T *)>> make_T(X *ptr, 
                                                      std::allocator<T> alloc, 
                                                      std::size_t size) 
{
    auto deleter = [](T *p, std::allocator<T> alloc, std::size_t size) 
    {
        for (int i = 0; i < size; ++i) 
        {
            alloc.destroy(&p[i]);
        }
        alloc.deallocate(p, sizeof(T) * size);
    };

    return {ptr, std::bind(deleter, std::placeholders::_1, alloc, size)};
}



int main(int argc, const char * argv[]) {
    std::allocator<X> alloc = std::allocator<X>();

    X *p = alloc.allocate(5);
    for (int i = 0; i < 5; ++i) {
        alloc.construct(&p[i], i + 1);
    }

    auto ptr = make_T(p, alloc, 5);

    return 0;
}

