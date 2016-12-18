#installation

    ./make 
 
Compile the source code.
 
   ./install
   
Configure the environmental variable, logging again is necessary to make all commands available.


#elemental Comands 

  motifTools   Gen  SimpleFile  FeatureFile 
  
The input file name and feature file name are followed by the keyword 'Gen', frequent sequence with 





 
# Encapsulated Commands

The encapsulated Command is composed of elemental commands and shell scripts.  

  FeatureGen   SampleFile  OutFileName

frequent sequences.

libsvmGenWithFeature PositiveSample NegativeSample FeatureFile FeatureProfile OutFileName

  This command will generate libsvm format file, PositiveSample, NegativeSample are combined together and are mapped with frequent sequences and mutation information for those frequent sequences.

libsvmGenerate  PositiveSample  NegativeSample  OutFileName

  This command will generate libsvm format file directly without Feature file input, The FeatureFile FeatureProfile are generated from Positive Sample.



The content of config file:

         SupportRatio = 0.2
         MinLength = 3
         ThresholdRatio = 0.1
         MinToleranceLength = 6

There are four parameters:

     SupportRatio is the minimum frequent for sequences without gaps.
     MinLength is the minimum length for sequences without gaps.
     ThresholdRatio is the minimum frequent for sequences with gaps.
     MinToleranceLength is the minimum Length for sequences will do approximate match.

