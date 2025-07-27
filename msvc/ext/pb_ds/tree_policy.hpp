#pragma once
#include "assoc_container.hpp"
#include <random>
#include <chrono>
#include <utility>
#include <vector>
#include <algorithm>
namespace std
{
	namespace __gnu_pbds
	{
		struct rb_tree_tag
		{
			using tree_type = rb_tree_tag;
		};
		template <typename T>
		struct tree_order_statistics_node_update
		{
			tree_order_statistics_node_update* l, * r;
			T         x;
			unsigned  y;
			unsigned  c;

			operator const T& ()
			{
				return x;
			}
		};

		template <typename T, typename Key, typename Compare, typename TreeTag, template<typename T2> typename Node>
		class tree
		{
		};

		template <typename T, typename Compare>
		class tree<T, null_type, Compare, rb_tree_tag, tree_order_statistics_node_update>
		{
		public:
			using Node = tree_order_statistics_node_update<T>;
			using Value = T;
			using iterator = Node*;
			using value_type = T;
			using allocator_type = allocator<T>;
			using pointer = T*;
			using const_pointer = const T*;
			using reference = T&;
			using const_reference = const T&;
			using size_type = size_t;
			using difference_type = size_t;

		private:
			Node* root;
			Compare func;

		protected:
			iterator create(const T& x)
			{
				iterator n = new Node;
				n->c = 1;
				n->x = x;
				n->y = rand();
				n->l = nullptr;
				n->r = nullptr;
				return n;
			}

			static inline size_t size(iterator n)
			{
				if (n == nullptr)
					return 0;
				return n->c;
			}

			static inline void update(iterator r)
			{
				if (r == nullptr)
					return;
				r->c = size(r->l) + size(r->r) + 1;
			}

			iterator find(iterator root, T x) const
			{
				if (root == nullptr)
					return nullptr;
				if (!(func(root->x, x) || func(x, root->x)))
					return root;
				if (func(x, root->x))
					return find(root->l, x);
				return find(root->r, x);
			}

			std::pair <iterator, iterator>split(iterator root, const T& a)
			{
				if (root == nullptr)
					return { nullptr,nullptr };
				if (func(root->x, a))
				{
					auto p = split(root->r, a);
					root->r = p.first;
					update(root);
					return { root, p.second };
				}
				else
				{
					auto p = split(root->l, a);
					root->l = p.second;
					update(root);
					return { p.first, root };
				}
			}

			iterator merge(iterator a, iterator b)
			{
				if (a == nullptr)
					return b;
				if (b == nullptr)
					return a;
				if (a->y < b->y)
				{
					a->r = merge(a->r, b);
					update(a);
					return a;
				}
				b->l = merge(a, b->l);
				update(b);
				return b;
			}

			iterator insert(iterator root, const T& x)
			{
				iterator n = create(x);
				auto p = split(root, x);
				return merge(p.first, merge(n, p.second));
			}

			iterator remove(iterator r, const T& x)
			{
				if (r == nullptr)
					return nullptr;
				if (!(func(r->x, x) || func(x, r->x)))
				{
					iterator t = merge(r->l, r->r);
					delete r;
					return t;
				}
				if (func(x, r->x))
				{
					r->l = remove(r->l, x);
					update(r);
					return r;
				}
				r->r = remove(r->r, x);
				update(r);
				return r;
			}

			iterator lower_bound(iterator root, const T& x) const
			{
				if (root == nullptr)
					return nullptr;
				if (func(root->x, x))
					return lower_bound(root->r, x);
				iterator t = lower_bound(root->l, x);
				if (t != nullptr)
					return t;
				return root;
			}

			iterator upper_bound(iterator root, const T& x) const
			{
				if (root == nullptr)
					return root;
				if (!func(x, root->x))
					return upper_bound(root->r, x);
				iterator it = upper_bound(root->l, x);
				if (it == nullptr)
					return root;
				return it;
			}

			iterator kth_element(iterator root, size_t cnt) const
			{
				if (size(root) <= cnt)
					return nullptr;
				if (size(root->l) == cnt)
					return root;
				if (size(root->l) < cnt)
					return kth_element(root->r, cnt - size(root->l) - 1);
				else
					return kth_element(root->l, cnt);
			}

