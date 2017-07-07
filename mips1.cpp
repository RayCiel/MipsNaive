#include<iostream>
#include<fstream>
#include<cstring>
#include<string>
#include<map>
#include<deque>
#include<stdlib.h>


using namespace std;

class Store
{
	string op;
	int kind;
	int Pointer;
	string GetOp(string line)
	{
		string str = "";
		Pointer = 0;
		while (line[Pointer] == ' ' || line[Pointer] == '\t')
			Pointer++;
		while (line[Pointer] != ' ' && line[Pointer] != ':' && line[Pointer] != '\n' && line[Pointer] != '\t')
			str += line[Pointer++];
		return str;
	}
public:
	string obj1, obj2, obj3;
	int PC = 0;
	Store() : op(""), kind(-1), obj1(""), obj2(""), obj3(""), Pointer(0), PC(0) {}
	Store &operator= (const Store &other)
	{
		op = other.op;
		kind = other.kind;
		obj1 = other.obj1;
		obj2 = other.obj2;
		obj3 = other.obj3;
		Pointer = other.Pointer;
		PC = other.PC;
		return *this;
	}
	Store &operator= (const string &line)
	{
		op = GetOp(line);
		kind = -1;
		if (op[0] == '.')
		{
			obj1.clear();
			Pointer++;
			while (line[Pointer] != '\n' && line[Pointer] != '\0')
				obj1 += line[Pointer++];
		}
		else
		{
			if (line[Pointer] == ':')
				kind = 0;
			else
			{
				Pointer++;
				obj1.clear();
				obj2.clear();
				obj3.clear();
				while (line[Pointer] != ',' && line[Pointer] != '\n' && line[Pointer] != ' ' && line[Pointer] != '\0')
					obj1 += line[Pointer++];
				if (obj1 != "")
				{
					kind = 1;
					if (line[Pointer] == ',')
						Pointer += 2;
					else Pointer++;
					while (line[Pointer] != ',' && line[Pointer] != '\n' && line[Pointer] != ' ' && line[Pointer] != '\0')
						obj2 += line[Pointer++];
					if (obj2 != "" && obj1 != "")
					{
						kind = 2;
						if (line[Pointer] == ',')
							Pointer += 2;
						else Pointer++;
						while (line[Pointer] != ',' && line[Pointer] != '\n' && line[Pointer] != ' ' && line[Pointer] != '\0')
							obj3 += line[Pointer++];
						if (obj3 != "" && obj1 != "" && obj2 != "")
							kind = 3;
					}
				}
			}
		}
		return *this;
	}
	int Kind() const
	{
		return kind;
	}
	string Op() const
	{
		return op;
	}
};

class Label
{
	string label;
	int address;
	int index;

public:
	bool textflag = 1;
	Label() : label(""), address(-1), index(-1), textflag(1) {}
	Label(const Store &other)
	{
		label = other.Op();
	}
	Label &operator= (const Store other)
	{
		label = other.Op();
		return *this;
	}
	string str()
	{
		return label;
	}
	void Address(int x)
	{
		address = x;
	}
	int Add()
	{
		return address;
	}
	void Index(int x)
	{
		index = x;
	}
	int Ind()
	{
		return index;
	}

};

string ChangeInt(long long n)//long long?int
{
	string ret = "";
	bool flag = 0;
	if (n < 0)
	{
		flag = 1;
		n *= -1;
	}
	while (n > 0)
	{
		ret = (char)(n % 10 + '0') + ret;
		n /= 10;
	}
	if (flag)
		ret = '-' + ret;
	return ret;
	//char ret = (char)n;
	//return ret;
}

const int maxN = 5000004;
const int maxM = 2000004;
char tem[104];
string tmp[maxM];
Store storage[maxM];
deque<Store> Deque;
char Data[maxN];
deque<Store> Datadeque;
Label lab[maxM], Datalab[maxM];
int num = 0, nul = 0;
int pos = 0, Datapos = 0, Buffer = 1000004;
long long con[35];
long long lo, hi;
bool flag = 0, returna0 = 0;
map<string, int> Map;
map<string, int> Oper;
int m;
int HazardCon[35];
int DHazard;
int CHazard;
ofstream fout("output.out");
Store Sempty;
struct Preparation
{
	Store st = Sempty;
	string op;
	map<string, int>::iterator iter = Oper.end();
	int opKind;
	int con1 = -1;
	int con2 = -1;
	int con3 = -1;
	int imm1;
	int imm2;
	int imm3;
	int num1;
	int num2;
	int num3;
	int Rsc2;
	int Rsc4;
	int Rsc5;
	int Rsc31;
	int labelAdd1;
	int labelAdd2;
	int labelAdd3;
	int labelIdx = -1;
	int PC;
	int n;
	int SearchCon = -1;
	bool Jump = 0;
	long long low, high;
};

struct Ret
{
	Store st = Sempty;
	string op;
	map<string, int>::iterator iter = Oper.end();
	int opKind;
	int con1 = -1;
	int con2 = -1;
	int con3 = -1;
	int imm1;
	int imm2;
	int imm3;
	int num1;
	int num2;
	int num3;
	int ARsc2;
	int Rsc2;
	int Rsc4;
	int Rsc5;
	int Rsc31;
	int labelAdd1;
	int labelAdd2;
	int labelAdd3;
	int labelIdx = -1;
	int SearchCon = -1;
	int n;
	int PC;
	bool Jump = 0;
	long long low, high;
};
Preparation Pempty;
Ret Rempty;


long long Change(string line)//string?long long
{
	int i = 0;
	long long ret = 0;
	int b = 1;
	if (line[0] == '-')
	{
		b = -1;
		i++;
	}
	while (line[i] >= '0' && line[i] <= '9')
		ret = ret * 10 + line[i++] - '0';
	ret *= b;
	return ret;
	//long long ret = (long long)line;
	//return ret;
}

int SearchLabel(string l)//??label????
{
	for (int j = 0; j < nul; j++)
	{
		if (lab[j].str() == l)
			return lab[j].Add();
	}
	return -1;
}

int SearchLabelIndex(string l)//??label????
{
	for (int j = 0; j < nul; j++)
	{
		if (lab[j].str() == l)
			return lab[j].Ind();
	}
	return -1;
}

int SearchCon(string str)//???????
{
	map<string, int>::iterator iter;
	iter = Map.find(str);
	if (iter != Map.end())
		return iter->second;
	return -1;
}

bool SEmpty(Store obj)
{
	return ((obj.obj1 == Sempty.obj1) && (obj.obj2 == Sempty.obj2) && (obj.obj3 == Sempty.obj3) && (obj.Op() == Sempty.Op()));
}

bool PEmpty(Preparation obj)
{
	return SEmpty(obj.st);
}

bool REmpty(Ret obj)
{
	return SEmpty(obj.st);
}

Store Fetch()
{
	Store sto;
	sto = Deque[pos];
	sto.PC = pos;
	//if (pos != num - 1)
		pos++;
	return sto;
}

Preparation DataPreparation(Store st)
{
	if (SEmpty(st))
		return Pempty;
	Preparation pre;
	pre.st = st;
	pre.op = st.Op();
	pre.opKind = st.Kind();
	pre.iter = Oper.find(pre.op);
	if (pre.iter != Oper.end())
	{
		if (pre.iter->second <= -23 && pre.iter->second >= -39)
			CHazard++;
	}
	pre.con1 = SearchCon(st.obj1);
	pre.con2 = SearchCon(st.obj2);
	pre.con3 = SearchCon(st.obj3);
	pre.imm1 = Change(st.obj1);
	pre.imm2 = Change(st.obj2);
	pre.imm3 = Change(st.obj3);
	if (pre.con1 != -1)
		pre.num1 = con[pre.con1];
	if (pre.con2 != -1)
		pre.num2 = con[pre.con2];
	if (pre.con3 != -1)
		pre.num3 = con[pre.con3];
	pre.Rsc2 = con[2];
	pre.Rsc4 = con[4];
	pre.Rsc5 = con[5];
	pre.Rsc31 = con[31];
	pre.low = lo;
	pre.high = hi;
	pre.labelAdd1 = SearchLabel(st.obj1);
	pre.labelAdd2 = SearchLabel(st.obj2);
	pre.labelAdd3 = SearchLabel(st.obj3);
	pre.labelIdx = SearchLabelIndex(st.Op());
	pre.PC = st.PC;
	if (pre.iter != Oper.end())
	{
		switch (pre.iter->second)
		{
		case -1: case -2: case -3: case -4: case -5: case -10: case -11: case -12: case -13: case -14: case -15: case -16: case -17: case -18: case -19: case -20: case -21: case -22: case -40: case -41: case -42: case -43: case -47: case -48: case -49:
		{
			HazardCon[pre.con1]++;
			break;
		}
		case -6: case -7: case -8: case -9:
		{
			if (pre.opKind == 3)
			{
				HazardCon[pre.con1]++;
			}
			break;
		}
		}
	}
	if (pre.iter != Oper.end())
	{
		if (pre.iter->second >= -39 && pre.iter->second <= -38)
			HazardCon[31]++;
		if (pre.iter->second >= -9 && pre.iter->second <= -6)
		{
			if (pre.opKind == 2)
				HazardCon[32]++, HazardCon[33]++;
		}
		if (pre.iter->second == -51)
		{
			if (pre.Rsc2 == 5 || pre.Rsc2 == 9)
				HazardCon[2]++;
		}
		if (pre.iter->second >= -46 && pre.iter->second <= -40)
		{
			if (pre.labelAdd2 == -1)
			{
				int j = 0;
				string s = "";
				while (pre.st.obj2[j] != '(')
					s += pre.st.obj2[j++];
				pre.n = Change(s);
				if (s == "") pre.n = 0;
				s.clear();
				j++;
				while (pre.st.obj2[j] != ')')
					s += pre.st.obj2[j++];
				pre.SearchCon = SearchCon(s);
			}
		}
	}
	
	return pre;
}

