#ifndef _my_cell_
#define _my_cell_

#include <iostream>
using namespace std;


typedef unsigned int uint;

static inline bool test_solved(uint dMask)
{
    
    return((dMask & (dMask-1)==0)&&(dMask!=0));
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

class Vcell {
  
  uint cellData;
   
  public : 
  bool is_solved() { return(test_solved(cellData)); };
  bool set_patrn(uint patrn) { cellData = patrn&0x3fe; return(test_solved(cellData));}
  bool clr_patrn() { cellData = 0; return(test_solved(cellData));}
  bool add_patrn(uint patrn) {cellData =  (cellData | patrn)&0x3fe; return(test_solved(cellData));};
  bool rem_patrn(uint patrn) {cellData = (cellData & (~patrn))&0x3fe; return(test_solved(cellData));};
  uint get_cell_patrn() {return(cellData&0x3fe);};
  uint get_cell_val() {return (mask_to_number(cellData));};
} ;

#endif