			size_t order(iterator root, const T& x) const
			{
				if (root == nullptr)
					return 0;
				if ((!func(root->x, x)) && (!func(x, root->x)))
					return size(root->l);
				if (func(root->x, x))
					return size(root->l) + 1 + order(root->r, x);
				return order(root->l, x);
			}

			static iterator min(iterator root)
			{
				if (root == nullptr)
					return root;
				iterator t = min(root->l);
				if (t == nullptr)
					return root;
				return t;
			}

			static iterator max(iterator root)
			{
				if (root == nullptr)
					return root;
				iterator t = max(root->r);
				if (t == nullptr)
					return root;
				return t;
			}

			static void iter(iterator root, vector<iterator>& out)
			{
				if (!root)
					return;
				iter(root->l, out);
				out.push_back(root);
				iter(root->r, out);
			}

			static const T& value_from_iterator(iterator root)
			{
				return root->x;
			}

			static void clear(iterator root)
			{
				if (!root)
					return;
				clear(root->l);
				clear(root->r);
				delete root;
			}

		public:
			//template <typename _T, typename _C>
			class tree_iterator
			{
			public:
				using tree_t = const tree<T, null_type, Compare, rb_tree_tag, tree_order_statistics_node_update>;
				//using tree_iterator = tree_iterator_t<_T, _C>;
				using iterator_category = bidirectional_iterator_tag;
				using value_type = const T;
				using difference_type = size_t;
				using pointer = const T*;
				using reference = const T&;
			private:
				const tree_t* obj;
				tree_order_statistics_node_update<T>* it;

			public:
				const T& operator*() const
				{
					return it->x;
				}

				const T* operator->() const
				{
					return &it->x;
				}

				tree_iterator& operator++()
				{
					it = obj->next(it);
					return *this;
				}

				tree_iterator& operator--()
				{
					it = obj->prev(it);
					return *this;
				}

				tree_iterator operator++(int)
				{
					tree_iterator t = *this;
					it = obj->next(it);
					return t;
				}

				tree_iterator operator--(int)
				{
					tree_iterator t = *this;
					it = obj->prev(it);
					return t;
				}

				bool operator==(const tree_iterator& oth) const
				{
					return it == oth;
				}

				bool operator!=(const tree_iterator& oth) const
				{
					return it != oth;
				}

				operator tree_order_statistics_node_update<T>* () const
				{
					return it;
				}

			public:
				tree_iterator(const void* a, void* b)
				{
					it = reinterpret_cast<tree_t::iterator>(b);
					obj = reinterpret_cast<const tree_t*>(a);
					//obj = p;
					//it = it2;
				}
			};

			//using tree_iterator = tree_iterator_t<T, Compare>;

			tree()
			{
				root = nullptr;
			}

			tree(const Compare& cmp) : func(cmp)
			{
				root = nullptr;
			}

			~tree()
			{
				clear(root);
			}

			/// <summary>
			/// Return iterator for the next element
			/// </summary>
			/// <param name="cur">Current iterator</param>
			/// <returns>Next iterator</returns>
			iterator next(iterator cur) const
			{
				if (!cur)
					return cur;
				return kth_element(root, order(root, cur->x) + 1);
			}

			/// <summary>
			/// Return iterator for the previous element
			/// </summary>
			/// <param name="cur">Current iterator</param>
			/// <returns>Previous iterator</returns>
			iterator prev(iterator cur) const
			{
				if (!cur)
					return max(root);
				return kth_element(root, order(root, cur->x) + 1);
			}

			tree_iterator begin() const
			{
				return tree_iterator(this, min(root));
			}

			tree_iterator end() const
			{
				return tree_iterator(this, nullptr);
			}

			/// <summary>
			/// Return size
			/// </summary>
			inline size_t size() const
			{
				return size(root);
			}

			/// <summary>
			/// Return true if tree is empty
			/// </summary>
			inline bool empty() const
			{
				return size() == 0;
			}

