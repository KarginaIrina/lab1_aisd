#include <iostream>
#include <stdexcept>
#include <complex>
#include <random>


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
	Параметры: размеры матрицы, нижняя граница, верхняя граница;                   кое-как done     возможно потом переделаю     */ 
	Matrix(size_t rows, size_t cols, T up, T low) : rows_(rows), cols_(cols)
	{
		if (rows == 0 || cols == 0) {
			throw std::runtime_error("Incorrect size of matrix");
		}
		data_ = new T[rows * cols];
		for (size_t i = 0; i < rows * cols; ++i) {
			data_[i] = low + static_cast<T>(rand()) / (static_cast<T>(RAND_MAX) / (up - low));
		}
	};


	
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


	// оператор сравнения на равенство и неравенство

	

	//оператор () для чтения/записи элемента матрицы по указанным индексам; (один константный, другой неконстантный)          done

	T& operator() (size_t i, size_t j) {
		if (i >= rows_ || j >= cols_) {
			throw std::out_of_range("This index is out of range");
		}
		return data_[i * cols_ + j];
	 }

	const T& operator() (size_t i, size_t j) const {
		if (i >= rows_ || j >= cols_) {
			throw std::out_of_range("This index is out of range");
		}
		return data_[i * cols_ + j];
	}






	//оператор сложения матриц



	//оператор вычитания матриц



	
	//оператор умножения матриц




	//оператор умножения матрицы на скаляр (обеспечить коммутативность)




	//оператор деления матрицы на скаляр




	//вычисление следа матрицы




	//Привести заданную квадратную матрицу A к нижнетреугольному виду 
	//(разложить на верхне- и нижнетреугольную матрицу, при произведении дающую исходную).


};



//перегрузка оператора вывода                                                                 наверное     done
template <typename T>
std::ostream& operator<<(std::ostream& os, const Matrix<T>& rix){
	for (size_t i = 0; i < rix.rows(); ++i) {
		for(size_t j = 0; j < rix.cols(); ++j) {
			os << rix(i, j) << ' ';
		}
		os << "\n";
	}
	return os;
}





int main() {
	try {
		//проверка всех операторов и методов
		Matrix<float> rix(3, 3, -10.0f, 10.0f);
		std::cout << "Your matrix: \n" << rix;


		Matrix< std::complex<float>> rx(3, 3, -10.0f, 10.0f);
		std::cout << "Your matrix: \n";
		for (size_t i = 0; i < 3; ++i) {
			for (size_t j = 0; j < 3; ++j) {
				auto& val = rx(i, j);
				std::cout << "(" << val.real() << " + " << val.imag() << "i) ";
			}
			std::cout << "\n";
		}




	}
	catch (const std::exception& ex) {
		std::cerr << "Oh, there is a mistake: " << ex.what() << "\n";
	}



	return 0;
}
