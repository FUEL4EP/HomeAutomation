/* Box design originally by:
////////////////////////////////////////////////////////////////////
              -    FB Aka Heartman/Hearty 2016     -
              -   http://heartygfx.blogspot.com    -
              -       OpenScad Parametric Box      -
              -         CC BY-NC 3.0 License       -
              - adaptions 2023-07-29 (C) FUEL4EP   -
////////////////////////////////////////////////////////////////////

Improved by jbebel:
http://github.com/jbebel/Ultimate-Box-Maker
Adapted to case for HB-UNI-SenAct-4-4-SC_DS_FUEL4EP by FUEL4EP
https://github.com/FUEL4EP/HomeAutomation

V1.1 with notches for power plug hole

To create a box, start by modifying the numerical parameters in the sections
below. This can be Dized, so if you double all the non-feature parameters
you will double the box size in every dimension. Certain parameters are derived
from other parameters. If you wish to override them, you may, but sensible
defaults have been chosen. Notably the design in this revision is particularly
PCB-centric, in that you should start with your PCB size and adjust the margins
around it to determine the box size. If you care more about the box size, you
can set the Length, Width, and Height explicitly, but read the comments around
them.

Once your box is sized appropriately, you can use the Panel modules to design
the holes and text for the front and back panels. Helper variables are provided
to assist you in positioning these holes relative to the PCB, if your holes are
for PCB-mounted components.

When you are ready to print, adjust the values in the "STL element to export"
section, and export each part one at a time.

Experimental options are provided for a screwless design, but these are
untested. In particular, the box fixation tabs may need thicknesses adjusted
in order to have the appropriate flexibility.
*/

/* For STL generation please set below one of to '1':

[STL element to export] 
// - Top shell
TShell = 0; // [0:No, 1:Yes]
// - Bottom shell
BShell = 0; // [0:No, 1:Yes]
// - Front panel
FPanL = 1; //  [0:No, 1:Yes]
// - Back panel
BPanL = 0; // [0:No, 1:Yes]
// - Panel holes and text
PanelFeatures = 1; // [0:No, 1:Yes]
// Fixation Stick for ACDC convertor
Fixation_Stick = 0; // [0:No, 1:Yes]

*/

// preview[view:north east, tilt:top diagonal]
//----------------------- Box parameters ---------------------------


/* [STL element to export] */
// - Top shell
TShell = 0; // [0:No, 1:Yes]
// - Bottom shell
BShell = 1; // [0:No, 1:Yes]
// - Front panel
FPanL = 1; // [0:No, 1:Yes]
// - Back panel
BPanL = 1; // [0:No, 1:Yes]
// - Panel holes and text
PanelFeatures = 1; // [0:No, 1:Yes]
// Fixation Stick for ACDC convertor
Fixation_Stick = 1; // [0:No, 1:Yes]

/* [Box options] */
// - Wall thickness
Thick = 3;
// - Extra Wall thickness of Back Panel
BackPanelThick = 4.5;
// - Panel thickness
PanelThick = 2;
// delta to aavoid artefacts
delta = 0.001;
// - Font Thickness
FontThick = 0.3;
// - Filet Radius
Filet = 2;
// - 0 for beveled, 1 for rounded
Round = 1; // [0:No, 1:Yes]
// - Printer margin around interior cutouts
CutoutMargin = 0.1;
// - Margin between mating parts
PartMargin = 0.1;
// - PCB feet? (x4)
PCBFeet = 1; // [0:No, 1:Yes]
// - Decorations?
Decorations = 1; // [0:No, 1:Yes]
// - Decorations to ventilation holes
Vent = 1; // [0:No, 1:Yes]
// - Decoration-Holes width (in mm)
Vent_width = 1.5;
// - Tolerance (Panel/rails gap on one edge)
PanelThickGap = CutoutMargin + PartMargin;
PanelVerticalGap = PartMargin;
PanelHorizontalGap = CutoutMargin + PartMargin;

TopShellGap = 0.2;


/* [Box Fixation Tabs] */
// - Side screw hole (or snap) diameter
ScrewHole = 2.2606;
// - Screw thread major diameter for outer shell
BoxHole = 2.8448;
// Thickness of fixation tabs
TabThick = 2;
// Back left tab
BLTab = 1; // [0:Bottom, 1:Top]
// Back right tab
BRTab = 1; // [0:Bottom, 1:Top]
// Front left tab
FLTab = 1; // [0:Bottom, 1:Top]
// Front right tab
FRTab = 1; // [0:Bottom, 1:Top]
// EXPERIMENTAL: Snap tabs
SnapTabs = 0; // [0:Screws, 1:Snaps]


/* [PCB options] */
// - PCB Length
PCBLength = 63;
// - PCB Width
PCBWidth = 86;
// - PCB Thickness
PCBThick = 1.6;
// You likely need to maintain |TabThick| margin on the left and right for tabs
// and whatnot.
// - Margin between front panel and PCB
FrontEdgeMargin = 19;
// - Margin between back panel and PCB
BackEdgeMargin = 39;
// - Margin between left wall and PCB
LeftEdgeMargin = 11;
// - Margin between right wall and PCB
RightEdgeMargin = 104;
// - Margin between top of PCB and box top.
TopMargin = 50;


