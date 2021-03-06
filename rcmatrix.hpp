#ifndef MATRIX
#define MATRIX
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <exception>

using namespace std;

class file_error : public exception {
  virtual const char* what() const throw() { return "Wystąpił problem z wczytaniem macierzy z pliku."; }
};

class wrong_matrix_error : public exception {
  virtual const char* what() const throw() { return "Macierze nie sa takie same."; }
};

class out_of_index_error : public exception {
  virtual const char* what() const throw() { return "Wyszedles poza macierz."; }
};


class rcmatrix
{
private:
  struct matrix;
  matrix *macierz;
public:
  class Cref;
  rcmatrix();
  rcmatrix(const char*);
  rcmatrix(const rcmatrix&);
  ~rcmatrix();
  rcmatrix& operator= (const rcmatrix&);
  rcmatrix& operator+= (const rcmatrix&);
  rcmatrix operator+ (const rcmatrix&) const;
  rcmatrix& operator-= (const rcmatrix&);
  rcmatrix operator- (const rcmatrix&) const;
  rcmatrix& operator*= (const rcmatrix&);
  rcmatrix operator* (const rcmatrix&) const;
  Cref operator() (int,int);
  friend std::ostream & operator << (std::ostream & , const rcmatrix &);
  double read(int, int) const;
  void write(int, int, double);
};


struct rcmatrix::matrix
{
  double *data;
  int n;
  int ver,col;
  matrix()
  {
    ver = 0;
    col = 0;
    data = NULL;
    n = 1;

  }

  matrix(int m_ver, int m_col, double* source) {
	  if (source != NULL) {
		  ver = m_ver;
		  col = m_col;
		  data = new double[ver*col];
		  for (int i = 0; i < ver; i++) {
			  for (int j = 0; j < col; j++) {
				  data[i*col + j] = source[i*col + j];
			  }
		  }
		  n = 1;
	  }
	  else {
		  ver = 0;
		  col = 0;
		  data = NULL;
		  n = 1;
	  }
  }

  matrix(const char* nameFile)
  {
    fstream file;
    file.open(nameFile,ios::in);
    if(file.good() == 0)
    {
      throw file_error();
    }
    file >> ver;
    file >> col;
    data = new double[ver*col];
    for(int i=0;i<ver;i++)
      for(int j=0;j<col;j++)
        file >> data[i*col+j];
	  n = 1;
  }

  matrix(int m_ver,int m_col) {
    ver = m_ver;
    col = m_col;
    data = new double [ver*col];
    n = 1;
  }

  ~matrix() {
	  delete[] data;
  }

  void add(const struct matrix*);
  void sub(const struct matrix*);
  void mul(const struct matrix*, const struct matrix*);
  matrix* detach();
};


class rcmatrix::Cref
{
  friend class rcmatrix;
  rcmatrix& m;
  int i, j;
  public:
   Cref (rcmatrix& mm, int ii, int jj): m(mm), i(ii), j(jj) {};
   operator double();
   rcmatrix::Cref& operator=(double);
};



#endif
