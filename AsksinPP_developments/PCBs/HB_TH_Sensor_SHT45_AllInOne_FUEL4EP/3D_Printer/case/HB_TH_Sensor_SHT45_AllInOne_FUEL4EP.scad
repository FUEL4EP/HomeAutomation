// remix of https://www.thingiverse.com/thing:2411898
// (C)Inhumierer June 30, 2017
// my credits for Inhumierer

// V 1.3 adaption to HB_TH_Sensor_SHT45_AllInOne_FUEL4EP by FUEL4EP 01/2024
// V 1.2 Correction / hint from molotok3D, some minor fixes
// V 1.1- added opening helper and an optional separating wall 

// IMPORTANT: Please adjust the variable 'gap' below to
// get a proper and not too tight fitting of the cover
// and the bottom box.
// The necessary adjustment may depend on your 3D
// printer's tolerances.

wPCB=25;
lPCB=75;
b=3;       // border distance innner wall to PCB

wi=wPCB+b;	// inner width, length & heigth
li=lPCB+b;
h=25;
th=2;	// wall thickness
r=3;	// radius of rounded corners
opening_help=true;	// make a gap to ease opening of the cover, f.ex.
		// with a coin - girls are afraid of their finger nails ;-)
separator=0;	// generate a separating wall inside - set to 0 for none

e=0.01;
ri=(r>th)?r-th:e;	// needed for the cover - needs to be larger than 0 for proper results
l=li-2*r;
w=wi-2*r;
delta=0.001;


// PCB feet coordinates and parameters


x1_PCB_feet=wPCB/2-1.778;
y1_PCB_feet=-lPCB/2+59.817;

x2_PCB_feet=wPCB/2-23.079;
y2_PCB_feet=-lPCB/2+59.817;

x3_PCB_feet=wPCB/2-1.778;
y3_PCB_feet=-lPCB/2+20.447;

x4_PCB_feet=wPCB/2-5.588;
y4_PCB_feet=-lPCB/2+2.175;

h_PCB_feet=4;             //height
d_PCB_feet=3.5;           //diameter
screw_dia_PCB_feet=1.2;   //diameter of screws

th_PCB=0.8;               // PCB thickness

show_PCB=false;           // set to true for checking purpose

// venting slits for sensor

n_slits=2;
h_slits=2*h_PCB_feet;
y_center_slits=-lPCB/2+14.214;
w_slits=1.5;
inc_slits=3;

module box(){
	difference(){
		translate([0,0,-th])hull(){
			for (i=[[-w/2,-l/2],[-w/2,l/2],[w/2,-l/2],[w/2,l/2]]){
				translate(i)cylinder(r=r+th,h=h+th,$fn=8*r);
			}
		}
		hull(){
			for (i=[[-w/2,-l/2],[-w/2,l/2,],[w/2,-l/2],[w/2,l/2]]){
				translate(i)cylinder(r=r,h=h+delta,$fn=8*r);
			}
		}
		translate([-w/2,l/2+r,h-2])rotate([0,90,0])cylinder(d=1.2,h=w,$fn=12);
		translate([-w/2,-l/2-r,h-2])rotate([0,90,0])cylinder(d=1.2,h=w,$fn=12);
		translate([w/2+r,l/2,h-2])rotate([90,0,0])cylinder(d=1.2,h=l,$fn=12);
		translate([-w/2-r,l/2,h-2])rotate([90,0,0])cylinder(d=1.2,h=l,$fn=12);

		// if you need some adjustment for the opening helper size or position,
		// this is the right place
		if (opening_help)translate([w/2-10,l/2+13.5,h-1.8])cylinder(d=20,h=10,$fn=32);

// venilation slits 
     
        for ( i=[-n_slits:1:n_slits-1] ){   
          translate([wi/2+th/2+delta,i*inc_slits+y_center_slits,h_slits/2])
            cube([th+2*delta,w_slits,h_slits],center=true);
          translate([-wi/2-th/2-delta,i*inc_slits+y_center_slits,h_slits/2])
            cube([th+2*delta,w_slits,h_slits],center=true);
          translate([wi/2-h_slits/2,i*inc_slits+y_center_slits,-th/2-delta])
            cube([h_slits,w_slits,th+2*delta],center=true);      translate([-wi/2+h_slits/2,i*inc_slits+y_center_slits,-th/2-delta])
            cube([h_slits,w_slits,th+2*delta],center=true);
        }
    
	}
    

