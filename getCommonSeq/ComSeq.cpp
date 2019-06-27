#include "ComSeq.h"
#include "stdio.h"
#define SmoothFactor 0.0000000000000000001
double  Tolerance_ = 1;



void ComSeq::fastaread(FILE * ip)
{
     char * seqName[1024];
     std::vector<unsigned short int>*Temp;
     unsigned int entry = 1;

     while(!feof(ip))
	{

        sample.resize(sample.size()+1);
        Temp = &sample[sample.size()-1];

		fscanf(ip,"%[\b|\t]*",seqName);
	    fscanf(ip,"%s",seqName);
        printf("%s\n",seqName);
		fscanf(ip,"{\b|\t|\n}*",seqName);
        fscanf(ip,"%[\b|\t]*",seqName);
        entry = fgetc(ip);

      do{
          Temp->resize(Temp->size()+1);
          (*Temp)[Temp->size()-1] = entry;
          fscanf(ip,"{\b|\t}*",seqName);
          entry = fgetc(ip);
      } while(entry!='\n'&&entry!=EOF);

      Sample.resize(Sample.size()+1);

          for( int i=0;i<Temp->size();i++)
          {
                 int aa = (*Temp)[i];
                 Sample[Sample.size()-1].append(1,(char)aa);
           }
	}
}

void ComSeq::read(FILE * ip)
{
    std::vector<unsigned short int>*Temp;
    char buffer[1024];
    unsigned int entry = 1;

    while(entry!=EOF)
    {
        sample.resize(sample.size()+1);
        Temp = &sample[sample.size()-1];
        fscanf(ip,"%[\b|\t|\n]*",buffer);
        entry = fgetc(ip);
     do{
          Temp->resize(Temp->size()+1);
          (*Temp)[Temp->size()-1] = entry;
          fscanf(ip,"{\b|\t}*",buffer);
          entry = fgetc(ip);
      } while(entry!='\n'&&entry!=EOF);
    };
}
int ComSeq::support(std::vector<ResultEntry> Node)
{    unsigned int  oldID = 0xffffffff;
     int support =0;

     for (unsigned int n = 0;n<Node.size();n++){
          ResultEntry *cur = &Node[n];
          if(cur->ID!=oldID){
               support++;
          }
          oldID = cur->ID;
     }
     return support;

}

void ComSeq::support(DFS &Node)
{    unsigned int  oldID = 0xffffffff;

     Node.support =0;

     for (unsigned int n = 0;n<Node.Projected.size();n++){
          PDFS *cur = &Node.Projected[n];
          if(cur->ID!=oldID){
             Node.support++;
          }
          oldID = cur->ID;
     }
}

 void printfvector(std::vector <unsigned short>&SubSeq)
 {
   for(int i=0;i<SubSeq.size();i++)
   {
        printf("%d ",SubSeq[i]);
   }
     printf("\n");
 }

 void printfvector( std::vector<PDFS> &SubSeq)
 {
   for(int i=0;i<SubSeq.size();i++)
   {
        printf("%d-",SubSeq[i].ID);
        printf("%d ",SubSeq[i].subID);
   }
     printf("\n");
 }
void ComSeq::report(std::vector <PDFS>&reportset,int length)
{
   std::vector<ResultEntry>subset;
   unsigned int  oldID = 0xffffffff;

   for (std::vector <PDFS>::iterator cur = reportset.begin(); cur != reportset.end(); ++cur)
   {

            subset.resize(subset.size()+1);
            subset[subset.size()-1].ID = (*cur).ID;
            subset[subset.size()-1].subID = (*cur).subID;
            subset[subset.size()-1].length = SubSeq.size();
   }

       std::sort(subset.begin(),subset.end(),ResultEntryCompare);

       TranArray.resize(TranArray.size()+1);
       TranArray[TranArray.size()-1]=subset;

}

std::vector <int> ComSeq::Common_Set(std::vector <int>a,std::vector <int>b)
{

        std::vector <int> CoVector;

        for(int i=0,j=0;i<a.size()&&j<b.size();)
        {
            if(a[i]==b[j])
            {

               CoVector.resize(CoVector.size()+1);
               CoVector[CoVector.size()-1]= a[i];
               i++;
               j++;
               continue;
            }
            if(a[i]>b[j])
            {
                j++;
            }else
            {
                i++;
            }

        }
        return  CoVector;

    }



