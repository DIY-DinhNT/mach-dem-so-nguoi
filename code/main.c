	#include <at89x52.h>

	unsigned char code number[10]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};		// Mang luu ma so tu 0 den 9	

	//******************** Dinh nghia chan *****************//
	sbit chuc = P0^7;
	sbit dvi  = P0^6;
	sbit ctr  = P0^4;
	sbit in1  = P3^1;
	sbit in2  = P3^2;
	////////////////////////////////////////////////////////////

	char ram = 0, value = 0;				// Bien toan cuc luu so 

	void delay_ms(int t){					// Ham Delay tre t mS khong tra ve du lieu
		int y,x;{
			for(x=1;x<t;++x)
				for(y=1;y<123;++y);
		}
	}
	

	void TimerInit(){			   // Ham khoi tao Timer0 khong tra ve du lieu

		TMOD &= 0xF0;		//		Xoa di cac bit chon MODE cua Timer0
		TMOD |= 0x01;		//		Timer0 ho9at dong o che do MODE 1

		ET0 = 1;	//		Cho phep ngat Timer0	
		EA = 1; 	//		Cho phep ngat toan cuc

		TH0 = (65536-1000)>>8;	   // 		Nap  8 bit cao cho thanh ghi TH0	 // TImer0 cu moi 10mS ngat xay ra 1 lan de quet hien thi LED 7 thanh
		TL0 = (65536-1000)&0x00FF; //		Nap  8 bit thap cho thanh ghi TL0

		TR0 = 1;	//		Cho phep Timer0 bat dau dem
	
	}

	void display() interrupt 1 	   // ham ngat Timer0 hien thi so tren LED 7 doan 
	{
	   unsigned char j;			  // Khai bao bien cuc bo su dung trong ham
//	   chuc = dvi = 1;			  // Keo 2 chan dieu khien chan Anot LED len cao qua A1015 thanh xuong thap

	   for (j = 0; j < 5; j++){  // Lap lai 25 lan moi lan ngat de tao thoi gian mat du nhin duoc so ( 25 x 2mS = 50mS )
	      P2 = number[value%10];  // Gan gia tri Port xuat du lieu ra so hang don vi
	      dvi = 0;				  // Cho phep chan Anot Led hang don vi sang
	      delay_ms(5);			  // Tre 1 mS
	      dvi = 1;				  // Tat led hang don vi di 
	      
	      P2 = number[value/10];  // Gan gia tri Port xuat du lieu ra so hang chuc
	      chuc = 0;				  // Cho phep chan Anot Led hang chuc sang
	      delay_ms(5);			  // Tre 1mS
	      chuc = 1;				  // Tat led hang chuc di
	   }

		TH0 = (65536-1000)>>8;	   // 		Nap  8 bit cao cho thanh ghi TH0 de tiep tuc dem va ngat o cac lan tiep theo
		TL0 = (65536-1000)&0x00FF; //		Nap  8 bit thap cho thanh ghi TL0 de tiep tuc dem va ngat o cac lan tiep theo
	}	

	int count(){					  // ham quet 2 trang thai chan de phat hien nguoi di ra hay di vao
		
		if(in2==1){
			delay_ms(1);
			while(in2==1);
			if(in1==1){
				delay_ms(1);
				while(in1==1);
				ram+=1;
				if(ram<0){		  
					ram=0;
				}
				if (ram==100){	   // So nguuoi vuot qua 99 nguoi thi khong tang len nua
					ram=99;
				}
			}
		}
	
		if(in1==1){
			delay_ms(1);
			while(in1==1);
			if(in2==1){
				delay_ms(1);
				while(in2==1);
				ram-=1;
				if(ram<0)	
					ram=0;
				if (ram==100)
					ram=99;
			}																			   
		}
		return ram;
	}

	void main(){

	 	/////////////////////////////   Khoi tao trang thai ban dau cac chan can thiet
		P0 = 0x00;
		P2 = 0xff;
		ctr = 0;
		in2=0;
		in1=0;

		////////////////////////////	Goi ham cau hinh Timer0
		TimerInit();

		while(1){
		////////////////// Vong lap vo tan kiem tra trang thai cam bien de lay du lieu so nguoi trong phong hien tai
		value = count();	  /// Bien nhan so nguoi trong phong hien tau
			if (value==0) 	 // cau lenh IF ELSE kiem tra khong co nguoi thi tat den
				ctr = 0;
			else ctr = 1;
		}	
	 }
