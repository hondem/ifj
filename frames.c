#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "instrlist.h"
#include "frames.h"
#include "interpret.h"
#include "error_codes.h"

frame_t *new_frame = NULL;
frame_t *active_frame = NULL;

void fr_print(frame_t *frame) {
    printf("frame: next_instr: %p ( ", (void *) frame->next_instr);
    
    if (frame->next_instr != NULL)
        print_instr(&(frame->next_instr->instruction));

    printf(" )\n");

    frame_item_t *temp = frame->first_item;

    while (temp != NULL) {
        if (temp->var != NULL)
            printf("frame_item: var: %p, id: %s, value: %i\n",
                (void *)temp->var, temp->var->id, temp->value.ival);
        else
            printf("some error/n");
        temp = temp->next;
    }
}

void fr_print_frames() {
    if (new_frame != NULL) {
        printf("INTERPRET: new_frame: \n");
        fr_print(new_frame);
    }

    frame_t *temp = active_frame;

    while (temp != NULL) {
        printf("INTERPRET: active_frame: \n");
        fr_print(temp);
        temp = temp->next;
    }
}

int fr_add_item(frame_t *frame, symtab_elem_t *var) {
    frame_item_t *temp = malloc(sizeof(frame_item_t));

    if (temp == NULL)
        return INTERNAL_ERROR;

    temp->var = var;

    st_value_t null_val;
    null_val.ival = 0;
    temp->value = null_val;

    temp->next = frame->first_item;
    frame->first_item = temp;

    return 0;
}

frame_item_t *fr_get_item(frame_t *frame, symtab_elem_t *var) {
    frame_item_t *temp = frame->first_item;

    while (temp != NULL) {
        if (temp->var == var)
            return temp;

        temp = temp->next;
    }

    return NULL;
}

int fr_set(frame_t *frame, symtab_elem_t *var, st_value_t val) {
    if (frame == NULL)
        return INTERNAL_ERROR;

    frame_item_t *item = fr_get_item(frame, var);

    if (item == NULL) {
        if (fr_add_item(frame, var) == INTERNAL_ERROR)
            return INTERNAL_ERROR;

        item = fr_get_item(frame, var);

        if (item == NULL)
            return INTERNAL_ERROR;
    }

    item->value = val;

    return 0;
}

int fr_get(frame_t *frame, symtab_elem_t *var, st_value_t *val) {
    if (frame == NULL)
        return INTERNAL_ERROR;

    frame_item_t *item = fr_get_item(frame, var);

    if (item == NULL) {
        if (fr_add_item(frame, var) == INTERNAL_ERROR)
            return INTERNAL_ERROR;

        item = fr_get_item(frame, var);

        if (item == NULL)
            return INTERNAL_ERROR;
    }

    *val = item->value;

    return 0;
}

void fr_free(frame_t **frame) {
    frame_item_t *item = (*frame)->first_item;

    while (item != NULL) {
        frame_item_t *temp = item;
        item = item->next;
        free(temp);
    }

    free(*frame);
    *frame = NULL;
}

int call_instr(tListOfInstr *instrlist, inter_stack *stack, symtab_elem_t *func) {
    printf("INTERPRET: inside call instruction, params: %p %p %p\n", (void *) instrlist, (void *) stack, (void *) func);

    new_frame = malloc(sizeof(frame_t));

    if (new_frame == NULL)
        return INTERNAL_ERROR;

    symtab_elem_t *param = func->first_param;

    while (param != NULL) {
        fr_add_item(new_frame, param);

        inter_value param_value;
        stack_inter_Top(&param_value, stack);
        stack_inter_Pop(stack);
        fr_set(new_frame, param, param_value.union_value);

        printf("INTERPRET: adding param to frame, id: %s\n", param->id);
        param = param->next_param;
    }

    new_frame->next = active_frame;
    active_frame = new_frame;
    new_frame = NULL;

    if (strcmp(func->id, "run") == 0) {
        printf("!!!! setting next_instr to %p\n", (void *) NULL);
        active_frame->next_instr = NULL;
    }
    else {
        printf("!!!! setting next_instr to %p\n", (void *)instrlist->active->nextItem);
        active_frame->next_instr = instrlist->active->nextItem;
    }
    printf("!!!! going to %p\n", (void *)func->first_instr);
    listGoto(instrlist, func->first_instr);

    fr_print_frames();

    return ER_OK;
}

int return_instr(symtab_t *symtab, tListOfInstr *instrlist) {
    printf("inside return instruction, params: %p %p\n", (void *) symtab, (void *) instrlist);

    if (active_frame == NULL)
        return INTERNAL_ERROR;

    printf("!!!! going to %p\n", (void *)active_frame->next_instr);
    listGoto(instrlist, active_frame->next_instr);

    frame_t *temp = active_frame;
    active_frame = active_frame->next;

    fr_free(&temp);

    fr_print_frames();

    return ER_OK;
}

st_value_t get_value(symtab_elem_t *var) {
    st_value_t val;
    fr_get(active_frame, var, &val);
    return val;
}

void set_value(symtab_elem_t *var, inter_value *value) {
    fr_set(active_frame, var, value->union_value);
    fr_print_frames();
}
