#pragma once
#include <functional>
#include <utility>
#include <stdlib.h>
#include <random>

namespace std
{
	template <typename T>
	class Rope
	{
	public:
		struct Node
		{
			size_t  key;
			size_t  cnt;
			T       sum;
			T       add;
			T       val;
			Node* l, * r;
			bool updFlag;
			operator const T& () { return val; }
		};

		struct Exception : std::exception
		{
		};

		using iterator = Node*;
		using value_type = T;

	private:
		std::mt19937 random;
		iterator root;

	protected:
		T __val;

	public:
		Rope(const T& def) : __val(def)
		{
#ifdef _DEBUG
			random.seed(100);
#else
			random.seed(7923);
#endif
			root = nullptr;
		}

	protected:
		virtual void add(iterator t, const T& val) {}
		virtual void push(iterator t)
		{
			if (t == nullptr)
				return;
			if (t->updFlag)
			{
				add(t->l, t->add);
				add(t->r, t->add);
				t->updFlag = false;
			}
		}
		virtual T func(const T& t, const T&) { return t; }

		virtual iterator create(const T& val)
		{
			iterator n = new Node();
			n->key = random();
			n->cnt = 1;
			n->sum = val;
			n->val = val;
			n->l = nullptr;
			n->r = nullptr;
			n->updFlag = 0;
			return n;
		}

	public:
		static inline size_t size(iterator it)
		{
			if (it == nullptr)
				return 0;
			return it->cnt;
		}

		inline virtual const T& seg(iterator it)
		{
			if (it == nullptr)
				return __val;
			return it->sum;
		}

	protected:
		void update(iterator it)
		{
			if (it == nullptr)
				return;
			//update(it->l);
			//update(it->r);
			it->sum = func(seg(it->l), func(it->val, seg(it->r)));
			it->cnt = size(it->l) + 1 + size(it->r);
		}

	protected:
		std::pair <iterator, iterator> split(iterator t, size_t k)
		{
			push(t);
			if (t == nullptr)
				return { nullptr,nullptr };
			if (size(t->l) >= k)
			{
				auto p = split(t->l, k);
				t->l = p.second;
				update(t);
				return { p.first, t };
			}
			auto p = split(t->r, k - size(t->l) - 1);
			t->r = p.first;
			update(t);
			return { t,p.second };
		}

		iterator merge(iterator a, iterator b)
		{
			push(a);
			push(b);
			if (a == nullptr)
				return b;
			if (b == nullptr)
				return a;
			if (a->key < b->key)
			{
				a->r = merge(a->r, b);
				update(a);
				return a;
			}
			b->l = merge(a, b->l);
			update(b);
			return b;
		}

	protected:
		iterator insert(iterator t, const T& elem, size_t k)
		{
			auto p = split(t, k);
			p.second = merge(create(elem), p.second);
			return merge(p.first, p.second);
		}

		iterator erase(iterator t, size_t k)
		{
			auto p = split(t, k);
			auto q = split(p.second, 1);
			delete q.first;
			return merge(p.first, q.second);
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

	public:
		inline bool insert(const T& elem, size_t k = 0)
		{
			if (k > size(root))
				return false;
			root = insert(root, elem, k);
			return true;
		}

		bool paste(iterator val, size_t k = 0)
		{
			push(root);
			if (val == nullptr)
				return false;
			if (k > size(root))
				return false;
			auto p = split(root, k);
			p.second = merge(val, p.second);
			root = merge(p.first, p.second);
			return true;
		}

		inline iterator at(size_t i)
		{
			push(root);
			update(root);
			return kth_element(root, i);
		}

		iterator cut(size_t ql, size_t qr)
		{
			push(root);
			if (qr > size(root))
				return nullptr;
			auto p = split(root, qr);
			auto q = split(p.first, ql);
			root = merge(q.first, p.second);
			return q.second;
		}

		inline const T& operator[](size_t ind)
		{
			iterator it = at(ind);
			if (it == nullptr)
				throw Exception();
			return *it;
		}

		T segment(size_t l, size_t r)
		{
			iterator it = cut(l, r);
			T tmp = seg(it);
			paste(it, l);
			return tmp;
		}

		inline size_t size()
		{
			return size(root);
		}

		inline iterator data()
		{
			return root;
		}

		inline void push_back(const T& t)
		{
			insert(t, size(root));
		}

		inline void push_front(const T& t)
		{
			insert(t, 0);
		}
	};
#ifdef EXT // EXT library inline
#pragma message("Mark_Pr (c) SegmentTree.hpp")
#pragma message("#define EXT: This template is not tested")
#pragma message("#define EXT: Don't use \"using namespace ext\"")
#pragma message("#define EXT: Inline EXT lib doesn't supported")
	namespace ext
	{
		template <
			typename T,
			typename Tadd
		>
			struct Rope
		{
			struct Node
			{
				size_t  key;
				size_t  cnt;
				T       sum;
				Tadd    add;
				T       val;
				Node* l, * r;
				bool updFlag;
				bool revFlag;
				operator const T& () { return val; }
			};

			using iterator = Node*;
			using value_type = T;

		protected:
			std::mt19937 random;
			iterator root;
			T __value;
			Tadd __add;
			
