// remix of https://www.thingiverse.com/thing:2411898
// (C)Inhumierer June 30, 2017
// my credits for Inhumierer

// V 1.3 adaption to HB_TH_Sensor_SHT45_BME280_FUEL4EP by FUEL4EP 02/2024
// V 1.2 Correction / hint from molotok3D, some minor fixes
// V 1.1- added opening helper and an optional separating wall 

// IMPORTANT: Please adjust the variable 'gap' below to
// get a proper and not too tight fitting of the cover
// and the bottom box.
// The necessary adjustment may depend on your 3D
// printer's tolerances.

$fa = 0.05;
$fs=0.05;
wPCB=22.2;
lPCB=40.2;
b=1.6;       

wSensor=22.2;
lSensor=17.2;

gap_device=0.8;

wi=wPCB+b+gap_device;	
li=lPCB+b+gap_device;

e=0.01;

delta=0.001;


th_PCB=5.5;              // PCB thickness with assembled components
th_Sensor=3;             // PCB thickness with assembled components
h_pin_header=3;
d_Sensor=3;


h_PCB_holder=th_PCB+th_Sensor+h_pin_header+d_Sensor;
cutoutSensor=h_PCB_holder;

wNose=5;
hNose=1.5;
lNose=2;

show_PCB=false;           // set to true for checking purpose

// venting slits for sensor

n_slits=2;
h_slits=8;
zo_slits=1;
y_center_slits=10;
w_slits=1.5;
inc_slits=3.5;


module PCB_holder(x,y,z) {

  difference(){
    translate([x,y,z])rotate([0,0,0]) cube([wi,li,h_PCB_holder]);
    difference() {
          translate([x+b/2,y+b/2,z])rotate([0,0,0]) cube([wSensor+gap_device+2*delta,lSensor+gap_device+2*delta,cutoutSensor+2*delta]);
      rotate([0,0,0]) 
        translate([wi/2-wNose/2,b/2,cutoutSensor-th_PCB/2-hNose])
          cube([wNose,lNose,hNose]);
    }
    translate([x+b/2,y+b/2,z+h_PCB_holder-th_PCB/2+delta])rotate([0,0,0]) cube([wPCB+gap_device,lPCB+gap_device,th_PCB/2+2*delta]);
    translate([wi/2-wPCB/2-gap_device/2,lSensor+gap_device+b/2,cutoutSensor-th_PCB/2+delta]) rotate([0,0,0]) cube([wPCB+gap_device,lPCB-(lSensor+b/4),th_PCB/2]);
    translate([wi/2-wPCB/2+gap_device/4,lSensor+gap_device+b/2,cutoutSensor-th_PCB+delta]) rotate([0,0,0]) cube([wPCB-gap_device/2,lPCB-(lSensor+b/4+gap_device/2),th_PCB/2]);
    
    
// venilation slits 
    translate([0,0,zo_slits]) { 
      for ( i=[-n_slits:1:n_slits-1] ){   
        translate([-delta,i*inc_slits+y_center_slits,0])
            cube([b/2+2*delta,w_slits,h_slits]);
        translate([wi-b/2-delta,i*inc_slits+y_center_slits,0])
            cube([b/2+2*delta,w_slits,h_slits]);
    
     }
   }
   
 }
 if (show_PCB==true) {
   d_device=0;  
   translate([x+b/2+gap_device/2,y+b/2+gap_device/2,z+h_PCB_holder-th_PCB+d_device])rotate([0,0,0]) cube([wPCB,lPCB,th_PCB]);
   translate([x+b/2+gap_device/2,y+b/2+gap_device/2,z+h_PCB_holder-th_PCB+d_device-h_pin_header-th_Sensor]) rotate([0,0,0]) cube([wSensor,lSensor,th_Sensor]);
  }
}

PCB_holder(0,0,0);
echo("width = ",wi);
echo("length = ",li);
echo("height = ",h_PCB_holder);


