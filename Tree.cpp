#include <iostream>
#include <iomanip>
#include <vector>
#include "Rational.h"

using ll = Rational;

class SegmentTree {
private:
	int size;
	ll neutral;
	vector<ll> tree;

	void clear(int v) {
		tree[v] = neutral;
	}

	void merge(int target, int source) {
		tree[target] = tree[target] + tree[source];
	}

	void update_vertex(int v, int v_left, int v_right) {
		clear(v);
		merge(v, v_left);
		merge(v, v_right);
	}

	vector<Rational> a;
	void build_tree(int v, int left, int right) {
		// [left; right)
		if (left + 1 == right) {
			tree[v] = a[left];
		}
		else {
			int mid = (left + right) / 2;
			int v_left = (v << 1), v_right = v_left + 1;
			build_tree(v_left, left, mid);
			build_tree(v_right, mid, right);
			update_vertex(v, v_left, v_right);
		}
	}

	int index;
	Rational value;

	void set_tree_pos(int v, int left, int right) {
		if (left + 1 == right) {
			tree[v] = value;
		}
		else {
			int mid = (left + right) / 2;
			int v_left = (v << 1), v_right = v_left + 1;
			if (index < mid) set_tree_pos(v_left, left, mid);
			else set_tree_pos(v_right, mid, right);
			update_vertex(v, v_left, v_right);
		}
	}

	int v_res = 0;
	int start, end;

	void get_tree_seg(int v, int left, int right) {
		if (start <= left && right <= end) {
			merge(v_res, v);
		}
		else {
			int mid = (left + right) / 2;
			int v_left = (v << 1), v_right = v_left + 1;
			if (start < mid) get_tree_seg(v_left, left, mid);
			if (mid < end) get_tree_seg(v_right, mid, right);
		}
	}

public:
	SegmentTree(int n, ll _neutral)
		: size(n)
		, neutral(_neutral)
		, tree(size << 2, neutral)
	{ }

	void build(vector<Rational>& _a) {
		a = _a;
		build_tree(1, 0, size);
	}

	void set(int _index, Rational _value) {
		index = _index;
		value = _value;
		set_tree_pos(1, 0, size);
	}

	ll get_sum(int _start, int _end) { // [_start, _end]
		start = _start;
		end = _end + 1;

		clear(v_res);
		get_tree_seg(1, 0, size);
		return tree[v_res];
	}
};

SegmentTree create_tree(vector<Rational>& a) {
	SegmentTree tree(a.size(), ll(0,1));
	tree.build(a);
	return tree;
}

int main() {
	int n, q;
	std::cout << "Enter number of elements: " << std::endl;
	std::cin >> n;
	std::cout << "Enter number of operation: " << std::endl;
	std::cin>>q;

	std::vector<Rational> a;
	int num;
	int det;
	std::cout << "Enter value of elements:" << std::endl;
	for (int i = 0; i < n; i++) {
		cin >> num>>det;
		a.push_back(Rational(num, det));
	}



	SegmentTree tree = create_tree(a);

	const int SET = 1, SUM = 2;

	while (q-- > 0) {
		int type;
		std::cout << "Choose operetion:" << std::endl << "1 - change value;" << std::endl << "2 - get sum on interval[left; right);" << std::endl;
		std::cin >> type;

		if (SET == type) {
			int index,num, det;
			std::cin >> index>>num >> det;
			Rational value(num,det);

			tree.set(index, value);
		}
		else {
			int start, end;
			std::cin >> start >> end;

			std::cout << tree.get_sum(start, end - 1) << std::endl;
		}
	}

	return 0;
}

/*Enter number of elements:
3
Enter number of operation:
3
1
2
2
3
8
3
Choose operetion:
1 - change value;
2 - get sum on interval[left; right);
2
0
2
7/6
Choose operetion:
1 - change value;
2 - get sum on interval[left; right);
1
0
5
3
Choose operetion:
1 - change value;
2 - get sum on interval[left; right);
2
0
3
5/1
*/