void ComSeq::merged_report()
{

    std::vector <ResultEntry> old_one;
    ResultEntryArray.resize(FiltedResult.size());
    ResultIDArray.resize(FiltedResult.size());

     int i =0;
     for(std::map <std::vector <filteKey>,std::vector<ResultEntry>,compare>::iterator cur = FiltedResult.begin();cur != FiltedResult.end(); ++cur)
    {
          ResultEntryArray[i]=cur->second;
          i++;
     }


    int n = sample.size();
    int m = ResultEntryArray.size();
    PatternCountArray.resize(m);
    for(int i=0;i<m;i++)
    {
            PatternCountArray[i].resize(n+1);

       for(int j=0;j<n+1;j++)
       {
            PatternCountArray[i][j]=0;
       }
    }
   PatternLocalBegin.resize(m);
   for(int i=0;i<m;i++)
   {
       PatternLocalBegin[i].resize(n);
       for(int j=0;j<n;j++)
       {
            PatternLocalBegin[i][j]=0;
       }

   }
   PatternLocalEnd.resize(m);
   for(int i=0;i<m;i++)
   {
       PatternLocalEnd[i].resize(n);
       for(int j=0;j<n;j++)
       {
           PatternLocalEnd[i][j] =0;
       }
   }





    for(int i=0;i<ResultEntryArray.size();i++)
    {
        if(ResultEntryArray[i][0].length<minlength)
            continue;
        int Num = support(ResultEntryArray[i]);
        ResultIDArray[i].resize(Num);
      {
            unsigned int  oldID = 0xffffffff;
            int  old_n = -1;
            int j =0;
            int Count = 0;
            for (unsigned int n = 0;n<ResultEntryArray[i].size();n++){
               ResultEntry *cur = &ResultEntryArray[i][n];
               Count ++;
               if(cur->ID!=oldID){

                    ResultIDArray[i][j] = cur->ID;
                    PatternCountArray[i][cur->ID] = Count;
                    j++;
                    Count = 0;
                    PatternLocalBegin[i][cur->ID] = n;
                    if(old_n>=0)
                    {
                       PatternLocalEnd[i][oldID] = n;
                    }
                    old_n = n;
                  }
           oldID = cur->ID;
       }
       if(old_n>=0)
       PatternLocalEnd[i][oldID] = ResultEntryArray[i].size();
    }
    }

     for(int i=0;i<m;i++)
    {
       for(int j=0;j<n;j++)
       {
           if(PatternCountArray[i][j]>PatternCountArray[i][n])
           {
                PatternCountArray[i][n]=PatternCountArray[i][j];
           }
            //=0;
       }
    }

    associte();



    std::list<std::string> Motif;
    std::vector <unsigned short>  merged_result;
    std::vector <MotifElem>Motiflist;
    std::vector <ResultEntry> Temp;
    std::string MotifEntry;
    printf("Merged_Result_Array.size:%d \n", Merged_Result_Array.size());


    double progress;
    int progressUnit;

    progress = Merged_Result_Array.size();

    progressUnit = Merged_Result_Array.size()/64+2;


    for(int i=0;i<Merged_Result_Array.size();i++)
    {

        Motif.resize(0);
        Motiflist.resize(0);

        if (i%progressUnit==0)
        printf("Merged Progress have been finished %lf percent\n", i*1.0/progress);

        for(int j=0;j<Merged_Result_Array[i].Items.size();j++)
        {
            int SampleID = Merged_Result_Array[i].Items[j];
            Temp.resize(0);
             for(int k=0;k<Merged_Result_Array[i].PatternIDs.size();k++)
             {
                  int Pindex = Merged_Result_Array[i].PatternIDs[k];
                  for(int l1 = PatternLocalBegin[Pindex][SampleID];l1<PatternLocalEnd[Pindex][SampleID];l1++)
                  {
                      assert(ResultEntryArray[Pindex][l1].ID==SampleID);

                            if(ResultEntryArray[Pindex][l1].length>1)
                           {
                               Temp.push_back(ResultEntryArray[Pindex][l1]);
                           }
                  }
                 // printf("%d ",Pindex);
             }
                 // printf("\n");
               if(Temp.size()>0)
               {



                   int lower_bound_;
                   int up_bound_;
                   MotifEntry.resize(0);
                   Motif.resize(0);
                   std:sort(Temp.begin(),Temp.end(),ResultEntryCompare);
                   up_bound_ = lower_bound_ =-1;
                   for(int i=0;i<Temp.size();i++)
                   {
                       if( up_bound_ == -1)
                       {
                           lower_bound_ = Temp[i].subID;
                           up_bound_    = Temp[i].subID + Temp[i].length-1;
                           continue;
                       }
                       if( Temp[i].subID <= up_bound_+1)
                       {
                            int EndIndex = Temp[i].subID + Temp[i].length-1;
                            if(EndIndex>up_bound_)
                            {
                                up_bound_ = EndIndex;
                            }
                            continue;
                       }
                       if( Temp[i].subID > up_bound_+1)
                       {

                           for(int j=lower_bound_;j<=up_bound_;j++)
                            {

                                MotifEntry.append((const char*)&sample[SampleID][j]);

                            }
                              const char a = '\0';
                              MotifEntry.append(&a);
                              //printf("%s .",MotifEntry.c_str());
                              Motif.push_back(MotifEntry);
                              MotifEntry.resize(0);

                           lower_bound_ = Temp[i].subID;
                           up_bound_    = Temp[i].subID + Temp[i].length-1;
                           continue;
                       }
                   }

                        {
                            for(int j=lower_bound_;j<=up_bound_;j++)
                            {
                                MotifEntry.append((const char*)&sample[SampleID][j]);
                                }
                                const char a = '\0';
                                MotifEntry.append(&a);
                                //printf("%s .",MotifEntry.c_str());
                                Motif.push_back(MotifEntry);
                                MotifEntry.resize(0);
                           }

                  bool Insert = true;
                  int freq =1;


                  for(int i=0;i<Motiflist.size();i++)
                {

                    std::list<std::string>::iterator it1;
                    std::list<std::string>::iterator it2;

                    bool Mismatch = false;
                    for (it1 =  Motiflist[i].Motif.begin(),it2 = Motif.begin(); it1 != Motiflist[i].Motif.end()&&it2!= Motif.end(); it1++,it2++)
                    {       std::string temp1;
                            std::string temp2;
                            temp1 = (*it1);
                            temp2 = (*it2);
                           if(!(temp1.compare(temp2)==0))
                           {
                               Mismatch = true;
                               break;
                           }
                    }
                    if(it1 != Motiflist[i].Motif.end()||it2!= Motif.end())
                    {
                       Mismatch = true;
                    }
                    if(Mismatch)
                    {
                            std::string temp2;
                            std::string temp1;

                            it2 = Motif.begin();
                            temp2 = (*it2);
                            const  char * str = temp2.c_str();
                            for (it1 = Motiflist[i].Motif.begin(); it1 != Motiflist[i].Motif.end()&&it2!=Motif.end(); it1++)
                            {
                                      temp1 = (*it1);
                                      while(it2!= Motif.end())
                                     {

                                         str = strstr(str,temp1.c_str());

                                         if(str)
                                         {

                                            str = str + strlen(temp1.c_str());

                                            break;
                                         }else
                                         {
                                            it2++;
                                            if(it2!= Motif.end())
                                           {
                                               temp2 = (*it2);
                                               str = temp2.c_str();
                                           }

                                         }
                                     }
                                  if(it2 == Motif.end())
                                 {
                                    break;
                                 }

                            }
                            if(it1 == Motiflist[i].Motif.end())
                            {
                                Motiflist[i].Freq = Motiflist[i].Freq+1;
                            }



                            it2 = Motiflist[i].Motif.begin();
                            temp2 = (*it2);
                            str = temp2.c_str();
                            for (it1 = Motif.begin(); it1 != Motif.end()&&it2!=Motiflist[i].Motif.end(); it1++)
                            {
                                      temp1 = (*it1);
                                      while(it2!= Motiflist[i].Motif.end())
                                     {

                                           str = strstr(str,temp1.c_str());



                                         if(str)
                                         {
                                            str = str + strlen(temp1.c_str());

                                            break;
                                         }else
                                         {
                                           it2++;
                                           if(it2!= Motiflist[i].Motif.end())
                                           {
                                               temp2 = (*it2);
                                               str = temp2.c_str();
                                           }

                                         }
                                     }
                                 if(it2 == Motiflist[i].Motif.end())
                                 {
                                    break;
                                 }

                            }
                            if(it1 == Motif.end())
                            {
                                 freq = freq+1;
                            }

                    }else{

                          Motiflist[i].Freq = Motiflist[i].Freq+1;
                          Insert = false;
                    }
                  }
                  if(Insert)
                  {
                      MotifElem Entry;
                      Entry.Motif = Motif;
                      Entry.Freq  = freq;
                      Motiflist.push_back(Entry);
                  }
               //   printf("\n");
                }
            }


      for(int i=0;i<Motiflist.size();i++)
      {
        MotifElem One;
        One = Motiflist[i];
        merged_result.resize(0);
        if(One.Freq > Threshold)
        {
           std::list<std::string>::iterator it;
           for (it = One.Motif.begin();it!=One.Motif.end();it++)
           {
               std::string temp;
               temp = (*it);
               const char * str = temp.c_str();
               for(int i=0;i<temp.length();i++)
              {
                 merged_result.resize(merged_result.size()+1);
                 merged_result[merged_result.size()-1] = str[i];
               }
                 merged_result.resize(merged_result.size()+1);
                 merged_result[merged_result.size()-1]='.';
           }
            if(std::find(Results.begin(),Results.end(),merged_result)==Results.end())
            {
                Results.resize(Results.size()+1);
                Results[Results.size()-1]=merged_result;
            }
        }
      }
      }
    }

