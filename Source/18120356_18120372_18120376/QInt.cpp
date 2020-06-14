#include "QInt.h"

using namespace std;

string plusBitStr(string s1, string s2) //bit string pluses bit string
{
	//assuming those 2 strings have the same length
	bool carried = false;
	string sum(128, 0);
	int i;

	for (i = 127; i >= 0; i--)
	{
		if (carried)
		{
			if (int(s1[i]) == 49 && int(s2[i]) == 49)
			{
				sum[i] = 49;
			}
			else if ((int(s1[i]) == 49 && int(s2[i]) == 48) || (int(s1[i]) == 48 && int(s2[i]) == 49))
			{
				sum[i] = 48;
			}
			else if (int(s1[i]) == 48 && int(s2[i]) == 48)
			{
				sum[i] = 49;
				carried = false;
			}
		}
		else
		{
			if (int(s1[i]) == 49 && int(s2[i]) == 49)
			{
				sum[i] = 48;
				carried = true;
			}
			else if ((int(s1[i]) == 49 && int(s2[i]) == 48) || (int(s1[i]) == 48 && int(s2[i]) == 49))
			{
				sum[i] = 49;
			}
			else if (int(s1[i]) == 48 && int(s2[i]) == 48)
			{
				sum[i] = 48;
			}
		}
	}

	return sum;
}

string getTwoComplement(string temp) //get the two's complement of the string
{
	string str(temp);
	int n = str.length();

	for (int i = n - 1; i >= 0; i--)
	{
		if (int(str[i]) == 49)
		{
			for (int j = 0; j < i; j++)
			{
				if (int(str[j]) == 49) str[j] = 48;
				else str[j] = 49;
			}

			break;
		}
	}

	return str;
}

void shiftStrLeft_by1(string& str)  //shift the string to the left by one
{
	for (size_t i = 0; i < str.length() - 1 ; i++)
	{
		str[i] = str[i + 1];
	}

	str[str.length() - 1] = 48;
}

void shiftStrRight_by1(string& str) //shift the string to the right by one
{
	for (size_t i = str.length() - 1; i >= 1; i--)
	{
		str[i] = str[i - 1];
	}

	str[0] = 48;
}

string normalizeString(string str, int radix) //to normalize the string by adding zeros (Binary: 128bit , Hexadecimal:32characters )
{
		if (radix == 2)  // Binary
		{
			string temp;

			if (str.size() < 128)
			{
				temp.append(128 - str.size(), 48);
				temp.append(str);
				return temp;
			}
			else return str;
		}
		else if (radix == 16)  //Hexadecimal
		{
			string temp;

			if (str.size() < 32)
			{
				temp.append(32 - str.size(), 48);
				temp.append(str);
				return temp;
			}
			else return str;
		}
		else return str;
}

string shrinkString(string str)  //cutting the leading zeros
{
	size_t index;
	for (index = 0; index < str.length(); index++)
	{
		if (int(str[index]) != 48) break;
	}

	if (!index) return str;

	string temp = str.substr(index, str.length() - index);
	return temp;
}

bool operator==(const QInt& q1, const QInt& q2)  // operator == (condition)
{
	return ((q1._high == q2._high) && (q1._low == q2._low));
}

bool operator!=(const QInt& q1, const QInt& q2)  // operator !=
{
	return ((q1._high != q2._high) || (q1._low != q2._low));
}

QInt operator-(const QInt& q)  //take the negative QInt
{
	string str = q.toBinary();
	str = normalizeString(str, 2);	

	string str1 = getTwoComplement(str);

	QInt temp(str1, 2);
	return temp;
}

QInt operator+(const QInt& q1, const QInt& q2) // add two QInt together
{
	string string1 = q1.toBinary();
	string string2 = q2.toBinary();

	string1 = normalizeString(string1, 2);
	string2 = normalizeString(string2, 2);

	string str = plusBitStr(string1, string2);

	QInt temp(str, 2);

	return temp;
}

QInt operator-(const QInt& q1, const QInt& q2) // subtract two QInt together
{
	QInt temp = q1 + (-q2);
	return temp;
}

QInt operator*(const QInt& q1, const QInt& q2)  // multiply 2 QInt together
{
	QInt result, temp(q1);

	string str = q2.toBinary();
	str = normalizeString(str, 2);

	for (int i = 127; i >= 0; i--)
	{
		if (int(str[i]) == 49)
		{
			result = result + temp;
			temp = temp << 1;
		}
		else temp = temp << 1;
	}

	return result;
}