/* [PCB_Feet] */
// - Foot height above box interior
FootHeight = 8;
// - Foot diameter
FootDia = 8;
// - Hole diameter, or peg for screwless design
FootHole = 2.2606; // tap size for #4 coarse-thread
// - EXPERIMENTAL Screwless design
Screwless = 0; // [0:Screws, 1:Screwless]
FootFilet = FootHeight/4;

// Foot centers are specified as distance from PCB back-left corner.
// X is along the "length" axis, and Y is along the "width" axis.
// - Foot 1 distance from back PCB edge
Foot1X = 4;
// - Foot 1 distance from left PCB edge
Foot1Y = 4;
// - Foot 2 distance from back PCB edge
Foot2X = 4;
// - Foot 2 distance from right PCB edge
Foot2YFromEdge = 4;
Foot2Y = PCBWidth - Foot2YFromEdge;
// - Foot 3 distance from front PCB edge
Foot3XFromEdge = 4;
Foot3X = PCBLength - Foot3XFromEdge;
// - Foot 3 distance from left PCB edge
Foot3Y = 4;
// - Foot 4 distance from front PCB edge
Foot4XFromEdge = 4;
Foot4X = PCBLength - Foot4XFromEdge;
// - Foot 4 distance from right PCB edge
Foot4YFromEdge = 4;
Foot4Y = PCBWidth - Foot4YFromEdge;



// Resolution based on Round parameter. Set this first number to something
// smaller to speed up processing. It should always be a multiple of 4.
Resolution = Round ? 100: 4;

/* Calculate box dimensions from PCB. If you want a more box-centric design
   where the outer diameter of the box matters more than the margins around
   the PCB you can set these manually. The PCB will still be placedaccording
   to the left and back margins, and if you want to use the screwless box
   design, you will need to set the TopMargin to
   (Height - Thick*2 - FootHeight - PCBThick)
*/
Length = PCBLength + FrontEdgeMargin + BackEdgeMargin + ((Thick + PanelThick + PanelThickGap*2)*2);
Width = PCBWidth + LeftEdgeMargin + RightEdgeMargin + Thick*2;
Height = FootHeight + PCBThick + TopMargin + Thick*2;
echo("Box: ", Length=Length, Width=Width, Height=Height);
// X position inset of mounting holes and tabs
MountInset = Thick*4 + PanelThick + PanelThickGap*2 + ScrewHole*4;

// Calculate panel dimensions from box dimensions.
PanelWidth = Width - Thick*2 - PanelHorizontalGap*2;
PanelHeight = Height - Thick*2 - PanelVerticalGap*2;

// front panel definitions

XFrontLOffset = 27;
XincrLEDs = 21;
YoffsetLED = 43;
DiaLED = 6;
DiaButtton = 8;
YoffsetButton = 26;
XoffCharge = XFrontLOffset+3*XincrLEDs + 21;
TextHeightFront = 10;

// back panel definitions

Width_PlugSocket = 40.4;
Height_PlugSocket = 41.2;
XIncr_PlugSocket = 50;
XOffsetRight_PlugSockets = 10;   
YOffsetBottom_PlugSockets = (PanelHeight - Height_PlugSocket)/2;
XBackCentOffset_PlugSockets = PanelWidth - Width_PlugSocket -XOffsetRight_PlugSockets ;

Width_power_plug = 27.2;

XBackCentOffsetFuse = Width_power_plug/2 + XOffsetRight_PlugSockets;
DiaFuse = 13;
XBackOffsetCentFuse = 15;

Height_power_plug = 19.5;
XOffsetCenter_power_plug = XBackCentOffsetFuse;
YOffsetBottom_power_plug = YOffsetBottom_PlugSockets;

YoffsetCentFuse = PanelHeight - (PanelHeight - Height_power_plug - YOffsetBottom_power_plug)/2;

// ACDC Convertor
Width_ACDC_convertor = 50.0;
Depth_ACDC_convertor = 47.9;
Height_ACDC_convertor = 23.6;
Height_ACDC_Frame = 15;
X_Offset_ACDC_Center = 18;
Y_Offset_ACDC_Center = 7;

Width_Fixation_Hole = 5;
Height_Fixation_Hole = 3;

// Battery Pack 4x AA
Width_battery_pack = 64.6;
Depth_battery_pack = 70.1;
Height_battery_pack = 20.0;
Height_battery_pack_Frame = 10;
Height_bottom_pillar_battery_pack = Height - 2 * Thick - Height_battery_pack - PartMargin;
Height_bottom_pillar_battery_pack_extra = 10;

/* [Hidden] */
// - Shell color
Couleur1 = "Orange";
// - Panel color
Couleur2 = "OrangeRed";
// - Text color
TextColor = "White";
// - making decorations thicker if it is a vent to make sure they go through
// shell
// Add a small number to Thick in case Filet is 0.
Dec_Thick = Vent ? Thick*1.001 + Filet : Thick/2;
// Separate vents with a square pillar by default.
Dec_Spacing = Thick + Vent_width;
// X offset to center of first vent
Dec_Offset = Thick*2 + PanelThick + PanelThickGap*2 + Dec_Spacing - Vent_width/2;


/*  Panel Manager

    Use the below 4 modules to produce holes and text on the front and back
    panels. The holes modules should contain instances of SquareHole or
    CylinderHole defined later in this file. The text modules should contain
    instances of LText or CText defined later in this file. It is
    recommended to use variables that you define for your needs to create
    the size and positions of these objects.
*/