			/// <summary>
			/// Return min element in tree
			/// </summary>
			inline tree_iterator min_element() const
			{
				return tree_iterator(this, min(root));
			}

			/// <summary>
			/// Return max element in tree
			/// </summary>
			inline tree_iterator max_element() const
			{
				return tree_iterator(this, max(root));
			}

			/// <summary>
			/// Find element
			/// </summary>
			/// <param name="x">element to find</param>
			/// <returns>nullptr if element not in list, else iterator of element</returns>
			inline tree_iterator find(const T& x) const
			{
				return tree_iterator(this, find(root, x));
			}

			/// <summary>
			/// Insert element
			/// </summary>
			/// <param name="x">element to insert</param>
			/// <returns>true if element sucessfull inserted</returns>
			bool insert(const T& x)
			{
				if (find(x) != nullptr)
					return false;
				root = insert(root, x);
				return true;
			}

			/// <summary>
			/// Erase element by key
			/// </summary>
			/// <param name="x">element to erase</param>
			/// <returns>true if sucessfull</returns>
			bool erase(const T& x)
			{
				if (find(x) == nullptr)
					return false;
				root = remove(root, x);
				return true;
			}

			/// <summary>
			/// Return tree root
			/// </summary>
			inline iterator data()
			{
				return root;
			}

			/// <summary>
			/// Return kth element in sorted list
			/// </summary>
			/// <param name="x">index of element</param>
			/// <returns>kth element</returns>
			inline tree_iterator find_by_order(size_t k) const
			{
				return tree_iterator(this, kth_element(root, k));
			}

			/// <summary>
			/// Return order of element x
			/// </summary>
			/// <param name="x">Element</param>
			/// <returns>Order of element</returns>
			inline size_t order_of_key(const T& x) const
			{
				return order(root, x);
			}

			/// <summary>
			/// min tmp, tmp &gt;= x
			/// </summary>
			inline tree_iterator lower_bound(const T& x) const
			{
				return tree_iterator(this, lower_bound(root, x));
			}

			/// <summary>
			/// min tmp, tmp &gt; x
			/// </summary>
			inline tree_iterator upper_bound(const T& x) const
			{
				return tree_iterator(this, upper_bound(root, x));
			}

			inline vector<iterator> elements() const
			{
				vector<iterator> tmp;
				iter(root, tmp);
				return tmp;
			}

			inline vector <T> values() const
			{
				vector <iterator> tmp = elements();
				vector <T> res(tmp.size());
				transform(tmp.begin(), tmp.end(), res.begin(), value_from_iterator);
				return res;
			}

			inline void clear()
			{
				clear(root);
				root = nullptr;
			}

			/// <summary>
			/// Swap two trees in O(1)
			/// </summary>
			/// <param name="oth">Tree to swap</param>
			inline void swap(tree<T, null_type, Compare, rb_tree_tag, tree_order_statistics_node_update>& oth)
			{
				iterator tmp = root;
				root = oth.root;
				oth.root = tmp;
			}

			tree(const tree<T, null_type, Compare, rb_tree_tag, tree_order_statistics_node_update>& oth) : func(oth.func)
			{
				for (auto& i : oth)
				{
					insert(i);
				}
			}

			inline bool erase(tree_iterator it)
			{
				return erase(*it);
			}

			tree<T, null_type, Compare, rb_tree_tag, tree_order_statistics_node_update>& operator=(const tree<T, null_type, Compare, rb_tree_tag, tree_order_statistics_node_update>& oth)
			{
				func = oth.func;
				clear();
				for (auto& i : oth)
				{
					insert(i);
				}
				return *this;
			}
		};
	}

	template <typename T, typename C>
	void swap(__gnu_pbds::tree<T, __gnu_pbds::null_type, C, __gnu_pbds::rb_tree_tag, __gnu_pbds::tree_order_statistics_node_update>& p,
		__gnu_pbds::tree<T, __gnu_pbds::null_type, C, __gnu_pbds::rb_tree_tag, __gnu_pbds::tree_order_statistics_node_update>& q) noexcept
	{
		p.swap(q);
	}
}