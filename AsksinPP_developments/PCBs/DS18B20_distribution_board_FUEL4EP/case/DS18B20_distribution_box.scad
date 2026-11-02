// ====================================================================
// openSCAD case for DS18B20 distribution board DS18B20_distribution_board_FUEL4EP
// (C) FUEL4EP 08. Sep 2026 
// License: Creative Commons BY-NC-SA 4.0
// PARAMETRIC ENCLOSURE WITH SNAP-IN LID & INTEGRATED STRAIN RELIEF
// PART 1 OF 3: CONFIGURATION & MAIN VARIABLES WITH PCB MOCKUP
// Optimized for WAGO 233-503 terminals & precise Y-coordinates
// ====================================================================

// configuration flags to create subset(s) are below
// set to true to
// - see the transparent PCB inside the box
// - for STL creation just turn one option to true
//show_pcb = true;  				// for visualizing the PCB mock-up
//show_main_box = true;
//show_lid = true;
//show_strain_relief_clamps = true;
//show_top_strain_relief_clamp = true;



$fn = 60; // Rendering quality for circular shapes

// --- PCB & Enclosure Dimensions ---
pcb_width = 40.64;      // Width of the PCB (X)
pcb_length = 91.44;     // Length of the PCB (Y)
pcb_thickness = 1.6;    // PCB thickness (Standard: 1.6mm)

wall_thickness = 2.0;   // Wall thickness of the enclosure
clearance = 1.0;        // Inner tolerance/spacing around the PCB
inner_height = 30.0;    // Increased inner height for WAGO 233-503

// Extension in Y-direction to clear the mounting holes
enclosure_extension_y = 12.0; // Adds 6mm padding to the top and 6mm to the bottom

// --- Mounting Holes (M3) ---
hole_distance_w = 32.839; // Hole spacing width (X)
hole_distance_l = 83.566; // Hole spacing length (Y)
screw_diameter = 3.0;     // M3 screw diameter
standoff_height = 5.0;    // Height of standoffs (bottom to PCB)
standoff_diameter = 6.0;  // Outer diameter of standoffs

// --- Cable Entry Holes ---
cable_diameter = 5.5;     // Diameter for the cable entries
delta_cable_diameter = 0.4; // Reduce diameter of strain reliefs

// --- Strain Relief Config ---
strain_screw_dia = 2.5;   // Holes for M2.5 or M3 screws to clamp the strain relief
clamp_block_width = 6.0;  // Width of the internal strain relief block
add_cabling_width = 3;    // Add room for cabling

// --- Calculated Inner and Outer Dimensions ---
enclosure_inner_w = pcb_width + (clearance * 2) + ((add_cabling_width + clamp_block_width) * 2);
enclosure_inner_l = pcb_length + (clearance * 2) + ((add_cabling_width + clamp_block_width) * 2);
enclosure_outer_w = enclosure_inner_w + (wall_thickness * 2);
enclosure_outer_l = enclosure_inner_l + (wall_thickness * 2);
enclosure_outer_h = inner_height + wall_thickness;

// Exact Y-center coordinates of the WAGO terminals from PCB bottom edge
wago_y_coordinates = [15.673, 31.421, 47.169, 62.917];

// Side clamp blocks configuration
side_clamp_length = 70.0; 
clamp_start_y = 6 + 6.5;

// Top center clamp block configuration
top_clamp_length = 25.0;
top_clamp_start_x = (enclosure_outer_w - top_clamp_length) / 2;

// Exact Z-Center position of the cable holes
hole_z_pos = wall_thickness + standoff_height + pcb_thickness + 6.0; 
clamp_block_height = hole_z_pos; 

// FIXED X-OFFSET: Center the PCB in X-direction
pcb_offset_x = (enclosure_outer_w - hole_distance_w) / 2;
pcb_origin_x = pcb_offset_x - (pcb_width - hole_distance_w) / 2;

