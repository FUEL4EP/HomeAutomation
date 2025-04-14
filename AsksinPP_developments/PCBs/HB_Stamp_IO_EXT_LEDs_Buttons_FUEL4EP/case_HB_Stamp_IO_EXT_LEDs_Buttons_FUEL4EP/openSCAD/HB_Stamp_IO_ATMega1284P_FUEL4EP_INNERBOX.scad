// remix of https://www.thingiverse.com/thing:2411898
// (C)Inhumierer June 30, 2017
// my credits for Inhumierer

// V 1.0 (C) FUEL4EP 05/2025

// IMPORTANT: Please adjust the variable 'gap' below to
// get a proper and not too tight fitting of the cover
// and the bottom box.
// The necessary adjustment may depend on your 3D
// printer's tolerances.

//$fa = 0.05;
//$fs=0.05;


debug_i=0;

$fn=200;
delta=0.001;
th=0.01;
margin=0.2;
rm=0.15;
wPCBEXT=22 + margin;
lPCBEXT=27 + margin;
b=0.5; 
b2=7.3;
d=0.5;

w_outer=24.6;
l_outer=42.6;
h_outer=22.0-1.29-2.0; //18.71
d_outer=(w_outer-wPCBEXT)/2;


d_Stamp_PCB=0.7;
h_Stamp_PCB=3.0;
zo_Stamp_PCB=17.0-1.29-h_Stamp_PCB;

x1 = 6.8;
x2 = 15.2;
y1 = 4.54;
y2 = 12.54;
y3 = 19;
y4 = 24;

r1= 5.8/2 + rm;
r2= 3.0/2 + rm;
r3= 4.0/2 + rm;

b3=0.5;


n_slits=2;
h_slits=8;
zo_slits=3;
y_center_slits=-l_outer/2+10;
w_slits=1.5;
inc_slits=3.5;
wi=w_outer;
th_s=d_outer+d;

show_LED_solder_helper=false;   // set to true for creating the LED solder helper

module ventilation_slits() {
    
  // venting slits for sensor

  for ( i=[-n_slits:1:n_slits-1] ){   
     translate([wi/2-delta-d_outer,i*inc_slits+y_center_slits,zo_slits])
     cube([th_s+2*delta,w_slits,h_slits]);
     translate([-wi/2-delta+d,i*inc_slits+y_center_slits,zo_slits])
     cube([th_s+2*delta,w_slits,h_slits]);
  }
}


module pyramidal_frustum(w,l,b,h) {
    hull() {
        cube([w, l, th]);
        translate([b/2, b/2, h-th]) cube([(w-b), (l-b), th]);
    }
}



module framed_wedge(w,l,d,h) {
  difference(){
    cube([w,l,h]);
    translate([0, 0, -2*delta]) pyramidal_frustum(w,l,d,h+5*delta);
  }
    
}


module inner_holes(h) {
    translate([x1,y1,h/2-delta]) cylinder(h+5*delta,r=r1,center=true);
    translate([x2,y1,h/2-delta]) cylinder(h+5*delta,r=r1,center=true);
    translate([x1,y2,h/2-delta]) cylinder(h+5*delta,r=r1,center=true);
    translate([x2,y2,h/2-delta]) cylinder(h+5*delta,r=r1,center=true);
    
    translate([x1,y3,h/2-delta]) cylinder(h+5*delta,r=r2,center=true);
    translate([x2,y3,h/2-delta]) cylinder(h+5*delta,r=r2,center=true);
    translate([x1,y4,h/2-delta]) cylinder(h+5*delta,r=r2,center=true);
    translate([x2,y4,h/2-delta]) cylinder(h+5*delta,r=r2,center=true);
    /*
    translate([x1,y3,(b-b3)+b3/2-delta]) cylinder(b3+5*delta,r=r3,center=true);
    translate([x2,y3,(b-b3)+b3/2-delta]) cylinder(b3+5*delta,r=r3,center=true);
    translate([x1,y4,(b-b3)+b3/2-delta]) cylinder(b3+5*delta,r=r3,center=true);
    translate([x2,y4,(b-b3)+b3/2-delta]) cylinder(b3+5*delta,r=r3,center=true);*/
}