void ComSeq::associte()
{

    printf("ResultIDArray.size() %d \n",ResultIDArray.size());
    for(int i=0;i<ResultIDArray.size();i++)
    {
        for(int Count =PatternCountArray[i][PatternCountArray[i].size()-1];Count>=1;Count--)
        {

             std::vector<int> ResultIDArrayTemp;
             ResultIDArrayTemp.resize(0);

                for(int j=0;j<ResultIDArray[i].size();j++)
                {
                    if(PatternCountArray[i][ResultIDArray[i][j]]>=Count)
                    {
                      ResultIDArrayTemp.resize(ResultIDArrayTemp.size()+1);
                      ResultIDArrayTemp[ResultIDArrayTemp.size()-1] = ResultIDArray[i][j];
                    }
                }
            if(filterset.find(ResultIDArrayTemp) == filterset.end())
            {
                 filterset.insert(ResultIDArrayTemp);
            }
            DFSRpath.push(ResultIDArrayTemp,i);
            DFSRpath[DFSRpath.size()-1].Count = Count;
            DFSRpath[DFSRpath.size()-1].depth = 0;
        }
    }
{
    std::vector <int>reportPath;
    std::vector <int>reportPathCount;
    reportPath.resize(0);
    filterset.clear();

    while(DFSRpath.size())
    {
         DFSR * DFSset = &DFSRpath[DFSRpath.size()-1];
         int order = DFSset->index;
         std::vector<int> CoVector;
         std::vector<int> parentSet;
         int max_support = 0;

         parentSet.resize(DFSset->Projected.size());
         int old_support = DFSset->Projected.size();
         int old_depth = DFSset->depth;
         std::copy(DFSset->Projected.begin(),DFSset->Projected.end(),parentSet.begin());
         reportPath.resize(DFSset->depth+1);
         reportPath[DFSset->depth]= DFSset->index;
         reportPathCount.resize(DFSset->depth+1);
         reportPathCount[DFSset->depth]= DFSset->Count;


         DFSRpath.pop();

         if(filterset.find(parentSet)!= filterset.end())
         {
                continue;
         }



         for(int i=0;i<order;i++)
        {
            CoVector = Common_Set(ResultIDArray[i],parentSet);

             if(CoVector.size()<=minsupport)
             {
                 continue;
             }
             {
               std::vector<int> CoVectorTemp;
               std::vector<int> CoVectorTempOld;

               CoVectorTempOld.resize(0);
               assert(PatternCountArray[i][PatternCountArray[i].size()-1]>0);
               for(int Count =PatternCountArray[i][PatternCountArray[i].size()-1];Count>0;Count--)
              {

                 CoVectorTemp.resize(0);

                 for(int j=0;j<CoVector.size();j++)
                 {


                     if(PatternCountArray[i][CoVector[j]]>=Count)
                     {
                         CoVectorTemp.resize(CoVectorTemp.size()+1);
                         CoVectorTemp[CoVectorTemp.size()-1] = CoVector[j];
                     }

                  }
                  if(EqualVector(CoVectorTempOld,CoVectorTemp))
                  {
                      CoVectorTemp.resize(0);
                      continue;
                  }else
                  {
                      CoVectorTempOld = CoVectorTemp;
                  }


                    if(max_support<CoVectorTemp.size())
                    {
                        max_support=CoVectorTemp.size();
                    }
                    if(CoVectorTemp.size()<=minsupport)
                    {
                        continue;
                    }
                    if(filterset.find(CoVectorTemp)!= filterset.end())
                    {
                        continue;
                    }
                    DFSRpath.push(CoVectorTemp,i);
                    DFSRpath[DFSRpath.size()-1].depth = old_depth+1;
                    DFSRpath[DFSRpath.size()-1].Count = Count;
                }

            }
        }

            if((max_support<old_support))
          {

            filterset.insert(parentSet);
            Merged_Result Entry;
            Entry.Items = parentSet;
            Entry.PatternIDs = reportPath;
            Entry.PatternIDsCount = reportPathCount;
            assert(reportPath.size()==reportPathCount.size());
            Merged_Result_Array.push_back(Entry);
          }

    }
}

  std::sort(Merged_Result_Array.begin(),Merged_Result_Array.end(),Merged_ResultArrayComp);

  for(int i=0;i<Merged_Result_Array.size();i++)
  {
       std::vector <int> DistributionSet;
       DistributionSet = Merged_Result_Array[i].Items;
       DistributionSet.resize(DistributionSet.size()+1);
       DistributionSet[DistributionSet.size()-1] = UNINSERTED;
      if(filtersetTolerance.find(DistributionSet) ==filtersetTolerance.end())
      {
          DistributionSet[DistributionSet.size()-1] = INSERTED;
          filtersetTolerance.insert(DistributionSet);
      }else
      {
          Merged_Result_Array[i].PatternIDs.resize(0);
          Merged_Result_Array[i].PatternIDsCount.resize(0);

      }
  }
   for(int i=0;i<Merged_Result_Array.size();i++)
   {
       if(Merged_Result_Array[i].PatternIDs.size()== 0)
       {
           Merged_Result_Array[i] = Merged_Result_Array[Merged_Result_Array.size()-1];
           Merged_Result_Array.resize(Merged_Result_Array.size()-1);
       }
   }

}




