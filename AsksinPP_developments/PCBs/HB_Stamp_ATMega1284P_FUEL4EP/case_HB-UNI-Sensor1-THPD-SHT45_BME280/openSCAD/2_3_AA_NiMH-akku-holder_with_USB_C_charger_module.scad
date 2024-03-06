// battery holder for 2/3 AA Mignon NiMH Akku 1,2V 750mAh
// with NiMH charger module "1S 2S 3S 4S NiMH battery charging board type-c micro usb 5v input charger module charging current 500mA"

// battery holder derived from https://github.com/not-enough-time-for-all-these-projects/18650-holder
// credits to  'Not Enough Time For All These Projects'
// modified by FUEL4EP to fit to GP75AAH Akku 2/3AA Mignon NiMH Akku 1.2V 750mAh 
//  e.g from https://www.amazon.de/Sparset-GP75AAH-Mignon-750mAh-wiederaufladbar-gr%C3%BCn/dp/B08HM5DL27
// contacts e.g. https://www.amazon.de/sourcing-map-Federplatte-Vernickelt-12mmx12mm/dp/B0B1HJHPZJ/ref=asc_df_B0B1HJHPZJ/
//  fitting NiMH charging module: https://de.aliexpress.com/item/1005005777972846.html
include  <2_3_AA_NiMH-akku-holder.scad>

module NiMH_akku_supply(){

hConnector=3;
wChargingModule=17.1;
lChargingModule=24.0;
dChargingModule=4.3;
dWall=1;
hLEDChargingModule=6.9;
x_offset_LED_ChargingModule=1.5;
y_offset_LED_ChargingModule=2.4;
dLEDChargingModule=3.1;
hUSB_C=3.5;
wUSB_C=9;
x1_USB_C=3.5;
x2_USB_C=5.4;
wWiringChannel_1=2;
lWiringChannel_1=boxLength-2*dWall;
yoWiringChannel_1=dWall;
wWiringChannel_2=2;
lWiringChannel_2=(boxWidth)/2;
yoWiringChannel_2=(boxWidth-lWiringChannel_1)/2;
wWiringChannel_3=4;
lWiringChannel_3=3;
wWiringChannel_4=wWiringChannel_2;
lWiringChannel_4=11+hConnector+dWall;
wWiringChannel_5=dWall+dChargingModule-wWiringChannel_4;
lWiringChannel_5=wWiringChannel_2;
lLEDOpening=6;
hLEDOpening=4;

xFill=24.6;
yFill=3.9;
hFill=wChargingModule+dWall;

difference(){
  translate([0,0,-hConnector]) rotate([0,0,0]) cube([dChargingModule+2*dWall,boxLength,wChargingModule+dWall]);

  translate([dWall,boxLength-lChargingModule,-hConnector+dWall]) rotate([0,0,0]) cube([dChargingModule,lChargingModule,wChargingModule]);
  translate([dWall+dChargingModule-wWiringChannel_4,boxLength-lChargingModule-wWiringChannel_4,-hConnector]) rotate([0,0,0]) cube([wWiringChannel_4,wWiringChannel_4,lWiringChannel_4]);
   #translate([0,boxLength-lChargingModule-wWiringChannel_4,-hConnector]) rotate([0,0,0]) cube([wWiringChannel_5,lWiringChannel_5,hConnector]);
   #translate([dChargingModule+dWall,boxLength-lLEDOpening,-hConnector+wChargingModule-hLEDOpening+dWall]) rotate([0,0,0]) cube([dWall,lLEDOpening,hLEDOpening]);
}

translate([-boxWidth,0,0]) rotate([0,0,0]) 2_3_A_NiMH_akku_holder();

difference(){
  translate([-boxWidth,0,-hConnector]) rotate([0,0,0]) cube([boxWidth,boxLength,hConnector]);
  translate([-wWiringChannel_1,yoWiringChannel_1,-hConnector]) rotate([0,0,0]) cube([wWiringChannel_1,lWiringChannel_1,hConnector]);
  translate([-wWiringChannel_1-lWiringChannel_2,yoWiringChannel_1,-hConnector]) rotate([0,0,0]) cube([lWiringChannel_2,wWiringChannel_2,hConnector]);
  translate([-wWiringChannel_1-lWiringChannel_2,boxLength-dWall-wWiringChannel_2,-hConnector]) rotate([0,0,0]) cube([lWiringChannel_2,wWiringChannel_2,hConnector]);
  translate([-wWiringChannel_1-lWiringChannel_2,boxLength-lWiringChannel_3,-hConnector]) rotate([0,0,0]) cube([wWiringChannel_3,lWiringChannel_3,hConnector]);
  translate([-wWiringChannel_1-lWiringChannel_2,0,-hConnector]) rotate([0,0,0]) cube([wWiringChannel_3,lWiringChannel_3,hConnector]);
}

translate([-xFill++2*dWall+dChargingModule,-yFill,-hConnector]) rotate([0,0,0]) cube([xFill,yFill,hFill]);

echo("width = ",boxWidth+2*dWall+dChargingModule);
echo("length = ",boxLength+yFill);
echo("height = ",wChargingModule+dWall);

}

NiMH_akku_supply();