/*
 * Lista de testes para o modulo Synth
 * Objetivo: 
 * Modulo recebe as teclas que foram pressionadas e soltas.
 * A cada intervalo de tempo, uma interrupcao gerada pelo timer ativa a escrita para o DAC
 * da amplitude instantanea da forma de onda sendo gerada no momento.
 * Apos a interrupcao, o modulo calcula o valor da proxima amostra com base nas teclas pressionadas
 * Cada tecla tem uma forma de onda que progride no tempo ate decair completamente
 * O pressionamento de uma tecla reinicia a progressao do comeco
 * Caso a tecla seja solta antes do fim, o decaimento e acelerado.
 * O valor gerado pela forma de onda de cada tecla e somado e o resultado e guardado
 * para a proxima escrita do DAC
 * 
 * Primeira versao do modulo usa ondas quadradas sem ADSR e frequencia de amostragem de 32KHz
 * 
 * Synth é iniciado com todas as teclas nao pressionadas/ativas
 * Timer e DAC são inicializados durante inicialização do modulo                                OK
 * DAC é escrito em 128 após inicialização do modulo Synth                                      OK
 * DAC de teste decta se houve nova escrita                                                     OK
 * DAC de teste, limpar ultima escrita                                                          OK
 * Novas escritas no DAC por interrupção são 128 enquanto não há teclas pressionadas            OK
 * Pressionar tecla gera aumento de 12 na escrita do DAC (140 no DAC)                           OK
 *      (127/8 < 12, excursão positiva/negativa suficiente para todos os botoes)                
 * Nova tecla só é processada após a interrupção do timer                                       OK
 *      Processamento do próximo valor do DAC só é feito após interrupção
 *      Novo valor só aparece no DAC após uma outra interrupção
 * Interrupção do Timer é desabilitada ao verificar se é tempo de processar novo valor DAC      OK
 * Interrupção do Timer é desabilitada enquanto se escreve em memoria compartilhada             OK
 * Pressionar tecla mantem escrita em 140 por meio periodo da tecla                             OK?
 * Após um numero de interrupções + execuções do modulo, tecla adiciona -12 a escrita do DAC    OK?
 *      Após 123 interrupções para a nota C4 (261Hz) considerando sampling de 32Khz
 * Soltar tecla desativa sua influencia no DAC                                                  OK
 * Pressionar tecla novamente reinicia progressao da tecla                                      OK
 * Manter tecla pressionada repete periodicamente seu sinal na saida                            OK
 * Testar para as 8 teclas isoladas                                                             OK
 *      C4, D4, E4, F4, G4, A4, B4, C5
 * Duas ou mais teclas pressionadas somam as ondas no DAC                                       OK
 * Soltar uma das teclas mantem a outra tecla no DAC                                            OK
 * 
 * 
*/

#include "unity.h"
#include "unity_fixture.h"
#include <stdbool.h>

#include "Synth.h"
#include "FakeSynthDAC.h"
#include "FakeSynthTimer.h"

TEST_GROUP(Synth);

TEST_SETUP(Synth)
{
    Synth_Open();
}

TEST_TEAR_DOWN(Synth)
{
    Synth_Close();
}

static void checkDACForNewWriteAndValue(unsigned char value)
{
    bool newWrite = false;
    unsigned char writeValue;

    newWrite = FakeSynthDAC_GetLastWrite(&writeValue);

    TEST_ASSERT(newWrite);
    TEST_ASSERT_EQUAL_UINT8(value, writeValue);
}

static void checkDACForNoNewWrite(void)
{
    bool newWrite = false;
    unsigned char writeValue;

    newWrite = FakeSynthDAC_GetLastWrite(&writeValue);

    TEST_ASSERT(!newWrite);
}

static void runSynthCycles(unsigned int cycles)
{
    for (int i = 0; i < cycles; i++)
    {
        Synth_Run();
        FakeSynthTimer_Interrupt(); 
    }
}

TEST(Synth, OpenAndClose)
{
    Synth_Open();
    Synth_Close();
}