Preparation FalDataPreparation(Store st)
{
	if (SEmpty(st))
		return Pempty;
	Preparation pre;
	pre.st = st;
	pre.op = st.Op();
	pre.opKind = st.Kind();
	pre.iter = Oper.find(pre.op);
	pre.con1 = SearchCon(st.obj1);
	pre.con2 = SearchCon(st.obj2);
	pre.con3 = SearchCon(st.obj3);
	pre.imm1 = Change(st.obj1);
	pre.imm2 = Change(st.obj2);
	pre.imm3 = Change(st.obj3);
	if (pre.con1 != -1)
		pre.num1 = con[pre.con1];
	if (pre.con2 != -1)
		pre.num2 = con[pre.con2];
	if (pre.con3 != -1)
		pre.num3 = con[pre.con3];
	pre.Rsc2 = con[2];
	pre.Rsc4 = con[4];
	pre.Rsc5 = con[5];
	pre.Rsc31 = con[31];
	pre.low = lo;
	pre.high = hi;
	pre.labelAdd1 = SearchLabel(st.obj1);
	pre.labelAdd2 = SearchLabel(st.obj2);
	pre.labelAdd3 = SearchLabel(st.obj3);
	pre.labelIdx = SearchLabelIndex(st.Op());
	pre.PC = st.PC;
	//if (pre.con1 != -1)
	//	HazardCon[pre.con1]++;
	if (pre.iter != Oper.end())
	{
		if (pre.iter->second >= -46 && pre.iter->second <= -40)
		{
			if (pre.labelAdd2 == -1)
			{
				int j = 0;
				string s = "";
				while (pre.st.obj2[j] != '(')
					s += pre.st.obj2[j++];
				pre.n = Change(s);
				if (s == "") pre.n = 0;
				s.clear();
				j++;
				while (pre.st.obj2[j] != ')')
					s += pre.st.obj2[j++];
				pre.SearchCon = SearchCon(s);
			}
		}
	}

	return pre;
}

Ret Evaluate(Preparation pre)
{
	
	Ret ans;
	ans.st = pre.st;
	ans.op = pre.op;
	ans.opKind = pre.opKind;
	ans.iter = pre.iter;
	ans.con1 = pre.con1;
	ans.con2 = pre.con2;
	ans.con3 = pre.con3;
	ans.imm1 = pre.imm1;
	ans.imm2 = pre.imm2;
	ans.imm3 = pre.imm3;
	ans.num1 = pre.num1;
	ans.num2 = pre.num2;
	ans.num3 = pre.num3;
	ans.Rsc2 = pre.Rsc2;
	ans.Rsc4 = pre.Rsc4;
	ans.Rsc5 = pre.Rsc5;
	ans.Rsc31 = pre.Rsc31;
	ans.labelAdd1 = pre.labelAdd1;
	ans.labelAdd2 = pre.labelAdd2;
	ans.labelAdd3 = pre.labelAdd3;
	ans.labelIdx = pre.labelIdx;
	ans.low = pre.low;
	ans.high = pre.high;
	ans.n = pre.n;
	ans.SearchCon = pre.SearchCon;
	ans.PC = pre.PC;
	ans.Jump = pre.Jump;
	return ans;
}

Ret Eva(Ret pre)
{
	Ret ans;
	ans.st = pre.st;
	ans.op = pre.op;
	ans.opKind = pre.opKind;
	ans.iter = pre.iter;
	ans.con1 = pre.con1;
	ans.con2 = pre.con2;
	ans.con3 = pre.con3;
	ans.imm1 = pre.imm1;
	ans.imm2 = pre.imm2;
	ans.imm3 = pre.imm3;
	ans.num1 = pre.num1;
	ans.num2 = pre.num2;
	ans.num3 = pre.num3;
	ans.ARsc2 = pre.ARsc2;
	ans.Rsc2 = pre.Rsc2;
	ans.Rsc4 = pre.Rsc4;
	ans.Rsc5 = pre.Rsc5;
	ans.Rsc31 = pre.Rsc31;
	ans.labelAdd1 = pre.labelAdd1;
	ans.labelAdd2 = pre.labelAdd2;
	ans.labelAdd3 = pre.labelAdd3;
	ans.labelIdx = pre.labelIdx;
	ans.low = pre.low;
	ans.high = pre.high;
	ans.n = pre.n;
	ans.PC = pre.PC;
	ans.Rsc2 = pre.Rsc2;
	ans.Rsc4 = pre.Rsc4;
	ans.Rsc31 = pre.Rsc31;
	ans.SearchCon = pre.SearchCon;
	ans.Jump = pre.Jump;
	return ans;
}

