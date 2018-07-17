//---------------------------------
// Mohammed Hassnain
// cpsc 223c
// Oct. 11, 2015
// Project No.7
// Question 1.
// purpose: the purpose of this program is to copy a text file to a raf file in numerical order,be able to 
//display individual records from the raf file. Make deposits and withdrawals from the raf file. Remove 
//accounts from the raf file and then copy the contents over to the text file.
//-----------------------------------


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

struct PERSON
{
	int ID;
	char name[20];
	float balance;
};


void RAF(char *fname);
void CopyText2RAF(char* txtname, char* rafname);

void DisplayText(char FullText[]);
void DisplayRAF(char *fname);
void DisplayRecord(char* txtname, int ID);
/*******************************************************************/
void Deposit(char* txtname, char* rafname, int ID, float amount);
void Withdraw(char* txtname, char* rafname, int ID, float amount);
void AccountRemoval(char* rafname, int ID);
void CopyRAF2Text(char* rafname, char* txtname);
void DisplayTextModified(char *txtname, int ID);

int AuthenticateID(char* fname, int ID);

int main()
{
	char FullText[500];
	int choice, choiceID;
	float money;

	RAF("raf.txt");
	CopyText2RAF("data.txt", "raf.txt");

	printf("------------Bank of Americana--------------\n");
	printf("1. Display all records in the text file\n");
	printf("2. Display the RAF after transferring all records from the text file\n");
	printf("3. Display a record when ID is given\n");
	printf("4. Deposit in an account when ID is given\n");
	printf("5. Withdraw from an account when ID is given\n");
	printf("6. Close an account for a given ID\n");
	printf("7. Exit(Copy RAF into the original text file)");
	printf("\n\n");

	do
	{
		printf("\nPlease enter your option(1-7): ");
		scanf("%d", &choice);
		printf("\n");

		if (choice == 1)
		{
			printf("This is the content of the original text file\n\n");
			DisplayText(FullText);
		}
		else if (choice == 2)
		{
			printf("This is the content of the RAF file\n\n");
			DisplayRAF("raf.txt");
		}
		else if (choice == 3)
		{
			printf("Enter the ID to view the record: ");
			scanf("%d", &choiceID);
			printf("\n");
			DisplayRecord("data.txt", choiceID);
		}
		else if (choice == 4)
		{
			printf("Enter the ID for the account: ");
			scanf("%d", &choiceID);
			printf("\n");
			int auth = AuthenticateID("raf.txt", choiceID);
			if (auth == 0)
			{
				printf("Enter the deposit amount: ");
				scanf("%f", &money);
				printf("\n");
				Deposit("data.txt", "raf.txt", choiceID, money);
				DisplayRAF("raf.txt");
			}
		}
		else if (choice == 5)
		{
			printf("Enter the ID for the account: ");
			scanf("%d", &choiceID);
			printf("\n");
			printf("Enter the withdrawal amount: ");
			scanf("%f", &money);
			printf("\n");
			Withdraw("data.txt", "raf.txt", choiceID, money);
			DisplayRAF("raf.txt");
		}
		else if (choice == 6)
		{
			printf("Enter the ID for the account you want to remove: ");
			scanf("%d", &choiceID);
			printf("\n");
			AccountRemoval("raf.txt", choiceID);

			CopyRAF2Text("raf.txt", "data.txt");
			DisplayTextModified("data.txt", choiceID);
		}
		else if (choice == 7)
		{

			printf("This is the content of the original text file modified\n\n");
			CopyRAF2Text("raf.txt", "data.txt");
			DisplayTextModified("data.txt", choiceID);
		}

	} while (choice != 7);

	printf("\n\n");
	system("PAUSE");
	return 0;

}








void RAF(char *fname)
{
	FILE * f;
	struct PERSON p = { 0 , "blank\t", 0.0 };

	// OPEN THE RAF FILE - to write in binary format
	f = fopen(fname, "wb");

	//INITIALIZE THE FILE WITH RECORD p
	int i;
	for (i = 1; i <= 10; i++)
	{          // writing from &p-to-f
		fwrite((char *)&p, sizeof(p), 1, f);
	}

	fclose(f);
}


void CopyText2RAF(char* txtname, char* rafname)
{
	FILE * ftxt = fopen(txtname, "r");
	FILE * fraf = fopen(rafname, "rb+");

	//READ FROM TEXT FILE
	struct PERSON p;
	int ByteOfText;


	while (!feof(ftxt))
	{
		fscanf(ftxt, "%d %s %f", &p.ID, p.name, &p.balance);
		ByteOfText = ((p.ID / 10 - 1)*sizeof(p));
		fseek(fraf, ByteOfText, SEEK_SET);
		fwrite((char *)&p, sizeof(p), 1, fraf);

	}

	fclose(ftxt);
	fclose(fraf);
}

void DisplayText(char FullText[])
{
	FILE * fd;
	fd = fopen("data.txt", "r");
	int i;

	printf("ID \t NAME \t\t BALANCE \n");
	printf("---------------------------------------\n");
	for (i = 0; i < 7; i++)
	{
		fgets(FullText, 500, fd);
		printf("%s", FullText);
	}

	printf("\n\n--Done Printing--\n");

	//free(FullText);

	fclose(fd);

}

