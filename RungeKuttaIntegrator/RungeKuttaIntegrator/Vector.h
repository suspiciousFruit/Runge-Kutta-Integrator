#pragma once
#include <iostream>



class CalculateException { };
template <typename T> class Matrix;
template <typename T> class Vector;



template <typename T>
class Vector
{
private:
	T* array_;
	size_t size_;
public:
	Vector(); // Конструктор без аргументов
	Vector(size_t); // Конструктор с заранее заданным размером
	Vector(const std::initializer_list<T>&); // Конструктор для списка инициализации
	Vector(const Vector&); // Конструктор копирования
	Vector(Vector&&); // Коструктор перемещения
	~Vector(); // Деструктор

	Vector& operator= (const Vector&); // Оператор присваивания с копированием
	Vector& operator= (Vector&&); // Оператор присваивания с перемещением

	static Vector<T> linspace(T start, T stop, size_t number);

	void resize(size_t); // Функция изменения размера
	inline size_t size() const;

	inline T& operator[] (size_t); // Оператор []
	inline const T& operator[] (size_t) const;

	Vector& apply(T(*)(T)); // Применить ко всем
	Vector map(T(*)(T)) const; // Применить к копии
	// Срезы как в пайтоне
	Vector operator() () const;
	Vector operator() (int start) const;
	Vector operator() (int start, int stop) const;
	Vector operator() (int start, int stop, int step) const;
	// Арифметические операции
	template <typename D>
	friend Vector<D> operator+ (const Vector<D>&, const Vector<D>&);
	template <typename D>
	friend Vector<D> operator- (const Vector<D>&, const Vector<D>&);
	template <typename D>
	friend Vector<D> operator* (const Vector<D>&, D);
	template <typename D>
	friend Vector<D> operator* (D, const Vector<D>&);
	template <typename D>
	friend Vector<D> operator/ (const Vector<D>&, D);

	Vector<T>& operator*= (T);
	Vector<T>& operator/= (T);
	Vector<T>& operator+= (const Vector<T>&);
	Vector<T>& operator-= (const Vector<T>&);
	// Скалярное проиведение
	template <typename D>
	friend D operator, (const Vector<D>&, const Vector<D>&);
	// Ввод и вывод
	template <typename D>
	friend std::ostream& operator<< (std::ostream&, const Vector<D>&);
	/////////////////////////////////////////////
	template <typename D>
	friend Vector<D> operator* (const Matrix<D>&, const Vector<D>&);

	explicit operator Matrix<T>() const
	{
		Matrix<T> res;
		size_t resRows;

		resRows = size_;
		res.resize(resRows, 1);
		for (register size_t i = 0; i < resRows; ++i)
			res[i][0] = array_[i];

		return res;
	}
};

template <typename T>
Vector<T>::Vector() : array_(nullptr), size_(0)
{ }

template <typename T>
Vector<T>::Vector(size_t newSize) : size_(newSize)
{
	if (newSize)
		array_ = new T[newSize];
	else
		array_ = nullptr;
}

template <typename T>
Vector<T>::Vector(const std::initializer_list<T>& arg) : size_(arg.size())
{
	size_t i = 0;

	if (size_) array_ = new T[size_];
	else array_ = nullptr;

	for (auto& element : arg)
	{
		array_[i] = element;
		++i;
	}
}

template <typename T>
Vector<T>::Vector(const Vector<T>& arg) : size_(arg.size_)
{
	if (size_)
	{
		array_ = new T[size_];
		register T* arg_array = arg.array_;
		for (register size_t i = 0; i < size_; ++i)
			array_[i] = arg_array[i];
	}
	else
	{
		array_ = nullptr;
	}
}

template <typename T>
Vector<T>::Vector(Vector<T>&& arg) : size_(arg.size_), array_(arg.array_)
{
	arg.size_ = 0;
	arg.array_ = nullptr;
}

template <typename T>
Vector<T>::~Vector()
{
	if (array_)
	{
		delete[] array_;
		array_ = nullptr;
		size_ = 0;
	}
}

