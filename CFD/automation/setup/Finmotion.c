						  /**********************************************************
node motion based on simple beam deflection equation
compiled UDF
**********************************************************/
#include "udf.h"
#include "math.h"

double Amax =  0.06737878869;
double lambda = 0.54864;
double w = 8;


double CalcPos(double upper, double lower, double time, Node *v)
{
	double pos;
 pos = (Amax*((upper)*sin(2*M_PI/lambda*(upper) - w*time)-(lower)*sin(2*M_PI/lambda*(lower) - w*time))/(upper-lower) * (NODE_X(v)) + Amax*(lower)*sin(2*M_PI/lambda*(lower) - w*time) - Amax*((upper)*sin(2*M_PI/lambda*(upper) - w*time)-(lower)*sin(2*M_PI/lambda*(lower) - w*time))/(upper -lower)*(lower))*(1-exp(-time)) ;

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

			
			if(NODE_POS_NEED_UPDATE(v))
			{
		
			
				/* indicate that node position has been update
				so that it's not updated more than once */
				NODE_POS_UPDATED(v); /* Tells fluent that something has changed with the node */
				
				if (NODE_X(v) <= .2032 && NODE_X(v) >= .127)
				{
				
				
				posNew[1] =  CalcPos(.2032,.127,time,v); 
				if(time > dtime) {
					posOld[1] = CalcPos(.2032,.127,time-dtime,v); 
				} else {
					posOld[1] = 0;
				}				
				}
				
				
				if (NODE_X(v) <= .2794 && NODE_X(v) > .2032 )
				{
				
					posNew[1] = CalcPos(.2794,.2032,time,v);
				if(time > dtime) {
				posOld[1] = CalcPos(.2794,.2032,(time-dtime),v);
				} else {
					posOld[1] = 0;
				}				
				
				}
			
				if (NODE_X(v) <= .3556 && NODE_X(v) > .2794)
				{
				
					posNew[1] = CalcPos(.3556,.2794,time,v);
				if(time > dtime) {
				posOld[1] = CalcPos(.3556,.2794,(time-dtime),v);
				} else {
					posOld[1] = 0;
				}				
				
				}
				if (NODE_X(v) <= .4318 && NODE_X(v) > .3556 )
				{
				
					posNew[1] = CalcPos(.4318,.3556,time,v);
				if(time > dtime) {
				posOld[1] = CalcPos(.4318,.3556,(time-dtime),v);
				} else {
					posOld[1] = 0;
				}				
				
				}
				
				if (NODE_X(v) <= .5334 && NODE_X(v) > .4318 )
				{
				
					posNew[1] = CalcPos(.5334,.4318,time,v);
				if(time > dtime) {
				posOld[1] = CalcPos(.5334,.4318,(time-dtime),v);
				} else {
					posOld[1] = 0;
				}				
				
				}
				
				if (NODE_X(v) <= .6096 && NODE_X(v) > .5334 )
				{
				
					posNew[1] = CalcPos(.6096,.5334,time,v);
				if(time > dtime) {
				posOld[1] = CalcPos(.6096,.5334,(time-dtime),v);
				} else {
					posOld[1] = 0;
				}				
				
				}
				
				if ( NODE_X(v) <= .67564 && NODE_X(v) > .6096 )
				{
				
					posNew[1] = CalcPos(.67564,.6096,time,v);
				if(time > dtime) {
				posOld[1] = CalcPos(.67564,.6096,(time-dtime),v);
				} else {
					posOld[1] = 0;
				}				
				
				}
				

		
			posOld[1] = NODE_Y(v) - posOld[1]; 
						
				NV_D(dx, =, NODE_X(v), posNew[1] + posOld[1], NODE_Z(v)); 
				NV_V(NODE_COORD(v), =, dx); /* x + dx */ 
					
				
			}
		}		
	}
	end_f_loop(f,tf);
	
}

