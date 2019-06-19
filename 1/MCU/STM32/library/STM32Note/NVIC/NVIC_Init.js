/* 通过preemption、sub priority得到正确的priority，
   通过NVIC_IRQChannel将priority配置到对应的NVIC->IP，
   通过NVIC_IRQChannel使能通道
*/
void NVIC_Init(NVIC_InitTypeDef* NVIC_InitStruct){
  uint32_t tmppriority = 0x00, tmppre = 0x00, tmpsub = 0x0F;
  
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NVIC_InitStruct->NVIC_IRQChannelCmd));
  assert_param(IS_NVIC_PREEMPTION_PRIORITY(NVIC_InitStruct->NVIC_IRQChannelPreemptionPriority));  
  assert_param(IS_NVIC_SUB_PRIORITY(NVIC_InitStruct->NVIC_IRQChannelSubPriority));
    
  if (NVIC_InitStruct->NVIC_IRQChannelCmd != DISABLE){
    /* Compute the Corresponding IRQ Priority --------------------------------*/    
    /* 通过AIRCR得到tmppriority（priorityGroupx） */
    tmppriority = (0x700 - ((SCB->AIRCR) & (uint32_t)0x700))>> 0x08;
    /* 在四个配置位中，preemption配置位左移的位数 */
    tmppre = (0x4 - tmppriority);
    /* 在四个配置位中，sub配置位全部set */
    tmpsub = tmpsub >> tmppriority;
    /* 通过temppre将NVIC_IRQChannelPreemptionPriority移动到四个priority配置位中的对应位 */
    tmppriority = (uint32_t)NVIC_InitStruct->NVIC_IRQChannelPreemptionPriority << tmppre;
    /* 通过tmpsub将NVIC_IRQChannelSubPriority配置到四个priority配置位中的对应位 */
    tmppriority |=  NVIC_InitStruct->NVIC_IRQChannelSubPriority & tmpsub;
    /* 将priority移动到高四位，得到正确的priority配置 */
    tmppriority = tmppriority << 0x04;
    /* 通过NVIC_IRQChannel将priority配置到对应位置 */
    NVIC->IP[NVIC_InitStruct->NVIC_IRQChannel] = tmppriority;
    
    /* Enable the Selected IRQ Channels --------------------------------------*/
    NVIC->ISER[NVIC_InitStruct->NVIC_IRQChannel >> 0x05] =
      (uint32_t)0x01 << (NVIC_InitStruct->NVIC_IRQChannel & (uint8_t)0x1F);
  }
  else
  {
    /* Disable the Selected IRQ Channels -------------------------------------*/
    NVIC->ICER[NVIC_InitStruct->NVIC_IRQChannel >> 0x05] =
      (uint32_t)0x01 << (NVIC_InitStruct->NVIC_IRQChannel & (uint8_t)0x1F);
  }
}