Ret Execution(Preparation pre)
{
	if (PEmpty(pre))
		return Rempty;
	Ret ans;
	ans = Evaluate(pre);
	if (pre.op[0] != '.')
	{
		if (pre.iter != Oper.end())
		{
			switch (pre.iter->second)
			{
			case -1: case -2:
			{
				if (pre.con3 != -1)
					ans.num1 = pre.num2 + pre.num3;
				else
					ans.num1 = pre.num2 + pre.imm3;
				//cout << m << ' ' << st.Op() << ' ' << con[a] << ' ' << con[b] << ' ' << st.obj3 << '\n';
				break;
			}
			case -3:
				ans.num1 = pre.num2 + pre.imm3; break;
			case -4: case -5:
			{
				if (pre.con3 != -1)
					ans.num1 = pre.num2 - pre.num3;
				else
					ans.num1 = ans.num2 - pre.imm3;
				//cout << m << ' ' << st.Op() << ' ' << con[a] << ' ' << con[b] << ' ' << st.obj3 << '\n';
				
				break;
			}
			case -6:
			{
				if (pre.opKind == 3)
				{
					if (pre.con3 != -1)
						ans.num1 = pre.num2 * pre.num3;
					else
						ans.num1 = pre.num2 * pre.imm3;
					//cout << m << ' ' << st.Op() << ' ' << con[a] << ' ' << con[b] << ' ' << st.obj3 << '\n';
				}
				else if (pre.opKind == 2)
				{
					if (pre.con2 != -1)
					{
						ans.low = (pre.num1 * pre.num2) % (1 << 32);
						ans.high = (pre.num1 * pre.num2) / (1 << 32);
					}
					else
					{
						ans.low = (pre.num1 * pre.imm2) % (1 << 32);
					    ans.high = (pre.num1 * pre.imm2) / (1 << 32);
					}
					//cout << m << ' ' << st.Op() << ' ' << con[a] << ' ' << st.obj2 << ' ' << hi << ' ' << lo << '\n';
				}
				
				break;
			}
			case -7:
			{
				if (pre.opKind == 3)
				{
					if (pre.con3 != -1)
						ans.num1 = (unsigned)(pre.num2 * pre.num3);
					else
						ans.num1 = (unsigned)(pre.num2 * pre.imm3);
					//cout << m << ' ' << st.Op() << ' ' << con[a] << ' ' << con[b] << ' ' << st.obj3 << '\n';
				}
				else if (pre.opKind == 2)
				{
					if (pre.con2 != -1)
					{
						ans.low = (unsigned)((pre.num1 * pre.num2) % (1 << 32));
						ans.high = (unsigned)((pre.num1 * pre.num2) / (1 << 32));
					}
					else
					{
						ans.low = (unsigned)((pre.num1 * pre.imm2) % (1 << 32));
						ans.high = (unsigned)((pre.num1 * pre.imm2) / (1 << 32));
					}
					//cout << m << ' ' << st.Op() << ' ' << con[a] << ' ' << st.obj2 << ' ' << hi << ' ' << lo << '\n';
				}
				
				break;
			}
			case -8:
			{
				if (pre.opKind == 3)
				{
					if (pre.con3 != -1)
						ans.num1 = pre.num2 / pre.num3;
					else
						ans.num1 = pre.num2 / pre.imm3;
					//cout << m << ' ' << st.Op() << ' ' << con[a] << ' ' << con[b] << ' ' << st.obj3 << '\n';
				}
				else if (pre.opKind == 2)
				{
					if (pre.con2 != -1)
					{
						ans.low = pre.num1 / pre.num2;
						ans.high = pre.num1 % pre.num2;
					}
					else
					{
						ans.low = pre.num1 / pre.imm2;
						ans.high = pre.num1 % pre.imm2;
					}
					//cout << m << ' ' << st.Op() << ' ' << con[a] << ' '  << st.obj2 << ' ' << hi << ' ' << lo << '\n';
				}
				
				break;
			}
			case -9:
			{
				if (pre.opKind == 3)
				{
					if (pre.con3 != -1)
						ans.num1 = (unsigned)(pre.num2 / pre.num3);
					else
						ans.num1 = (unsigned)(pre.num2 / pre.imm3);
					//cout << m << ' ' << st.Op() << ' ' << con[a] << ' ' << con[b] << ' ' << st.obj3 << '\n';
				}
				else if (pre.opKind == 2)
				{
					if (pre.con2 != -1)
					{
						ans.low = (unsigned)(pre.num1 / pre.num2);
						ans.high = (unsigned)(pre.num1 % pre.num2);
					}
					else
					{
						ans.low = (unsigned)(pre.num1 / pre.imm2);
						ans.high = (unsigned)(pre.num1 % pre.imm2);
					}
					//cout << m << ' ' << st.Op() << ' ' << con[a] << ' '  << st.obj2 << ' ' << hi << ' ' << lo << '\n';
				}
				
				break;
			}
			case -10: case -11:
			{
				if (pre.con3 != -1)
					ans.num1 = pre.num2 ^ pre.num3;
				else
					ans.num1 = pre.num2 ^ pre.imm3;
				//cout << m << ' ' << st.Op() << ' ' << con[a] << ' ' << con[b] << ' ' << st.obj3 << '\n';
				
				break;
			}
			case -12: case -13:
				ans.num1 = -1 * pre.num2; break;
			case -14: case -15:
			{
				if (pre.con3 != -1)
					ans.num1 = pre.num2 % pre.num3;
				else
					ans.num1 = pre.num2 % pre.imm3;
				//cout << m << ' ' << st.Op() << ' ' << con[a] << ' ' << con[b] << ' ' << st.obj3 << '\n';
				
				break;
			}
			case -16:
				ans.num1 = pre.imm2; break;
			case -17:
			{
				if (pre.con3 != -1)
					ans.num1 = (pre.num2 == pre.num3);
				else
					ans.num1 = (pre.num2 == pre.imm3);
				//cout << m << ' ' << st.Op() << ' ' << con[a] << ' ' << con[b] << ' ' << st.obj3 << '\n';
				
				break;
			}
			case -18:
			{
				if (pre.con3 != -1)
					ans.num1 = (pre.num2 >= pre.num3);
				else
					ans.num1 = (pre.num2 >= pre.imm3);
				//cout << m << ' ' << st.Op() << ' ' << con[a] << ' ' << con[b] << ' ' << st.obj3 << '\n';
				
				break;
			}
			case -19:
			{
				if (pre.con3 != -1)
					ans.num1 = (pre.num2 > pre.num3);
				else
					ans.num1 = (pre.num2 > pre.imm3);
				//cout << m << ' ' << st.Op() << ' ' << con[a] << ' ' << con[b] << ' ' << st.obj3 << '\n';
				
				break;
			}
			case -20:
			{
				if (pre.con3 != -1)
					ans.num1 = (pre.num2 <= pre.num3);
				else
					ans.num1 = (pre.num2 <= pre.imm3);
				//cout << m << ' ' << st.Op() << ' ' << con[a] << ' ' << con[b] << ' ' << st.obj3 << '\n';
				
				break;
			}
			case -21:
			{
				if (pre.con3 != -1)
					ans.num1 = (pre.num2 < pre.num3);
				else
					ans.num1 = (pre.num2 < pre.imm3);
				//cout << m << ' ' << st.Op() << ' ' << con[a] << ' ' << con[b] << ' ' << st.obj3 << '\n';
				
				break;
			}
			case -22:
			{
				if (pre.con3 != -1)
					ans.num1 = (pre.num2 != pre.num3);
				else
					ans.num1 = (pre.num2 != pre.imm3);
				//cout << m << ' ' << st.Op() << ' ' << con[a] << ' ' << con[b] << ' ' << st.obj3 << '\n';
				
				break;
			}
			case -23:
				ans.PC = pre.labelAdd1;  ans.Jump = 1; break;
			case -24:
			{
				int d = pre.labelAdd3;
				if (pre.con2 != -1)
				{
					if (pre.num1 == pre.num2)
					{
						ans.PC = d;
						ans.Jump = 1;
					}
				}
				else
				{
					if (pre.num1 == pre.imm2)
					{
						ans.Jump = 1;
						ans.PC = d;
					}
				}
				//cout << m << ' ' << st.Op() << ' ' << con[a] << ' ' << st.obj2 << ' ' << pos << ' ' << d << '\n';
				break;
			}
			case -25:
			{
				int d = pre.labelAdd3;
				if (pre.con2 != -1)
				{
					if (pre.num1!= pre.num2)
						ans.PC = d, ans.Jump = 1;
				}
				else
				{
					if (pre.num1 != pre.imm2)
						ans.PC = d, ans.Jump = 1;
				}
				//cout << m << ' ' << st.Op() << ' ' << con[a] << ' ' << st.obj2 << ' ' << pos << ' ' << d << '\n';
				//pos++;
				break;
			}
			case -26:
			{
				int d = pre.labelAdd3;
				if (pre.con2 != -1)
				{
					if (pre.num1 >= pre.num2)
						ans.PC = d, ans.Jump = 1;
				}
				else
				{
					if (pre.num1 >= pre.imm2)
						ans.PC = d, ans.Jump = 1;
				}
				//cout << m << ' ' << st.Op() << ' ' << con[a] << ' ' << st.obj2 << ' ' << pos << ' ' << d << '\n';
				//pos++;
				break;
			}
			case -27:
			{
				int d = pre.labelAdd3;
				if (pre.con2 != -1)
				{
					if (pre.num1 <= pre.num2)
						ans.PC = d, ans.Jump = 1;
				}
				else
				{
					if (pre.num1 <= pre.imm2)
						ans.PC = d, ans.Jump = 1;
				}
				//cout << m << ' ' << st.Op() << ' ' << con[a] << ' ' << st.obj2 << ' ' << pos << ' ' << d << '\n';
				//pos++;
				break;
			}
			case -28:
			{
				int d = pre.labelAdd3;
				if (pre.con2 != -1)
				{
					if (pre.num1 > pre.num2)
						ans.PC = d, ans.Jump = 1;
				}
				else
				{
					if (pre.num1 > pre.imm2)
						ans.PC = d, ans.Jump = 1;
				}
				//cout << m << ' ' << st.Op() << ' ' << con[a] << ' ' << st.obj2 << ' ' << pos << ' ' << d << '\n';
				//pos++;
				break;
			}
			case -29:
			{
				int d = pre.labelAdd3;
				if (pre.con2 != -1)
				{
					if (pre.num1 < pre.num2)
						ans.PC = d, ans.Jump = 1;
				}
				else
				{
					if (pre.num1 < pre.imm2)
						ans.PC = d, ans.Jump = 1;
				}
				//cout << m << ' ' << st.Op() << ' ' << con[a] << ' ' << st.obj2 << ' ' << pos << ' ' << d << '\n';
				//pos++;
				break;
			}
			case -30:
			{
				int d = pre.labelAdd2;
				if (pre.num1 == 0)
					ans.PC = d, ans.Jump = 1;
				//cout << m << ' ' << st.Op() << ' ' << con[a] << ' ' << pos << ' ' << d << '\n';
				break;
			}
			case -31:
			{
				int d = pre.labelAdd2;
				if (pre.num1 != 0)
					ans.PC = d, ans.Jump = 1;
				//cout << m << ' ' << st.Op() << ' ' << con[a] << ' ' << pos << ' ' << d << '\n';
				break;
			}
			case -32:
			{
				int d = pre.labelAdd2;
				if (pre.num1 <= 0)
					ans.PC = d, ans.Jump = 1;
				//cout << m << ' ' << st.Op() << ' ' << con[a] << ' ' << pos << ' ' << d << '\n';
				break;
			}
			case -33:
			{
				int d = pre.labelAdd2;
				if (pre.num1 >= 0)
					ans.PC = d, ans.Jump = 1;
				//cout << m << ' ' << st.Op() << ' ' << con[a] << ' ' << pos << ' ' << d << '\n';
				break;
			}
			case -34:
			{
				int d = pre.labelAdd2;
				if (pre.num1 > 0)
					ans.PC = d, ans.Jump = 1;
				//cout << m << ' ' << st.Op() << ' ' << con[a] << ' ' << pos << ' ' << d << '\n';
				break;
			}
			case -35:
			{
				int d = pre.labelAdd2;
				if (pre.num1 < 0)
					ans.PC = d, ans.Jump = 1;
				//cout << m << ' ' << st.Op() << ' ' << con[a] << ' ' << pos << ' ' << d << '\n';
				break;
			}
			case -36:
				ans.PC = pre.labelAdd1;  ans.Jump = 1; break;
			case -37:
				ans.PC = pre.num1;  ans.Jump = 1; break;
			case -38:
			{
				ans.Rsc31 = pre.PC + 1;
				ans.PC = pre.labelAdd1;
				ans.Jump = 1;
				break;
			}
			case -39:
			{
				ans.Rsc31 = pre.PC + 1;
				ans.PC = pre.num1;
				ans.Jump = 1;
				break;
			}
			case -47:
				ans.num1 = pre.num2; break;
			case -48:
				ans.num1 = pre.high; break;
			case -49:
				ans.num1 = pre.low; break;
				//case -50:
			case -51:
			{
				switch (pre.Rsc2)
				{
				case 1: //cout << "**1 " << m << ' ' << con[4]; pos++; break;
				{
					cout << pre.Rsc4;
					
					//cout << m << ' ' << st.Op() << ' ' << con[2] << ' ' << con[4] << '\n';
					break;
				}
				case 4:
				{
					int k = pre.Rsc4;
					while (Data[k] != '\0')
					{
						cout << Data[k++];
					}
					//cout << Data[con[4]];
					
					//cout << m << ' ' << st.Op() << ' ' << con[2] << ' ' << con[4] << ' ' << Data[con[4]] << '\n';

					break;
				}
				case 5:
				{
					int n;
					cin >> n;
					ans.ARsc2 = n;
					
					//cout << m << ' ' << st.Op() << " 5 " << con[2] << ' ' << n << ' ' << '\n';
					break;
				}
				case 8:
				{
					string s = "";
					for (int k = 0; k < pre.Rsc5 - 1; k++)
					{
						char c = getchar();
						if (k == 0 && c == '\n')
							k++;
						else if (c && c != '\n')
							s += c;
						else
						{
							break;
						}
					}
					for (int k = 0; k < s.size(); k++)
					{
						new (Data + pre.Rsc4 + k) char(s[k]);
					}
					Datapos += s.size();
					
					//cout << m << ' ' << st.Op() << ' ' << con[2] << ' ' << con[4] << ' ' << Data[con[4]] << ' ' << s << '\n';
					break;
				}
				case 9:
				{
					ans.ARsc2 = Datapos;
					Datapos += pre.Rsc4;
					
					//cout << m << ' ' << st.Op() << " 9 " << con[2] << ' ' << con[4] << ' ' << Datapos << '\n';
					break;
				}
				case 10:
					flag = 1; break;
				case 17:
					flag = 1; returna0 = 1; break;
				}
			}
			}
		}
		else
		{
			int k = pre.labelIdx;
			if (k != -1)
			{
				if (lab[k].textflag == 0)
				{
					lab[k].Address(Datapos);
				}
			}
			
		}
	}
	else
	{
		switch (pre.iter->second)
		{
		case -52:
		{
			int n = pre.imm1;
			int k = 0;
			while (Datapos >= k * (1 << n))
				k++;
			Datapos = k * (1 << n);
			
			break;
		}
		case -53:
		{
			string s;
			s.clear();
			int k = 0;

			while (k < pre.st.obj1.size())
			{
				if (pre.st.obj1[k] != '"')
				{
					if (pre.st.obj1[k] == '\\' && pre.st.obj1[k + 1] == 'n')
					{
						s += '\n';
						k += 2;
					}
					else s += pre.st.obj1[k++];
				}
				else k++;
			}
			for (int k = 0; k < s.size(); k++)
			{
				new (Data + Datapos) char(s[k]);
				Datapos++;
			}
			
			//cout << m << ' ' << st.Op() << ' ' << Datapos << ' ' << Data[Datapos] << ' ' << s << '\n';
			break;
		}
		case -54:
		{
			string s = "";
			int k = 0;
			while (k < pre.st.obj1.size())
			{
				if (pre.st.obj1[k] != '"')
				{
					if (pre.st.obj1[k] == '\\' && pre.st.obj1[k + 1] == 'n')
					{
						s += '\n';
						k += 2;
					}
					else s += pre.st.obj1[k++];
				}
				else k++;
			}
			for (int k = 0; k < s.size(); k++)
			{
				new (Data + Datapos) char(s[k]);
				Datapos++;
			}
			
			//cout << m << ' ' << st.Op() << ' ' << Datapos << ' ' << Data[Datapos] << ' ' << s << '\n';
			break;
		}
		case -55:
		{
			int k = 0;
			string s = "";
			while (pre.st.obj1[k])
			{
				while (pre.st.obj1[k] != ',' && pre.st.obj1[k] != ' ' && pre.st.obj1[k] != '\n' && pre.st.obj1[k] != '\0' && pre.st.obj1[k] != '\t')
				{
					if (pre.st.obj1[k] != '"')
					{
						if (pre.st.obj1[k] == '\\' && pre.st.obj1[k + 1] == 'n')
						{
							s += '\n';
							k += 2;
						}
						else s += pre.st.obj1[k++];
					}
					else k++;
				}
				if (s != "")
				{
					new (Data + Datapos) char(s[0]);
					s.clear();
					Datapos++;
					//cout << m << ' ' << st.Op() << ' ' << Datapos << ' ' << Data[Datapos] << ' ' << s << '\n';
				}
			}
			
			break;
		}
		case -56:
		{
			int k = 0;
			string s = "";
			while (pre.st.obj1[k])
			{
				while (pre.st.obj1[k] != ',' && pre.st.obj1[k] != ' ' && pre.st.obj1[k] != '\n' && pre.st.obj1[k] != '\0' && pre.st.obj1[k] != '\t')
				{
					if (pre.st.obj1[k] != '"')
					{
						if (pre.st.obj1[k] == '\\' && pre.st.obj1[k + 1] == 'n')
						{
							s += '\n';
							k += 2;
						}
						else s += pre.st.obj1[k++];
					}
					else k++;
				}
				if (s != "")
				{
					//char x[10];
					//new (x) string(s);
					int w = Change(s);
					new (Data + Datapos) short(w);
					s.clear();
					Datapos += 2;
					//cout << m << ' ' << st.Op() << ' ' << Datapos << ' ' << Data[Datapos] << ' ' << s << '\n';
				}
			}
			
			break;
		}
		case -57:
		{
			int k = 0;
			string s = "";
			while (pre.st.obj1[k])
			{
				while (pre.st.obj1[k] != ',' && pre.st.obj1[k] != ' ' && pre.st.obj1[k] != '\n' && pre.st.obj1[k] != '\0' && pre.st.obj1[k] != '\t')
				{
					if (pre.st.obj1[k] != '"')
					{
						if (pre.st.obj1[k] == '\\' && pre.st.obj1[k + 1] == 'n')
						{
							s += '\n';
							k += 2;
						}
						else s += pre.st.obj1[k++];
					}
					else k++;
				}
				if (s != "")
				{
					//char x[10];
					//new (x) string(s);
					int w = Change(s);
					new (Data + Datapos) int(w);
					s.clear();
					Datapos += 4;
					//cout << m << ' ' << st.Op() << ' ' << Datapos << ' ' << Data[Datapos] << ' ' << s << '\n';
				}
			}
			
			break;
		}
		case -58:
		{
			Datapos += Change(pre.st.obj1);
			//cout << m << ' ' << st.Op() << ' ' << Datapos << ' ' << st.obj1 << '\n';
			break;
		}
		case -59: case -60:
			break;
		}
	}
	return ans;
}