// FIXED Y-ALIGNMENT: Force the PCB position so that the first WAGO terminal center
// lines up perfectly with the first enclosure side hole.
first_wago_y = wago_y_coordinates[0];
target_hole_y = wall_thickness + clearance + (enclosure_extension_y / 2) + first_wago_y;
pcb_origin_y = target_hole_y - first_wago_y;

// Calculate the corresponding offset for the standoff layout
pcb_offset_y = pcb_origin_y + (pcb_length - hole_distance_l) / 2;

// configuration flags to create subset(s)
// set to true to
// - see the transparent PCB inside the box
// - for STL creation just turn one option to true
show_pcb = true;  				// for visualizing the PCB mock-up
show_main_box = true;
show_lid = true;
show_strain_relief_clamps = true;
show_top_strain_relief_clamp = true;

// ====================================================================
// MAIN PROGRAM (Renders everything side-by-side)
// ====================================================================
if (show_main_box) {
  main_box();
}

if (show_pcb) {
    pcb_mockup();
}

if (show_lid) {
  translate([enclosure_outer_w + 15, 0, 0])
    lid();
}

if (show_strain_relief_clamps) {
  translate([enclosure_outer_w + 15, enclosure_outer_l + 10, 0])
    strain_relief_clamp();
	translate([enclosure_outer_w + 25, enclosure_outer_l + 10, 0])
    strain_relief_clamp();
}

if (show_top_strain_relief_clamp) {
  translate([enclosure_outer_w + 35, enclosure_outer_l + 10, 0])
    top_strain_relief_clamp();
}
// ====================================================================
// PARAMETRIC ENCLOSURE WITH SNAP-IN LID & INTEGRATED STRAIN RELIEF
// PART 2 OF 3: MAIN ENCLOSURE MODULE
// ====================================================================

