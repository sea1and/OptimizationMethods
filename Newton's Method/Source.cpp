#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>
using namespace std;

//Класс для работы с векторами 1х2
class Vector1x2 {
public:
	double x1, x2;
	Vector1x2() {
		x1 = 0;
		x2 = 0;
	}
	Vector1x2(double a, double b) {
		x1 = a;
		x2 = b;
	}
};

//Класс для работы с векторами 2х1
class Vector2x1 {
public:

	double x1, x2;

	Vector2x1(double a, double b) {
		x1 = a;
		x2 = b;
	}
	Vector2x1() {
		x1 = 0;
		x2 = 0;
	}
	friend Vector2x1 operator + (Vector2x1 vec1, Vector2x1 vec2) //сложение векторов
	{
		Vector2x1 tempVec = Vector2x1(vec1.x1 + vec2.x1, vec1.x2 + vec2.x2);
		return tempVec;
	}
	friend Vector2x1 operator - (Vector2x1 vec1, Vector2x1 vec2) //вычитание векторов
	{
		Vector2x1 tempVec = Vector2x1(vec1.x1 - vec2.x1, vec1.x2 - vec2.x2);
		return tempVec;
	}
	Vector2x1 operator += (const Vector2x1& vec) {
		x1 += vec.x1;
		x2 += vec.x2;
		return *this;
	}
	friend Vector2x1 operator * (Vector2x1 vec, double a) //умножение вектора на скаляр
	{
		Vector2x1 tempVec = Vector2x1(vec.x1*a, vec.x2*a);
		return tempVec;
	}
	double norm() {
		return sqrt(x1*x1 + x2*x2);
	}
	Vector1x2 transposition() {
		Vector1x2 result = Vector1x2(x1, x2);
		return result;
	}
};

//Функция f1
class f1 {
public:
	double operator() (Vector2x1 v) {
		return 100 * (v.x2 - v.x1*v.x1)*(v.x2 - v.x1*v.x1) + 5 * (1 - v.x1)*(1 - v.x1);
	}
	Vector2x1 gradient(Vector2x1 v) {
		double x1 = 10 * (40 * v.x1*v.x1*v.x1 - 40 * v.x1*v.x2 + v.x1 - 1);
		double x2 = 200 * (v.x2 - v.x1*v.x1);
		Vector2x1 result = Vector2x1(x1, x2);
		return result;
	}
};

//Функция f2
class f2 {
public:
	double operator() (Vector2x1 v) {
		return (v.x1*v.x1 + v.x2 - 11)*(v.x1*v.x1 + v.x2 - 11) + (v.x1 + v.x2*v.x2 - 7)*(v.x1 + v.x2*v.x2 - 7);
	}
	Vector2x1 gradient(Vector2x1 v) {
		double x1 = 2 * (2 * v.x1*(v.x1*v.x1 + v.x2 - 11) + v.x1 + v.x2*v.x2 - 7);
		double x2 = 2 * (v.x1*v.x1 + 2 * v.x2*(v.x1 + v.x2*v.x2 - 7) + v.x2 - 11);
		Vector2x1 result = Vector2x1(x1, x2);
		return result;
	}
};

//Класс для работы с матрицами
class Matrix2x2 {
public:
	double a11, a12, a21, a22;
	Matrix2x2(double a, double b, double c, double d) {
		a11 = a;
		a12 = b;
		a21 = c;
		a22 = d;
	}
	Matrix2x2() {
		a11 = 1;
		a12 = 0;
		a21 = 0;
		a22 = 1;
	}
	friend Matrix2x2 operator + (Matrix2x2 mat1, Matrix2x2 mat2) //сложение матриц
	{
		Matrix2x2 tempMat = Matrix2x2(mat1.a11+mat2.a11, mat1.a12 + mat2.a12, mat1.a21 + mat2.a21, mat1.a22 + mat2.a22);
		return tempMat;
	}
	Vector2x1 multiplication(Vector2x1 vec) {
		Vector2x1 result = Vector2x1(a11*vec.x1 + a12*vec.x2, a21*vec.x1 + a22*vec.x2);
		return result;
	}
	Matrix2x2 delenie(double a) {
		return Matrix2x2(a11 / a, a12 / a, a21 / a, a22 / a);
	}
};

//умножение двух векторов и получение матрицы 2x2
Matrix2x2 multiply_2x1_and_1x2(Vector2x1 vec1, Vector1x2 vec2) {
	Matrix2x2 tempMat = Matrix2x2(vec1.x1*vec2.x1, vec1.x1*vec2.x2, vec1.x2*vec2.x1, vec1.x2*vec2.x2);
	return tempMat;
}

//умножение двух векторов и получение скаляра
double multiply_1x2_and_2x1(Vector1x2 vec1, Vector2x1 vec2) {
	double result = vec1.x1*vec2.x1 + vec1.x2*vec2.x2;
	return result;
}

