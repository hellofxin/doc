问题：在DMA的正常传输模式下，尝试进行多次传输，发现传输指挥进行一次
思路：传输完成后，DMAy_Channelx->CNDTRx的值变为0，需要重新赋值才会进行传输；
	 但ENABLE DMA时是不能改变DMA的配置的，所以先DISABLE DMA，对DMAy_Channelx->CNDTRx重新赋值。