module inner_box(){
	
if ( debug_i == 1 ) {
	color("pink") rotate([0,0,0]) cylinder(60,r=0.1,center=true);
	translate([w_outer,0,0]) color("pink") rotate([0,0,0]) cylinder(60,r=0.1,center=true);
	translate([0,l_outer,0]) color("pink") rotate([0,0,0]) cylinder(60,r=0.1,center=true);
	translate([w_outer,l_outer,0]) color("pink") rotate([0,0,0]) cylinder(60,r=0.1,center=true);
}
    
translate([d_outer+margin/2,d_outer+margin/2,0]) {

difference(){
union(){
    if ( debug_i == 1 ) {
			  translate([0,0,0]){
        translate([x1,y1,0]) color("blue") rotate([90,0,0]) cylinder(50,r=0.1,center=true);
        translate([wPCBEXT/2-margin/2,y1,0]) color("blue") rotate([90,0,0]) cylinder(50,r=0.1,center=true);
        translate([-d_outer-margin/2,y1,0]) color("blue") rotate([90,0,0]) cylinder(50,r=0.1,center=true);
        translate([wPCBEXT+d_outer-margin/2,y1,0]) color("blue") rotate([90,0,0]) cylinder(50,r=0.1,center=true);
        translate([wPCBEXT/2,-margin/2-d_outer,0]) color("orange") rotate([0,90,0]) cylinder(50,r=0.1,center=true);
        translate([wPCBEXT/2,10-2*inc_slits-margin/2,zo_slits]) color("green") rotate([0,90,0]) cylinder(50,r=0.1,center=true);
        translate([x1,y1,b/2-delta]) cylinder(50,r=0.1,center=true);
        translate([wPCBEXT/2-margin/2,0,b/2-delta]) cylinder(50,r=0.1,center=true);
        translate([wPCBEXT-margin/2,-margin/2,b/2-delta]) cylinder(50,r=0.1,center=true);
				translate([-margin/2,-margin/2,b/2-delta]) cylinder(50,r=0.1,center=true);
        translate([0,lPCBEXT,b/2-delta]) cylinder(50,r=0.1,center=true);
        translate([wPCBEXT/2,lPCBEXT,b/2-delta]) cylinder(50,r=0.1,center=true);
        translate([wPCBEXT,lPCBEXT,b/2-delta]) cylinder(50,r=0.1,center=true);
				translate([10,-margin/2-d_outer,h_outer]) color("brown") rotate([0,90,0]) cylinder(50,r=0.1,center=true);
				}
    }

difference(){
    translate([-margin/2,-margin/2,0]) cube([wPCBEXT,lPCBEXT,b]);
    inner_holes(b);
}

difference(){
   translate([-margin/2,-margin/2,b]) cube([wPCBEXT,lPCBEXT,b2]); 
   translate([-margin/2+d,-margin/2+d,b-delta]) cube([wPCBEXT-2*d,lPCBEXT-2*d,b2+2*delta]); 
}

if (show_LED_solder_helper != true) {
  difference(){
    translate([-margin/2-d_outer,-margin/2-d_outer,0]) cube([w_outer,l_outer,h_outer]);
    translate([-margin/2,-margin/2,-delta]) cube([w_outer-2*d_outer,l_outer-2*d_outer,h_outer+3*delta]);
  }

  translate([-margin/2,-margin/2,zo_Stamp_PCB]) framed_wedge(w_outer-2*d_outer,l_outer-2*d_outer,d_Stamp_PCB,h_Stamp_PCB);

}

}

if (show_LED_solder_helper != true) {
  translate([w_outer/2-d_outer-d-margin/2,l_outer/2-margin/2,0]) ventilation_slits();

  translate([-margin/2,-margin/2+2*d_Stamp_PCB,zo_Stamp_PCB]) cube([w_outer-2*d_outer,lPCBEXT,h_Stamp_PCB]);
}
}

}
}

inner_box();