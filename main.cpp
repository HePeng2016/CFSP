#include <iostream>
#include "stdio.h"
#include "getCommonSeq/ComSeq.h"
using namespace std;

int main( int argc , char *argv[] )
{

    ComSeq test;
    FILE * ip1,*op,*ip2;
    FILE * ip3;
    FILE * configip =NULL;
    double SupportRatio =-1;
	double ThresholdRatio =-1;
    // fopen(argv[3],"r");


    for(int i =0;i<argc-1;i++)
    {
       if( strcmp(argv[i],"-config")==0)
       {
           configip = fopen(argv[i+1],"r");
       }
    }

    if( configip == NULL)
    {
        configip = fopen("config","r");
    }

    test.minlength = 4;
  // test.SampleRead(ip1);
 // test.featureread(ip2);
    test.Threshold = 13;

    if(configip!= NULL)
    {
       char buffer[1024];


       while (!feof(configip))
       {
               fscanf(configip,"%[\b|\t]*",buffer);
               fscanf(configip,"%s",buffer);

               if(strcmp(buffer,"SupportRatio")==0)
               {
                   fscanf(configip,"%[\b|\t]*",buffer);
                   fscanf(configip,"%s",buffer);

                   if(strcmp(buffer,"=")==0)
                   {
                      fscanf(configip,"%lf",&SupportRatio);
                   }
                   fscanf(configip,"%[\b|\t|\n]",buffer);
                   continue;
               }

               if(strcmp(buffer,"MinLength")==0)
               {
                   fscanf(configip,"%[\b|\t]*",buffer);
                   fscanf(configip,"%s",buffer);

                   if(strcmp(buffer,"=")==0)
                   {
                      fscanf(configip,"%d",&test.minlength);
                   }
                   fscanf(configip,"%[\b|\t|\n]",buffer);
                   continue;
               }
               if(strcmp(buffer,"ThresholdRatio")==0)
               {
                   fscanf(configip,"%[\b|\t]*",buffer);
                   fscanf(configip,"%s",buffer);


                   if(strcmp(buffer,"=")==0)
                   {
                      fscanf(configip,"%lf",&ThresholdRatio);
                   }
                   fscanf(configip,"%[\b|\t|\n]",buffer);
                   continue;
               }
              if(strcmp(buffer,"MinToleranceLength")==0)
               {
                   fscanf(configip,"%[\b|\t]*",buffer);
                   fscanf(configip,"%s",buffer);


                   if(strcmp(buffer,"=")==0)
                   {
                      fscanf(configip,"%d",&test.Min_Tolerance_Length);
                   }
                   fscanf(configip,"%[\b|\t|\n]",buffer);
                   continue;
               }

       }

    }



    for(int i =0;i<argc-1;i++)
    {
       if( strcmp(argv[i],"Gen")==0)
       {
           ip1 = fopen(argv[i+1],"r");
           op  = fopen(argv[i+2],"w+");

           if(ip1==NULL||op==NULL)
           {
               printf("Usaging: motifTools Gen  SimpleFile OutputFile");
               return 0;
           }
              printf("\n");




           test.read(ip1);
           if(SupportRatio>0)
           {
                test.minsupport = test.sample.size()*SupportRatio;
           }else
           {
                test.minsupport = test.sample.size()*0.3;
           }

		   if(ThresholdRatio>0)
		   {
			    test.Threshold = ThresholdRatio*test.sample.size();
			}else
		    {
				 test.Threshold = test.sample.size()*0.1;
			}






           test.run_intern();
           test.output(op);

           return 0;

       }
        if( strcmp(argv[i],"Stat")==0)
       {
             ip1 = fopen(argv[i+1],"r");
             ip2 = fopen(argv[i+2],"r");
             op  = fopen(argv[i+3],"w+");

            if(ip1==NULL||op==NULL||ip2==NULL)
            {
               printf("Usaging: motifTools Stat  FeatureFile  SimpleFile OutputFile");
               return 0;
            }
              printf("\n");



             test.featureread(ip1);
             test.SampleRead(ip2);
             test.featurestat(op);
             return 0;

       }
        if( strcmp(argv[i],"Map")==0)
       {
               ip1 = fopen(argv[i+1],"r");
               ip2 = fopen(argv[i+2],"r");
               ip3 = fopen(argv[i+3],"r");
               op  = fopen(argv[i+4],"w+");


              if(ip1==NULL||ip3 ==NULL||op==NULL)
              {
                 printf("Usaging: motifTools Map  FeatureFile FeatureMutationalFile   SimpleFileWithLabel OutputFile");
                 return 0;
              }

                printf("\n");

               test.featureread(ip1);
               if(ip2!=NULL)
                 test.featurestatRead(ip2);
               test.SampleReadWithLabel(ip3);
               test.Mapping(op);
               return 0;
       }


        if( strcmp(argv[i],"ToPSSM")==0)
       {
               ip1 = fopen(argv[i+1],"r");
               ip2 = fopen(argv[i+2],"r");
               ip3 = fopen(argv[i+3],"w+");
               test.featureread(ip1);
               test.featurestatRead(ip2);
               test.featuresToPSSM(ip3);

       }
  }
   return 0;
}
