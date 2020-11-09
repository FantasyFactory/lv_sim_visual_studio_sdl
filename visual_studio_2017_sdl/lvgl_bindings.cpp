//#define LILYGO_WATCH_2020_V1
#ifdef LILYGO_WATCH_2020_V1
#include <TTGO.h>
#include "hardware/display.h"
#else
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#define log_i printf
#endif
#include "my_basic.h"
#include "lvgl/lvgl.h"
#include "lvgl_bindings.h"

static lv_obj_t* my_basic_output_label;
static lv_obj_t* my_basic_main_lv_obj;
lv_style_t* my_basic_main_lv_style;
LV_FONT_DECLARE(Ubuntu_16px);
#define MaxLvEvtHandlers 32

struct LvEventHandler {
    lv_obj_t* obj;
    mb_value_t routine;
    struct mb_interpreter_t* s;
    void** l;
} LvEventHandlers[MaxLvEvtHandlers];
int_t LvEvtHandlersCount = 0;


static int lvglprint(const char* format, ...) {
    char* buf = (char*)malloc(128); // Massimo 128 bytes per linea ? Nessun controllo qui !
    int result = 0;
    va_list ap;
    va_start(ap, format);
    result = vsnprintf(buf, 128, format, ap);
    lv_label_ins_text(my_basic_output_label, LV_LABEL_POS_LAST, buf);
    va_end(ap);
    free(buf);
    return result;
}

static int lvglclear(struct mb_interpreter_t* s, void** l) {
    int result = MB_FUNC_OK;

    mb_assert(s && l);
    mb_check(mb_attempt_func_begin(s, l));
    mb_check(mb_attempt_func_end(s, l));

    lv_label_set_text(my_basic_output_label, "");

    return result;
}

static void lv_obj_unref(struct mb_interpreter_t* s, void* d) {
    lv_obj_t* p = (lv_obj_t*)d;

    mb_assert(s);

    if (p != my_basic_main_lv_obj) {
        log_i("Destroy lv_obj_t * 0x%llx", (long long)p);
        free(p);
    }
}

static void* lv_obj_clone(struct mb_interpreter_t* s, void* d) {
    lv_obj_t* p = (lv_obj_t*)d;
    lv_obj_t* q = (lv_obj_t*)malloc(sizeof(lv_obj_t));

    mb_assert(s);
    if (q) memcpy(q, p, sizeof(lv_obj_t));

    return q;
}

lv_coord_t xor_coords(lv_area_t c) {
    return c.x1 ^ c.x2 ^ c.y1 ^ c.y2;
}

static unsigned int lv_obj_hash(struct mb_interpreter_t* s, void* d) {
    lv_obj_t* p = (lv_obj_t*)d;

    mb_assert(s);

    return xor_coords(p->coords);
}

static int lv_obj_cmp(struct mb_interpreter_t* s, void* l, void* r) {
    lv_obj_t* p = (lv_obj_t*)l;
    lv_obj_t* q = (lv_obj_t*)r;
    int tmp = 0;

    mb_assert(s);

    tmp = xor_coords(p->coords) - xor_coords(q->coords);
    return tmp;
}

static int lv_obj_fmt(struct mb_interpreter_t* s, void* d, char* b, unsigned z) {
    int result = 0;
    lv_obj_t* p = (lv_obj_t*)d;

    mb_assert(s);

    result = snprintf(b, z, "[0x%llx]", (long long)p) + 1;

    return result;
}

