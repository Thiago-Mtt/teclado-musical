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
 * Pressionar tecla gera aumento de 12 na escrita do DAC (140 no DAC)
 *      (127/8 < 12, excursão positiva/negativa suficiente para todos os botoes)                
 * Nova tecla só é processada após a interrupção do timer
 *      Processamento do próximo valor do DAC só é feito após interrupção
 *      Novo valor só aparece no DAC após uma outra interrupção
 * Após um numero de interrupções + execuções do modulo, tecla adiciona -12 a escrita do DAC
 *      Após 123 interrupções para a nota C4 (261Hz) considerando sampling de 32Khz
 * Soltar tecla desativa sua influencia no DAC
 * Pressionar tecla novamente reinicia progressao da tecla
 * Testar para as 8 teclas isoladas
 *      C4, D4, E4, F4, G4, A4, B4, C5
 * Duas ou mais teclas pressionadas somam as ondas no DAC
 * Soltar uma das teclas mantem a outra tecla no DAC
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
    TEST_ASSERT_EQUAL_UINT8(128, writeValue);
}

static void checkDACForNoNewWrite(void)
{
    bool newWrite = false;
    unsigned char writeValue;

    newWrite = FakeSynthDAC_GetLastWrite(&writeValue);

    TEST_ASSERT(!newWrite);
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

//TEST(Synth, PressC4IncrementsDACBy12)
//{
//    FakeSynthDAC_ClearLastWrite();
//
//    Synth_Press(C4);
//    checkDACForNoNewWrite();
//
//    FakeSynthTimer_Interrupt();
//    checkDACForNewWriteAndValue(128);
//
//    Synth_Run();
//    checkDACForNoNewWrite();
//
//    FakeSynthTimer_Interrupt();
//    checkDACForNewWriteAndValue(140);
//}