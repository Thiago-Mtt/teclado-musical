#ifndef D_BUTTONHANDLER_H
#define D_BUTTONHANDLER_H

#include "ButtonState.h"

#define NUMBER_OF_BUTTONS 8


void    ButtonHandler_Open                 (void);
void    ButtonHandler_Close                (void);

/*Verifica estado dos botoes*/
void    ButtonHandler_Run                  (void);

/*Retorna 1 se estado do botao foi alterado*/
int     ButtonHandler_GetButtonState        (unsigned int button, ButtonState * state);

#endif /*D_BUTTONHANDLER_H*/