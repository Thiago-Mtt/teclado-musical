/*
 * Lista de testes para o módulo ButtonHandler
 * Objetivo: Detectar quando um botão troca de estado e enviar a nota sendo pressionada/solta
 * para o módulo synth
 * 
 * Iniciar ButtonReader (parte que le os pinos no hardware) durante inicializacao (open)    OK
 * Fechar ButtonReader durante fechamento                                                   OK
 * 
 * 8 Botoes (0-7) iniciados como abertos                                                    OK
 * Botoes inexistentes (8-...) retornam estado de erro                                      OK
 * 
 * Pressionar botao troca estado para pressionado                                           OK
 * Soltar botao troca estado para aberto                                                    OK
 * Pressionar N botoes troca os estados corretamente                                        OK
 * Soltar N-1 botoes trocam o estado corretamente                                           OK
 * 
 * Leitura de botão após inicialização não retorna alteração de estado do botão             OK
 * Retorna estado alterado (1)o apos pressionar botao da primeira vez que funcao e chamada  OK
 * Retorna estado inalterado (0) da segunda vez que funcao e chamada                        OK  
 * Retorna estado alterado apos soltar botao                                                OK
 * Retorna estado inalterado apos segunda chamada da funcao                                 OK
 * Detectar estado alterado de multiplos botoes
 * 
 * 
*/

#include "unity.h"
#include "unity_fixture.h"

#include "ButtonHandler.h"
#include "FakeButtonReader.h"

TEST_GROUP(ButtonHandler);

TEST_SETUP(ButtonHandler)
{
    ButtonHandler_Open();
}

TEST_TEAR_DOWN(ButtonHandler)
{
    ButtonHandler_Close();
}

TEST(ButtonHandler, OpenAndClose)
{
    ButtonHandler_Open();
    ButtonHandler_Close();
}

TEST(ButtonHandler, InitializeButtonReader)
{
    int isInitialized = FakeButtonReader_IsInitialized();
    TEST_ASSERT(isInitialized);
}

TEST(ButtonHandler, CloseButtonReaderOnClose)
{
    ButtonHandler_Close();
    int isInitialized = FakeButtonReader_IsInitialized();

    TEST_ASSERT(!isInitialized);
}

TEST(ButtonHandler, ButtonsInitializedAsOpen)
{
    ButtonState state;

    for(unsigned int i = 0; i < NUMBER_OF_BUTTONS; i++)
    {
        ButtonHandler_GetButtonState(i, &state);

        TEST_ASSERT(state == open);
    }
}

TEST(ButtonHandler, ButtonsOutOfRangeReadErrorValue)
{
    ButtonState state;

    ButtonHandler_GetButtonState(NUMBER_OF_BUTTONS, &state);

    TEST_ASSERT(state == error);
}

TEST(ButtonHandler, ButtonPressedChangesStateToPressed)
{
    unsigned int button = 0;
    ButtonState state;

    FakeButtonReader_PressButton(button);
    ButtonHandler_Run();
    ButtonHandler_GetButtonState(button, &state);

    TEST_ASSERT(state == pressed);
}

TEST(ButtonHandler, ButtonReleaseChangesStateToOpen)
{
    unsigned int button = 0;
    ButtonState state;

    FakeButtonReader_PressButton(button);
    ButtonHandler_Run();
    FakeButtonReader_ReleaseButton(button);
    ButtonHandler_Run();
    ButtonHandler_GetButtonState(button, &state);

    TEST_ASSERT(state == open);
}

TEST(ButtonHandler, ManyButtonsPressedChangesStatesToPressed)
{
    ButtonState state;

    for (int i = 0; i < (NUMBER_OF_BUTTONS - 3); i++)
    {
        FakeButtonReader_PressButton(i);
    }
    ButtonHandler_Run();

    for (int i = 0; i < (NUMBER_OF_BUTTONS - 3); i++)
    {
        ButtonHandler_GetButtonState(i, &state);
        TEST_ASSERT(state == pressed);
    }

    for (int i = NUMBER_OF_BUTTONS - 3; i < (NUMBER_OF_BUTTONS); i++)
    {
        ButtonHandler_GetButtonState(i, &state);
        TEST_ASSERT(state == open);
    }
}

TEST(ButtonHandler, ManyButtonsPressedThenReleasedChangesStatesToOpen)
{
    ButtonState state;

    for (int i = 0; i < (NUMBER_OF_BUTTONS - 3); i++)
    {
        FakeButtonReader_PressButton(i);
    }
    
    ButtonHandler_Run();

    for (int i = 0; i < (NUMBER_OF_BUTTONS - 3); i++)
    {
        FakeButtonReader_ReleaseButton(i);
    }

    ButtonHandler_Run();

    for (int i = 0; i < (NUMBER_OF_BUTTONS - 3); i++)
    {
        ButtonHandler_GetButtonState(i, &state);
        TEST_ASSERT(state == open);
    }
}

TEST(ButtonHandler, InitializedButtonIsNotChanged)
{
    ButtonState state;
    int change = 0;

    ButtonHandler_Run();
    state = ButtonHandler_GetButtonState(0, &state);

    TEST_ASSERT(!change);
}

TEST(ButtonHandler, DetectButtonChangeFromOpenToPressed)
{
    ButtonState state;
    int change = 0;

    FakeButtonReader_PressButton(0);
    ButtonHandler_Run();
    
    change = ButtonHandler_GetButtonState(0, &state);
    TEST_ASSERT(change);
}

TEST(ButtonHandler, ReturnNoChangeAfterSecondButtonReadCall)
{
    ButtonState state;
    int change = 0;

    FakeButtonReader_PressButton(0);
    ButtonHandler_Run();
    
    change = ButtonHandler_GetButtonState(0, &state);
    change = ButtonHandler_GetButtonState(0, &state);

    TEST_ASSERT(!change);
}

TEST(ButtonHandler, DetectButtonChangeFromPressedToOpen)
{
    ButtonState state;
    int change = 0;

    FakeButtonReader_PressButton(0);
    ButtonHandler_Run();
    change = ButtonHandler_GetButtonState(0, &state);
    
    FakeButtonReader_ReleaseButton(0);
    ButtonHandler_Run();
    change = ButtonHandler_GetButtonState(0, &state);

    TEST_ASSERT(state == open);
    TEST_ASSERT(change);
}


TEST(ButtonHandler, ReturnNoChangeAfterSecondButtonOpenRead)
{
    ButtonState state;
    int change = 0;

    FakeButtonReader_PressButton(0);
    ButtonHandler_Run();
    change = ButtonHandler_GetButtonState(0, &state);
    
    FakeButtonReader_ReleaseButton(0);
    ButtonHandler_Run();
    change = ButtonHandler_GetButtonState(0, &state);
    change = ButtonHandler_GetButtonState(0, &state);

    TEST_ASSERT(state == open);
    TEST_ASSERT(!change);
}

TEST(ButtonHandler, DetectChangeFromMultipleButtons)
{
    ButtonState state;
    int change = 0;

    for (int i = 0; i < NUMBER_OF_BUTTONS; i++)
    {
        FakeButtonReader_PressButton(i);
    }
    ButtonHandler_Run();
    for (int i = 0; i < NUMBER_OF_BUTTONS; i++)
    {
        change = ButtonHandler_GetButtonState(i, &state);
        TEST_ASSERT(change);
    }
}