module main_box() {
    difference() {
        // --- 1. Base Geometry (Full High Box + Lower Internal Clamp Blocks) ---
        union() {
            cube([enclosure_outer_w, enclosure_outer_l, enclosure_outer_h]);
            
            // Left & Right internal clamp blocks (Low)
            translate([wall_thickness, clamp_start_y, wall_thickness]) 
                cube([clamp_block_width, side_clamp_length, clamp_block_height - wall_thickness]);
                
            translate([enclosure_outer_w - wall_thickness - clamp_block_width, clamp_start_y, wall_thickness]) 
                cube([clamp_block_width, side_clamp_length, clamp_block_height - wall_thickness]);
                
            // Top center internal clamp block (Low)
            translate([top_clamp_start_x, enclosure_outer_l - wall_thickness - clamp_block_width, wall_thickness])
                cube([top_clamp_length, clamp_block_width, clamp_block_height - wall_thickness]);
        }
        
        // --- 2. Segmented Cavity Subtractions ---
        // A: Core center cavity (Full Height) - leaves room for all side and top blocks
        translate([wall_thickness + clamp_block_width, wall_thickness, wall_thickness])
            cube([enclosure_inner_w - (clamp_block_width * 2), enclosure_inner_l - clamp_block_width, inner_height + 1]);
            
        // B: Clear space above the side and top blocks (From clamp_block_height up to top)
        translate([wall_thickness, clamp_start_y, clamp_block_height])
            cube([clamp_block_width, side_clamp_length, enclosure_outer_h]);
        translate([enclosure_outer_w - wall_thickness - clamp_block_width, clamp_start_y, clamp_block_height])
            cube([clamp_block_width, side_clamp_length, enclosure_outer_h]);
        translate([top_clamp_start_x, enclosure_outer_l - wall_thickness - clamp_block_width, clamp_block_height])
            cube([top_clamp_length, clamp_block_width, enclosure_outer_h]);

        // C: Clear space inside in front of and behind the side clamp blocks (Left, Full Height)
        translate([wall_thickness, wall_thickness, wall_thickness])
            cube([clamp_block_width, clamp_start_y - wall_thickness, inner_height + 1]);
        translate([wall_thickness, clamp_start_y + side_clamp_length, wall_thickness])
            cube([clamp_block_width, enclosure_outer_l - clamp_start_y - side_clamp_length - wall_thickness, inner_height + 1]);
            
        // D: Clear space inside in front of and behind the side clamp blocks (Right, Full Height)
        translate([enclosure_outer_w - wall_thickness - clamp_block_width, wall_thickness, wall_thickness])
            cube([clamp_block_width, clamp_start_y - wall_thickness, inner_height + 1]);
        translate([enclosure_outer_w - wall_thickness - clamp_block_width, clamp_start_y + side_clamp_length, wall_thickness])
            cube([clamp_block_width, enclosure_outer_l - clamp_start_y - side_clamp_length - wall_thickness, inner_height + 1]);
            
        // E: Clear space to the left and right of the top center block (Full Height)
        translate([wall_thickness + clamp_block_width, enclosure_outer_l - wall_thickness - clamp_block_width, wall_thickness])
            cube([top_clamp_start_x - (wall_thickness + clamp_block_width), clamp_block_width, inner_height + 1]);
        translate([top_clamp_start_x + top_clamp_length, enclosure_outer_l - wall_thickness - clamp_block_width, wall_thickness])
            cube([enclosure_outer_w - wall_thickness - clamp_block_width - (top_clamp_start_x + top_clamp_length), clamp_block_width, inner_height + 1]);
        
        // --- 3. Holes and Channels ---
        // 1-Wire Connection (Top Center Hole)
        translate([enclosure_outer_w / 2, enclosure_outer_l - wall_thickness - clamp_block_width - 1, hole_z_pos])
            rotate([-90, 0, 0])
                cylinder(d=cable_diameter, h=wall_thickness + clamp_block_width + 2);
        
        // 4 Inputs Left & 4 Inputs Right
        for (y_val = wago_y_coordinates) {
            y_pos = wall_thickness + clearance + (enclosure_extension_y / 2) + y_val;
            
            // Left side cable channel
            translate([-1, y_pos, hole_z_pos])
                rotate([0, 90, 0])
                    cylinder(d=cable_diameter, h=wall_thickness + clamp_block_width + 2);
                    
            // Right side cable channel
            translate([enclosure_outer_w - wall_thickness - clamp_block_width - 1, y_pos, hole_z_pos])
                rotate([0, 90, 0])
                    cylinder(d=cable_diameter, h=wall_thickness + clamp_block_width + 2);
        }
        
        // --- 4. Snap-In Slots (Female) ---
        snap_slot_y1 = enclosure_outer_l * 0.25;
        snap_slot_y2 = enclosure_outer_l * 0.75;
        snap_slot_z = enclosure_outer_h - 3.5;
        
        translate([wall_thickness - 0.4, snap_slot_y1, snap_slot_z]) cube([0.6, 12, 1.2]);
        translate([wall_thickness - 0.4, snap_slot_y2, snap_slot_z]) cube([0.6, 12, 1.2]);
        translate([enclosure_outer_w - wall_thickness - 0.2, snap_slot_y1, snap_slot_z]) cube([0.6, 12, 1.2]);
        translate([enclosure_outer_w - wall_thickness - 0.2, snap_slot_y2, snap_slot_z]) cube([0.6, 12, 1.2]);
        
        // Opening Pry Notch
        translate([-1, enclosure_outer_l - 15, enclosure_outer_h - 1.5])
            cube([wall_thickness + 2, 10, 2]);
            
        // Screw Holes for the Side Clamps
        clamp_screw_y1 = clamp_start_y + 3.0;                  
        clamp_screw_y2 = clamp_start_y + side_clamp_length - 3.0;   
        
        translate([wall_thickness + (clamp_block_width / 2), clamp_screw_y1, hole_z_pos - 12]) cylinder(d=strain_screw_dia - 0.2, h=14);
        translate([wall_thickness + (clamp_block_width / 2), clamp_screw_y2, hole_z_pos - 12]) cylinder(d=strain_screw_dia - 0.2, h=14);
        translate([enclosure_outer_w - wall_thickness - (clamp_block_width / 2), clamp_screw_y1, hole_z_pos - 12]) cylinder(d=strain_screw_dia - 0.2, h=14);
        translate([enclosure_outer_w - wall_thickness - (clamp_block_width / 2), clamp_screw_y2, hole_z_pos - 12]) cylinder(d=strain_screw_dia - 0.2, h=14);
        
        // Screw Holes for the Top Clamp
        top_screw_x1 = top_clamp_start_x + 3.0;
        top_screw_x2 = top_clamp_start_x + top_clamp_length - 3.0;
        top_screw_y = enclosure_outer_l - wall_thickness - (clamp_block_width / 2);
        
        translate([top_screw_x1, top_screw_y, hole_z_pos - 12]) cylinder(d=strain_screw_dia - 0.2, h=14);
        translate([top_screw_x2, top_screw_y, hole_z_pos - 12]) cylinder(d=strain_screw_dia - 0.2, h=14);
    }
    
