std::vector< std::list<std::string> >features;

void featureread(const char *filename)
{
    std::string tempword;
    std::list<std::string> tempsentence;
    char charbuffer[1024];
    FILE * ip = fopen(filename,"r");

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

void featurestat(FILE *filename)
{

          for(int i=0;i<features.size();i++)
        {



		for(int j=0;j<Sample.)
            std::list<std::string>::iterator it;
            int count =0;
            bool matched = true;
            char * str = buff;
			/*
			       fscanf(fp,"%lf",&prob.y[n]);
                   fscanf(fp,"%[\b|\t|\n]*",buff);
                   fscanf(fp,"%s",buff)
			*/

            for (it = features[i].begin(); it != features[i].end(); it++)
            {
                std::string temp;
                temp = (*it);
                str = strstr(str,temp.c_str());
                if(!str)
                {
                    matched =false;
                    break;
                  }else
                  {
                       printf("matchesting:%s\n",temp.c_str());
                  }
            }
            if (matched)
            {
				count++;
            }

			  fprintf(filename,"%d\n",count);
        }

}