static int _lv_btn_create(struct mb_interpreter_t* s, void** l) {
    int result = MB_FUNC_OK;

    mb_assert(s && l);
    mb_check(mb_attempt_open_bracket(s, l));

    {
        lv_obj_t* p;

#ifdef SimpleUsertype
        mb_check(mb_pop_usertype(s, l, &p));
#else
        mb_value_t arg;
        mb_make_nil(arg);
        mb_check(mb_pop_value(s, l, &arg));
        mb_check(mb_get_ref_value(s, l, arg, (void**)&p));
#endif

        int_t x1, y1, x2, y2;
        mb_check(mb_pop_int(s, l, &x1));
        mb_check(mb_pop_int(s, l, &y1));
        mb_check(mb_pop_int(s, l, &x2));
        mb_check(mb_pop_int(s, l, &y2));

        lv_obj_t* _p = lv_btn_create(p, NULL);
        lv_obj_set_pos(_p, x1, y1);
        lv_obj_set_size(_p, x2, y2);
        lv_task_handler();

#ifdef SimpleUsertype
        mb_value_t ret;
        mb_make_nil(ret);
        ret.type = MB_DT_USERTYPE;
        ret.value.usertype = _p;
        mb_check(mb_push_usertype(s, l, &ret));
#else
        mb_value_t ret;
        mb_check(mb_make_ref_value(s, _p, &ret, lv_obj_unref, lv_obj_clone, lv_obj_hash, lv_obj_cmp, lv_obj_fmt));
        mb_check(mb_push_value(s, l, ret));
        mb_check(mb_unref_value(s, l, arg));
#endif

        log_i("lv_btn_create(0x%llx)=0x%llx", (long long)p, (long long)_p);
    }

    mb_check(mb_attempt_close_bracket(s, l));
    lv_task_handler();
    return result;
}


static int _lv_label_create(struct mb_interpreter_t* s, void** l) {
    int result = MB_FUNC_OK;

    mb_assert(s && l);
    mb_check(mb_attempt_open_bracket(s, l));

    {
        lv_obj_t* p;

#ifdef SimpleUsertype
        mb_check(mb_pop_usertype(s, l, &p));
#else
        mb_value_t arg;
        mb_make_nil(arg);
        mb_check(mb_pop_value(s, l, &arg));
        mb_check(mb_get_ref_value(s, l, arg, (void**)&p));
#endif

        int_t x1, y1, x2, y2;
        mb_check(mb_pop_int(s, l, &x1));
        mb_check(mb_pop_int(s, l, &y1));
        mb_check(mb_pop_int(s, l, &x2));
        mb_check(mb_pop_int(s, l, &y2));

        lv_obj_t* _p = lv_label_create(p, NULL);
        lv_obj_set_pos(_p, x1, y1);
        lv_obj_set_size(_p, x2, y2);
        lv_task_handler();

#ifdef SimpleUsertype
        mb_value_t ret;
        mb_make_nil(ret);
        ret.type = MB_DT_USERTYPE;
        ret.value.usertype = _p;
        mb_check(mb_push_usertype(s, l, &ret));
#else
        mb_value_t ret;
        mb_check(mb_make_ref_value(s, _p, &ret, lv_obj_unref, lv_obj_clone, lv_obj_hash, lv_obj_cmp, lv_obj_fmt));
        mb_check(mb_push_value(s, l, ret));
        mb_check(mb_unref_value(s, l, arg));
#endif

        log_i("lv_label_create(0x%llx)=0x%llx", (long long)p, (long long)_p);
    }

    mb_check(mb_attempt_close_bracket(s, l));
    lv_task_handler();
    return result;
}


static int _lv_label_set_text(struct mb_interpreter_t* s, void** l) {
    int result = MB_FUNC_OK;

    mb_assert(s && l);
    mb_check(mb_attempt_open_bracket(s, l));

    {
        lv_obj_t* p;

#ifdef SimpleUsertype
        mb_check(mb_pop_usertype(s, l, &p));
#else
        mb_value_t arg;
        mb_make_nil(arg);
        mb_check(mb_pop_value(s, l, &arg));
        mb_check(mb_get_ref_value(s, l, arg, (void**)&p));
#endif

        char* str = 0;
        mb_check(mb_pop_string(s, l, &str));

        lv_label_set_text(p, str);
        lv_task_handler();

#ifndef SimpleUsertype
        mb_check(mb_unref_value(s, l, arg));
#endif 
    }

    mb_check(mb_attempt_close_bracket(s, l));
    lv_task_handler();
    return result;
}


static int _get_main_lv_obj(struct mb_interpreter_t* s, void** l) {
    int result = MB_FUNC_OK;

    mb_assert(s && l);
    mb_check(mb_attempt_open_bracket(s, l));

    {
        mb_value_t ret;
        mb_make_nil(ret);
#ifdef SimpleUsertype
        ret.type = MB_DT_USERTYPE;
        ret.value.usertype = my_basic_main_lv_obj;
#else
        mb_make_ref_value(s, my_basic_main_lv_obj, &ret, lv_obj_unref, lv_obj_clone, lv_obj_hash, lv_obj_cmp, lv_obj_fmt);
#endif 
        mb_check(mb_push_value(s, l, ret));
        log_i("Main LvObj * 0x%llx", (long long)my_basic_main_lv_obj);
    }

    mb_check(mb_attempt_close_bracket(s, l));
    lv_task_handler();
    return result;
}



