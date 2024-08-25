#include<stdio.h>
#include<string.h>
#include<math.h>
#include<stdlib.h>
struct patient{
char name[20];
int gender;
int age;
int sugar;
int pulse;
int chol;
int bps;
int bpd;

};
int FileExist(char path[]){
	FILE *file;
	if (file=fopen(path,"r")){
		fclose(file);
		return 1;
	} else return 0;
}
int GetSugar(FILE *file,int prevsug,int fexist){
float sugar;
int suglev;
char mgdl[5]="mg/dL",mmoll[6]="mmol/L",unit[6];
printf("\n||******************************************************************************||");
printf("\n||                                                                              ||");
printf("\n||             ENTER YOUR SUGAR LEVEL WITH APPROPRIATE UNITS                    ||");
printf("\n||                                                                              ||");
printf("\n||******************************************************************************||");
printf("\n						Sugar level: ");
scanf("%f %s",&sugar,unit);fflush(stdin);
printf("\n||******************************************************************************||");
if (strcmp(mmoll,unit)==0){
	sugar=sugar*18;
}
if (fexist){
	if (prevsug<sugar){
		printf("\nYour sugar level has increased since your last visit!\n");
	} else if (prevsug>sugar){
		printf("\nYour sugar level has decreased since your last visit!\n");
	}
}
if (sugar<70){
suglev = -1;
} else if (sugar>=70 && sugar<100){
suglev = 0;
} else if (sugar>=100 && sugar<126){
suglev = 1;
} else {
suglev = 2;
}
fprintf(file,"SUGAR LEVEL (in mg/dL): %d\n",(int)sugar,unit);
return suglev;
}

void GetBP(int *sys, int *dias,FILE *file,int prevsys,int prevdias,int fexist){
int X,Y;
printf("\n||******************************************************************************||");
printf("\n||                                                                              ||");
printf("\n||             ENTER YOUR BLOOD PRESSURE IN THE FORMAT \"X/Y\",                 ||");
printf("\n||  WHERE X IS THE SYSTOLIC (HIGHER) AND Y IS THE DIASTOLIC (LOWER) READING.    ||");
printf("\n||                         (UNITS NOT REQUIRED)                                 ||");
printf("\n||******************************************************************************||");
printf("\n							 Blood pressure: ");
scanf("%d/%d",&X,&Y);
printf("\n||******************************************************************************||");
fprintf(file,"SYSTOLIC BLOOD PRESSURE (in mm Hg): %d\nDIASTOLIC BLOOD PRESSURE (in mm Hg): %d\n",X,Y);
if (fexist){
	if (prevsys<X){
		printf("\nYour systolic blood pressure has increased since your last visit!\n");
	} else if (prevsys>X){
		printf("\nYour systolic blood pressure has decreased since your last visit!\n");
	}
}
if (fexist){
	if (prevdias<Y){
		printf("\nYour diastolic blood pressure has increased since your last visit!\n");
	} else if (prevdias>Y){
		printf("\nYour diastolic blood pressure has decreased since your last visit!\n");
	}
}
if (X<90){
*sys=-1;
} else if (X>=90 && X<=120){
*sys=0;
} else {
if (X<130){
*sys=1;
} else {
*sys=2;
}
}
if (Y<60){
*dias=-1;
} else if (Y>=60 && Y<=80){
*dias=0;
} else if (Y>80 && Y<=90){
*dias=1;
} else {
*dias=2;
}
}

int Getpulse(FILE *file,int prevpul,int fexist)
{
int pulse;
printf("\n||************************************************************************||");
printf("\n||                                                                        ||");
printf("\n||                      ENTER YOUR PULSE RATE                             ||");
printf("\n||                      (UNITS NOT REQUIRED)                              ||");
printf("\n||************************************************************************||");
printf("\n						      Pulse rate: ");
scanf("%d",&pulse);
printf("\n||************************************************************************||");
fprintf(file,"PULSE RATE (in BPM): %d\n",pulse);
if (fexist){
	if (prevpul<pulse){
		printf("\nYour pulse rate has increased since your last visit!\n");
	} else if (prevpul>pulse){
		printf("\nYour pulse rate has decreased since your last visit!\n");
	}
}
if(pulse < 60)
{
return -1;
}
else if(pulse>=60 && pulse<=100)
{
return 0;
}
else
{
return 1;
}
}

