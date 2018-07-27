#include "matrix.h"
#include <algorithm>
#include <cstdlib>

int RandomNumber() { return (rand()%100); }

double RandNumber() { return double(rand())/100000.; }

int main()
{
   matrix<int, 3, 4> m(5);
  
   try { 
      m[make_pair<size_t,size_t>(0,0)]= 0;
      m[make_pair<size_t,size_t>(1,1)]= 1;
      m[make_pair<size_t,size_t>(1,2)]= 0;
      m[make_pair<size_t,size_t>(0,2)]= 1;
      m[make_pair<size_t,size_t>(2,3)]= 3;
      m[make_pair<size_t,size_t>(2,2)]= 2;
      m[make_pair<size_t,size_t>(2,1)]= 1;
      //cout << m[make_pair<size_t,size_t>(2,3)] << endl;
   } catch (matrixException& e) {
      std::cerr << e.error() << endl;
      // Better to return a specific error code here eventually
      return -1;
   }

   m.print();
   vector<int> in{0, 1, 0, 2, 0, 0, 1, 2, -1, 3, 0, 0, 0, 3, 6, 1, 1};
   matrix<int, 4, 5> n(in);
   n.print();

   cout << "Multiplication Result: " << endl;
   auto result = m*n;
   result.print();

   vector<int> exp{5, 10, 10, -2, 21, 5, 11, 2, 9, 3, 3, 9, 2, 15, 15};
   matrix<int, 3, 5> expected(exp);
   //expected.print();

   if( result == expected )
   {
      cout << "Test1 passed" << endl;
   }
   else
   {
      cout << "Test1 failed!" << endl;
   }

   // Transpose and print
   auto mt = result.transpose();
   mt.print();

   // Multiple with its transpose and result is a symmetric matrix
   // auto d = m*mt;
   // d.print();

   // Since we have multiplication and transpose :
   // can use (A*B)^T = (B^T)*(A^T)  for unit testing
   vector<int> x{2, 3, 1, 0, 4, 5, 1, 0, 3, 0, -1, -2, 0, 3, 4, 5, 7, 6, \
                 5, 3, 0, -2, 2, 5, -1, 2, 3, 0, 3, 9, 2, 0, 2, 1, -1, \
                 -1, 0, 2, 1, -2, 0, 1, 0, 3, -4, 1, -2, -1, 0, 1, 2, 1 };

   matrix <int, 6, 7> a(x);
   vector <int> exp2(63);

   for(int i=2; i< 11; ++i)
   {
      generate (exp2.begin(), exp2.end(), RandomNumber);
      matrix <int, 7, 9> b(exp2);

      auto c = (a*b).transpose();
      c.print();

      auto a1 = a.transpose();
      auto e = b.transpose()*a1;
      e.print();
  
      if( c  == e )
      {
         cout << "Test"+to_string(i)+"  passed" << endl;
      }
      else
      {
         cout << "Test"+to_string(i)+"  failed!" << endl;
      }
   } 

   for( int i=11; i <= 30; ++i)
   {
      vector <double> b1(1920);
      generate (b1.begin(), b1.end(), RandNumber);
      matrix <double, 80, 24> b1m(b1);
   
      vector <double> b2(2400);
      generate (b2.begin(), b2.end(), RandNumber);
      matrix <double, 24, 100> b2m(b2); 

      auto cb = (b1m*b2m).transpose();
      //cb.print();

      auto ab1 = b1m.transpose();
      auto e1 = b2m.transpose()*ab1;
      //e1.print();

      if( cb  == e1 )
      {
         cout << "Test"+to_string(i)+" passed" << endl;
      }
      else
      {
         cout << "Test"+to_string(i)+" failed!" << endl;
      }
   }

   for( int i=31; i <= 50; ++i)
   {
      vector <float> b1(19200);
      generate (b1.begin(), b1.end(), RandNumber);
      matrix <float, 80, 240> b1m(b1);

      vector <float> b2(57600);
      generate (b2.begin(), b2.end(), RandNumber);
      matrix <float, 240, 240> b2m(b2);

      auto cb = (b1m*b2m).transpose();
      //cb.print();

      auto ab1 = b1m.transpose();
      auto e1 = b2m.transpose()*ab1;
      //e1.print();

      if( cb  == e1 )
      {
         cout << "Test"+to_string(i)+" passed" << endl;
      }
      else
      {
         cout << "Test"+to_string(i)+" failed!" << endl;
      }
   }

   for( int i=51; i <= 70; ++i)
   {
      vector <float> b1(192000);
      generate (b1.begin(), b1.end(), RandNumber);
      matrix <float, 800, 240> b1m(b1);

      vector <float> b2(240000);
      generate (b2.begin(), b2.end(), RandNumber);
      matrix <float, 240, 1000> b2m(b2);

      auto cb = (b1m*b2m).transpose();
      //cb.print();

      auto ab1 = b1m.transpose();
      auto e1 = b2m.transpose()*ab1;
      //e1.print();

      if( cb  == e1 )
      {
         cout << "Test"+to_string(i)+" passed" << endl;
      }
      else
      {
         cout << "Test"+to_string(i)+" failed!" << endl;
      }
   }

   for( int i=71; i <= 80; ++i)
   {
      vector <float> b1(768000);
      generate (b1.begin(), b1.end(), RandNumber);
      matrix <float, 800, 960> b1m(b1);

      vector <float> b2(960000);
      generate (b2.begin(), b2.end(), RandNumber);
      matrix <float, 960, 1000> b2m(b2);

      auto cb = (b1m*b2m).transpose();
      //cb.print();

      auto ab1 = b1m.transpose();
      auto e1 = b2m.transpose()*ab1;
      //e1.print();

      if( cb  == e1 )
      {
         cout << "Test"+to_string(i)+" passed" << endl;
      }
      else
      {
         cout << "Test"+to_string(i)+" failed!" << endl;
      }
   }

}
