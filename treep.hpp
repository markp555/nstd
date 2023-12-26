#pragma once
#include <stdlib.h>
#include <random>
#include <functional>

namespace std
{
	/// <summary>
	/// Treep class interface
	/// </summary>
	/// <typeparam name="T">Type of element</typeparam>
	template <typename T>
	class Treep
	{
	public:
		struct Node
		{
			T       x;
			size_t  y;
			size_t  c;
			T		s;
			Node* l;
			Node* r;
			inline operator const T& () { return x; }
		};
		using iterator = Node*;
		using value_type = T;

	protected:
		std::mt19937 random;
		iterator root;

		function<bool(const T&, const T&)> func;

	public:
		Treep(function<bool(const T&, const T&)> f) : func(f)
		{
			root = nullptr;
			random.seed(123);
		}

	protected:
		iterator create(const T& x)
		{
			iterator n = new Node;
			n->c = 1;
			n->s = x;
			n->x = x;
			n->y = random();
			n->l = nullptr;
			n->r = nullptr;
			return n;
		}

		static inline size_t size(iterator n)
		{
			if (n == nullptr)
				return 0;
			// update(n);
			return n->c;
		}

		virtual inline void update(iterator r)
		{
			if (r == nullptr)
				return;
			r->c = size(r->l) + size(r->r) + 1;
		}

		iterator find(iterator root, T x)
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
				return { root,p.second };
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

		iterator lower_bound(iterator root, const T& x)
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

		iterator upper_unbound(iterator root, const T& x)
		{
			if (root == nullptr)
				return nullptr;
			if (!func(root->x, x))
				return upper_unbound(root->l, x);
			iterator t = upper_unbound(root->r, x);
			if (t == nullptr)
				return root;
			return t;
		}

		iterator upper_bound(iterator root, const T& x)
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

		iterator kth_element(iterator root, size_t cnt)
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

		size_t order(iterator root, const T& x)
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

	public:
		/// <summary>
		/// Find element
		/// </summary>
		/// <param name="x">element to find</param>
		/// <returns>nullptr if element not in list, else iterator of element</returns>
		inline iterator find(const T& x)
		{
			return find(root, x);
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
		inline iterator kth_element(size_t k)
		{
			return kth_element(root, k);
		}

		/// <summary>
		/// Return order of element x
		/// </summary>
		/// <param name="x">Element</param>
		/// <returns>Order of element</returns>
		inline size_t order_of_element(const T& x)
		{
			return order(root, x);
		}

		/// <summary>
		/// min tmp, tmp &gt;= x
		/// </summary>
		inline iterator lower_bound(const T& x)
		{
			return lower_bound(root, x);
		}

		/// <summary>
		/// max tmp, tmp &lt; x
		/// </summary>
		inline iterator upper_unbound(const T& x)
		{
			return upper_unbound(root, x);
		}

		/// <summary>
		/// min tmp, tmp &lt;= x
		/// </summary>
		inline iterator lower_unbound(const T& x)
		{

		}

		/// <summary>
		/// min tmp, tmp &gt; x
		/// </summary>
		inline iterator upper_bound(const T& x)
		{
			return upper_bound(root, x);
		}

		/// <summary>
		/// Return size
		/// </summary>
		inline size_t size()
		{
			return size(root);
		}

		/// <summary>
		/// Return true if tree is empty
		/// </summary>
		inline bool empty()
		{
			return size() == 0;
		}

		/// <summary>
		/// Return min element in tree
		/// </summary>
		inline iterator min_element()
		{
			return min(root);
		}

		/// <summary>
		/// Return max element in tree
		/// </summary>
		inline iterator max_element()
		{
			return max(root);
		}
	};

	template<typename T>
	class TreepSum : public Treep<T>
	{
	protected:
		function<T(const T&, const T&)> sumf;

	public:
		using iterator = Treep<T>::Node*;
		using value_type = T;

		value_type __value;

		TreepSum(function<bool(const T&, const T&)> f, function<T(const T&, const T&)> fs, const T& def) : Treep<T>(f),sumf(fs),__value(def)
		{
		}

