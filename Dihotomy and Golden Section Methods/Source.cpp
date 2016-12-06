#include <iostream>
#include <math.h>
#include <Windows.h>
using namespace std;

//Значение f(x) в точке x
double f(double x) {
	return 62.0 / exp(x) + 8.5*x;
}

//Метод дихотомии
void dihotomy(double leftBorder, double rightBorder, double epsilon) {
	double nextLeftBorder, nextRightBorder;
	double delta = epsilon;

	double x1 = (leftBorder + rightBorder - delta) / 2;
	double x2 = (leftBorder + rightBorder + delta) / 2;
	double f_x1 = f(x1);
	double f_x2 = f(x2);

	//Выбираем новые границы в зависимости от значений f_x1 и f_x2
	if (f_x1 <= f_x2) {
		nextLeftBorder = leftBorder;
		nextRightBorder = x2;
	}
	else {
		nextLeftBorder = x1;
		nextRightBorder = rightBorder;
	}

	double nextEpsilon = (nextRightBorder - nextLeftBorder) / 2;
	if (nextEpsilon <= epsilon) {
		//Остановка процесса и вывод результата на экран
		double result_x = (nextLeftBorder + nextRightBorder) / 2;
		double result_f_x = f(result_x);
		cout << endl;
		cout << "Метод дихотомии:" << endl;
		cout << "x = " << result_x << endl << endl;
		return;
	}
	else {
		//Следующий шаг
		dihotomy(nextLeftBorder, nextRightBorder, epsilon);
	}
}

//Метод золотого сечения
//flag = 0, если это первая итерация метода
//flag = 1, если в прошлой итерации f_x1 <= f_x2, в переменную saved_x запоминаем значение x1
//flag = 2, если в прошлой итерации f_x1 > f_x2, в переменную saved_x передаем значение x2
void golden_section(double leftBorder, double rightBorder, double epsilon, double saved_x, int flag) {
	double nextLeftBorder, nextRightBorder, x1, x2, next_saved_x;
	int next_flag;

	if (flag == 0) {
		x1 = leftBorder + (3 - sqrt(5)) / 2 * (rightBorder - leftBorder);
		x2 = leftBorder + (sqrt(5) - 1) / 2 * (rightBorder - leftBorder);
	}
	else if (flag == 1) {
		x1 = leftBorder + (3 - sqrt(5)) / 2 * (rightBorder - leftBorder);
		x2 = saved_x;
	}
	else if (flag == 2) {
		x1 = saved_x;
		x2 = leftBorder + (sqrt(5) - 1) / 2 * (rightBorder - leftBorder);
	}
	
	double f_x1 = f(x1);
	double f_x2 = f(x2);

	//Выбираем новые границы в зависимости от значений f_x1 и f_x2 (также запоминаем x1 или x2 для след. итерации)
	if (f_x1 <= f_x2) {
		nextLeftBorder = leftBorder;
		nextRightBorder = x2;
		next_saved_x = x1;
		next_flag = 1;
	}
	else {
		nextLeftBorder = x1;
		nextRightBorder = rightBorder;
		next_saved_x = x2;
		next_flag = 2;
	}

	double nextEpsilon = (nextRightBorder - nextLeftBorder) / 2;
	if (nextEpsilon <= epsilon) {
		//Остановка процесса и вывод результата на экран
		double result_x = (nextLeftBorder + nextRightBorder) / 2;
		double result_f_x = f(result_x);
		cout << endl;
		cout << "Метод золотого сечения:" << endl;
		cout << "x = " << result_x << endl << endl;
		return;
	}
	else {
		//Следующий шаг
		golden_section(nextLeftBorder, nextRightBorder, epsilon, next_saved_x, next_flag);
	}
}

int main() {
	setlocale(LC_ALL, "Russian");
	bool correct_one = false, correct_two = false;
	double eps_dih, eps_gold;

	//Просьба ввести пользователя погрешности
	while (!correct_one) {
		double eps;
		cout << "Введите погрешность для метода дихотомии: ";
		cin >> eps;
		if (eps > 0) {
			eps_dih = eps;
			correct_one = true;
		}
		else {
			system("color 04");
			cout << "Неправильный ввод" << endl;
			system("color 07");
			
		}
	}
	

	//границы a=0, b=5 (исходя из примерного графика функции)
	dihotomy(0, 5, eps_dih);

	while (!correct_two) {
		double eps;
		cout << "Введите погрешность для метода золотого сечения: ";
		cin >> eps;
		if (eps > 0) {
			eps_gold = eps;
			correct_two = true;
		}
		else {
			system("color 04");
			cout << "Неправильный ввод" << endl;
			system("color 07");
		}
	}

	golden_section(0, 5, eps_gold, 0, 0);
	return EXIT_SUCCESS;
}