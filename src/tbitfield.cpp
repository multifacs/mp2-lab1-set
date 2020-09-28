// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
	if (len < 0) throw logic_error("Invalid size");
	BitLen = len;
	MemLen = (len / (sizeof(TELEM) * 8)) + 1;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)
		pMem[i] = 0;
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	BitLen = bf.GetLength();
	MemLen = bf.GetMemLen();
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)
		pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
	BitLen = 0;
	MemLen = 0;
	delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	return(n / (sizeof(TELEM) * 8));
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	int index = (n % (sizeof(TELEM) * 8));
	TELEM mask = 1;
	mask = mask << index;
	return mask;
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

int TBitField::GetMemLen() const
{
	return MemLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if ((n < 0) || (n >= (*this).BitLen)) throw logic_error("Invalid index");
	TELEM mask = (*this).GetMemMask(n);
	pMem[GetMemIndex(n)] = pMem[GetMemIndex(n)] | mask;
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if ((n < 0) || (n >= (*this).BitLen)) throw logic_error("Invalid index");
	TELEM mask = (*this).GetMemMask(n);
	mask = ~mask;
	pMem[GetMemIndex(n)] = pMem[GetMemIndex(n)] & mask;
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if ((n < 0) || (n >= (*this).BitLen)) throw logic_error("Invalid index");
	TELEM mask = (*this).GetMemMask(n);
	TELEM bit = pMem[GetMemIndex(n)];
	bit = bit & mask;
	(bit != 0) ? (bit = 1) : (bit);
	return bit;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	if (this == &bf)
		return (*this);
	
	BitLen = bf.GetLength();
	MemLen = bf.GetMemLen();
	delete[](*this).pMem;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)
		pMem[i] = bf.pMem[i];

	return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	if ((*this).GetLength() == bf.GetLength())
	{
		for (int i = 0; i < (*this).GetMemLen(); i++)
		{
			if ((*this).pMem[i] != bf.pMem[i])
			{
				return 0;
			}
		}
		return 1;
	}
	return 0;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
	if ((*this).GetLength() == bf.GetLength())
	{
		for (int i = 0; i < (*this).GetMemLen(); i++)
		{
			if ((*this).pMem[i] != bf.pMem[i])
			{
				return 1;
			}
		}
		return 0;
	}
	return 1;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	if (bf.BitLen > (*this).BitLen) (*this).BitLen = bf.BitLen;
	if ((this->MemLen) < bf.MemLen)
	{
		TELEM* ptr = (this->pMem);
		pMem = new TELEM[bf.MemLen];
		for (int i = 0; i < MemLen; i++)
		{
			pMem[i] = ptr[i];
		}
		for (int i = MemLen; i < bf.MemLen; i++)
		{
			pMem[i] = 0;
		}
		delete[]ptr;
		MemLen = bf.MemLen;

	}
	for (int i = 0; i < bf.GetMemLen(); i++)
	{
		(*this).pMem[i] = (*this).pMem[i] | bf.pMem[i];
	}
	return (*this);
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	if (bf.BitLen > (*this).BitLen) (*this).BitLen = bf.BitLen;
	if ((this->MemLen) < bf.MemLen)
	{
		TELEM* ptr = (this->pMem);
		pMem = new TELEM[bf.MemLen];
		for (int i = 0; i < MemLen; i++)
		{
			pMem[i] = ptr[i];
		}
		for (int i = MemLen; i < bf.MemLen; i++)
		{
			pMem[i] = 0;
		}
		delete[]ptr;
		MemLen = bf.MemLen;

	}
	for (int i = 0; i < bf.GetMemLen(); i++)
	{
		(*this).pMem[i] = (*this).pMem[i] & bf.pMem[i];
	}
	return (*this);
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField mask(BitLen);
	for (int i = 0; i < mask.GetLength(); i++)
	{
		mask.SetBit(i);
	}
	for (int i = 0; i < (*this).GetMemLen(); i++)
	{
		(*this).pMem[i] = ~pMem[i];
		(*this).pMem[i] = (*this).pMem[i] & mask.pMem[i];
	}

	return (*this);
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	int n;
	istr >> n;
	TBitField s(n);
	for (int i = 0; i < n; i++)
	{
		istr >> n;
		if (n == 1)
			s.SetBit(i);
	}
	bf = move(s);
	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	ostr << bf.GetLength() << endl;
	for (int i = 0; i < bf.GetMemLen(); i++)
	{
		ostr << bitset<sizeof(TELEM) * 8>(bf.pMem[i]);
	}
	return ostr;
}