Ret MemoryAccess(Ret ex)
{
	if (REmpty(ex))
		return Rempty;
	Ret ans;
	ans = Eva(ex);
	if (ex.iter != Oper.end())
	{
		switch (ex.iter->second)
		{
		case -40:
		{
			int d = ex.labelAdd2;
			if (d != -1)
				ans.num1 = d;
			else
				ans.num1 = con[ex.SearchCon] + ans.n;
			break;
		}
		case -41:
		{
			int d = ex.labelAdd2;
			if (d != -1)
			{
				ans.num1 = *((char*)(Data + d));
			}
			else
			{
				ans.num1 = *((char*)(Data + con[ex.SearchCon] + ex.n));
			}
			break;
		}
		case -42:
		{
			int d = ex.labelAdd2;
			if (d != -1)
			{
				ans.num1 = *((short*)(Data + d));
			}
			else
			{
				ans.num1 = *((short*)(Data + con[ex.SearchCon] + ex.n));
			}
			break;
		}
		case -43:
		{
			int d = ex.labelAdd2;
			if (d != -1)
			{
				ans.num1 = *((int*)(Data + d));
			}
			else
			{
				ans.num1 = *((int*)(Data + con[ex.SearchCon] + ex.n));
			}
			break;
		}
		case -44:
		{
			int d = ex.labelAdd2;
			if (d != -1)
			{
				new(Data + d) char(ex.num1);
			}
			else
			{
				new(Data + con[ex.SearchCon] + ex.n) char(ex.num1);
			}
			break;
		}
		case -45:
		{
			int d = ex.labelAdd2;
			if (d != -1)
			{
				new(Data + d) short(ex.num1);
			}
			else
			{
				new(Data + con[ex.SearchCon] + ex.n) short(ex.num1);
			}
			break;
		}
		case -46:
		{
			int d = ex.labelAdd2;
			if (d != -1)
			{
				new(Data + d) int(ex.num1);
			}
			else
			{
				new(Data + con[ex.SearchCon] + ex.n) int(ex.num1);
			}
			break;
		}
		}
	}
	return ans;
}

void WriteBack(Ret pre)
{
	if (REmpty(pre))
		return;
	if (pre.iter != Oper.end())
	{
		switch (pre.iter->second)
		{
		case -1: case -2: case -3: case -4: case -5: case -10: case -11: case -12: case -13: case -14: case -15: case -16: case -17: case -18: case -19: case -20: case -21: case -22: case -40: case -41: case -42: case -43: case -47: case -48: case -49:
		{
			con[pre.con1] = pre.num1;
			HazardCon[pre.con1]--;
			break;
		}
		case -6: case -7: case -8: case -9:
		{
			if (pre.opKind == 3)
			{
				con[pre.con1] = pre.num1;
				HazardCon[pre.con1]--;
			}
			else if (pre.opKind == 2)
			{
				lo = pre.low;
				hi = pre.high;
				HazardCon[32]--;
				HazardCon[33]--;
			}
			break;
		}
		
		case -38: case -39:
		{
			con[31] = pre.Rsc31;
			HazardCon[31]--;
			break;
		}
		
		case -51:
		{
			switch (pre.Rsc2)
			{
			case 5: case 9:
			{
				con[2] = pre.ARsc2;
				HazardCon[2]--;
				break;
			}
			break;
			}
		}
		
		}
		//if (pos == num - 1)
			//pos++;
		if (pre.Jump)
			pos = pre.PC;
		if (pre.iter->second <= -23 && pre.iter->second >= -39)
		{
			CHazard--;
		}
	}
	
}

