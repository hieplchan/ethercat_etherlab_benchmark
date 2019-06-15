/**
  ******************************************************************************
  * @file    parameter.h
  * @author  hiep.lchan@gmail.com
  * @date    6-January-2018
  * @brief   Embedded Orocos KDL for 7DOF Robot arm to LinuxCNC project.
  *          This file contains parameter config of your 7DOF robot arm.
	*/

/*------------GRAVITY VECTOR OF YOUR D-H PARAMETERS------------*/
#define gravity_x 0
#define gravity_y 10
#define gravity_z 0


/*------------PARAMETERS OF 6DOF ROBOT LINK------------*/

/*------------MASS OF LINK*------------*/
#define link1_mass 1
#define link2_mass 1
#define link3_mass 1
#define link4_mass 1
#define link5_mass 1
#define link6_mass 1
#define link7_mass 1

/*------------CENTER MASS OF LINK------------*/
//Link 1
#define link1_center_x  0
#define link1_center_y  0
#define link1_center_z  0
//Link 2
#define link2_center_x  -0.3638
#define link2_center_y  0.006
#define link2_center_z  0.2275
//Link 3
#define link3_center_x  -0.0203
#define link3_center_y  -0.0141
#define link3_center_z  -0.070
//Link 4
#define link4_center_x  0
#define link4_center_y  0.019
#define link4_center_z  0
//Link 5
#define link5_center_x  0
#define link5_center_y  0
#define link5_center_z  0
//Link 6
#define link6_center_x  0
#define link6_center_y  0
#define link6_center_z  0.032
//Link 7
#define link7_center_x  0
#define link7_center_y  0
#define link7_center_z  0.032

/*------------NERTIA OF LINK------------*/
//Link 1
#define link1_Ixx 0
#define link1_Iyy 0
#define link1_Izz 0
#define link1_Ixy 0
#define link1_Ixz 0
#define link1_Iyz 0
//Link 2
#define link2_Ixx 0.13
#define link2_Iyy 0.524
#define link2_Izz 0.539
#define link2_Ixy 0
#define link2_Ixz 0
#define link2_Iyz 0
//Link 3
#define link3_Ixx 0.066
#define link3_Iyy 0.086
#define link3_Izz 0.0125
#define link3_Ixy 0
#define link3_Ixz 0
#define link3_Iyz 0
//Link 4
#define link4_Ixx 10.8e-3
#define link4_Iyy 10.8e-3
#define link4_Izz 10.8e-3
#define link4_Ixy 0
#define link4_Ixz 0
#define link4_Iyz 0
//Link 5
#define link5_Ixx 0.3e-3
#define link5_Iyy 0.4e-3
#define link5_Izz 0.3e-3
#define link5_Ixy 0
#define link5_Ixz 0
#define link5_Iyz 0
//Link 6
#define link6_Ixx 0.15e-3
#define link6_Iyy 0.15e-3
#define link6_Izz 0.04e-3
#define link6_Ixy 0
#define link6_Ixz 0
#define link6_Iyz 0
//Link 7
#define link7_Ixx 0.15e-3
#define link7_Iyy 0.15e-3
#define link7_Izz 0.04e-3
#define link7_Ixy 0
#define link7_Ixz 0
#define link7_Iyz 0

/*------------D-H PARAMETERS OF LINK------------*/
//Link 1
#define link1_DH_a      0
#define link1_DH_alpha  M_PI/2
#define link1_DH_d      0
#define link1_DH_theta  0
//Link 2
#define link2_DH_a      0
#define link2_DH_alpha  M_PI/2
#define link2_DH_d      0.7
#define link2_DH_theta  0
//Link 3
#define link3_DH_a      1.2
#define link3_DH_alpha  0
#define link3_DH_d      0
#define link3_DH_theta  M_PI/2
//Link 4
#define link4_DH_a      0
#define link4_DH_alpha  M_PI/2
#define link4_DH_d      0
#define link4_DH_theta  0
//Link 5
#define link5_DH_a      0
#define link5_DH_alpha  -M_PI/2
#define link5_DH_d      0.43
#define link5_DH_theta  0
//Link 6
#define link6_DH_a      0
#define link6_DH_alpha  M_PI/2
#define link6_DH_d      0
#define link6_DH_theta  0
//Link 7
#define link7_DH_a      0
#define link7_DH_alpha  0
#define link7_DH_d      0.25
#define link7_DH_theta  0
