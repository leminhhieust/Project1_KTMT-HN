#pragma once
#include <string>
#include <string.h>
#include <sstream>
#include <iostream>
#include <map>
using namespace std;

class QInt //1 sign bit, 127 bit data - radix 10
{
private:
	__int64 _high; //1 sign bit, 63 bit data
	unsigned __int64 _low; //64 bit data

public: //constructors
	QInt() { _high = _low = 0; }
	QInt(__int64 a, unsigned __int64 b) { _high = a; _low = b; } //internal use
	QInt(const QInt& q)
	{
		_high = q._high;
		_low = q._low;
	}
	QInt(string bitstr, int radix);

public: //destructor
	~QInt() { /*do nothing*/ }

public:  //get functions
	unsigned __int64 getLow() const { return _low; }
	__int64 getHigh() const { return _high; }

public: //assignment operator
	QInt& operator=(const QInt& q); 

public: //logical operators
	friend bool operator==(const QInt& q1, const QInt& q2);
	friend bool operator!=(const QInt& q1, const QInt& q2);

public:
	friend QInt operator-(const QInt& q);

public: //athrimetic operators
	friend QInt operator+(const QInt& q1, const QInt& q2);
	friend QInt operator-(const QInt& q1, const QInt& q2);
	friend QInt operator*(const QInt& q1, const QInt& q2);
	friend QInt operator/(const QInt& q1, const QInt& q2);

public: //bitwise operators //assuming those strings' length is 128
	friend QInt operator&(const QInt& q1, const QInt& q2);
	friend QInt operator^(const QInt& q1, const QInt& q2);
	friend QInt operator|(const QInt& q1, const QInt& q2);
	QInt& operator~();

public:
	QInt& operator<<(const int& q); //left logical shift
	QInt& operator>>(const int& q); //right athrimetic shift

public:
	void ROL(); //Rotate left
	void ROR(); //Rotate right

public: //converter
	string toBinary() const;
	string toHexa() const;
	string toDecimal() const;

public:
	string printQINT(int radix);
};

string getDevinedBy2(string str);
string getDevinedBy2PowI(string str, int i);
void convertToNegativeBin(string& bin);
string plusBitStr(string s1, string s2); //bit string pluses bit string
string getTwoComplement(string str); //get the two's complement of the string
void shiftStrLeft_by1(string& str); //shift the string to the left by one
void shiftStrRight_by1(string& str); //shift the string to the right by one
string normalizeString(string str, int radix); //to normalize the string by adding zeros
string shrinkString(string str); //cutting zeros