// Calculate board-relative positions with respect to the panel, for
// convenience in placing panel elements.
TopOfBoardWRTPanel = FootHeight + PCBThick - PanelVerticalGap;
LeftEdgeOfBoardWRTFPanel = LeftEdgeMargin - PanelHorizontalGap;
LeftEdgeOfBoardWRTBPanel = RightEdgeMargin - PanelHorizontalGap;
// Visible panel edges
PanelBottomEdge = Thick - PanelVerticalGap;
PanelTopEdge = PanelHeight - Thick + PanelVerticalGap;
PanelLeftEdge = Thick - PanelHorizontalGap;
PanelRightEdge = PanelWidth - Thick + PanelHorizontalGap;


// Holes for front panel
module FPanelHoles() {
    // SquareHole(On/Off, Xpos,Ypos,Length,Width,Filet)
    //SquareHole(1, 20, 20, 15, 10, 1);
    //SquareHole(1, 40, 20, 15, 10, 1);
    //SquareHole(1, 60, 20, 15, 10, 1);
    // CylinderHole(On/Off, Xpos, Ypos, Diameter)
    // LEDs
    
    CylinderHole(1, XFrontLOffset, YoffsetLED, DiaLED);
    CylinderHole(1, XFrontLOffset+1*XincrLEDs, YoffsetLED, DiaLED);
    CylinderHole(1, XFrontLOffset+2*XincrLEDs, YoffsetLED, DiaLED);
    CylinderHole(1, XFrontLOffset+3*XincrLEDs, YoffsetLED, DiaLED);
    CylinderHole(1, XoffCharge, YoffsetLED, DiaLED);
    //Buttons
    CylinderHole(1, XFrontLOffset, YoffsetButton, DiaButtton);
    CylinderHole(1, XFrontLOffset+1*XincrLEDs, YoffsetButton, DiaButtton);
    CylinderHole(1, XFrontLOffset+2*XincrLEDs, YoffsetButton, DiaButtton);
    CylinderHole(1, XFrontLOffset+3*XincrLEDs, YoffsetButton, DiaButtton);
    //CylinderHole(1, XoffCharge, YoffsetButton, DiaButtton);
}


// Text for front panel
module FPanelText() {
    // CText(On/Off, Xpos, Ypos, "Font", Size, Diameter, Arc(Deg), Starting Angle(Deg),"Text")
    /*CText(1, 93, 29, "Arial Black", 4, 10, 180, 0,
          ["1", "." , "3", "." , "5", "." , "7", "." , "9", "." , "11"]);
    */
    LText(1, XFrontLOffset, TextHeightFront, "Arial Black", 6, "M", HAlign="center");
    LText(1, XFrontLOffset+1*XincrLEDs, TextHeightFront, "Arial Black", 6, "L", HAlign="center");
    LText(1, XFrontLOffset+2*XincrLEDs, TextHeightFront, "Arial Black", 6, "H", HAlign="center");
    LText(1, XFrontLOffset+3*XincrLEDs, TextHeightFront, "Arial Black", 6, "P", HAlign="center");
    LText(1, XoffCharge, TextHeightFront, "Arial Black", 6, "C", HAlign="center");
}


// Holes for back panel
module BPanelHoles() {
    /*CylinderHole(1,
                 LeftEdgeOfBoardWRTBPanel + 16.4,
                 TopOfBoardWRTPanel + 7,
                 5 + PartMargin*2);*/
    
 


    
    //SquareHole(On/Off, Xpos,Ypos,Length,Width,Filet)
    
    //SquareHole for 3x power sockets
    PowerSocketHole(1,
               XBackCentOffset_PlugSockets,
               YOffsetBottom_PlugSockets,
               Width_PlugSocket,
               Height_PlugSocket,
               0);
    
    PowerSocketHole(1,
               XBackCentOffset_PlugSockets - 1 * XIncr_PlugSocket,
               YOffsetBottom_PlugSockets,
               Width_PlugSocket,
               Height_PlugSocket,
               0);
    
    PowerSocketHole(1,
               XBackCentOffset_PlugSockets - 2 * XIncr_PlugSocket,
               YOffsetBottom_PlugSockets,
               Width_PlugSocket,
               Height_PlugSocket,
               0);
               
    PowerPlugSquareHole(1,
               XOffsetCenter_power_plug - Width_power_plug/2,
               YOffsetBottom_power_plug,
               Width_power_plug,
               Height_power_plug,
               0);
    // CylinderHole(On/Off, Xpos, Ypos, Diameter)
    CylinderHole(1,
                 XBackCentOffsetFuse,
                 YoffsetCentFuse,
                 DiaFuse);
}


// Text for back panel
module BPanelText() {
    /*LText(1,
          LeftEdgeOfBoardWRTBPanel + 16.4,
          TopOfBoardWRTPanel + 7 + 5,
          "Arial Black",
          4, "PWR"
    );
    LText(1,
          LeftEdgeOfBoardWRTBPanel + 37.3 + 39.2/2,
          TopOfBoardWRTPanel + 12.55 + 2,
          "Arial Black",
          4,
          "DATA"
    );*/
}


// ------- You probably don't need to modify anything below this line. --------


