#include <iterator>
#include <iostream>
#include <stdlib.h>
#include <map>
#include <vector>
#include <list>
#include <set>
#include <math.h>
#include "assert.h"
#include <algorithm>
#include "string.h"
#define INSERTED  1
#define UNINSERTED 0

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */
#include <stdio.h>
#include <stdlib.h>
/*  agrep.c - Approximate grep

  This software is released under a BSD-style license.
  See the file LICENSE for details and copyright.
*/


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */
#include <locale.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <assert.h>
#include <limits.h>
#include <unistd.h>
#ifdef HAVE_GETOPT_H
#include <getopt.h>
#endif /* HAVE_GETOPT_H */
#include "regex.h"

#ifdef HAVE_GETTEXT
#include <libintl.h>
#else
#define gettext(s) s
#define bindtextdomain(p, d)
#define textdomain(p)
#endif

#define int64 long long

#define _(String) gettext(String)

#undef MAX
#undef MIN
#define MAX(a, b) (((a) >= (b)) ? (a) : (b))
#define MIN(a, b) (((a) <= (b)) ? (a) : (b))
#define INS   (1<<8)
#define DEL   (2<<8)
#define SUB1  (3<<8)
#define SUB2  (4<<8)



 typedef struct PDFS {
	unsigned int ID;	// The ID of the original input sequence
	unsigned int subID; // The location  of the subsequence  in the same sequence
}PDFS;

 typedef struct ResultEntry{
  unsigned int ID;
  unsigned int subID;
  unsigned int length;
 }ResultEntry;
 typedef struct filteKey{
  unsigned int ID;
  unsigned int endsubID;
 }filteKey;



typedef struct DFS {
    unsigned  short label;// Label in the sequence
    unsigned int depth;
    unsigned int support;
    std::vector <PDFS> Projected;
}DFS;

typedef struct DFSR {
    unsigned int Count;
    unsigned int index;
    unsigned int depth;
    std::vector <int> Projected;
}DFSR;
typedef struct Merged_Result
{
    std::vector <int> PatternIDs;
    std::vector <int> PatternIDsCount;
    std::vector <int> Items;

}Merged_Result;

typedef std::map<unsigned  short, std::vector <PDFS> > Projected_map;


typedef struct MotifElem
{
    std::list<std::string>  Motif;
    int Freq;
}MotifElem;


 static const  int ResultEntryCompare(ResultEntry a,ResultEntry b)
{

	if(a.ID<b.ID)
	{
	    return 1;

	}else if(a.ID>b.ID)
	{
	    return  0;

	}else if(a.subID<b.subID)
	{
		return 1;

	}else
	{
	    return  0;
	 }

}

 static const bool merged_asc( const std::vector <ResultEntry>&a, const std::vector <ResultEntry>&b) {
  if(a.size()!=b.size())
  {
          return false;
  }
  for(int i=0;i<a.size();i++)
  {
      if(a[i].ID != b[i].ID||(a[i].subID>(b[i].subID-a[i].length)))
      {
          return false;
      }
  }
         return true;
       }

 static const bool merged_asc_fuzz( const std::vector <ResultEntry>&a, const std::vector <ResultEntry>&b) {

 double cross = 0.0;
 int i =0;
 int j =0;
  for(;i<a.size()&&j<b.size();i++,j++)
  {
      if(a[i].ID == b[j].ID)
      {
          cross ++;

      if (a[i].subID > b[j].subID||(a[i].subID>(b[j].subID-a[i].length)))
      {
          return false;
      }

      }
      if(a[i].ID > b[j].ID)
      {
         j++;
      }
      if(a[i].ID < b[j].ID)
      {
         i++;
      }

  }

        if((cross*cross)/((a.size()*b.size())*1.0)>0.9*0.9)
        {
           return true;
        }else
         {
           return false;
         }
}


struct Compare
{
bool operator()(const std::vector<int> &a, const std::vector<int> &b) const
{

      if(a.size()<b.size())
      {
          return false;
      }
        if(a.size()>b.size())
      {
          return true;
      }
    for(int i=0;i<a.size();i++)
    {
              if (a[i] > b[i])
              {
                 return  true;
              }
              if(a[i] < b[i])
              {
                 return  false;
              }


    }
          return   false;
}
};

extern double Tolerance_;

