#include<fstream.h>
#include<ctype.h>
#include<conio.h>
#include<stdio.h>
#include<iomanip.h>
#include<stdlib.h>
#include<string.h>
#define recsize 40
#define max 10
#define fn "File0.txt"
fstream file,file1;
char buffer[80];
int array[50];
int n=0;
class Order{
	public:
		char ordno[5],total[10];
		void pack(char *);
		void unpack();
};
void Order::pack(char* buffer1){
	 file1.open("file1.txt",ios::app);
	 file1<<buffer1<<endl;
}
void Order::unpack(){
	file1.getline(ordno,5,'|');
	file1.getline(total,10,'\n');
}
class Menu
{
	 public:
		  char pid[5],name[30],price[5];
		  void initial();
		  void read();
		  void store(int);
		  int remove(int,char *);
		  int hash(char *);
		  void displayall();
		  int retrieve(int addr,char k[]);
 };
void Menu::initial()
{
	int i,j;
	file.open(fn,ios::in);
	if(!file)
	{
		file.open(fn,ios::out);
		if(!file)
		{
			cout<<"Unable to open the file";
			getch();
			exit(1);
		}
		for(i=0;i<max;i++)
		{
			file.seekp(i*recsize*5+i*2,ios::beg);
			for(j=0;j<(recsize)*5;j++)
				file<<"#";
			file<<endl;
		}
		cout<<"Empty file created"<<endl;
	}
	else
	{
		char* dummy;
		for(int i=0;i<max;i++){
			for(int j=0;j<5;j++){
				file.seekg(i*recsize*5+j*recsize+i*2);
				file.getline(dummy,5,'\n');
				if(strcmp(dummy,"####")!=0){
					file.seekg(i*recsize*5+j*recsize+i*2);
					file.getline(pid,5,'|');
					file.getline(name,30,'|');
					file.getline(price,5,'|');
					array[n++]=atoi(pid);
				}
			}
		}
	}
	file.close();
	return;
 }
void Menu::read()
{
	cout<<"Enter the product ID:";	gets(pid);
       //	for(int i=0;i<5;i++){
    //	if(!isdigit(pid[i]))
       //	{
	//	cout<<"Enter a Valid Product ID!\n";
     //	}
      //	 else{
		label1:	for(int i=0;i<n;i++)
		{
			if(atoi(pid)==array[i])
			{
			cout<<"Invalid PID or PID value already exists !\nPlease enter a new PID:";
			gets(pid);
			goto label1;
			}
		}
	cout<<"Enter the name:";        gets(name);
	cout<<"Enter the price:";      	gets(price);
	strcpy(buffer,pid);             strcat(buffer,"|");
	strcat(buffer,name);            strcat(buffer,"|");
	strcat(buffer,price);           strcat(buffer,"|");
     //	}

	return;
 }