		protected:
			//void add(iterator, const Tadd&);
			//T func(const T&, const T&);

			iterator create(const T& val)
			{
				iterator n = new Node();
				n->key = random();
				n->cnt = 1;
				n->sum = val;
				n->val = val;
				n->l = nullptr;
				n->r = nullptr;
				n->updFlag = 0;
				n->revFlag = 0;
				return n;
			}

			function<T(const T&, const T&)> fsum;
			function<void (iterator, const Tadd&)> fadd;

			void push(iterator t)
			{
				if (t == nullptr)
					return;
				if (t->updFlag)
				{
					fadd(t->l, t->add);
					fadd(t->r, t->add);
					t->updFlag = false;
				}
				if (t->revFlag)
				{
					if (t->l != nullptr)
						t->l->revFlag ^= true;
					if (t->r != nullptr)
						t->r->revFlag ^= true;
					swap(t->l, t->r);
					t->revFlag = false;
				}
			}

		public:
			Rope(const T& def, const Tadd& add, decltype(fsum) f1, decltype(fadd)f2 = nullptr) : __value(def), __add(add), fsum(f1), fadd(f2)
			{
#ifdef _DEBUG
				random.seed(100);
#else
				random.seed(time(NULL));
#endif
				root = nullptr;
			}

			static inline size_t size(iterator it)
			{
				if (it == nullptr)
					return 0;
				return it->cnt;
			}

			inline const T& seg(iterator it)
			{
				if (it == nullptr)
					return __value;
				return it->sum;
			}

			inline void rev(iterator it)
			{
				it->revFlag ^= true;
			}

		protected:
			void update(iterator it)
			{
				if (it == nullptr)
					return;
				//update(it->l);
				//update(it->r);
				it->sum = fsum(seg(it->l), fsum(it->val, seg(it->r)));
				it->cnt = size(it->l) + 1 + size(it->r);
			}

		protected:
			pair <iterator, iterator> split(iterator t, size_t k)
			{
				push(t);
				if (t == nullptr)
					return { nullptr,nullptr };
				if (size(t->l) >= k)
				{
					auto p = split(t->l, k);
					t->l = p.second;
					update(t);
					return { p.first, t };
				}
				auto p = split(t->r, k - size(t->l) - 1);
				t->r = p.first;
				update(t);
				return { t,p.second };
			}

			iterator merge(iterator a, iterator b)
			{
				push(a);
				push(b);
				if (a == nullptr)
					return b;
				if (b == nullptr)
					return a;
				if (a->key < b->key)
				{
					a->r = merge(a->r, b);
					update(a);
					return a;
				}
				b->l = merge(a, b->l);
				update(b);
				return b;
			}

		protected:
			iterator insert(iterator t, const T& elem, size_t k)
			{
				auto p = split(t, k);
				p.second = merge(create(elem), p.second);
				return merge(p.first, p.second);
			}

			iterator erase(iterator t, size_t k)
			{
				auto p = split(t, k);
				auto q = split(p.second, 1);
				delete q.first;
				return merge(p.first, q.second);
			}

			iterator kth_element(iterator root, size_t cnt)
			{
				push(root);
				if (size(root) <= cnt)
					return nullptr;
				if (size(root->l) == cnt)
					return root;
				if (size(root->l) < cnt)
					return kth_element(root->r, cnt - size(root->l) - 1);
				else
					return kth_element(root->l, cnt);
			}

		public:
			inline bool insert(const T& elem, size_t k = 0)
			{
				if (k > size(root))
					return false;
				root = insert(root, elem, k);
				return true;
			}

			bool paste(iterator val, size_t k = 0)
			{
				push(root);
				if (val == nullptr)
					return false;
				if (k > size(root))
					return false;
				auto p = split(root, k);
				p.second = merge(val, p.second);
				root = merge(p.first, p.second);
				return true;
			}

			inline iterator at(size_t i)
			{
				push(root);
				update(root);
				return kth_element(root, i);
			}

			iterator cut(size_t ql, size_t qr)
			{
				push(root);
				if (qr > size(root))
					return nullptr;
				auto p = split(root, qr);
				auto q = split(p.first, ql);
				root = merge(q.first, p.second);
				return q.second;
			}

			inline const T& operator[](size_t ind)
			{
				iterator it = at(ind);
				if (it == nullptr)
					throw;
				return *it;
			}

			T segment(size_t l, size_t r)
			{
				iterator it = cut(l, r);
				T tmp = seg(it);
				paste(it, l);
				return tmp;
			}

			void reverse(size_t l, size_t r)
			{
				iterator it = cut(l, r);
				rev(it);
				paste(it, l);
			}

			inline size_t size()
			{
				return size(root);
			}

			inline iterator data()
			{
				return root;
			}

			inline void push_back(const T& t)
			{
				insert(t, size(root));
			}

			inline void push_front(const T& t)
			{
				insert(t, 0);
			}

			inline T front()
			{
				return this->operator[](0);
			}

			inline T back()
			{
				return this->operator[](size(root) - 1);
			}

			inline void pop_back()
			{
				delete cut(size() - 1, size());
			}

			inline void pop_front()
			{
				delete cut(0, 1);
			}

			inline void erase(size_t t)
			{
				delete cut(t, t + 1);
			}
		};
	}
#endif
}