static void _LvEventHandler(lv_obj_t* obj, lv_event_t event) {

    switch (event) {
    case(LV_EVENT_CLICKED):
    {
        for (int i = 0; i < LvEvtHandlersCount; i++) {
            if (LvEventHandlers[i].obj == obj) {
                log_i("Handle event %d for object 0x%llx at 0x%llx", event, (long long)LvEventHandlers[i].obj, (long long)LvEventHandlers[i].routine.value.usertype);
                mb_value_t args[2];
                mb_make_nil(args[0]);

#ifdef SimpleUsertype
                args[0].type = MB_DT_USERTYPE;
                args[0].value.usertype = LvEventHandlers[i].obj;
#else
                lv_obj_t* _p = (lv_obj_t*)lv_obj_clone(LvEventHandlers[i].s, LvEventHandlers[i].obj);
                mb_make_ref_value(LvEventHandlers[i].s, _p, &args[0], lv_obj_unref, lv_obj_clone, lv_obj_hash, lv_obj_cmp, lv_obj_fmt);
#endif
                mb_make_nil(args[1]);
                args[1].type = MB_DT_INT;
                args[1].value.integer = event;

                mb_value_t ret;
                mb_make_nil(ret);
                mb_eval_routine(LvEventHandlers[i].s, LvEventHandlers[i].l, LvEventHandlers[i].routine, args, 2, &ret); /* Evaluate the "FUN" routine with arguments, and get the returned value */
                printf("Returned %d.\n", ret.value.integer);
            }
        }

        break;
    }
    }

}

static int _SetLvEventHandler(struct mb_interpreter_t* s, void** l) {
    int result = MB_FUNC_OK;

    mb_assert(s && l);
    mb_check(mb_attempt_open_bracket(s, l));

    {
        lv_obj_t* p;
        mb_value_t arg;
        mb_make_nil(arg);

#ifdef SimpleUsertype
        mb_check(mb_pop_usertype(s, l, &arg));
        p = arg.value.usertype;
#else
        mb_check(mb_pop_value(s, l, &arg));
        mb_check(mb_get_ref_value(s, l, arg, (void**)&p));
#endif

        char* str = NULL;
        mb_check(mb_pop_string(s, l, &str));
        for (int i = 0; str[i] != '\0'; i++) {
            if (str[i] >= 'a' && str[i] <= 'z') {
                str[i] = str[i] - 32;
            }
        }

        mb_value_t routine;
        mb_make_nil(routine);
        mb_check(mb_get_routine(s, l, str, &routine));   /* Get the routine from name*/

        LvEventHandlers[LvEvtHandlersCount].obj = p;
        LvEventHandlers[LvEvtHandlersCount].routine = routine;
        LvEventHandlers[LvEvtHandlersCount].s = s;
        LvEventHandlers[LvEvtHandlersCount].l = l;
        lv_obj_set_event_cb(p, _LvEventHandler);

        log_i("Set handler %d for object x%llx as %s\n", LvEvtHandlersCount, (long long)p, str);
        LvEvtHandlersCount++;

    }

    mb_check(mb_attempt_close_bracket(s, l));
    lv_task_handler();
    return result;
}


void enableLVGLprint(struct mb_interpreter_t* bas, lv_obj_t* l) {
    my_basic_output_label = l;
    mb_set_printer(bas, lvglprint);
    mb_register_func(bas, "CLS", lvglclear);
}

void enableLVGL(struct mb_interpreter_t* bas, lv_obj_t* p, lv_style_t* s) {
    my_basic_main_lv_obj = p;
    my_basic_main_lv_style = s;
    //mb_begin_module(s, "LVGL");
    mb_register_func(bas, "GetMainLvObj", _get_main_lv_obj);
    mb_register_func(bas, "LvButtonCreate", _lv_btn_create);
    mb_register_func(bas, "LvLabelCreate", _lv_label_create);
    mb_register_func(bas, "LvLabelSetText", _lv_label_set_text);
    mb_register_func(bas, "SetLvEventHandler", _SetLvEventHandler);
    //mb_end_module(s);
}
