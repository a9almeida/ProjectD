//ProjectD by Aliya Goncalves Almeida & Maya Elysse


void printString(char *);
void printChar(char);
void readString(char *);
void readSector(char *, int);
void handleInterrupt21(int, int, int, int);
void readFile(char *, char *, char *);
void executeProgram(char*);
void terminate();
void writeSector(char *, int);
void deleteFile(char *);
void writeFile(char *, char *, int);


void main()
{

/*	char line[80];
	makeInterrupt21();
	interrupt(0x21,1,line,0,0);
	interrupt(0x21,0,line,0,0);
*/

//	char buffer[13312]; //this is the maximum size of a file
//	int sectorsRead;
	makeInterrupt21();
/*	interrupt(0x21, 3, "messag", buffer, &sectorsRead); //read the file into buffer
	if(sectorsRead > 0)
		interrupt(0x21, 0, buffer, 0, 0); //print out the file
	else
		interrupt(0x21, 0, "messag not found\r\n", 0, 0); //no sectors read? then print an error

	interrupt(0x21,4,"tstpr1",0,0);
*/
//	interrupt(0x21,8,"this is a test message","testmg",3);
	interrupt(0x21,4,"shell",0,0);

	while(1);
}
void printChar(char chars)
{
		interrupt(0x10, 0xe*256+chars,0,0,0);
}


void printString(char* chars)
{
	char al = '\n';
	while(*chars != '\0'){

		al = *chars;
		interrupt(0x10, 0xe * 256 + al, 0, 0, 0);
		chars++;
	}
}

void readString(char* str)
{
	int i =0;
	int length = 80;
	int enterKey = 0xd;
	int lineFeed = 0xa;
	int endString = 0x0;
	int backSpace = 0x8;

	while(i != length)
	{
		str[i] = interrupt(0x16,0,0,0);
		printChar(str[i]);

		if(str[i] == enterKey){
			str[i+1] = lineFeed;
			printChar(lineFeed);
			str[i+2] = endString;
			break;
		}

		else if(str[i] == backSpace){
			if(i > 0){

				printChar(' ');
				printChar(backSpace);
				i--;
			}
		}

		else{
			i++;
		}
	}
}


void readSector(char *buffer, int sector)
{
	int ax = 2*256+1;
	int cx = 0*256+sector+1;
	int dx = 0*256+0x80;

	interrupt(0x13, ax, buffer, cx, dx);
}

void handleInterrupt21(int ax, int bx, int cx, int dx)
{
	if(ax == 0){
		printString(bx);
	}

	else if(ax == 1){
		readString(bx);
	}

	else if(ax == 2){
		readSector(bx, cx);
	}

	else if(ax == 3){
		readFile(bx, cx, dx);
	}

	else if(ax == 4){
		executeProgram(bx);
	}

	else if(ax == 5){
		terminate();
	}

	else if(ax == 6){
		writeSector(bx, cx);
	}

	else if(ax == 7){
		deleteFile(bx);
	}


	else if(ax == 8){
		writeFile(bx, cx, dx);
	}

	else{
		printString("An error occured when ax is greather than 8");
	}

}

void readFile(char *filename, char *buffer,int *sectorsread)
{

	char dir[512];
	int fileentry;
	int sectors;
	int foundfile = 0;
	int entry;
	int i = 0;
	*sectorsread = 0;


	readSector(dir, 2);

		for(entry=0; entry<512; entry+=32){
			foundfile=1;
			for(fileentry=0; fileentry<6; fileentry++){

			//	printChar(dir[fileentry+entry]);	//
			//	printChar(filename[fileentry]);		//

				if(filename[fileentry]!=dir[fileentry+entry]){
					foundfile=0;
					break;
				}
			}
			if(foundfile==1){
				break;
			}
		}
		if(foundfile==0){
			*sectorsread = 0;
			return;
		}

		for(i=6; i<26; i++){
			sectors = dir[entry+i];
			if(sectors==0)
			break;

			readSector(buffer, sectors);
			buffer += 512;
			*sectorsread += 1;
		}
}

void executeProgram(char* name)
{
	char buffer[13312];
	int i=0;
	int sectorsread;
	int segment = 0x2000;
	readFile(name, buffer, &sectorsread);
	if(sectorsread == 0){
		return;
	}

	while(i<13312){
		putInMemory(segment, i, buffer[i]);
		i++;
	}

	launchProgram(segment);

}

void terminate()
{
	char shellname[6];
	shellname[0] = 's';
	shellname[1] = 'h';
	shellname[2] = 'e';
	shellname[3] = 'l';
	shellname[4] = 'l';
	shellname[5] = '\0';

	executeProgram(shellname);
}


void writeSector(char *buffer, int sector)
{
        int ax =3*256+1;
        int cx = 0*256+sector+1;
        int dx = 0*256+0x80;

        interrupt(0x13, ax, buffer, cx, dx);
}

void deleteFile(char *filename)
{
	char dir[512];
	char map[512];
	int entry;
	int i;

	interrupt(0x21, 2, dir, 2, 0);
	interrupt(0x21, 2, map, 1, 0);

	for(entry = 0; entry < 512; entry += 32)
	{
		int foundFile = 1;
		for(i = 0; i < 6; i++)
		{
			if(filename[i] != dir[entry + i])
			{
				foundFile = 0;
				break;
			}
		}

		if(foundFile)
		{
			dir[entry] = '\0';
			for(i = 6; i < 32; i++)
			{
				int sector = dir[entry +i];
				if(sector == 0)
				{
					break;
				}
				map[sector] = 0;
			}
			interrupt(0x21, 6, dir, 2, 0);
			interrupt(0x21, 6, map, 1, 0);
			return;
		}
	}
	interrupt(0x21, 0, "File not found\r\n", 0,0);

}


void writeFile(char *filename,char *buffer, int numberOfSectors)
{
	char dir[512];
	char map[512];
	int entry;
	int i,j;

	interrupt(0x21, 2, map, 1, 0);
	interrupt(0x21, 2, dir, 2, 0);

	for(entry = 0; entry < 512; entry += 32)
	{
		if(dir[entry] == '\0')
		{
			for(i = 0; i < 6 && filename[i] != '\0'; i++)
			{
				dir[entry + i] = filename[i];
			}
			for(j = i; j < 6; j++)
			{
				dir[entry + j] = '\0';
			}
			for(i = 0; i < numberOfSectors; i++)
			{
				int freeSector = -1;
				for(j = 3; j < 512; j++)
				{
					if(map[j] == '\0')
					{
						freeSector = j;
						break;
					}
				}
				if(freeSector == -1)
				{
					return;
				}
				map[freeSector] = 0xFF;

				dir[entry + 6 + i] = (char)freeSector;

				writeSector(buffer+512*i, freeSector);
			}
			for(j = i; j < 26; j++)
			{
				dir[entry + 6 + j] = '\0';
			}

			interrupt(0x21, 6, map, 1, 0);
			interrupt(0x21, 6, dir, 2, 0);

			return;
		}
	}
	return;
}
