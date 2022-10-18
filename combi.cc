#include <iostream>
#include <vector>

using namespace std;

void n_choose_k(int n, int k,  vector <int>::const_iterator itr, vector <int> &seq, vector<vector <int>> &res)
{
  for(int cnt1=0; cnt1<n-k+1; ++cnt1)
  {
    seq.push_back(*itr);
    ++itr;

    // Recurse while there are more levels
    if (k>1) n_choose_k(n-cnt1-1, k-1, itr, seq, res);
    else {
         res.push_back(seq);
    }
    seq.pop_back();
  }
}

void combi( int k, vector <vector <int>> &res)
{

vector <int> start_data = {0,1,2,3,4,5,6,7,8};
vector <int> seq;

vector <int>::const_iterator itr =start_data.cbegin();

n_choose_k(start_data.size(), k, itr, seq, res);

}

void gen_combi_mask(int k, vector <unsigned int> &combi_masks)
{
  static vector <vector <int>> res;  // Temporary vriable

  res.clear();
  combi(k, res);  // Generate the list of combinatorial sequqnces

  for( auto seq:res)
  {
    unsigned int mask=0;
    for( auto ent:seq) mask = mask|(1u<<ent);
    combi_masks.push_back(mask);
  }
}

#if 0
int main()
{

vector <vector <int>>  res;

combi(6,res);

int idx=0;
for (auto itr:res)
{
  cout << idx << " : ";
  for (auto itr2: itr)
  cout << itr2 << " ";

  cout<<endl;
  idx++;

}


}
#endif