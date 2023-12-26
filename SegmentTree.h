#pragma once
#include <vector>
#include <functional>

namespace std
{
#pragma message("Mark_Pr (c) SegmentTree.h")
#ifdef _MSC_VER
#define EXPR_ASSERT(x) _ASSERT(x)
#else
#include <cassert>
#define EXPR_ASSERT(x) assert(x)
#endif
	template <typename _Type, typename _Result = _Type>
	class SegmentTree
	{
	protected:
		size_t tsize;
		vector <_Result> tree;
		function<_Type(const _Type&, const _Type&)> func;

		_Result nullval;

	protected:
		void build(vector <_Type> &l, size_t x, size_t lx, size_t rx)
		{
			if (lx + 1 == rx) {
				if (lx < l.size())
					tree[x] = l[lx];
			}
			else {
				size_t m = (lx + rx) / 2;
				build(l, 2 * x + 1, lx, m);
				build(l, 2 * x + 2, m, rx);
				tree[x] = func(tree[2 * x + 1], tree[2 * x + 2]);
			}
		}

		template <typename _Iter>
		void build(_Iter& first, _Iter& last, size_t x, size_t lx, size_t rx)
		{
			if (lx + 1 == rx) {
				if (first != last)
				{
					tree[x] = *first;
					++first;
				}
			}
			else {
				size_t m = (lx + rx) / 2;
				build(first, last, 2 * x + 1, lx, m);
				build(first,last, 2 * x + 2, m, rx);
				tree[x] = func(tree[2 * x + 1], tree[2 * x + 2]);
			}
		}

		void set(size_t i, const _Type& val, size_t x, size_t lx, size_t rx)
		{
			if (lx == rx - 1) {
				tree[x] = val;
			}
			else {
				size_t m = (lx + rx) / 2;
				if (i < m)
					set(i, val, 2 * x + 1, lx, m);
				else
					set(i, val, 2 * x + 2, m, rx);
				tree[x] = func(tree[2 * x + 1], tree[2 * x + 2]);
			}
		}

		_Result segment(size_t l, size_t r, size_t x, size_t lx, size_t rx)
		{
			if (rx <= l || r <= lx)
				return nullval;
			if (l <= lx && rx <= r)
				return tree[x];
			size_t m = (lx + rx) / 2;
			return func(segment(l, r, 2 * x + 1, lx, m), segment(l, r, 2 * x + 2, m, rx));
		}
	public:
		SegmentTree(function<_Type(const _Type&, const _Type&)> function, const _Result& nullarg):nullval(nullarg)
		{
			this->func = function;
		}

		void init(size_t n)
		{
			tsize = 1;
			while (tsize < n) {
				tsize *= 2;
			}
			tree.assign(2 * tsize - 1, nullval);
		}

		void init(size_t n, const _Result& nullelem)
		{
			this->nullval = nullelem;
			this->init(n);
		}

		void set(size_t i, const _Type& t)
		{
			if (i >= tsize)
				return;
			set(i, t, 0, 0, tsize);
		}

		void build(vector <_Type>& a)
		{
			this->init(a.size());
			a.resize(tsize, nullval);
			this->build(a, 0, 0, tsize);
		}

		template <typename _Iter>
		void build(_Iter first, _Iter last)
		{
			EXPR_ASSERT(first < last);
			this->init(distance(first, last));
			this->build(first, last, 0, 0, tsize);
		}

		size_t assign(size_t n, const _Result& elem)
		{
			init(n);
			vector <_Result> tmp(tsize, elem);
			build(tmp);
			return tsize;
		}

		_Result segment(size_t i, size_t j)
		{
			j = min(j, tsize);
			if (i >= j)
				return nullval;
			return segment(i, j, 0, 0, tsize);
		}

		void build()
		{
			vector <_Type> empty;
			build(empty, 0, 0, tsize);
		}

		size_t size()
		{
			return tsize;
		}

		const _Result& get(size_t i)
		{
			if (i >= tsize)
				return nullval;
			return tree[tsize - 1 + i];
		}
	};
#ifdef EXT
#pragma message("Mark_Pr (c) SegmentTree.hpp")
#pragma message("#define EXT: This template is not tested")
#pragma message("#define EXT: Don't use \"using namespace ext\"")
	namespace ext
	{
		template <typename _Type, typename _Add, typename _Result = _Type>
		class SegmentTree
		{
			//#pragma warning("_Type must have operator+(_Type, _Add)")
		protected:
			long long tsize;
			vector <_Result> tree;
			vector <_Add> upd;
			function<_Result(const _Result&,const _Result&)> func;
			function<_Add(const _Add&, const _Add&)> uniq;
			function<void(_Result&, const _Add&, size_t)> bf;
			_Result nullval;
			_Add nulladd;

		protected:
			void push(size_t v, size_t l, size_t r)
			{
				if (upd[v] != nulladd)
				{
					bf(tree[v], upd[v], r - l);
					if (l != r - 1)
					{
						upd[2 * v + 1] = uniq(upd[2 * v + 1], upd[v]);
						upd[2 * v + 2] = uniq(upd[2 * v + 2], upd[v]);
					}
					upd[v] = nulladd;
				}
			}

			void build(vector <_Type>& l, size_t x, size_t lx, size_t rx)
			{
				if (lx + 1 == rx) {
					if (lx < l.size())
						tree[x] = l[lx];
				}
				else {
					size_t m = (lx + rx) / 2;
					build(l, 2 * x + 1, lx, m);
					build(l, 2 * x + 2, m, rx);
					tree[x] = func(tree[2 * x + 1], tree[2 * x + 2]);
				}
			}

