#pragma once
#include <algorithm>
#include <stdlib.h>
#include <utility>
#include <tuple>

namespace china
{
	template<typename T>
	using carray = std::pair<size_t, T*>;

	template <typename T>
	struct MoAlgorithmRequest
	{
		using answer_type = T;

		size_t l = 0, r = 0;

		virtual size_t left()
		{
			return l;
		}

		virtual size_t right()
		{
			return r;
		}

		virtual void dleft()
		{
		}

		virtual void aleft()
		{
		}

		virtual void dright()
		{
		}

		virtual void aright()
		{
		}

		virtual answer_type answer();

		answer_type segment(size_t lq, size_t rq)
		{
			while (r < rq)
				aright(), r++;
			while (l > lq)
				aleft(), l--;
			while (l < lq)
				dleft(), l++;
			while (r > rq)
				dright(), r--;
			return answer();
		}
	};

	struct IMoRequest
	{
	protected:
		virtual void aleft();
		virtual void dleft();
		virtual void aright();
		virtual void dright();

		int l, r;

	public:
		void prepare(int lq, int rq)
		{
			while (l < lq)
				dleft();
			while (l > lq)
				aleft();
			while (r > rq)
				dright();
			while (r < rq)
				aright();
		}

		virtual int answer();
		virtual int error() { return 0; }

		IMoRequest* segment(int lq, int rq, int* res)
		{
			if (this == nullptr)
				return NULL;
			prepare(lq, rq);
			if (error())
				return NULL;
			*res = answer();
			if (error())
				return NULL;
			return this;
		}
	};

	IMoRequest* MoRequestAlgorithm(carray <int[3]> q, carray<int> a, int C, IMoRequest* r)
	{
		if (r == nullptr)
			return r;
		std::sort(q.second, q.second + q.first, [C](int a[3], int b[3])
			{
				if (a[0] / C != b[0] / C)
					return a[0] < b[0];
				if ((a[0] / C) % 2 == 0)
					return a[1] < b[1];
				return a[1] > b[1];
			});
		int(*_Begin)[3] = q.second, (*_End)[3] = q.second + q.first;
		int* out = a.second;
		if (_End - _Begin > a.first)
			return NULL;
		while (_Begin != _End)
			r = r->segment((*_Begin)[0], (*_Begin)[1], out), out++, _Begin++;
	}

	template <typename T>
	struct MoAlgorithmListRequest: public MoAlgorithmRequest<T>
	{
		virtual void insert(const T& x);
		virtual void erase(const T& x);

		carray<const T> lst;
		carray<const T>::second_type _Begin, _End;

		MoAlgorithmListRequest(carray<const T> arr) :lst(arr), _Begin(arr.second), _End(arr.second) {}
		MoAlgorithmListRequest(const vector<T>& l) :MoAlgorithmListRequest((carray<T>) { l.size(), l.data() }) {}

		virtual void dleft()
		{
		}

		virtual void aleft()
		{
		}

		virtual void dright()
		{
		}

		virtual void aright()
		{
		}
	};

	template <typename Request = MoAlgorithmRequest<int>>
	void MoAlgorithm(
		carray<std::tuple<size_t, size_t,size_t>> q,
		carray<typename Request::answer_type> a,
		size_t gsize,
		Request& r)
	{
		auto* st = q.second, * lt = q.second + q.first;
		auto* out = a.second;
		std::sort(q.second, q.second + q.first, [gsize](const std::tuple<size_t, size_t, size_t>& q1,
			const std::tuple<size_t, size_t, size_t>& q2)
			{
				if (std::get<0>(q1) / gsize != std::get<0>(q2) / gsize)
					return std::get<0>(q1) < std::get<0>(q2);
				if ((std::get<0>(q1) / gsize) % 2 == 0)
					return std::get<1>(q1) < std::get<1>(q2);
				return std::get<1>(q1) > std::get<1>(q2);
			});
		while (st != lt)
		{
			out[std::get<2>(*st)] = r.segment(std::get<0>(*st), std::get<1>(*st));
			st++;
		}
	}
}

