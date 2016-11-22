#ifndef IG_HOOK_IG_HOOK_ALLOC_H
# define IG_HOOK_IG_HOOK_ALLOC_H

//<<<<<< INCLUDES                                                       >>>>>>

# include <memory>
# include <cstdlib>
# if __GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 3)
// http://gcc.gnu.org/onlinedocs/libstdc++/20_util/allocator.html
#  include <ext/pool_allocator.h>
# endif

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

# if __GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 3)
template <class T> class IgHookAlloc : public __gnu_cxx::__pool_alloc<T> {};
# elif __GNUC__ == 3 && __GNUC_MINOR__ <= 2
template <class T>
class IgHookAlloc : public std::__allocator<T, std::__default_alloc_template<true,-789> >
{
};
# else
template <class T>
class IgHookAlloc : public std::allocator<T>
{
public:
    template <class O> struct rebind
    {
	typedef IgHookAlloc<O> other;
    };

    typedef typename std::allocator<T>::size_type	size_type;
    typedef typename std::allocator<T>::pointer		pointer;

    IgHookAlloc (void) throw ();
    IgHookAlloc (const IgHookAlloc &x) throw ();
    template <class O> IgHookAlloc (const IgHookAlloc<O> &x) throw ();
    ~IgHookAlloc (void) throw ();

    T *allocate (size_type n, const T *hint = 0);
    void deallocate (pointer p, size_type count);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

template <class T>
inline
IgHookAlloc<T>::IgHookAlloc (void) throw ()
{}

template <class T>
inline
IgHookAlloc<T>::IgHookAlloc (const IgHookAlloc &x) throw ()
    : std::allocator<T> (x)
{}

template <class T>
template <class O>
inline
IgHookAlloc<T>::IgHookAlloc (const IgHookAlloc<O> &x) throw ()
    : std::allocator<T> (x)
{}

template <class T>
inline
IgHookAlloc<T>::~IgHookAlloc (void) throw ()
{}

template <class T>
inline T *
IgHookAlloc<T>::allocate (size_type n, const T * /* hint = 0 */)
{ return (T *) malloc (n * sizeof (T)); }

template <class T>
inline void
IgHookAlloc<T>::deallocate (pointer p, size_type /* n */)
{ free (p); }

# endif // __GNUC__
#endif // IG_HOOK_IG_HOOK_ALLOC_H
