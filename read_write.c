/****************************************************************************************************
NAME: conf2dump
FUNCTION: Reads the input file with parameters
INPUT: Parameters file
RETURN: 0
****************************************************************************************************/

int conf2dump( char filename[] )
{
    char cmd[1000];
    int nread;
    /*
    sprintf( cmd, "grep -v \"#\" %s | grep -v \"^$\" | gawk -F\"=\" '{print $2}' > %s.dump", 
	     filename, filename );
    */
    sprintf( cmd, "grep -v \"#\" %s | grep -v \"^$\" | awk -F\"=\" '{print $2}' > %s.dump", 
	     filename, filename );
    nread = system( cmd );

    return 0;
}


/****************************************************************************************************
NAME: read_parameters
FUNCTION: Reads the parameters
INPUT: Parameters file
RETURN: 0
****************************************************************************************************/
int read_parameters( char filename[] )
{
  int nread;
  char cmd[1000], filenamedump[1000];
  FILE *file;
  
  /*+++++ Loading the file +++++*/
  file = fopen( filename, "r" );
  if( file==NULL )
    {
      printf( "  * The file '%s' doesn't exist!\n", filename );
      return 1;
    }
  fclose(file);
  /*+++++ Converting to plain text +++++*/
  conf2dump( filename );
  sprintf( filenamedump, "%s.dump", filename );
  file = fopen( filenamedump, "r" );
  
  /*+++++ Parameters for binary data +++++*/
#ifdef BINARYDATA
  nread = fscanf(file, "%d", &GV.NCELLS);
  nread = fscanf(file, "%s", GV.FILENAME);    
#endif
  
  
  /*+++++ Parameters for ASCII data +++++*/
#ifdef ASCIIDATA
  /*+++++ Simulation parameters +++++*/
  nread = fscanf(file, "%d", &GV.NCELLS);
  nread = fscanf(file, "%lf", &GV.BoxSize);
  nread = fscanf(file, "%s", GV.FILENAME);
  /*+++++ Cosmological parameters +++++*/  
  nread = fscanf(file, "%lf", &GV.Omega_M0);
  nread = fscanf(file, "%lf", &GV.Omega_L0);
  nread = fscanf(file, "%lf", &GV.z_RS);
  nread = fscanf(file, "%lf", &GV.H0);    
  GV.a_SF = 1.0/(1.0 + GV.z_RS); 
#endif

  fclose( file );
  
  printf( "  * The file '%s' has been loaded!\n", filename );
  printf("File to read is %s ", GV.FILENAME);
  printf("with %d cells per axis\n", GV.NCELLS);
  
  
  sprintf( cmd, "rm -rf %s.dump", filename );
  nread = system( cmd );
  
  return 0;
}


/**************************************************************************************************** 
NAME: read_binary
FUNCTION: Reads the binary data file
INPUT: None
RETURN: 0 
****************************************************************************************************/

int read_binary()
{
  int m, nread;
  double pos_aux[3];
  double dummy;
  double dummy_mom[3];
  FILE *inFile=NULL;
  
  inFile = fopen(GV.FILENAME, "r");

  /*+++++ Saving Simulation parameters +++++*/
  nread = fread(&GV.BoxSize, sizeof(double), 1, inFile);  //Box Size
  nread = fread(&GV.Omega_M0, sizeof(double), 1, inFile);  //Matter density parameter
  nread = fread(&GV.Omega_L0, sizeof(double), 1, inFile);  //Cosmological constant density parameter
  nread = fread(&GV.z_RS, sizeof(double), 1, inFile);  //Redshift
  nread = fread(&GV.H0, sizeof(double), 1, inFile);  //Hubble parameter

  GV.a_SF = 1.0 / (1.0 + GV.z_RS);
  printf("-----------------------------------------------\n");
  printf("Cosmological parameters:\n");
  printf("OmegaM0=%lf OmegaL0=%lf redshift=%lf HubbleParam=%lf\n",
	 GV.Omega_M0,
	 GV.Omega_L0,
	 GV.z_RS,
	 GV.H0);
  printf("-----------------------------------------------\n");

  printf("Simulation parameters:\n");
  printf("L=%lf\n",
	 GV.BoxSize);
  printf("-----------------------------------------------\n");


  for(m=0; m<GV.NTOTALCELLS; m++ )
    {             
      nread = fread(&pos_aux[0], sizeof(double), 3, inFile);
      nread = fread(&dummy_mom[0], sizeof(double), 3, inFile);
      nread = fread(&gp[m].potential, sizeof(double), 1, inFile);
      
      if(m%5000000==0)
	{
	  printf("Reading m=%d x=%lf y=%lf z=%lf potential=%lf\n", 
		 m, pos_aux[X], pos_aux[Y], pos_aux[Z],
		 gp[m].potential);
	}//if
      
    }//for m

  fclose(inFile);
  return 0;
}//read_binary
