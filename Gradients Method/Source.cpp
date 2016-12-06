#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>
using namespace std;

//����� ��� ������ � ���������
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
	friend Vector2x1 operator + (Vector2x1 vec1, Vector2x1 vec2) //�������� ��������
	{
		Vector2x1 tempVec = Vector2x1(vec1.x1 + vec2.x1, vec1.x2 + vec2.x2);
		return tempVec;
	}
	friend Vector2x1 operator - (Vector2x1 vec1, Vector2x1 vec2) //��������� ��������
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
	friend Vector2x1 operator * (Vector2x1 vec, double a) //��������� ������� �� ������
	{
		Vector2x1 tempVec = Vector2x1(vec.x1*a, vec.x2*a);
		return tempVec;
	}
	double norm() {
		return sqrt(x1*x1 + x2*x2);
	}
};

//������� f1
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

//������� f2
class f2 {
	public:
		double operator() (Vector2x1 v) {
			return (v.x1*v.x1 + v.x2 - 11)*(v.x1*v.x1 + v.x2 - 11) + (v.x1 + v.x2*v.x2 - 7)*(v.x1 + v.x2*v.x2 - 7);
		}
		Vector2x1 gradient(Vector2x1 v) {
			double x1 = 2*(2*v.x1*(v.x1*v.x1 + v.x2 - 11) + v.x1 + v.x2*v.x2 - 7);
			double x2 = 2*(v.x1*v.x1 + 2*v.x2*(v.x1 + v.x2*v.x2 - 7) + v.x2 - 11);
			Vector2x1 result = Vector2x1(x1, x2);
			return result;
		}
};

//����������� ����� ��� ������� F1
void gradientMinimizationF1(Vector2x1 x_k, double epsilon, double delta, double lambda, double alpha_k, int count, Vector2x1& res) {
	
	// alpha - ��������� ���
	// lambda - ��������, �� ������� �������� ���
	// epsilon - ����������� ��� ������� ��������� ����
	// delta - ����������� ��� �������� ��������
	
	//count - ������� �������� (�� ����������� ��� ��������)
	//res - ����������, ���� �������� ���������

	f1 function;

	count++;
	if (count > 1000) { //���� ���������� �������� ��������� 1000, �� ��������� �������� (������ ��������� ������), (���� �� ���������������, �� ��������� ��������)
		res = Vector2x1(0, 0);
		return;
	}

	double alpha = alpha_k; //���

	bool correct = false;
	while (!correct) { //���� ��������, ���� �� ���������� ������� �� 98 ������
		Vector2x1 gradient = function.gradient(x_k);
		double norm = gradient.norm(); //����� ������� ���������

		Vector2x1 x = x_k - gradient*alpha; //����� ����� x_k_1
		double f_x = function(x); //�������� � ���� �����

		if (function(x) - function(x_k) <= -alpha*epsilon*norm*norm) { 
			correct = true;
			alpha_k = alpha; // ��������� ��� ��� ����
		}
		else { // ������ ���
			alpha /= lambda;
		}
	}
	
	Vector2x1 x_k_1 = x_k - function.gradient(x_k)*alpha_k;
	Vector2x1 grad = function.gradient(x_k_1);
	if (grad.norm() <= delta) { // �������� ���������
		res = x_k_1;
		return;
	}
	else {
		gradientMinimizationF1(x_k_1, epsilon, delta, lambda, alpha_k, count, res); //��������� ��������
	}

}

