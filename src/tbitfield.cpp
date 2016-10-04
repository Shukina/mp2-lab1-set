// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)//:BitLen(len)
{
	if (len < 0)
	{
		throw 'l<0';
	}
	else
	{
		BitLen = len;
		MemLen = (len - 1) / (8 * sizeof(TELEM)) + 1;
		//MemLen = (len + 31) >> 5;
		//MemLen = (len/16)+1;
		pMem = new TELEM[MemLen];
		if (pMem != NULL)
			for (int i = 0; i < MemLen; i++)
				pMem[i] = 0;
	}
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	MemLen = bf.MemLen;
	BitLen = bf.BitLen;
	pMem = new TELEM[MemLen];
	if (pMem != NULL)
		for (int i = 0; i < MemLen; i++)
			pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{ 
	delete [] pMem ;
	//pMem = NULL;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n  ?
{
	return n >> 5;
	//return (n/16)+1;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	return 1 << (n & 31);// (n % 32);

}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if (n < 0)
	{
		throw 'n<0'; 
	}
	if (n > BitLen)
	{
		throw 'n>BL';
	}
	if ((n >-1) && (n < BitLen))
		pMem[GetMemIndex(n)] |= GetMemMask(n);

}

void TBitField::ClrBit(const int n) // очистить бит
{
	if (n < 0)
	{
		throw 'n<0'; 
	}
	if (n > BitLen)
	{
		throw 'n>BL';
	}
		if ((n >-1) && (n < BitLen))
			pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита  
{
	if (n < 0)
	{
		throw 'n<0'; 
	}
	if (n > BitLen)
	{
		throw 'n>BL';
	}
	if ((n > -1) && (n < BitLen))
		return ((pMem[GetMemIndex(n)]) & GetMemMask(n));
	else
		return 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	BitLen = bf.BitLen;
	/*if (MemLen != bf.MemLen)
	{
		MemLen = bf.MemLen;*/
			delete [] pMem;
		pMem = new TELEM[MemLen];
	
	if(pMem!=NULL)
	for (int i = 0; i < MemLen; i++)
		pMem[i] = bf.pMem[i];
	return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение(возвращает 0, если не равны, и 1, если равны)
{
	int temp = 1;
	if (BitLen == bf.BitLen)
	{
		for (int i = 0; i < MemLen; i++)
		{
			if (pMem[i] != bf.pMem[i])
			{
				temp = 0;
				break;
			}
		}
	}else
		temp = 0;
  return temp;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение(возвращает 0, если не равны, и 1, если равны)
{
	int temp = 0;
	if (BitLen == bf.BitLen)
	{
		for (int i = 0; i < MemLen; i++)
		{
			if (pMem[i] != bf.pMem[i])
			{
				temp =1;
				break;
			}
		}
	}
	else
		temp = 1;
	return temp;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	int len = BitLen;
	if (bf.BitLen > len)
		len = bf.BitLen;
	TBitField temp(len);
	for (int i = 0; i < MemLen; i++)
		temp.pMem[i] = pMem[i];
	for (int i = 0; i < bf.MemLen; i++)
		temp.pMem[i] |= bf.pMem[i];
	return temp;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	int len = BitLen;
	if (bf.BitLen > len)
		len = bf.BitLen;
	TBitField temp(len);
	for (int i = 0; i < MemLen; i++)
		temp.pMem[i] = pMem[i];
	for (int i = 0; i < bf.MemLen; i++)
		temp.pMem[i] &= bf.pMem[i];
	return temp;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField temp(*this);
	for (int i = 0; i < MemLen - 1; i++) //цикл по длине исходного массива
	{
		temp.pMem[i] = ~(temp.pMem[i]); //делаем инверсию всех значений
	}
	for (int i = (32 * (MemLen - 1)); i < BitLen; i++)
	{
		/*if (((temp).GetBit(i)) == 1) //если значение бита=1
		{
			temp.ClrBit(i); //вставляем вместо 1-> 0
		}*/
		if (((temp).GetBit(i)) == 0)
		{
			temp.SetBit(i); //вставляем вместо 0-> 1
		}
		else
		{
			temp.ClrBit(i); //вставляем вместо 1-> 0
		}
	}
	return temp;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	int i = 0;
	char ts;
	do
	{
		istr >> ts;
	} while (ts != ' ');
	while(1)
	{
		istr >> ts;
		if (ts == '0')
			bf.ClrBit(i++);
		else
			if (ts == '1')
				bf.SetBit(i++);
			else
				break;
	}
	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	int len = bf.GetLength();
	for (int i = 0; i < len; i++)
		if (bf.GetBit(i))
			ostr << '0';
		else
			ostr << '1';
	return ostr;
}