template <typename T>
Vector<T>& Vector<T>::operator= (const Vector<T>& arg)
{
	if (this != &arg)
	{
		if (size_ != arg.size_)
			resize(arg.size_);

		register T* arg_array = arg.array_;
		for (register size_t i = 0; i < size_; ++i)
			array_[i] = arg_array[i];
	}

	return *this;
}

template <typename T>
Vector<T>& Vector<T>::operator= (Vector<T>&& arg)
{
	if (this != &arg)
	{
		std::swap(array_, arg.array_);
		std::swap(size_, arg.size_);
	}

	return *this;
}

template <typename T>
Vector<T> Vector<T>::linspace(T start, T stop, size_t number)
{
	Vector<T> res;
	T stepSize;

	if (start < stop && number > 1)
	{
		res.resize(number);
		stepSize = (stop - start) / static_cast<T>(number - 1);
		for (size_t i = 0; i < number; ++i)
		{
			res[i] = start;
			start += stepSize;
		}
	}

	return res;
}

template <typename T>
void Vector<T>::resize(size_t newSize)
{
	if (newSize)
	{
		if (newSize != size_)
		{
			T* newArray = new T[newSize];
			size_t copySize;
			if (array_)
			{
				copySize = (size_ < newSize ? size_ : newSize);
				for (register size_t i = 0; i < copySize; ++i)
					newArray[i] = array_[i];

				delete[] array_;
			}
			array_ = newArray;
			size_ = newSize;
		}
	}
	else
	{
		if (array_)
			delete[] array_;
		array_ = nullptr;
		size_ = 0;
	}
}

template <typename T>
inline T& Vector<T>::operator[] (size_t index)
{
	return array_[index];
}

template <typename T>
inline const T& Vector<T>::operator[] (size_t index) const
{
	return array_[index];
}
/////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////
template <typename T>
Vector<T>& Vector<T>::apply(T(*f)(T))
{
	for (register size_t i = 0; i < size_; ++i)
		array_[i] = f(array_[i]);

	return *this;
}

template <typename T>
Vector<T> Vector<T>::map(T(*f)(T)) const // Can be changed for simple type
{
	Vector<T> res;
	res.resize(size_);

	register T* res_array = res.array_;
	for (register size_t i = 0; i < size_; ++i)
		res_array[i] = f(array_[i]);

	return res;
}

template <typename T>
Vector<T> Vector<T>::operator() () const
{
	Vector<T> res;

	res.resize(size_);
	register T* res_array = res.array_;
	for (register size_t i = 0; i < size_; ++i)
		res_array[i] = array_[i];

	return res;
}

template <typename T>
Vector<T> Vector<T>::operator() (int start) const
{
	Vector<T> res;

	if (start >= 0 && start < size_)
	{
		res.resize(1);
		res.array_[0] = array_[start];
	}

	return res;
}

template <typename T>
Vector<T> Vector<T>::operator() (int start, int stop) const
{
	Vector<T> res;
	size_t resSize;

	if (start >= 0 && start < size_)
	{
		if (start < stop)
		{
			resSize = (stop < size_ ? stop : size_);
			resSize -= start;
			res.resize(resSize);
			for (int i = start, copyIndex = 0; copyIndex < resSize; ++i, ++copyIndex)
				res.array_[copyIndex] = array_[i];
		}
	}

	return res;
}

template <typename T>
Vector<T> Vector<T>::operator() (int start, int stop, int step) const
{
	Vector<T> res;
	size_t resSize = 0;
	int index = start;
	int resWall;
	int copyIndex = 0;

	if (start >= 0 && start < size_)
	{
		if (step > 0)
		{
			if (start < stop)
			{
				resWall = (stop < size_ ? stop : size_);
				while (1)
				{
					if (index >= resWall) break;
					++resSize;
					index += step;
				}
			}
			else ++resSize;
		}
		else if (step < 0)
		{
			if (stop < start)
			{
				resWall = (stop > -1 ? stop : -1);
				while (1)
				{
					if (index <= resWall) break;
					++resSize;
					index += step;
				}
			}
			else ++resSize;
		}
		res.resize(resSize);
		for (index = start; copyIndex < resSize; index += step, ++copyIndex)
			res.array_[copyIndex] = array_[index];
	}

	return res;
}