void DisplayRAF(char *fname)
{
	FILE * f = fopen(fname, "rb");
	struct PERSON p;

	int i;
	printf("ID \t NAME \t\t BALANCE \n");
	printf("---------------------------------------\n");

	for (i = 1; i <= 10; i++)
	{
		fread((char *)&p, sizeof(p), 1, f);
		printf("%d \t %s \t %.2f \n", p.ID, p.name, p.balance);
	}

	fclose(f);
}

void DisplayRecord(char* txtname, int ID)
{
	// opening text file
	FILE * ftxt = fopen(txtname, "r");
	struct PERSON p;

	printf("ID \t NAME \t\t BALANCE \n");
	printf("---------------------------------------\n");

	while (!feof(ftxt))
	{
		fscanf(ftxt, "%d %s %f", &p.ID, p.name, &p.balance);
		if (ID == p.ID)
		{
			printf("%d \t %s \t %.2f \n", p.ID, p.name, p.balance);
			break;
		}
	}
	fclose(ftxt);
}

void Deposit(char* txtname, char* rafname, int ID, float amount)
{

	struct PERSON p;
	FILE * fraf = fopen(rafname, "rb+");

	int ByteOfSet;


	ByteOfSet = ((ID / 10) - 1) * sizeof(p);
	fseek(fraf, ByteOfSet, SEEK_SET);

	fread((char *)&p, sizeof(p), 1, fraf);
	p.balance += amount;


	fseek(fraf, ByteOfSet, SEEK_SET);
	fwrite((char *)&p, sizeof(p), 1, fraf);



	fclose(fraf);

}

void Withdraw(char* txtname, char* rafname, int ID, float amount)
{

	struct PERSON p;
	FILE * fraf = fopen(rafname, "rb+");

	int ByteOfSet;

	printf("\n\nthis is the size of p: %ld \n\n", sizeof(p));

	ByteOfSet = ((ID / 10) - 1) * sizeof(p);
	fseek(fraf, ByteOfSet, SEEK_SET);

	fread((char *)&p, sizeof(p), 1, fraf);
	p.balance -= amount;


	fseek(fraf, ByteOfSet, SEEK_SET);
	fwrite((char *)&p, sizeof(p), 1, fraf);



	fclose(fraf);

}

void AccountRemoval(char* rafname, int ID)
{

	FILE * fraf = fopen(rafname, "rb+");

	struct PERSON p;
	int ByteOfSet;

	char del[20];
	strcpy(del, "blank\t");


	ByteOfSet = ((ID / 10) - 1) * sizeof(p);
	fseek(fraf, ByteOfSet, SEEK_SET);
	fread((char *)&p, sizeof(p), 1, fraf);


	p.ID = 0;
	strcpy(p.name, del);
	p.balance = 0.00;

	fseek(fraf, ByteOfSet, SEEK_SET);
	fwrite((char *)&p, sizeof(p), 1, fraf);

	fclose(fraf);
}




void CopyRAF2Text(char* rafname, char* txtname)
{

	FILE * fraf = fopen(rafname, "rb");
	FILE * ftxt = fopen(txtname, "w+");

	//READ FROM BINARY FILE
	struct PERSON p;
	int ByteOfBin;
	int i;


	printf("ID \t NAME \t\t BALANCE \n");
	printf("---------------------------------------\n");



	while (!feof(fraf))
	{
		fread((char *)&p, sizeof(p), 1, fraf);
		fprintf(ftxt, "%d \t %s \t %.2f \n", p.ID, p.name, p.balance);

	}


	fclose(fraf);
	fclose(ftxt);

}



void DisplayTextModified(char *txtname, int ID)
{
	FILE *fp1, *fp2;
	char ch;
	int delete_line;
	int temp = 1;

	fp1 = fopen(txtname, "r");
	ch = getc(fp1);


	//rewind
	rewind(fp1);

	//open new file in write mode
	fp2 = fopen("replica.txt", "w");

	int linecount;

	while (ch != EOF)
	{
		ch = getc(fp1);
		if (ch == '\n')
		{
			temp++;
		}
		if ((temp != 7) && (temp != 8) && (!(temp >= 10)))
		{
			putc(ch, fp2);
		}
	}

	fclose(fp1);
	fclose(fp2);
	remove(txtname);

	//rename the file replica.txt to original name
	rename("replica.txt", txtname);

	// opening the renamed file to print
	fp1 = fopen(txtname, "r");
	ch = getc(fp1);

	while (ch != EOF)
	{
		printf("%c", ch);
		ch = getc(fp1);
	}

	fclose(fp1);

}


int AuthenticateID(char* fname, int ID)
{
	FILE * f = fopen(fname, "rb");
	struct PERSON p;

	int i;

	printf("---------------------------------------\n");

	for (i = 1; i <= 10; i++)
	{
		fread((char *)&p, sizeof(p), 1, f);

		if (p.ID == ID)
		{
			printf("ID FOUND");
			printf("\n---------------------------------------\n");
			fclose(f);
			return 0;
		}

	}
	printf("ID NOT FOUND");
	printf("\n---------------------------------------\n");
	fclose(f);


	return 1;
}
