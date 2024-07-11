// inner shield and sensor mount for TFA Dostmann's Protective cover for outdoor transmitter with solar fan 98.1111
// https://www.tfa-dostmann.de/en/product/protective-cover-for-outdoor-transmitter-with-solar-fan-98-1111/
// fitting to FUEL4EP's AsksinPP Homebrew climate sensor HB-UNI-Sensor1-THP_MA-AHT20_BMP280 and PCB HB-UNI-SEN-BATT_ATMega1284P_E07-868MS10_FRAM_FUEL4EP
// HB-UNI-Sensor1-THP_MA-AHT20_BMP280: https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/sketches/HB-UNI-Sensor1-THP_MA-AHT20_BMP280
// HB-UNI-SEN-BATT_ATMega1284P_E07-868MS10_FRAM_FUEL4EP: https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/PCBs/HB-UNI-SEN-BATT_ATMega1284P_E07-868MS10_FRAM_FUEL4EP
// (C) 05.07.2024 FUEL4EP (Creative Commons)
// https://creativecommons.org/licenses/by-nc-sa/4.0/
// You are free to Share & Adapt under the following terms:
// Give Credit, NonCommercial, ShareAlike

// IMPORTANT: Please adjust the variable 'GapOverlap_Top' below to
// get a proper and not too tight fitting of the cover
// and the bottom box.
// The necessary adjustment may depend on your 3D
// printer's tolerances.
//
// V1.0 10.07.2024 initial version 
// 
// You can highlight an object by putting a '#' in
// front of the generating command
// You can just show one specific object by putting a '!' in
// front of the generating command

$fn = 200;
delta=0.01;

zTotalHeight=168;

// height of the three segments:
// top loft          : zhTopLoft    =  24
// sensor container  : wPCB+2*ewPCB =  75.2+2*0.9
// bottom loft       : zhBottomLoft =  67
// = 67 + 75.2+2*0.9 + 24           = 168

// switches to enable/disable the parts of the drawing
showTopLoft=1;
showBottomLoft=1;
showSensorContainer=1;
showTFAWedge=1;
showHeightCube=0;        // for check purpose only
showOuterPipe=0;         // for check purpose only
showBottomPlate=0;       // for check purpose only

//============================
// for top part, please select
//showTopLoft=1;
//showBottomLoft=0;
//showSensorContainer=0;
//showTFAWedge=0;
//============================

//===============================
// for bottom part, please select
//showTopLoft=0;
//showBottomLoft=1;
//showSensorContainer=1;
//showTFAWedge=1;
//===============================


dPCB=1.8;

hPCB=20;
hPCBandBattery=17.7-dPCB;
ebPCB=0.8;
ewPCB=1.0;
wPCB=75.4;  // height z direction
bPCB=75.2;  // width x direction
dWall=2.5;

hHelper=0.1;
rMAX=77.9/2;
rInnerTFA=80/2;
rDrillHole=2;
RDrillHoleOuter=22;
RDrillHoleInner=13;

zhBottomLoft=67;
rBottom=rMAX-dWall;

zhTopLoft=24;
rTop=60/2;

zOverlap_Top=20;
GapOverlap_Top=0.3;

xTFA_Wedge=32.3;
zTFAWedge=70.2;
ytopTFAWedge=5.2;
ybottomTFAWedge=10.3;
dxTFAWedge=15;

rBottomPlate=67/2;
rHoleBottomPlate=3.5/2;
wFilletBottom_Plate=6;

w_corner_post   = 1.7;
b_corner_post   = 10;
h0_corner_post  = 7;
h1_corner_post  = 2.0;
h2_corner_post  = 4.5;
zo_corner_post  = 3.5;
o_corner_post   = 1;
gap_corner_post = 0.2;

ex_corner_post   = 4.4;
ey_corner_post   = 1.9;
ee_corner_post   = 1.5;