template <typename T>
inline size_t Vector<T>::size() const
{
	return size_;
}

template <typename T>
Vector<T> operator+ (const Vector<T>& a, const Vector<T>& b)
{
	Vector<T> res;
	if (a.size_ == b.size_)
	{
		res.resize(a.size_);
		for (register size_t i = 0; i < a.size_; ++i)
			res[i] = a[i] + b[i];
	}
	else throw CalculateException();

	return res;
}

template <typename T>
Vector<T> operator- (const Vector<T>& a, const Vector<T>& b)
{
	Vector<T> res;
	if (a.size_ == b.size_)
	{
		res.resize(a.size_);
		for (register size_t i = 0; i < a.size_; ++i)
			res[i] = a[i] - b[i];
	}
	else throw CalculateException();

	return res;
}

template <typename T>
Vector<T> operator* (const Vector<T>& a, T b)
{
	Vector<T> res;
	if (a.size_)
	{
		res.resize(a.size_);
		for (register size_t i = 0; i < a.size_; ++i)
			res[i] = a[i] * b;
	}

	return res;
}

template <typename T>
Vector<T> operator* (T b, const Vector<T>& a)
{
	Vector<T> res;
	if (a.size_)
	{
		res.resize(a.size_);
		for (register size_t i = 0; i < a.size_; ++i)
			res[i] = a[i] * b;
	}

	return res;
}

template <typename T>
Vector<T> operator/ (const Vector<T>& a, T b)
{
	Vector<T> res;
	if (a.size_)
	{
		res.resize(a.size_);
		for (register size_t i = 0; i < a.size_; ++i)
			res[i] = a[i] / b;
	}

	return res;
}

template <typename T>
Vector<T>& Vector<T>::operator+= (const Vector<T>& a)
{
	if (size_ == a.size_)
	{
		for (size_t i = 0; i < size_; ++i)
			array_[i] += a.array_[i];
	}
	else throw CalculateException();

	return *this;
}

template <typename T>
Vector<T>& Vector<T>::operator-= (const Vector<T>& a)
{
	if (size_ == a.size_)
	{
		for (size_t i = 0; i < size_; ++i)
			array_[i] -= a.array_[i];
	}
	else throw CalculateException();

	return *this;
}

template <typename T>
Vector<T>& Vector<T>::operator*= (T a)
{
	for (register size_t i = 0; i < size_; ++i)
		array_[i] *= a;

	return *this;
}

template <typename T>
Vector<T>& Vector<T>::operator/= (T a)
{
	for (register size_t i = 0; i < size_; ++i)
		array_[i] /= a;

	return *this;
}

template <typename T>
T operator, (const Vector<T>& a, const Vector<T>& b)
{
	T res = 0;
	if (a.size_ == b.size_ && a.size_)
	{
		for (register size_t i = 0; i < a.size_; ++i)
			res += a[i] * b[i];
	}
	else throw CalculateException();

	return res;
}

