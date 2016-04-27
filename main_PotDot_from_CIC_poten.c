/****** INCLUDING LIBRARIES ******/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#include <string.h>


/****** INCLUDING SUPPORTING FILES ******/
#include "variables.c"
#include "read_write.c"
#include "PotDot_poten.c"


int main(int argc, char *argv[])
{
  char *infile = NULL;
  
  /*+++++ Reading parameters file +++++*/
  if(argc < 2)
    {
      printf("Error: Incomplete number of parameters. Execute as follows:\n");
      printf("%s Parameters_file\n", argv[0]);
      exit(0);
    }//if   
  
  infile = argv[1];
    
  read_parameters( infile );

  /*+++++ Some variables +++++*/  
  GV.ZERO         = 1.0e-300;
  GV.NTOTALCELLS  = GV.NCELLS*GV.NCELLS*GV.NCELLS;
  printf("Variables are ready to use!\n");
  printf("-----------------------------------------------------------------\n");
  
  
  /*--- MEMORY ALLOCATION ---*/
  gp = (struct grid *) malloc((size_t) GV.NTOTALCELLS*sizeof(struct grid));

  read_binary();
  printf("Binary data file has been read succesfully!\n");
  printf("-----------------------------------------------------------------\n");

  
  /*+++++ Hubble's constant in terms of redshift +++++*/
  GV.Hz       = GV.H0 * sqrt(GV.Omega_L0 + GV.Omega_M0 * pow( (1+GV.z_RS), 3 ) ); 
  GV.CellSize = GV.BoxSize/(1.0*GV.NCELLS); //Size of each cell per axis
  
  printf("Simulation parameters\n");
  printf("GV.NCELLS:%12d GV.NTOTALCELLS:%12d\n" 
	 "GV.BoxSize:%16.8lf GV.CellSize:%16.8lf\n", 
	 GV.NCELLS,
	 GV.NTOTALCELLS,
	 GV.BoxSize,
	 GV.CellSize);
  
  printf("----------------------------------------------------------------\n");
  printf("Cosmological parameters\n");
  printf("GV.z_RS=%lf GV.H0=%lf \nGV.Hz=%lf GV.a_SF=%lf\n", 
	 GV.z_RS,
	 GV.H0, 
	 GV.Hz, 
	 GV.a_SF);
  printf("-----------------------------------------------------------------\n");
  
  
  /*+++++ Computing PotDot in terms of gravitational potential +++++*/
  PotDot_from_poten();
  
  
  return 0;
}//main
