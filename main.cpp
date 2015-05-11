#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
bool isAdmin = false;
char str[5000] = " ";
void makeCleanStr(){
	for(int i=0;i<5000;i++){
		str[i] = ' ';
	}
	str[5000] = '\0';
}
void cleanScreen(){
	if (strcmp(str," ")==0){
		makeCleanStr();
	}
	clrscr();
	printf("%s",str);
	clrscr();
	return;
}
void checkConf(){
	if (access("foods.dat",2) != 0){
		FILE* fp = fopen("foods.dat","w");
		fclose(fp);
	}
	if (access("orders.dat",2) != 0){
		FILE* fp = fopen("orders.dat","w");
		fclose(fp);
	}
}
void Login(){
	char userName[10];
	char passWord[10] = "";
	char welcomeString[] = "Welcome to EM Order System!\nPlease input your Password: ";
	char invalidWaring[] = "Invalid User Name or Password!\nSystem will exit.";
	cleanScreen();
	printf("%s",welcomeString);
	scanf("%s",&userName);
	cleanScreen();
	printf("%s",welcomeString);
	int starNumber = 0;
	do{
		char word;
		word = getch();
		if (word == '\r'){
			break;
		}else if(word == '\b'){
			clrscr();
			printf("%s",welcomeString);
			int tmpNumber = starNumber;
			while(tmpNumber>0){
				printf(" ");
				tmpNumber--;
			}
			clrscr();
			printf("%s",welcomeString);
			tmpNumber = starNumber - 1;
			starNumber--;
			while(tmpNumber>0){
				printf("*");
				tmpNumber--;
			}
			passWord[strlen(passWord)-1] = '\0';
		}else{
			sprintf(passWord, strcat(passWord,"%c"),word);
			starNumber++;
			printf("*");
		}
	}while(1);
	if (strcmp(userName,"admin")==0){
		if (strcmp(passWord,"admin")!=0){
			cleanScreen();
			printf("%s",invalidWaring);
			exit(-1);
		}
		isAdmin = true;
	}else if(strcmp(userName,"staff")==0){
		if (strcmp(passWord,"staff")!=0){
			cleanScreen();
			printf("%s",invalidWaring);
			exit(-1);
		}
	}else{
		cleanScreen();
		printf("%s",invalidWaring);
		exit(-1);
	}
	return;
}
void functionSelect();
int getFoodNum(){
	int orderNum = 0;
	{
		char tmpStr[20];
		FILE* fp = fopen("foods.dat","r");
		while(!feof(fp)){
			fgets(tmpStr,20,fp);
			if (tmpStr[0] == '['){
				orderNum++;
			}
		}
		fclose(fp);
	}
	return orderNum;
}
int getOrderNum(){
	int orderNum = 0;
	{
		char tmpStr[50];
		FILE* fp = fopen("orders.dat","r");
		while(!feof(fp)){
			fgets(tmpStr,50,fp);
			if (tmpStr[0] == '['){
				orderNum++;
			}
		}
		fclose(fp);
	}
	return orderNum;
}
void editFoodList();
void addFood(){
	cleanScreen();
	printf("Add a food to the food list!\n\nFood name: ");
	char foodName[20];
	scanf("%s",&foodName);
	cleanScreen();
	printf("Add a food to the food list!\n\nFood price: ");
	int foodPrice;
	scanf("%d",&foodPrice);
	int orderNum = getFoodNum();
	int foodNumber;
	{
		char tmpStr[20];
		FILE* fp = fopen("foods.dat","r");
		for (int i=0;i<orderNum;i++){
			fgets(tmpStr,20,fp);
			if (tmpStr[0] == '['){
				i++;
				int u=1;
				while(tmpStr[u]!=']'){
					tmpStr[u-1] = tmpStr[u];
					u++;
				}
				tmpStr[u-1] = '\0';
				foodNumber = (i == orderNum) ? atoi(tmpStr)+1 : 0;
			}
			i--;
		}
		fclose(fp);
	}
	{
		FILE* fp = fopen("foods.dat","at");
		fprintf(fp, "\n\n[%04d]\n%s\n%d",foodNumber,foodName,foodPrice);
		fclose(fp);
	}
	editFoodList();
}
void delFood(int chosedFoodNum){
	cleanScreen();
	printf("Are you sure you want to delete this food?It can't be undo!\nPress y to delete or Press n to cancel.");
	do{
		int a = getch();
		if (a == 121){
			break;
		}else if (a == 110){
			editFoodList();
		}
	}while(1);
	{
		char tmplist[10240] = "";
		int i = 0;
		char tmpStr[20];
		FILE* fp = fopen("foods.dat","r");
		while(!feof(fp)){
			fgets(tmpStr,20,fp);
			if (tmpStr[0] == '['){
				i++;
				if (i != chosedFoodNum){
					char tmpName[20];
					fgets(tmpName,20,fp);
					char tmpPrice[20];
					fgets(tmpPrice,20,fp);
					strcat(tmplist,"\n\n");
					strcat(tmplist,tmpStr);
					strcat(tmplist,tmpName);
					strcat(tmplist,tmpPrice);
				}
			}
		}
		fclose(fp);
		fp = fopen("foods.dat","w");
		fprintf(fp,"%s",tmplist);
		fclose(fp);
	}
	editFoodList();
}
void editFood(int chosedFoodNum){
	char foodName[20];
	char foodPrice[20];
	{
		int i = 0;
		char tmpStr[20];
		FILE* fp = fopen("foods.dat","r");
		while(!feof(fp)){
			fgets(tmpStr,20,fp);
			if (tmpStr[0] == '['){
				i++;
				if (i == chosedFoodNum){
					fgets(foodName,20,fp);
					foodName[strlen(foodName)-1] = (foodName[strlen(foodName)-1] == '\n') ? '\0' : foodName[strlen(foodName)-1] ;
					fgets(foodPrice,20,fp);
					foodPrice[strlen(foodPrice)-1] = (foodPrice[strlen(foodPrice)-1] == '\n') ? '\0' : foodPrice[strlen(foodPrice)-1] ;
				}
			}
		}
		fclose(fp);
	}
	cleanScreen();
	printf("You are editing %s ,the price is %sS$.\n",foodName,foodPrice);
	printf("Please input the new name: ");
	scanf("%s",&foodName);
	cleanScreen();
	printf("You are editing %s ,the price is %sS$.\n",foodName,foodPrice);
	printf("Please input the new price: ");
	int newPrice;
	scanf("%d",&newPrice);
	{
		char tmplist[10240] = "";
		int i = 0;
		char tmpStr[20];
		FILE* fp = fopen("foods.dat","r");
		while(!feof(fp)){
			fgets(tmpStr,20,fp);
			if (tmpStr[0] == '['){
				i++;
				if (i == chosedFoodNum){
					strcat(tmplist,"\n\n");
					strcat(tmplist,tmpStr);
					strcat(tmplist,foodName);
					strcat(tmplist,"\n");
					char strPrice[10];
					sprintf(strPrice,"%d",newPrice);
					strcat(tmplist,strPrice);
					fgets(tmpStr,20,fp);
					fgets(tmpStr,20,fp);
				}else{
					char tmpName[20];
					fgets(tmpName,20,fp);
					char tmpPrice[20];
					fgets(tmpPrice,20,fp);
					strcat(tmplist,"\n\n");
					strcat(tmplist,tmpStr);
					strcat(tmplist,tmpName);
					strcat(tmplist,tmpPrice);
				}
			}
		}
		fclose(fp);
		fp = fopen("foods.dat","w");
		fprintf(fp,"%s",tmplist);
		fclose(fp);
	}
	editFoodList();
}
void editFoodList(){
	int orderNum = getFoodNum();
	char* selectStr;
	if (orderNum == 0){
		selectStr = "There is no food in list!Please press a to add food first.";
	}else{
		selectStr = "Please select a food to edit:";
	}
	int chosedFoodNum = 1;
	do{
		cleanScreen();
		printf("%s\n\n",selectStr);
		char chosedSign;
		int list = 3;
		{
			char tmpStr[20];
			FILE* fp = fopen("foods.dat","r");
			for (int i=0;i<orderNum;i++){
				fgets(tmpStr,20,fp);
				if (tmpStr[0] == '['){
					i++;
					fgets(tmpStr,20,fp);
					tmpStr[strlen(tmpStr)-1] = (tmpStr[strlen(tmpStr)-1] == '\n') ? '\0' : tmpStr[strlen(tmpStr)-1] ;
					chosedSign = (i == chosedFoodNum) ? '>' : ' ' ;
					printf("%c%d.%s",chosedSign,i,tmpStr);
					fgets(tmpStr,20,fp);
					tmpStr[strlen(tmpStr)-1] = (tmpStr[strlen(tmpStr)-1] == '\n') ? '\0' : tmpStr[strlen(tmpStr)-1] ;
					printf("--%sS$",tmpStr);
					if (i%list == 0 || i == orderNum){
						printf("\n\n");
					}else{
						printf("   ");
					}
				}
				i--;
			}
			printf("Press a to add a new food or press d to delete the food is chose.");
			fclose(fp);
		}
		int code1,code2;
		code1 = getch();
		if (code1 == 224){
			code2 = getch();
			switch(code2){
				case 72:
					chosedFoodNum -= (chosedFoodNum>list) ? list : 0 ;
					break;
				case 80:
					chosedFoodNum += (chosedFoodNum+list<=orderNum) ? list : 0 ;
					break;
				case 75:
					chosedFoodNum -= (chosedFoodNum%list!=1) ? 1 : 0 ;
					break;
				case 77:
					chosedFoodNum += (chosedFoodNum%list!=0 && chosedFoodNum!=orderNum) ? 1 : 0 ;
					break;
			}
		}else if (code1 == 13){
			editFood(chosedFoodNum);
			break;
		}else if (code1 == 97){
			addFood();
			break;
		}else if (code1 == 100){
			delFood(chosedFoodNum);
			break;
		}else if (code1 == 27){
			functionSelect();
			break;
		}
	}while(1);
}
void editOrderList();
void addOrder(){
	cleanScreen();
	printf("Add a order to the order list!\n\nCustomer name: ");
	char orderName[50];
	scanf("%s",&orderName);
	cleanScreen();
	printf("Add a order to the order list!\n\nCustomer phone number: ");
	char phoneNumber[50];
	scanf("%s",&phoneNumber);
	cleanScreen();
	printf("Add a order to the order list!\n\nCustomer address: ");
	char address[50];
	scanf("%s",&address);
	char foodList[50] = "";
	do{
		int total = 0;
		cleanScreen();
		printf("Add a order to the order list!\n\n");
		{
			int orderNum = getFoodNum();
			char tmpStr[50];
			FILE* fp = fopen("foods.dat","r");
			for (int i=0;i<orderNum;i++){
				int list = 3;
				fgets(tmpStr,50,fp);
				if (tmpStr[0] == '['){
					i++;
					int u=1;
					while(tmpStr[u]!=']'){
						tmpStr[u-1] = tmpStr[u];
						u++;
					}
					tmpStr[u-1] = '\0';
					char tmpName[50];
					fgets(tmpName,50,fp);
					tmpName[strlen(tmpName)-1] = (tmpName[strlen(tmpName)-1] == '\n') ? '\0' : tmpName[strlen(tmpName)-1] ;
					char tmpPrice[50];
					fgets(tmpPrice,50,fp);
					tmpPrice[strlen(tmpPrice)-1] = (tmpPrice[strlen(tmpPrice)-1] == '\n') ? '\0' : tmpPrice[strlen(tmpPrice)-1] ;
					printf("%s.%s--%sS$",tmpStr,tmpName,tmpPrice);
					if (i%list == 0 || i == orderNum){
						printf("\n");
					}else{
						printf(" ");
					}
				}
				i--;
			}
			fclose(fp);
		}
		printf("\nchosed food:%s\n",foodList);
		{
			if (foodList[0] != '\0'){
				char tmpNum[50] = "";
				int u = 0;
				while(foodList[u] != '\0'){
					if (foodList[u] != ','){
						int len = strlen(tmpNum);
						tmpNum[len] = foodList[u];
						tmpNum[len+1] = '\0';
					}else{
						{
							int orderNum = getFoodNum();
							char tmpStr[50];
							FILE* fp = fopen("foods.dat","r");
							for (int i=0;i<orderNum;i++){
								int list = 3;
								fgets(tmpStr,50,fp);
								if (tmpStr[0] == '['){
									i++;
									int o=1;
									while(tmpStr[o]!=']'){
										tmpStr[o-1] = tmpStr[o];
										o++;
									}
									tmpStr[o-1] = '\0';
									char tmpName[50];
									fgets(tmpName,50,fp);
									tmpName[strlen(tmpName)-1] = (tmpName[strlen(tmpName)-1] == '\n') ? '\0' : tmpName[strlen(tmpName)-1] ;
									char tmpPrice[50];
									fgets(tmpPrice,50,fp);
									tmpPrice[strlen(tmpPrice)-1] = (tmpPrice[strlen(tmpPrice)-1] == '\n') ? '\0' : tmpPrice[strlen(tmpPrice)-1] ;
									if (strcmp(tmpNum,tmpStr) == 0){
										total += atoi(tmpPrice);
									}
								}
								i--;
							}
							fclose(fp);
						}
						tmpNum[0] = '\0';
					}
					u++;
				}
			}
		}
		printf("Total: %d\n", total);
		printf("Please input food number :");
		char foodNumber[5];
		scanf("%s",&foodNumber);
		if (strcmp(foodNumber,"end")==0){
			int orderNumber;
			{
				int orderNum = getOrderNum();
				char tmpStr[20];
				FILE* fp = fopen("orders.dat","r");
				for (int i=0;i<orderNum;i++){
					fgets(tmpStr,20,fp);
					if (tmpStr[0] == '['){
						i++;
						int u=1;
						while(tmpStr[u]!=']'){
							tmpStr[u-1] = tmpStr[u];
							u++;
						}
						tmpStr[u-1] = '\0';
						orderNumber = (i == orderNum) ? atoi(tmpStr)+1 : 0;
					}
					i--;
				}
				fclose(fp);
			}
			{
				FILE* fp = fopen("orders.dat","at");
				fprintf(fp, "\n\n[%04d]\n%s\n%d\n%s\n%s\n%s",orderNumber,foodList,total,orderName,phoneNumber,address);
				fclose(fp);
			}
			editOrderList();
			break;
		}
		char tmpfoodNum[5];
		sprintf(tmpfoodNum,"[%s]\n",foodNumber);
		{
			char tmpStr[20];
			FILE* fp = fopen("foods.dat","r");
			while(!feof(fp)){
				fgets(tmpStr,20,fp);
				if (tmpStr[0] == '['){
					if (strcmp(tmpStr,tmpfoodNum)==0){
						strcat(foodList,foodNumber);
						strcat(foodList,",");
					}
				}
			}
			fclose(fp);
		}
	}while(1);
}
void editOrder(int i){
	char foodList[50];
	char total[50];
	char orderName[50];
	char phoneNumber[50];
	char address[50];
	{
		char tmpStr[50];
		FILE* fp = fopen("orders.dat","r");
		for(int u=0;u<i;u++){
			fgets(tmpStr,50,fp);
			if (tmpStr[0] == '['){
				u++;
				fgets(foodList,50,fp);
				foodList[strlen(foodList)-1] = (foodList[strlen(foodList)-1] == '\n') ? '\0' : foodList[strlen(foodList)-1] ;
				fgets(total,50,fp);
				total[strlen(total)-1] = (total[strlen(total)-1] == '\n') ? '\0' : total[strlen(total)-1] ;
				fgets(orderName,50,fp);
				orderName[strlen(orderName)-1] = (orderName[strlen(orderName)-1] == '\n') ? '\0' : orderName[strlen(orderName)-1] ;
				fgets(phoneNumber,50,fp);
				phoneNumber[strlen(phoneNumber)-1] = (phoneNumber[strlen(phoneNumber)-1] == '\n') ? '\0' : phoneNumber[strlen(phoneNumber)-1] ;
				fgets(address,50,fp);
				address[strlen(address)-1] = (address[strlen(address)-1] == '\n') ? '\0' : address[strlen(address)-1] ;
			}
			u--;
		}
	}
	cleanScreen();
	do{
		printf("You are editing the order that belong to %s\nThe phoneNumber is %s\nThe foods are %s\nTotal %sS$\n\n",orderName,phoneNumber,foodList,total);
		printf("Press f to change the foods,d to delete the order.Press esc to come back.");
		int code = getch();
		switch(code){
			case 27:
				editOrderList();
				break;
			case 100:
				do{
					cleanScreen();
					printf("Do you really want to delete this order?\nPress y to delete or n to cancel.");
					int code2 = getch();
					if (code2 == 110){
						editOrder(i);
					}else if (code2 == 121){
						break;
					}
				}while(1);
				{
					char tmplist[10240] = "";
					int u = 0;
					char tmpStr[50];
					FILE* fp = fopen("orders.dat","r");
					while(!feof(fp)){
						fgets(tmpStr,50,fp);
						if (tmpStr[0] == '['){
							u++;
							if (u != i){
								printf("%d\n",i);
								char foodList[50];
								char total[50];
								char orderName[50];
								char phoneNumber[50];
								char address[50];
								fgets(foodList,50,fp);
								fgets(total,50,fp);
								fgets(orderName,50,fp);
								fgets(phoneNumber,50,fp);
								fgets(address,50,fp);
								strcat(tmplist,"\n\n");
								strcat(tmplist,tmpStr);
								strcat(tmplist,foodList);
								strcat(tmplist,total);
								strcat(tmplist,orderName);
								strcat(tmplist,phoneNumber);
								strcat(tmplist,address);
							}
						}
					}
					fclose(fp);
					fp = fopen("orders.dat","w");
					fprintf(fp,"%s",tmplist);
					fclose(fp);
				}
				editOrderList();
				break;
			case 102:
				cleanScreen();
				char* oriFoodList = foodList;
				char foodList[50] = "";
				do{
					int total = 0;
					cleanScreen();
					printf("The original foods are : %s\n\n",oriFoodList);
					{
						int orderNum = getFoodNum();
						char tmpStr[50];
						FILE* fp = fopen("foods.dat","r");
						for (int i=0;i<orderNum;i++){
							int list = 3;
							fgets(tmpStr,50,fp);
							if (tmpStr[0] == '['){
								i++;
								int u=1;
								while(tmpStr[u]!=']'){
									tmpStr[u-1] = tmpStr[u];
									u++;
								}
								tmpStr[u-1] = '\0';
								char tmpName[50];
								fgets(tmpName,50,fp);
								tmpName[strlen(tmpName)-1] = (tmpName[strlen(tmpName)-1] == '\n') ? '\0' : tmpName[strlen(tmpName)-1] ;
								char tmpPrice[50];
								fgets(tmpPrice,50,fp);
								tmpPrice[strlen(tmpPrice)-1] = (tmpPrice[strlen(tmpPrice)-1] == '\n') ? '\0' : tmpPrice[strlen(tmpPrice)-1] ;
								printf("%s.%s--%sS$",tmpStr,tmpName,tmpPrice);
								if (i%list == 0 || i == orderNum){
									printf("\n");
								}else{
									printf(" ");
								}
							}
							i--;
						}
						fclose(fp);
					}
					printf("\nchosed food:%s\n",foodList);
					{
						if (foodList[0] != '\0'){
							char tmpNum[50] = "";
							int u = 0;
							while(foodList[u] != '\0'){
								if (foodList[u] != ','){
									int len = strlen(tmpNum);
									tmpNum[len] = foodList[u];
									tmpNum[len+1] = '\0';
								}else{
									{
										int orderNum = getFoodNum();
										char tmpStr[50];
										FILE* fp = fopen("foods.dat","r");
										for (int i=0;i<orderNum;i++){
											int list = 3;
											fgets(tmpStr,50,fp);
											if (tmpStr[0] == '['){
												i++;
												int o=1;
												while(tmpStr[o]!=']'){
													tmpStr[o-1] = tmpStr[o];
													o++;
												}
												tmpStr[o-1] = '\0';
												char tmpName[50];
												fgets(tmpName,50,fp);
												tmpName[strlen(tmpName)-1] = (tmpName[strlen(tmpName)-1] == '\n') ? '\0' : tmpName[strlen(tmpName)-1] ;
												char tmpPrice[50];
												fgets(tmpPrice,50,fp);
												tmpPrice[strlen(tmpPrice)-1] = (tmpPrice[strlen(tmpPrice)-1] == '\n') ? '\0' : tmpPrice[strlen(tmpPrice)-1] ;
												if (strcmp(tmpNum,tmpStr) == 0){
													total += atoi(tmpPrice);
												}
											}
											i--;
										}
										fclose(fp);
									}
									tmpNum[0] = '\0';
								}
								u++;
							}
						}
					}
					printf("Total: %d\n", total);
					printf("Please input food number :");
					char foodNumber[5];
					scanf("%s",&foodNumber);
					if (strcmp(foodNumber,"end")==0){
						{
							char tmplist[10240] = "";
							int u = 0;
							char tmpStr[50];
							FILE* fp = fopen("orders.dat","r");
							while(!feof(fp)){
								fgets(tmpStr,50,fp);
								if (tmpStr[0] == '['){
									u++;
									if (u != i){
										printf("%d\n",i);
										char tmpfoodList[50];
										char tmptotal[50];
										char tmporderName[50];
										char tmpphoneNumber[50];
										char tmpaddress[50];
										fgets(tmpfoodList,50,fp);
										fgets(tmptotal,50,fp);
										fgets(tmporderName,50,fp);
										fgets(tmpphoneNumber,50,fp);
										fgets(tmpaddress,50,fp);
										strcat(tmplist,"\n\n");
										strcat(tmplist,tmpStr);
										strcat(tmplist,tmpfoodList);
										strcat(tmplist,tmptotal);
										strcat(tmplist,tmporderName);
										strcat(tmplist,tmpphoneNumber);
										strcat(tmplist,tmpaddress);
									}else{
										strcat(tmplist,"\n\n");
										strcat(tmplist,tmpStr);
										strcat(tmplist,foodList);
										char tmpPrice[50];
										sprintf(tmpPrice,"%d",total);
										strcat(tmplist,"\n");
										strcat(tmplist,tmpPrice);
										strcat(tmplist,"\n");
										strcat(tmplist,orderName);
										strcat(tmplist,"\n");
										strcat(tmplist,phoneNumber);
										strcat(tmplist,"\n");
										strcat(tmplist,address);
									}
								}
							}
							fclose(fp);
							fp = fopen("orders.dat","w");
							fprintf(fp,"%s",tmplist);
							fclose(fp);
						}
						editOrder(i);
						break;
					}
					char tmpfoodNum[5];
					sprintf(tmpfoodNum,"[%s]\n",foodNumber);
					{
						char tmpStr[20];
						FILE* fp = fopen("foods.dat","r");
						while(!feof(fp)){
							fgets(tmpStr,20,fp);
							if (tmpStr[0] == '['){
								if (strcmp(tmpStr,tmpfoodNum)==0){
									strcat(foodList,foodNumber);
									strcat(foodList,",");
								}
							}
						}
						fclose(fp);
					}
				}while(1);
				break;
		}
	}while(1);
}
void searchOrder(){
	cleanScreen();
	printf("Please input the phone number: \n");
	char phoneNumber[20];
	scanf("%s",&phoneNumber);
	{
		FILE* fp = fopen("orders.dat","r");
		char tmpStr[50];
		int i = 0;
		strcat(phoneNumber,"\n");
		while(!feof(fp)){
			fgets(tmpStr,50,fp);
			if (tmpStr[0] == '['){
				i++;
				fgets(tmpStr,50,fp);
				fgets(tmpStr,50,fp);
				fgets(tmpStr,50,fp);
				fgets(tmpStr,50,fp);
				if (strcmp(tmpStr,phoneNumber)==0){
					editOrder(i);
				}
			}
		}
		printf("Can't find this order,press any key to go back");
		getch();
		editOrderList();
	}
}
void listOrder(){
	int orderNum = getOrderNum();
	int pageNum = (orderNum + 5 - 1)/5;
	int correctPage = 1;
	do{
		cleanScreen();
		if (orderNum != 0){
			{
				char tmpStr[50];
				FILE* fp = fopen("orders.dat","r");
				int i = 0 - (correctPage-1)*5;
				while(!feof(fp)){
					fgets(tmpStr,50,fp);
					if (tmpStr[0] == '['){
						i++;
						fgets(tmpStr,50,fp);
						char total[50];
						fgets(total,50,fp);
						total[strlen(total)-1] = (total[strlen(total)-1] == '\n') ? '\0' : total[strlen(total)-1] ;
						char orderName[50];
						fgets(orderName,50,fp);
						orderName[strlen(orderName)-1] = (orderName[strlen(orderName)-1] == '\n') ? '\0' : orderName[strlen(orderName)-1] ;
						char phoneNumber[50];
						fgets(phoneNumber,50,fp);
						phoneNumber[strlen(phoneNumber)-1] = (phoneNumber[strlen(phoneNumber)-1] == '\n') ? '\0' : phoneNumber[strlen(phoneNumber)-1] ;
						if (i>0 && i<=5){
						printf("%d.%s--%s--%sS$\n",i+(correctPage-1)*5,orderName,phoneNumber,total);
						}
					}
				}
				fclose(fp);
			}
		}else{
			printf("There is no order!Press any key to go back.");
			getch();
			editOrderList();
		}
		printf("Press up or down button to change page.\nPress esc to go back.");
		int code1 = getch();
		if (code1 == 224){
			int code2 = getch();
			if (code2 == 72){
				correctPage -= (correctPage>1) ? 1 : 0;
			}else if(code2 == 80){
				correctPage += (correctPage<pageNum) ? 1 : 0;
			}
		}else if (code1 == 27){
			editOrderList();
			break;
		}
	}while(1);
}
void editOrderList(){
	char SelectStr[] = "Please select a function:";
	char function1[] = ".Add an order";
	char function2[] = ".Search an order by phone number";
	char function3[] = ".List all orders";
	int chosedFuncNum = 1;
	char chosedSign;
	int code1;
	int code2;
	do{
		cleanScreen();
		printf("%s\n",SelectStr);
		chosedSign = (chosedFuncNum == 1 ) ? '>':' ';
		printf(" %c%d%s\n\n",chosedSign,1,function1);
		chosedSign = (chosedFuncNum == 2 ) ? '>':' ';
		printf(" %c%d%s\n\n",chosedSign,2,function2);
		chosedSign = (chosedFuncNum == 3 ) ? '>':' ';
		printf(" %c%d%s\n\n",chosedSign,3,function3);
		code1 = getch();
		if (code1 == 224){
			code2 = getch();
			if (code2 == 72){
				chosedFuncNum-=(chosedFuncNum>1)?1:0;
			}else if (code2 == 80){
				chosedFuncNum+=(chosedFuncNum<3)?1:0;
			}
		}else if (code1 == 13){
			switch (chosedFuncNum){
				case 1:
					addOrder();
					break;
				case 2:
					searchOrder();
					break;
				case 3:
					listOrder();
					break;
			}
		}else if (code1 == 27){
			functionSelect();
			break;
		}
	}while(1);
}
void functionSelect(){
	char SelectStr[] = "Please select a function:";
	char function1[] = ".Edit the food list";
	char function2[] = ".Edit the order list";
	char function3[] = ".Exit the System";
	int chosedFuncNum = 1;
	char chosedSign;
	int code1;
	int code2;
	if(!isAdmin){
		do{
			cleanScreen();
			printf("%s\n",SelectStr);
			chosedSign = (chosedFuncNum == 1 ) ? '>':' ';
			printf(" %c%d%s\n\n",chosedSign,1,function2);
			chosedSign = (chosedFuncNum == 2 ) ? '>':' ';
			printf(" %c%d%s\n\n",chosedSign,2,function3);
			code1 = getch();
			if (code1 == 224){
				code2 = getch();
				if (code2 == 72){
					chosedFuncNum-=(chosedFuncNum>1)?1:0;
				}else if (code2 == 80){
					chosedFuncNum+=(chosedFuncNum<2)?1:0;
				}
			}else if (code1 == 13){
				switch (chosedFuncNum){
					case 1:
						editOrderList();
						break;
					case 2:
						cleanScreen();
						printf("Have a nice day!Bye~");
						exit(-1);
				}
			}
		}while(1);
	}else{
		do{
			cleanScreen();
			printf("%s\n",SelectStr);
			chosedSign = (chosedFuncNum == 1 ) ? '>':' ';
			printf(" %c%d%s\n\n",chosedSign,1,function1);
			chosedSign = (chosedFuncNum == 2 ) ? '>':' ';
			printf(" %c%d%s\n\n",chosedSign,2,function2);
			chosedSign = (chosedFuncNum == 3 ) ? '>':' ';
			printf(" %c%d%s\n\n",chosedSign,3,function3);
			code1 = getch();
			if (code1 == 224){
				code2 = getch();
				if (code2 == 72){
					chosedFuncNum-=(chosedFuncNum>1)?1:0;
				}else if (code2 == 80){
					chosedFuncNum+=(chosedFuncNum<3)?1:0;
				}
			}else if (code1 == 13){
				switch (chosedFuncNum){
					case 1:
						editFoodList();
						break;
					case 2:
						editOrderList();
						break;
					case 3:
						cleanScreen();
						printf("Have a nice day!Bye~");
						exit(-1);
				}
			}
		}while(1);
	}
}
int main(){
	checkConf();
	Login();
	functionSelect();
	return 0;
}