template <typename T>
std::ostream& operator<< (std::ostream& stream, const Vector<T>& arg)
{
	for (register size_t i = 0; i < arg.size_; ++i)
	{
		// Печатаем все элементы
		stream << arg.array_[i];
		if (i != arg.size() - 1)
			stream << ", ";
	}

	return stream;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////    МАТРИЦА СОСТОИТ ИЗ ВЕКТОРА ВЕКТОРОВ    /////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


template <typename T>
class Matrix
{
private:
	Vector<Vector<T>> array_;
public:
	class Slice;
	Matrix(); // Конструктор по умолчанию
	Matrix(const std::initializer_list< std::initializer_list<T> >&); // Инициализация с инициализирующим листом
	Matrix(const Matrix&); // Конструктор копирования
	Matrix(Matrix&&); // Перемещения
	~Matrix(); // Деструктор

	Matrix<T>& operator= (const Matrix<T>&); // Оператор присваивания
	Matrix<T>& operator= (Matrix<T>&&); // Опертор присваивания с перемещением
	Matrix<T>& operator= (const Vector<T>&);

	inline Vector<T>& operator[] (size_t);
	inline const Vector<T>& operator[] (size_t) const;

	void resize(size_t, size_t);

	Matrix<T>& apply(T(*)(T));
	Matrix<T> map(T(*)(T)) const;

	size_t columns() const;
	size_t rows() const;

	Matrix<T>& transpose();
	template <typename D>
	friend Matrix<D> transpose_matrix(const Matrix<D>&);
	template <typename D>
	friend Matrix<D> operator* (const Matrix<D>&, const Matrix<D>&);
	template <typename D>
	friend Matrix<D> operator- (const Matrix<D>&, const Matrix<D>&);
	template <typename D>
	friend Matrix<D> operator+ (const Matrix<D>&, const Matrix<D>&);

	template <typename D>
	friend Matrix<D> operator* (const Matrix<D>&, D);
	template <typename D>
	friend Matrix<D> operator* (D, const Matrix<D>&);

	template <typename D>
	friend std::ostream& operator<< (std::ostream&, const Matrix<D>&);

	Matrix<T>::Slice operator() () const;
	Matrix<T>::Slice operator() (int start) const;
	Matrix<T>::Slice operator() (int start, int stop) const;
	Matrix<T>::Slice operator() (int start, int stop, int step) const;

	template <typename D>
	friend Matrix<D> transpose_matrix(const Vector<D>&);

	// Slice
public:
	class Slice
	{
	private:
		const Vector<Vector<T>>& array_;
		Vector<size_t> index_array_;
	public:

		Slice(Vector<Vector<T>>&, Vector<size_t>);
		Slice() = delete;
		Slice(const Slice&) = delete;
		Slice& operator= (const Slice&) = delete;
		Slice& operator= (Slice&&) = delete;
		Slice(Matrix::Slice&&);
		//~Slice();

		Matrix<T> operator() () const;
		Matrix<T> operator() (int start) const;
		Matrix<T> operator() (int start, int stop) const;
		Matrix<T> operator() (int start, int stop, int step) const;
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//                                               ОПЕРАТОРЫ ПРИВЕДЕНИЯ ТИПА
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		operator Matrix<T>() const
		{
			Matrix<T> res;
			size_t resRows;
			size_t index;

			resRows = index_array_.size();
			if (resRows)
			{
				res.resize(resRows, array_[0].size());
				for (size_t i = 0; i < resRows; ++i)
				{
					index = index_array_[i];
					res[i] = array_[index];
				}
			}

			return res;
		}
	};

	operator Vector<T>() const
	{
		Vector<T> res;
		size_t resSize;

		resSize = array_.size();
		if (resSize && array_[0].size() == 1)
		{
			res.resize(resSize);
			for (register size_t i = 0; i < resSize; ++i)
				res[i] = array_[i][0];
		}
		else throw CalculateException();

		return res;
	}
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                          ОПРЕДЕЛЕНИЕ МЕТОДОВ МАТРИЦЫ
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename T>
Matrix<T>::Matrix()
{ }

template <typename T>
Matrix<T>::Matrix(const std::initializer_list< std::initializer_list<T> >& arg)
{
	size_t newColumns;
	size_t newRows;

	newRows = arg.size();
	if (newRows)
	{
		newColumns = arg.begin()->size();

		array_.resize(newRows);
		for (size_t i = 0; i < newRows; ++i)
			array_[i].resize(newColumns);

		size_t copyI, copyJ;
		auto i = arg.begin();
		for (copyI = 0; i != arg.end(); ++i, ++copyI)
		{
			auto j = i->begin();
			for (copyJ = 0; j != i->end() && copyJ < newColumns; ++j, ++copyJ)
				array_[copyI][copyJ] = *j;
		}
	}
}

template <typename T>
Matrix<T>::Matrix(const Matrix<T>& arg)
{
	size_t newColumns;
	size_t newRows;

	newRows = arg.array_.size();
	if (newRows)
	{
		newColumns = arg[0].size();
		resize(newRows, newColumns);

		for (size_t i = 0; i < newRows; ++i)
			for (size_t j = 0; j < newColumns; ++j)
				array_[i][j] = arg[i][j];
	}
}

template <typename T>
Matrix<T>::Matrix(Matrix&& arg) : array_(arg.array_)
{ }

template <typename T>
Matrix<T>::~Matrix()
{ }

template <typename T>
void Matrix<T>::resize(size_t newRows, size_t newColumns)
{
	if (!newRows || !newColumns)
	{
		array_.resize(0);
	}
	else
	{
		array_.resize(newRows);
		for (size_t i = 0; i < newRows; ++i)
			array_[i].resize(newColumns);
	}
}

template <typename T>
Matrix<T> Matrix<T>::map(T(*f)(T)) const
{
	Matrix<T> res;
	size_t resRows;
	size_t resColumns;

	resRows = array_.size();
	if (resRows)
	{
		resColumns = array_[0].size();
		if (resColumns)
		{
			res.resize(resRows, resColumns);
			for (size_t i = 0; i < resRows; ++i)
				for (size_t j = 0; j < resColumns; ++j)
					res[i][j] = f(array_[i][j]);
		}
	}

	return res;
}

template <typename T>
Matrix<T>& Matrix<T>::apply(T(*f)(T))
{
	register size_t resRows;
	register size_t resColumns;

	resRows = array_.size();
	if (resRows)
	{
		resColumns = array_[0].size();
		for (size_t i = 0; i < resRows; ++i)
			for (size_t j = 0; j < resColumns; ++j)
				array_[i][j] = f(array_[i][j]);
	}

	return *this;
}

template <typename T>
Matrix<T>& Matrix<T>::transpose()
{
	Matrix<T> res;
	size_t resRows;
	size_t resColumns;

	resRows = array_.size();
	if (resRows)
	{
		resColumns = array_[0].size();
		res.resize(resColumns, resRows);
		for (size_t i = 0; i < resColumns; ++i)
			for (size_t j = 0; j < resRows; ++j)
				res[i][j] = array_[j][i];
		operator=(res);
	}

	return *this;
}

template <typename T>
Matrix<T>& Matrix<T>::operator= (const Matrix<T>& arg)
{
	if (&arg != this)
	{
		size_t newRows;
		size_t newColumns;

		newRows = arg.array_.size();
		if (newRows)
		{
			newColumns = arg[0].size();
			resize(newRows, newColumns);
			for (size_t i = 0; i < newRows; ++i)
				for (size_t j = 0; j < newColumns; ++j)
					array_[i][j] = arg.array_[i][j];
		}
	}

	return *this;
}

template <typename T>
Matrix<T>& Matrix<T>::operator= (Matrix&& arg)
{
	if (&arg != this)
	{
		array_ = arg.array_;
	}

	return *this;
}

template <typename T>
Matrix<T>& Matrix<T>::operator= (const Vector<T>& arg)
{
	if (&arg != this)
	{
		size_t resRows;
		resRows = arg.size();
		resize(resRows, 1);
		if (resRows)
			for (size_t i = 0; i < resRows; ++i)
				array_[i][0] = arg[i];
	}

	return *this;
}

template <typename T>
inline Vector<T>& Matrix<T>::operator[] (size_t index)
{
	return array_[index];
}

template <typename T>
inline const Vector<T>& Matrix<T>::operator[] (size_t index) const
{
	return array_[index];
}

template <typename T>
inline size_t Matrix<T>::rows() const
{
	return array_.size();
}

template <typename T>
inline size_t Matrix<T>::columns() const
{
	if (array_.size() > 0)
		return array_[0].size();
	return 0;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                          АРИФМЕТИЧЕСКИЕ ОПЕРАЦИИ
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename T>
Matrix<T> transpose_matrix(const Matrix<T>& arg)
{
	Matrix<T> res;
	size_t resRows;
	size_t resColumns;

	resRows = arg.array_.size();
	if (resRows)
	{
		resColumns = arg.array_[0].size();
		res.resize(resColumns, resRows);
		for (size_t i = 0; i < resColumns; ++i)
			for (size_t j = 0; j < resRows; ++j)
				res.array_[i][j] = arg.array_[j][i];
	}

	return res;
}

template <typename T>
Matrix<T> transpose_matrix(const Vector<T>& arg)
{
	Matrix<T> res;
	size_t resSize;

	resSize = arg.size();
	if (resSize)
	{
		res.resize(1, resSize);
		for (size_t i = 0; i < resSize; ++i)
			res[0][i] = arg[i];
	}

	return res;
}

template <typename T>
Vector<T> operator* (const Matrix<T>& a, const Vector<T>& b)
{
	Vector<T> res;
	size_t resRows;

	if (a.columns() == b.size())
	{
		T number;
		resRows = a.rows();
		res.resize(resRows);
		for (size_t i = 0; i < resRows; ++i)
		{
			number = 0;
			for (size_t k = 0; k < b.size(); ++k)
				number += a[i][k] * b[k];
			res[i] = number;
		}
	}
	else throw CalculateException();

	return res;
}

template <typename T>
Matrix<T> operator* (const Matrix<T>& a, const Matrix<T>& b)
{
	Matrix<T> res;
	size_t resRows;
	size_t resColumns;
	size_t resDimention;
	T number;

	if (a.columns() == b.rows())
	{
		resRows = a.rows();
		resColumns = b.columns();
		resDimention = a.columns();
		res.resize(resRows, resColumns);
		for (size_t i = 0; i < resRows; ++i)
		{
			for (size_t j = 0; j < resColumns; ++j)
			{
				number = 0;
				for (register size_t k = 0; k < resDimention; ++k)
					number += a[i][k] * b[k][j];
				res[i][j] = number;
			}
		}
	}
	else throw CalculateException();

	return res;
}

template <typename T>
Matrix<T> operator* (const Matrix<T>& a, T b)
{
	Matrix<T> res;
	size_t resRows;
	size_t resColumns;

	resRows = a.rows();
	if (resRows)
	{
		resColumns = a.columns();
		res.resize(resRows, resColumns);
		for (size_t i = 0; i < resRows; ++i)
			for (size_t j = 0; j < resColumns; ++j)
				res[i][j] = a[i][j] * b;
	}

	return res;
}

template <typename T>
Matrix<T> operator* (T b, const Matrix<T>& a)
{
	return operator* (a, b);
}

template <typename T>
Matrix<T> operator+ (const Matrix<T>& a, const Matrix<T>& b)
{
	Matrix<T> res;
	size_t resRows;
	size_t resColumns;

	resRows = a.rows();
	resColumns = a.columns();
	if (resRows == b.rows() && resColumns == b.columns())
	{
		res.resize(resRows, resColumns);
		for (size_t i = 0; i < resRows; ++i)
			for (size_t j = 0; j < resColumns; ++j)
				res[i][j] = a[i][j] + b[i][j];
	}
	else throw CalculateException();

	return res;
}

template <typename T>
Matrix<T> operator- (const Matrix<T>& a, const Matrix<T>& b)
{
	Matrix<T> res;
	size_t resRows;
	size_t resColumns;

	resRows = a.rows();
	resColumns = a.columns();
	if (resRows == b.rows() && resColumns == b.columns())
	{
		res.resize(resRows, resColumns);
		for (size_t i = 0; i < resRows; ++i)
			for (size_t j = 0; j < resColumns; ++j)
				res[i][j] = a[i][j] - b[i][j];
	}
	else throw CalculateException();

	return res;
}

template <typename T>
std::ostream& operator<< (std::ostream& stream, const Matrix<T>& arg)
{
	for (size_t i = 0; i < arg.rows(); ++i)
	{
		for (size_t j = 0; j < arg.columns(); ++j)
		{
			stream << arg[i][j];
			if (j != arg.columns() - 1)
				stream << ", ";
		}
		if (i != arg.rows() - 1)
			stream << '\n';
	}

	return stream;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                  SLICES
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
typename Matrix<T>::Slice Matrix<T>::operator() () const
{
	Vector<size_t> indexArray;

	indexArray.resize(array_.size());
	for (register size_t i = 0; i < array_.size(); ++i)
		indexArray[i] = i;

	return Matrix<T>::Slice((Vector<Vector<T>>&)array_, indexArray);
}

template <typename T>
typename Matrix<T>::Slice Matrix<T>::operator() (int start) const
{
	Vector<size_t> indexArray;

	if (start >= 0 && start < array_.size())
	{
		indexArray.resize(1);
		indexArray[0] = start;
	}

	return Matrix<T>::Slice((Vector<Vector<T>>&)array_, indexArray);
}

template <typename T>
typename Matrix<T>::Slice Matrix<T>::operator() (int start, int stop) const
{
	Vector<size_t> indexArray;

	indexArray.resize(array_.size());
	for (size_t i = 0; i < array_.size(); ++i)
		indexArray[i] = i;
	indexArray = indexArray(start, stop);

	return Matrix<T>::Slice((Vector<Vector<T>>&)array_, indexArray);
}

template <typename T>
typename Matrix<T>::Slice Matrix<T>::operator() (int start, int stop, int step) const
{
	Vector<size_t> indexArray;

	indexArray.resize(array_.size());
	for (size_t i = 0; i < array_.size(); ++i)
		indexArray[i] = i;
	indexArray = indexArray(start, stop, step);

	return Matrix<T>::Slice((Vector<Vector<T>>&)array_, indexArray);
}

template <typename T>
Matrix<T>::Slice::Slice(Vector<Vector<T>>& p, Vector<size_t> i) : index_array_(i), array_(p)
{ }

template <typename T>
Matrix<T>::Slice::Slice(Matrix<T>::Slice&& arg) : index_array_(arg.index_array_), array_(arg.array_)
{ }

template <typename T>
Matrix<T> Matrix<T>::Slice::operator() () const
{
	Matrix<T> res;
	size_t index;
	size_t resRows;
	size_t resColumns;

	resRows = index_array_.size();
	if (resRows)
	{
		resColumns = array_[0].size();
		res.resize(resRows, resColumns);
		for (size_t i = 0; i < resRows; ++i)
		{
			index = index_array_[i];
			for (size_t j = 0; j < resColumns; ++j)
				res[i][j] = array_[index][j];
		}
	}

	return res;
}

template <typename T>
Matrix<T> Matrix<T>::Slice::operator() (int start) const
{
	Matrix<T> res;
	size_t index;
	size_t resRows;

	resRows = index_array_.size();
	if (resRows)
	{
		if (start >= 0 && start < array_[0].size())
		{
			res.resize(resRows, 1);
			for (size_t i = 0; i < resRows; ++i)
			{
				index = index_array_[i];
				res[i][0] = array_[index][start];
			}
		}
	}

	return res;
}

template <typename T>
Matrix<T> Matrix<T>::Slice::operator() (int start, int stop) const
{
	Matrix<T> res;
	size_t resRows;
	size_t index;

	resRows = index_array_.size();
	if (resRows)
	{
		res.resize(resRows, 1);
		for (size_t i = 0; i < resRows; ++i)
		{
			index = index_array_[i];
			res[i] = array_[index](start, stop);
		}
	}

	return res;
}

template <typename T>
Matrix<T> Matrix<T>::Slice::operator() (int start, int stop, int step) const
{
	Matrix<T> res;
	size_t resRows;
	size_t index;

	resRows = index_array_.size();
	if (resRows)
	{
		res.resize(resRows, 1);
		for (size_t i = 0; i < resRows; ++i)
		{
			index = index_array_[i];
			res[i] = array_[index](start, stop, step);
		}
	}

	return res;
}
