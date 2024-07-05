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
// V1.0 initial version

$fn = 100;
delta=0.02;


// switches to enable/disable the parts of the drawing
showTopLoft=1;
showBottomLoft=0;
showSensorContainer=0;
showTFAWedge=0;

hPCB=21;
ehPCB=5;
wPCB=75.2;
bPCB=75.2;
dWall=2;

hHelper=0.1;

zhBottomLoft=75;
rBottom=90/2;

zhTopLoft=25;
rTop=60/2; 

dPCB=1.7;


wxPCBSlide=2;
wyPCBSlide=2;
dyPCBSlide=hPCB/2-wyPCBSlide-dPCB/2-ehPCB;

zWedge=3*wxPCBSlide;



zOverlap_Top=20;
GapOverlap_Top=0.2;

xTFA_Wedge=32.2;
zTFAWedge=70.2;
ytopTFAWedge=5.2;
ybottomTFAWedge=10.2;
dxTFAWedge=15;

rBottomPlate=70/2;
rHoleBottomPlate=3.5/2;
wFilletBottom_Plate=6;

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


module PCB_single_slide(X,Y,Z) {
  translate ([0,0,-Z/2]) cube([X,Y,Z]);  
}

module PCB_single_conductor(X,Y,Z){
    translate ([0,dPCB/2,0]) PCB_single_slide(wxPCBSlide,wyPCBSlide,Z);
    translate ([0,-dPCB/2-wyPCBSlide,0]) PCB_single_slide(wxPCBSlide,wyPCBSlide,Z);
    translate ([wxPCBSlide,dPCB/2+wyPCBSlide,-Z/2]) rotate([270,0,180]) wedge(wxPCBSlide,2*wyPCBSlide+dPCB,0,wyPCBSlide);
}

module sensor_container(X,Y,Z) {
  translate ([0,0,0]) {
    difference(){
      cube([X+2*dWall,Y+2*dWall,Z],center=true);
      translate ([0,0,-delta]) difference(){
      cube([X,Y,Z+3*delta],center=true);
      }
    }
  }
  translate ([X/2-wxPCBSlide,dyPCBSlide,0]) PCB_single_conductor(0,0,wPCB);
  mirror([1,0,0]) translate ([X/2-wxPCBSlide,dyPCBSlide,0]) PCB_single_conductor(0,0,wPCB);

}



module loft_rect2circ(X,Y,R,Z) {
  hull(){
      translate([0,0,0]) cube([X,Y,hHelper],center=true);
      translate([0,0,-Z])cylinder(h=hHelper,r=R,center=true);
    }; 
}

module overlap(X,Y,Z,Gap){
  difference() {
    cube([X+4*dWall+2*Gap,Y+4*dWall+2*Gap,Z],center=true);
    translate ([0,0,-delta]) cube([X+2*dWall+2*Gap,Y+2*dWall+2*Gap,Z+3*delta],center=true); 
  }
}


// bottom loft
if (showBottomLoft) {
  translate ([0,0,0]) difference() {
    loft_rect2circ(wPCB+2*dWall,hPCB+ehPCB+2*dWall,rBottom+dWall,zhBottomLoft);
  translate ([0,0,delta]) loft_rect2circ(wPCB,hPCB+ehPCB,rBottom,zhBottomLoft+2*delta); 
  }
}

// top loft
if (showTopLoft) {
  mirror ([0,0,1]) translate ([0,0,-wPCB]) difference() {
      loft_rect2circ(wPCB+4*dWall+2*GapOverlap_Top,hPCB+ehPCB+4*dWall+2*GapOverlap_Top,rTop+dWall,zhTopLoft);
      translate ([0,0,delta]) loft_rect2circ(wPCB+dWall,hPCB+ehPCB+dWall,rTop,zhTopLoft+2*delta); 
  }
  #translate ([0,0,+wPCB-zOverlap_Top/2]) overlap(wPCB,hPCB+ehPCB,zOverlap_Top,GapOverlap_Top);
}

// sensor container
if (showSensorContainer) {
  translate ([0,0,wPCB/2]) sensor_container(bPCB,hPCB+ehPCB,wPCB);
}

// TFA_wedge
if (showTFAWedge) {
   translate ([-xTFA_Wedge/2,dxTFAWedge,-zhBottomLoft+zTFAWedge]) rotate([0,90,0]) difference(){
      translate ([-dWall,-dWall,-dWall]) wedge(zTFAWedge+dWall, xTFA_Wedge+2*dWall, ybottomTFAWedge+2*dWall, ytopTFAWedge+2*dWall) ;
      translate ([-0,0,-delta]) wedge(zTFAWedge, xTFA_Wedge, ybottomTFAWedge, ytopTFAWedge) ; 
   }  
 difference(){
   translate ([0,0,-zhBottomLoft+dWall/2]) cylinder(h=dWall,r=rBottomPlate, center=true);
   translate ([0,dxTFAWedge+ybottomTFAWedge/2,-zhBottomLoft+dWall/2-delta]) cube([xTFA_Wedge,ybottomTFAWedge,dWall+2*delta],center=true);
   translate ([0,0,-zhBottomLoft+dWall/2-delta]) cylinder(r=rHoleBottomPlate,h=dWall+2*delta,center=true);
  }

  difference(){
    union(){
      translate ([0,0,-zhBottomLoft+dWall/2]) cube([2*rBottom,wFilletBottom_Plate,dWall],center=true);
      difference(){
        translate ([0,0,-zhBottomLoft+dWall/2]) rotate([0,0,90]) cube([2*rBottom,wFilletBottom_Plate,dWall],center=true);
        translate ([0,dxTFAWedge+ybottomTFAWedge/2,-zhBottomLoft+dWall/2-delta]) cube([xTFA_Wedge,ybottomTFAWedge,dWall+2*delta],center=true);
     }
   }
   translate ([0,0,-zhBottomLoft+dWall/2-delta]) cylinder(r=rHoleBottomPlate,h=dWall+2*delta,center=true);
  }
  
}