//Метод дихотомии для функции f1
void dihotomyForF1(double leftBorder, double rightBorder, double epsilon, Vector2x1 x_k, Matrix2x2 H_k, double& output) {
	double nextLeftBorder, nextRightBorder;
	double delta = epsilon;

	f1 f;
	Vector2x1 gradient = f.gradient(x_k);

	double a1 = (leftBorder + rightBorder - delta) / 2;
	double a2 = (leftBorder + rightBorder + delta) / 2;
	double f_a1 = f(x_k - H_k.multiplication(gradient)*a1);
	double f_a2 = f(x_k - H_k.multiplication(gradient)*a2);

	//Выбираем новые границы в зависимости от значений f_x1 и f_x2
	if(f_a1 <= f_a2) {
		nextLeftBorder = leftBorder;
		nextRightBorder = a2;
	} else {
		nextLeftBorder = a1;
		nextRightBorder = rightBorder;
	}

	double nextEpsilon = (nextRightBorder - nextLeftBorder) / 2;
	if(nextEpsilon <= epsilon) {
		//Остановка процесса и вывод результата на экран
		double result_a = (nextLeftBorder + nextRightBorder) / 2;
		double result_f_a = f(x_k - H_k.multiplication(gradient)*result_a);
		output = result_a;
		return;
	} else {
		//Следующий шаг
		dihotomyForF1(nextLeftBorder, nextRightBorder, epsilon, x_k, H_k, output);
	}
}

//Метод дихотомии для функции f2
void dihotomyForF2(double leftBorder, double rightBorder, double epsilon, Vector2x1 x_k, Matrix2x2 H_k, double& output) {
	double nextLeftBorder, nextRightBorder;
	double delta = epsilon;

	f2 f;
	Vector2x1 gradient = f.gradient(x_k);

	double a1 = (leftBorder + rightBorder - delta) / 2;
	double a2 = (leftBorder + rightBorder + delta) / 2;
	double f_a1 = f(x_k - H_k.multiplication(gradient)*a1);
	double f_a2 = f(x_k - H_k.multiplication(gradient)*a2);

	//Выбираем новые границы в зависимости от значений f_x1 и f_x2
	if(f_a1 <= f_a2) {
		nextLeftBorder = leftBorder;
		nextRightBorder = a2;
	} else {
		nextLeftBorder = a1;
		nextRightBorder = rightBorder;
	}

	double nextEpsilon = (nextRightBorder - nextLeftBorder) / 2;
	if(nextEpsilon <= epsilon) {
		//Остановка процесса и вывод результата на экран
		double result_a = (nextLeftBorder + nextRightBorder) / 2;
		double result_f_a = f(x_k - H_k.multiplication(gradient)*result_a);
		output = result_a;
		return;
	} else {
		//Следующий шаг
		dihotomyForF2(nextLeftBorder, nextRightBorder, epsilon, x_k, H_k, output);
	}
}

//Квазиньютоновский метод для функции f1
void minimizationF1(Vector2x1 x_k, Matrix2x2 H_k, double epsilon, int k, int count, Vector2x1& res) {
	f1 f;

	count++;
	if (count > 1000) {
		res = Vector2x1(0, 0);
		return;
	}

	Vector2x1 x_k_1;
	Matrix2x2 H_k_1;

	double alpha_k;
	dihotomyForF1(0, 100, 0.0000000001, x_k, H_k, alpha_k); //одномерная минимизация для нахождения шага

	Vector2x1 gradient = f.gradient(x_k);
	x_k_1 = x_k - H_k.multiplication(gradient)*alpha_k;

	//То есть на нечетных шагах матрица H_k_1 будет единичной, а на четных считается по формуле из Аббасова
	if((k + 1) % 2 == 0) {
		H_k_1 = Matrix2x2(); //единичная матрица
	} else {

		Vector2x1 gamma_k = f.gradient(x_k_1) - f.gradient(x_k);
		Vector2x1 lambda_k = x_k_1 - x_k;

		Vector2x1 temp = lambda_k - H_k.multiplication(gamma_k);

		Matrix2x2 up = multiply_2x1_and_1x2(temp, temp.transposition()); //числитель (матрица 2х2)
		double down = multiply_1x2_and_2x1(temp.transposition(), gamma_k); //знаменатель (число)
		H_k_1 = H_k + up.delenie(down); //H_k плюс дробь (числитель поделить на знаменатель)
	}
	if(f.gradient(x_k_1).norm() <= epsilon) { //критерий остановки
		res = x_k_1;
		return;
	} else {
		minimizationF1(x_k_1, H_k_1, epsilon, k + 1, count, res); //следующая итерация
	}
}

