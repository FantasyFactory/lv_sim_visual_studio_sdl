/**
* @file main
*
*/

/*********************
*      INCLUDES
*********************/
#include <stdlib.h>
#include <Windows.h>
#include <SDL.h>
#include "lvgl/lvgl.h"
#include "lv_drivers/display/monitor.h"
#include "lv_drivers/indev/mouse.h"
#include "lv_drivers/indev/keyboard.h"
#include "lv_examples/lv_examples.h"

/*********************
*      My Basic LvGl
*********************/
#include "my_basic.h"
#include "lvgl_bindings.h"
#include <assert.h>

lv_obj_t* babau(lv_obj_t* p, lv_style_t* s);
static void _on_error(struct mb_interpreter_t* s, mb_error_e e, const char* m, const char* f, int p, unsigned short row, unsigned short col, int abort_code);
int bas_delay(struct mb_interpreter_t* s, void** l);
#define log_e printf




/*********************
*      DEFINES
*********************/

/**********************
*      TYPEDEFS
**********************/

/**********************
*  STATIC PROTOTYPES
**********************/
static void hal_init(void);
static int tick_thread(void *data);

/**********************
*  STATIC VARIABLES
**********************/
static lv_indev_t * kb_indev;

/**********************
*      MACROS
**********************/

/**********************
*   GLOBAL FUNCTIONS
**********************/

int main(int argc, char** argv)
{
    /*Initialize LittlevGL*/
    lv_init();

    /*Initialize the HAL for LittlevGL*/
    hal_init();

    /*
     * Demos, benchmarks, and tests.
     *
     * Uncomment any one (and only one) of the functions below to run that
     * item.
     */

    //lv_demo_widgets();
    //lv_demo_benchmark();
    //lv_demo_keypad_encoder();
    //lv_demo_printer();
    //lv_demo_stress();
    //lv_ex_get_started_1();
    //lv_ex_get_started_2();
    //lv_ex_get_started_3();

    //lv_ex_style_1();
    //lv_ex_style_2();
    //lv_ex_style_3();
    //lv_ex_style_4();
    //lv_ex_style_5();
    //lv_ex_style_6();
    //lv_ex_style_7();
    //lv_ex_style_8();
    //lv_ex_style_9();
    //lv_ex_style_10();
    //lv_ex_style_11();

    /*
     * There are many examples of individual widgets found under the
     * lv_examples/src/lv_ex_widgets directory.  Here are a few sample test
     * functions.  Look in that directory to find all the rest.
     */
    //lv_ex_arc_1();
    //lv_ex_cpicker_1();
    //lv_ex_gauge_1();
    //lv_ex_img_1();
    //lv_ex_tileview_1();


    static lv_style_t MyBasic_output_style;
    lv_style_init(&MyBasic_output_style);
    lv_obj_t* MyBasic_output = lv_scr_act();
    lv_obj_add_style(MyBasic_output, LV_OBJ_PART_MAIN, &MyBasic_output_style);



#ifndef skipthis
    struct mb_interpreter_t* bas = NULL;
    char* buffer =  "CrLf = chr(13) + chr(10)\n"
                    "l=0\n"
                    "def OnButton1Click()\n"
                    "  LvLabelSetText(l, \"Button 1 CLICKED\")\n"
                    "  print \"Variable scope l \", l, CrLf\n"
                    "enddef\n"         
                    "print \"Hello basic!\", CrLf\n"                 
                    "x = GetMainLvObj()\n"
                    "print \"Get main lv obj \", x, CrLf\n"
                    "b = LvButtonCreate(x, 32, 64, 160, 48)\n"
                    "print \"button 1 \", b, CrLf\n"
                    "b1 = LvButtonCreate(x, 32, 128, 160, 48)\n"
                    "print \"button 2 \", b1, CrLf\n"
                    "l = LvLabelCreate(b, 8, 8, 128, 32)\n"
                    "print \"Create a label into button 1\", l, crlf\n"            
                    "l1 = LvLabelCreate(x, 32, 192, 128, 32)\n"
                    "print \"Create a label into main lv obj\", l1, CrLf\n"
                    "LvLabelSetText(l, \"Button 1 caption\")\n"
                    "'OnButton1Click()\n"
                    "t = \"Ciao\"\n"
                    "LvOnClick(b, \"OnButton1Click(t)\")\n"
                    "for i=1 to 10\n"
                    "t = \"Label 2 text \" + str(i)\n"
                    "LvLabelSetText(l1, t)\n"
                    "delay(1000)\n"
                    "print t, CrLf\n"
                    "next\n";
    printf("%s", buffer);
    mb_init();
    mb_open(&bas);
    mb_register_func(bas, "DELAY", bas_delay);
    mb_set_error_handler(bas, _on_error);
    enableLVGL(bas, MyBasic_output, &MyBasic_output_style);
    mb_load_string(bas, buffer, true);
    mb_run(bas, true);

#else
    lv_obj_t* _p1 = babau(MyBasic_output, &MyBasic_output_style);
#endif 

    while (1) {
        /* Periodically call the lv_task handler.
        * It could be done in a timer interrupt or an OS task too.*/
        lv_task_handler();
        Sleep(10);       /*Just to let the system breathe */
    }

#ifndef skipthis
    mb_close(&bas);
    mb_dispose();
#endif

    return 0;
}



