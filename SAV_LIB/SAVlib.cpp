#include "pch.h"
#include "stdafx.h"
#include <iostream>
#include <conio.h>
#include <string>
#include <strstream>
#include <ctime>

extern "C"
{

	unsigned int __stdcall power(unsigned int a, unsigned int b)
	{
		return pow(a, b);
	}

	unsigned int __stdcall random(unsigned int a)
	{
		if (a == 0)
			return 0;

		srand(time(NULL));

		int k = 0 + rand() % a;

		if (k < 0) k = k * (-1);

		return k;
	}

	unsigned int __stdcall strLen(char* str)
	{
		int len = 0;

		for (int i = 0; str[i] != '\0'; i++)
			len++;

		return len;
	}

	char* __stdcall concat(char* str0, char* str1) {
		std::strstream stream;

		stream << str0 << str1 << '\0';

		return stream.str();
	}

	char* __stdcall inttostr(unsigned int a) {
		std::strstream stream;

		stream << a << '\0';

		return stream.str();
	}

	char* __stdcall copy(char* a) {
		std::strstream stream;

		stream << a << '\0';

		return stream.str();
	}

	char* __stdcall getDateNow()
	{
		char dateNow[100];
		std::strstream stream;

		tm local;
		time_t currentTime;

		currentTime = time(NULL);
		localtime_s(&local, &currentTime);
		strftime(dateNow, 100, "%d.%m.%Y", &local);

		stream << dateNow << '\0';

		return stream.str();
	}

	char* __stdcall getTimeNow()
	{
		char timeNow[100];
		std::strstream stream;

		tm local;
		time_t currentTime;

		currentTime = time(NULL);
		localtime_s(&local, &currentTime);
		strftime(timeNow, 100, "%H:%M:%S", &local);

		stream << timeNow << '\0';

		return stream.str();
	}

	unsigned int __stdcall inputInt() {
		unsigned int result;

		std::cin >> result;

		return result;
	}
	char* __stdcall inputString() {
		char* buf = (char*)calloc(1024, 1);
		char* str;
		int len;

		std::cin >> buf;

		len = strlen(buf);

		str = (char*)calloc(len + 1, 1);

		for (short i = 0; i < len; i++)
			str[i] = buf[i];
		
		str[len] = '\0';

		return str;
	}
	char* __stdcall inputChar() {
		char symbol = _getch();
		char* result = (char*)calloc(2, 1);

		std::cout << symbol << std::endl;

		result[0] = symbol;
		result[1] = '\0';

		return result;
	}

	int __stdcall outnum(unsigned int value)
	{
		std::cout << value;

		return 0;
	}
	int __stdcall outstr(char* ptr)
	{
		if (ptr == nullptr)
			std::cout << std::endl;

		for (int i = 0; ptr[i] != '\0'; i++)
			std::cout << ptr[i];

		return 0;
	}

	int __stdcall outnumline(unsigned int value)
	{
		std::cout << value << std::endl;

		return 0;
	}
	unsigned int __stdcall outstrline(char* ptr)
	{
		if (ptr == nullptr)
			std::cout << std::endl;

		for (int i = 0; ptr[i] != '\0'; i++)
			std::cout << ptr[i];

		std::cout << std::endl;

		return 0;
	}

	int __stdcall system_pause()
	{
		system("pause");
		return 0;
	}
}