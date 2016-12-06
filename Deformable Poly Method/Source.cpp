#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>
using namespace std;

class Vector2x1 {
	public:	
		
		double x1, x2;
	
		Vector2x1(double a, double b)
		{
			x1 = a;
			x2 = b;
		}
		Vector2x1()
		{
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
		Vector2x1 operator += (const Vector2x1& vec)
		{
			x1 += vec.x1;
			x2 += vec.x2;
			return *this;
		}
		friend Vector2x1 operator * (Vector2x1 vec, double a) //умножение вектора на скаляр
		{
			Vector2x1 tempVec = Vector2x1(vec.x1*a, vec.x2*a);
			return tempVec;
		}
};

double f(Vector2x1 vec, bool flag) //flag=0, если используем f_1(x) и flag=1, если используем f_2(x)
{	
	if (!flag) {
		return 100 * (vec.x2 - vec.x1*vec.x1) * (vec.x2 - vec.x1*vec.x1) + 5 * (1 - vec.x1) * (1 - vec.x1);
	}
	else {
		return (vec.x1*vec.x1 + vec.x2 - 11)*(vec.x1*vec.x1 + vec.x2 - 11) + (vec.x1 + vec.x2*vec.x2 - 7)*(vec.x1 + vec.x2*vec.x2 - 7);
	}
	
}

//Проверка условия остановки
bool check(vector<Vector2x1> PointStorage, Vector2x1 x_c, double epsilon, bool flag)
{
	double g = sqrt(0.25 * ((f(PointStorage[0], flag) - f(x_c, flag))*(f(PointStorage[0], flag) - f(x_c, flag)) + (f(PointStorage[1], flag) - f(x_c, flag))*(f(PointStorage[1], flag) - f(x_c, flag)) + (f(PointStorage[2], flag) - f(x_c, flag))* (f(PointStorage[2], flag) - f(x_c, flag))));
	if (g <= epsilon) {
		return true;
	}
	return false;
}

Vector2x1 minimization(vector<Vector2x1> PointStorage, double epsilon, bool flag)
{	
	bool trigger;
	double alpha = 1.0; 
	double gamma = 2.0;
	double beta = 0.5;
	Vector2x1 x_l, x_s, x_h, x_g, x_g1, x_r, x_c;
	Vector2x1 tempPoint;
	
	double values[3];
	double f_l, f_s, f_h, f_g, f_g1, f_r;
	double tempValue;
	
	values[0] = f(PointStorage[0], flag);
	values[1] = f(PointStorage[1], flag);
	values[2] = f(PointStorage[2], flag);

	int count = 0;
	while (!check(PointStorage, x_c, epsilon, flag))	// Проверка на условие выхода
	{	
		count++;
		if (count > 1000) {
			return Vector2x1(-999, -999);
		}

		sort(PointStorage.begin(), PointStorage.end(), [&](Vector2x1 a, Vector2x1 b) {
			return f(a, flag) < f(b, flag);
		});
		x_l = PointStorage[0]; 
		f_l = values[0];
		x_s = PointStorage[1];	
		f_s = values[1];
		x_h = PointStorage[2];
		f_h = values[2];

		//Вычисление центра тяжести
		x_c = (x_l + x_s)*0.5;
		
		//Отражение точки x_h относительно точки x_c
		x_g = x_c + (x_c - x_h)*alpha;
		f_g = f(x_g, flag);

		if (f_g <= f_l)
		{	
			x_g1 = x_c + (x_g - x_c)*gamma;
			f_g1 = f(x_g1, flag);
			if (f_g1 < f_l) {
				PointStorage[2] = x_g1;
				values[2] = f_g1;
			}
			else {
				PointStorage[2] = x_g;
				values[2] = f_g;
			}
		}

		if (f_l < f_g && f_g <= f_s)
		{
			PointStorage[2] = x_g;
			values[2] = f_g;
		}

		trigger = false;
		if (f_s < f_g && f_g <= f_h)
		{
			tempValue = f_h;
			tempPoint = x_h;
			PointStorage[2] = x_g;
			values[2] = f_g;
			x_g = tempPoint; // (поменяли местами x_h и x_g)
			f_g = tempValue; 
			trigger = true;
		}
		
		if (f_g > f_h)	{
			trigger = true;
		}
			
		if (trigger) //сжатие
		{
			x_r = x_c + (x_h - x_c)*beta;
			f_r = f(x_r, flag);
			if (f_r < f_h)
			{
				tempValue = f_h;
				tempPoint = x_h;
				PointStorage[2] = x_r;
				values[2] = f_r;
				x_r = tempPoint;
				f_r = tempValue;
			}
			else //редукция
			{	
				PointStorage[1] = x_l + (PointStorage[1] - x_l) * 0.5;
				PointStorage[2] = x_l + (PointStorage[2] - x_l) * 0.5;
			}
		}
	}
	
	return x_l;
}

//Рандомное вещ. в интервале от a до b
float RandomFloat(float a, float b) {
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}


int main()
{	
	bool correct = false;
	bool exit = false;
	double epsilon;
	setlocale(LC_ALL, "Russian");

	//Просьба ввести пользователя погрешность
	while (!correct) {
		double eps;
		cout << "Введите погрешность: ";
		cin >> eps;
		if (eps > 0) {
			epsilon = eps;
			correct = true;
		}
		else {
			cout << "Неправильный ввод" << endl;
		}
	}

	int k = 10;
	float r = k*(sqrt(3) + 1) / (sqrt(2) * 2);
	float s = k*(sqrt(3) - 1) / (sqrt(2) * 2);

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
		Vector2x1 x1 = Vector2x1(x0.x1 + r, x0.x2 + s);
		Vector2x1 x2 = Vector2x1(x0.x1 + s, x0.x2 + r);
		PointStorage.push_back(x0);
		PointStorage.push_back(x1);
		PointStorage.push_back(x2);

		Vector2x1 result = minimization(PointStorage, epsilon, 0);
		cout << "x1 = " << result.x1 << " ; x2 = " << result.x2 << endl;
	}
	else {
		vector<Vector2x1> storage;
		while (storage.size() < 1) {
			vector<Vector2x1> PointStorage;

			Vector2x1 x0 = Vector2x1(RandomFloat(-10, 10), RandomFloat(-10, 10));
			Vector2x1 x1 = Vector2x1(x0.x1 + r, x0.x2 + s);
			Vector2x1 x2 = Vector2x1(x0.x1 + s, x0.x2 + r);
			PointStorage.push_back(x0);
			PointStorage.push_back(x1);
			PointStorage.push_back(x2);


			Vector2x1 result = minimization(PointStorage, epsilon, 0);

			if (result.x1 != -999 && result.x2 != -999) {

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
		Vector2x1 x1 = Vector2x1(x0.x1 + r, x0.x2 + s);
		Vector2x1 x2 = Vector2x1(x0.x1 + s, x0.x2 + r);
		PointStorage.push_back(x0);
		PointStorage.push_back(x1);
		PointStorage.push_back(x2);

		Vector2x1 result = minimization(PointStorage, epsilon, 1);
		cout << "x1 = " << result.x1 << " ; x2 = " << result.x2 << endl;
	}
	else {
		vector<Vector2x1> storage;
		while (storage.size() < 4) {
			vector<Vector2x1> PointStorage;

			Vector2x1 x0 = Vector2x1(RandomFloat(-10, 10), RandomFloat(-10, 10));
			Vector2x1 x1 = Vector2x1(x0.x1 + r, x0.x2 + s);
			Vector2x1 x2 = Vector2x1(x0.x1 + s, x0.x2 + r);
			PointStorage.push_back(x0);
			PointStorage.push_back(x1);
			PointStorage.push_back(x2);


			Vector2x1 result = minimization(PointStorage, epsilon, 1);

			if (result.x1 != -999 && result.x2 != -999) {

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