#include "delay.h"
#include "sys.h"
#include "lcd.h"
#include "touch.h"
#include "gui.h"
#include "test.h"

int main(void)
{
	SystemInit();
	delay_init(72);
	LCD_Init();	   

	while(1)
	{
		main_test(); 		
		Test_Color();  		
		Test_FillRec();		
		Test_Circle(); 		
		Test_Triangle();    
		English_Font_test();
		Chinese_Font_test();
		Pic_test();			
		Rotate_Test();  
		// comment next if not necessary		
		Touch_Test();		
	}
}