void ComSeq::output( FILE * op)
{
   int N =0;

   for(int i=0;i<Results.size();i++)
   {
      if(Results[i].size()<minlength)
      {
          continue;
      }else
       {
          N++;
       }
      for(int j =0;j<Results[i].size();j++)
      {
         fprintf(op,"%c",Results[i][j]);
      }
       fprintf(op,"%c",'\n');
   }
      fclose(op);
      printf("size :%d",N);
}
void ComSeq::filter()
{

  std::vector <filteKey> key;


  for(int i=0;i<TranArray.size();i++)
  {
         key.resize(0);
      for(int j=0;j<TranArray[i].size();j++)
      {
         key.resize(key.size()+1);
         key[key.size()-1].ID = TranArray[i][j].ID;
         key[key.size()-1].endsubID = TranArray[i][j].subID + TranArray[i][j].length-1;
      }
      if(FiltedResult.find(key)!=FiltedResult.end())
      {
          if(FiltedResult[key][0].length<TranArray[i][0].length)
          {
             FiltedResult[key] = TranArray[i];
          }

      }else{

             FiltedResult.insert(make_pair(key,TranArray[i]));
      }
  }
}




void  ComSeq::SampleReadWithLabel(FILE * ip)
{
      char  labelbuffer[1024];


      while(!feof(ip))
     {

         fscanf(ip,"{\b|\t|\n}*",labelbuffer);

         if(fscanf(ip,"%s",labelbuffer)==EOF)
              break;
         SampleLabel.push_back(labelbuffer);
         try{

            char entry = fgetc(ip);

	    fscanf(ip,"{\b|\t|\n}*",labelbuffer);

	    if(entry!='\n'&&entry!=EOF )
	     {
		  Sample.resize(Sample.size()+1);
		}
            while(entry!='\n'&&entry!=EOF){

		   fscanf(ip,"{\b|\t}*",labelbuffer);
                   Sample[Sample.size()-1].append(1,entry);
                   entry = fgetc(ip);
	       };
          }catch(int e)
          {

          }
    }
}





void  ComSeq::SampleRead(FILE * ip)
{
      char charbuffer[1024];

      while(!feof(ip))
     {

          fscanf(ip,"{\b|\t|\n}*",charbuffer);
          char entry = fgetc(ip);
          if(entry!='\n'&&entry!=EOF )
	      {
		     Sample.resize(Sample.size()+1);
          }
           while(entry!='\n'&&entry!=EOF){

                fscanf(ip,"{\b|\t}*",charbuffer);
                Sample[Sample.size()-1].append(1,entry);
                entry = fgetc(ip);
           }
     }
}









void ComSeq::featureread( FILE * ip)
{
    std::string tempword;
    std::list<std::string> tempsentence;
    char charbuffer[1024];

    while(!feof(ip))
   {
        fscanf(ip,"%[\b|\t]*",charbuffer);
       if(fscanf(ip,"%[^.]s",charbuffer)!=0)
       {
           tempword.assign(charbuffer);
           tempsentence.push_back(tempword);
       }
        fscanf(ip,"%[.]s",charbuffer);
        fscanf(ip,"%[\b|\t]*",charbuffer);
       if(fscanf(ip,"%[\n]*",charbuffer)!=0)
       {
           features.push_back(tempsentence);
           tempsentence.resize(0);
       }

   }
}