    // --- 5. Standalone M3 Standoffs Layout ---
    translate([pcb_offset_x, pcb_offset_y, wall_thickness]) {
        single_standoff(0, 0);
        single_standoff(hole_distance_w, 0);
        single_standoff(0, hole_distance_l);
        single_standoff(hole_distance_w, hole_distance_l);
    }
}
// ====================================================================
// PARAMETRIC ENCLOSURE WITH SNAP-IN LID & INTEGRATED STRAIN RELIEF
// PART 3 OF 3: SUB-MODULES & VISUAL TEST PCB MODULE
// ====================================================================

module single_standoff(x_pos, y_pos) {
    translate([x_pos, y_pos, 0]) {
        difference() {
            cylinder(d=standoff_diameter, h=standoff_height);
            translate([0, 0, -1])
                cylinder(d=screw_diameter - 0.2, h=standoff_height + 2);
        }
    }
}

module lid() {
    cube([enclosure_outer_w, enclosure_outer_l, wall_thickness]);
    
    lip_tolerance = 0.3; 
    difference() {
        translate([wall_thickness + lip_tolerance, wall_thickness + lip_tolerance, wall_thickness])
            cube([enclosure_inner_w - (lip_tolerance * 2), enclosure_inner_l - (lip_tolerance * 2), 3]);
        translate([wall_thickness + lip_tolerance + 1.5, wall_thickness + lip_tolerance + 1.5, wall_thickness - 1])
            cube([enclosure_inner_w - (lip_tolerance * 2) - 3, enclosure_inner_l - (lip_tolerance * 2) - 3, 6]);
    }
    
    snap_ridge_y1 = enclosure_outer_l * 0.25;
    snap_ridge_y2 = enclosure_outer_l * 0.75;
    
    translate([wall_thickness + lip_tolerance - 0.3, snap_ridge_y1 + 1, wall_thickness + 1]) cube([0.5, 10, 1.0]);
    translate([wall_thickness + lip_tolerance - 0.3, snap_ridge_y2 + 1, wall_thickness + 1]) cube([0.5, 10, 1.0]);
    translate([enclosure_outer_w - wall_thickness - lip_tolerance - 0.2, snap_ridge_y1 + 1, wall_thickness + 1]) cube([0.5, 10, 1.0]);
    translate([enclosure_outer_w - wall_thickness - lip_tolerance - 0.2, snap_ridge_y2 + 1, wall_thickness + 1]) cube([0.5, 10, 1.0]);
    
    translate([-1, enclosure_outer_l - 15, 0]) cube([1, 10, wall_thickness]);
}

module strain_relief_clamp() {
    clamp_screw_y1 = 3.0; 
    clamp_screw_y2 = side_clamp_length - 3.0;
    
    difference() {
        cube([clamp_block_width, side_clamp_length, 6.0]);
        
        translate([clamp_block_width / 2, clamp_screw_y1, -1]) cylinder(d=strain_screw_dia + 0.3, h=8);
        translate([clamp_block_width / 2, clamp_screw_y2, -1]) cylinder(d=strain_screw_dia + 0.3, h=8);
        
        for (y_val = wago_y_coordinates) {
            y_pos = wall_thickness + clearance + y_val - clamp_start_y + (enclosure_extension_y / 2);
            translate([-1, y_pos, 0]) 
                rotate([0, 90, 0]) 
                    cylinder(d=cable_diameter - delta_cable_diameter, h=clamp_block_width + 2);
        }
    }
}