			template <typename _Iter>
			void build(_Iter& first, _Iter& last, size_t x, size_t lx, size_t rx)
			{
				if (lx + 1 == rx) {
					if (first != last)
					{
						tree[x] = *first;
						++first;
					}
				}
				else {
					size_t m = (lx + rx) / 2;
					build(first, last, 2 * x + 1, lx, m);
					build(first, last, 2 * x + 2, m, rx);
					tree[x] = func(tree[2 * x + 1], tree[2 * x + 2]);
				}
			}

			void set(size_t i, const _Type& val, size_t x, size_t lx, size_t rx)
			{
				push(x, lx, rx);
				if (lx == rx - 1) {
					tree[x] = val;
				}
				else {
					size_t m = (lx + rx) / 2;
					if (i < m)
						set(i, val, 2 * x + 1, lx, m);
					else
						set(i, val, 2 * x + 2, m, rx);
					tree[x] = func(tree[2 * x + 1], tree[2 * x + 2]);
				}
			}

			_Result segment(size_t l, size_t r, size_t x, size_t lx, size_t rx)
			{
				push(x, lx, rx);
				if (rx <= l || r <= lx)
					return nullval;
				if (l <= lx && rx <= r)
					return tree[x];
				size_t m = (lx + rx) / 2;
				return func(segment(l, r, 2 * x + 1, lx, m), segment(l, r, 2 * x + 2, m, rx));
			}

			void update(size_t v, size_t l, size_t r, size_t ql, size_t qr, const _Add& adv)
			{
				push(v, l, r);
				if (ql >= r || qr <= l)
					return;
				if (ql <= l && r <= qr) {
					upd[v] = uniq(upd[v], adv);
					push(v, l, r);
					return;
				}
				size_t m = (l + r) / 2;
				update(2 * v + 1, l, m, ql, qr, adv);
				update(2 * v + 2, m, r, ql, qr, adv);
				tree[v] = func(tree[2 * v + 1], tree[2 * v + 2]);
			}

			void confirm(size_t v, size_t l, size_t r)
			{
				push(v, l, r);
				if (l == r - 1)
					return;
				size_t m = (l + r) / 2;
				confirm(2 * v + 1, l, m);
				confirm(2 * v + 2, m, r);
			}

		public:
			/// <summary>
			/// Initialize SegmentTree
			/// </summary>
			/// <param name="func">segment(l, r) = func(a_l, ..., a_r-1)</param>
			/// <param name="unique">using to add two updates</param>
			/// <param name="plus">update element function; update(l, r, x) -> plus(a_l,x),...,plus(a_r-1,x)</param>
			/// <param name="nullarg">func(x, nullarg) = x</param>
			/// <param name="nulladd">unique(x, nulladd) = x</param>
			SegmentTree(
				function<_Result(const _Result&, const _Result&)> func, 
				function<_Add(const _Add&, const _Add&)> unique, 
				function<void(_Result&, const _Add&, size_t)> plus, 
				const _Result& nullarg, const _Add& nulladd) :nullval(nullarg), nulladd(nulladd)
			{
				this->func = func;
				this->uniq = unique;
				this->bf = plus;
			}

			/// <summary>
			/// Initialize for n elements
			/// </summary>
			/// <param name="n"></param>
			inline void init(size_t n)
			{
				tsize = 1;
				while (tsize < n) {
					tsize *= 2;
				}
				tree.assign(2 * tsize - 1, nullval);
				upd.assign(2 * tsize - 1, nulladd);
			}

			inline void init(size_t n, const _Result&val,const _Add& add)
			{
				this->nullval = val;
				this->nulladd = add;
				tsize = 1;
				while (tsize < n) {
					tsize *= 2;
				}
				tree.assign(2 * tsize - 1, nullval);
				upd.assign(2 * tsize - 1, nulladd);
			}

			inline void set(size_t i, const _Type& t)
			{
				set(i, t, 0, 0, tsize);
			}

			inline void build(vector <_Type>& a)
			{
				this->init(a.size());
				a.resize(tsize, nullval);
				this->build(a, 0, 0, tsize);
			}

			/// <summary>
			/// Initialize for n elements and assign all values to elem
			/// </summary>
			/// <param name="n"></param>
			/// <param name="nullelem"></param>
			/// <returns>real tree size</returns>
			inline size_t assign(size_t n, const _Type& elem)
			{
				init(n);
				vector <_Type> l(n, elem);
				build(l);
				return tsize;
			}

			inline _Result segment(size_t i, size_t j)
			{
				return segment(i, j, 0, 0, tsize);
			}

			inline size_t size()
			{
				return tsize;
			}

			inline void update(size_t ql, size_t qr, const _Add& adv)
			{
				update(0, 0, tsize, ql, qr, adv);
			}

			inline void confirm()
			{
				confirm(0, 0, tsize);
			}

			inline _Result get_confirmed(size_t i)
			{
#pragma message("get_confirmed(i): Call get_confirmed only after confirm() operation")
				return tree[i + tsize - 1];
			}

			template <typename _Iter>
			inline void build(_Iter first, _Iter last)
			{
				this->init(distance(first, last));
				this->build(first, last, 0, 0, tsize);
			}
		};
	}
#endif
#undef EXPR_ASSERT
}
