/**
  ******************************************************************************
  * @file    orocos_cnc.cpp
  * @author  hiep.lchan@gmail.com
	* @date    6-January-2018
  * @brief   Embedded Orocos KDL for 7DOF Robot arm to LinuxCNC project.
  *          This file contains inverse dynamics function to create
  *          dynamics libraries liborocos_cnc.so
  *          using to build LinuxCNC HAL userspace component.
	*/

#include <kdl/chainfksolverpos_recursive.hpp>
#include <kdl/chainidsolver_recursive_newton_euler.hpp>
#include <kdl/chaindynparam.hpp>
#include <kdl/jntspaceinertiamatrix.hpp>
#include <kdl/chain.hpp>

#include <parameter.h>

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

using namespace KDL;
using namespace std;

/*------------MOMENT TYPEDEF OF ROBOT ARM (N.m)------------*/
extern "C" struct momentType
{
	double mm1, mm2, mm3, mm4, mm5, mm6;
};

/*------------OROCOS INVERSE DYNAMICS CALCULATION------------
  * @brief  Calculate moment of Robot Arm from angle positions, velocities, accelerates
  * @param  p1, p2, p3, p4, p5, p6: postions of robot arm (degree).
	* @param  v1, v2, v3, v4, v5, p6: velocities of robot arm (degree/s).
	* @param  a1, a2, a3, a4, a5, a6: accelerates of robot arm (degree/s^2).
  * @retval Momet of each joint (N.m)
  */