	protected:
		inline T sum(iterator n)
		{
			if (n == nullptr)
				return __value;
			return n->s;
		}

		virtual inline void update(iterator r)
		{
			if (r == nullptr)
				return;
			r->c = size(r->l) + size(r->r) + 1;
			if (!sumf)
				return;
			r->s = sumf(sumf(sum(r->l), sum(r->r)), r->x);
		}

		T sum(iterator root, const T& l, const T& r)
		{
			if (root == nullptr)
				return __value;
			const T& lx = *min(root);
			const T& rx = *max(root);
			/*if (func(lx, l) || func(r, rx))
				return sum(root);*/
			if (func(r, lx) || func(rx, l))
				return __value;
			if ((!func(lx, l)) && (!func(r, rx)))
				return sum(root);
			if (func(root->x, l))
				return sum(root->r, l, r);
			if (func(r, root->x))
				return sum(root->l, l, r);
			if (!sumf)
				return __value;
			return sumf(sumf(sum(root->l, l, r), sum(root->r, l, r)), root->x);
		}

	public:
		/// <summary>
		/// Return value of sumf on elements from l to r
		/// </summary>
		/// <param name="l">left border</param>
		/// <param name="r">right border</param>
		inline T segment(const T& l, const T& r)
		{
			return sum(this->root, l, r);
		}
	};

	template <typename _Res, class..._Args>
	class Operation
	{
		function<_Res(_Args)> f;
		_Res nullres;
		Operation(function<_Res(_Args)> f, const _Res& r) :f(f), nullres(r) {}
		_Res operator()(_Args... argl)
		{
			if (f)
				return f(argl);
			return nullres;
		}
		_Res operator()()
		{
			return nullres;
		}
	};

#ifdef EXT
	namespace ext
	{
#pragma message("#define EXT: don't use \"using namespace std\"")
		template <typename Data>
		class Treep
		{
			// !Inherit your classes from it;
#pragma message("TreepEx: Mark_Pr (c) 2023-3000")
		public:
			struct Node
			{
				Data    x;
				size_t  y;
				size_t  c;
				Node* l;
				Node* r;
				inline operator const T& () { return x; }
			};
			using iterator = Node*;
			using value_type = T;
		protected:
			std::mt19937 random;
			iterator root;

			Operation <bool, const Data&, const Data&> less;
			Operation <bool, Node*, decltype(this)> updatef;

		public:
			Treep(const Operation<bool, const Data&, const Data&>& op1,
				const Operation <void, Node*, decltype(this)>& op2) : less(op1),update(op2)
			{
				root = nullptr;
				random.seed(123);
			}

		protected:
			iterator create(const T& x)
			{
				iterator n = new Node;
				n->c = 1;
				n->s = x;
				n->x = x;
				n->y = random();
				n->l = nullptr;
				n->r = nullptr;
				update(n, this);
				return n;
			}

			static inline size_t size(iterator n)
			{
				if (n == nullptr)
					return 0;
				// update(n);
				return n->c;
			}

		public:
			virtual inline void update(iterator r)
			{
				if (r == nullptr)
					return;
				r->c = size(r->l) + size(r->r) + 1;
				updatef(r);
			}

		protected:
			iterator find(iterator root, Data x)
			{
				update(root);
				if (root == nullptr)
					return nullptr;
				if (!(less(root->x, x) || less(x, root->x)))
					return root;
				if (less(x, root->x))
					return find(root->l, x);
				return find(root->r, x);
			}

