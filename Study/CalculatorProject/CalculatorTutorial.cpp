#include <iostream>
#include "Calculator.h"

using namespace std;

int main()
{
	double x = 0.0;
	double y = 0.0;
	double result = 0.0;
	char oper = '+';

	cout << "Calculator Console Application" << endl << endl;
	cout << "Please enter the operation to perform. Format: a+b | a-b | a*b | a/b"
		<< endl;

	Calculator c;
	while (true)
	{
		cin >> x >> oper >> y;
		if (oper == '/' && y == 0)
		{
			cout << "Division by 0 exception" << endl;
			continue;
		}
		else
		{
			result = c.Calculate(x, oper, y);
		}
		cout << "Result is: " << result << endl;
	}

	return 0;
}





































/*cout.put('S');
	cout.put('t').put('u');
	cout.put(100);
	cout.put(121.3);
	cout.put('\n');

	const char* str = "Apple";
	int len = strlen(str);
	cout.write(str, len);
	cout << endl;

	char str[10];
	cout << "엔터가 눌리기 전까지 문자열 입력 -> ";
	cin.getline(str, 10);
	cout << "입력받은 문자열 -> " << str << endl;

	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	cout.tie(NULL);*/

// 진법 변경 -> 변경 후 지속됨 주의
	//int num = 100;
	//cout << "8진수:" << oct << num << endl;
	//cout << "16진수:" << hex << num << endl;
	////cout << "10진수:" << dec << num << endl;

	//cout << num << endl;

	//cout.setf(ios::left);
	//cout.setf(ios::right);
	//cout.unsetf(ios::left);
	// 
//int a = 10000;
//cout << setw(8) << a << endl;
//cout << setw(10) << a << endl;
//cout << right << setw(10) << a << "," << endl;
//cout << left << setw(10) << a << "," << endl;

//// 부동 소수점의 유효자리 설정
//double b = 3.123456;
//cout << setiosflags(ios::fixed) << endl;
//cout << setprecision(2) << b << endl;
//cout << setw(10) << setprecision(3) << b << endl;
//cout << setw(10) << setprecision(2) << setfill('0') << b << endl;

// 자료형
	//int n = 0;
	//double d = 3.14;
	//float f = 3.14f;
	//char c = 'a';
	//bool b = false;

	//cout << sizeof(n) << endl; // -2^32 ~ 0 ~ 2^31-1
	//cout << sizeof(d) << endl;
	//cout << sizeof(f) << endl;
	//cout << sizeof(c) << endl;
	//cout << sizeof(b) << endl;

	//cout << setw(8) << n << ":" << endl;
	//cout << setw(5) << 1 << setw(5) << 3 << setw(5) << 5 << endl;


/*
	int n1 = 1;
	int n2 = 2;
	scanf_s("%d %d", &n1, &n2);

	printf("%d + %d = %d\n", n1, n2, n1 + n2);
	printf("%d - %d = %d\n", n1, n2, n1 - n2);
	printf("%d * %d = %d\n", n1, n2, n1 * n2);
	printf("%d / %d = %d\n", n1, n2, n1 / n2);*/

	//cout << "Hello World" << "\n";
	//cout << "hi" << "\n";
	//cout << 'A' << "\n";	// char
	//cout << 365 << "\n";	// int
	//cout << 3.14 << "\n";	// double
	//cout << true << "\n";	// bool

	//int n1, n2;
	//cin >> n1 >> n2;

	//cout << n1 << "," << n2 << endl;