#include <vector>

template <typename T>
class matrix {
	std::vector<std::vector<T>> data;
public:
	std::vector<T> &operator[](size_t row) { return data[row]; }
	matrix(size_t nr_row, size_t nr_col, const T &val = T()) : data(nr_row, vector<T>(nr_col, val)) {}
	size_t nr_row() const { return data.size(); }
	size_t nr_col() const { return data[0].size(); }
	matrix operator* (const matrix<T> &that) const {
		if (nr_col() != that.nr_row())
			throw 0;

		size_t nrow = nr_row(), ncol = that.nr_col(), nmid = nr_col();
		matrix<T> result(nrow, ncol);
		for (int i = 0; i < nrow; ++i) {
			for (int j = 0; j < ncol; ++j) {
				T x = T();
				for (int k = 0; k < nmid; ++k)
					x += (*this)[i][k] * that[k][j];
				result[i][j] = x;
			}
		}
	}
};