void ComSeq::Mapping( FILE *op)
{
    regex_t preg[1024];
    int  len;
    int  flags;
    char * buf;
    regaparams_t  Entry;
    regamatch_t match;
    regmatch_t pmatch[1];
    int result[1024];
    std::vector< std::vector<unsigned int> > SimpleIndex;
    std::vector< std::vector<double> > SimpleValue;


     SimpleIndex.resize(Sample.size());
     SimpleValue.resize(Sample.size());


    memset(&match, 0, sizeof(match));
    match.pmatch = pmatch;
    match.nmatch = 1;
    match.matchresult = result;

    Entry.cost_ins = 1;
    Entry.cost_del = 1;
    Entry.cost_subst = 1;
    Entry.max_cost = 1;

    Entry.max_ins = 1024;
    Entry.max_del = 1024;
    Entry.max_subst = 1024;
    Entry.max_err = 1024;



     int MaxLength =0;

     for(int j=0;j<Sample.size();j++)
     {
         int Length = strlen(Sample[j].c_str());

         if ( MaxLength < Length )
         {
             MaxLength = Length;
         }

     }



    if(MutationInfoSet.size()!=0)
    {
        buf = (char*)malloc(sizeof(char)*MAXSIZE*(0.5)*(MaxLength+1));
    }else
    {
        buf = NULL;
    }


      int progressUnit;
      double progress;

      progress = features.size();
      progressUnit = progress/64+2;


      for(int i=0;i<features.size();i++)
      {
              int Count =0;
              bool matched ;
              int index =0;
              double value;
              std::list<std::string>::iterator it;
              std::vector <unsigned int> key;

              if(i%progressUnit == 0)
              {
                 printf("Mapping progress have been finished %lf percent\n",(i*1.0)/progress);
              }



              for (it = features[i].begin(); it != features[i].end(); it++)
              {
                     std::string temp;
                     temp = (*it);
                     if(buf!=NULL)
                     tre_regcomp(&preg[index],temp.c_str(),buf,flags);
                     index = index+1;
               }
                for(int j=0;j<Sample.size();j++)
                {
                     const  char * strPre = Sample[j].c_str();
                     const  char *str;
                     key.resize(0);

                     matched = true;
                     index =0;

                     key.push_back((unsigned int)i);


                     for (it = features[i].begin(); it != features[i].end(); it++)
                    {
                        std::string temp;
                        temp = (*it);
                        str = strstr(strPre,temp.c_str());
                        if(!str)
                        {
                           Entry.max_cost = strlen(temp.c_str())/Min_Tolerance_Length;
                           if(Entry.max_cost==0||buf==NULL)
                           {
                               matched = false;
                               break;
                           }
                        if( tre_reganexec(&preg[index],strPre,strlen(strPre),&match,Entry,flags) != REG_NOMATCH )
                         {
                            int i;
                            unsigned int flags =0;
                            const char * strMatch = temp.c_str();


                            for(i=0;i<strlen(strMatch);i++)
                            {
                                    flags = 0;
                                    if(match.matchresult[i]<0)
                                    {
                                      flags = (index<<10)|i;
                                      flags = flags<<12;
                                      flags = flags|DEL;
                                      flags = flags|strMatch[i];
                                      key.push_back(flags);
                                      continue;
                                    }
                                    if( strPre[match.matchresult[i]]!=strMatch[i])
                                    {
                                      flags = (index<<10)|i;
                                      flags = flags<<12;
                                      flags = flags|SUB1;
                                      flags = flags|strMatch[i];
                                      key.push_back(flags);

                                      flags = 0;
                                      flags = (index<<10)|i;
                                      flags = flags<<12;
                                      flags = flags|SUB2;
                                      flags = flags|strPre[match.matchresult[i]];
                                      key.push_back(flags);
                                      continue;
                                    }
                                    if(i<(strlen(strMatch)-1))
                                    {
                                       for(int k =(match.matchresult[i]+1);k<match.matchresult[i+1];k++)
                                       {

                                            flags = 0;
                                            flags = (index<<10)|i;
                                            flags = flags<<12;
                                            flags = flags|INS;
                                            flags = flags|strPre[k];
                                            key.push_back(flags);
                                       }
                                    }
                                }
                               strPre = strPre + match.matchresult[i-1]+1;
                         }else
                         {
                            matched = false;
                            break;
                         }
                        }else
                        {
                          strPre = str + strlen(temp.c_str());
                        }
                         index = index+1;
                    }

                        if(matched)
                        {

                            SimpleIndex[j].push_back(i);
                            if(key.size()<=1)
                             {
                               SimpleValue[j].push_back(1.0);
                             }
                            else
                            {
                                if(MutationInfoSet.find(key)==MutationInfoSet.end())
                                {
                                   SimpleValue[j].push_back(SmoothFactor);
                                }else
                                {
                                   SimpleValue[j].push_back(MutationInfoSet[key]+SmoothFactor);
                                }
                            }

                        }
                }
      }
   for(int j=0;j<Sample.size();j++)
    {
            std::list<std::string>::iterator it;
            fprintf(op,"%s",SampleLabel[j].c_str());

                if(SimpleIndex[j].size()!=0)
                {
                    for(int i=0;i<SimpleIndex[j].size();i++)
                    {
                        fprintf(op,"  %d:",SimpleIndex[j][i]+1);
                        fprintf(op,"%lf",SimpleValue[j][i]);
                    }
                }
           fprintf(op,"\n");
    }
}



