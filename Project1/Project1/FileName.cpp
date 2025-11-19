#include <iostream>
#include <stdexcept>
#include <complex>
#include <random>



// Константа точности сравнения
template<typename T>
struct Traits {
	static constexpr T epsilon = static_cast<T>(1E-6);
};

// Специализация для комплексных типов
template<>
struct Traits<std::complex<float>> {
	static constexpr float epsilon = 1E-3f;
};
template<>
struct Traits<std::complex<double>> {
	static constexpr double epsilon = 1E-6;
};


// функция для сравнения
template<typename T>
bool nearly_equal(T a, T b) {
	return std::abs(a - b) <= Traits<T>::epsilon;
}



//класс матрицы
template <typename T>
class Matrix {
private:
	size_t rows_;
	size_t cols_;
	T* data_;


public:
	//конструктор с параметрами: 
	// размеры матрицы и значение для заполнения;                                вроде бы done
	Matrix(size_t rows, size_t cols, const T& value): 
		rows_(rows), cols_(cols), data_(nullptr) {
		if (rows == 0 || cols == 0) {
			throw std::invalid_argument("The dimensions of the matrix must be positive: those are the rules");
		}
		data_ = new T[rows * cols];
		for (size_t i = 0; i < rows * cols; ++i) {
			data_[i] = value;
		}

	}



	/*конструктор с параметрами, заполняющий матрицу случайными значениями. 
	Параметры: размеры матрицы, нижняя граница, верхняя граница;                   кое-как done     возможно потом переделаю, не факт, ну если только попросите, не, не сделаю     */ 
	/*Matrix(size_t rows, size_t cols, T up, T low) : rows_(rows), cols_(cols){
		if (rows == 0 || cols == 0) {
			throw std::runtime_error("Incorrect size of matrix");
		}
		data_ = new T[rows * cols];
		for (size_t i = 0; i < rows * cols; ++i) {
			data_[i] = low + static_cast<T>(rand()) / (static_cast<T>(RAND_MAX) / (up - low));
		}
	}*/

	//конструктор с параметрами, заполняющий матрицу случайными значениями.
	Matrix(size_t rows, size_t cols, T low, T up) : rows_(rows), cols_(cols), data_(nullptr) {
		if (rows == 0 || cols == 0)
			throw std::invalid_argument("Incorrect size of matrix");
		data_ = new T[rows * cols];
		std::random_device rd;
		std::mt19937 gen(rd());

		if constexpr (std::is_floating_point<T>::value || std::is_same<T, std::complex<float>>::value || std::is_same<T, std::complex<double>>::value) {
			// Для комплексных чисел заполняем отдельно реальную и мнимую части
			if constexpr (std::is_same<T, std::complex<float>>::value || std::is_same<T, std::complex<double>>::value) {
				std::uniform_real_distribution<typename T::value_type> dist(low.real(), up.real());
				for (size_t i = 0; i < rows * cols; ++i) {
					typename T::value_type re = dist(gen);
					typename T::value_type im = dist(gen);
					data_[i] = T(re, im);
				}
			}
			else {
				std::uniform_real_distribution<T> dist(low, up);
				for (size_t i = 0; i < rows * cols; ++i)
					data_[i] = dist(gen);
			}
		}
		else if constexpr (std::is_integral<T>::value) {
			std::uniform_int_distribution<T> dist(low, up);
			for (size_t i = 0; i < rows * cols; ++i)
				data_[i] = dist(gen);
		}
		else {
			static_assert(sizeof(T) == 0, "Unknown type");
		}
	}


	
	//конструктор копирования                                                        done      
	Matrix(const Matrix& another): 
		rows_(another.rows_), cols_(another.cols_), data_(nullptr)  {
		data_ = new T[rows_ * cols_];
		for (size_t i = 0; i < rows_ * cols_; ++i) {
			data_[i] = another.data_[i];
		}
	}



	//Деструктор                                                                      done
	~Matrix() {
		delete[] data_;
	}



	size_t rows() const { return rows_; }
	size_t cols() const { return cols_; }


	// Оператор присваивания
	Matrix& operator=(const Matrix& other) {
		if (this != &other) {
			delete[] data_;
			rows_ = other.rows_;
			cols_ = other.cols_;
			data_ = new T[rows_ * cols_];
			for (size_t i = 0; i < rows_ * cols_; ++i)
				data_[i] = other.data_[i];
		}
		return *this;
	}



	// оператор сравнения на равенство и неравенство
	bool operator==(const Matrix& other) const {
		if (rows_ != other.rows_ || cols_ != other.cols_)
			return false;
		for (size_t i = 0; i < rows_ * cols_; ++i) {
			if (!nearly_equal(data_[i], other.data_[i]))
				return false;
		}
		return true;
	}

	bool operator!=(const Matrix& other) const {
		return !(*this == other);
	}

	

	//оператор () для чтения/записи элемента матрицы по указанным индексам; (один константный, другой неконстантный)          done
	T& operator() (size_t i, size_t j) {
		if (i >= rows_ || j >= cols_) {
			throw std::out_of_range("This index is out of range");
		}
		return data_[i * cols_ + j];
	 }

	T& operator() (size_t i, size_t j) const {
		if (i >= rows_ || j >= cols_) {
			throw std::out_of_range("This index is out of range");
		}
		return data_[i * cols_ + j];
	}



	//оператор сложения матриц
	Matrix operator+(const Matrix& other) const {
		if (rows_ != other.rows_ || cols_ != other.cols_)
			throw std::invalid_argument("Matrix sizes are different");
		Matrix result(rows_, cols_, T());
		for (size_t i = 0; i < rows_ * cols_; ++i)
			result.data_[i] = data_[i] + other.data_[i];
		return result;
	}



