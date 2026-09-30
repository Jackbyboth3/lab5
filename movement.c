/*
 * movement.c
 *
 *  Created on: Sep 16, 2026
 *      Author: jackbyb
 */





#include "open_interface.h"
#include "movement.h"
void move_forward(oi_t*sensor,int centimeters){
    //4 times
    int sum = 0;
    oi_setWheels(200,200);
     //Creates a variable to keep track of how far the robot has traveled.

         //Tells both wheels to go forward at 500 mm/s.
         //positive = foward ; max = 500mm/s



         //thi scontrolls how far it goes
         //50,500 controlls the circle
       while (sum < centimeters*10){
       //    while (sum < centimeters){
           oi_update(sensor);
             //runs, the robot and updates sensor information
           sum+= sensor->distance;
           }
     //stops wheels
       oi_setWheels(0,0);

}
void turn_clockwise(oi_t*sensor, int degrees){
//    oi_update(sensor);

    double sum = 0;

    //left wheel, right wheel
    //left = forward ; right = backward
    oi_setWheels(-120,120);

    while (sum > -degrees){
        oi_update(sensor);
        sum+= sensor->angle;


    }
    oi_setWheels(0,0);
//    oi_free(0,0);
}