void ComSeq::featurestat(FILE *filename)
{
    regex_t preg[1024];
    int  len;
    int  flags;
    char * buf;
    regaparams_t  Entry;
    regamatch_t match;
    regmatch_t pmatch[1];
    int result[1024];
    std::map <std::vector <unsigned int>,unsigned int> MutationCombinations;


    memset(&match, 0, sizeof(match));
    match.pmatch = pmatch;
    match.nmatch = 1;
    match.matchresult = result;

    Entry.cost_ins = 1;
    Entry.cost_del = 1;
    Entry.cost_subst = 1;
    Entry.max_cost = 1;

    Entry.max_ins = 1024;
    Entry.max_del = 1024;
    Entry.max_subst = 1024;
    Entry.max_err = 1024;


    double progress;
    int progressUnit;



    buf = (char*)malloc(sizeof(char)*MAXSIZE*64);



     int MaxLength =0;

     for(int j=0;j<Sample.size();j++)
     {
         int Length = strlen(Sample[j].c_str());
         if ( MaxLength < Length )
         {
             MaxLength = Length;
         }
     }


     buf = (char*)malloc(sizeof(char)*MAXSIZE*(0.3)*(MaxLength+1));
     progress = features.size();
     progressUnit = progress/64+2;


          for(int i=0;i<features.size();i++)
        {
              int Count =0;
              bool matched ;
              int index =0;
              std::list<std::string>::iterator it;
              std::vector <unsigned int> key;

              if(i%progressUnit==0)
              {
                printf("Mutation statistics progress have been finished %lf percent\n",(i*1.0)/progress);
              }


              MutationCombinations.clear();


              for (it = features[i].begin(); it != features[i].end(); it++)
              {
                     std::string temp;
                     temp = (*it);
                     tre_regcomp(&preg[index],temp.c_str(),buf,flags);
                     index = index+1;
               }



                for(int j=0;j<Sample.size();j++)
                {


                    const  char * strPre = Sample[j].c_str();
                    const  char *str;
                    key.resize(0);

                    matched = true;
                    index =0;

                    for (it = features[i].begin(); it != features[i].end(); it++)
                    {
                            std::string temp;

                            temp = (*it);
                            str = strstr(strPre,temp.c_str());
                        if(!str)
                        {
                            Entry.max_cost = strlen(temp.c_str())/Min_Tolerance_Length;

                            if(Entry.max_cost==0)
                            {
                               matched = false;
                               break;
                            }

                           if ( tre_reganexec(&preg[index],strPre,strlen(strPre),&match,Entry,flags) != REG_NOMATCH )
                           {
                                 int i;
                                 unsigned int flags =0;
                                 const char * strMatch = temp.c_str();

                                 for(i=0;i<strlen(strMatch);i++)
                                {
                                    flags = 0;
                                    if(match.matchresult[i]<0)
                                    {
                                      flags = (index<<10)|i;
                                      flags = flags<<12;
                                      flags = flags|DEL;
                                      flags = flags|strMatch[i];
                                      key.push_back(flags);
                                      continue;
                                    }
                                    if( strPre[match.matchresult[i]]!=strMatch[i])
                                    {
                                      flags = (index<<10)|i;
                                      flags = flags<<12;
                                      flags = flags|SUB1;
                                      flags = flags|strMatch[i];
                                      key.push_back(flags);

                                      flags = 0;
                                      flags = (index<<10)|i;
                                      flags = flags<<12;
                                      flags = flags|SUB2;
                                      flags = flags|strPre[match.matchresult[i]];
                                      key.push_back(flags);
                                      continue;
                                    }
                                    if(i<(strlen(strMatch)-1))
                                    {
                                       for(int k =(match.matchresult[i]+1);k<match.matchresult[i+1];k++)
                                       {

                                            flags = 0;
                                            flags = (index<<10)|i;
                                            flags = flags<<12;
                                            flags = flags|INS;
                                            flags = flags|strPre[k];
                                            key.push_back(flags);
                                       }

                                    }


                                }
                                strPre = strPre + match.matchresult[i-1]+1;


                           }else
                           {
                              matched = false;
                              break;
                           }
                        }else
                        {
                              strPre = str + strlen(temp.c_str());
                        }
                       index = index+1;
                    }
            if (matched)
            {
              if(key.size()==0)
				Count++;
              else
              {
                  if(MutationCombinations.find(key)!= MutationCombinations.end())
                  {
                       MutationCombinations[key] = MutationCombinations[key]+1;
                  }else
                   {
                       MutationCombinations.insert(make_pair(key,1));
                   }
              }
            }
        }

    fprintf(filename,"%d    ",i);
    fprintf(filename,"%d\n",Count);
    fprintf(filename,"%d\n",MutationCombinations.size());

    for(std::map <std::vector <unsigned int>,unsigned int>::iterator cur = MutationCombinations.begin();cur!=MutationCombinations.end();++cur)
    {
        std::vector<unsigned int> temp = cur->first;

       /*    if(cur->second >= Count )
        {
           continue;
        }
       */

        fprintf(filename,"%d ",cur->second);
        fprintf(filename,"%s","[");
        for(int i =0;i<temp.size();i++)
        {
            int index;
            int location;
            unsigned int TYPE;
            index = temp[i]>>22;
            location = (temp[i]>>12)&0x3ff;
            TYPE = (temp[i])&(0xf<<8);

            if( TYPE == DEL)
            {
                fprintf(filename,"%s ","DEL");
                fprintf(filename,"%d ",index);
                fprintf(filename,"%d ",location);
                fprintf(filename,"%c ",(char)temp[i]&0xff);
            }
            if( TYPE == INS)
            {
                fprintf(filename,"%s ","INS");
                fprintf(filename,"%d ",index);
                fprintf(filename,"%d ",location);
                fprintf(filename,"%c ",(char)temp[i]&0xff);
            }
            if( TYPE == SUB1)
            {
                fprintf(filename,"%s ","SUB");
                fprintf(filename,"%d ",index);
                fprintf(filename,"%d ",location);
                fprintf(filename,"%c ",(char)temp[i]&0xff);
            }
            if( TYPE == SUB2)
            {
                fprintf(filename,"%c ",(char)temp[i]&0xff);
            }
          }
           fprintf(filename,"%s","]");
           fprintf(filename,"\n");
         }
       }

    free(buf);

}

