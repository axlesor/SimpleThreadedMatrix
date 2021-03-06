#ifndef MATRIX_THREADED_IMPL_H_
#define MATRIX_THREADED_IMPL_H_

#include <iostream>
#include <vector>
#include <utility>
#include <iterator>
#include <string>
#include <thread>
#include <future>


using namespace std;

// Various error cases
const string matrix_row_error ="Row is outside its range";
const string matrix_col_error ="Column is outside its range";

// to handle wrong access issues with matrix
class matrixException
{
public:
   matrixException(string err):mesg(err)
   {;}

   string& error() 
   {
      return mesg;
   }
private:
   string mesg;
};

template <typename T, size_t row, size_t col>
class matrix
{
public:
   // Constructor where all values initialized to the same value
   matrix <T, row, col> (T initialval=T() ):
      data(move(vector<vector <T>> (row, vector<T>(col, initialval))))
   {;}

   // Initialize using a vector
   matrix <T, row, col> (vector<T>& inp)
   {
      int mx = row*col;
      vector<T> rw;
      for(int i=0; i < inp.size() && i < mx; ++i)
      {
         rw.push_back(inp[i]);
         if( rw.size() == col )
         {
            data.push_back(rw);
            rw.clear();
         }
      }
      // fill the rest with default values
      while(data.size() < row)
      {
         while(rw.size() < col)
         {
            rw.push_back(T());
         }
	 data.push_back(rw);
         rw.clear();
      }
   }

   // default copy/move constructor, assignment and destructor
   matrix <T, row, col> (const matrix<T, row, col>& inp) = default;
   matrix <T, row, col> (matrix<T, row, col>&& inp) = default;
   ~matrix <T, row, col>() = default;
   matrix <T, row, col>& operator=(const matrix<T, row, col>& inp) = default;
   matrix <T, row, col>& operator=(matrix<T, row, col>&& inp) = default;
   

   // To multiple first matrix column must be equal to second matrix row
   // If not, there will be a compile time error
   template< size_t C>
   matrix<T, row, C> operator*( matrix<T, col ,C>& other)
   {
      // Allocate result matrix 
      matrix<T, row, C> res;
      
      vector<future<void>> futures;
      int nc = 2*thread::hardware_concurrency(); // using twice is just arbitrary

      //cout << "Current machine hw cores: " << nc << endl;
      int beg = 0;
      int end = row/nc;
      int step = end;
      for (unsigned int l=1 ; l < nc; ++l)
      {
         futures.push_back( async([&](int b, int e) -> void {
            for (unsigned int i=b ; i < e; ++i)
            {
               for(unsigned int j=0; j < C; ++j)
               {
                  T tot = getVal(i,0)*other.getVal(0,j);
                  for (unsigned int k = 1; k< col; ++k)
                  {
                     tot += getVal(i,k)*other.getVal(k,j);
                  }
                  res.getVal(i,j)= tot;
               }
            }
         }, beg, end));
         beg = end;
         end = beg + step;
      }
      end = row;
      // do the end tail
      for (unsigned int i=beg; i < end ; ++i)
      {
         for(unsigned int j=0; j < C; ++j)
         {
            T tot = getVal(i,0)*other.getVal(0,j);
            for (unsigned int k = 1; k< col; ++k)
            {
               tot += getVal(i,k)*other.getVal(k,j);
            }
            res.getVal(i,j)= tot;
         }

      }
      // wait other workers to finish
      for (auto& f: futures)
         f.get();
      return res;
   }

   // Diagonal remains unchanged, just swap upper with lower triangular matrix
   matrix<T, col, row> transpose()
   {
      matrix<T, col, row> res;

      vector<future<void>> futures;
      int nc = 2*thread::hardware_concurrency(); // 2 is arbitrary

      int beg = 0;
      int end = row/nc;
      int step = end;
      for (unsigned int l=1; l < nc; ++l) // leave one for the main
      {
         futures.push_back( async([&](int b, int e) {
            for(unsigned int i=b; i<e; ++i)
            {
               for(unsigned int j=0; j<col; ++j)
               {
                  res.getVal(j,i)=data[i][j];
               }
            }
         }, beg, end));
         beg = end;
         end = beg + step;
      }
      end = row;
      // do the end tail
      for(unsigned int i=beg; i<end; ++i)
      {
         for(unsigned int j=0; j<col; ++j)
         {
            res.getVal(j,i)=data[i][j];
         }
      }
      // wait other workers to finish
      for (auto& f: futures)
         f.get();
      
      return res;
   }

   bool operator==( matrix<T, row, col>& other)
   {
      for (unsigned int i=0; i< row; ++i)
      {
         if(data[i] != other.data[i])
            return false;
      }
      return true;
   }

   // to get the full row
   vector<T>& operator[](size_t rw)
   {
      if( rw >= row )
         throw matrixException(matrix_row_error);
      return data[rw];
   }

   // to read or write any location
   T& operator[](pair<size_t,size_t> k)
   {
      if( k.first >= row )
         throw matrixException(matrix_row_error);
      if( k.second >= col )
         throw matrixException(matrix_col_error);
      return data[k.first][k.second];
   }

   // seem redundant but match faster than pair access
   T& getVal(size_t i, size_t j)
   {
      /*** remove check for quick access
      if( i >= row )
         throw matrixException(matrix_row_error);
      if( j >= col )
         throw matrixException(matrix_col_error);
      ***/
      return data[i][j];
   }
  
   void print()
   {
      cout << "(" << endl;
      for( vector<T>& rw : data )
      {
         cout << rw << endl;
      }
      cout << ")" << endl;
   }


private:
   vector<vector<T>> data;

};

// to print a vector
template <typename K>
std::ostream& operator<< (std::ostream& out, const std::vector<K>& v) 
{
  if ( !v.empty() ) {
    out << '[';
    std::copy (v.begin(), v.end(), std::ostream_iterator<K>(out, ", "));
    out << "\b\b]";
  }
  return out;
}

#endif