TEST(Synth, DACInitialization)
{
    bool initialized = false;
    initialized = FakeSynthDAC_Initialized();

    TEST_ASSERT(initialized);
}

TEST(Synth, TimerInitialization)
{
    bool initialized = false;
    initialized = FakeSynthTimer_Initialized();

    TEST_ASSERT(initialized); 
}

TEST(Synth, DACWriteAfterInitialization)
{
    bool newWrite = false;
    unsigned char writeValue;
    
    newWrite = FakeSynthDAC_GetLastWrite(&writeValue);

    TEST_ASSERT(newWrite);
    TEST_ASSERT_EQUAL_UINT8(128, writeValue);
}

TEST(Synth, DACDetectNewWrite)
{
    bool newWrite = false;
    unsigned char writeValue;
    
    newWrite = FakeSynthDAC_GetLastWrite(&writeValue);
    TEST_ASSERT(newWrite);
    TEST_ASSERT_EQUAL_UINT8(128, writeValue);

    newWrite = FakeSynthDAC_GetLastWrite(&writeValue);
    TEST_ASSERT(!newWrite);
}

TEST(Synth, DACClearLastWrite)
{
    bool newWrite = false;
    unsigned char writeValue;

    FakeSynthDAC_ClearLastWrite();
    newWrite = FakeSynthDAC_GetLastWrite(&writeValue);

    TEST_ASSERT(!newWrite);
    TEST_ASSERT_EQUAL(0, newWrite);
}

TEST(Synth, DACWriteMidLevelWhileNoPressedKeys)
{
    bool newWrite = false;
    unsigned char writeValue;

    FakeSynthDAC_ClearLastWrite();
    FakeSynthTimer_Interrupt();

    newWrite = FakeSynthDAC_GetLastWrite(&writeValue);
    TEST_ASSERT(newWrite);
    TEST_ASSERT_EQUAL(128, writeValue);
}

TEST(Synth, PressC4IncrementsDACBy12)
{
    FakeSynthDAC_ClearLastWrite();

    Synth_Press(Note_C4);
    checkDACForNoNewWrite();

    FakeSynthTimer_Interrupt();
    checkDACForNewWriteAndValue(128);

    Synth_Run();
    checkDACForNoNewWrite();

    FakeSynthTimer_Interrupt();
    checkDACForNewWriteAndValue(140);
}

TEST(Synth, PauseTimerInterruptOnCheckingIfIsTimeToProcessNextDAC)
{
    unsigned int enables, disables;
    Synth_Run();

    enables = FakeSynthTimer_GetEnables();
    disables = FakeSynthTimer_GetDisables();

    TEST_ASSERT_EQUAL(1, enables);
    TEST_ASSERT_EQUAL(1, disables);
}

TEST(Synth, PauseTimerInterruptOnNextDACValueUpdate)
{
    unsigned int enables, disables;
    FakeSynthTimer_Interrupt();
    Synth_Run();

    enables = FakeSynthTimer_GetEnables();
    disables = FakeSynthTimer_GetDisables();

    TEST_ASSERT_EQUAL(2, enables);
    TEST_ASSERT_EQUAL(2, disables);
}

TEST(Synth, ProcessNextDACValueOnlyAfterDACWrite)
{
    FakeSynthDAC_ClearLastWrite();
    Synth_Press(Note_C4);
    Synth_Run();
    FakeSynthTimer_Interrupt();

    checkDACForNewWriteAndValue(128); // C4 não contabilizado

    Synth_Run();
    Synth_Press(Note_D4);
    Synth_Run();
    FakeSynthTimer_Interrupt();
    checkDACForNewWriteAndValue(140); // C4 contabilizado, mas não D4
}

TEST(Synth, PressC4IncrementsDACBy12ForHalfPeriod)
{
    Synth_Press(Note_C4);
    Synth_Run();
    FakeSynthTimer_Interrupt();

    for (int i = 0; i < 61; i++)
    {
        Synth_Run();
        FakeSynthTimer_Interrupt();
        checkDACForNewWriteAndValue(140);
    }
}

