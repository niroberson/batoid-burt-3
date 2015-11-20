						  /**********************************************************
						  /**********************************************************
node motion based on simple beam deflection equation
compiled UDF
**********************************************************/
#include "udf.h"
#include "math.h"

double Amax = .1; /*Maximum Amplitude */
double lambda = 100;
double w = 2.9;/*angular frequency Temporal Angular Frequency*/




				double CalcPos(double upper, double lower, double time, Node *v)
				{
					double pos;
				 pos = (Amax*((upper)*sin(2*M_PI/lambda*(upper) - w*time)-(lower)*sin(2*M_PI/lambda*(lower) - w*time))/(upper-lower) * NODE_X(v) + Amax*(lower)*sin(2*M_PI/lambda*(lower) - w*time) - Amax*((upper)*sin(2*M_PI/lambda*(upper) - w*time)-(lower)*sin(2*M_PI/lambda*(lower) - w*time))/(upper -lower)*(lower))*(1-exp(-time)) ;
				
				return pos;
				}







DEFINE_GRID_MOTION(fin,domain,dt,time,dtime)
{
	Thread *tf = DT_THREAD(dt); /* Threads come out of macro DT_THREAD */
	/* tf is the variable with type thread with a pointer*/
	/* The "thread" variable is tf */
	/* tf is a pointer to the thread, the asterisk tells me tf is a pointer (a sticky note
versus a very large file) */
	/* dt is an argument which is passed into the DT_THREAD macro */ 
	/* dt stands for dynamic thread */
	/* Fluent calls DEFINE_GRID_MOTION (a UDF) that defines the grid motion. When fluent calls
	DEFINE_GRID_MOTION, it supplies fin, domain, dt, time, dtime */
	face_t f;
	/* f is a variable of type face_t, which is a face in our geometry, fin */
	Node *v;
	/* The variable, v is a node pointer */
	/* If v was just a node, it would hold the actual data of the node. A node pointer holds the 
	address for where the data of the node is held */
	real NV_VEC(posNew), NV_VEC(axis), NV_VEC(dx), NV_VEC(posOld);
	real NV_VEC(origin), NV_VEC(rvec);
	real sign;
	

	
	
	
	int counter = 0;
	int n;
	
	
	
	
	/* set deforming flag on adjacent cell zone */
	SET_DEFORMING_THREAD_FLAG(THREAD_T0(tf));
	sign = 0;
	/*Message ("time = %f, omega = %f\n", time, sign);*/
	/*NV_S(omega, =, 0.0);*/
	NV_D(axis, =, 0.0, 1.0, 0.0);
	NV_D(origin, =, 0.0, 0.0, 0);
	begin_f_loop(f,tf) /* Loop over every face, f in thread, tf */
	{
		f_node_loop(f,tf,n) /* Loop over every node, n, in face, f */
		/* For every face, define the integer value of every node composing the face */
		{
			v = F_NODE(f,tf,n); /* v is our node variable. Fill my variable v with the data of
			the nth node. F_NODE is a macro that executes this. */

			// Only update if not previously updated
			if(NODE_POS_NEED_UPDATE(v))
			{
				//if(counter == 0) {
					//Message ("Stay2!");
				}
				counter++;
				/* indicate that node position has been update
				so that it's not updated more than once */
				NODE_POS_UPDATED(v); /* Tells fluent that something has changed with the node */
				/*if (NODE_X(v) == 15) 
				{
					posNew[1] = 55.4;
					
				posNew[1] =  55.4; 
				if(time > dtime) {
					posOld[1] = 55.4; 
				} else {
					posOld[1] = 0;
				}				
				}*/
				
			




				if (NODE_X(v) <= 29 && NODE_X(v) >= 15)
				{
				
				
				posNew[1] =  CalcPos(29,15,time,v); 
				if(time > dtime) {
					posOld[1] = CalcPos(29,15,time-dtime,v); 
				} else {
					posOld[1] = 0;
				}				
				}
				
				
				if (NODE_X(v) <= 43 && NODE_X(v) > 29 )
				{
				
					posNew[1] = CalcPos(43,29,time,v);
				if(time > dtime) {
				posOld[1] = CalcPos(43,29,(time-dtime),v);
				} else {
					posOld[1] = 0;
				}				
				
				}
			
				if (NODE_X(v) <= 57 && NODE_X(v) > 43)
				{
				
					posNew[1] = CalcPos(57,43,time,v);
				if(time > dtime) {
				posOld[1] = CalcPos(57,43,(time-dtime),v);
				} else {
					posOld[1] = 0;
				}				
				
				}
				if (NODE_X(v) <= 71 && NODE_X(v) > 57 )
				{
				
					posNew[1] = CalcPos(71,57,time,v);
				if(time > dtime) {
				posOld[1] = CalcPos(71,57,(time-dtime),v);
				} else {
					posOld[1] = 0;
				}				
				
				}
				
				if (NODE_X(v) <= 85 && NODE_X(v) > 71 )
				{
				
					posNew[1] = CalcPos(85,71,time,v);
				if(time > dtime) {
				posOld[1] = CalcPos(85,71,(time-dtime),v);
				} else {
					posOld[1] = 0;
				}				
				
				}
				
				if (NODE_X(v) <= 99 && NODE_X(v) > 85 )
				{
				
					posNew[1] = CalcPos(99,85,time,v);
				if(time > dtime) {
				posOld[1] = CalcPos(99,85,(time-dtime),v);
				} else {
					posOld[1] = 0;
				}				
				
				}
				
				if ( NODE_X(v) > 99 )
				{
				
					posNew[1] = CalcPos(115,99,time,v);
				if(time > dtime) {
				posOld[1] = CalcPos(115,99,(time-dtime),v);
				} else {
					posOld[1] = 0;
				}				
				
				}
				

				
				
				
				
				
			posOld[1] = NODE_Y(v) - posOld[1]; // The original position (prone to numerical error)
						
				NV_D(dx, =, NODE_X(v), posNew[1] + posOld[1], NODE_Z(v)); // dx is a 3D vector with x and z coords initialized as 0
				NV_V(NODE_COORD(v), =, dx); /* x + dx */ 
				
				
				
				
				
			}
		}		
	}
	end_f_loop(f,tf);
	
	
