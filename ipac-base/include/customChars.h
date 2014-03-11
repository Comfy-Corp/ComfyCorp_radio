//Animation functions
extern void CustomCharsAnimation();
extern void CustomCharsAlarmIcon(u_char);
extern void CustomCharsLoading(u_char, u_char);


u_char forwardSlash[] = {
	0x10,
	0x08,
	0x04,
	0x02,
	0x01,
	0x00,
	0x00,
	0x00
};

u_char alarmChar[] = { //Stolen from http://www.quinapalus.com/hd44780udg.html
	0x04,
	0x0e,
	0x0e,
	0x0e,
	0x1f,
	0x00,
	0x04,
	0x00
};

//Custom char definitions
u_char animationA[] = {
	0x00,  // 00000
	0x00,  // 00000
	0x00,  // 00000
	0x00,  // 00000
	0x00,  // 00000
	0x00,  // 00000
	0x10,  // 10000
	0x00   // 00000
};

u_char animationB[] = {
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x10,
	0x08,
	0x00
};
	
u_char animationC[] = {
	0x00,
	0x10,
	0x10,
	0x10,
	0x10,
	0x10,
	0x18,
	0x10
};
	

u_char animationD[] = {
	0x00,
	0x18,
	0x18,
	0x18,
	0x08,
	0x08,
	0x18,
	0x08
};

u_char animationE[] = {
	0x00,
	0x0C,
	0x0C,
	0x0C,
	0x04,
	0x0C,
	0x16,
	0x04
};
	

u_char animationF[] = {
	0x00,
	0x0C,
	0x0C,
	0x0C,
	0x04,
	0x04,
	0x0C,
	0x04
};
	
u_char animationG[] = {
	0x00,
	0x0C,
	0x0C,
	0x0C,
	0x04,
	0x04,
	0x04,
	0x04
};

u_char animationH[] = {
	0x00,
	0x0E,
	0x0C,
	0x0E,
	0x04,
	0x0E,
	0x04,
	0x04	
};
	

u_char animationI[] = { //Reusable stick figure frame, in case the animation looks as stupid as I think.
	0x00, // 00000
	0x0E, // 01110
	0x0A, // 01010
	0x0E, // 01110
	0x04, // 00100
	0x0E, // 01110
	0x15, // 10101
	0x04  // 00100
};

u_char animationJ[] = {
	0x00,
	0x0E,
	0x0A,
	0x0E,
	0x05,
	0x0E,
	0x14,
	0x04
};
	//Repeat I

u_char animationK[] = {
	0x00,
	0x0E,
	0x06,
	0x0E,
	0x04,
	0x0E,
	0x04,
	0x04
};
	

u_char animationL[] = {
	0x00,
	0x06,
	0x06,
	0x06,
	0x04,
	0x04,
	0x04,
	0x04
};
	

u_char animationM[] = {
	0x00,
	0x06,
	0x06,
	0x06,
	0x04,
	0x04,
	0x0C,
	0x04
};

u_char animationN[] = {
	0x00,
	0x0C,
	0x0C,
	0x0C,
	0x08,
	0x18,
	0x0C,
	0x08
}; 

u_char animationO[] = {
	0x00,
	0x18,
	0x18,
	0x18,
	0x10,
	0x18,
	0x14,
	0x10
};

u_char animationP[] = {
	0x00,
	0x10,
	0x10,
	0x10,
	0x00,
	0x00,
	0x10,
	0x00
};