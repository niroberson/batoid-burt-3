/**********************************************************
node motion based on simple beam deflection equation
compiled UDF
**********************************************************/
#include "udf.h"
#include "math.h"
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
	real NV_VEC(origin), NV_VEC(rvec), NV_VEC(pt1),NV_VEC(pt2);
	double div1 = -0.2;
	double div2 = 0.0;
	double div3 = 0.2;
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
				if(counter == 0) {
					//Message ("Processing node with (%f,%f,%f) and time = %f  (dtime = %f)\n",NODE_X(v),NODE_Y(v),NODE_Z(v),time,dtime);
				}
				counter++;
				/* indicate that node position has been update
				so that it's not updated more than once */
				NODE_POS_UPDATED(v); /* Tells fluent that something has changed with the node */
				
				if(NODE_X(v)<div2) {
				pt1[1]=(div1+0.2)*0.25*sin(2*M_PI/0.335*div1-15*time)*(1-exp(-time));
				pt2[1]=(div2+0.2)*0.25*sin(2*M_PI/0.335*div2-15*time)*(1-exp(-time));
				posNew[1] = (pt2[1]-pt1[2])/(div2-div1)*(NODE_X(v)-div1)+pt1[1];
				}
				
				else if (NODE_X(v)>div2) {
				pt1[1]=(div2+0.2)*0.25*sin(2*M_PI/0.335*div2-15*time)*(1-exp(-time));
				pt2[1]=(div3+0.2)*0.25*sin(2*M_PI/0.335*div3-15*time)*(1-exp(-time));
				posNew[1] = (pt2[1]-pt1[2])/(div3-div2)*(NODE_X(v)-div2)+pt1[1];
				}
				
				if(time > dtime) {
				
				if(NODE_X(v)<0.0) {
				pt1[1]=(div1+0.2)*0.25*sin(2*M_PI/0.335*div1-15*(time-dtime))*(1-exp(-(time-dtime)));
				pt2[1]=(div2+0.2)*0.25*sin(2*M_PI/0.335*div2-15*(time-dtime))*(1-exp(-(time-dtime)));
				posOld[1] = (pt2[1]-pt1[2])/(div2-div1)*(NODE_X(v)-div1)+pt1[1];
				}
				
				else if (NODE_X(v)>0.0) {
				pt1[1]=(div2+0.2)*0.25*sin(2*M_PI/0.335*div2-15*(time-dtime))*(1-exp(-(time-dtime)));
				pt2[1]=(div3+0.2)*0.25*sin(2*M_PI/0.335*div3-15*(time-dtime))*(1-exp(-(time-dtime)));
				posOld[1] = (pt2[1]-pt1[2])/(div3-div2)*(NODE_X(v)-div2)+pt1[1];
				}
				
				} else {
					posOld[1] = 0;
				}	
				
				posOld[1] = NODE_Y(v) - posOld[1]; // The original position (prone to numerical error)
						
				NV_D(dx, =, NODE_X(v), posNew[1] + posOld[1], NODE_Z(v)); // dx is a 3D vector with x and z coords initialized as 0
				NV_V(NODE_COORD(v), =, dx); /* x + dx */ 
			}
		}		
	}
	end_f_loop(f,tf);
	
	counter = 0;
}
