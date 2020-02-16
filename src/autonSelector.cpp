#include "autonSelector.hpp"


lv_obj_t* screen = lv_cont_create(lv_scr_act(), NULL);
lv_obj_t* label = lv_label_create(lv_scr_act(), NULL);
lv_obj_t* button = lv_btn_create(lv_scr_act(), NULL);
lv_obj_t* buttonLabel = lv_label_create(button, NULL);

void autonSelectorInit()
{
    lv_obj_set_size(screen, 476, 272);
    lv_obj_set_pos(label, 10, 10);
    lv_obj_align(label, NULL, LV_ALIGN_IN_TOP_MID, 0, 30);
    lv_obj_set_size(label, 20, 250);
    lv_obj_set_style(label, &lv_style_pretty_color);

    lv_btn_set_action(button, LV_BTN_ACTION_CLICK, buttonPressed);
    lv_obj_align(button, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, -30);
    lv_obj_set_pos(button, 10, 60);
    lv_obj_set_size(button, 250, 50);
    lv_obj_set_style(button, &lv_style_pretty_color);

    lv_label_set_text(buttonLabel, "Change Auton");
    lv_obj_align(buttonLabel, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style(buttonLabel, &lv_style_pretty_color);
    autonSelectorReset();
}

lv_res_t buttonPressed(lv_obj_t * obj)
{
    autonSelection = autonSelection == autons.size() - 1 ? 0 : autonSelection + 1;
    autonSelectorReset();
    return LV_RES_OK;
}

void autonSelectorReset()
{
    lv_label_set_text(label, autons[autonSelection].code.c_str());
}