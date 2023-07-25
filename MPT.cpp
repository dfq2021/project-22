#pragma once
#include<iostream>
#include<string>

using namespace std;

struct MPT
{
	int prefix;
	string shared_nibbles;
	MPT* next_node[16];
	int value = 0;
	bool leaf = 0;
};
/*
[Keys,Values]
a711355 45
a77d337 1
a7f9365 2
a77d397 5
*/

MPT* Easy_MPT = new MPT();

void Setup()
{
	MPT* tmp = NULL;
	Easy_MPT->prefix = 0;
	Easy_MPT->shared_nibbles = "a7";
	Easy_MPT->next_node[1] = new MPT();
	Easy_MPT->next_node[1]->prefix = 2;
	Easy_MPT->next_node[1]->leaf = 1;
	Easy_MPT->next_node[1]->shared_nibbles = "1355";
	Easy_MPT->next_node[1]->value = 45;
	Easy_MPT->next_node[15] = new MPT();
	Easy_MPT->next_node[15]->prefix = 2;
	Easy_MPT->next_node[15]->leaf = 1;
	Easy_MPT->next_node[15]->shared_nibbles = "9365";
	Easy_MPT->next_node[15]->value = 2;
	Easy_MPT->next_node[7] = new MPT();
	tmp = Easy_MPT->next_node[7];
	tmp->prefix = 0;
	tmp->shared_nibbles = "d3";
	tmp->next_node[3] = new MPT();
	tmp->next_node[3]->prefix = 3;
	tmp->next_node[3]->leaf = 1;
	tmp->next_node[3]->shared_nibbles = "7";
	tmp->next_node[3]->value = 1;
	tmp->next_node[9] = new MPT();
	tmp->next_node[9]->prefix = 3;
	tmp->next_node[9]->leaf = 1;
	tmp->next_node[9]->shared_nibbles = "7";
	tmp->next_node[9]->value = 5;

	delete tmp;
}

int to_int(const char* c)
{
	if ((c[0] >= 'A') && (c[0] <= 'Z'))
	{
		return c[0] - 'A' + 10;
	}
	else if ((c[0] >= 'a') && (c[0] <= 'z'))
	{
		return c[0] - 'a' + 10;
	}
	else if ((c[0] >= '0') && (c[0] <= '9'))
	{
		return c[0] - '0';
	}
}

bool Node_Verify(string m)
{
	int mlength = m.length();
	MPT* tmp;
	string str = "";
	tmp = Easy_MPT;
	while (tmp->leaf == 0 && mlength > 0)
	{
		str = str + tmp->shared_nibbles;
		string a;
		if (m.substr(0, str.length()) == str)
		{
			string p = m.substr(str.length(), 1);
			str += p;
			tmp = tmp->next_node[to_int(p.c_str())];
		}
		else
		{
			return false;
		}
	}
	mlength -= str.length();
	if (mlength != tmp->shared_nibbles.length())
		return false;
	str = str + tmp->shared_nibbles;
	string a;
	if (m.substr(0, str.length()) == str)
	{
		return true;
	}
	else
	{
		return false;
	}


}
int main()
{
	Setup();
	cout << "a7f9365" << endl;
	if (Node_Verify("a7f9365"))
	{   

		cout << "验证成功存在" << endl;
		
	}
	else cout << "验证失败" << endl;
}