	if (separator>0){
		translate([separator-wi/2,-li/2-e,-e])difference(){
			cube([th,li+2*e,h]);
			translate([-e,-e,h-3])cube([th+2*e,2*th+2+2*e,5]);
			translate([-e,e+li-2*th-2,h-3])cube([th+2+2*e,2*th+2+2*e,5]);
		}
	}
}

module cover(){
    
    gap=0.15;  // added FUEL4EP after test print, cover was too tight wiithout additional gap
    
	translate([0,0,-th])hull(){
		for (i=[[-w/2,-l/2],[-w/2,l/2],[w/2,-l/2],[w/2,l/2]]){
			translate(i)cylinder(r=r+th,h=th,$fn=8*r);
		}
	}
	difference(){
		translate([0,0,-th])hull(){
			for (i=[[-w/2+gap,-l/2+gap],[-w/2+gap,l/2-gap],[w/2-gap,-l/2+gap],[w/2-gap,l/2-gap]]){
				translate(i)cylinder(r=r,h=th+3,$fn=8*r);
			}
		}
		hull(){
			for (i=[[-w/2+gap,-l/2+gap],[-w/2+gap,l/2-gap],[w/2-gap,-l/2+gap],[w/2-gap,l/2-gap]]){
				if (r>th){
					translate(i)cylinder(r=r-th,h=3,$fn=8*r);
				}else{
					translate(i)cylinder(r=e,h=3,$fn=8*r);
				}
			}
		}
	} 
	translate([-w/2+1+gap,l/2-gap+r-0.2,2])rotate([0,90,0])cylinder(d=1.2,h=w-2-2*gap,$fn=12);
	translate([-w/2+1+gap,-l/2+gap-r+0.2,2])rotate([0,90,0])cylinder(d=1.2,h=w-2-2*gap,$fn=12);
	translate([w/2-gap+r-0.2,l/2-1-gap,2])rotate([90,0,0])cylinder(d=1.2,h=l-2-2*gap,$fn=12);
	translate([-w/2+gap-r+0.2,l/2-1-gap,2])rotate([90,0,0])cylinder(d=1.2,h=l-2-2*gap,$fn=12);

}

// PCBfeets

module PcbFeets(h_PCB_feet, d_PCB_feet, screw_dia_PCB_feet, x1_PCB_feet, y1_PCB_feet, x2_PCB_feet, y2_PCB_feet,x3_PCB_feet, y3_PCB_feet, x4_PCB_feet, y4_PCB_feet){
    translate([x1_PCB_feet,y1_PCB_feet,th]) {
        difference() {
            cylinder(d=d_PCB_feet, h=h_PCB_feet, center=true, $fs=0.1);
            cylinder(d=screw_dia_PCB_feet, h=h_PCB_feet+.01, $fs=0.1,center=true);
        }
    }
    translate([x2_PCB_feet,y2_PCB_feet,th]) {
        difference() {
            cylinder(d=d_PCB_feet, h=h_PCB_feet, center=true, $fs=0.1);
            cylinder(d=screw_dia_PCB_feet, h=h_PCB_feet+.01, center=true, $fs=0.1);
        }
    }
    translate([x3_PCB_feet,y3_PCB_feet,th]) {
        difference() {
            cylinder(d=d_PCB_feet, h=h_PCB_feet, center=true, $fs=0.1);
            cylinder(d=screw_dia_PCB_feet, h=h_PCB_feet+.01, center=true, $fs=0.1);
        }
    }
    translate([x4_PCB_feet,y4_PCB_feet,th]) {
        difference() {
            cylinder(d=d_PCB_feet, h=h_PCB_feet, center=true, $fs=0.1);
            cylinder(d=screw_dia_PCB_feet, h=h_PCB_feet+.01, center=true, $fs=0.1);
        }
    }
}





box();
translate([0,li+3+2*th,0])
	cover();
translate([0,0,0])
  PcbFeets(h_PCB_feet, d_PCB_feet, screw_dia_PCB_feet, x1_PCB_feet, y1_PCB_feet, x2_PCB_feet, y2_PCB_feet,x3_PCB_feet, y3_PCB_feet, x4_PCB_feet, y4_PCB_feet);
if(show_PCB==true) {
rotate([0,180,0]) 
  translate([0,0,-h_PCB_feet-th_PCB])
    import("HB_TH_Sensor_SHT45_AllInOne_FUEL4EP_mod.stl");
}