#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>

#define CONST_G (100.0+0.0)/(pnum+0.0)
#define EPSILON 0.001

typedef struct particles{
    double position_x, position_y, mass, velocity_x, velocity_y, brightness; 
}particle; 

typedef struct forces{
    double x, y; 
}vector2d; 


int main(int argc, char* argv[]){
    // Check if input parameters are sufficient. 
    if(argc!=6){
	printf("Error: Insufficient input parameters! \n"); 
	return 1; 
    }
    // Assign parameters to variables 
    const int pnum = atoi(argv[1]), nsteps = atoi(argv[3]); 
    const float delta_t = atof(argv[4]); 
    const char graphic = atoi(argv[5]); 
    particle* restrict part_ary = malloc(sizeof(particle)*pnum); 
    
    // Read particle data from input file to particle information array
    FILE* fp = fopen(argv[2], "r"); 
    // Check if file pointer is valid and read data into memory
    if(fp==NULL){
	printf("Error: Invalid file to read! \n");
	return 1; 	
    }
    if(fread(part_ary, sizeof(particle), pnum, fp) == pnum){
	fclose(fp);
    }
    else{
	printf("Error: Invalid input particle numbers. \n");
	fclose(fp); 	
	return 1; 
    }

#if 0
    for(int i=0; i<pnum; i++){
    printf("Particle %d: \n", i); 
    printf("%f, %f, %f, %f, %f, %f. \n", part_ary[i].position_x, 
	    part_ary[i].position_y, part_ary[i].mass, part_ary[i].velocity_x, 
	    part_ary[i].velocity_y,part_ary[i].brightness); 
    }
#endif


    //struct timeval allo_begin, allo_end, force_begin, force_end, attr_begin, attr_end; 
    //double allo_time=0, force_time=0, attr_time=0; 
    // Create a matrix(sequence) to store the coefficient of forces between each two particles and initialize all default value as 0
    //gettimeofday(&allo_begin, NULL);
    vector2d* restrict force_mat = (vector2d*)malloc(pnum*pnum*sizeof(vector2d)); 
    //gettimeofday(&allo_end, NULL); 
    //allo_time = (double)(allo_begin.tv_usec - allo_end.tv_usec)/1000+1000*(allo_begin.tv_sec - allo_end.tv_sec); 

    // Loop to update particle array step by step
    vector2d acce[pnum]; 
    for(int step=0; step<nsteps; step++){
	/* printf("Now is step: %d. \n", step); */ 
	double rij, temp_coeffi; 
#if 1
	// To update the force matrix
	//gettimeofday(&force_begin, NULL); 
	for(int i=0; i<pnum; i++){
	    acce[i].x = 0; 
	    acce[i].y = 0; 
	    for(int j=0; j<pnum; j++){
		if(i == j){continue; }
		else{
		// Calculate the value of rij
		/* x_cord = (part_ary[i].position_x - part_ary[j].position_x) * (part_ary[i].position_x - part_ary[j].position_x); */
		/* y_cord = (part_ary[i].position_y - part_ary[j].position_y) * (part_ary[i].position_y - part_ary[j].position_y); */ 
		temp_coeffi = part_ary[j].mass/pow((sqrt(pow((part_ary[i].position_x - part_ary[j].position_x), 2) +
			   pow((part_ary[i].position_y - part_ary[j].position_y), 2)) + EPSILON), 3); 

		/* rij = sqrt(pow((part_ary[i].position_x - part_ary[j].position_x), 2) + */ 
		/* 	   pow((part_ary[i].position_y - part_ary[j].position_y), 2)); */ 
		/* printf("Now rij is: %f. \n", rij); */ 		
		// Calculate the value of non-vector part
		
		/* temp = -CONST_G * part_ary[i].mass * part_ary[j].mass / pow((rij + EPSILON), 3); */ 
		/* temp_coeffi = pow(rij, 3); */
		/* printf("Now temp coefficient is: %f and rij is: %f. \n", temp_coeffi, rij); */ 	
		/* temp_coeffi = -CONST_G / (rij * (x_cord + y_cord) + 3 * rij * EPSILON * */ 
		/*     EPSILON + 3 * (x_cord + y_cord) * EPSILON + EPSILON * EPSILON * EPSILON); */ 
		/* printf("Now const is: %f. \n", temp); */ 
		// Store two coordinates of force vector into force array
		acce[i].x += -CONST_G * temp_coeffi * (part_ary[i].position_x - part_ary[j].position_x); 
		acce[i].y += -CONST_G * temp_coeffi * (part_ary[i].position_y - part_ary[j].position_y);
		/* printf("Now temp coeffi is: %.20f. and force.x and y are: %.20f, %.20f. \n", temp_coeffi, force_mat[i*pnum+j].x, force_mat[i*pnum+j].y); */ 	

		}
	    }
	}
#endif
	//gettimeofday(&force_end, NULL); 
	//force_time += (double)(force_begin.tv_usec - force_end.tv_usec)/1000 + 1000*(force_begin.tv_sec - force_end.tv_sec); 
#if 0
	if(step==1){
	for(int i=0; i<pnum; i++){
	    for(int j=0; j<pnum; j++){
		printf("(%.15f, %.15f) ", force_mat[i*pnum+j].x, force_mat[i*pnum+j].y); 
	    }
	    printf("\n"); }}
	
#endif

#if 1
	// Update attributes of particles in part_ary
	//gettimeofday(&attr_begin, NULL); 
	for(int i=0; i<pnum; i++){
	    part_ary[i].velocity_x += delta_t * acce[i].x;
	    part_ary[i].velocity_y += delta_t * acce[i].y; 	
#else
	    part_ary[i].velocity_x += acc2.x * delta_t;	
	    part_ary[i].velocity_y += acc2.y * delta_t; 
#endif
	    part_ary[i].position_x += delta_t * part_ary[i].velocity_x; 
	    part_ary[i].position_y += delta_t * part_ary[i].velocity_y; 
	}
	//gettimeofday(&attr_end, NULL); 
	//attr_time += (double)(attr_begin.tv_usec - attr_end.tv_usec)/1000 + 1000*(attr_begin.tv_sec - attr_end.tv_sec); 
#if 0
    for(int i=0; i<pnum; i++){
    printf("Particle %d: \n", i); 
    printf("%f, %f, %f, %f, %f, %f. \n", part_ary[i].position_x, 
	    part_ary[i].position_y, part_ary[i].mass, part_ary[i].velocity_x, 
	    part_ary[i].velocity_y,part_ary[i].brightness); 
    }
#endif
    }
#if 1
    fp = fopen("result.gal", "w"); 
    if(fwrite(part_ary, pnum, sizeof(particle), fp) == 0){
	printf("Error: Write file failed! \n"); 
	return 1; 	
     }; 
    fclose(fp); 
    free(part_ary);
    free(force_mat); 
    //printf("Allo: %.3f, Force: %.3f, Attr: %.3f. \n", allo_time, force_time, attr_time);  
#endif
}
