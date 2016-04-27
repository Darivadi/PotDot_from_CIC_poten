/****************************************************************************************************
NAME: growth_rate_OmegaL0
FUNCTION: Computes the growth rate f(t) for first linear approx. proportional to Omega_Lambda0
INPUT: Scale factor
RETURN: Growth rate f(t) for first approx. proportional to Omega_Lambda0
****************************************************************************************************/

double growth_rate_OmegaL0(double a_SF)
{ 
  double GR_OmegaL0, a_cube;  
  a_cube = pow(a_SF, 3.0);
  GR_OmegaL0 = 1.0/( pow( (1.0+GV.Omega_L0*a_cube), 0.6) );

  printf("-----------------------------------------------------------------\n");
  printf("First approximation to f(t)\n");
  printf("OmegaL0=%lf,  growth rate f(t)=%lf\n", 
	 GV.Omega_L0, GR_OmegaL0);
  printf("-----------------------------------------------------------------\n");

  return GR_OmegaL0;
  
}//growth_rate_app1


/****************************************************************************************************
NAME: growth_rate_OmegaM
FUNCTION: Computes the growth rate f(t) for second linear approx. proportional to Omega_M(a)
INPUT: Scale factor
RETURN: Growth rate f(t) for second linear approx. proportional to Omega_M(a)
****************************************************************************************************/

double growth_rate_OmegaM(double a_SF)
{ 
  double OmegaM_ofa, mu, GR_OmegaM, z;
  
  mu = a_SF * pow((GV.Omega_L0/GV.Omega_M0), 1.0/3.0);
  OmegaM_ofa = GV.Omega_M0 / ( (double) (1 + pow(mu, 3.0)) ); 
  GR_OmegaM = pow(OmegaM_ofa, 0.6);

  
  printf("-----------------------------------------------------------------\n");
  printf("Second approximation to f(t)\n");
  printf("mu=%lf, OmegaM(a)=%lf, growth rate f(t)=%lf\n", 
	 mu, OmegaM_ofa, GR_OmegaM);
  printf("-----------------------------------------------------------------\n");
  
  return GR_OmegaM;
}//growth_rate_app2


/****************************************************************************************************
NAME: PotDot_from_poten
FUNCTION: Computes PotDot from known values of gravitational potential obtained from a CIC mass 
assignment scheme. This is taken from Watson et al 2014.
INPUT: None
RETURN: 0
****************************************************************************************************/

int PotDot_from_poten(void)
{
  int m, i, j, k;
  double pos_aux[3];
  double fn_app1, fn_app2;
  FILE *pf1 = NULL, *pf2 = NULL;

  /*----- Computing the approximations to the linear growth rate f -----*/
  fn_app1 = 1.0 - ( growth_rate_OmegaL0(GV.a_SF) );
  fn_app2 = 1.0 - ( growth_rate_OmegaM(GV.a_SF) );  
  
  printf("GR_OmegaL0=%lf GR_OmegaM=%lf a_SF=%lf\n", 
	 growth_rate_OmegaL0(GV.a_SF), growth_rate_OmegaM(GV.a_SF), GV.a_SF);
  printf("---------------------------------------\n");
  

  for(m=0; m<GV.NTOTALCELLS; m++)
    {
      gp[m].potDot_app1 = -1.0 * gp[m].potential * GV.Hz * ( 1.0 - fn_app1 );
      gp[m].potDot_app2 = -1.0 * gp[m].potential * GV.Hz * ( 1.0 - fn_app2 );
    }//for m
  
    printf("Saving data in binary file for both approximations\n");
  printf("--------------------------\n");


  pf1 = fopen("./../../Processed_data/PotDot_from_poten_app1.bin", "w");
  pf2 = fopen("./../../Processed_data/PotDot_from_poten_app2.bin", "w");
  
  /*+++++ Saving Simulation parameters +++++*/
  fwrite(&GV.BoxSize, sizeof(double), 1, pf1);  // Box Size
  fwrite(&GV.Omega_M0, sizeof(double), 1, pf1);  // Matter density parameter
  fwrite(&GV.Omega_L0, sizeof(double), 1, pf1);  // Cosmological constant density parameter
  fwrite(&GV.z_RS, sizeof(double), 1, pf1);  // Redshift
  fwrite(&GV.H0, sizeof(double), 1, pf1);  // Hubble parameter

  fwrite(&GV.BoxSize, sizeof(double), 1, pf2);  // Box Size
  fwrite(&GV.Omega_M0, sizeof(double), 1, pf2);  // Matter density parameter
  fwrite(&GV.Omega_L0, sizeof(double), 1, pf2);  // Cosmological constant density parameter
  fwrite(&GV.z_RS, sizeof(double), 1, pf2);  // Redshift
  fwrite(&GV.H0, sizeof(double), 1, pf2);  // Hubble parameter

  for(i=0; i<GV.NCELLS; i++)  
    {
      for(j=0; j<GV.NCELLS; j++)
	{
	  for(k=0; k<GV.NCELLS; k++)
	    {
	      m = INDEX_C_ORDER(i,j,k);
	      pos_aux[X] = (i + 0.5) * GV.CellSize;
	      pos_aux[Y] = (j + 0.5) * GV.CellSize;
	      pos_aux[Z] = (k + 0.5) * GV.CellSize;
	      
	      fwrite(&pos_aux[0], sizeof(double), 3, pf1);
	      fwrite(&gp[m].potDot_app1, sizeof(double), 1, pf1);

	      fwrite(&pos_aux[0], sizeof(double), 3, pf2);
	      fwrite(&gp[m].potDot_app2, sizeof(double), 1, pf2);
	    }//for k	  
	}//for j
    }//for i
  
  fclose(pf1);
  fclose(pf2);

  return 0;
  
}//PotDot_from_poten