/* Generic rounded box

    Produces a box of the specified dimensions. Corners are rounded
    according to Filet and Resolution parameters.

    Arguments:
    xshrink: the amount to reduce the length on one end compared to the full
        length
    yzshrink: the amount to reduce the width or height on one edge compared
        to the full box
*/
module RoundBox(xshrink=0, yzshrink=0) {
    Filet = (Filet > Thick*2) ? Filet - yzshrink : Filet;
    translate([xshrink, yzshrink, yzshrink]) {
        rotate([90, 0, 90]) {
            linear_extrude(height=Length - xshrink*2) {
                translate([Filet, Filet, 0]) {
                    offset(r=Filet, $fn=Resolution) {
                        square([Width - 2*yzshrink - 2*Filet, Height - 2*yzshrink - 2*Filet]);
                    }
                }
            }
        }
    }
}


/*  MainBox: Main box module

    This module produces the simple main box half. No feet, tabs, vents or
    fixation is applied here.
*/
module MainBox() {
    difference() {
        union() {
            // Makes a hollow box with walls of Thick thickness.
            difference() {
                RoundBox();
                RoundBox(xshrink=Thick, yzshrink=Thick);
            }
            // Makes interior backing for panel as a wall
            translate([(BackPanelThick - PanelThick)/2,0, 0 ]) {
                difference() {
                    RoundBox(xshrink=(Thick  + (BackPanelThick + PanelThick)/2 + PanelThickGap*2), yzshrink=Thick/2);
                    RoundBox(xshrink=(Thick * 3 + ( BackPanelThick + PanelThick)/2 + PanelThickGap*2));
                }
             }
        }
        // Remove the top half
        translate([-Thick, -Thick, Height/2]) {
            cube([Length + Thick*2, Width + Thick*2, Height]);
        }
        // Remove the center for panel visibility.
        RoundBox(xshrink=-Thick, yzshrink=Thick*2);
    }
}


module frame(width=1, depth=1, height=1, thickness=0.5, gap=0.2, xoffset=0, yoffset=0, zoffset=0,delta=0.01, paint=[1,0,0], withslot=false, slotwidth=0){
    translate([xoffset,yoffset,zoffset]) {
        color(paint) {
            difference () {
                cube([width+2*thickness+2*gap, depth+2*thickness+2*gap, height], center=true);
                if (withslot) {
                    translate([0,0,0]) {
                    cube ( [width+2*gap, depth+2*gap, height+delta], center=true);
                    }
                    translate([0,depth/2+gap+thickness/2-delta,0]) {
                        cube ( [slotwidth, thickness+3*delta, height+delta], center=true);
                    }
                    translate([0,-depth/2-gap-thickness/2+delta,0]) {
                        cube ( [slotwidth, thickness+3*delta, height+delta], center=true);
                    }
                }
            }
        }
    } 
}


/*  decoration: a single box decoration
*/
module decoration() {
    translate([-Vent_width/2, -Thick, 2* Thick]) {
        cube([Vent_width, Dec_Thick + Thick, Height/6 + Thick]);
    }
}


/* LeftDecorations: left decorations module

    Produces the decorations/vents for just the left side of the box.
    These can be rotated and translated for the right side.
*/
module LeftDecorations() {
    for (i=[0 : Dec_Spacing : Length/4]) {
        translate([Dec_Offset + 5 + i, 0, 0]) {
            decoration();
        }
        translate([Length - Dec_Offset - 5  - i, 0, 0]) {
            decoration();
        }
    }
}


/*  Decorations: decorations module

    This module produces the box vents or decorations.
*/
module Decorations() {
    LeftDecorations();
    // Mirror for the right side decorations
    translate([0, Width, 0]) {
        mirror([0, 1, 0]) {
            LeftDecorations();
        }
    }
}


/*  Coque: Shell module

    This module takes no arguments, but produces a box shell. This is half
    the box, including slots for end panels, rounded corners according to
    Filet and Resolution, and vents/decorations according to parameters.
*/
module Coque() {
    color(Couleur1) {
        difference() {
            MainBox();
            if (Decorations) {
                Decorations();
            }
        }
    }
}


/*  tab: tab module

    Produces a single box fixation tab with screw hole or snap button
*/
module tab() {
    translate([0, Thick, Height/2]) {
        rotate([90, 0, 180]) {
            difference() {
                linear_extrude(TabThick) {
                    difference() {
                        circle(r=4*ScrewHole, $fn=6);
                        if (!SnapTabs) {
                            translate([0, ScrewHole*2, 0]) {
                                circle(d=ScrewHole, $fn=100);
                            }
                        }
                    }
                }
                translate([-4*ScrewHole, -ScrewHole, TabThick]) {
                    rotate([90+45, 0, 0]) {
                        cube([8*ScrewHole, 3*ScrewHole, 5*ScrewHole]);
                    }
                }
                translate([-4*ScrewHole, 0, -PartMargin]) {
                    cube([8*ScrewHole,4*ScrewHole,PartMargin*2]);
                }
            }
            if (SnapTabs) {
                translate([0, ScrewHole*2, PartMargin]) {
                    difference() {
                        sphere(d=(ScrewHole - PartMargin*2), $fn=100);
                        translate([0, 0, ScrewHole/2 + TabThick/2]) {
                            cube(ScrewHole, center=true);
                        }
                    }
                }
            }
        }
    }
}