			std::pair <iterator, iterator>split(iterator root, const Data& a)
			{
				update(root);
				if (root == nullptr)
					return { nullptr,nullptr };
				if (less(root->x, a))
				{
					auto p = split(root->r, a);
					root->r = p.first;
					update(root);
					return { root,p.second };
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
				update(a);
				update(b);
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

			iterator insert(iterator root, const Data& x)
			{
				update(root);
				iterator n = create(x);
				auto p = split(root, x);
				return merge(p.first, merge(n, p.second));
			}

			iterator remove(iterator r, const Data& x)
			{
				update(r);
				if (r == nullptr)
					return nullptr;
				if (!(less(r->x, x) || less(x, r->x)))
				{
					iterator t = merge(r->l, r->r);
					delete r;
					return t;
				}
				if (less(x, r->x))
				{
					r->l = remove(r->l, x);
					update(r);
					return r;
				}
				r->r = remove(r->r, x);
				update(r);
				return r;
			}

			iterator lower_bound(iterator root, const Data& x)
			{
				update(root);
				if (root == nullptr)
					return nullptr;
				if (less(root->x, x))
					return lower_bound(root->r, x);
				iterator t = lower_bound(root->l, x);
				if (t != nullptr)
					return t;
				return root;
			}

			iterator upper_unbound(iterator root, const Data& x)
			{
				update(root);
				if (root == nullptr)
					return nullptr;
				if (!less(root->x, x))
					return upper_unbound(root->l, x);
				iterator t = upper_unbound(root->r, x);
				if (t == nullptr)
					return root;
				return t;
			}

			iterator upper_bound(iterator root, const Data& x)
			{
				update(root);
				if (root == nullptr)
					return root;
				if (!less(x, root->x))
					return upper_bound(root->r, x);
				iterator it = upper_bound(root->l, x);
				if (it == nullptr)
					return root;
				return it;
			}

			iterator kth_element(iterator root, size_t cnt)
			{
				update(root);
				if (size(root) <= cnt)
					return nullptr;
				if (size(root->l) == cnt)
					return root;
				if (size(root->l) < cnt)
					return kth_element(root->r, cnt - size(root->l) - 1);
				else
					return kth_element(root->l, cnt);
			}

			size_t order(iterator root, const Data& x)
			{
				update(root);
				if ((!less(root->x, x)) && (!less(x, root->x)))
					return size(root->l);
				if (less(root->x, x))
					return size(root->l) + 1 + order(root->r, x);
				return order(root->l, x);
			}

			static iterator min(iterator root)
			{
				update(root);
				if (root == nullptr)
					return root;
				iterator t = min(root->l);
				if (t == nullptr)
					return root;
				return Data;
			}

			static iterator max(iterator root)
			{
				update(root);
				if (root == nullptr)
					return root;
				iterator t = max(root->r);
				if (t == nullptr)
					return root;
				return Data;
			}

		public:
			/// <summary>
			/// Find element
			/// </summary>
			/// <param name="x">element to find</param>
			/// <returns>nullptr if element not in list, else iterator of element</returns>
			inline iterator find(const Data& x)
			{
				return find(root, x);
			}

			/// <summary>
			/// Insert element
			/// </summary>
			/// <param name="x">element to insert</param>
			/// <returns>true if element sucessfull inserted</returns>
			bool insert(const Data& x)
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
			bool erase(const Data& x)
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
			inline iterator kth_element(size_t k)
			{
				return kth_element(root, k);
			}

			/// <summary>
			/// Return order of element x
			/// </summary>
			/// <param name="x">Element</param>
			/// <returns>Order of element</returns>
			inline size_t order_of_element(const Data& x)
			{
				return order(root, x);
			}

			/// <summary>
			/// min tmp, tmp &gt;= x
			/// </summary>
			inline iterator lower_bound(const Data& x)
			{
				return lower_bound(root, x);
			}

			/// <summary>
			/// max tmp, tmp &lt; x
			/// </summary>
			inline iterator upper_unbound(const Data& x)
			{
				return upper_unbound(root, x);
			}

			/// <summary>
			/// min tmp, tmp &gt; x
			/// </summary>
			inline iterator upper_bound(const Data& x)
			{
				return upper_bound(root, x);
			}

			/// <summary>
			/// Return size
			/// </summary>
			inline size_t size()
			{
				return size(root);
			}

			/// <summary>
			/// Return true if tree is empty
			/// </summary>
			inline bool empty()
			{
				return size() == 0;
			}

			/// <summary>
			/// Return min element in tree
			/// </summary>
			inline iterator min_element()
			{
				return min(root);
			}

			/// <summary>
			/// Return max element in tree
			/// </summary>
			inline iterator max_element()
			{
				return max(root);
			}
		};
	}
#endif
}