void ComSeq::featurestatRead(FILE *ip)
{
    char charbuffer[1024];
    int ID;
    int MotifFreq;
    int MutationNum;
    int MutationFreq;
    int MutatoinID;
    unsigned int MotifInf;

    while(!feof(ip))
   {
        fscanf(ip,"%{\b|\t}*",charbuffer);
        fscanf(ip,"%d",&ID);
        fscanf(ip,"%{\b|\t}*",charbuffer);
        fscanf(ip,"%d",&MotifFreq);
        fscanf(ip,"%{\b|\t|\n}",charbuffer);
        fscanf(ip,"%d",&MutationNum);

        for(int i=0;i<MutationNum;i++)
        {
          std::vector <unsigned int> MotifInfArray;

          fscanf(ip,"%d",&MutationFreq);
          fscanf(ip,"%{\b|\t}*",charbuffer);




          fscanf(ip,"[",charbuffer);

           if( MutationFreq >= MotifFreq)
           {
               char End = fgetc(ip);
               while( End != ']'&& End>0)
               {
                  End = fgetc(ip);
               }
           }else
           {

              MotifInfArray.push_back((unsigned int)ID);
              fscanf(ip,"%{\b|\t}*",charbuffer);
            while(!feof(ip))
            {


                fscanf(ip,"%{\b|\t}*",charbuffer);
                fscanf(ip,"%s",charbuffer);

                 if(strcmp(charbuffer,"]")==0)
                 {
                    break;
                 }

                if(strcmp(charbuffer,"SUB")==0)
                {
                    int index;
                    int location;
                    char Source;
                    char Destination;

                    fscanf(ip,"%{\b|\t}*",charbuffer);
                    fscanf(ip,"%d",&index);
                    fscanf(ip,"%{\b|\t}*",charbuffer);
                    fscanf(ip,"%d",&location);
                    fscanf(ip,"%{\b|\t}*",charbuffer);
                    fscanf(ip,"%c",&Source);
                    fscanf(ip,"%{\b|\t}*",charbuffer);
                    fscanf(ip,"%c",&Destination);

                    MotifInf = 0;
                    MotifInf = MotifInf|index;
                    MotifInf = MotifInf<<10;
                    MotifInf = MotifInf|location;
                    MotifInf = MotifInf<<12;
                    MotifInf = MotifInf|SUB1;
                    MotifInf = MotifInf|Source;
                    MotifInfArray.push_back(MotifInf);
                    MotifInf = 0;
                    MotifInf = MotifInf|index;
                    MotifInf = MotifInf<<10;
                    MotifInf = MotifInf|location;
                    MotifInf = MotifInf<<12;
                    MotifInf = MotifInf|SUB2;
                    MotifInf = MotifInf|Destination;
                    MotifInfArray.push_back(MotifInf);
                }
               if(strcmp(charbuffer,"DEL")==0)
               {
                    int index;
                    int location;
                    char DelChar;


                    fscanf(ip,"%{\b|\t}*",charbuffer);
                    fscanf(ip,"%d",&index);
                    fscanf(ip,"%{\b|\t}*",charbuffer);
                    fscanf(ip,"%d",&location);
                    fscanf(ip,"%{\b|\t}*",charbuffer);
                    fscanf(ip,"%c",&DelChar);

                     MotifInf = 0;
                     MotifInf = MotifInf|index;
                     MotifInf = MotifInf<<10;
                     MotifInf = MotifInf|location;
                     MotifInf = MotifInf<<12;
                     MotifInf = MotifInf|DEL;
                     MotifInf = MotifInf|DelChar;
                     MotifInfArray.push_back(MotifInf);
               }


                if(strcmp(charbuffer,"INS")==0)
               {
                    int index;
                    int location;
                    char InsertChar;


                    fscanf(ip,"%{\b|\t}*",charbuffer);
                    fscanf(ip,"%d",&index);
                    fscanf(ip,"%{\b|\t}*",charbuffer);
                    fscanf(ip,"%d",&location);
                    fscanf(ip,"%{\b|\t}*",charbuffer);
                    fscanf(ip,"%c",&InsertChar);

                     MotifInf = 0;
                     MotifInf = MotifInf|index;
                     MotifInf = MotifInf<<10;
                     MotifInf = MotifInf|location;
                     MotifInf = MotifInf<<12;
                     MotifInf = MotifInf|INS;
                     MotifInf = MotifInf|InsertChar;
                     MotifInfArray.push_back(MotifInf);
               }

                  fscanf(ip,"%{\b|\t}*",charbuffer);
               };

                 fscanf(ip,"%{\b|\t|\n}*",charbuffer);
                 MutationInfoSet.insert(make_pair(MotifInfArray,(MutationFreq*1.0)/(MotifFreq*1.0)));
              }
           }
           }
        }