module corner_post()  {
   translate([ex_corner_post-w_corner_post-gap_corner_post,0,-(ey_corner_post-w_corner_post-gap_corner_post)]) {
      linear_extrude(height = b_corner_post, center = false)
      polygon(points=[[0,0],[-ex_corner_post,0],[-ex_corner_post,-h1_corner_post],[ee_corner_post,-h2_corner_post]]);
     translate([-ex_corner_post,0,ey_corner_post-w_corner_post]) cube([w_corner_post,h0_corner_post,b_corner_post-(ey_corner_post-w_corner_post)]); 
   }
}





module wedge(length, width, begin_height, end_height) {
    
// length of the wedge (x axis)
// width of the wedge (z axis)
// height of the wedge at the beginning (y axis)
// height of the wedge at the end (y axis)
{
  linear_extrude(height = width, center = false)
    polygon(points=[[0,0],[length,0],[length,begin_height],[0,end_height],[0,0]]);
  }
}


module outer_pipe(h_outerpipe) {
  difference(){
     translate([0,0,delta]) cylinder(h=h_outerpipe,r=rMAX+dWall,center=true);
     translate([0,0,-delta]) cylinder(h=h_outerpipe+6*delta,r=rMAX,center=true);

  }
}

module TFA_inner_space(h) {
  difference(){
     translate([0,0,delta]) cylinder(h=h,r=rInnerTFA+dWall,center=true);
     translate([0,0,-delta]) cylinder(h=h+6*delta,r=rInnerTFA,center=true);

  }
}

    
    
module PCB_notch() {
  translate([0,0,ewPCB]) cube([bPCB,dPCB,wPCB+ewPCB+delta],center=true);
}


module sensor_container(X,Y,Z) {
  difference(){
    cube([bPCB+2*ebPCB,hPCB+2*dWall,wPCB+2*ewPCB],center=true);
    translate([0,0,-delta]) cube([bPCB+2*ebPCB-2*dWall,hPCB,wPCB+2*ewPCB+3*delta],center=true);
    translate([0,-hPCB/2+dPCB/2+hPCBandBattery,-delta]) PCB_notch();
    outer_pipe(wPCB+5);
  }

}

module loft_rect2circ(X,Y,R,Z) {
  hull(){
      translate([0,0,0]) cube([X,Y,hHelper],center=true);
      translate([0,0,-Z])cylinder(h=hHelper,r=R,center=true);
    };
}

module add_ventilation_holes_bottom_plate() {
  for(variable = [-108 : 360/20 : 108]) 
        rotate([0,0,variable]) {
      translate ([0,-RDrillHoleOuter,-zhBottomLoft+dWall/2-delta]) cylinder(r=rDrillHole,h=dWall+3*delta,center=true);
    }
    for(variable = [-108 : 360/10 : 108]) 
        rotate([0,0,variable]) {
      translate ([0,-RDrillHoleInner,-zhBottomLoft+dWall/2-delta]) cylinder(r=rDrillHole,h=dWall+3*delta,center=true);
    }
}

// top loft
if (showTopLoft) {
  translate ([0,0,0]) { 
    mirror ([0,0,1]) translate ([0,0,-wPCB-2*ewPCB])   difference() {
      loft_rect2circ(bPCB+2*ebPCB,hPCB+2*dWall,rTop+dWall,zhTopLoft);
      translate ([0,0,1*delta]) loft_rect2circ(bPCB+2*ebPCB-2*dWall,hPCB,rTop,zhTopLoft+2*delta);
      translate ([0,0,-zhTopLoft/2+5*delta])outer_pipe(zhTopLoft);
    }
    if (showHeightCube) { 
        translate([0,0,wPCB+2*ewPCB+zhTopLoft-delta]) cylinder(h=delta,r=rTop,center=true);
    }
    
  }
  translate ([0,0,+wPCB+2*ewPCB]) rotate([0,0,90]) mirror([0,0,1]) {
    translate([ -hPCB/2,(bPCB+2*ebPCB-2*dWall)/2,0]) mirror([1,0,0]) rotate([90,0,0])corner_post();
    translate([hPCB/2,(bPCB+2*ebPCB-2*dWall)/2,0]) rotate([90,0,0])corner_post();
    
   translate([ -hPCB/2,-(bPCB+2*ebPCB-2*dWall)/2,0]) mirror([0,0,0]) rotate([90,0,180])corner_post();
   translate([+hPCB/2,-(bPCB+2*ebPCB-2*dWall)/2,0]) mirror([1,0,0]) rotate([90,0,180])corner_post();
}   

}

