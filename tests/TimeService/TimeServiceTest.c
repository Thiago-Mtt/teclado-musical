/*
 * Lista de Testes do TimeService
 * Objetivo: Manter o tempo de sistema e gerenciar serviço de alarmes/temporizadores
 * Tempo de sistema é zero ao criar o módulo    OK
 * Tempo do sistema aumenta quando função de callback é chamada por X microsegundos     OK
 * Leitura do tempo não é corrompida por interrupção (feito por pausa e continuação)    OK?
 * Tempo de sistema é zero após destruir e criar módulo novamente                       OK
 * Tempo de sistema não aumenta após destruir módulo?
 * Cria temporizador com tempo limite (período para retornar verdadeiro)
 *      
 *      Retorna false caso tempo limite não tenha sido atingido ainda                   OK
 *      Retorna true caso tempo limite tenha sido atingido e reinicia temporizador      OK
 *      Retorna false depois de retornar true e outro período ainda não passou          OK
 *      Retorna true multiplas vezes sempre após o período de tempo do temporizador     OK
 *      Retorna false próximo do overflow de tempo do sistema corretamente              OK
 *      Retorna true próximo do overflow de tempo do sistema corretamente               OK
 *      Retorna false corretamente mesmo após overflow de tempo                         OK
 *      Retorna true corretamente mesmo após overflow de tempo                          OK
 *      Função para reiniciar temporizador                                              OK
 *      Função para deletar temporizador                                                OK
 *      O que fazer se for passado tempo limite 0?
 *      
*/


#include "TimeService.h"
#include "FakeSystemTimer.h"
#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP(TimeService);

static timeMicroseconds time;

TEST_SETUP(TimeService)
{
  TimeService_Create();
  time = -1;
}

TEST_TEAR_DOWN(TimeService)
{
  TimeService_Destroy();
}

TEST(TimeService, TimeIsZeroAfterCreate)
{
  time = TimeService_Time();
  TEST_ASSERT(time == 0);
}

TEST(TimeService, TimeIncreasedAfterSystemCallback)
{
  FakeSystemTimer_AddTime(1000);
    
  time = TimeService_Time();

  TEST_ASSERT_EQUAL_INT(1000, time);
}

TEST(TimeService, TimeResetAfterDestroy)
{
  FakeSystemTimer_AddTime(1000);
  TimeService_Destroy();
  TimeService_Create();
  
  time = TimeService_Time();

  TEST_ASSERT_EQUAL_INT(0, time);
}

TEST(TimeService, TimeReadIsInterruptProof)
{
  /*  Este teste não garante que a leitura do tempo não será interrompida
      Só verifica se houve desabilitação e habilitação da interrupção em algum momento ao pedir uma leitura
  */
  time = TimeService_Time();
  time = TimeService_Time();
  time = TimeService_Time();

  int pauses = FakeSystemTimer_GetPauses();
  int continues = FakeSystemTimer_GetContinues();

  TEST_ASSERT_EQUAL_INT(3, pauses);
  TEST_ASSERT_EQUAL_INT(3, continues);
}

/*********************************************************************************/

TEST_GROUP(TimeService_SoftTimer);

static SoftTimer testTimer;

TEST_SETUP(TimeService_SoftTimer)
{
  TimeService_Create();
  time = -1;
  testTimer = TimeService_CreateTimer(10000);
}

TEST_TEAR_DOWN(TimeService_SoftTimer)
{
  TimeService_DestroyTimer(testTimer);
  TimeService_Destroy();
}

TEST(TimeService_SoftTimer, CreateAndDestroySoftwareTimer)
{
  // Para implementação que utilize alocação dinâmica de memória
  // Este teste depende da configuração do Unity de verificação de vazamento de memória estar ligada
  SoftTimer myTimer = TimeService_CreateTimer(10000);
  TimeService_DestroyTimer(myTimer);
}

TEST(TimeService_SoftTimer, CheckTimerReturnsFalseWhenItsNotTime)
{
  FakeSystemTimer_AddTime(5000);
  bool itsTime = TimeService_CheckTimer(testTimer);
  
  TEST_ASSERT(!itsTime);
}

TEST(TimeService_SoftTimer, CheckTimerReturnsTrueWhenItsTime)
{
  FakeSystemTimer_AddTime(15000);
  bool itsTime = TimeService_CheckTimer(testTimer);
  
  TEST_ASSERT(itsTime);
}

