/*****************************************************************
                            STRUCTURES
******************************************************************/

struct grid
{  

  /*+++ Potential and its time derivative +++*/
  double potential;   // Gravitational potential from the CIC assignment scheme
  double potDot_app1; /* PotDot obtained from potential with the first functional form of the 
		       linear growth rate proportional only to Omega_L0 */
  double potDot_app2; /* PotDot obtained from potential with the second functional form of the 
			 linear growth rate proportional only to Omega_M(a) */
}*gp; //grid


struct GlobalVariables
{
  char FILENAME[1000]; //Path of the data file
  
  /*+++ Grid constants +++*/
  double BoxSize;      // Size of the simulation box in one axis (all must be the same)
  int NCELLS;       // Number of cells in one axis
  int NTOTALCELLS;  // Total number of cell
  double CellSize;  // Size of the cell
  double ZERO;      // Zero for the computer
 
  /*+++ Cosmological Parameters +++*/
  double H0;      //= 100.0 Hubble's constant in the inner units
  double z_RS;    // = 0.0 Redshift of the simulation
  double a_SF;    // Scale factor's time derivative
  double Hz;      //Hubble's parameter a_dot/a
  double Omega_M0; //= 0.258 Density parameter of matter
  double Omega_L0; //= 0.742 Density parameter of cosmological constant
  double MeanDen; // MeanDens;= 7.160809 Units *1E10 M_Sun/h

}GV;//globalVariables


/***************************************************************
                       DEFINITIONS
 ***************************************************************/
#define X 0
#define Y 1
#define Z 2
#define INDEX_C_ORDER(i,j,k) (k)+GV.NCELLS*((j)+GV.NCELLS*(i)) //Index in C-order
