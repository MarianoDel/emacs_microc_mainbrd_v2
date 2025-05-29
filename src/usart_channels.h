//---------------------------------------------
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    STM32F103
// ##
// #### USART_CHANNELS.H ######################
//---------------------------------------------

#ifndef _USART_CHANNELS_H_
#define _USART_CHANNELS_H_


// Module Exported Macros ------------------------------------------------------
#define UsartChannel1Config()    Usart2Config()
#define UsartChannel1Send(X)    Usart2Send(X)
#define UsartChannel1ReadBuffer(X,Y)    Usart2ReadBuffer(X,Y)
#define UsartChannel1HaveData()    Usart2HaveData()
#define UsartChannel1HaveDataReset()    Usart2HaveDataReset()
#define UsartChannel1Shutdownn()    Usart2Shutdown()

#define UsartEncConfig()    Usart3Config()
#define UsartEncSend(X)    Usart3Send(X)
#define UsartEncReadBuffer(X,Y)    Usart3ReadBuffer(X,Y)
#define UsartEncHaveData()    Usart3HaveData()
#define UsartEncHaveDataReset()    Usart3HaveDataReset()
#define UsartEncShutdownn()    Usart3Shutdown()

#define UsartConnConfig()    Uart4Config()
#define UsartConnSend(X)    Uart4Send(X)
#define UsartConnReadBuffer(X,Y)    Uart4ReadBuffer(X,Y)
#define UsartConnHaveData()    Uart4HaveData()
#define UsartConnHaveDataReset()    Uart4HaveDataReset()
#define UsartConnShutdownn()    Uart4Shutdown()

#define UsartSupplyConfig()    Uart5Config()
#define UsartSupplySend(X)    Uart5Send(X)
#define UsartSupplyReadBuffer(X,Y)    Uart5ReadBuffer(X,Y)
#define UsartSupplyHaveData()    Uart5HaveData()
#define UsartSupplyHaveDataReset()    Uart5HaveDataReset()
#define UsartSupplyShutdownn()    Uart5Shutdown()

#define UsartRpiConfig()    Usart1Config()
#define UsartRpiSend(X)    Usart1Send(X)
#define UsartRpiReadBuffer(X,Y)    Usart1ReadBuffer(X,Y)
#define UsartRpiHaveData()    Usart1HaveData()
#define UsartRpiHaveDataReset()    Usart1HaveDataReset()
#define UsartRpiShutdown()    Usart1Shutdown()


#endif     /* _USART_CHANNELS_H_ */