void Operation(Store st)
{
	string str = st.Op();
	if (str[0] != '.')
	{
		map<string, int>::iterator iter;
		iter = Oper.find(str);
		int a = SearchCon(st.obj1);
		int b = SearchCon(st.obj2);
		int c = SearchCon(st.obj3);
		if (iter != Oper.end())
		{
			switch (iter->second)
			{
			case -1: case -2:
			{
				if (c != -1)
					con[a] = con[b] + con[c];
				else
					con[a] = con[b] + Change(st.obj3);
				//cout << m << ' ' << st.Op() << ' ' << con[a] << ' ' << con[b] << ' ' << st.obj3 << '\n';
				pos++;
				break;
			}
			case -3:
			{
				con[a] = con[b] + Change(st.obj3); pos++; //cout <<st.Op() << con[a] << ' ' << con[b] << ' ' << st.obj3 << '\n'; 
				break;
			}
			case -4: case -5:
			{
				if (c != -1)
					con[a] = con[b] - con[c];
				else
					con[a] = con[b] - Change(st.obj3);
				//cout << m << ' ' << st.Op() << ' ' << con[a] << ' ' << con[b] << ' ' << st.obj3 << '\n';
				pos++;
				break;
			}
			case -6:
			{
				if (st.Kind() == 3)
				{
					if (c != -1)
						con[a] = con[b] * con[c];
					else
						con[a] = con[b] * Change(st.obj3);
					//cout << m << ' ' << st.Op() << ' ' << con[a] << ' ' << con[b] << ' ' << st.obj3 << '\n';
				}
				else if (st.Kind() == 2)
				{
					if (b != -1)
					{
						lo = (con[a] * con[b]) % (1 << 32);
						hi = (con[a] * con[b]) / (1 << 32);
					}
					else
					{
						lo = (con[a] * Change(st.obj2)) % (1 << 32);
						hi = (con[a] * Change(st.obj2)) / (1 << 32);
					}
					//cout << m << ' ' << st.Op() << ' ' << con[a] << ' ' << st.obj2 << ' ' << hi << ' ' << lo << '\n';
				}
				pos++;
				break;
			}
			case -7:
			{
				if (st.Kind() == 3)
				{
					if (c != -1)
						con[a] = (unsigned)(con[b] * con[c]);
					else
						con[a] = (unsigned)(con[b] * Change(st.obj3));
					//cout << m << ' ' << st.Op() << ' ' << con[a] << ' ' << con[b] << ' ' << st.obj3 << '\n';
				}
				else if (st.Kind() == 2)
				{
					if (b != -1)
					{
						lo = (unsigned)((con[a] * con[b]) % (1 << 32));
						hi = (unsigned)((con[a] * con[b]) / (1 << 32));
					}
					else
					{
						lo = (unsigned)((con[a] * Change(st.obj2)) % (1 << 32));
						hi = (unsigned)((con[a] * Change(st.obj2)) / (1 << 32));
					}
					//cout << m << ' ' << st.Op() << ' ' << con[a] << ' ' << st.obj2 << ' ' << hi << ' ' << lo << '\n';
				}
				pos++;
				break;
			}
			case -8:
			{
				if (st.Kind() == 3)
				{
					if (c != -1)
						con[a] = con[b] / con[c];
					else
						con[a] = con[b] / Change(st.obj3);
					//cout << m << ' ' << st.Op() << ' ' << con[a] << ' ' << con[b] << ' ' << st.obj3 << '\n';
				}
				else if (st.Kind() == 2)
				{
					if (b != -1)
					{
						lo = con[a] / con[b];
						hi = con[a] % con[b];
					}
					else
					{
						lo = con[a] / Change(st.obj2);
						hi = con[a] % Change(st.obj2);
					}
					//cout << m << ' ' << st.Op() << ' ' << con[a] << ' '  << st.obj2 << ' ' << hi << ' ' << lo << '\n';
				}
				pos++;
				break;
			}
			case -9:
			{
				if (st.Kind() == 3)
				{
					if (c != -1)
						con[a] = (unsigned)(con[b] / con[c]);
					else
						con[a] = (unsigned)(con[b] / Change(st.obj3));
					//cout << m << ' ' << st.Op() << ' ' << con[a] << ' ' << con[b] << ' ' << st.obj3 << '\n';
				}
				else if (st.Kind() == 2)
				{
					if (b != -1)
					{
						lo = (unsigned)(con[a] / con[b]);
						hi = (unsigned)(con[a] % con[b]);
					}
					else
					{
						lo = (unsigned)(con[a] / Change(st.obj2));
						hi = (unsigned)(con[a] % Change(st.obj2));
					}
					//cout << m << ' ' << st.Op() << ' ' << con[a] << ' '  << st.obj2 << ' ' << hi << ' ' << lo << '\n';
				}
				pos++;
				break;
			}
			case -10: case -11:
			{
				if (c != -1)
					con[a] = con[b] ^ con[c];
				else
					con[a] = con[b] ^ Change(st.obj3);
				//cout << m << ' ' << st.Op() << ' ' << con[a] << ' ' << con[b] << ' ' << st.obj3 << '\n';
				pos++;
				break;
			}
			case -12: case -13:
				con[a] = -1 * con[b]; pos++; //cout << m << ' ' << st.Op() << ' ' << con[a] << ' ' << con[b] << '\n'; 
				break;
			case -14: case -15:
			{
				if (c != -1)
					con[a] = con[b] % con[c];
				else
					con[a] = con[b] % Change(st.obj3);
				//cout << m << ' ' << st.Op() << ' ' << con[a] << ' ' << con[b] << ' ' << st.obj3 << '\n';
				pos++;
				break;
			}
			case -16:
				con[a] = Change(st.obj2); //cout << m << ' ' << st.Op() << ' ' << con[a] << ' '  << st.obj2 << '\n'; 
				pos++; break;
			case -17:
			{
				if (c != -1)
					con[a] = (con[b] == con[c]);
				else
					con[a] = (con[b] == Change(st.obj3));
				//cout << m << ' ' << st.Op() << ' ' << con[a] << ' ' << con[b] << ' ' << st.obj3 << '\n';
				pos++;
				break;
			}
			case -18:
			{
				if (c != -1)
					con[a] = (con[b] >= con[c]);
				else
					con[a] = (con[b] >= Change(st.obj3));
				//cout << m << ' ' << st.Op() << ' ' << con[a] << ' ' << con[b] << ' ' << st.obj3 << '\n';
				pos++;
				break;
			}
			case -19:
			{
				if (c != -1)
					con[a] = (con[b] > con[c]);
				else
					con[a] = (con[b] > Change(st.obj3));
				//cout << m << ' ' << st.Op() << ' ' << con[a] << ' ' << con[b] << ' ' << st.obj3 << '\n';
				pos++;
				break;
			}
			case -20:
			{
				if (c != -1)
					con[a] = (con[b] <= con[c]);
				else
					con[a] = (con[b] <= Change(st.obj3));
				//cout << m << ' ' << st.Op() << ' ' << con[a] << ' ' << con[b] << ' ' << st.obj3 << '\n';
				pos++;
				break;
			}
			case -21:
			{
				if (c != -1)
					con[a] = (con[b] < con[c]);
				else
					con[a] = (con[b] < Change(st.obj3));
				//cout << m << ' ' << st.Op() << ' ' << con[a] << ' ' << con[b] << ' ' << st.obj3 << '\n';
				pos++;
				break;
			}
			case -22:
			{
				if (c != -1)
					con[a] = (con[b] != con[c]);
				else
					con[a] = (con[b] != Change(st.obj3));
				//cout << m << ' ' << st.Op() << ' ' << con[a] << ' ' << con[b] << ' ' << st.obj3 << '\n';
				pos++;
				break;
			}
			case -23:
				pos = SearchLabel(st.obj1); break;
			case -24:
			{
				int d = SearchLabel(st.obj3);
				if (b != -1)
				{
					if (con[a] == con[b])
						pos = d;
					else pos++;
				}
				else
				{
					if (con[a] == Change(st.obj2))
						pos = d;
					else pos++;
				}
				//cout << m << ' ' << st.Op() << ' ' << con[a] << ' ' << st.obj2 << ' ' << pos << ' ' << d << '\n';
				break;
			}
			case -25:
			{
				int d = SearchLabel(st.obj3);
				if (b != -1)
				{
					if (con[a] != con[b])
						pos = d;
					else pos++;
				}
				else
				{
					if (con[a] != Change(st.obj2))
						pos = d;
					else pos++;
				}
				//cout << m << ' ' << st.Op() << ' ' << con[a] << ' ' << st.obj2 << ' ' << pos << ' ' << d << '\n';
				//pos++;
				break;
			}
			case -26:
			{
				int d = SearchLabel(st.obj3);
				if (b != -1)
				{
					if (con[a] >= con[b])
						pos = d;
					else pos++;
				}
				else
				{
					if (con[a] >= Change(st.obj2))
						pos = d;
					else pos++;
				}
				//cout << m << ' ' << st.Op() << ' ' << con[a] << ' ' << st.obj2 << ' ' << pos << ' ' << d << '\n';
				//pos++;
				break;
			}
			case -27:
			{
				int d = SearchLabel(st.obj3);
				if (b != -1)
				{
					if (con[a] <= con[b])
						pos = d;
					else pos++;
				}
				else
				{
					if (con[a] <= Change(st.obj2))
						pos = d;
					else pos++;
				}
				//cout << m << ' ' << st.Op() << ' ' << con[a] << ' ' << st.obj2 << ' ' << pos << ' ' << d << '\n';
				break;
			}
			case -28:
			{
				int d = SearchLabel(st.obj3);
				if (b != -1)
				{
					if (con[a] > con[b])
						pos = d;
					else pos++;
				}
				else
				{
					if (con[a] > Change(st.obj2))
						pos = d;
					else pos++;
				}
				//cout << m << ' ' << st.Op() << ' ' << con[a] << ' ' << st.obj2 << ' ' << pos << ' ' << d << '\n';
				break;
			}
			case -29:
			{
				int d = SearchLabel(st.obj3);
				if (b != -1)
				{
					if (con[a] < con[b])
						pos = d;
					else pos++;
				}
				else
				{
					if (con[a] < Change(st.obj2))
						pos = d;
					else pos++;
				}
				//cout << m << ' ' << st.Op() << ' ' << con[a] << ' ' << st.obj2 << ' ' << pos << ' ' << d << '\n';
				break;
			}
			case -30:
			{
				int d = SearchLabel(st.obj2);
				if (con[a] == 0)
					pos = d;
				else pos++;
				//cout << m << ' ' << st.Op() << ' ' << con[a] << ' ' << pos << ' ' << d << '\n';
				break;
			}
			case -31:
			{
				int d = SearchLabel(st.obj2);
				if (con[a] != 0)
					pos = d;
				else pos++;
				//	cout << m << ' ' << st.Op() << ' ' << con[a] << ' ' << pos << ' ' << d << '\n';
				break;
			}
			case -32:
			{
				int d = SearchLabel(st.obj2);
				if (con[a] <= 0)
					pos = d;
				else pos++;
				//	cout << m << ' ' << st.Op() << ' ' << con[a] << ' ' << pos << ' ' << d << '\n';
				break;
			}
			case -33:
			{
				int d = SearchLabel(st.obj2);
				if (con[a] >= 0)
					pos = d;
				else pos++;
				//	cout << m << ' ' << st.Op() << ' ' << con[a] << ' ' << pos << ' ' << d << '\n';
				break;
			}
			case -34:
			{
				int d = SearchLabel(st.obj2);
				if (con[a] > 0)
					pos = d;
				else pos++;
				//	cout << m << ' ' << st.Op() << ' ' << con[a] << ' ' << pos << ' ' << d << '\n';
				break;
			}
			case -35:
			{
				int d = SearchLabel(st.obj2);
				if (con[a] < 0)
					pos = d;
				else pos++;
				//	cout << m << ' ' << st.Op() << ' ' << con[a] << ' ' << pos << ' ' << d << '\n';
				break;
			}
			case -36:
				pos = SearchLabel(st.obj1); break;
			case -37:
				pos = con[a]; break;
			case -38:
			{
				con[31] = pos + 1;
				pos = SearchLabel(st.obj1);
				break;
			}
			case -39:
			{
				con[31] = pos + 1;
				pos = con[a];
				break;
			}
			case -40:
			{
				int d = SearchLabel(st.obj2);
				if (d != -1)
				{
					con[a] = d;
					pos++;
					//		cout << m << ' ' << st.Op() << ' ' << con[a] << ' ' << pos << ' ' << d << '\n';
				}
				else
				{
					int j = 0;
					string s = "";
					while (st.obj2[j] != '(')
						s += st.obj2[j++];
					int n = Change(s);
					if (s == "") n = 0;
					s.clear();
					j++;
					while (st.obj2[j] != ')')
						s += st.obj2[j++];
					int e = SearchCon(s);
					con[a] = con[e] + n;
					pos++;
					//		cout << m << ' ' << st.Op() << ' ' << con[a] << ' ' << pos << ' ' << con[d] << ' ' << n << '\n';
				}
				break;
			}
			case -41:
			{
				int d = SearchLabel(st.obj2);
				if (d != -1)
				{
					con[a] = *((char*)(Data + d));
					pos++;
					//	cout << m << ' ' << st.Op() << ' ' << con[a] << ' ' << pos << ' ' << Data[d + 1] << ' ' << d + 1 << '\n';
				}
				else
				{
					int j = 0;
					string s = "";
					while (st.obj2[j] != '(')
						s += st.obj2[j++];
					int n = Change(s);
					if (s == "") n = 0;
					s.clear();
					j++;
					while (st.obj2[j] != ')')
						s += st.obj2[j++];
					int e = SearchCon(s);
					con[a] = *((char*)(Data + con[e] + n));
					pos++;
					//		cout << m << ' ' << st.Op() << ' ' << con[a] << ' ' << pos << ' ' << Data[con[d] + n + 1] << ' ' << con[d] + n + 1 << ' ' << n << '\n';
				}
				break;
			}
			case -42:
			{
				int d = SearchLabel(st.obj2);
				if (d != -1)
				{
					con[a] = *((short*)(Data + d));
					pos++;
					//		cout << m << ' ' << st.Op() << ' ' << con[a] << ' ' << pos << ' ' << Data[d + 2] << ' ' << d + 2 << '\n';
				}
				else
				{
					int j = 0;
					string s = "";
					while (st.obj2[j] != '(')
						s += st.obj2[j++];
					int n = Change(s);
					if (s == "") n = 0;
					s.clear();
					j++;
					while (st.obj2[j] != ')')
						s += st.obj2[j++];
					int e = SearchCon(s);
					con[a] = *((short*)(Data + con[e] + n));
					pos++;
					//		cout << m << ' ' << st.Op() << ' ' << con[a] << ' ' << pos << ' ' << Data[con[d] + n + 2] << ' ' << con[d] + n + 2 << ' ' << n << '\n';
				}
				break;
			}
			case -43:
			{
				int d = SearchLabel(st.obj2);
				if (d != -1)
				{
					con[a] = *((int*)(Data + d));
					pos++;
					//		cout << m << ' ' << st.Op() << ' ' << con[a] << ' ' << pos << ' ' << Data[d + 4] << ' ' << d + 4 << '\n';
				}
				else
				{
					int j = 0;
					string s = "";
					while (st.obj2[j] != '(')
						s += st.obj2[j++];
					int n = Change(s);
					if (s == "") n = 0;
					s.clear();
					j++;
					while (st.obj2[j] != ')')
						s += st.obj2[j++];
					int e = SearchCon(s);
					con[a] = *((int*)(Data + con[e] + n));
					pos++;
					//		cout << m << ' ' << st.Op() << ' ' << con[a] << ' ' << pos << ' ' << Data[con[d] + n + 4] << ' ' << con[d] + n + 4 << ' ' << n << '\n';
				}
				break;
			}
			case -44:
			{
				int d = SearchLabel(st.obj2);
				if (d != -1)
				{
					new(Data + d) char(con[a]);
					//Datapos++;
					pos++;
					//		cout << m << ' ' << st.Op() << ' ' << con[a] << ' ' << pos << ' ' << Data[d + 1] << ' ' << d + 1 << '\n';
				}
				else
				{
					int j = 0;
					string s = "";
					while (st.obj2[j] != '(')
						s += st.obj2[j++];
					int n = Change(s);
					if (s == "") n = 0;
					s.clear();
					j++;
					while (st.obj2[j] != ')')
						s += st.obj2[j++];
					int e = SearchCon(s);
					new(Data + con[e] + n) char(con[a]);
					//Datapos++;
					pos++;
					//		cout << m << ' ' << st.Op() << ' ' << con[a] << ' ' << pos << ' ' << Data[con[d] + n + 1] << ' ' << con[d] + n + 1 << ' ' << n << '\n';
				}
				break;
			}
			case -45:
			{
				int d = SearchLabel(st.obj2);
				if (d != -1)
				{
					new(Data + d) short(con[a]);
					//Datapos += 2;
					pos++;
					//		cout << m << ' ' << st.Op() << ' ' << con[a] << ' ' << pos << ' ' << Data[d + 2] << ' ' << d + 2 << '\n';
				}
				else
				{
					int j = 0;
					string s = "";
					while (st.obj2[j] != '(')
						s += st.obj2[j++];
					int n = Change(s);
					if (s == "") n = 0;
					s.clear();
					j++;
					while (st.obj2[j] != ')')
						s += st.obj2[j++];
					int e = SearchCon(s);
					new(Data + con[e] + n) short(con[a]);
					//Datapos += 2;
					pos++;
					//		cout << m << ' ' << st.Op() << ' ' << con[a] << ' ' << pos << ' ' << Data[con[d] + n + 2] << ' ' << con[d] + n + 2 << ' ' << n << '\n';
				}
				break;
			}
			case -46:
			{
				int d = SearchLabel(st.obj2);
				if (d != -1)
				{
					new(Data + d) int(con[a]);
					//Datapos += 4;
					pos++;
					//		cout << m << ' ' << st.Op() << ' ' << con[a] << ' ' << pos << ' ' << Data[d + 4] << ' ' << d + 4 << '\n';
				}
				else
				{
					int j = 0;
					string s = "";
					while (st.obj2[j] != '(')
						s += st.obj2[j++];
					int n = Change(s);
					if (s == "") n = 0;
					s.clear();
					j++;
					while (st.obj2[j] != ')')
						s += st.obj2[j++];
					int e = SearchCon(s);
					new(Data + con[e] + n) int(con[a]);
					//Datapos += 4;
					pos++;
					//		cout << m << ' ' << st.Op() << ' ' << con[a] << ' ' << pos << ' ' << Data[con[d] + n + 4] << ' ' << con[d] + n + 4 << ' ' << n << '\n';
				}
				break;
			}
			case -47:
				con[a] = con[b]; pos++; break;
			case -48:
				con[a] = hi; pos++; break;
			case -49:
				con[a] = lo; pos++; break;
				//case -50:
			case -51:
			{
				switch (con[2])
				{
				case 1: //cout << "**1 " << m << ' ' << con[4]; pos++; break;
				{
					cout << con[4];
					pos++;
					//cout << "**" << m << ' ' << st.Op() << ' ' << con[2] << ' ' << con[4] << "**" << '\n';
					system("pause");
					break;
				}
				case 4:
				{
					int k = con[4];
					while (Data[k] != '\0')
					{
						cout << Data[k++];
					}
					//cout << Data[con[4]];
					pos++;
					//cout << "**" << m << ' ' << st.Op() << ' ' << con[2] << ' ' << con[4] << ' ' << Data[con[4]] << "**" << '\n';
					system("pause");
					break;
				}
				case 5:
				{
					int n;
					cin >> n;
					con[2] = n;
					pos++;
					//		cout << m << ' ' << st.Op() << " 5 " << con[2] << ' ' << n << ' ' << '\n';
					break;
				}
				case 8:
				{
					string s = "";
					for (int k = 0; k < con[5] - 1; k++)
					{
						//char c = getchar();
						char c = cin.get();
						if (k == 0 && c == '\n')
							k++;
						else if (c && c != '\n')
							s += c;
						else
						{
							break;
						}
					}
					for (int k = 0; k < s.size(); k++)
					{
						new (Data + con[4] + k) char(s[k]);
					}
					Datapos += s.size();
					pos++;
					//		cout << m << ' ' << st.Op() << ' ' << con[2] << ' ' << con[4] << ' ' << Data[con[4]] << ' ' << s << '\n';
					break;
				}
				case 9:
				{
					con[2] = Datapos;
					Datapos += con[4];
					pos++;
					//		cout << m << ' ' << st.Op() << " 9 " << con[2] << ' ' << con[4] << ' ' << Datapos << '\n';
					break;
				}
				case 10:
					pos++; flag = 1; break;
				case 17:
					pos++; flag = 1; returna0 = 1; break;
				}
			}
			}
		}
		else
		{
			int k = SearchLabelIndex(st.Op());
			if (k != -1)
			{
				if (lab[k].textflag == 0)
				{
					lab[k].Address(Datapos);
				}
			}
			pos++;
		}
	}
	else
	{
		map<string, int>::iterator iter;
		iter = Oper.find(str);
		int a = SearchCon(st.obj1);
		int b = SearchCon(st.obj2);
		int c = SearchCon(st.obj3);
		switch (iter->second)
		{
		case -52:
		{
			int n = Change(st.obj1);
			int k = 0;
			while (Datapos >= k * (1 << n))
				k++;
			Datapos = k * (1 << n);
			pos++;
			break;
		}
		case -53:
		{
			string s;
			s.clear();
			int k = 0;

			while (k < st.obj1.size())
			{
				if (st.obj1[k] != '"')
				{
					if (st.obj1[k] == '\\' && st.obj1[k + 1] == 'n')
					{
						s += '\n';
						k += 2;
					}
					else s += st.obj1[k++];
				}
				else k++;
			}
			for (int k = 0; k < s.size(); k++)
			{
				new (Data + Datapos) char(s[k]);
				Datapos++;
			}
			pos++;
			//cout << m << ' ' << st.Op() << ' ' << Datapos << ' ' << Data[Datapos] << ' ' << s << '\n';
			break;
		}
		case -54:
		{
			string s = "";
			int k = 0;
			while (k < st.obj1.size())
			{
				if (st.obj1[k] != '"')
				{
					if (st.obj1[k] == '\\' && st.obj1[k + 1] == 'n')
					{
						s += '\n';
						k += 2;
					}
					else s += st.obj1[k++];
				}
				else k++;
			}
			for (int k = 0; k < s.size(); k++)
			{
				new (Data + Datapos) char(s[k]);
				Datapos++;
			}
			pos++;
			//cout << m << ' ' << st.Op() << ' ' << Datapos << ' ' << Data[Datapos] << ' ' << s << '\n';
			break;
		}
		case -55:
		{
			int k = 0;
			string s = "";
			while (st.obj1[k])
			{
				while (st.obj1[k] != ',' && st.obj1[k] != ' ' && st.obj1[k] != '\n' && st.obj1[k] != '\0' && st.obj1[k] != '\t')
				{
					if (st.obj1[k] != '"')
					{
						if (st.obj1[k] == '\\' && st.obj1[k + 1] == 'n')
						{
							s += '\n';
							k += 2;
						}
						else s += st.obj1[k++];
					}
					else k++;
				}
				if (s != "")
				{
					new (Data + Datapos) char(s[0]);
					s.clear();
					Datapos++;
					//cout << m << ' ' << st.Op() << ' ' << Datapos << ' ' << Data[Datapos] << ' ' << s << '\n';
				}
			}
			pos++;
			break;
		}
		case -56:
		{
			int k = 0;
			string s = "";
			while (st.obj1[k])
			{
				while (st.obj1[k] != ',' && st.obj1[k] != ' ' && st.obj1[k] != '\n' && st.obj1[k] != '\0' && st.obj1[k] != '\t')
				{
					if (st.obj1[k] != '"')
					{
						if (st.obj1[k] == '\\' && st.obj1[k + 1] == 'n')
						{
							s += '\n';
							k += 2;
						}
						else s += st.obj1[k++];
					}
					else k++;
				}
				if (s != "")
				{
					//char x[10];
					//new (x) string(s);
					int w = Change(s);
					new (Data + Datapos) short(w);
					s.clear();
					Datapos += 2;
					//cout << m << ' ' << st.Op() << ' ' << Datapos << ' ' << Data[Datapos] << ' ' << s << '\n';
				}
			}
			pos++;
			break;
		}
		case -57:
		{
			int k = 0;
			string s = "";
			while (st.obj1[k])
			{
				while (st.obj1[k] != ',' && st.obj1[k] != ' ' && st.obj1[k] != '\n' && st.obj1[k] != '\0' && st.obj1[k] != '\t')
				{
					if (st.obj1[k] != '"')
					{
						if (st.obj1[k] == '\\' && st.obj1[k + 1] == 'n')
						{
							s += '\n';
							k += 2;
						}
						else s += st.obj1[k++];
					}
					else k++;
				}
				if (s != "")
				{
					//char x[10];
					//new (x) string(s);
					int w = Change(s);
					new (Data + Datapos) int(w);
					s.clear();
					Datapos += 4;
					//cout << m << ' ' << st.Op() << ' ' << Datapos << ' ' << Data[Datapos] << ' ' << s << '\n';
				}
			}
			pos++;
			break;
		}
		case -58:
		{
			Datapos += Change(st.obj1);
			//cout << m << ' ' << st.Op() << ' ' << Datapos << ' ' << st.obj1 << '\n';
			break;
		}
		case -59: case -60:
			pos++; break;
		}
	}
}

