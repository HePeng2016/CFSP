#Installation

    ./make 
 
Compile the source code.
 
    ./install
   
Configure the environmental variable, logging again is necessary to make all commands available.


#Elemental Comands 

    motifTools   Gen  SimpleFile  FeatureFile 
  
The input file name and output feature file name are followed by the keyword 'Gen', frequent sequence complexs which is composed by frequent sequence and gaps are recorded in FeatureFile. 

The format of SimpleFile is described as follow:

    TTTAACCTT.. 
    AAACCCTTG.. 
    CCCGGTTTA..
    ...
    
There is one sequence in each line.

 
The format of Feature file is described as follow:

    GGCAC.TGGCA...
    
Each line indicated a frequent sequence complex,Symbol '.' denote  a gap between two frequent sequence.    

    motifTools  Stat   FeatureFile  SimpleFile  FeatureProfileFile
   
  The name of input files and output feature file are followed by the keyword 'Stat',one input file is frequent sequence complexs file and another file is orginal sequences 
file. The output file recorded the mutational information for each  frequent sequence in 
the FeatureFile. 

The format of Feature FeatureProfile  file is described as follow:

   
   









 
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