QInt operator/(const QInt& q1, const QInt& q2) // a QInt divide a QInt
{
	//assuming all strings here have the same length, they have to
	string string1 = q1.toBinary(); //Q
	string string2 = q2.toBinary(); //M

	string1 = normalizeString(string1, 2);
	string2 = normalizeString(string2, 2);

	if (int(string1[0]) == 48 && int(string2[0]) == 48) //both positive
	{
		string remainder;
		
		if (int(string1[0]) == 49)
		{
			remainder.resize(128, 49);
		}
		else remainder.resize(128, 48);

		int k = 128;

		while (k--)
		{
			shiftStrLeft_by1(remainder);
			shiftStrLeft_by1(string1);

			if (int(string1[0]) == 49) remainder[127] = 49;
			else remainder[127] = 48;

			remainder = plusBitStr(remainder, getTwoComplement(string2)); //A = A - M

			if (int(remainder[0]) == 49) { remainder = plusBitStr(remainder, string2); string1[127] = 48; } //A < 0 -> A = A + M; Q[127] = 0;
			else string1[127] = 49; //Q[127] = 1;
		}

		QInt result(string1, 2);
		result = result >> 1; //the demandatory line for the code to run successfully :)))

		return result;
	}
	else if (int(string1[0]) == 49 && int(string2[0]) == 48) return -((-q1) / q2);
	else if (int(string1[0]) == 48 && int(string2[0]) == 49) return -(q1 / (-q2));
	else return (-q1) / (-q2);
}

QInt operator&(const QInt& q1, const QInt& q2) //operator AND 
{
	string string1 = q1.toBinary();
	string string2 = q2.toBinary();

	string1 = normalizeString(string1, 2);
	string2 = normalizeString(string2, 2);

	string str(128, 0);

	for (int i = 0; i < 128; i++)
	{
		if (int(string1[i]) == 49 && int(string2[i]) == 49) str[i] = 49;
		else str[i] = 48;
	}

	QInt temp(str, 2);
	return temp;
}

QInt operator^(const QInt& q1, const QInt& q2)  // operator XOR
{
	string string1 = q1.toBinary();
	string string2 = q2.toBinary();

	string1 = normalizeString(string1, 2);
	string2 = normalizeString(string2, 2);

	string str(128, 0);

	for (size_t i = 0; i < string1.length(); i++)
	{
		if ((int(string1[i]) == 49 && int(string2[i]) == 48) || (int(string2[i]) == 49 && int(string1[i]) == 48)) str[i] = 49;
		else str[i] = 48;
	}

	QInt temp(str, 2);
	return temp;
}

QInt operator|(const QInt& q1, const QInt& q2) //operator OR
{
	string string1 = q1.toBinary();
	string string2 = q2.toBinary();

	string1 = normalizeString(string1, 2);
	string2 = normalizeString(string2, 2);

	string str(128, 0);

	for (size_t i = 0; i < string1.length(); i++)
	{
		if ((int(string1[i]) == 48 && int(string2[i]) == 48)) str[i] = 48;
		else str[i] = 49;
	}

	QInt temp(str, 2);
	return temp;
}

string getDevinedBy2(string str)  // divined by 2 for Decimal to QInt
{
	{
		if (str[0] == '-')
		{
			str = str.substr(1, str.size() - 1);

			string temp("-");
			temp = temp.append(getDevinedBy2(str));

			return temp;
		}

		if (str == "1") return "0";

		string result = "";
		int x = 0;
		size_t length = str.length();

		for (size_t i = 0; i < length; ++i)
		{
			int k = (x * 10 + str[i] - '0');
			if (k / 2 || i) result += k / 2 + '0';
			x = k % 2;
		}

		return result;
	}
}

QInt::QInt(string str, int radix)  //Convert string to QInt based radix
{
	_high = 0;
	_low = 0;
	if (!str.empty())
	{
		if (radix == 2) //binary
		{
			str = normalizeString(str, radix);

			for (int i = 127; i >= 64; i--)
			{
				if (int(str[i]) == 49) _low += (unsigned __int64)(pow(2, 127 - i));
			}

			for (int i = 63; i >= 0; i--)
			{
				if (int(str[i]) == 49) _high += (__int64)(pow(2, 63 - i));
			}
		}
		else if (radix == 16)//hexadecimal
		{
			string binary = "";
			str = normalizeString(str, radix);
			map<char, string> hexa;
			hexa['0'] = "0000"; hexa['1'] = "0001"; hexa['2'] = "0010"; hexa['3'] = "0011";
			hexa['4'] = "0100"; hexa['5'] = "0101"; hexa['6'] = "0110"; hexa['7'] = "0111";
			hexa['8'] = "1000"; hexa['9'] = "1001"; hexa['A'] = "1010"; hexa['B'] = "1011";
			hexa['C'] = "1100"; hexa['D'] = "1101"; hexa['E'] = "1110"; hexa['F'] = "1111";
			for (size_t i = 0; i < str.size(); ++i)
				binary += hexa.at(str[i]);
			QInt temp(binary, 2);
			*this = temp;
		}
		else if (radix == 10) //decimal
		{
			string n_positive = str;
			bool negative = false;
			string binary = "";
			char temp;
			int count = 0;

			if (str[0] == '-') { //if negative number
				n_positive = str.substr(1, str.length() - 1);
				negative = true;
			}

			while (n_positive != "0" && count < 128) {
				temp = (n_positive[n_positive.size() - 1] - '0') % 2 + '0';
				binary = temp + binary;
				n_positive = getDevinedBy2(n_positive);
			}

			if (negative) binary = getTwoComplement(normalizeString(binary, 2));
			QInt qintTemp(binary, 2);

			*this = qintTemp;
		}
	}
}