bool ControlHazard(Store st)
{
	map<string, int>::iterator iter = Oper.end();
	iter = Oper.find(st.Op());
	if (iter != Oper.end())
	{
		if (iter->second >= -39 && iter->second <= -23)
			return true;
	}
	return false;
}

bool DataHazard(Preparation pre)
{
	if (pre.iter != Oper.end())
	{
		if (pre.iter->second >= -22 && pre.iter->second <= -1)
		{
			if (pre.con2 != -1)
				if (HazardCon[pre.con2] > 0)
					return true;
			if (pre.con3 != -1)
				if (HazardCon[pre.con3] > 0)
					return true;
		}
		if ((pre.iter->second >= -37 && pre.iter->second <= -24) || pre.iter->second == -39 || (pre.iter->second >= -47 && pre.iter->second <= -44))
		{
			if (pre.con1 != -1)
				if (HazardCon[pre.con1] > 0)
					return true;
			if (pre.con2 != -1)
				if (HazardCon[pre.con2] > 0)
					return true;
		}
		if (pre.iter->second >= -46 && pre.iter->second <= -40)
		{
			if (pre.SearchCon != -1)
				if (HazardCon[pre.SearchCon] == 1)
					return true;
		}
		if (pre.iter->second == -48)
		{
			if (HazardCon[32] > 0)
				return true;
		}
		if (pre.iter->second == -49)
		{
			if (HazardCon[33] > 0)
				return true;
		}
		if (pre.iter->second == -51)
		{
			if (HazardCon[2] > 0)
				return true;
			if (pre.Rsc2 == 1 || pre.Rsc2 == 4 || pre.Rsc2 == 8 || pre.Rsc2 == 9 || pre.Rsc2 == 17)
				if (HazardCon[4] > 0)
					return true;
			if (pre.Rsc2 == 8 && HazardCon[5] > 0)
				return true;
		}
	}
	return false;
}