TEST(TimeService_SoftTimer, CheckTimerReturnsFalseAfterReturningTrueAndItsNotTimeYetAgain)
{
  FakeSystemTimer_AddTime(15000);
  bool itsTime1 = TimeService_CheckTimer(testTimer);
  FakeSystemTimer_AddTime(5000);
  bool itsTime2 = TimeService_CheckTimer(testTimer);
 
  TEST_ASSERT(itsTime1);
  TEST_ASSERT(!itsTime2);
}

TEST(TimeService_SoftTimer, CheckTimerReturnsTrueAfterAnotherPeriod)
{
  FakeSystemTimer_AddTime(15000);
  bool itsTime1 = TimeService_CheckTimer(testTimer);
  FakeSystemTimer_AddTime(5000);
  bool itsTime2 = TimeService_CheckTimer(testTimer);
  FakeSystemTimer_AddTime(10000);
  bool itsTime3 = TimeService_CheckTimer(testTimer);
  
  TEST_ASSERT(itsTime1);
  TEST_ASSERT(!itsTime2);
  TEST_ASSERT(itsTime3);
}

TEST(TimeService_SoftTimer, CheckTimerReturnsFalseBeforeTimeOverflows)
{
  // Este teste põe o tempo de sistema guardado por TimeService próximo do overflow,
  // com o próximo 'alarme' marcado para depois do overflow e verifica que o teste retona false
  // antes do overflow e antes deste período.
  // Este teste supõe que o tempo de sistema está guardada numa variável tipo unsigned long
  FakeSystemTimer_AddTime(ULONG_MAX-500);
  bool itsTime1 = TimeService_CheckTimer(testTimer);
  
  FakeSystemTimer_AddTime(10);
  bool itsTime2 = TimeService_CheckTimer(testTimer);

  TEST_ASSERT(itsTime1);
  TEST_ASSERT(!itsTime2);
}

TEST(TimeService_SoftTimer, CheckTimerReturnsTrueBeforeTimeOverflows)
{
  // Este teste põe o tempo de sistema guardado por TimeService próximo do overflow,
  // com o próximo 'alarme' marcado para antes do overflow e verifica que o teste retona true
  // antes do overflow e depois deste período.
  // Este teste supõe que o tempo de sistema está guardada numa variável tipo unsigned long
  FakeSystemTimer_AddTime(ULONG_MAX-20000);
  bool itsTime1 = TimeService_CheckTimer(testTimer);
  
  FakeSystemTimer_AddTime(15000);
  bool itsTime2 = TimeService_CheckTimer(testTimer);
  
  TEST_ASSERT(itsTime1);
  TEST_ASSERT(itsTime2);
}

TEST(TimeService_SoftTimer, CheckTimerReturnsFalseAfterTimeOverflows)
{
  // Este teste põe o tempo de sistema guardado por TimeService depois do overflow,
  // e verifica que a função retorna falso corretamente.
  // Este teste supõe que o tempo de sistema está guardada numa variável tipo unsigned long
  FakeSystemTimer_AddTime(ULONG_MAX);
  bool itsTime1 = TimeService_CheckTimer(testTimer);
  
  FakeSystemTimer_AddTime(10);
  bool itsTime2 = TimeService_CheckTimer(testTimer);
  
  TEST_ASSERT(itsTime1);
  TEST_ASSERT(!itsTime2);
}

TEST(TimeService_SoftTimer, CheckTimerReturnsTrueAfterTimeOverflows)
{
  // Este teste põe o tempo de sistema guardado por TimeService depois do overflow,
  // e verifica que a função retorna verdadeiro corretamente.
  // Este teste supõe que o tempo de sistema está guardada numa variável tipo unsigned long
  FakeSystemTimer_AddTime(ULONG_MAX);
  bool itsTime1 = TimeService_CheckTimer(testTimer);
  
  FakeSystemTimer_AddTime(20000);
  bool itsTime2 = TimeService_CheckTimer(testTimer);
  
  TEST_ASSERT(itsTime1);
  TEST_ASSERT(itsTime2);
}

TEST(TimeService_SoftTimer, CheckTimerReturnsFalseAfterResetingTimer)
{
  FakeSystemTimer_AddTime(9000);
  TimeService_ResetTimer(testTimer);
  FakeSystemTimer_AddTime(5000);
  
  bool itsTime = TimeService_CheckTimer(testTimer);
  
  TEST_ASSERT(!itsTime);
}