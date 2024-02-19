// battery holder for 2/3AA Mignon NiMH Akku 1,2V 750mAh
// derived from https://github.com/not-enough-time-for-all-these-projects/18650-holder

// credits to  'Not Enough Time For All These Projects'
// (C) 2020 'Not Enough Time For All These Projects' (Creative Commons)
// modified by FUEL4EP to fit to GP75AAH Akku 2/3AA Mignon NiMH Akku 1,2V 750mAh 
//  e.g from https://www.amazon.de/Sparset-GP75AAH-Mignon-750mAh-wiederaufladbar-gr%C3%BCn/dp/B08HM5DL27
// contacts e.g. https://www.amazon.de/sourcing-map-Federplatte-Vernickelt-12mmx12mm/dp/B0B1HJHPZJ/ref=asc_df_B0B1HJHPZJ/
//  requires BOSL library: https://github.com/revarbat/BOSL)

include <BOSL/constants.scad>
use <BOSL/masks.scad>
use <BOSL/transforms.scad>
use <BOSL/metric_screws.scad>

// count of cells
numBatteries = 1;

// BatteryDimensions
// https://en.wikipedia.org/wiki/List_of_battery_sizes#Cylindrical_lithium-ion_rechargeable_battery
// 18650
batteryDiameter = 14.8;
batteryLength = 29.2+2.5;

// variables
wallThick = 3.5;
batterySpacer = wallThick/4;
EPS=0.1;

boxWidth = (batteryDiameter + 2* batterySpacer) * numBatteries + batterySpacer * 2;
boxLength = batteryLength+ 2*(wallThick);
boxHeight = batteryDiameter / 2.0;

contactWidth         = 12.4;
contactHeight        = 12.4;
contactThick         =  0.9;
contactSpringWidth   =  6.4;
contactSlot          =  .8;
contactContactWidth  =  3.3;
contactContactLength =  7;

bracketCubeSize=0;

$fn=120;

module _contactslot() {
  translate([0, 0, contactSlot]) {
    // plate
    cube([contactWidth, contactHeight, contactThick]);
    // slot 
    cube([contactWidth, contactHeight+boxHeight, contactThick+EPS]);
    // spring
    translate([(contactWidth-contactSpringWidth)/2, 0, -contactSlot]) {
      cube([contactSpringWidth, contactHeight+boxHeight, contactSlot]);
    }
    // Contact for the plug
    translate([(contactWidth-contactContactWidth)/2,-contactContactLength,0]) {
      cube(size=[contactContactWidth, contactContactLength, contactThick]);
    }
  }
}

module contactslots() {
  translate([0, wallThick, boxHeight/2]) {
    rotate([90, 0, 0]) _contactslot();
  }
  translate([contactWidth, (boxLength-wallThick), boxHeight/2]) {
    rotate([90, 0, 180]) _contactslot();     
  }
}

module _boltBracket() {
  screwElevation=bracketCubeSize/2-get_metric_bolt_head_height(3);
  difference() {
    cube(size=[bracketCubeSize, bracketCubeSize, bracketCubeSize/2]);
    up(bracketCubeSize) {
      fillet_mask_z(l=bracketCubeSize, r=bracketCubeSize/2, align=V_DOWN);
      back(bracketCubeSize) fillet_mask_z(l=bracketCubeSize, r=bracketCubeSize/2, align=V_DOWN);
    }
    translate([bracketCubeSize/2, bracketCubeSize/2, screwElevation]) {
      rotate([0, 0, 0]) {
      // boltHole(size=3, length=5);   
      // metric_bolt(size=3, l=3, pitch=0);
      screw(screwsize=3.2, screwlen=15, headsize=get_metric_bolt_head_size(3), headlen=get_metric_bolt_head_height(3));
    }
  }

  }
}

module boltBrackets() {
  // screwHoles
  translate([-bracketCubeSize,0,0]) {
    _boltBracket();
  }
  translate([-bracketCubeSize,boxLength-bracketCubeSize,0]) {
    _boltBracket();
  }
  translate([boxWidth, boxLength, 0]) {
    rotate([180,180,0]) {
      translate([-bracketCubeSize,0,0]) {
        _boltBracket();
      }
      translate([-bracketCubeSize,boxLength-bracketCubeSize,0]) {
        _boltBracket();
      }
    }
  }
}

module outerWalls() {
  // outer walls
  thick=wallThick; 

  difference() {
    cube([boxWidth, thick, batteryDiameter]);
    up(batteryDiameter) rotate([90, 0, 0]) {
      fillet_mask_z(l=thick, r=batteryDiameter/2, align=V_DOWN);
      right(boxWidth) fillet_mask_z(l=thick, r=batteryDiameter/2, align=V_DOWN);

    }
  }

  translate([0, boxLength-wallThick, 0]) {
    difference() {
      cube([boxWidth, thick, batteryDiameter]);
      up(batteryDiameter) rotate([90, 0, 0]) {
        fillet_mask_z(l=thick, r=batteryDiameter/2, align=V_DOWN);
        right(boxWidth) fillet_mask_z(l=thick, r=batteryDiameter/2, align=V_DOWN);
      }
    }
  }
}
    
module BatteryHolder() {
  difference() {
    // baseplate
    cube([boxWidth, boxLength, boxHeight]); 

    // battery cutout
    for (i=[0:numBatteries-1]) {
      translate([i * (batteryDiameter+2*batterySpacer) + batteryDiameter/2+batterySpacer*2, boxLength, /*batteryDiameter/2+wallThick */boxHeight-.07]) {
        rotate([90, 0, 0]) {
          cylinder(h=boxLength,d=batteryDiameter);     
        }              
      }
    }
  } //difference basePlate
  outerWalls();
  //boltBrackets();
}

difference() {
  BatteryHolder();
  // cut out contact slots
  for (i=[0:numBatteries-1]) {
    translate([i*(batteryDiameter+batterySpacer*2)+(boxWidth-contactWidth)/2, 0, 0]) {
      contactslots();
    }
  }
}

 /* for (i=[0:numBatteries-1]) {
    translate([i*(batteryDiameter+batterySpacer*2)+(boxWidth-contactWidth)/2, 0, 0]) {
      contactslots();
    }
} */
