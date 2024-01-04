// 내가 해시함수 구현한거랑 hash_map이랑 비교해보기
#include "TimeSystem.h"

#include <fstream>
#include <iostream>
#include <set>
#include <vector>
#include <unordered_map>

#pragma warning(disable : 4996)

using namespace std;

#define MAX_SIZE 195000

template<typename T1, typename T2>
unsigned int Hash(T1& Key, T2 len)
{
	unsigned int hash = 0;
	for (size_t i = 0; i < len; ++i)
	{
		hash = 65599 * hash + Key[i];
	}
	return hash ^ (hash >> 16);
}

void LoadWordsFromFile(set<string>& cotainer)
{
	fstream file;
	string word, t, q, filename;
	filename = "../english3.txt";
	file.open(filename.c_str());
	while (file >> word)
	{
		cotainer.insert(word);
	}
}

set<string> g_SetAll;
vector<string> Chaining[MAX_SIZE];

int main()
{
	TimeSystem timeSystem1;
	TimeSystem timeSystem2;

	LoadWordsFromFile(g_SetAll);

	timeSystem1.Start();
	for (set<string>::iterator it = g_SetAll.begin(); it != g_SetAll.end(); it++)
	{
		string word = *it;
		int index = Hash(*it, word.size()) % MAX_SIZE;
		Chaining[index].push_back(word);
	}
	timeSystem1.End();
	cout << "내 해시함수: ";
	double myHashElapsedTime1 = timeSystem1.GetElaspedTime();

	unordered_map<string, int> um;
	timeSystem2.Start();
	for (set<string>::iterator it = g_SetAll.begin(); it != g_SetAll.end(); it++)
	{
		string word = *it;
		um[word]++;
		//um.insert(make_pair(*it, 0));
	}
	timeSystem2.End();
	cout << "unordered_map: ";
	double myHashElapsedTime2 = timeSystem2.GetElaspedTime();
}