struct Compare_Tolerance
{
    bool operator()(const std::vector<int> &a, const std::vector<int> &b) const
    {

       // if(a.size()<b.size())
        {
         //   return false;
        }
        int CoSet =0;
        int i=0,j=0;
        assert( (b[b.size()-1]==INSERTED||b[b.size()-1]==UNINSERTED ) &&(a[a.size()-1] ==UNINSERTED||a[a.size()-1] ==INSERTED) ) ;
        for(i=0,j=0;i<a.size()-1&&j<b.size()-1;)
        {
            if(a[i] == b[j])
            {
                CoSet++;
                i++;
                j++;
                continue;
            }
            if(a[i] > b[j])
            {
                j++;

            }else
            {
                i++;
            }
        }
        if(i<j)
        {
          return false;
        }
        int Size = std::min(i,j);
        if(i==j)
        {
             for(i=0;i<a.size()-1;i++)
            {
                if (a[i] > b[i])
                {
                    break;
                }
                if(a[i] < b[i])
                {
                    return  false;
                }
                }
            if(i== (a.size()-1))
            {
                 return false;
            }

        }
        float diff = (1.0-(CoSet/(sqrt(a.size()-1)* sqrt(b.size()-1))));
        if(diff<Tolerance_&&(CoSet==(b.size()-1))&&b[b.size()-1]==INSERTED&&a[a.size()-1] ==UNINSERTED)
        {
            return false;
        };
          return true;
    }
};



static const bool  EqualVector(std::vector<int> &a, std::vector<int> &b)
{

      if(a.size()!=b.size())
      {
          return false;
      }
    for(int i=0;i<a.size();i++)
    {
              if (a[i]!=b[i])
              {
                 return  false;
              }
    }
    return   true;
}
struct compare
{

bool operator()( const std::vector <filteKey>&a, const std::vector <filteKey>&b) const
{

      if(a.size()<b.size())
      {
          return false;
      }
        if(a.size()>b.size())
      {
          return true;
      }
    for(int i=0;i<a.size();i++)
    {
              if (a[i].ID > b[i].ID)
              {
                 return  true;
              }
              if(a[i].ID < b[i].ID)
              {
                 return  false;
              }
              if(a[i].ID == b[i].ID)
              {
                  if(a[i].endsubID < b[i].endsubID)
                  {
                      return  true;
                  }
              }
    }
          return   false;
}
};


struct DFSPath: public std::vector <DFS> {
public:
	void push (int label,std::vector <PDFS>&Projected,int depth)
	{
		resize (size() + 1);
		DFS &d = (*this)[size()-1];
		d.label = label;
		d.depth = depth;
		d.Projected = Projected;
	}
	void pop () { resize (size()-1); }
};//The  type  of deep first search



struct DFSRPath: public std::vector <DFSR> {
public:
	void push (std::vector <int>&Projected,int Index)
	{
		resize (size() + 1);
		DFSR &d = (*this)[size()-1];
		d.Projected = Projected;
        d.index     = Index;

  }
	void pop () { resize (size()-1); }
};

 static const  int  Merged_ResultArrayComp( Merged_Result a, Merged_Result b)
 {
     if(a.PatternIDs.size() > b.PatternIDs.size())
     {
       return 1;
     }

 }




class ComSeq{
private:
    std::vector <unsigned short>  SubSeq;
    DFSPath   DFSpath;
    DFSRPath   DFSRpath;
    Projected_map  new_projected;
    void  support(DFS &Node);
    int   support(std::vector<ResultEntry> Node);
    void report(std::vector <PDFS>&reportset,int length);

public:
    unsigned int minsupport ;
    int minlength = 5;
    int Threshold = 36;
    int Min_Tolerance_Length = 6;
    std::map <std::vector <filteKey>,std::vector<ResultEntry>,compare> FiltedResult;
    std::vector<std::vector<ResultEntry> >TranArray;
    std::vector<std::vector<unsigned short> >Results;
    std::vector<std::vector<unsigned short> >sample;
    std::set < std::vector<int>,Compare>filterset;
    std::vector< std::vector<ResultEntry> >ResultEntryArray;
    std::vector<Merged_Result>Merged_Result_Array;
    std::vector< std::vector<int> >ResultIDArray;
    std::vector< std::vector<int> >PatternCountArray;
    std::vector< std::vector<int> >PatternLocalEnd;
    std::vector< std::vector<int> >PatternLocalBegin;
    std::set < std::vector<int>,Compare_Tolerance>filtersetTolerance;
    std::vector< std::list<std::string> >features;
    std::vector<std::string>Sample;
    std::vector<std::string>SampleLabel;
    std::map < std::vector <unsigned int>,float > MutationInfoSet;

    void featurestatRead(FILE *ip);
    void featurestat(FILE *filename);
    void Mapping( FILE *op);
    void featureread( FILE * ip);
    void SampleRead(FILE * ip);
    void SampleReadWithLabel(FILE * ip);
    void associte();
    void merged_report();
    void fastaread(FILE * ip);
    void read(FILE *ip);
    void output(FILE *op);
    void filter();
    void run_intern ();
    std::vector <int> Common_Set(std::vector <int>a,std::vector <int>b);


};