/*  Tabs: tabs module

    This module produces the wall fixation box tabs.
    Tabs are produced according to the parameters for XXTab indicating top
    or bottom.

    Arguments:
        top: 0 for bottom shell tabs. 1 for top shell tabs. defaults to bottom.
*/
module Tabs(top=0) {
    color(Couleur1) {
        if (BLTab == top) {
            translate([MountInset, 0, 0]) {
                tab();
            }
        }
        if (FLTab == top) {
            translate([Length - MountInset, 0, 0]) {
                tab();
            }
        }
        if (BRTab == top) {
            translate([MountInset, Width, 0]) {
                rotate([0, 0, 180]) {
                    tab();
                }
            }
        }
        if (FRTab == top) {
            translate([Length - MountInset, Width, 0]) {
                rotate([0, 0, 180]) {
                    tab();
                }
            }
        }
    }
}


/*  hole: hole module

    Produces a box hole for fixation. This is either a cylinder for a screw
    or a semispherical indention for snap tabs.
*/
module hole() {
    if (SnapTabs) {
        translate([0, -Thick, Height/2 - 2*ScrewHole]) {
            sphere(d=ScrewHole, $fn=100);
        }
    }
    else {
        translate([0, Thick, Height/2 - 2*ScrewHole]) {
            rotate([90, 0, 0]) {
                cylinder(Thick*3, d=BoxHole, $fn=100);
            }
        }
    }
}


/*  Holes: holes module

    This module produces the holes necessary in the box fixation tabs and in
    the wall of the box for the corresponding tabs to affix to. Holes are
    produced according to the parameters for XXTab indicating top or bottom.

    Arguments:
        top: 0 for bottom shell holes. 1 for top shell holes. defaults to
            bottom.
*/
module Holes(top=0) {
    color(Couleur1) {
        if (BRTab != top) {
            translate([MountInset, Width, 0]) {
                hole();
            }
        }
        if (FRTab != top) {
            translate([Length - MountInset, Width, 0]) {
                hole();
            }
        }
        if (BLTab != top) {
            translate([MountInset, 0, 0]) {
                rotate([0, 0, 180]) {
                    hole();
                }
            }
        }
        if (FLTab != top) {
            translate([Length - MountInset, 0, 0]) {
                rotate([0, 0, 180]) {
                    hole();
                }
            }
        }
    }
}

/*  PCB: PCB module

    Produces the model of the PCB using parameters for its size and thickness.
    The text PCB is placed on top of the board. This is called by the Feet()
    module with the % modifier which makes this module translucent and only
    viewed in preview mode.
*/
module PCB() {
    translate([0, 0, FootHeight]) {
        cube([PCBLength, PCBWidth, PCBThick]);
        translate([PCBLength/2, PCBWidth/2, PCBThick]) {
            color("Olive") {
                linear_extrude(height=FontThick) {
                    text("PCB", font="Arial black", halign="center", valign="center");
                }
            }
        }
    }
}


/*  foot module

    Produces a single foot for PCB mounting.
*/
module foot(top=0) {
    color(Couleur1) {
        rotate_extrude($fn=100) {
            difference() {
                union() {
                    if (Screwless && top) { // Foot with TopMargin height
                        square([FootDia/2 + FootFilet, TopMargin]);
                    }
                    else if (Screwless && !top) { // Foot for PCB peg
                        square([FootDia/2 + FootFilet, FootHeight + PCBThick*2]);
                    }
                    else if (!Screwless && !top) { // Foot with screw hole
                        translate([FootHole/2 + CutoutMargin, 0, 0]) {
                            square([(FootDia - FootHole)/2 - CutoutMargin + FootFilet, FootHeight]);
                        }
                    }
                }
                translate([FootDia/2 + FootFilet, FootFilet, 0]) {
                    offset(r=FootFilet, $fn=Resolution) {
                        square(Height);
                    }
                }
                if (Screwless && !top) { // Remove around peg
                    translate([FootHole/2 - PartMargin, FootHeight]) {
                        polygon([[0, 0],
                                 [FootDia/2, 0],
                                 [FootDia/2, PCBThick*3],
                                 [-FootHole/3, PCBThick*3],
                                 [0, PCBThick]
                                ]
                        );
                    }
                }
                if (Screwless && top) { // Remove hole for peg
                    translate([-FootHole/2, TopMargin - PCBThick, 0]) {
                        polygon([[0, 0],
                                 [(FootHole*5/6 + CutoutMargin), 0],
                                 [(FootHole + CutoutMargin), PCBThick],
                                 [(FootHole + CutoutMargin), PCBThick*2],
                                 [0, PCBThick*2],
                                ]
                        );
                    }
                }

            }
        }
    }
}


/*  Feet module

    Combines four feet to form mounting platform for PCB.
    A model of the PCB is included with the background modifier. It is
    translucent but visible in the preview, but not in the final render.

    No arguments are used, but parameters provide the PCB and foot dimensions.
*/
module Feet(top=0) {
    translate([BackEdgeMargin + Thick + PanelThick + PanelThickGap*2, LeftEdgeMargin + Thick, Thick]) {
        if (!top) {
            %PCB();
        }
        
        if (Screwless || !top ) {
            translate([Foot1X, Foot1Y]) {
                foot(top=top);
            }
            translate([Foot2X, Foot2Y]) {
                foot(top=top);
                }
            translate([Foot3X, Foot3Y]) {
                foot(top=top);
                }
            translate([Foot4X, Foot4Y]) {
                foot(top=top);
            }
        }
    }
    