QInt& QInt::operator=(const QInt& q) //operator assignment QInt
{
	if (this == &q) return *this;

	_high = q._high;
	_low = q._low;

	return *this;
}

QInt& QInt::operator~() //operator NOT
{
	string str = this->toBinary();
	str = normalizeString(str, 2);

	for (size_t i = 0; i < str.length(); i++)
	{
		if (int(str[i]) == 49) str[i] = 48;
		else str[i] = 49;
	}

	QInt temp(str, 2);
	*this = temp;
	return *this;
}

QInt& QInt::operator<<(const int& q) // shift QInt left
{
	string binary = this->toBinary();
	binary = normalizeString(binary, 2);

	int n = q;

	while (n--)
	{
		shiftStrLeft_by1(binary);
	}

	QInt temp(binary, 2);
	*this = temp;
	return *this;
}

QInt& QInt::operator>>(const int& q)  //shift QInt right
{
	string binary = this->toBinary();
	binary = normalizeString(binary, 2);

	int sign = int(binary[0]);
	int count = q;

	while (count--)
	{
		shiftStrRight_by1(binary);
		binary[0] = sign;
	}

	QInt temp(binary, 2);
	*this = temp;
	return *this;
}

void QInt::ROL()  //turn left 1 bit QInt 
{
	string binary = this->toBinary();
	binary = normalizeString(binary, 2);
	char temp = binary[0];

	shiftStrLeft_by1(binary);
	binary[127] = temp;

	QInt q(binary, 2);

	*this = q;
}

void QInt::ROR()  //turn right 1 bit QInt
{
	string binary = this->toBinary();
	binary =  normalizeString(binary, 2);
	char temp = binary[127];

	shiftStrRight_by1(binary);
	binary[0] = temp;

	QInt q(binary, 2);

	*this = q;
}

string QInt::toBinary() const  //convert QInt to Binary(2)
{
	string str1(64, 0); //high 
	string str2(64, 0); //low
	bool minus = false;
	unsigned __int64 low = _low;
	__int64 high = _high;

	if (high < 0) { high = -high; minus = true; }

	for (int i = 63; i > -1; i--)
	{
		str2[i] = (low % 2) ? 49 : 48;
		low /= 2;
	}

	for (int i = 63; i > -1; i--)
	{
		str1[i] = (high % 2) ? 49 : 48;
		high /= 2;
	}

	if (minus) str1 = getTwoComplement(str1);

	str1.append(str2);
	str1 = shrinkString(str1);
	return str1;
}

string QInt::toHexa() const  //convert QInt to Hexadecimal(16)
{
	string binary = this->toBinary();
	binary = normalizeString(binary,2);

	string result(32, 48);

	for (int i = 127, j = 31; i >= 0 && j >= 0; i -= 4, j--)
	{
		int temp = (int(binary[i]) - 48) + (int(binary[i - 1]) - 48) * 2 + (int(binary[i - 2]) - 48) * 4 + (int(binary[i - 3]) - 48) * 8;

		if (temp >= 0 && temp <= 9) result[j] = (temp + 48);
		else if (temp >= 10 && temp <= 15) result[j] = (temp + 55);
	}

	result = shrinkString(result);
	return result;
}

string QInt::toDecimal() const  //convert QInt to Decimal(10)
{
	string binary = this->toBinary();
	string result(40, 48); //2 ^ 128 - 1 has about 39 digits
	bool minus = false;
	size_t n = binary.length();

	if (binary[0] == 49 && n == 128) { minus = true; binary = getTwoComplement(binary); }
	string::iterator p = binary.begin();

	for (size_t i = 0; i < n; i++)
	{
		int carried = 0;

		for (int i = 39; i >= 0; i--)
		{
			int temp = int(result[i]) * 2 - 96 + carried;
			carried = temp > 9;
			result[i] = temp % 10 + 48;
		}

		if (*p++ == 49) result[39] |= 1;
	}

	result = shrinkString(result);
	if (!minus) return result;

	string temp("-");
	temp.append(result);
	return temp;
}

string QInt::printQINT(int radix)  // convert to that put into file output (in main)
{
	if (radix == 2) return  this->toBinary();
	else if (radix == 10) return  this->toDecimal();
	else if (radix == 16) return this->toHexa();
	else
	{
		string empty("0");
		return empty;
	}
}