void ComSeq::featuresToPSSM(FILE *op)
{
            int index;
            int location;
            int type;
            char Source;
            unsigned MotifInf;
            std::vector < std::vector<std::vector<BaseType> > >FeatureFSSM;



            FeatureFSSM.resize(features.size());

            for(int i=0;i<features.size();i++)
            {
              FeatureFSSM[i].resize(0);
            }



   for( std::map < std::vector <unsigned int>,float >::iterator Index = MutationInfoSet.begin();Index!=MutationInfoSet.end(); ++Index )
   {

        std::vector <unsigned int> MotifInfArray = (*Index).first;
        double Ratio = (*Index).second;
        int ID = MotifInfArray[0];
        std::list<std::string> tempsentence;
        tempsentence = features[ID];
        std::vector<std::string> TempArray;
        std::list<std::string>::iterator it;


        for (it = features[ID].begin(); it != features[ID].end(); it++)
        {
                     std::string temp;
                     temp = (*it);
                     TempArray.resize(TempArray.size()+1);
                     TempArray[TempArray.size()-1]=temp;
        }

        if (FeatureFSSM[ID].size()==0)
        {

              FeatureFSSM[ID].resize( TempArray.size() );



             for(int i=0;i<TempArray.size();i++)
            {
              char * Array = (char*)TempArray[i].c_str();
              FeatureFSSM[ID][i].resize(strlen(TempArray[i].c_str()));

              for(int I=0;I<strlen(TempArray[i].c_str());I++)
              {
                  FeatureFSSM[ID][i][I].T =0.0;
                  FeatureFSSM[ID][i][I].A =0.0;
                  FeatureFSSM[ID][i][I].C =0.0;
                  FeatureFSSM[ID][i][I].G =0.0;


                  if( Array[I]=='T'|| Array[I]=='U')
                  {
                     FeatureFSSM[ID][i][I].T=1.0;
                  }
                  if( Array[I]=='A' )
                  {
                     FeatureFSSM[ID][i][I].A=1.0;
                  }
                  if(  Array[I]=='C' )
                  {
                     FeatureFSSM[ID][i][I].C=1.0;
                  }
                  if(  Array[I]=='G')
                  {
                     FeatureFSSM[ID][i][I].G=1.0;
                  }
              }
            }
        }



             for(int i=0;i<TempArray.size();i++)
            {
              char * Array = (char*)TempArray[i].c_str();

              for(int I=0;I<strlen(TempArray[i].c_str());I++)
              {
                  if(  Array[I]=='T'|| Array[I]=='U')
                  {
                     FeatureFSSM[ID][i][I].T=FeatureFSSM[ID][i][I].T+Ratio;
                  }
                  if(  Array[I]=='A')
                  {
                     FeatureFSSM[ID][i][I].A=FeatureFSSM[ID][i][I].A+Ratio;
                  }
                  if( Array[I]=='C')
                  {
                     FeatureFSSM[ID][i][I].C=FeatureFSSM[ID][i][I].C+Ratio;
                  }
                  if( Array[I]=='G')
                  {
                     FeatureFSSM[ID][i][I].G=FeatureFSSM[ID][i][I].G+Ratio;
                  }
              }
            }






        for(int i=1;i<MotifInfArray.size();i++)
        {
              MotifInf = MotifInfArray[i];
              index    = MotifInf>>22;
              location = (MotifInf>>12)&0x3ff;
              Source   = MotifInf&0xff;
              type     = ((MotifInf>>8)<<8)&0xfff;

              if(type==SUB2)
              {
                  if ( (Source=='T')||(Source=='U') )
                  {
                      FeatureFSSM[ID][index][location].T=FeatureFSSM[ID][index][location].T+Ratio;
                  }
                  if( Source=='A')
                  {
                      FeatureFSSM[ID][index][location].A=FeatureFSSM[ID][index][location].A+Ratio;
                  }
                  if( Source=='C')
                  {
                     FeatureFSSM[ID][index][location].C=FeatureFSSM[ID][index][location].C+Ratio;
                  }
                  if( Source=='G')
                  {
                     FeatureFSSM[ID][index][location].G=FeatureFSSM[ID][index][location].G+Ratio;
                  }
              }

              if(type==SUB1||type==DEL)
              {
                  if( Source=='T'|| Source=='U')
                  {
                      FeatureFSSM[ID][index][location].T=FeatureFSSM[ID][index][location].T-Ratio;
                  }
                  if( Source=='A' )
                  {
                      FeatureFSSM[ID][index][location].A=FeatureFSSM[ID][index][location].A-Ratio;
                  }
                  if( Source=='C')
                  {
                     FeatureFSSM[ID][index][location].C=FeatureFSSM[ID][index][location].C-Ratio;
                  }
                  if( Source=='G')
                  {
                     FeatureFSSM[ID][index][location].G=FeatureFSSM[ID][index][location].G-Ratio;
                  }

              }




        }
    }

             for(int i=0;i<FeatureFSSM.size();i++)
            {

                  std::list<std::string>::iterator it;

                  fprintf(op,">");

                  for (it = features[i].begin(); it != features[i].end(); it++)
                  {

                     std::string temp;
                     temp = (*it);
                     fprintf(op,"%s.",temp.c_str());
                  }

                     fprintf(op,"\n");

                for(int j=0;j<FeatureFSSM[i].size();j++)
                {

                    fprintf(op,"%s","A [");

                    for(int j2=0;j2<FeatureFSSM[i][j].size();j2++)
                    {
                      fprintf(op,"%lf  ",FeatureFSSM[i][j][j2].A);
                    }
                     fprintf(op,"%s","]");
                     fprintf(op,"\n");

                      fprintf(op,"%s","C [");
                     for(int j2=0;j2<FeatureFSSM[i][j].size();j2++)
                     {
                       fprintf(op,"%lf  ",FeatureFSSM[i][j][j2].C);
                     }

                      fprintf(op,"%s","]");
                      fprintf(op,"\n");

                        fprintf(op,"%s","G [");
                      for(int j2=0;j2<FeatureFSSM[i][j].size();j2++)
                      {
                       fprintf(op,"%lf  ",FeatureFSSM[i][j][j2].G);
                      }
                       fprintf(op,"%s","]");
                      fprintf(op,"\n");

                      fprintf(op,"%s","T [");
                      for(int j2=0;j2<FeatureFSSM[i][j].size();j2++)
                      {
                        fprintf(op,"%lf  ",FeatureFSSM[i][j][j2].T);
                      }
                       fprintf(op,"%s","]");
                       fprintf(op,"\n");
                    }
                    fprintf(op,"\n");


                }

            }








void ComSeq::run_intern()
{

       unsigned int old_depth = 0;
       unsigned int max_support = 0;
       unsigned int old_support = 0;
       std::vector <PDFS> reportset;

       FiltedResult.clear();
       TranArray.clear();
       new_projected.clear();
       SubSeq.resize(1);

       for(int i=0;i<sample.size();i++){
           for(int j=0;j<sample[i].size();j++){
             PDFS cur;
             cur.ID = i;
             cur.subID = j;
             new_projected[sample[i][j]].push_back(cur);
           }
       }

       for(Projected_map::iterator cur =new_projected.begin();cur != new_projected.end(); ++cur){
            DFSpath.push(cur->first,cur->second,0);
            support(DFSpath[DFSpath.size()-1]);
            if(DFSpath[DFSpath.size()-1].support<minsupport)
            {
               DFSpath.pop();
            }

        }
       //inite


       while(DFSpath.size())
      {

        DFS * DFSset = &DFSpath[DFSpath.size()-1];
        new_projected.clear();

        for (unsigned int n = 0; n < DFSset->Projected.size(); ++n){
            PDFS *cur = &DFSset->Projected[n];
                if(cur->subID+DFSset->depth+1<sample[cur->ID].size())
                    new_projected[sample[cur->ID][cur->subID+DFSset->depth+1]].push_back(*cur);
                }

        SubSeq.resize(DFSset->depth+1);
        SubSeq[DFSset->depth] = DFSset->label;

        old_depth = DFSset->depth;
        old_support = DFSset->support;
        reportset = DFSset->Projected;
        DFSpath.pop();
        max_support = 0;

        for(Projected_map::iterator cur =new_projected.begin();cur != new_projected.end(); ++cur)
        {
            DFSpath.push(cur->first,cur->second,old_depth+1);
            support(DFSpath[DFSpath.size()-1]);
            if(DFSpath[DFSpath.size()-1].support<minsupport)
            {
                DFSpath.pop();
                continue;
            }//
            if(DFSpath[DFSpath.size()-1].support>max_support)
            {
                max_support = DFSpath[DFSpath.size()-1].support;
            }
        }
        if(max_support<old_support)
        {
             report(reportset,old_depth);
        }
         reportset.clear();
    }
     filter();
     merged_report();
}