    //translate( [ 7.7, 20, Thick]) { cube(39.7); }
    


    
    translate( [ Length/2+X_Offset_ACDC_Center, Y_Offset_ACDC_Center+LeftEdgeMargin + PCBWidth + (Width-LeftEdgeMargin-PCBWidth-Thick)/2, Thick]) {
        translate( [0, 0, Height_ACDC_convertor/2]) { 
            //cube([Width_ACDC_convertor,Depth_ACDC_convertor,Height_ACDC_convertor],center=true);
        }
        translate( [0, 0, Height_ACDC_Frame/2]) {
            frame( width=Width_ACDC_convertor,depth=Depth_ACDC_convertor,height=Height_ACDC_Frame, thickness=3, gap=0.1, withslot=true, slotwidth=15, xoffset=0, yoffset=0, zoffset=0 );
        }
    } 
    
    
    
    
    // Battery Pack
    
    // bottom pillars

    difference () {
        union () {
            translate( [ Length/2+X_Offset_ACDC_Center, Y_Offset_ACDC_Center+LeftEdgeMargin + PCBWidth + (Width-LeftEdgeMargin-PCBWidth-Thick)/2, Thick+Height_bottom_pillar_battery_pack/2]) {
                 translate([Width_battery_pack/2-Thick,0,0])cube ([Thick*2,Depth_battery_pack,Height_bottom_pillar_battery_pack],center=true);
                 translate([-Width_battery_pack/2+Thick,0,0])cube ([Thick*2,Depth_battery_pack,Height_bottom_pillar_battery_pack],center=true);
            }
            
            translate( [ Length/2+X_Offset_ACDC_Center, Y_Offset_ACDC_Center+LeftEdgeMargin + PCBWidth + (Width-LeftEdgeMargin-PCBWidth-Thick)/2, Thick+(Height_bottom_pillar_battery_pack+10)/2]) {
                 translate([Width_battery_pack/2+0.5*Thick,0,0])cube ([Thick,Depth_battery_pack,Height_bottom_pillar_battery_pack+10],center=true);
                 translate([-Width_battery_pack/2-0.5*Thick,0,0])cube ([Thick,Depth_battery_pack,Height_bottom_pillar_battery_pack+10],center=true);
            }

            translate( [ Length/2+X_Offset_ACDC_Center, Y_Offset_ACDC_Center+LeftEdgeMargin + PCBWidth + (Width-LeftEdgeMargin-PCBWidth-Thick)/2, Thick+(Height_bottom_pillar_battery_pack+Height_bottom_pillar_battery_pack_extra)/2]) {
                translate([Width_battery_pack/2-0.5*Thick,-(Depth_battery_pack+Thick)/2,0])cube ([3*Thick,Thick,Height_bottom_pillar_battery_pack+Height_bottom_pillar_battery_pack_extra],center=true);
                 translate([-Width_battery_pack/2+0.5*Thick,-(Depth_battery_pack+Thick)/2,0])cube ([3*Thick,Thick,Height_bottom_pillar_battery_pack+Height_bottom_pillar_battery_pack_extra],center=true);
            }
        }
        
        translate( [ Length/2+X_Offset_ACDC_Center, Y_Offset_ACDC_Center+LeftEdgeMargin + PCBWidth + (Width-LeftEdgeMargin-PCBWidth-Thick)/2, Thick+Height_ACDC_convertor+Height_Fixation_Hole/2+3*PartMargin]) {
           cube([Width_battery_pack+2*Thick+PartMargin,Width_Fixation_Hole,Height_Fixation_Hole],center=true); 
        }
    
   }
    
    
}



/*  TopShell: top shell module

    Produces the top shell, including requested fixation tabs and holes
    Model reneis rotated and translated to the appropriate position.
*/
module TopShell() {
    translate([0, 0, Height + TopShellGap]) {
        mirror([0, 0, 1]) {
            difference() {
                union() {
                    Coque();
                    Tabs(top=1);
                    if (Screwless && PCBFeet) {
                       Feet(top=1);
                    }
                }
                Holes(top=1);
            }
        }
    }
    
    // Frame for Battery Pack 4x AA
    
    translate( [ Length/2+X_Offset_ACDC_Center, Y_Offset_ACDC_Center+LeftEdgeMargin + PCBWidth + (Width-LeftEdgeMargin-PCBWidth-Thick)/2, Height-Thick+TopShellGap]) {
        translate( [0, 0, -Height_battery_pack/2]) { 
            //cube([Width_battery_pack,Depth_battery_pack,Height_battery_pack],center=true);
        }
        translate( [0, 0, -Height_battery_pack_Frame/2]) {
            frame( width=Width_battery_pack,depth=Depth_battery_pack,height=Height_battery_pack_Frame, thickness=3, gap=0.1, withslot=true, slotwidth=10, xoffset=0, yoffset=0, zoffset=0 );
        }
    } 
    
}


/*  BottomShell: bottom shell module

    Produces the bottom shell, including requested fixation tabs, holes,
    and PCB feet.
*/
module BottomShell() {
    difference() {
        union() {
            Coque();
            Tabs();
            if (PCBFeet) {
               Feet(top=0);
            }
        }
        Holes();
    }  
}


