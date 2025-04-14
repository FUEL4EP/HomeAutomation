// remix of https://www.thingiverse.com/thing:2411898
// (C)Inhumierer June 30, 2017
// my credits for Inhumierer

// V 1.0 (C) FUEL4EP 05/2025 

// IMPORTANT: Please adjust the variable 'gap' below to
// get a proper and not too tight fitting of the cover
// and the bottom INNERBOX.
// The necessary adjustment may depend on your 3D
// printer's tolerances.
//

// put a # in front of a command in order to identify the concerning created object

use <HB_Stamp_IO_ATMega1284P_FUEL4EP_INNERBOX.scad>
use <2_3_AA_NiMH-akku-holder_with_USB_C_charger_module.scad>

debug=0;

$fn=50;

wINNERBOX=24.6;
lINNERBOX=42.6;
b=0.5;            // border distance innner wall to INNERBOX

wi=wINNERBOX+b;	  // inner width, length & heigth
li=lINNERBOX+b;
th=2;	// wall thickness
h=14.5+18.1+2*th+3.1-3.5+7-2.0;
r=1;	// radius of rounded corners
opening_help=true;	// make a gap to ease opening of the cover, f.ex.
		// with a coin - girls are afraid of their finger nails ;-)
e=0.01;
ri=(r>th)?r-th:e;	// needed for the cover - needs to be larger than 0 for proper results
l=li-2*r;
w=wi-2*r;
delta=0.001;

th_top=1.5;

margin=0.2;
wPCBEXT=22 + margin;
lPCBEXT=27 + margin;

w_outer=24.6;
l_outer=42.6;
h_outer= 22.0-1.29-2.0; //18.71
d_outer=(w_outer-wPCBEXT)/2;  //24.6-22.2=2.4

h_cover=3.9;


show_INNERBOX=false;           // set to true for checking purpose
show_BATTERYBOX=false;         // set to true for checking purpose
show_OuterCase=true;          // set to false for test printing of cover only purpose
show_Cover=true;               // set to false for test printing of OuterCase only purpose               
// venting slits for sensor

n_slits=2;
h_slits=8;
zo_slits=3;
inc_slits=3.5;
w_slits=1.5;
y_center_slits=-l_outer/2+10;



d_usb_c=3.5;
w_usb_c=9-d_usb_c;
zo_usb_c=5.0-3.5+1.5;
xo_usb_c=0.4+0.1;

d_LED=3.1;
yo_LED=2.7+2.54/2;
zo_LED=2.4-3.5+1.5+0.5;


module OuterCase(){
    
 if ( debug == 1 ){
	  translate([-0.0,0,0]) color("pink") cylinder(50,r=0.2,center=true);
    translate([-w/2-r,-l/2-r,0]) color("red") cylinder(50,r=0.1,center=true);
    translate([0,-l/2-r,0]) color("blue") cylinder(50,r=0.1,center=true);
    translate([w/2+r,-l/2-r,0]) color("red") cylinder(50,r=0.1,center=true);
    translate([0,-l/2,0]) color("orange") rotate([90,0,0]) cylinder(50,r=0.1,center=true);
	  translate([0,-l/2-r,0]) color("pink") rotate([0,90,0]) cylinder(50,r=0.1,center=true);
 }
	difference(){
     
  union() {

		  translate([0,0,-th_top])hull(){
		  	for (i=[[-w/2,-l/2],[-w/2,l/2],[w/2,-l/2],[w/2,l/2]]){
	  			translate(i)cylinder(r=r+th,h=h+th_top,$fn=32*r);
		  	}
	  	}
  }
		hull(){
			for (i=[[-w/2,-l/2],[-w/2,l/2],[w/2,-l/2],[w/2,l/2]]){
				translate(i)cylinder(r=r,h=h+delta,$fn=32*r);
			}
  }
  translate([-w/2-r,-l/2-r,0]) cube([wi,li,h]);
	
	translate([-w/2,l/2+r,h-2])rotate([0,90,0])cylinder(d=1.2,h=w,$fn=24);
	translate([-w/2,-l/2-r,h-2])rotate([0,90,0])cylinder(d=1.2,h=w,$fn=24);
	translate([w/2+r,l/2,h-2])rotate([90,0,0])cylinder(d=1.2,h=l,$fn=24);
	translate([-w/2-r,l/2,h-2])rotate([90,0,0])cylinder(d=1.2,h=l,$fn=24);



	// if you need some adjustment for the opening helper size or position,
	// this is the right place
	if (opening_help)translate([0,l/2+11.9,h-1.7])cylinder(d=20,h=1.7,$fn=32);
			
  translate([-wINNERBOX/2+(d_outer+margin/2),-lINNERBOX/2+(d_outer+margin/2),-th_top]) inner_holes(th_top);	

// venilation slits 
   translate([0,(w_outer-wPCBEXT)/2,0]) {
        for ( i=[-n_slits:1:n_slits-1] ){   
          translate([wi/2-delta,i*inc_slits+y_center_slits,zo_slits])
            cube([th+2*delta,w_slits,h_slits]);
          translate([-wi/2-delta-th,i*inc_slits+y_center_slits,zo_slits])
            cube([th+2*delta,w_slits,h_slits]);
				}
  }
  
// USB C opening
  
  translate([wi/2-th-xo_usb_c,+li/2+th,h-w_usb_c-d_usb_c/2-zo_usb_c-th-3.1]) rotate([90,0,0]) hull() {
      cylinder(r=d_usb_c/2,h=th,$fn=32*r);
      translate([0,w_usb_c,0]) cylinder(r=d_usb_c/2,h=th,$fn=32*r);
    }
      
      
// charging LED opening
   
  translate([w/2+r,l/2+d_LED/2-yo_LED,h-zo_LED-th-3.1]) rotate([0,90,0]) cylinder(r=d_LED/2,h=th,$fn=32*r);
  translate([-0.1+w/2+r,l/2+d_LED/2-yo_LED,h-zo_LED-th-3.1]) rotate([0,0,0]) cylinder(r=d_LED/2,h=7.5-2.0+0.5,$fn=32*r);       
    
	}

}  

