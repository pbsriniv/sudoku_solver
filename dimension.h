#ifndef _dimension_h_
#define _dimension_h_
#include "vcell.h"

enum class dim_type {ROW,COL,SQUARE};

class Dimension {
    
    bool clean_up();
    void init(Vcell (&) sudArr[9][9], int indx , enum  dim_type dimType);
    enum dim_type get_type() {return(type);};
    enum dim_type get_index() { return(instIndx);};
    uint get_cell_patrn(int cellIdx)   {return(vcell[cellIdx]->get_cell_patrn());};
    private :
    Vcell *vcell[9];
    enum dim_type type;
    int instIndx;
} ;


void Dimension::init( Vcell (&) sudArr[9][9], int indx , enum  dim_type dimType);
{
    type = dimType;
    instIndx = indx;
    switch(dimType)
    {
    
    case ROW:
      for (cntRow =0; cntRow<9; ++cntRow)   { vcell[cntRow] = &(sudArr[cntRow][indx]);};
      break;
    
    case COL:
      for (cntCol =0; cntCol<9; ++cntCol)  { vcell[cntCol] = &(sudArr[indx][cntCol]);};
      break;    
      
    case SQUARE:
     int startRow = (indx/3)*3;
     int startCol = (indx%3)*3;
      for (cntRow =0; cntRow<3, ++cntRow)  
      {
        for (cntCol =0; cntCol<3, ++cntCol)  
        { 
          vcell[cnt] = &(sudArr[startRow+cntRow][startCol+cntCol]);};
        }
      }
     break;
    }
}


bool clean_up()
{
  uint solved_bitmap = 0;
  int num_solved =0;
 
  for (int indx=0; indx<9; ++indx)
  {
      if (vcell[indx]->is_solved())
      {
          ++num_solved;
          solved_bitmap = solved_bitmap | (1<<vcell[indx]->get_cell_val());
      }
  }
  solved_bitmap = solved_bitmap & 0x3fe;
  assert(get_number_ones(solved_bitmap) == num_solved);
  
  // Remove any unnecessary solved numbers
  for (int indx =0; indx <9; ++indx) {
    if (vcell[indx]->is_solved())  vcell[indx]->rem_patrn(solved_bitmap);
  }
}


void get_inter_pattern(Dimension &intr,uint &intrCellPat, uint &compCellPatrnIn, uint &compPatrnOut)
{
    intrCellPat =0;
    compCellPatrnIn =0;
    compPatrnOut =0;
    
    assert(intrCellPat->get_type() != SQUARE)
    if (type == SQUARE)
    {
      if (intr.get_type()=ROW)
      {
   
        // Loop Over the Row Indexes
        for (int cnt=0;cnt<9;++cnt)
        {
          othrCellPatrn  = intr.get_cell_patrn(cnt);
        
        if ((cnt>=startRow)&&(cnt<startRow+3) {
          intrCellPat |= othrCellPatrn;
        }
        else {
          compPatrnOut |= othrCellPatrn;
        }
     
        // Loop Over Mini Square indexes
        uint tempOvrlpPatrn=0;
        uint compPatrnIn=0;
        int rowIndx = intrCellPat->get_index();
        
        for (int cnt=0;cnt<9;++cnt)
        {
          int col = (instIndx%3)*3+(cnt%3);
          int row = (instIndx/3)*3+(cnt/3);
          
          othrCellPatrn  = intr.get_cell_patrn(cnt);
        
        if (rowIndx == row) {
          tempOvrlpPatrn |= othrCellPatrn;
        }
        else {
          compPatrnIn |= othrCellPatrn;
        }
     
        // Ensire the Overlap Patterns between mini-cell and Row match
        assert(tempOvrlpPatrn == intrCellPat);
      }
      else if (intr.get_type()=COL)
      {
        // Loop Over the col Indexes
        for (int cnt=0;cnt<9;++cnt)
        {
          othrCellPatrn  = intr.get_cell_patrn(cnt);
        
          if ((cnt>=startCol)&&(cnt<startCol+3)
          {
            intrCellPat |= othrCellPatrn;
          }
          compPatrnOut |= othrCellPatrn;
        }
        
        // Loop Over Mini Square indexes
        uint tempOvrlpPatrn=0;
        uint compPatrnIn=0;
        int colIndx = intrCellPat->get_index();
        
        for (int cnt=0;cnt<9;++cnt)
        {
          int col = (instIndx%3)*3+(cnt%3);
          int row = (instIndx/3)*3+(cnt/3);
          
          othrCellPatrn  = intr.get_cell_patrn(cnt);
        
        if (colIndx == col) {
          tempOvrlpPatrn |= othrCellPatrn;
        }
        else {
          compPatrnIn |= othrCellPatrn;
        }
     
        // Ensire the Overlap Patterns between mini-cell and Row match
        assert(tempOvrlpPatrn == intrCellPat);
      }
      else
      {
        
      }
    }
}

#endif
