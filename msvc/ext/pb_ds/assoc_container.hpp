#pragma once
#include <unordered_map>
#include <unordered_set>
namespace std
{
    namespace __gnu_pbds
    {
        struct null_type
        {
        };

        //template <typename Key, typename Value, typename Hash, typename Compare = equal_to<Key>>
        //struct gp_hash_table : public unordered_map<Key, Value, Hash, Compare>
        //{
        //    using hasher = _Hasher;
        //    using key_type = _Kty;
        //    using mapped_type = _Ty;
        //    using key_equal = _Keyeq;

        //    using value_type = pair<const _Kty, _Ty>;
        //    using allocator_type = typename _Mybase::allocator_type;
        //    using size_type = typename _Mybase::size_type;
        //    using difference_type = typename _Mybase::difference_type;
        //    using pointer = typename _Mybase::pointer;
        //    using const_pointer = typename _Mybase::const_pointer;
        //    using reference = value_type&;
        //    using const_reference = const value_type&;
        //    using iterator = typename _Mybase::iterator;
        //    using const_iterator = typename _Mybase::const_iterator;

        //    using local_iterator = typename _Mybase::iterator;
        //    using const_local_iterator = typename _Mybase::const_iterator;
        //};

        //template <typename Key, typename Hash, typename Compare = equal_to<Key>>
        //struct gp_hash_table <Key, null_type, Hash, Compare> : public unordered_set<Key, Hash, Compare>
        //{

        //};
    }

    /*template <typename Key, typename Value, typename Hash, typename Compare>
    void swap(__gnu_pbds::gp_hash_table<Key, Value, Hash, Compare>& g1, __gnu_pbds::gp_hash_table<Key, Value, Hash, Compare>& g2) noexcept
    {
        g1.swap(g2);
    }*/
}