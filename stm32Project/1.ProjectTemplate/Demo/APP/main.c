#include "public.h"

void delay(unsigned int milliseconds) {
    // 假设每个循环迭代耗时约1毫秒
    unsigned int i,j;
    for (i = 0; i < milliseconds; ++i) {
        // 在这里可以添加一些延时无关的处理代码，以防止编译器优化空循环
        // 例如，可以添加一些计算操作，以消耗一定的CPU时间
        volatile int dummy = 0;
        for (j = 0; j < 1000; ++j) {
            dummy++;
        }
    }
}

/**
初始化小灯的引脚
**/
void ControlLightSwitch(){
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);
    
    GPIO_InitTypeDef GPIO_InitTypeDefStruct;
    GPIO_InitTypeDefStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitTypeDefStruct.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitTypeDefStruct.GPIO_Speed =GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitTypeDefStruct);
    
    GPIO_InitTypeDefStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitTypeDefStruct.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitTypeDefStruct.GPIO_Speed =GPIO_Speed_50MHz;
    GPIO_Init(GPIOE, &GPIO_InitTypeDefStruct);
    
}

void USARTInit(){
    
}
int main(){
    ControlLightSwitch();
   while(1){
       GPIO_WriteBit(GPIOB,GPIO_Pin_5,Bit_SET);
       GPIO_WriteBit(GPIOE,GPIO_Pin_5,Bit_RESET);
       delay(10000);
       GPIO_WriteBit(GPIOB,GPIO_Pin_5,Bit_RESET);
       GPIO_WriteBit(GPIOE,GPIO_Pin_5,Bit_SET);
       delay(10000);
   } 
   return 0; 
}

 