//Квазиньютоновский метод для функции f2
void minimizationF2(Vector2x1 x_k, Matrix2x2 H_k, double epsilon, int k, int count, Vector2x1& res) {
	f2 f;

	count++;
	if (count > 1000) {
		res = Vector2x1(0, 0);
		return;
	}

	Vector2x1 x_k_1;
	Matrix2x2 H_k_1;

	double alpha_k;
	dihotomyForF2(0, 100, 0.0000000001, x_k, H_k, alpha_k);

	Vector2x1 gradient = f.gradient(x_k);
	x_k_1 = x_k - H_k.multiplication(gradient)*alpha_k;

	if((k + 1) % 2 == 0) {
		H_k_1 = Matrix2x2(); //единичная матрица
	} else {

		Vector2x1 gamma_k = f.gradient(x_k_1) - f.gradient(x_k);
		Vector2x1 lambda_k = x_k_1 - x_k;

		Vector2x1 temp = lambda_k - H_k.multiplication(gamma_k);

		Matrix2x2 up = multiply_2x1_and_1x2(temp, temp.transposition()); //числитель
		double down = multiply_1x2_and_2x1(temp.transposition(), gamma_k); //знаменатель
		H_k_1 = H_k + up.delenie(down);
	}
	if(f.gradient(x_k_1).norm() <= epsilon) {
		res = x_k_1;
		return;
	} else {
		minimizationF2(x_k_1, H_k_1, epsilon, k + 1, count, res);
	}
}

//Рандомное вещ. в интервале от a до b
float RandomFloat(float a, float b) {
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}


int main() {

	setlocale(LC_ALL, "Russian");
	Matrix2x2 H0 = Matrix2x2();
	double epsilon;

	bool correct = false;
	//Просьба ввести пользователя погрешность
	while(!correct) {
		double eps;
		cout << "Введите погрешность: ";
		cin >> eps;
		if(eps > 0) {
			epsilon = eps;
			correct = true;
		} else {
			cout << "Неправильный ввод" << endl;
		}
	}

	f1 f_1;
	f2 f_2;

	cout << endl;
	cout << "Функция f1" << endl;

	char str;
	cout << "Вы хотите ввести начальные данные вручную, или сгенерировать их случайно? (y/n): ";
	cin >> str;

	if (str == 'y') {
		float a, b;
		cout << "Введите координату x1 = ";
		cin >> a;
		cout << "Введите координату x2 = ";
		cin >> b;
		vector<Vector2x1> PointStorage;
		Vector2x1 x0 = Vector2x1(a, b);

		Vector2x1 result;
		minimizationF1(x0, H0, epsilon, 0, 0, result);
		cout << "x1 = " << result.x1 << " ; x2 = " << result.x2 << endl;
	}
	else {
		vector<Vector2x1> storage;
		while (storage.size() < 1) {
			vector<Vector2x1> PointStorage;

			Vector2x1 x0 = Vector2x1(RandomFloat(-10, 10), RandomFloat(-10, 10));

			Vector2x1 result;
			minimizationF1(x0, H0, epsilon, 0, 0, result);

			if (result.x1 != 0 && result.x2 != 0) {

				bool found = false;
				for (auto& elem : storage) {
					if (sqrt((elem.x1 - result.x1)*(elem.x1 - result.x1) + (elem.x2 - result.x2)*(elem.x2 - result.x2)) <= 1) {
						found = true;
					}
				}

				if (!found) {
					storage.push_back(result);
					cout << "x1 = " << result.x1 << " ; x2 = " << result.x2 << endl;
				}
			}
		}
	}


	cout << endl;
	cout << "Функция f2" << endl;
	cout << "Вы хотите ввести начальные данные вручную, или сгенерировать их случайно? (y/n): ";
	cin >> str;

	if (str == 'y') {
		float a, b;
		cout << "Введите координату x1 = ";
		cin >> a;
		cout << "Введите координату x2 = ";
		cin >> b;
		vector<Vector2x1> PointStorage;
		Vector2x1 x0 = Vector2x1(a, b);

		Vector2x1 result;
		minimizationF2(x0, H0, epsilon, 0, 0, result);

		cout << "x1 = " << result.x1 << " ; x2 = " << result.x2 << endl;
	}
	else {
		vector<Vector2x1> storage;
		while (storage.size() < 4) {
			vector<Vector2x1> PointStorage;

			Vector2x1 x0 = Vector2x1(RandomFloat(-10, 10), RandomFloat(-10, 10));

			Vector2x1 result;
			minimizationF2(x0, H0, epsilon, 0, 0, result);

			if (result.x1 != 0 && result.x2 != 0) {

				bool found = false;
				for (auto& elem : storage) {
					if (sqrt((elem.x1 - result.x1)*(elem.x1 - result.x1) + (elem.x2 - result.x2)*(elem.x2 - result.x2)) <= 1) {
						found = true;
					}
				}

				if (!found) {
					storage.push_back(result);
					cout << "x1 = " << result.x1 << " ; x2 = " << result.x2 << endl;
				}
			}
		}
	}

	cout << endl;

	/*
	cout << "Функция f1: " << endl;

	Vector2x1 x0 = Vector2x1(2, 2);
	Matrix2x2 H0 = Matrix2x2();
	minimizationF1(x0, H0, epsilon, 0);

	
	
	cout << "Функция f2: " << endl;

	x0 = Vector2x1(3, 1.8);
	minimizationF2(x0, H0, epsilon, 0);

	x0 = Vector2x1(-3.7, -3.2);
	minimizationF2(x0, H0, epsilon, 0);

	x0 = Vector2x1(-2.7, 3);
	minimizationF2(x0, H0, epsilon, 0);

	x0 = Vector2x1(3.5, -2);
	minimizationF2(x0, H0, epsilon, 0);*/

	return EXIT_SUCCESS;
}