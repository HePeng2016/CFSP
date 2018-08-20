# Installation

    ./make 
 
  Compile the source code.
 
    ./install
   
  Configure the environmental variables; to make all commands available, logging in again is necessary.



# Elemental Comands 

    motifTools   Gen  SimpleFile  FeatureFile 
  
  The input (SimpleFile) and output (FeatureFile) follow the keyword 'Gen'. Frequent sequences module, composed by frequent sequence and gaps, are recorded in the FeatureFile. 
  

The format of the SimpleFile is as follows:

    TTTAACCTT.. 
    AAACCCTTG.. 
    CCCGGTTTA..
    ... 
There is one sequence per line.



The format of the FeatureFile is as follows:

      GGCAC.TGGCA...
    
Each line indicates a frequent sequence module,where periods denote a gap between two frequent sequences. 


     motifTools  Stat   FeatureFile  SimpleFile  FeatureProfileFile
   
  The input (FeatureFile and SimpleFile) and output (FeatureProfileFile) follow the keyword, 'Stat'.  FeatureFile is a frequent sequence module file; SimpleFile is an original sequence file. Original sequence, mutational information for each frequent sequence, and each frequent sequence are recorded in the SimpleFile, FeatureProfileFile, and FeatureFile, respectively.


 The format of Feature FeatureProfile file is as follows:
 
     N   M 
     A
     B [ SUB N1 N2 C A ]
    
    
  N is a number identify which frequent sequences module this muational information belong to.
  
  M is the frequence for this frequent sequence module.
  
  A is the count of mutional information for for this frequent sequence module.
  
  B is the frequence for this mutional information.
  
  N1 is a number to identify which frequent sequence the muation was occurred in.
  
  N2 is the position of the mutation at the frequent seuqeunce.
  
  Letters following N1,N2 denote the change from one to another.
  
  SUB/INS/DEL is the muational type. 


        motifTools  Map Feature FeatureProfile SimpleTagFile  Out.libsvm
        
  This command is composed of the keyword, ‘Map’,  followed by the Feature file, FeatureProfile file , sequence file which recorded sequences with tag (SimpleTagFile) and out file (Out.libsvm with libsvm as the format). 
 

  The first character in a sequence is the tag for that sequence.
   

  The config file is necessary that exist in the same directory with data file. if not,the path of config file must be specified.
  
 
 e.g 
  
      motifTools  Gen  SimpleFile  OutFileName  -config  configPATH/config
      
 
The content of config file:


     SupportRatio = 0.2 
     MinLength = 3  
     ThresholdRatio = 0.1  
     MinToleranceLength = 6  
     
There are four parameters:
  
 
   SupportRatio is the minimum frequence for single frequent sequence.
   
   MinLength is the minimum length for single frequent sequence.
   
   ThresholdRatio is the minimum frequence for frequnent sequences module.
   
   MinToleranceLength is the minimum length for frequent sequence will do approximate match.
   
   
 
# Encapsulated Commands

The encapsulated Command is composed of elemental commands and shell scripts.  

        FeatureGen   SampleFile  OutFileName
        
   This command will generate two files, one file is feature file that record the frequent sequence with gaps, and another file is feature profile file that record the mutation information for those frequent sequences. The SampleFile is fasta format.  And names of two outputfile are OutFileName.Feature, OutFileName.FeatureProfile. 

       libsvmGenWithFeature FeatureFile FeatureProfile  Class1Sample Class2Sample OutFileName
                  
  This command will generate libsvm format file, Class1Sample, Class2Sample … ClassNSample are combined together and are
mapped with frequent sequences and mutation information for those frequent sequences. Class1Sample, Class2Sample … ClassNSample are fasta format.

       libsvmGenerate  PositiveSample  NegativeSample  OutFileName

  This command will generate libsvm format file directly without Feature file input,the FeatureFile FeatureProfile are generated from positive Sample. positiveSample and negativeSample are fasta format. 
  
  
     
# The demo commands. 
   
   Demo files for test are located in MotifTest file. 
   
   For two class:
   
        libsvmGenerate PositiveSample NegativeSample OutFileName 
        
   
   For muticlasss: 
   
   
        FeatureGen   FeatureDateSet  FeatureFile
        libsvmGenWithFeature  FeatureFile.Feature  FeatureFile.FeatureProfile  Class1Sample Class2Sample ..  ClassNSample  OutFileName
        
    