/*  ACDC_Fixation: ACDC_Fixation stick creation module

    Produces the stick for fixing tthe ACDC convertor.
*/
module ACDC_Fixation () {
    translate([Length+10,Width_battery_pack+6*Thick,(Height_Fixation_Hole-2*PartMargin)/2]) { cube([Width_Fixation_Hole-2*PartMargin,Width_battery_pack+2*Thick,Height_Fixation_Hole-2*PartMargin],center=true); }
}


/*  Panel module

    Produces a single panel with potentially rounded corners. Takes no
    arguments but uses the global parameters.
*/
module Panel() {
    Filet = (Filet > Thick*2) ? Filet - Thick - PanelVerticalGap : Filet - PanelVerticalGap;
    echo("Panel:", Thick=PanelThick, PanelWidth=PanelWidth, PanelHeight=PanelHeight, Filet=Filet);
    translate([Filet, Filet, 0]) {
        offset(r=Filet, $fn=Resolution) {
            square([PanelWidth - Filet*2,  PanelHeight - Filet*2]);
        }
    }
}


/*  Cylinder Hole module

    Produces a cylinder for use as a holein a panel

    Arguments:
    OnOff: Rendered only if 1
    Cx: X position of hole center
    Cy: Y position of hole center
    Cdia: diameter of hole
*/
module CylinderHole(OnOff, Cx, Cy, Cdia) {
    if (OnOff) {
        echo("CylinderHole:", Cx=Cx, Cy=Cy, Cdia=Cdia + CutoutMargin*2);
        translate([Cx, Cy, 0]) {
            circle(d=Cdia + CutoutMargin*2, $fn=100);
        }
    }
}


/*  PowerSocket Hole module

    Produces a rectangular prism with potentially rounded corner for use as
    a hole in a panel

    Arguments:
    OnOff: Rendered only if 1
    Sx: X position of bottom left corner
    Sy: Y position of bottom left corner
    Sl: width of rectangle
    Sw: height of rectangle
    Filet: radius of rounded corner
*/


Width_Nose_PowerSocketHole = 3.8;
Height_Nose_PowerSocketHole = 1.1;
Edge_Offset_Nose_Height_Nose_PowerSocketHole = 5.4;

Width_Rip_PowerSocketHole = 10.1;
Height_Rip_PowerSocketHole = 1.0;

module PowerSocketHole(OnOff, Sx, Sy, Sl, Sw, Filet) {
    if (OnOff) {
        Offset = Filet + CutoutMargin;
        echo("SquareHole:", Sx=Sx - CutoutMargin, Sy=Sy - CutoutMargin,
             Sl=Sl + CutoutMargin*2, Sw=Sw + CutoutMargin*2, Filet=Offset);
        translate([Sx + Filet, Sy + Filet, 0]) {
            offset(r=Offset, $fn=Resolution) {
                square([Sl - Filet*2, Sw - Filet*2]);
            }
            my_height = Sw - Filet*2;
            my_width  = Sl - Filet*2;
            translate ([-Height_Nose_PowerSocketHole,Edge_Offset_Nose_Height_Nose_PowerSocketHole,0]) square([Height_Nose_PowerSocketHole,Width_Nose_PowerSocketHole]);
            translate ([-Height_Nose_PowerSocketHole,my_height-Width_Nose_PowerSocketHole-Edge_Offset_Nose_Height_Nose_PowerSocketHole,0]) square([Height_Nose_PowerSocketHole,Width_Nose_PowerSocketHole]);
            translate ([my_width,Edge_Offset_Nose_Height_Nose_PowerSocketHole,0]) square([Height_Nose_PowerSocketHole,Width_Nose_PowerSocketHole]);
            translate ([my_width,my_height-Width_Nose_PowerSocketHole-Edge_Offset_Nose_Height_Nose_PowerSocketHole,0]) square([Height_Nose_PowerSocketHole,Width_Nose_PowerSocketHole]);
            translate ([my_width/2-Width_Rip_PowerSocketHole/2,my_height]) square([Width_Rip_PowerSocketHole,Height_Rip_PowerSocketHole]);
            translate ([my_width/2-Width_Rip_PowerSocketHole/2,-Height_Rip_PowerSocketHole]) square([Width_Rip_PowerSocketHole,Height_Rip_PowerSocketHole]);
        }
        
    }
}


/*  Square Hole module

    Produces a rectangular prism with potentially rounded corner for use as
    a hole in a panel

    Arguments:
    OnOff: Rendered only if 1
    Sx: X position of bottom left corner
    Sy: Y position of bottom left corner
    Sl: width of rectangle
    Sw: height of rectangle
    Filet: radius of rounded corner
*/
module SquareHole(OnOff, Sx, Sy, Sl, Sw, Filet) {
    if (OnOff) {
        Offset = Filet + CutoutMargin;
        echo("SquareHole:", Sx=Sx - CutoutMargin, Sy=Sy - CutoutMargin,
             Sl=Sl + CutoutMargin*2, Sw=Sw + CutoutMargin*2, Filet=Offset);
        translate([Sx + Filet, Sy + Filet, 0]) {
            offset(r=Offset, $fn=Resolution) {
                square([Sl - Filet*2, Sw - Filet*2]);
            }
        }
    }
}


/*  Powersocket Square Hole module

    Produces a rectangular prism with potentially rounded corner for use as
    a hole in a panel with some notches for fixation of the power socket

    Arguments:
    OnOff: Rendered only if 1
    Sx: X position of bottom left corner
    Sy: Y position of bottom left corner
    Sl: width of rectangle
    Sw: height of rectangle
    Filet: radius of rounded corner
*/
module PowerPlugSquareHole(OnOff, Sx, Sy, Sl, Sw, Filet) {

