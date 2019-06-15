/**
  ******************************************************************************
  * @file    orocos_cnc.h
  * @author  hiep.lchan@gmail.com
  * @date    4-November-2017
	* @brief   Embedded Orocos KDL for 7DOF Robot arm to LinuxCNC project.
  *          This file create function's header convert from C++ to C
  *          inverse dynamics using Orocos.
	*/

/*------------MOMENT TYPEDEF OF ROBOT ARM (N.m)------------*/
struct momentType
{
	double mm1, mm2, mm3, mm4, mm5, mm6, mm7;
};

/*------------OROCOS INVERSE DYNAMICS CALCULATION------------
  * @brief  Calculate moment of Robot Arm from angle positions, velocities, accelerates
  * @param  p1, p2, p3, p4, p5, p6: postions of robot arm (degree).
	* @param  v1, v2, v3, v4, v5, p6: velocities of robot arm (degree/s).
	* @param  a1, a2, a3, a4, a5, a6: accelerates of robot arm (degree/s^2).
  * @retval Momet of each joint (N.m)
  */
struct momentType inverse_dynamic(float p1, float p2, float p3, float p4, float p5, float p6, float p7, float v1, float v2, float v3, float v4, float v5, float v6, float v7, float a1, float a2, float a3, float a4, float a5, float a6, float a7);