module cover(){
    
gap=0.2;  // added FUEL4EP after test print, cover was too tight wiithout additional gap
    
	translate([0,0,-th_top])hull(){
		for (i=[[-w/2,-l/2],[-w/2,l/2],[w/2,-l/2],[w/2,l/2]]){
			translate(i)cylinder(r=r+th,h=th_top,$fn=32*r);
		}
	}
	difference(){
		translate([0,0,-th_top])hull(){
			for (i=[[-w/2+gap,-l/2+gap],[-w/2+gap,l/2-gap],[w/2-gap,-l/2+gap],[w/2-gap,l/2-gap]]){
				translate(i)cylinder(r=r,h=th_top+h_cover,$fn=32*r);
			}
		}
		hull(){
			for (i=[[-w/2+gap,-l/2+gap],[-w/2+gap,l/2-gap],[w/2-gap,-l/2+gap],[w/2-gap,l/2-gap]]){
				if (r>th_top){
					translate(i)cylinder(r=r-th_top,h=3+h_cover,$fn=32*r);
				}else{
					#translate(i)cylinder(r=e,h=h_cover,$fn=32*r);
				}
			}
		}
	} 
	translate([-w/2+1+gap,l/2-gap+r-0.2,2])rotate([0,90,0])cylinder(d=1.2,h=w-2-2*gap,$fn=24);
	translate([-w/2+1+gap,-l/2+gap-r+0.2,2])rotate([0,90,0])cylinder(d=1.2,h=w-2-2*gap,$fn=24);
	translate([w/2-gap+r-0.2,l/2-1-gap,2])rotate([90,0,0])cylinder(d=1.2,h=l-2-2*gap,$fn=24);
	translate([-w/2+gap-r+0.2,l/2-1-gap,2])rotate([90,0,0])cylinder(d=1.2,h=l-2-2*gap,$fn=24);
}



if ( show_OuterCase == true ) {
// outer case
	OuterCase();
}

if ( show_Cover == true ) {
// cover
	translate([0,li+3+2*th,0]) cover();
}

if ( show_INNERBOX == true ) {
  
  translate([-wINNERBOX/2,-lINNERBOX/2,0]) color("blue") inner_box();  
	//translate([0,0,0]) inner_box(); 
}

if ( show_BATTERYBOX == true ) {
  
  translate([-wINNERBOX/2,-lINNERBOX/2,h_outer]) color("red") NiMH_akku_supply();  
	
	
}
// outer dimenions
echo("width = ",wi+2*th);
echo("length = ",li+2*th);
echo("height = ",h+2*th);
