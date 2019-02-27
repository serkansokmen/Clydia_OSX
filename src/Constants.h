//
//  Constants.h
//  Clydia_old
//
//  Created by Serkan Sokmen on 27.02.2019.
//

#pragma once

#define PXL_SCALE           1.0

#define GRABBER_WIDTH       320
#define GRABBER_HEIGHT      240

#define IMAGE_DRAW_RES_X    50
#define IMAGE_DRAW_RES_Y    50
#define DRAG_DRAW_RES       5

#define DIFFUSION_MIN       0.024 * PXL_SCALE
#define DIFFUSION_MAX       0.16 * PXL_SCALE

#define USE_COLOR_TRACKER   false
#define SETTINGS_FILE_NAME  "settings.xml"

// strings
#define CLYD_DRAWING_Flat_colors        "CLYD:DRAWING:Flat_colors"
#define CLYD_DRAWING_Draw_brush         "CLYD:DRAWING:Draw_brush"
#define CLYD_DRAWING_Freehand           "CLYD:DRAWING:Freehand"
#define CLYD_DRAWING_Total_branches     "CLYD:DRAWING:Total_branches"
#define CLYD_DRAWING_Branch_length      "CLYD:DRAWING:Branch_length"
#define CLYD_DRAWING_Branch_diffusion   "CLYD:DRAWING:Branch_diffusion"
#define CLYD_DRAWING_Branch_color       "CLYD:DRAWING:Branch_color"
#define CLYD_DRAWING_BG_color           "CLYD:DRAWING:BG_color"
#define CLYD_DRAWING_Clear_on_Draw      "CLYD:DRAWING:Clear_on_Draw"
#define CLYD_ANIMATOR_Enabled           "CLYD:ANIMATOR:Enabled"
#define CLYD_ANIMATOR_Speed             "CLYD:ANIMATOR:Speed"
#define CLYD_ANIMATOR_Radius_X          "CLYD:ANIMATOR:Radius_X"
#define CLYD_ANIMATOR_Radius_Y          "CLYD:ANIMATOR:Radius_y"

#define CANVAS_SIZE_35                  4134
#define CANVAS_SIZE_50                  5906
#define CANVAS_SIZE_80                  9449
#define CANVAS_WIDTH                    5906
#define CANVAS_HEIGHT                   5906

// branches
#define CL_BRANCH_AGE_MIN           200.0f
#define CL_BRANCH_AGE_MAX           600.0f
#define CL_BRANCH_AGING_COEFF_MIN   1.0f
#define CL_BRANCH_AGING_COEFF_MAX   4.0f