// sensor container
if (showSensorContainer) {
  translate ([0,0,wPCB/2+ewPCB]) sensor_container(bPCB,hPCB,wPCB);
}

// bottom loft
if (showBottomLoft) {
  translate ([0,0,0]) difference() {
    loft_rect2circ(bPCB+2*ebPCB,hPCB+2*dWall,rBottom+dWall,zhBottomLoft);
  translate ([0,0,delta]) loft_rect2circ(bPCB+2*ebPCB-2*dWall,hPCB,rBottom,zhBottomLoft+2*delta);
  translate ([-xTFA_Wedge/2,dxTFAWedge,-zhBottomLoft+zTFAWedge-delta]) rotate([0,90,0]) translate ([-0,0,-delta]) wedge(zTFAWedge, xTFA_Wedge, ybottomTFAWedge, ytopTFAWedge);
    translate ([0,0,-zhBottomLoft/2+14])outer_pipe(zhBottomLoft);
  }

}

module bottom_plate() {
 difference(){
   translate ([0,0,-zhBottomLoft+dWall/2]) cylinder(h=dWall,r=rBottomPlate, center=true);
   translate ([0,dxTFAWedge+ybottomTFAWedge/2,-zhBottomLoft+dWall/2-delta]) cube([xTFA_Wedge,ybottomTFAWedge,dWall+3*delta],center=true);
   translate ([0,0,-zhBottomLoft+dWall/2-delta]) cylinder(r=rHoleBottomPlate,h=dWall+3*delta,center=true);
   add_ventilation_holes_bottom_plate();
  } 
}

// TFA_wedge
if (showTFAWedge) {
   translate ([-xTFA_Wedge/2,dxTFAWedge,-zhBottomLoft+zTFAWedge]) rotate([0,90,0]) difference(){
      translate ([-dWall,-dWall,-dWall]) wedge(zTFAWedge+dWall, xTFA_Wedge+2*dWall, ybottomTFAWedge+2*dWall, ytopTFAWedge+2*dWall) ;
      translate ([-0,0,-delta]) wedge(zTFAWedge+delta, xTFA_Wedge, ybottomTFAWedge, ytopTFAWedge) ; 
   }  
   bottom_plate();

   difference(){
    union(){
      translate ([0,0,-zhBottomLoft+dWall/2]) cube([2*rBottom,wFilletBottom_Plate,dWall],center=true);
      difference(){
        translate ([0,0,-zhBottomLoft+dWall/2]) rotate([0,0,90]) cube([2*rBottom,wFilletBottom_Plate,dWall],center=true);
        translate ([0,dxTFAWedge+ybottomTFAWedge/2,-zhBottomLoft+dWall/2-delta]) cube([xTFA_Wedge,ybottomTFAWedge,dWall+2*delta],center=true);
     }
   }
   translate ([0,0,-zhBottomLoft+dWall/2-delta]) cylinder(r=rHoleBottomPlate,h=dWall+3*delta,center=true);
   add_ventilation_holes_bottom_plate();
  }
 }
  
  
// for check purposes only
if (showHeightCube) {
  #translate ([0,0,zTotalHeight/2-zhBottomLoft]) cube([10,10,zTotalHeight],center=true);
}

// for check purposes only
if (showBottomPlate) {
  bottom_plate();
}

 


