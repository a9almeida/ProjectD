//ProjectD: Aliya Goncalves Almeida & Maya Elysse

void main()
{

char dir[512];
char buffer[13312];
char line[80];
char fileName[6];
char command[4];
int i;
int sectorsread;
int size = dir[i+6];


	while(1){

		syscall(0, "SHELL>");
		syscall(1, line);

		if(line[0] == 't' && line[1] == 'y' && line[2] == 'p' && line[3] == 'e'){
			buffer[0] = 0;
			syscall(3,line+5,buffer);
			if(buffer[0] =='\0'){
				syscall(0, "File not found\r\n");

			}
			else{
				syscall(0,buffer);
				syscall(0, "\r\n");
			}

		}
		else if(line[0] == 'e' && line[1] == 'x' && line[2] == 'e' && line[3] == 'c'){

			for(i = 5; i < 11 && line[i] != ' ' && line[i] != '\0'; i++){
				fileName[i - 5] = line[i];
			}
			fileName[i - 5] = '\0';
			//syscall(0, fileName);
			syscall(4, fileName);
			syscall(0, "Error");
		}

		else if(line[0] == 'd' && line[1] == 'i' && line[2] == 'r'){
			syscall(2,dir,2);
			for(i = 0; i < 512; i+=32){
				fileName[0] = 0;
				if(dir[i] !='\0'){
					fileName[0] = dir[i];
					fileName[1] = dir[i+1];
					fileName[2] = dir[i+2];
					fileName[3] = dir[i+3];
					fileName[4] = dir[i+4];
					fileName[5] = dir[i+5];
					fileName[6] = '\0';

				}
				if(fileName[0] != '\0'){
					syscall(0, fileName);
					syscall(0,"\r\n");
				}
			}
		}

		else if(line[0] == 'd' && line[1] == 'e' && line[2] == 'l'){
			for(i = 4; i < 10 && line[i] != ' ' && line[i] != '\0'; i++){
				fileName[i - 4] = line[i];
			}
			fileName[i - 4] = '\0';
			syscall(7, fileName);
		}

		else if(line[0] == 'c' && line[1] == 'o' && line[2] == 'p' && line[3] == 'y'){
			char fileName2[6];
			int i;
			int j;

			for(i = 5; i < 11 && line[i] != ' ' && line[i] != '\0'; i++){
				fileName[i - 5] = line[i];
			}
			fileName[i - 5] = '\0';
			j = i+1;
			for(i = i + 1; i < i+7 && line[i] != ' ' && line[i] != '\0'; i++){
				fileName2[i - j] = line[i];
			}
			fileName2[i - j] = '\0';
			syscall(3, fileName, buffer);
			syscall(8, fileName2, buffer);
//			syscall(0, fileName);
//			syscall(0, fileName2);
		}

		else if(line[0] == 'c' && line[1] == 'r' && line[2] == 'e' && line[3] == 'a' && line[4] == 't' && line[5] == 'e'){
			int i;
			int j=0;
			char newfilename[6];

			for(i = 7; i < 13 && line[i] != ' ' && line[i] != '\0'; i++){
				newfilename[i - 7] = line[i];
			}
			newfilename[i - 7] = '\0';
			syscall(0, newfilename);

			while(1){
				syscall(0, "> ");
				syscall(1, line);

				if (line[0] == '\n' || line[0] == '\r'){
					break;
				}

				for(i = 0; i < 80 && line[i] != '\0'; i++){
					buffer[j++] = line[i];
				}

				buffer[j++] = '\n';

			}

			buffer[j] = '\0';

			syscall(8, newfilename, buffer,1);


		}

		else{
			syscall(4,"Bad command");
		}

	}

}