int Getchol(FILE *file,int prevchol,int fexist)
{
int chol;
printf("\n||************************************************************************||");
printf("\n||                                                                        ||");
printf("\n||                    ENTER YOUR CHOLESTEROL LEVEL                        ||");
printf("\n||                      (UNITS NOT REQUIRED)                              ||");
printf("\n||************************************************************************||");
printf("\n                           Cholesterol: ");
scanf("%d",&chol);
printf("\n||************************************************************************||");
fprintf(file,"CHOLESTEROL (in mg/dL): %d\n",chol);
if (fexist){
	if (prevchol<chol){
		printf("\nYour cholesterol level has increased since your last visit!\n");
	} else if (prevchol>chol){
		printf("\nYour cholesterol level has decreased since your last visit!\n");
	}
}
if(chol<50)

{
return -1;
}
else if (chol>=50 && chol<100){
	return 0;
}
else if(chol>=100 && chol<130)
{
return 1;
}
else if(chol>=130&&chol<160)
{
return 2;
}
else if(chol>=160&&chol<=189)
{
return 3;
}
else return 4;

}

void sugartobp(int sug,int bp,int *cons){
	switch(sug){
		case -1:
			if (bp==-1){
				*cons=-4;
			} else if (bp==0){
				*cons=-3;
			} else {
				*cons=-2;
			}
			break;
		case 0:
			if (bp==-1){
				*cons=-1;
			} else if (bp==0){
				*cons=0;
			} else {
				*cons=1;
			}
			break;
		case 1:
		default:
			if (bp==-1){
				*cons=2;
			} else if (bp==0){
				*cons=3;
			} else {
				*cons=4;
			}
			break;
	}
}
void bptochol(int bp,int chol,int *cons){
	switch(bp){
		case -1:
			if (chol==-1){
				*cons=-4;
			} else if (chol==0){
				*cons=-3;
			} else {
				*cons=-2;
			}
			break;
		case 0:
			if (chol==-1){
				*cons=-1;
			} else if (chol==0){
				*cons=0;
			} else {
				*cons=1;
			}
			break;
		case 1:
		default:
			if (chol==-1){
				*cons=2;
			} else if (chol==0){
				*cons=3;
			} else {
				*cons=4;
			}
			break;
	}
}
void sugartopul(int sug,int pul,int *cons){
	switch(sug){
		case -1:
			if (pul==-1){
				*cons=-4;
			} else if (pul==0){
				*cons=-3;
			} else {
				*cons=-2;
			}
			break;
		case 0:
			if (pul==-1){
				*cons=-1;
			} else if (pul==0){
				*cons=0;
			} else {
				*cons=1;
			}
			break;
		case 1:
		default:
			if (pul==-1){
				*cons=2;
			} else if (pul==0){
				*cons=3;
			} else {
				*cons=4;
			}
			break;
			
	}
}
void sugartochol(int sug,int chol,int *cons){
	switch(sug){
		case -1:
			if (chol==-1){
				*cons=-4;
			} else if (chol==0){
				*cons=-3;
			} else {
				*cons=-2;
			}
			break;
		case 0:
			if (chol==-1){
				*cons=-1;
			} else if (chol==0){
				*cons=0;
			} else {
				*cons=1;
			}
			break;
		case 1:
		default:
			if (chol==-1){
				*cons=2;
			} else if (chol==0){
				*cons=3;
			} else {
				*cons=4;
			}
			break;
	}
}
void bptopul(int pul,int BP,int *cons){
	switch(BP){
		case -1:
			if (pul==-1){
				*cons=-4;
			} else if (pul==0){
				*cons=-3;
			} else {
				*cons=-2;
			}
			break;
		case 0:
			if (pul==-1){
				*cons=-1;
			} else if (pul==0){
				*cons=0;
			} else {
				*cons=1;
			}
			break;
		case 1:
		default:
			if (pul==-1){
				*cons=2;
			} else if (pul==0){
				*cons=3;
			} else {
				*cons=4;
			}
			break;
	}
}
void pultochol(int pul,int chol,int *cons){
	switch(pul){
		case -1:
			if (chol==-1){
				*cons=-4;
			} else if (chol==0){
				*cons=-3;
			} else {
				*cons=-2;
			}
			break;
		case 0:
			if (chol==-1){
				*cons=-1;
			} else if (chol==0){
				*cons=0;
			} else {
				*cons=1;
			}
			break;
		case 1:
			if (chol==-1){
				*cons=2;
			} else if (chol==0){
				*cons=3;
			} else {
				*cons=4;
			}
			break;
	}
}
int main(){
char name[20],data[100],line1[100],unit[5];
int check=0,patients,BP,cons[6],line=1,len,i=0,j=0,fexist,info[5]={0};
printf("Enter number of patients: ");
scanf("%d",&patients);fflush(stdin);
struct patient input[patients];
char *names[patients];
for (int a=0;a<patients;a++){
char path[]="D:\\";
FILE *fptr;
printf("\n||************************************************************************||");
printf("\n||                                                                        ||");
printf("\n||                      ENTER YOUR NAME :                                 ||");
printf("\n||                                                                        ||");
printf("\n||************************************************************************||\n");
printf("\n                           Name: ");
scanf("%s",&input[a].name);fflush(stdin);
printf("\n||************************************************************************||");
for (int b=0;b<a;++b){
	if (strcmp(input[a].name,names[b])==0){
		printf("Data for patient already taken.\n");
		break;
	}
	check++;
}
if (check==a){
	names[a]=(char*)malloc(strlen(input[a].name)*sizeof(char));
	strcpy(names[a],input[a].name);
	strcpy(name,input[a].name);
	strcat(path,strcat(input[a].name,".txt"));
	fexist=FileExist(path);
	if (fexist==1){
		fptr=fopen(path,"r");
		while(!feof(fptr)||line<=6){
			fgets(data,100,fptr);
			if (line==1){
				strcpy(line1,data);
			} else {
				len=strlen(data)-2;
				while (data[len]>='0'&&data[len]<='9'){
					info[i]+=(((int)(data[len])-'0')*pow(10,j));
					len--;
					j++;
				}
				i++;
			}
			j=0;
			line++;
		}
		line=0;
		i=0;
		fclose(fptr);
		printf("\nWelcome back %s!",name);
		fptr=fopen(path,"w+");
		fprintf(fptr,"%s",line1);
	} else {
		fptr=fopen(path,"w");
		fprintf(fptr,"NAME: %s, ",name);
		printf("\n||*********************************************************************||");;
		printf("\n||                                                                     ||");
    	printf("\n||             PLEASE SPECIFY YOUR GENDER (1=Male, 2=Female)           ||");
		printf("\n||                                                                     ||");
    	printf("\n||*********************************************************************||\n");
		printf("\n                            Gender: ");
		scanf("%d",&input[a].gender);fflush(stdin);
		printf("\n||*********************************************************************||");
		fprintf(fptr,"GENDER: %d, ",input[a].gender);
		printf("\n||*********************************************************************||");;
		printf("\n||                                                                     ||");
		printf("\n||                       ENTER YOUR AGE                                ||");
		printf("\n||                                                                     ||");
		printf("\n||*********************************************************************||\n");
		printf("\n                               Age: ");
		scanf("%d",&input[a].age);
		printf("\n||*********************************************************************||");
		fprintf(fptr,"AGE: %d\n",input[a].age);
	}
	input[a].sugar=GetSugar(fptr,info[0],fexist);
	GetBP(&input[a].bps,&input[a].bpd,fptr,info[1],info[2],fexist);
	switch(input[a].bps){
		case -1:
			if (input[a].bpd==-1){
				BP = -1;
			} else if (input[a].bpd==0){
				BP = 0;
			} else if (input[a].bpd==1){
				BP = 1;
			} else BP = 2;
			break;
		case 0:
			if (input[a].bpd<=0){
				BP = 0;
			} else if (input[a].bpd == 1){
				BP = 1;
			} else BP = 2;
			break;
		case 1:
			if (input[a].bpd<=1){
				BP = 1;
			} else BP = 2;
			break;
		default:
			BP = 2;
			break;
	}
input[a].pulse=Getpulse(fptr,info[3],fexist);
input[a].chol=Getchol(fptr,info[4],fexist);
printf("\nFrom your information, we can determine the following issues:\n");
switch(input[a].sugar){
	case -1:
		printf("-\tLow sugar level\n");
		break;
	case 0:
		;
		break;
	case 1:
		printf("-\tHigh sugar level\n");
		break;
	default:
		printf("-\tVery high sugar level\n");
		break;
}
switch(BP){
	case -1:
		printf("-\tLow blood pressure\n");
		break;
	case 0:
		;
		break;
	case 1:
		printf("-\tHigh blood pressure\n");
		break;
	default:
		printf("-\tVery high blood pressure\n");
		break;
}
switch(input[a].pulse){
	case -1:
		printf("-\tLow heart rate\n");
		break;
	case 0:
		;
		break;
	default:
		printf("-\tHigh heart rate\n");
		break;
}
switch(input[a].chol){
	case -1:
		printf("-\tLow cholesterol\n");
		break;
	case 0:
		;
		break;
	case 1:
		printf("-\tHigh cholesterol\n");
		break;
	default:
		printf("-\tVery high cholesterol\n");
		break;
}
printf("Please check your file for further information: %s",path);
fprintf(fptr,"\nDETAILS\n");
sugartobp(input[a].sugar,BP,&cons[0]);
switch(cons[0]){
	case -4:
		fprintf(fptr,"-\tYour low sugar level might be the main cause of your low blood pressure.\n\tThe main complications that this can cause are: dizziness, weakness, and fainting.\n");
		break;
	case -3:
		fprintf(fptr,"-\tYou have hypoglycemia.\n\tThis can result in: dizziness, sweating, anxiety, pale skin and hunger pangs.\n");
		break;
	case -2:
		fprintf(fptr,"-\tLow sugar and high blood pressure is a very alarming situation that may result in hypertension.\n\tIf systolic blood pressure is higher than 160 mm Hg, consult your doctor immediately.\n");
		break;
	case -1:
		fprintf(fptr,"-\tYou are experiencing low blood pressure, also known as hypotension.\n\tSide effects of this include dizziness, fainting, etc.\n\tConsult your doctor immediately in case you experience these symptoms for a prolonged duration.\n");
		break;
	case 0:
		;
		break;
	case 1:
		fprintf(fptr,"-\tYou are experiencing high blood pressure, also known as hypertension.\n\tSide effects of this include heart damage and brain damage.\n\tIt is recommended to consult a doctor immediately.\n");
		break;
	case 2:
		fprintf(fptr,"-\tHigh sugar with low blood pressure indicates diabetes.\n\tThe nerve sensors that monitor blood pressure do not work properly under high sugar, causing drastic drops in blood pressure.\n\tFurther symptoms of diabetes include extreme thirst and hunger, blurry vision and dry skin.\n");
		break;
	case 3:
		fprintf(fptr,"-\tYou have hyperglycemia.\n\tThis can result in: heart and kidney diseases, stroke, and vision and nerve problems. This can, of course, result in diabetes.\n");
		break;
	default:
		fprintf(fptr,"-\tHigh sugar and high blood pressure indicate risk of heart and kidney failure. Vision and nervous system can be compromised and major chances of stroke are there. Visit the doctor ASAP.\n");
		break;
}
sugartopul(input[a].sugar,input[a].pulse,&cons[1]);
switch(cons[1]){
	case -4:
		fprintf(fptr,"-\tAlthough there is no such association between the two, low sugar level (hypoglycaemia) and low pulse rate (bradychardia) combined can create fatal heart rhythms.");
		break;
	case -3:
		if (cons[0]==-3){
			;
		} else {
			fprintf(fptr,"-\tYou have hypoglycemia.\n\tThis can result in: dizziness, sweating, anxiety, pale skin and hunger pangs.\n");
		}
		break;
	case -2:
		fprintf(fptr,"-\tYour insufficient sugar levels might be causing rapid heart palpitations which is leading to a greater pulse.\n");
		break;
	case -1:
		fprintf(fptr,"-\tLow pulse rate is an indication of Bradychardia\n\tBradychardia can cause chest pain, confusion, and dizziness.\n\tBradychardia gets progressively worse over the years, so it is advisable to take precautions immediately.\n");
		break;
	case 0:
		;
		break;
	case 1:
		fprintf(fptr,"-\tHigh pulse rate is an indication of Tachychardia.\n\tTachychardia is not always a cause of concern, as heart rate tends to spike after physical activity like exercise.\n\tSigns of tachycardia include a pounding heartbeat, chest pain, shortness of breath, etc.\n\tIf you experience symptoms like fainting and weakness, seek immediate medical help.\n");
		break;
	case 2:
		fprintf(fptr,"-\tLow pulse is closely associated with low blood pressure, so coupled with high sugar, this can indicate diabetes.\n\tThe nerve sensors that monitor blood pressure do not work properly under high sugar, causing drastic drops in blood pressure.\n\tFurther symptoms of diabetes include extreme thirst and hunger, blurry vision and dry skin.\n");
		break;
	case 3:
		if (cons[0]==3){
			;
		} else {
			fprintf(fptr,"-\tYou have hyperglycemia.\nThis can result in: heart and kidney diseases, stroke, and vision and nerve problems. This can, of course, result in diabetes.\n");
		}
		break;
	default:
		fprintf(fptr,"-\tHigh pulse is a side-effect of high sugar levels.\n\tIf left untreated, these two conditions can lead to severe diseases including cardiovascular and kidney diseases.\n\tIf you have chest pain, weakness or fatigue, please consult your doctor immediately.\n");
		break;
}
sugartochol(input[a].sugar,input[a].chol,&cons[2]);
switch(cons[2]){
	case -4:
		fprintf(fptr,"-\tSigns of low sugar level (hypoglycaemia) include sweating, dizziness, pounding heartbeat, and hunger.\n\tThis coupled with a low cholesterol level increases risk for diseases such as cancer and hemorrhagic stroke.\n\tConsult your doctor immediately in case you are experiencing any symptoms.\n");
		break;
	case -3:
		if (cons[0]==-3||cons[1]==-3){
			;
		} else {
			fprintf(fptr,"-\tYou have hypoglycemia.\n\tThis can result in: dizziness, sweating, anxiety, pale skin and hunger pangs.\n");
		}
		break;
	case -2:
		fprintf(fptr,"-\tHigh cholesterol is normally caused by higher sugar consumption, so your case is unusual.\n\tHowever, look out for symptoms such as anxiety, pain in the arms and in the chest- your higher cholesterol levels might lead to a heart attack.\n");
		break;
	case -1:
		fprintf(fptr,"-\tLow levels of cholesterol are generally considered beneficial, but in some cases might lead to health problems.\n\tThese problems include cancer, hemorrhagic stroke and depression.\n\tConsult your doctor if you are experiencing symptoms of any of these diseases.\n");
		break;
	case 0:
		;
		break;
	case 1:
		fprintf(fptr,"-\tHigh cholesterol can be a result of various dangers, including:\n\t\tCoronary heart disease,\n\t\tStroke, or\n\t\tHeart attack\n\tSome general symptoms of these diseases include anxiety, chest pain, and dizziness.\n\tIf you have any of these symptoms, consult your doctor immediately.\n");
		break;
	case 2:
		fprintf(fptr,"-\tLow cholesterol is normally caused by lower sugar consumption, so your case is unusual.\n\tHowever, look out for symptoms such as extreme thirst and hunger, unintended weight loss, and dry skin- you might be showing early signs of diabetes.\n");
		break;
	case 3:
		if (cons[0]==3||cons[1]==3){
			;
		} else {
			fprintf(fptr,"-\tYou have hyperglycemia.\nThis can result in: heart and kidney diseases, stroke, and vision and nerve problems. This can, of course, result in diabetes.\n");
		}
		break;
	default:
		fprintf(fptr,"-\tHigh sugar can directly cause high cholesterol, and vice versa.\n\tThese two factors increase the risk of diabetes and heart attack.\n\tConsult your doctor if you are experiencing symptoms of these diseases.\n");
		break;
}
bptopul(input[a].pulse,BP,&cons[3]);
switch(cons[3]){
	case -4://low both
		fprintf(fptr,"-\tLow blood pressure might be a result of low heart rate, or bradychardia\n\tBradychardia can cause chest pain, confusion, and dizziness.\n\tBradychardia gets progressively worse over the years, so it is advisable to take precautions immediately.\n");
		break;
	case -3://low bp only
		if (cons[0]==-1){
			;
		} else {
			fprintf(fptr,"-\tYou are experiencing low blood pressure, also known as hypotension.\n\tSide effects of this include dizziness, fainting, etc.\n\tConsult your doctor immediately in case you experience these symptoms for a prolonged duration.\n");
		}
		break;
	case -2://low bp,high pul
		fprintf(fptr,"-\tLow blood pressure (hypotension) coupled with high pulse rate can be a sign of the following:\n\t\tPregnancy,\n\t\tNutrient deficiency,\n\t\tSevere infection,\n\t\tAllergic reaction, or\n\t\tDehydration.\n");
		break;
	case -1://low pul only
		if (cons[1]==-1){
			;
		} else {
			fprintf(fptr,"-\tLow pulse rate is an indication of Bradychardia\n\tBradychardia can cause chest pain, confusion, and dizziness.\n\tBradychardia gets progressively worse over the years, so it is advisable to take precautions immediately.\n");
		}
		break;
	case 0://normal
		;
		break;
	case 1://high pul only
		if (cons[1]==1){
			;
		} else {
			fprintf(fptr,"-\tHigh pulse rate is an indication of Tachychardia.\n\tTachychardia is not always a cause of concern, as heart rate tends to spike after physical activity like exercise.\n\tSigns of tachycardia include a pounding heartbeat, chest pain, shortness of breath, etc.\n\tIf you experience symptoms like fainting and weakness, seek immediate medical help.\n");
		}
		break;
	case 2://high bp,low pul
		fprintf(fptr,"-\tHigh blood pressure (hypertension) but low pulse can be due to use of anti-hypertensive medication or due to a thickened heart tissue\n\tIf you experience any of the following symptomps, please head immediately to an emergency room\n\t\tChest or back pain\n\t\tShortness of breath\n\t\tDifficulty speaking\n");
		break;
	case 3://high bp
		if (cons[0]==1){
			;
		} else {
			fprintf(fptr,"-\tYou are experiencing high blood pressure, also known as hypertension.\n\tSide effects of this include heart damage and brain damage.\n\tIt is recommended to consult a doctor immediately.\n");
		}
		break;
	default://high both
		fprintf(fptr,"-\tHigh pulse rate is closely associated with high blood pressure and is indicative of risk for cardiovascular disease\n\tSymptoms of cardiovascular disease include chest discomfort, pain in the neck, upper belly area, or back, weakness in the arms, etc.\n\tCardiovascular disease tends to decrease life expectancy by 10 per cent therefore it is advisable to seek immediate help.\n");
		break;
}
bptochol(BP,input[a].chol,&cons[4]);
switch(cons[4]){
	case -4:
		fprintf(fptr,"-\tIn general, lower levels of cholesterol and BP are beneficial.\n\tHowever, it is necessary to maintain them at a stable level, otherwise worrying symptoms such as dizziness, confusion, and even stroke, might appear.\n");
		break;
	case -3:
		if (cons[0]==-1||cons[3]==-3){
			;
		} else {
			fprintf(fptr,"-\tYou are experiencing low blood pressure, also known as hypotension.\n\tSide effects of this include dizziness, fainting, etc.\n\tConsult your doctor immediately in case you experience these symptoms for a prolonged duration.\n");
		}
		break;
	case -2:
		fprintf(fptr,"-\tEven though high cholesterol and low blood pressure might not be interconnected, they should both individually be tended to.\n\tHigh cholesterol can lead to several types of cardiovascular diseases, while low blood pressure can lead to dizziness, fainting, etc.\n");
		break;
	case -1:
		if (cons[2]==-1){
			;
		} else {
			fprintf(fptr,"-\tLow levels of cholesterol are generally considered beneficial, but in some cases might lead to health problems.\n\tThese problems include cancer, hemorrhagic stroke and depression.\n\tConsult your doctor if you are experiencing symptoms of any of these diseases.\n");
		}
		break;
	case 0:
		;
		break;
	case 1:
		if (cons[2]==1){
			;
		} else {
			fprintf(fptr,"-\tHigh cholesterol can be a result of various dangers, including:\n\t\tCoronary heart disease,\n\t\tStroke, or\n\t\tHeart attack\n\tSome general symptoms of these diseases include anxiety, chest pain, and dizziness.\n\tIf you have any of these symptoms, consult your doctor immediately.\n");
		}
		break;
	case 2:
		fprintf(fptr,"-\tAlthough low cholesterol is generally considered a good sign, your elevated blood pressure suggests otherwise.\n\tYou might have a condition known as metabolic syndrome, which puts you at risk of heart diseases.\n\tOther indications of metabolic syndrome include high glucose and triglyceride levels\n\tPlease get yourself tested for these and consult a doctor in case they are also elevated.\n");
		break;
	case 3:
		if (cons[0]==1||cons[3]==3){
			;
		} else {
			fprintf(fptr,"-\tYou are experiencing high blood pressure, also known as hypertension.\n\tSide effects of this include heart damage and brain damage.\n\tIt is recommended to consult a doctor immediately.\n");
		}
		break;
	default:
		fprintf(fptr,"-\tHigh blood pressure is common in a person with high cholesterol levels, and vice versa.\n\tBoth of these are risk factors for heart diseases, so it is suggested to consult your doctor immediately.\n");
		break;
}
pultochol(input[a].pulse,input[a].chol,&cons[5]);
switch(cons[5]){
	case -4:
		fprintf(fptr,"-\tLower pulse and cholesterol generally correspond to a lesser risk of heart diseases.\n\tHowever, it is still advised to maintain pulse and cholesterol at a higher level in order to avoid symptoms of bradychardia (low pulse rate) and symptoms associated with lower cholesterol.\n");
		break;
	case -3:
		if (cons[1]==-1||cons[3]==-1){
			;
		} else {
			fprintf(fptr,"-\tLow pulse rate is an indication of Bradychardia\n\tBradychardia can cause chest pain, confusion, and dizziness.\n\tBradychardia gets progressively worse over the years, so it is advisable to take precautions immediately.\n");
		}
		break;
	case -2:
		fprintf(fptr,"-\tGenerally, low pulse is associated with low cholesterol, so it is possible that a separate underlying disease is causing the elevated cholesterol level.\n\tIn such case, consult a doctor immediately.\n\tIt is also suggested to check for symptoms of bradychardia like chest pain, confusion and dizziness.\n");
		break;
	case -1:
		if (cons[2]==-1||cons[4]==-1){
			;
		} else {
			fprintf(fptr,"-\tLow levels of cholesterol are generally considered beneficial, but in some cases might lead to health problems.\n\tThese problems include cancer, hemorrhagic stroke and depression.\n\tConsult your doctor if you are experiencing symptoms of any of these diseases.\n");
		}
		break;
	case 0:
		;
		break;
	case 1:
		if (cons[2]==1||cons[4]==1){
			;
		} else {
			fprintf(fptr,"-\tHigh cholesterol can be a result of various dangers, including:\n\t\tCoronary heart disease,\n\t\tStroke, or\n\t\tHeart attack\n\tSome general symptoms of these diseases include anxiety, chest pain, and dizziness.\n\tIf you have any of these symptoms, consult your doctor immediately.\n");
		}
		break;
	case 2:
		fprintf(fptr,"-\tAlthough low cholesterol is generally a good sign, coupled with high heart rate it can be a sign of metabolic syndrome.\n\tThis syndrome increases the risk of heart disease.\n\tOther signs of it include high glucose and triglyceride levels, so it is recommended to get these levels checked.\n");
		break;
	case 3:
		if (cons[1]==1||cons[3]==1){
			;
		} else {
			fprintf(fptr,"-\tHigh pulse rate is an indication of Tachychardia.\n\tTachychardia is not always a cause of concern, as heart rate tends to spike after physical activity like exercise.\n\tSigns of tachycardia include a pounding heartbeat, chest pain, shortness of breath, etc.\n\tIf you experience symptoms like fainting and weakness, seek immediate medical help.\n");
		}
		break;
	default:
		fprintf(fptr,"-\tHigh pulse rate is often caused by high cholesterol levels and is a signal for heart diseases.\n\tIn case you experience symptoms of cardiovascular disease such as chest pain, short-breathedness and fatigue, etc. consult your doctor immediately.\n");
		break;
}
fprintf(fptr,"\nSOLUTIONS\n");
if (input[a].sugar==-1){
	fprintf(fptr,"\nTO INCREASE SUGAR LEVEL...\n-\tConsume foods and drinks with higher levels of sugar\n\tExamples of this include orange juice, cake soda,etc.\n-\tHave an extra snack after exercise\n-\tGet a glucagon shot\n-");
}
if (input[a].sugar>=1){
	fprintf(fptr,"\nTO DECREASE SUGAR LEVEL...\n-\tExercise regularly\n-\tDecrease your intake of foods that are rich in carbs like soft drinks, desserts, etc.\n-\tEat more fiber, i.e. vegetables and fruits\n-\tStay hydrated\n-\tManage your stress levels\n");
}
if (BP==-1){
	fprintf(fptr,"\nTO INCREASE BLOOD PRESSURE...\n-\tIncrease salt intake\n-\tStay hydrated\n-\tLimit high carbohydrate foods like potatoes and rice\n-\tExercise regularly\n");
}
if (BP>=1){
	fprintf(fptr,"\nTO DECREASE BLOOD PRESSURE...\n-\tMaintain a healthy weight\n-\tExercise regularly\n-\tLimit salt intake in your diet\n-\tQuit smoking\n-\tGet a good night\'s sleep\n");
}
if (input[a].pulse==-1){
	fprintf(fptr,"\nTO INCREASE PULSE RATE...\n-\tExercise regularly\n-\tMaintain a healthy weight\n-\tKeep blood pressure and cholesterol under control\n-\tAvoid smoking\n");
}
if (input[a].pulse>=1){
	fprintf(fptr,"\nTO DECREASE PULSE RATE...\n-\tExercise regularly\n-\tStay hydrated\n-\tManage your stress levels\n-\tLimit intake of stimulants such as adderall\n-\tSpend more time outdoors\n");
}
if (input[a].chol==-1){
	fprintf(fptr,"\nTO INCREASE CHOLESTEROL LEVEL...\n-\tEat food containing more fat\n-\tIn general, it is not advisable to increase your cholesterol levels.\n");
}
if (input[a].chol>=1){
	fprintf(fptr,"\nTO DECREASE CHOLESTEROL LEVEL...\n-\tFollow a heart healthy diet, e.g. the Mediterranean Diet\n-\tAvoid smoking\n-\tExercise regularly\n-\tMaintain a healthy weight.\n");
}

fclose(fptr);
} else {
	a--;
}
check=0;
}
return 0;
}
