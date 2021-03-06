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

static void _on_error(struct mb_interpreter_t* s, mb_error_e e, const char* m, const char* f, int p, unsigned short row, unsigned short col, int abort_code);
int bas_delay(struct mb_interpreter_t* s, void** l);
#define log_e printf

static lv_obj_t *my_basic_app_main_tile = NULL;
static lv_style_t my_basic_app_main_style;

static lv_obj_t* my_basic_cont;
static lv_style_t my_basic_cont_main_style;

static void event_handler(lv_obj_t* obj, lv_event_t event);

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



    lv_style_init(&my_basic_app_main_style);
    lv_style_set_bg_opa(&my_basic_app_main_style, LV_STATE_DEFAULT, LV_OPA_COVER);
    lv_style_set_bg_color(&my_basic_app_main_style, LV_STATE_DEFAULT, LV_COLOR_SILVER);
    my_basic_app_main_tile = lv_obj_create(lv_scr_act(), NULL);
    lv_obj_set_size(my_basic_app_main_tile, lv_disp_get_hor_res(NULL), lv_disp_get_ver_res(NULL));
    lv_obj_add_style(my_basic_app_main_tile, LV_OBJ_PART_MAIN, &my_basic_app_main_style);

    /************ my_basic_cont main container (for lvgl integration) *********/
    my_basic_cont = lv_obj_create(my_basic_app_main_tile, NULL);
    lv_obj_set_size(my_basic_cont, lv_disp_get_hor_res(NULL), lv_disp_get_ver_res(NULL) - 64);
    lv_style_init(&my_basic_cont_main_style); 
    lv_style_copy(&my_basic_cont_main_style, &my_basic_app_main_style);
    lv_style_set_bg_color(&my_basic_cont_main_style, LV_OBJ_PART_MAIN, LV_COLOR_BLUE);
    lv_obj_add_style(my_basic_cont, LV_OBJ_PART_MAIN, &my_basic_cont_main_style);
    lv_obj_align(my_basic_cont, my_basic_app_main_tile, LV_ALIGN_IN_TOP_MID, 0, 32);

    struct mb_interpreter_t* bas = NULL;
    char* buffer =  "CrLf = chr(13) + chr(10)\n"
                    "LV_EVENT_CLICKED = 6\n"
                    "GlobalVal = 100\n"
                    "print \"Hello basic!\", CrLf\n"
                    "x = GetMainLvObj()\n"
                    "print \"Get main lv obj \", x, CrLf\n"
                    "b1 = LvButtonCreate(x, 32, 16, 160, 48)\n"
                    "print \"button 1 \", b1, CrLf\n"
                    "b2 = LvButtonCreate(x, 32, 80, 160, 48)\n"
                    "print \"button 2 \", b2, CrLf\n"
                    "lx = LvLabelCreate(x, 32, 144, 160, 32)\n"
                    "print \"Create a label into main lv obj\", lx, CrLf\n"
                    "lb1 = LvLabelCreate(b1, 8, 8, 128, 32)\n"
                    "print \"Create a label into button 1\", lb1, crlf\n"            
                    "lb2 = LvLabelCreate(b2, 8, 8, 128, 32)\n"
                    "print \"Create a label into button 2\", lb2, crlf\n"
                    "LvLabelSetText(lx, \"Current value:\" + str(GlobalVal) )\n"
                    "LvLabelSetText(lb1, \"Click to increase\")\n"
                    "LvLabelSetText(lb2, \"Click to decrease\")\n"
                    "'r=LvMsgbox(\"Continue ?\",\"Yes\", \"No\",\"\",\"\")\n"
                    "'if r=\"No\" then\n"
                    "'  print \"Ciaone!\"\n"
                    "'  end\n"
                    "'endif\n"
                    "def UpdateLabel()\n"
                    "     t = \"New value: \" + str(GlobalVal)\n"
                    "     LvLabelSetText(lx, t )\n"
                    "enddef\n"
                    "def OnButton1(evt)\n"
                    "  if evt=LV_EVENT_CLICKED then\n"
                    "     GlobalVal = GlobalVal + 10\n"
                    "     UpdateLabel()\n"
                    "  endif\n"
                    "enddef\n"
                    "def OnButton2(evt)\n"
                    "  if evt=LV_EVENT_CLICKED then\n"
                    "     GlobalVal = GlobalVal - 10\n"
                    "     UpdateLabel()\n"
                    "  endif\n"
                    "enddef\n"
                    "SetLvEventHandler(b1, \"OnButton1\")\n"
                    "SetLvEventHandler(b2, \"OnButton2\")\n"
                    "t = \"Multitasking: \"\n"
                    "for i=1 to 10\n"
                    "  GlobalVal = GlobalVal +1\n"
                    "  t = \"i = \" + str(i) + \" GlobalVal = \" + str(GlobalVal)\n"
                    "  print t, CrLf\n"
                    "  UpdateLabel()\n"
                    "  delay(2000)\n" 
                    "next\n";
    printf("%s", buffer);
    mb_init();
    mb_open(&bas);
    mb_register_func(bas, "DELAY", bas_delay);
    mb_set_error_handler(bas, _on_error);
    enableLVGL(bas, my_basic_cont, &my_basic_cont_main_style);
    mb_load_string(bas, buffer, true);
    mb_run(bas, true);

    //mb_close(&bas);
    mb_dispose();
    lv_obj_clean(my_basic_cont);
    lv_obj_del(my_basic_cont);

    return 0;
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
