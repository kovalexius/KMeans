#include <iostream>
#include <functional>

#include <test_index_locker.h>

class A
{
public:
    A(int num)
    {
        a = (int)num << 1;
        b = (int)num << 2;
        c = (int)num << 3;
        d = (int)num << 4;
        ch = (char)num >> 2;
        ah = (char)num >> 3;
    }

    void print()
    {
        std::cout << "a = " << a << " b = " << b << " c = " << c << " d = " << d << " ch = " << (int)ch << " ah = " << (int)ah << std::endl;
    }

    int a;
    int b;
    int c;
    int d;
    char ch;
    char ah;
};

template<> 
struct std::hash<A>
{
    size_t operator() (const A & val) const
    {
        return val.a + val.b + val.c + val.d + (int)val.ch + (int)val.ah;
    }
};

int main()
{
    /*
    A a(3);
    std::hash<A> hash_fn;
    size_t str_hash = hash_fn( a );
    std::cout << str_hash << '\n';
    */

    test::test_index_locker();

    std::cin.get();
}