	//оператор вычитания матриц
	Matrix operator-(const Matrix& other) const {
		if (rows_ != other.rows_ || cols_ != other.cols_)
			throw std::invalid_argument("Matrix sizes are different");
		Matrix result(rows_, cols_, T());
		for (size_t i = 0; i < rows_ * cols_; ++i)
			result.data_[i] = data_[i] - other.data_[i];
		return result;
	}


			
	//оператор умножения матриц
	Matrix operator*(const Matrix& rix) const {
		if (cols_ != rix.rows_) {
			throw std::invalid_argument("Matrix sizes are different");
		}
		Matrix result(rows_, cols_, T());
		for (size_t i = 0; i < rows_; ++i) {
			for (size_t j = 0; j < rix.cols_; ++j) {
				T sum = T();
				for (size_t k = 0; k < cols_; ++k) {
					sum += (*this)(i, k) * rix(k, j);
				}
				result(i, j) = sum;
			}
		}
		return result;
	}

	//оператор умножения матрицы на скаляр (обеспечить коммутативность)
	Matrix operator*(const T& scalar) const {
		Matrix result(rows_, cols_, T());
		for (size_t i = 0; i < rows_ * cols_; ++i)
			result.data_[i] = data_[i] * scalar;
		return result;
	}

	friend Matrix operator*(const T& scalar, const Matrix& m) {
		return m * scalar;
	}



	//оператор деления матрицы на скаляр
	Matrix operator/(const T& scalar) const {
		if (scalar == T()) throw std::invalid_argument("We can't divide by 0");
		
		Matrix result(rows_, cols_, T());
		for (size_t i = 0; i < rows_ * cols_; ++i) {
			result.data_[i] = data_[i] / scalar;
		}
		return result;
	}



	//вычисление следа матрицы
	T trace() const {
		if (rows_ != cols_)
			throw std::invalid_argument("Matrix sizes are different");
		T sum = T();
		for (size_t i = 0; i < rows_; ++i)
			sum += (*this)(i, i);
		return sum;
	}

};



//перегрузка оператора вывода                                                                 наверное     done
template <typename T>
std::ostream& operator<<(std::ostream& os, const Matrix<T>& rix){
	for (size_t i = 0; i < rix.rows(); ++i) {
		for(size_t j = 0; j < rix.cols(); ++j) {
			os << rix(i, j) << "   ";
		}
		os << "\n";
	}
	return os;
}



//Привести заданную квадратную матрицу A к нижнетреугольному виду 
//(разложить на верхне- и нижнетреугольную матрицу, при произведении дающую исходную).
template<typename T>
void to_lower_triangular(const Matrix<T>& A) {
	size_t n = A.rows();
	if (n != A.cols())
		throw std::invalid_argument("Matrix sizes are different");
	Matrix<T> L = Matrix<T>(n, n, T(0));
	Matrix<T> U = Matrix<T>(n, n, T(0));

	for (size_t i = 0; i < n; ++i) {
		// Верхняя
		for (size_t j = i; j < n; ++j) {
			T sum = T();
			for (size_t k = 0; k < i; ++k)
				sum += L(i, k) * U(k, j);
			U(i, j) = A(i, j) - sum;
		}

		// Нижняя
		for (size_t j = i; j < n; ++j) {
			if (i == j)
				L(i, j) = T(1);
			else {
				T sum = T();
				for (size_t k = 0; k < i; ++k)
					sum += L(j, k) * U(k, i);
				if (U(i, i) == T())
					throw std::runtime_error("Matrix is singular");
				L(j, i) = (A(j, i) - sum) / U(i, i);
			}
		}
	}
	std::cout << L;
}


int main() {
	try {
		//проверка всех операторов и методов

		size_t size;
		std::cout << "Print size of matrix: ";
		std::cin >> size;

		// Создаем матрицу 3x3 со случайными значениями
		Matrix<std::complex<float>> A(size, size, -10.0f, 10.0f);

		std::cout << "Matrix A:\n";
		for (size_t i = 0; i < size; ++i) {
			for (size_t j = 0; j < size; ++j) {
				auto& val = A(i, j);
				std::cout << "(" << val.real() << " + " << val.imag() << "i) ";
			}
			std::cout << "\n";
		}
        
		std::cout << "lower tringular complex matrix: \n";
		to_lower_triangular(A);

		Matrix<double> rix(3, 3, -10.0, 10.0);
		std::cout << "Your matrix:\n" << rix;


		std::cout << "lower tringular matrix: \n";
		to_lower_triangular(rix);

		double s = rix.trace();
		std::cout << "Trace of matrix: " << s << "\n";

		
		Matrix<double> rix2(3, 3, -10, 10);
		std::cout << "Matrix 2:\n" << rix2;

		auto sum = rix + rix2;
		std::cout << "Sum:\n" << sum;

		auto diff = rix - rix2;
		std::cout << "Difference:\n" << diff;

		auto prod = rix * rix2;
		std::cout << "Product:\n" << prod;

		auto scaled = rix * 2.5;
		std::cout << "Scaled:\n" << scaled;

		auto divided = rix / 2.0;
		std::cout << "Divided:\n" << divided;

		std::cout << "matrix1 == matrix2? " << (rix == rix2 ? "Yes" : "No") << "\n";
		std::cout << "matrix1 != matrix2? " << (rix != rix2 ? "Yes" : "No") << "\n";



	}
	catch (const std::exception& ex) {
		std::cerr << "Oh, there is a mistake: " << ex.what() << "\n";
	}



	return 0;
}
