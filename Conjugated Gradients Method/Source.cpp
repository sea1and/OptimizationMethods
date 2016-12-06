#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>
using namespace std;

//Класс для работы с векторами
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

//Метод дихотомии для функции f1
void dihotomyForF1(double leftBorder, double rightBorder, double epsilon, Vector2x1 x_k, Vector2x1 d_k, double& output) {
	double nextLeftBorder, nextRightBorder;
	double delta = epsilon;

	f1 f;

	double a1 = (leftBorder + rightBorder - delta) / 2;
	double a2 = (leftBorder + rightBorder + delta) / 2;
	double f_a1 = f(x_k + d_k*a1);
	double f_a2 = f(x_k + d_k*a2);

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
		double result_f_a = f(x_k + d_k*result_a);
		output = result_a;
		return;
	} else {
		//Следующий шаг
		dihotomyForF1(nextLeftBorder, nextRightBorder, epsilon, x_k, d_k, output);
	}
}

//Метод сопряженных градиентов для функции f1
void minimizationF1(Vector2x1 x_k, Vector2x1 d_k, double epsilon, int k, int count, Vector2x1& res) {
	f1 f;

	count++; 
	if (count > 1000) { //если количество итераций превысило 1000, то метод расходится и надо остановиться
		res = Vector2x1(0, 0);
		return;
	}

	Vector2x1 x_k_1;
	Vector2x1 d_k_1;
	
	double alpha_k;
	dihotomyForF1(0, 100, 0.0000000001, x_k, d_k, alpha_k); //одномерная минимизация для нахождения alpha_k (длина шага в методе наискорейшего спуска)

	if ((k+1) % 2 == 0) { //если k+1 кратно 2 (k - номер текущей итерации)
		x_k_1 = x_k + d_k*alpha_k; //прошлая координата плюс антиградиент умноженный на шаг
		d_k_1 = f.gradient(x_k_1)*(-1);
	}
	else { //если k+1 не кратно 2
		x_k_1 = x_k + d_k*alpha_k;
		Vector2x1 grad_x_k_1 = f.gradient(x_k_1);
		Vector2x1 grad_x_k = f.gradient(x_k);
		double beta_k = (grad_x_k_1.norm()*grad_x_k_1.norm()) / (grad_x_k.norm()*grad_x_k.norm()); //формула из Аббасова
		d_k_1 = grad_x_k_1*(-1) + d_k*beta_k;
	}
	if (f.gradient(x_k_1).norm() <= epsilon) { //критерий остановки
		res = x_k_1;
		return;
	}
	else {
		minimizationF1(x_k_1, d_k_1, epsilon, k+1, count, res); //следующая итерация
	}
}

//Метод дихотомии для функции f2
void dihotomyForF2(double leftBorder, double rightBorder, double epsilon, Vector2x1 x_k, Vector2x1 d_k, double& output) {
	double nextLeftBorder, nextRightBorder;
	double delta = epsilon;

	f2 f;

	double a1 = (leftBorder + rightBorder - delta) / 2;
	double a2 = (leftBorder + rightBorder + delta) / 2;
	double f_a1 = f(x_k + d_k*a1);
	double f_a2 = f(x_k + d_k*a2);

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
		double result_f_a = f(x_k + d_k*result_a);
		output = result_a;
		return;
	} else {
		//Следующий шаг
		dihotomyForF2(nextLeftBorder, nextRightBorder, epsilon, x_k, d_k, output);
	}
}

//Метод сопряженных градиентов для функции f2
void minimizationF2(Vector2x1 x_k, Vector2x1 d_k, double epsilon, int k, int count, Vector2x1& res) {
	f2 f;

	Vector2x1 x_k_1;
	Vector2x1 d_k_1;

	double alpha_k;
	dihotomyForF2(0, 100, 0.0000000001, x_k, d_k, alpha_k); //одномерная минимизация для нахождения alpha_k (длина шага в методе наискорейшего спуска)

	count++;
	if (count > 1000) {
		res = Vector2x1(0, 0);
		return;
	}

	if((k + 1) % 2 == 0) { //если k+1 кратно 2 (k - номер текущей итерации)
		x_k_1 = x_k + d_k*alpha_k; //прошлая координата плюс антиградиент умноженный на шаг
		d_k_1 = f.gradient(x_k_1)*(-1);
	} else { //если k+1 не кратно 2 (k - номер текущей итерации)
		x_k_1 = x_k + d_k*alpha_k;
		Vector2x1 grad_x_k_1 = f.gradient(x_k_1);
		Vector2x1 grad_x_k = f.gradient(x_k);
		double beta_k = (grad_x_k_1.norm()*grad_x_k_1.norm()) / (grad_x_k.norm()*grad_x_k.norm()); //формула из Аббасова
		d_k_1 = grad_x_k_1*(-1) + d_k*beta_k;
	}
	if(f.gradient(x_k_1).norm() <= epsilon) { //критерий остановки
		res = x_k_1;
		return;

	} else {
		minimizationF2(x_k_1, d_k_1, epsilon, k + 1, count, res); //следующая итерация
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
		Vector2x1 d0 = f_1.gradient(x0)*(-1);
		

		Vector2x1 result;
		minimizationF1(x0, d0, epsilon, 0, 0, result);
		cout << "x1 = " << result.x1 << " ; x2 = " << result.x2 << endl;
	}
	else {
		vector<Vector2x1> storage;
		while (storage.size() < 1) {
			vector<Vector2x1> PointStorage;

			Vector2x1 x0 = Vector2x1(RandomFloat(-10, 10), RandomFloat(-10, 10));

			Vector2x1 d0 = f_1.gradient(x0)*(-1);

			Vector2x1 result;
			minimizationF1(x0, d0, epsilon, 0, 0, result);

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

		Vector2x1 d0 = f_2.gradient(x0)*(-1);

		Vector2x1 result;
		minimizationF2(x0, d0, epsilon, 0, 0, result);

		cout << "x1 = " << result.x1 << " ; x2 = " << result.x2 << endl;
	}
	else {
		vector<Vector2x1> storage;
		while (storage.size() < 4) {
			vector<Vector2x1> PointStorage;

			Vector2x1 x0 = Vector2x1(RandomFloat(-10, 10), RandomFloat(-10, 10));

			Vector2x1 d0 = f_2.gradient(x0)*(-1);

			Vector2x1 result;
			minimizationF2(x0, d0, epsilon, 0, 0, result);

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
	return EXIT_SUCCESS;
}