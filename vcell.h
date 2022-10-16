#ifndef _my_cell_
#define _my_cell_

#include <iostream>
#include <vector>

using namespace std;


class Vcell {
  
  friend Vcell  operator + (const Vcell &a, const Vcell &b);
  friend Vcell  operator - (const Vcell &a, const Vcell &b);
  friend Vcell  operator * (const Vcell &a, const Vcell &b);
  friend ostream & operator << (ostream & out,const Vcell &a);

  private:
  unsigned int  cell_info;

  public : 
  Vcell(): cell_info(0x1ff) {}
  bool is_solved() {  return((cell_info & (cell_info-1)==0)&&(cell_info!=0)); }
  void operator = (int a);
  void clr() {cell_info=0x1ff;}
  void set(int a) {cell_info= (1<<a)&0x1ff;}

} ;

Vcell operator + (const Vcell &a, const Vcell &b)
{
  // Union
  Vcell ret;
  ret.cell_info = (a.cell_info|b.cell_info) & 0x1ff;
  return(ret);
}

Vcell  operator - (const Vcell &a, const Vcell &b)
{
  Vcell ret;
  ret=(a &(~b))&0x1ff;
  return(ret);
}

Vcell operator * (const Vcell &a, const Vcell &b)
{
  // Intersection
  Vcell ret;
  ret.cell_info = (a.cell_info&b.cell_info) & 0x1ff;
  return(ret);
}

void Vcell::operator = (int a)
{
  cell_info= ((1<<a)) & 0x1ff;
}

ostream & operator << (ostream & out,const Vcell &a)
{  
#if 0
  out <<"(";
  for (int cnt=0;cnt<8;++cnt) {
    if (a.cell_info&(1<<cnt)) out<<cnt<<",";
  }
  if (a.cell_info&(1<<cnt)) out<<cnt;
  out<<")";
#else
   out << std::hex << a.cell_info; 
#endif
  return(out);
}


#endif