    if (OnOff) {
        width_lower_notch      = 7.2;
        height_lower_notch     = 1.4;
        depth_notches          = 1.0;
        width_upper_notch      = 4.5;
        height_upper_notch     = 1.2;
        distance_upper_notches = 9.5;    
        Offset = Filet + CutoutMargin;
        echo("PowerPlugSquareHole:", Sx=Sx - CutoutMargin, Sy=Sy - CutoutMargin,
             Sl=Sl + CutoutMargin*2, Sw=Sw + CutoutMargin*2, Filet=Offset);
        translate([Sx + Filet, Sy + Filet, 0]) {
            offset(r=Offset, $fn=Resolution) {
                square([Sl - Filet*2, Sw - Filet*2]);
            }
            my_height = Sw - Filet*2;
            my_width  = Sl - Filet*2;
            
            translate ([my_width/2-width_upper_notch-distance_upper_notches/2,my_height]) square([width_upper_notch,height_upper_notch]);
            translate ([my_width/2+distance_upper_notches/2,my_height]) square([width_upper_notch,height_upper_notch]);
            translate ([my_width/2-width_lower_notch/2,-height_lower_notch]) square([width_lower_notch,height_lower_notch]);
        } 
    }
}


/*  LText module

    Produces linear text for use on a panel

    Arguments:
    OnOff: Rendered only if 1
    Tx: X position of bottom left corner of text
    Ty: Y position of bottom left corner of text
    Font: Font to use for text
    Size: Approximate Height of text in mm.
    Content: The text
    HAlign: Text horizontal alignment. Defaults to "center". "left" and
        "right" available.
    VAlign: Text vertical alignment. Defaults to "baseline". "top",
        "center", and "bottom" available.
*/
module LText(OnOff,Tx,Ty,Font,Size,Content, HAlign="center", VAlign="baseline") {
    if (OnOff) {
        echo("LText:", Tx=Tx, Ty=Ty, Font=Font, Size=Size, Content=Content, HAlign=HAlign, VAlign=VAlign);
        translate([Tx, Ty, PanelThick-FontThick]) {
            linear_extrude(height=FontThick+delta) {
                text(Content, size=Size, font=Font, halign=HAlign, valign=VAlign);
            }
        }
    }
}


/*  CText module

    Produces circular text for a panel

    OnOff:Rendered only if 1
    Tx: X position of text
    Ty: Y position of text
    Font: Font to use for text
    Size: Approximate height of text in mm
    TxtRadius: Radius of text
    Angl: Arc angle
    Turn: Starting angle
    Content: The text
*/
module CText(OnOff, Tx, Ty, Font, Size, TxtRadius, Angl, Turn, Content) {
    if (OnOff) {
        echo("CText:", Tx=Tx, Ty=Ty, Font=Font, Size=Size,
             TxtRadius=TxtRadius, Turn=Turn, Content=Content);
        Angle = -Angl / (len(Content) - 1);
        translate([Tx, Ty, PanelThick]) {
            for (i= [0 : len(Content) - 1] ) {
                rotate([0, 0, i*Angle + 90 + Turn]) {
                    translate([0, TxtRadius, 0]) {
                        linear_extrude(height=FontThick) {
                            text(Content[i], size=Size, font=Font, halign="center");
                        }
                    }
                }
            }
        }
    }
}


/*  FPanL module

    Produces the front panel. No arguments are used, but this module imports
    FPanelHoles() and FPanelText() which must be edited to produce holes and
    text for your box.
*/
module FPanL() {
    translate([Length - (Thick + PanelThickGap + PanelThick),
               Thick + PanelHorizontalGap,
               Thick + PanelVerticalGap]) {
        rotate([90, 0, 90]) {
            difference() {
                color(Couleur2) {
                    linear_extrude(height=PanelThick) {
                        difference() {
                            Panel();
                            if (PanelFeatures) {
                                FPanelHoles();
                            }
                        }
                    }
                }
                color(TextColor) {
                    if (PanelFeatures) {
                        FPanelText();
                    }
                }
            }
        }
    }
    
}


/*  BPanL module

    Produces the back panel. No arguments are used, but this module imports
    BPanelHoles() and BPanelText() which must be edited to produce holes and
    text for your box.
*/ 
module BPanL() {
    PanelThick = BackPanelThick;
    translate([Thick + PanelThickGap + PanelThick,
               Thick + PanelHorizontalGap + PanelWidth,
               Thick + PanelVerticalGap]) {
        rotate([90, 0, 270]) {
            color(Couleur2) {
                linear_extrude(height=PanelThick) {
                    difference() {
                        Panel();
                        if (PanelFeatures) {
                            BPanelHoles();
                        }
                    }
                }
            }
            color(TextColor) {
                if (PanelFeatures) {
                    BPanelText();
                }
            }
        }
    }
}




// Top shell
if (TShell) {
    TopShell();
}

// Bottom shell
if (BShell) {
    BottomShell();
}

// Front panel
if (FPanL) {
    FPanL();
}

// Back panel
if (BPanL) {
    BPanL();
}


// Fixation stick
if (Fixation_Stick) {
    ACDC_Fixation();
}