module top_strain_relief_clamp() {
    screw_x1 = 3.0;
    screw_x2 = top_clamp_length - 3.0;
    
    difference() {
        cube([top_clamp_length, clamp_block_width, 6.0]);
        
        translate([screw_x1, clamp_block_width / 2, -1]) cylinder(d=strain_screw_dia + 0.3, h=8);
        translate([screw_x2, clamp_block_width / 2, -1]) cylinder(d=strain_screw_dia + 0.3, h=8);
        
        translate([top_clamp_length / 2, -1, 0])
            rotate([-90, 0, 0])
                cylinder(d=cable_diameter - delta_cable_diameter, h=clamp_block_width + 2);
    }
}

// --- VISUAL TEST: PCB Mockup with synchronized positioning ---
module pcb_mockup() {
    pcb_z = wall_thickness + standoff_height;
    
    wago_w = 9.92;   // Width (X)
    wago_d = 13.9;   // Depth/Length (Y)
    wago_h = 15.5;   // Total Height (Z)
    
    // Positioned using the corrected pcb_origin variables
    translate([pcb_origin_x, pcb_origin_y, pcb_z]) {
        // Render PCB Board
        color([0.2, 0.8, 0.2, 0.5]) {
            difference() {
                cube([pcb_width, pcb_length, pcb_thickness]);
                
                // M3 Mounting holes in the PCB
                translate([ (pcb_width - hole_distance_w)/2, (pcb_length - hole_distance_l)/2, -1 ]) cylinder(d=3.2, h=pcb_thickness + 2);
                translate([ (pcb_width + hole_distance_w)/2, (pcb_length - hole_distance_l)/2, -1 ]) cylinder(d=3.2, h=pcb_thickness + 2);
                translate([ (pcb_width - hole_distance_w)/2, (pcb_length + hole_distance_l)/2, -1 ]) cylinder(d=3.2, h=pcb_thickness + 2);
                translate([ (pcb_width + hole_distance_w)/2, (pcb_length + hole_distance_l)/2, -1 ]) cylinder(d=3.2, h=pcb_thickness + 2);
            }
        }
        
        // Render technical WAGO bodies
        for (y_val = wago_y_coordinates) {
            // LEFT SIDE TERMINALS (Now perfectly aligned to the horizontal holes)
            translate([0, y_val - wago_d/2, pcb_thickness]) {
                color([0.4, 0.4, 0.4, 0.6]) cube([wago_w, wago_d, wago_h - 2.5]);
                color([1.0, 0.5, 0.0, 0.7]) translate([1, 1, wago_h - 2.5]) cube([wago_w - 2, wago_d - 2, 2.5]);
            }
            
            // RIGHT SIDE TERMINALS (Now perfectly aligned to the horizontal holes)
            translate([pcb_width - wago_w, y_val - wago_d/2, pcb_thickness]) {
                color([0.4, 0.4, 0.4, 0.6]) cube([wago_w, wago_d, wago_h - 2.5]);
                color([1.0, 0.5, 0.0, 0.7]) translate([1, 1, wago_h - 2.5]) cube([wago_w - 2, wago_d - 2, 2.5]);
            }
        }
        
        // TOP CENTER TERMINAL (Entries face the back wall)
        translate([(pcb_width - wago_d)/2, pcb_length - wago_w, pcb_thickness]) {
            rotate([0, 0, 90]) translate([0, -wago_d, 0]) {
                color([0.4, 0.4, 0.4, 0.6]) cube([wago_w, wago_d, wago_h - 2.5]);
                color([1.0, 0.5, 0.0, 0.7]) translate([1, 1, wago_h - 2.5]) cube([wago_w - 2, wago_d - 2, 2.5]);
            }
        }
    }
}