void gradientMinimizationF2(Vector2x1 x_k, double epsilon, double delta, double lambda, double alpha_k, int count, Vector2x1& res) {

	// alpha - ��������� ���
	// lambda - ��������, �� ������� �������� ���
	// epsilon - ����������� ��� ������� ��������� ����
	// delta - ����������� ��� �������� ��������

	//count - ������� �������� (�� ����������� ��� ��������)
	//res - ����������, ���� �������� ���������

	f2 function;

	count++;
	if (count > 1000) {
		res = Vector2x1(0, 0);
		return;
	}

	double alpha = alpha_k;
	bool correct = false;
	while (!correct) {
		Vector2x1 gradient = function.gradient(x_k);
		double norm = gradient.norm();

		Vector2x1 x = x_k - gradient*alpha;
		double f_x = function(x);

		if (function(x) - function(x_k) <= -alpha*epsilon*norm*norm) { // ��������� ��� ��� ����
			correct = true;
			alpha_k = alpha;
		}
		else { // ������ ���
			alpha /= lambda;
		}
	}

	Vector2x1 x_k_1 = x_k - function.gradient(x_k)*alpha_k;
	Vector2x1 grad = function.gradient(x_k_1);
	if (grad.norm() <= delta) { // �������� ��������
		res = x_k_1;
		return;
	}
	else {
		gradientMinimizationF2(x_k_1, epsilon, delta, lambda, alpha_k, count, res);
	}

}

//��������� ������������ ����� � ��������� �� a �� b
float RandomFloat(float a, float b) {
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}

int main() {

	setlocale(LC_ALL, "Russian");

	double lambda = 1.2, alpha = 1.0, epsilon = 0.1, delta;
	// alpha - ��������� ���
	// lambda - ��������, �� ������� �������� ���
	// epsilon - ����������� ��� ������� ��������� ����
	// delta - ����������� ��� �������� ��������

	bool correct = false;

	//������� ������ ������������ �����������
	while (!correct) {
		double eps;
		cout << "������� �����������: ";
		cin >> eps;
		if (eps > 0) {
			delta = eps;
			correct = true;
		}
		else {
			cout << "������������ ����" << endl;
		}
	}

	cout << endl;
	cout << "������� f1" << endl;

	char str;
	cout << "�� ������ ������ ��������� ������ �������, ��� ������������� �� ��������? (y/n): ";
	cin >> str;

	//������ ���� ���� ��������� ��������� ��������� ������, ���� ������� ������������ ������ ��

	if (str == 'y') {
		float a, b;
		cout << "������� ���������� x1 = ";
		cin >> a;
		cout << "������� ���������� x2 = ";
		cin >> b;
		vector<Vector2x1> PointStorage;
		Vector2x1 x0 = Vector2x1(a, b);
		
		Vector2x1 result;  
		gradientMinimizationF1(x0, epsilon, delta, lambda, alpha, 0, result);
		cout << "x1 = " << result.x1 << " ; x2 = " << result.x2 << endl;
	}
	else {
		vector<Vector2x1> storage;
		while (storage.size() < 1) {
			vector<Vector2x1> PointStorage;

			Vector2x1 x0 = Vector2x1(RandomFloat(-10, 10), RandomFloat(-10, 10));

			Vector2x1 result;
			gradientMinimizationF1(x0, epsilon, delta, lambda, alpha, 0, result);

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
	cout << "������� f2" << endl;
	cout << "�� ������ ������ ��������� ������ �������, ��� ������������� �� ��������? (y/n): ";
	cin >> str;

	if (str == 'y') {
		float a, b;
		cout << "������� ���������� x1 = ";
		cin >> a;
		cout << "������� ���������� x2 = ";
		cin >> b;
		vector<Vector2x1> PointStorage;
		Vector2x1 x0 = Vector2x1(a, b);

		Vector2x1 result;
		gradientMinimizationF2(x0, epsilon, delta, lambda, alpha, 0, result);

		cout << "x1 = " << result.x1 << " ; x2 = " << result.x2 << endl;
	}
	else {
		vector<Vector2x1> storage;
		while (storage.size() < 4) {
			vector<Vector2x1> PointStorage;

			Vector2x1 x0 = Vector2x1(RandomFloat(-10, 10), RandomFloat(-10, 10));
			
			Vector2x1 result;
			gradientMinimizationF2(x0, epsilon, delta, lambda, alpha, 0, result);

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