TEST(Synth, PressC4DecrementDACBy12AfterHalfPeriod)
{
    Synth_Press(Note_C4);
    runSynthCycles(63);

    checkDACForNewWriteAndValue(128-12);

}

TEST(Synth, ReleaseC4KeyReturnsDACtoMidLevel)
{
    Synth_Press(Note_C4);
    runSynthCycles(2);

    checkDACForNewWriteAndValue(140);

    Synth_Release(Note_C4);
    runSynthCycles(2);

    checkDACForNewWriteAndValue(128);
}

TEST(Synth, ReleaseAndPressKeyResetsSignalProgression)
{
    Synth_Press(Note_C4);
    runSynthCycles(65);

    Synth_Release(Note_C4);
    runSynthCycles(2);
    Synth_Press(Note_C4);
    runSynthCycles(2);

    checkDACForNewWriteAndValue(140);
}

TEST(Synth, KeepKeyPressedRepeatsSignalOnDAC)
{
    Synth_Press(Note_C4);
    runSynthCycles(2);
    checkDACForNewWriteAndValue(128+12);

    runSynthCycles(63);
    checkDACForNewWriteAndValue(128-12);

    runSynthCycles(63);
    checkDACForNewWriteAndValue(128+12);

    runSynthCycles(63);
    checkDACForNewWriteAndValue(128-12);
}

TEST(Synth, PressD4)
{
    Synth_Press(Note_D4);
    runSynthCycles(55);

    checkDACForNewWriteAndValue(128 + 12);

    runSynthCycles(1);
    checkDACForNewWriteAndValue(128 - 12);
}

TEST(Synth, PressE4)
{
    Synth_Press(Note_E4);
    runSynthCycles(49);

    checkDACForNewWriteAndValue(128 + 12);

    runSynthCycles(1);
    checkDACForNewWriteAndValue(128 - 12);
}


TEST(Synth, PressF4)
{
    Synth_Press(Note_F4);
    runSynthCycles(46);

    checkDACForNewWriteAndValue(128 + 12);

    runSynthCycles(1);
    checkDACForNewWriteAndValue(128 - 12);
}


TEST(Synth, PressG4)
{
    Synth_Press(Note_G4);
    runSynthCycles(41);

    checkDACForNewWriteAndValue(128 + 12);

    runSynthCycles(1);
    checkDACForNewWriteAndValue(128 - 12);
}


TEST(Synth, PressA4)
{
    Synth_Press(Note_A4);
    runSynthCycles(37);

    checkDACForNewWriteAndValue(128 + 12);

    runSynthCycles(1);
    checkDACForNewWriteAndValue(128 - 12);
}

TEST(Synth, PressB4)
{
    Synth_Press(Note_B4);
    runSynthCycles(33);

    checkDACForNewWriteAndValue(128 + 12);

    runSynthCycles(1);
    checkDACForNewWriteAndValue(128 - 12);
}

TEST(Synth, PressC5)
{
    Synth_Press(Note_C5);
    runSynthCycles(31);

    checkDACForNewWriteAndValue(128 + 12);

    runSynthCycles(1);
    checkDACForNewWriteAndValue(128 - 12);
}

TEST(Synth, PressTwoKeysSumsTheirSignals)
{
    Synth_Press(Note_C4);
    Synth_Press(Note_D4);
    runSynthCycles(2);
    
    checkDACForNewWriteAndValue(128+12+12);

    runSynthCycles(54);
    checkDACForNewWriteAndValue(128+12-12);

    runSynthCycles(7);
    checkDACForNewWriteAndValue(128-12-12);
}

TEST(Synth, PressTwoKeysAndReleasingOneRemovesOnlyOneKey)
{
    Synth_Press(Note_C4);
    Synth_Press(Note_D4);
    runSynthCycles(4);
    
    checkDACForNewWriteAndValue(128+12+12);

    Synth_Release(Note_C4);
    runSynthCycles(2);
    checkDACForNewWriteAndValue(128+12);
}