lv_obj_t* babau(lv_obj_t* p, lv_style_t* s) {
    static lv_obj_t* my_basic_main_lv_obj;
    lv_style_t* my_basic_main_lv_style;
    my_basic_main_lv_obj = p;
    my_basic_main_lv_style = s;

    

    lv_obj_t* _p = (lv_obj_t*)malloc(sizeof(lv_obj_t));
    if (_p) memcpy(_p, my_basic_main_lv_obj, sizeof(lv_obj_t)); else return MB_EXTENDED_ABORT;

    LV_FONT_DECLARE(Ubuntu_16px);
    int x1=8, y1=100, x2=200, y2=40;

    lv_style_t new_lv_label_main_style;
    lv_style_init(&new_lv_label_main_style);
    lv_style_copy(&new_lv_label_main_style, my_basic_main_lv_style);
    //lv_style_set_text_font(&new_lv_label_main_style, LV_STATE_DEFAULT, &Ubuntu_16px);
    lv_style_set_text_color(&new_lv_label_main_style, LV_STATE_DEFAULT, LV_COLOR_BLUE);
    lv_style_set_bg_color(&new_lv_label_main_style, LV_OBJ_PART_MAIN, LV_COLOR_YELLOW);
    lv_task_handler();

    //lv_obj_t* _p1 = lv_btn_create(_p, NULL);
    lv_obj_t* _p1 = lv_btn_create(my_basic_main_lv_obj, NULL);
    lv_task_handler();
    //lv_obj_add_style(_p1, LV_OBJ_PART_MAIN, &new_lv_label_main_style);
    lv_obj_set_pos(_p1, x1, y1);
    lv_obj_set_size(_p1, x2, y2);

    return _p1;

}


/**********************
*   STATIC FUNCTIONS
**********************/

static void _on_error(struct mb_interpreter_t* s, mb_error_e e, const char* m, const char* f, int p, unsigned short row, unsigned short col, int abort_code) {
    mb_unrefvar(s);
    mb_unrefvar(p);

    if (e != SE_NO_ERR) {
        if (f) {
            if (e == SE_RN_WRONG_FUNCTION_REACHED) {
                log_e(
                    "Error:\n    Ln %d, Col %d in Func: %s\n    Code %d, Abort Code %d\n    Message: %s.\n",
                    row, col, f,
                    e, abort_code,
                    m
                );
            }
            else {
                log_e(
                    "Error:\n    Ln %d, Col %d in File: %s\n    Code %d, Abort Code %d\n    Message: %s.\n",
                    row, col, f,
                    e, e == SE_EA_EXTENDED_ABORT ? abort_code - MB_EXTENDED_ABORT : abort_code,
                    m
                );
            }
        }
        else {
            log_e(
                "Error:\n    Ln %d, Col %d\n    Code %d, Abort Code %d\n    Message: %s.\n",
                row, col,
                e, e == SE_EA_EXTENDED_ABORT ? abort_code - MB_EXTENDED_ABORT : abort_code,
                m
            );
        }
    }
}

int bas_delay(struct mb_interpreter_t* s, void** l) {
    int result = MB_FUNC_OK;
    int64_t n = 0;
    
    mb_assert(s && l);
    mb_check(mb_attempt_open_bracket(s, l));
    mb_check(mb_pop_int(s, l, &n));
    mb_check(mb_attempt_close_bracket(s, l));

    for (; n > 0; n -= 20) {
        SDL_Delay(20);
        lv_task_handler();
    }
    //Sleep(n);
    return result;
}


/**
* Initialize the Hardware Abstraction Layer (HAL) for the Littlev graphics library
*/
static void hal_init(void)
{
    /* Add a display
    * Use the 'monitor' driver which creates window on PC's monitor to simulate a display*/
    monitor_init();

    static lv_disp_buf_t disp_buf1;
    static lv_color_t buf1_1[LV_HOR_RES_MAX * 120];
    lv_disp_buf_init(&disp_buf1, buf1_1, NULL, LV_HOR_RES_MAX * 120);

    lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);            /*Basic initialization*/
    disp_drv.buffer = &disp_buf1;
    disp_drv.flush_cb = monitor_flush;
    lv_disp_drv_register(&disp_drv);

    /* Add the mouse (or touchpad) as input device
    * Use the 'mouse' driver which reads the PC's mouse*/
    mouse_init();
    lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);          /*Basic initialization*/
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = mouse_read;         /*This function will be called periodically (by the library) to get the mouse position and state*/
    lv_indev_drv_register(&indev_drv);

    /* If the PC keyboard driver is enabled in`lv_drv_conf.h`
    * add this as an input device. It might be used in some examples. */
#if USE_KEYBOARD
    lv_indev_drv_t kb_drv;
    lv_indev_drv_init(&kb_drv);
    kb_drv.type = LV_INDEV_TYPE_KEYPAD;
    kb_drv.read_cb = keyboard_read;
    kb_indev = lv_indev_drv_register(&kb_drv);
#endif

    /* Tick init.
    * You have to call 'lv_tick_inc()' in every milliseconds
    * Create an SDL thread to do this*/
    SDL_CreateThread(tick_thread, "tick", NULL);
}

/**
* A task to measure the elapsed time for LittlevGL
* @param data unused
* @return never return
*/
static int tick_thread(void *data)
{
    while (1) {
        lv_tick_inc(5);
        SDL_Delay(5);   /*Sleep for 1 millisecond*/
    }

    return 0;
}
