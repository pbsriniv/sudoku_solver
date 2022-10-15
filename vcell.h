#ifndef _my_cell_
#define _my_cell_

#include <iostream>
#include <vector>

using namespace std;


typedef unsigned int uint;


class Vcell {
  
  private:
  uint cell_info;
 
  public : 
  Vcell(): cell_info(0x1ff) {}
  bool is_solved() {  return((cell_info & (cell_info-1)==0)&&(cell_info!=0)); }
  Vcell & operator + (Vcell &a, Vcell &b);

} ;

Vcell & operator + (Vcell &a, Vcell &b)
{
  a.cell_info=(a.cell_info|b.cell_info)&0x1ff;
  return(a);
}

Vcell & operator - (Vcell &a, Vcell &b)
{
  a=(a &(~b))&0x1ff;
  return(a);
}



Vcell & operator = (Vcell &a, int const b)
{
  a=(a &(~b))&0x1ff;
  return(a);
}



static inline int mask_to_number(uint dMask)
{
  int cnt;
  if ((dMask>(1<<9))||(dMask<(1<<1))) cout << "Error Detected" << endl;
  if (test_solved(dMask))
  {
   for(cnt = 1; ((cnt<=9)&&(!(dMask&(1<<cnt)))); cnt++ );
  }
  return(cnt);
}


#endif