int Menu::hash(char* pid)
{
	int i=0,sum=0;
	while(pid[i]!='\0')
	{
		sum=sum+(pid[i]-48);
		i++;
	}
	return sum % max;
}
void Menu::store(int addr)
{
	char dummy[10];
	int flag=0,i,j;
	file.open(fn,ios::in|ios::out);
	file.seekp(addr*recsize*5+addr*2);
	file.getline(dummy,5,'\n');
	if(strcmp(dummy,"####")==0)
	{
		 file.seekp(addr*recsize*5+i*2);
		 file<<buffer;
		 flag=1;
	}
	else
	{
		for(i=0;i<5;i++)
		{
			file.seekg(addr*recsize*5+i*recsize+addr*2);
			file.getline(dummy,5,'\n');
			//cout<<"dummy "<<dummy<<addr*recsize*5+i*recsize+addr*2<<"\t";
			if(strcmp(dummy,"####")==0)
			{
				cout<<"\n Collision has occured\n";
				cout<<"Home address is "<<addr*recsize*5+addr*2<<" Actual address is "<<addr*recsize*5+i*recsize+i*2<<"\n";
				file.seekp(addr*recsize*5+i*recsize+2*addr);
				file<<buffer;
				flag=1;
				//cout<<"break now\n";
				return;
			}
		}
		for(i=addr+1;i!=addr;i++)
		{
			if(i%max==0)
				i=0;
			for(j=0;j<5;j++)
			{
				file.seekg(i*recsize*5+j*recsize+2*i);
				file.getline(dummy,5,'\n');
				if(strcmp(dummy,"####")==0)
				{
					cout<<"\n Collision has occured\n";
					cout<<"Home address is "<<addr*recsize*5+i*2<<" Actual address is "<<i*recsize*5+j*recsize+i*2<<"\n";
					file.seekp(i*recsize*5+j*recsize+2*i);
					file<<buffer;
					flag=1;
					return;
					// cout<<"Break now\n";
				}
			}
		}
	}
	if(i==addr && (!flag))
		cout<<"Hash file is full,record cannot be inserted\n";
	file.close();
	return;
}
int Menu::retrieve(int addr,char k[])
{
	int found=0,i;
	char dummy[10];
	i=addr;
	file.open(fn,ios::in|ios::out|ios::binary);
	do
	{
		file.seekg(i*recsize*5+i*2);
		file.getline(dummy,5,'\n');
		if(strcmp(dummy,"####")==0)
			break;
		file.seekg(i*recsize*5+i*2);
		file.getline(pid,5,'|');
	   //	cout<<pid<<endl;
		if(strcmp(pid,k)==0)
		{
			found=i*recsize*5+i*2;
			cout<<"Record found"<<endl;
			file.getline(name,30,'|');
			file.getline(price,5,'|');
			cout<<setiosflags(ios::left);
			for(int li=0;li<strlen(name);li++)
				name[li]=toupper(name[li]);
			cout<<"Pid = "<<pid<<endl<<"Name = "<<name<<endl<<"Price = "<<price<<endl;
			return found;
		}
		else
		{
			for(int j=0;j<5;j++){
				file.seekg(i*recsize*5+j*recsize+i*2);
				file.getline(dummy,5,'\n');
				//cout<<dummy<<" dummy "<<(i*recsize*5+j*recsize+i*2)<<"\t";
				if(strcmp(dummy,"####")==0)
					break;
				file.seekg(i*recsize*5+j*recsize+i*2);
				file.getline(pid,5,'|');
			  //	cout<<pid<<endl;
				if(strcmp(pid,k)==0)
				{
					found=i*recsize*5+j*recsize+i*2;
					cout<<"Record found"<<endl;
					file.getline(name,30,'|');
					file.getline(price,5,'|');
					cout<<setiosflags(ios::left);
					for(int li=0;li<strlen(name);li++)
						name[li]=toupper(name[li]);
					cout<<"Key = "<<pid<<endl<<"Name = "<<name<<endl<<"Price = "<<price<<endl;
					return found;
				}
			}
			i++;
			if(i%max==0)
					i=0;
		}
	} while(i!=addr);
	if(found==0)
		cout<<"\nRecord does not exist in hash file\n";
	return found;
}
int Menu::remove(int addr,char k[])
{
	int found=0,i;
	char dummy[10];
	i=addr;
	file.open(fn,ios::in|ios::out);
	do
	{
		file.seekg(i*recsize*5+i*2);
		file.getline(dummy,5,'\n');
		//cout<<dummy<<endl;
		if(strcmp(dummy,"####")==0)
			break;
		file.seekg(i*recsize*5+i*2);
		file.getline(pid,5,'|');
		//cout<<pid<<"   "<<k<<endl;
		if(strcmp(pid,k)==0)
		{
			found=1;
			cout<<"Record found"<<endl;
			file.getline(name,30,'|');
			file.getline(price,5,'|');
			cout<<setiosflags(ios::left);
			for(int li=0;li<strlen(name);li++)
				name[li]=toupper(name[li]);
			cout<<"Pid = "<<pid<<endl<<"Name = "<<name<<endl<<"Price = "<<price<<endl;
			file.seekp(i*recsize*5+i*2);
			for(int k=0;k<n;k++){
				if(array[k]==atoi(pid)){
				   while(k<n){
						array[k]=array[k+1];
						k++;
					}
				}
			}
			n--;
			for(int i=0;i<recsize;i++)
				file.put('#');
			return(found);
		}
		else
		{
			for(int j=0;j<5;j++){
				file.seekg(i*recsize*5+j*recsize+i*2);
				file.getline(dummy,5,'\n');
				if(strcmp(dummy,"####")==0)
					return(found);
				file.seekg(i*recsize*5+j*recsize+i*2);
				file.getline(pid,5,'|');
				if(strcmp(pid,k)==0)
				{
					found=1;
					cout<<"Record found"<<endl;
					file.getline(name,30,'|');
					file.getline(price,5,'|');
					cout<<setiosflags(ios::left);
					for(int li=0;li<strlen(name);li++)
						name[li]=toupper(name[li]);
					cout<<"Key = "<<pid<<endl<<"Name = "<<name<<endl<<"Price = "<<price<<endl;
					file.seekp(i*recsize*5+j*recsize+i*2);
					for(int k=0;k<n;k++){
						if(array[k]==atoi(pid)){
							while(k<n){
								array[k]=array[k+1];
								k++;
							}
						}
					}
					n--;
					for(int i=0;i<recsize;i++)
						file.put('#');
					return(found);
				}
			}
			i++;
			if(i%max==0)
				i=0;
		}
	} while(i!=addr);
	return(found);
}
void Menu::displayall(){
	file.open(fn,ios::in);
	char* dummy;
	cout<<setiosflags(ios::left);
	cout<<setw(5)<<"Pid"<<setw(30)<<"NAME"<<setw(5)<<"Price"<<endl;
	cout<<"----------------------------------------"<<endl;
	int l=0;
	for(int i=0;i<max;i++){
		for(int j=0;j<5;j++){
			file.seekg(i*recsize*5+j*recsize+i*2);
			file.getline(dummy,5,'\n');
			if(strcmp(dummy,"####")!=0){
				l++;
				if(l==11) {
					l=1;
					cout<<endl<<"Press anykey to display more:";
					getch();
					clrscr();
					cout<<endl<<endl<<endl<<setw(5)<<"Pid"<<setw(30)<<"NAME"<<setw(5)<<"Price"<<endl;
					cout<<"----------------------------------------"<<endl;
				}
				file.seekg(i*recsize*5+j*recsize+i*2);
				file.getline(pid,5,'|');
				file.getline(name,30,'|');
				file.getline(price,5,'|');
				cout<<setiosflags(ios::left);
				for(int li=0;li<strlen(name);li++)
					name[li]=toupper(name[li]);
				cout<<setw(5)<<pid<<setw(30)<<name<<setw(5)<<price<<endl;
			}
		}
	}
	cout<<endl;
	getch();
}
void main()
{
	int ch,addr,ch1,address;
	char skey[5];
	Menu m;
	Order o;
	clrscr();
	m.initial();
	cout<<"*************************************************************\n";
	cout<<"\t\t\tGROCERY BILLING SYSTEM\n";
	cout<<"*************************************************************\n";
	cout<<"\n ->Enter 1 for Admin Options \n\n ->Enter 2 for Billing"<<endl;

	cin>>ch1;
	if(ch1==1){
		for(;;)
		{       cout<<"\nADMIN OPTIONS:\n";
			cout<<endl<<"\t\t1.ADD PRODUCTS\n\t\t2.SEARCH PRODUCT\n\t\t3.REMOVE PRODUCT\n\t\t4.MODIFY PRODUCT DETAILS\n\t\t5.DISPLAY ALL\n\t\t6.TOTAL SALES\n\t\t7.EXIT\n";
			cin>>ch;
			int found;
			switch(ch)
			{
				case 1: //add product
					cout<<endl<<"Enter Product details:"<<endl;
					m.read();
					addr=m.hash(m.pid);
					m.store(addr);
					cout<<"\n  Product added Successfully..!  \n";
					cout<<"Press any key to continue..";
					getch();
					clrscr();
					break;
				case 2: //search
					cout<<"Enter Product ID :";
					cin>>skey;
					addr=m.hash(skey);
					address=m.retrieve(addr,skey);
					cout<<"Press any key to continue..";
					getch();
					clrscr();
					break;
				case 3:cout<<"Enter Product ID :";  //remove
					cin>>skey;
					addr=m.hash(skey);
					found=m.remove(addr,skey);
					if(found==0)
						cout<<"\n***Record does not exist in hash file..!!***\n";
					else
						cout<<"\nRecord Deleted sucessfully."<<endl;
					cout<<"\nPress any key to continue..";
					getch();
					clrscr();
					break;
				case 4:cout<<"Enter Product ID:";  //modify
					cin>>skey;
					addr=m.hash(skey);
					found=m.remove(addr,skey);
					file.close();
					cout<<found<<endl<<endl;
					if(found==0)
						cout<<"\nRecord does not exist in hash file"<<endl;
					else
					{
						cout<<endl<<"Enter modified Product details:"<<endl;
						m.read();
						addr=m.hash(m.pid);
						m.store(addr);
						cout<<"\n\nRecord updated sucessfully!"<<endl;
					}
					cout<<"Press any key to continue..";
					getch();
					clrscr();
					break;
				case 5:cout<<" List of Products:\n";  //display all
					m.displayall();
					clrscr();
					break;
				case 6:file1.open("file1.txt",ios::in);
					int pi=0,sales=0;
					while(!file1.eof()){
						if(pi==0){
							clrscr();
							cout<<endl<<endl<<endl<<"The sales are:"<<endl;
							cout<<setw(10)<<"Order No."<<setw(10)<<"Amount"<<endl;
						}
						o.unpack();
						if(strcmp(o.ordno,"")!=0){
							pi=(pi+1)%10;
							cout<<setw(10)<<o.ordno<<setw(10)<<o.total<<endl;
							sales+=atoi(o.total);
							if(pi==0){
								cout<<"Press any key to show more..";
								getch();
							}
						}
					}
					file1.close();
					cout<<endl<<"Total sales =Rs. "<<sales<<endl<<endl;
					cout<<"Press any key to continue..";
					getch();
					clrscr();
					break;
				default: cout<<"Press any key to exit:";
						getch();
						exit(0);
			}//switch
			file.close();
		}//for
	}//if
	else if(ch1==2)
	{

		int ord=0;
		file1.open("file1.txt",ios::in);
		while(!file1.eof()){
			o.unpack();
			if(strcmp(o.ordno,"")!=0)
				ord=atoi(o.ordno)+1;
		}
		file1.close();
		char oname[10][30];
		char op[5],buffer1[80];
		int ototal=0;
		int oprice[10],oquantity[10];
		int oi=0;
		for(;;)
		{
			cout<<"PRODUCT DETAILS :\n";
			m.displayall();
			file.close();
			//clrscr();
			cout<<"\n********************BILL GENERATION*******************\n";
			cout<<"\nEnter Product ID :";
			cin>>skey;
			addr=m.hash(skey);
			address=m.retrieve(addr,skey);
			file.close();
			if(address==0)
				continue;
			file.open(fn,ios::in);
			file.seekg(address);
			file.getline(skey,5,'|');
			file.getline(oname[oi],30,'|');
			file.getline(op,5,'|');
			file.close();
			oprice[oi]=atoi(op);
			cout<<"\nEnter the quantity :"<<endl;
			cin>>oquantity[oi];
			ototal+=oquantity[oi]*oprice[oi];
			oi++;
			cout<<"Enter 1 to checkout or 0 to bill more Products:";
			cin>>ch1;
			if(ch1!=1)
				continue;
			else{
				clrscr();
				cout<<"****************************************************************\n";
				cout<<"\t\t\t BILL DETAILS \t\t\t\n";
				cout<<"*****************************************************************\n";
				cout<<endl<<endl<<endl<<setw(6)<<"No."<<setw(30)<<"NAME"<<setw(10)<<"PRICE"<<setw(10)<<"QUANTITY"<<endl;
				cout<<"__________________________________________________________________\n";
				for(int ooi=0;ooi<oi;ooi++)
					cout<<setw(6)<<ooi+1<<setw(30)<<oname[ooi]<<setw(10)<<oprice[ooi]<<setw(10)<<oquantity[ooi]<<endl;
				cout<<endl<<endl;
				cout<<"The total amount is: Rs."<<ototal<<endl;
				itoa(ord,o.ordno,10);
				itoa(ototal,o.total,10);
				strcpy(buffer1,o.ordno);
				strcat(buffer1,"|");
				strcat(buffer1,o.total);
				o.pack(buffer1);
				file1.close();
				cout<<"Press any key to exit:";
				getch();
				exit(0);
			}
		}

	}
	else{
	cout<<"\n\nEnter Proper Choice...!\n";
	cout<<"Press any key to exit!\n";
	getch();}

}