void print(Store st)
{
	//for (int k = 0; k < Datapos; k++)
	//{
	//	fout << k << '\t' << (int)Data[k] << '\n';
	//}
	//fout << "----over----" << '\n';
	for (int k = 0; k < 32; k++)
	{
		fout << "con " << k << " = " << con[k] << '\n';
	}
	fout << "lo " << " = " << lo << '\n';
	fout << "hi " << " = " << hi << '\n';
	fout << "m: " << m << " pos: " << pos << " Datapos: " << Datapos << '\n';
	fout << st.Op() << ' ' << st.obj1 << ' ' << st.obj2 << ' ' << st.obj3 << '\n';
	fout << "----over------" << '\n';
	fout << '\n';
}

//int main()
int main(int argc, char* argv[])
{
	//system("pause");
	//ifstream fin("array_test1-mahaojun.s");
	//ifstream fin("horse-5100309153-yanghuan.s");
	//ifstream fin("pi-5090379042-jiaxiao.s");
	//ifstream fin("basicopt1-5100309127-hetianxing.s");
	ifstream fin(argv[1]);
	//system("pause");
	int i = 0;
	while (fin.getline(tem, 1000))//??
	{
		tmp[i] = tem;
		tmp[i] += '\n';
		//cout << tmp[i];
		i++;
	}
	//system("pause");
	num = i;
	i = 0;
	int p = 0, q = 0, pp = 0, qq = 0;
	Store sto, stoo;
	while (i < num)
	{
		sto = tmp[i];
		Deque.push_back(sto);
		//cout << sto.Kind() << ' ' << sto.Op() << ' ' << sto.obj1 << ' ' << sto.obj2 << ' ' << sto.obj3 << '\n';
		if (sto.Op() == ".data")
		{
			stoo = tmp[++i];
			while (stoo.Op() != ".text")
			{
				if (stoo.Kind() == 0)
				{
					lab[q] = stoo;
					lab[q].Address(i + 1);
					lab[q].Index(q);
					lab[q].textflag = 0;
					q++;
					i++;
					Deque.push_back(stoo);
					//cout << stoo.Kind() << ' ' << stoo.Op() << ' ' << stoo.obj1 << ' ' << stoo.obj2 << ' ' << stoo.obj3 << '\n';
					if (i >= num) break;
					stoo = tmp[i];
				}
				else
				{
					i++;
					Deque.push_back(stoo);
					//cout << stoo.Kind() << ' ' << stoo.Op() << ' ' << stoo.obj1 << ' ' << stoo.obj2 << ' ' << stoo.obj3 << '\n';
					if (i >= num) break;
					stoo = tmp[i];
				}
			}
		}
		else if (sto.Op() == ".text")
		{
			stoo = tmp[++i];
			while (stoo.Op() != ".data")
			{
				if (stoo.Kind() == 0)
				{
					lab[q] = stoo;
					lab[q].Address(i + 1);
					lab[q].Index(q);
					lab[q].textflag = 1;
					q++;
					i++;
					Deque.push_back(stoo);
					//cout << stoo.Kind() << ' ' << stoo.Op() << ' ' << stoo.obj1 << ' ' << stoo.obj2 << ' ' << stoo.obj3 << '\n';
					if (i >= num) break;
					stoo = tmp[i];
				}
				else
				{
					Deque.push_back(stoo);
					//cout << stoo.Kind() << ' ' << stoo.Op() << ' ' << stoo.obj1 << ' ' << stoo.obj2 << ' ' << stoo.obj3 << '\n';
					i++;
					if (i >= num) break;
					stoo = tmp[i];
				}
			}
		}
	}
	//system("pause");

	//system("pause");
	//num = p;
	nul = q;
	for (int j = 0; j < 1; j++)//?????????????
	{
		Map.insert(pair<string, int>("$zero", 0));
		Map.insert(pair<string, int>("$0", 0));
		Map.insert(pair<string, int>("$at", 1));
		Map.insert(pair<string, int>("$1", 1));
		Map.insert(pair<string, int>("$v0", 2));
		Map.insert(pair<string, int>("$2", 2));
		Map.insert(pair<string, int>("$v1", 3));
		Map.insert(pair<string, int>("$3", 3));
		Map.insert(pair<string, int>("$a0", 4));
		Map.insert(pair<string, int>("$4", 4));
		Map.insert(pair<string, int>("$a1", 5));
		Map.insert(pair<string, int>("$5", 5));
		Map.insert(pair<string, int>("$a2", 6));
		Map.insert(pair<string, int>("$6", 6));
		Map.insert(pair<string, int>("$a3", 7));
		Map.insert(pair<string, int>("$7", 7));
		Map.insert(pair<string, int>("$t0", 8));
		Map.insert(pair<string, int>("$8", 8));
		Map.insert(pair<string, int>("$t1", 9));
		Map.insert(pair<string, int>("$9", 9));
		Map.insert(pair<string, int>("$t2", 10));
		Map.insert(pair<string, int>("$10", 10));
		Map.insert(pair<string, int>("$t3", 11));
		Map.insert(pair<string, int>("$11", 11));
		Map.insert(pair<string, int>("$t4", 12));
		Map.insert(pair<string, int>("$12", 12));
		Map.insert(pair<string, int>("$t5", 13));
		Map.insert(pair<string, int>("$13", 13));
		Map.insert(pair<string, int>("$t6", 14));
		Map.insert(pair<string, int>("$14", 14));
		Map.insert(pair<string, int>("$t7", 15));
		Map.insert(pair<string, int>("$15", 15));
		Map.insert(pair<string, int>("$s0", 16));
		Map.insert(pair<string, int>("$16", 16));
		Map.insert(pair<string, int>("$s1", 17));
		Map.insert(pair<string, int>("$17", 17));
		Map.insert(pair<string, int>("$s2", 18));
		Map.insert(pair<string, int>("$18", 18));
		Map.insert(pair<string, int>("$s3", 19));
		Map.insert(pair<string, int>("$19", 19));
		Map.insert(pair<string, int>("$s4", 20));
		Map.insert(pair<string, int>("$20", 20));
		Map.insert(pair<string, int>("$s5", 21));
		Map.insert(pair<string, int>("$21", 21));
		Map.insert(pair<string, int>("$s6", 22));
		Map.insert(pair<string, int>("$22", 22));
		Map.insert(pair<string, int>("$s7", 23));
		Map.insert(pair<string, int>("$23", 23));
		Map.insert(pair<string, int>("$t8", 24));
		Map.insert(pair<string, int>("$24", 24));
		Map.insert(pair<string, int>("$t9", 25));
		Map.insert(pair<string, int>("$25", 25));
		Map.insert(pair<string, int>("$k0", 26));
		Map.insert(pair<string, int>("$26", 26));
		Map.insert(pair<string, int>("$k1", 27));
		Map.insert(pair<string, int>("$27", 27));
		Map.insert(pair<string, int>("$gp", 28));
		Map.insert(pair<string, int>("$28", 28));
		Map.insert(pair<string, int>("$sp", 29));
		Map.insert(pair<string, int>("$29", 29));
		Map.insert(pair<string, int>("$s8", 30));
		Map.insert(pair<string, int>("$fp", 30));
		Map.insert(pair<string, int>("$30", 30));
		Map.insert(pair<string, int>("$ra", 31));
		Map.insert(pair<string, int>("$31", 31));
	}
	for (int j = 0; j < 1; j++)//????
	{
		Oper.insert(pair<string, int>("add", -1));
		Oper.insert(pair<string, int>("addu", -2));
		Oper.insert(pair<string, int>("addiu", -3));
		Oper.insert(pair<string, int>("sub", -4));
		Oper.insert(pair<string, int>("subu", -5));
		Oper.insert(pair<string, int>("mul", -6));
		Oper.insert(pair<string, int>("mulu", -7));
		Oper.insert(pair<string, int>("div", -8));
		Oper.insert(pair<string, int>("divu", -9));
		Oper.insert(pair<string, int>("xor", -10));
		Oper.insert(pair<string, int>("xoru", -11));
		Oper.insert(pair<string, int>("neg", -12));
		Oper.insert(pair<string, int>("negu", -13));
		Oper.insert(pair<string, int>("rem", -14));
		Oper.insert(pair<string, int>("remu", -15));
		Oper.insert(pair<string, int>("li", -16));
		Oper.insert(pair<string, int>("seq", -17));
		Oper.insert(pair<string, int>("sge", -18));
		Oper.insert(pair<string, int>("sgt", -19));
		Oper.insert(pair<string, int>("sle", -20));
		Oper.insert(pair<string, int>("slt", -21));
		Oper.insert(pair<string, int>("sne", -22));
		Oper.insert(pair<string, int>("b", -23));
		Oper.insert(pair<string, int>("beq", -24));
		Oper.insert(pair<string, int>("bne", -25));
		Oper.insert(pair<string, int>("bge", -26));
		Oper.insert(pair<string, int>("ble", -27));
		Oper.insert(pair<string, int>("bgt", -28));
		Oper.insert(pair<string, int>("blt", -29));
		Oper.insert(pair<string, int>("beqz", -30));
		Oper.insert(pair<string, int>("bnez", -31));
		Oper.insert(pair<string, int>("blez", -32));
		Oper.insert(pair<string, int>("bgez", -33));
		Oper.insert(pair<string, int>("bgtz", -34));
		Oper.insert(pair<string, int>("bltz", -35));
		Oper.insert(pair<string, int>("j", -36));
		Oper.insert(pair<string, int>("jr", -37));
		Oper.insert(pair<string, int>("jal", -38));
		Oper.insert(pair<string, int>("jalr", -39));
		Oper.insert(pair<string, int>("la", -40));
		Oper.insert(pair<string, int>("lb", -41));
		Oper.insert(pair<string, int>("lh", -42));
		Oper.insert(pair<string, int>("lw", -43));
		Oper.insert(pair<string, int>("sb", -44));
		Oper.insert(pair<string, int>("sh", -45));
		Oper.insert(pair<string, int>("sw", -46));
		Oper.insert(pair<string, int>("move", -47));
		Oper.insert(pair<string, int>("mfhi", -48));
		Oper.insert(pair<string, int>("mflo", -49));
		Oper.insert(pair<string, int>("nop", -50));
		Oper.insert(pair<string, int>("syscall", -51));
		Oper.insert(pair<string, int>(".align", -52));
		Oper.insert(pair<string, int>(".ascii", -53));
		Oper.insert(pair<string, int>(".asciiz", -54));
		Oper.insert(pair<string, int>(".byte", -55));
		Oper.insert(pair<string, int>(".half", -56));
		Oper.insert(pair<string, int>(".word", -57));
		Oper.insert(pair<string, int>(".space", -58));
		Oper.insert(pair<string, int>(".data", -59));
		Oper.insert(pair<string, int>(".text", -60));
	}
	pos = 0;
	m = 0;
	pos = SearchLabel("main");
	int T = pos;
	pos = 0;
	con[29] = 4 * 1024 * 1024;
	while (pos < T)
	{
		if (flag == 0)
		{
			Operation(Deque[pos]);
			m++;
			//if (pos > 0)
			//cout << m << ' ' << Deque[pos - 1].Op() << ' ' << Deque[pos - 1].obj1 << ' ' << Deque[pos - 1].obj2 << ' ' << Deque[pos - 1].obj3 << "**" << pos - 1 << "**" << Datapos << '\n';
		}
		else if (flag == 1 && returna0 == 0)
			break;
		else if (flag == 1 && returna0 == 1)
		{
			cout << con[4];
			break;
		}
		if (Deque[pos].Op() == ".text")
		{
			while (Deque[pos].Op() != ".data" && (Deque[pos].Op() != "jr" && Deque[pos].obj1 != "$ra"))
			{
				pos++;
			}
		}
		if (Deque[pos].Op() == "jr" && Deque[pos].obj1 == "$ra")
			break;
	}
	pos = T;
	m = 0;
	//system("pause");
	/*while (pos < num)
	{
		if (m == 317)
			int x = 0;
		if (flag == 0)
		{
			Operation(Deque[pos]);
			m++;
			//cout << "***" << pos << '\n';
			//if (pos > 0)
			//cout  << "***" << m << ' ' << Deque[pos - 1].Op() << ' ' << Deque[pos - 1].obj1 << ' ' << Deque[pos - 1].obj2 << ' ' << Deque[pos - 1].obj3 << "**" << pos - 1 << "**" << Datapos << '\n';
		}
		else if (flag == 1 && returna0 == 0)
			break;
		else if (flag == 1 && returna0 == 1)
			//return con[4];
		{
			return con[4];
			break;
		}
	}*/
	Store ssto;	
	Preparation pre;
	Ret ex;
	Ret meo;
	//Store empty;
	while (pos <= num)
	{
		//if (m == 176)
		//{
		//	cout << 1;
		//}
		if (flag == 0)
		{
			WriteBack(meo);
		/*	if (!REmpty(meo))
			{	
				print(meo.st);
			}*/
			meo = MemoryAccess(ex);
			
		}
		else if (flag == 1 && returna0 == 0)
			break;
		else if (flag == 1 && returna0 == 1)
		{
			return con[4];
			break;
		}
		ex = Execution(pre);
		pre = DataPreparation(ssto);
		Preparation fal;
		if (pos < num)
			fal = FalDataPreparation(Deque[pos]);
		if (DataHazard(fal) || (CHazard > 0))
		{
			ssto = Sempty;
		}
		else
		{
			m++;
			//cout << m << ' ' << Deque[pos].Op() << ' ' << Deque[pos].obj1 << ' ' << Deque[pos].obj2 << ' ' << Deque[pos].obj3 << " pos: " << pos << " Datapos: " << Datapos << '\n';
			//print();
			ssto = Fetch();
		}

	}
	/*while (pos < num)
	{
		//if (m == 20)
			//cout << 1;
		ssto = Fetch();
		pre = DataPreparation(ssto);
		ex = Execution(pre);
		if (flag == 0)
		{
			meo = MemoryAccess(ex);
			WriteBack(meo);
			m++;
			cout << m << Deque[pos].Op() << '\n';
			//print();
			//system("pause");			
		}
		else if (flag == 1 && returna0 == 0)
			break;
		else if (flag == 1 && returna0 == 1)
		{
			//system("pause");
			return con[4];	
			break;
		}
	}*/
	//system("pause");
	return 0;
}