extern "C" momentType inverse_dynamic(float p1, float p2, float p3, float p4, float p5, float p6, float p7, float v1, float v2, float v3, float v4, float v5, float v6, float v7, float a1, float a2, float a3, float a4, float a5, float a6, float a7)
{
	//Gravity vector, depend on robot arm D-H parameters
	Vector gravity(gravity_x, gravity_y, gravity_z);

	//Create robot arm
	Chain RRBotKdl = Chain();

  //Create link of robot arm: Mass, Center mass position, Inertia (Ixx, Iyy, Izz, Ixy, Ixz, Iyz)
	RigidBodyInertia inert1 = RigidBodyInertia(link1_mass, Vector(link1_center_x, link1_center_y, link1_center_z), RotationalInertia(link1_Ixx, link1_Iyy, link1_Izz, link1_Ixy, link1_Ixz, link1_Iyz));
	RigidBodyInertia inert2 = RigidBodyInertia(link2_mass, Vector(link2_center_x, link2_center_y, link2_center_z), RotationalInertia(link2_Ixx, link2_Iyy, link2_Izz, link2_Ixy, link2_Ixz, link2_Iyz));
	RigidBodyInertia inert3 = RigidBodyInertia(link3_mass, Vector(link3_center_x, link3_center_y, link3_center_z), RotationalInertia(link3_Ixx, link3_Iyy, link3_Izz, link3_Ixy, link3_Ixz, link3_Iyz));
	RigidBodyInertia inert4 = RigidBodyInertia(link4_mass, Vector(link4_center_x, link4_center_y, link4_center_z), RotationalInertia(link4_Ixx, link4_Iyy, link4_Izz, link4_Ixy, link4_Ixz, link4_Iyz));
	RigidBodyInertia inert5 = RigidBodyInertia(link5_mass, Vector(link5_center_x, link5_center_y, link5_center_z), RotationalInertia(link5_Ixx, link5_Iyy, link5_Izz, link5_Ixy, link5_Ixz, link5_Iyz));
	RigidBodyInertia inert6 = RigidBodyInertia(link6_mass, Vector(link6_center_x, link6_center_y, link6_center_z), RotationalInertia(link6_Ixx, link6_Iyy, link6_Izz, link6_Ixy, link6_Ixz, link6_Iyz));
	RigidBodyInertia inert7 = RigidBodyInertia(link7_mass, Vector(link7_center_x, link7_center_y, link7_center_z), RotationalInertia(link7_Ixx, link7_Iyy, link7_Izz, link7_Ixy, link7_Ixz, link7_Iyz));

  //D-H parameters of robot arm (a, alpha, d, theta)
	Frame frame1 = Frame::DH(link1_DH_a, link1_DH_alpha, link1_DH_d, link1_DH_theta);
	Frame frame2 = Frame::DH(link2_DH_a, link2_DH_alpha, link2_DH_d, link2_DH_theta);
	Frame frame3 = Frame::DH(link3_DH_a, link3_DH_alpha, link3_DH_d, link3_DH_theta);
	Frame frame4 = Frame::DH(link4_DH_a, link4_DH_alpha, link4_DH_d, link4_DH_theta);
	Frame frame5 = Frame::DH(link5_DH_a, link5_DH_alpha, link5_DH_d, link5_DH_theta);
	Frame frame6 = Frame::DH(link6_DH_a, link6_DH_alpha, link6_DH_d, link6_DH_theta);
	Frame frame7 = Frame::DH(link7_DH_a, link7_DH_alpha, link7_DH_d, link7_DH_theta);

  //Create joint of robot arm: rotation or translation
	Joint joint1(Joint::TransZ);
	Joint joint2(Joint::RotZ);
	Joint joint3(Joint::RotZ);
	Joint joint4(Joint::RotZ);
	Joint joint5(Joint::RotZ);
	Joint joint6(Joint::RotZ);
	Joint joint7(Joint::RotZ);

  //Add links and joint to Robot arm
	RRBotKdl.addSegment(Segment(joint1, frame1, inert1));
	RRBotKdl.addSegment(Segment(joint2, frame2, inert2));
	RRBotKdl.addSegment(Segment(joint3, frame3, inert3));
	RRBotKdl.addSegment(Segment(joint4, frame4, inert4));
	RRBotKdl.addSegment(Segment(joint5, frame5, inert5));
	RRBotKdl.addSegment(Segment(joint6, frame6, inert6));
	RRBotKdl.addSegment(Segment(joint7, frame7, inert7));

  //Joint angle position (Orocos using radian instead of degree)
	JntArray jointPos = JntArray(7);
	jointPos(0) = p1*M_PI/180;       // Joint 1
	jointPos(1) = p2*M_PI/180;       // Joint 2
	jointPos(2) = p3*M_PI/180;       // Joint 3
	jointPos(3) = p4*M_PI/180;       // Joint 4
	jointPos(4) = p5*M_PI/180;       // Joint 5
	jointPos(5) = p6*M_PI/180;       // Joint 6
	jointPos(6) = p7*M_PI/180;       // Joint 7

  //Joint angle velocity (Orocos using radian/s instead of degree/s)
	JntArray jointVel = JntArray(7);
	jointVel(0) = v1*M_PI/180;
	jointVel(1) = v2*M_PI/180;
	jointVel(2) = v3*M_PI/180;
	jointVel(3) = v4*M_PI/180;
	jointVel(4) = v5*M_PI/180;
	jointVel(5) = v6*M_PI/180;
	jointVel(6) = v7*M_PI/180;

  //Joint angle accelerate (Orocos using radian/s^2 instead of degree/s^2)
	JntArray jointAcc = JntArray(7);
	jointAcc(0) = a1*M_PI/180;
	jointAcc(1) = a2*M_PI/180;
	jointAcc(2) = a3*M_PI/180;
	jointAcc(3) = a4*M_PI/180;
	jointAcc(4) = a5*M_PI/180;
	jointAcc(5) = a6*M_PI/180;
	jointAcc(6) = a7*M_PI/180;

  //Joint added moment or external force (added mass, friction force...)
	Wrenches jnt_wrenches;
	jnt_wrenches.push_back(Wrench());
	jnt_wrenches.push_back(Wrench());
	jnt_wrenches.push_back(Wrench());
	jnt_wrenches.push_back(Wrench());
	jnt_wrenches.push_back(Wrench());
	jnt_wrenches.push_back(Wrench());
	jnt_wrenches.push_back(Wrench());

  //Moment of each joint
	JntArray jointforce = JntArray(7);

	//Inverse Dynamic using Orocos
	ChainIdSolver_RNE gcSolver = ChainIdSolver_RNE(RRBotKdl, gravity);
	int ret_gc = gcSolver.CartToJnt(jointPos, jointVel, jointAcc, jnt_wrenches, jointforce);

	momentType new_moment;
	new_moment.mm1 = jointforce(0);
	new_moment.mm2 = jointforce(1);
	new_moment.mm3 = jointforce(2);
	new_moment.mm4 = jointforce(3);
	new_moment.mm5 = jointforce(4);
	new_moment.mm6 = jointforce(5);
	new_moment.mm6 = jointforce(6);

	return new_moment;
}
