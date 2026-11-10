//Progetto Mastermind Benigni e Signorelli 3^AII.
#include <iostream>
#include <algorithm>  //Per random_shuffle().
#include <stdlib.h>   
#include <time.h>
#include <conio.h>    //Per getche().
using namespace std;
const int T=8;        //Numero di tentativi totali.
const int C=4;        //Colonne della matrice dei tentativi e dimensione dei vari array.
const int N=6;        //Numeri da 1 a N che possono comparire nei tentativi e base del contachilometri.
const int R=N*N*N*N;  //Numero righe contachilometri (N alla C).

int trovaRighe();                         //Per trovare le righe della matrice contenente i tentativi della macchina.
void riempiSenzaRipetizioni(int comb[C]); //Per creare casualmente la combinazione che l'utente deve indovinare.
void input(int min, int max, int V[C]);   //Per il riempimento dei tentativi e della combinazione scelti dall'utente, con controlli.
void stampaLegenda(); 
void copiaRiga(int M[R][C], int ind);                                                 //Per copiare la riga precedente nel contachilometri.
void contaChilometri(int M[R][C]);                                                    //Per riempire una matrice come se fosse un contachilometri, dalla quale prendere le varie permutazioni (tentativi della macchina).
void riempiPermutazioni(int permutazione[][C], int contaChilometri[R][C]);            //Per riempire di permutazioni la matrice da cui prendere i tentativi.
bool tentativoMacchina(int mTent[][C], int righe, int tent[C], bool controllo[]);     //Il tentativo della macchina.
string creaRisposta(int comb[C], int tent[C]);                                        //Per ritornare la risposta al tentativo dell'utente confrontandolo con la combinazione.
void inputStr(string &risp);                                                          //Per far inserire all'utente la risposta al tentativo della macchina.
bool haVinto(string risp);                                                            //Controlla la risposta per verificare se l'utente o la macchina ha vinto.
void modifica(int mTent[][C], int tent[C], int righe, string risp, bool controllo[]); //Modifica la matrice dei tentativi della macchina in base alle risposte utente.
void stampa(int comb[C]);                                                             //Per mostrare all'utente la combinazione in caso perdesse.

int main()
{
	srand(time(0));       
	int combinazione[C];  //La combinazione da indovinare.
	int tentativo[C];     //Tentativo dell'utente o della macchina.
	int min=1,max=N;      //Range valori accettabili nei tentativi.
	int numTentativo;     //I tentativi rimanenti a ogni sbaglio.
	char risp;            
	string risposta;      //Risposta della macchina o dell'utente.
	
	do
    {
      cout<<"A quale versione del Mastermind vuoi giocare?"<<endl<<endl;
      cout<<".Premere 1 per giocare con il computer che fa da codificatore."<<endl;
      cout<<".Premere 2 per giocare con il computer che fa da decifratore."<<endl;
      do
	  {
	  	risp=getche();
	  	if(risp=='1' || risp=='2')    //Unici valori accettabili.
	  	  break;
	  	else
	  	  cout<<endl<<"Inserire solo 1/2: ";
	  }while(1);
	  system("cls");
	  numTentativo=T;   //I tentativi rimanenti a ogni sbaglio.
	  switch(risp)
	  {
	  	  case '1':
	  	  {  
	  	    //Prima versione del gioco.
   	    	riempiSenzaRipetizioni(combinazione);
	  		stampaLegenda(); 	
  	  		while(numTentativo>0)          //Finché non esaurisce i tentativi.
	  		{
				cout<<numTentativo<<"                            ";
	      		input(min,max,tentativo);  //Inserimento tentativo utente.
	    		cout<<"                ";
	    		risposta=creaRisposta(combinazione,tentativo);  //Per dare la risposta all'utente.
				cout<<risposta;
	    		if(haVinto(risposta))  
	    		{
	    			cout<<endl<<endl<<"Complimenti,hai vinto!";
	    			break;
				}
        		cout<<endl;
				numTentativo--;
	  		}  
	  		if(!haVinto(risposta))  //Se finiti i tentativi non ha vinto.
	  		{
	  	 		cout<<endl;
	  	 		cout<<"Mi spiace, hai perso!"<<endl;
		    	cout<<"la combinazione era: ";
		    	stampa(combinazione);
	  		}
			break;	  
	      }
	      case '2':
	      {
	      	//Seconda versione del gioco.
	      	int matContachilometri[R][C];             //La matrice contachilometri.
	        int righe=trovaRighe();                   //Righe matrice dei tentativi della macchina.
	        int tentMacchina[righe][C];               //Matrice con i tentativi della macchina.
	        bool controlloTent[righe];                //Array di controllo per verificare se un tentativo può essere usato dalla macchina.
			for(int i=0;i<righe;i++)                  //Lo inizializzo tutto a true.
	          controlloTent[i]=true;
			contaChilometri(matContachilometri);      //Per formare la matrice contachilometri dalla quale trovare le permutazioni.
	    	riempiPermutazioni(tentMacchina,matContachilometri);  //Per riempire la matrice dei tentativi della macchina.
	  	    cout<<"Scegli la combinazione che la macchina deve indovinare: ";
	    	input(min,max,combinazione);   //Riempimento combinazione dall'utente con controlli.
	    	cout<<endl<<endl;
	  		stampaLegenda(); 	
  	  		while(numTentativo>0)         //Finché non esaurisce i tentativi.
	  		{
				cout<<numTentativo<<"                            ";
                if(!tentativoMacchina(tentMacchina,righe,tentativo,controlloTent))  //Manda a video il tentativo della macchina, in caso esso non ci sia perché la risposta dell'utente è sbagliata, esco dal ciclo.
                {
                	cout<<"...."<<endl<<endl<<"Mi spiace ma non riesco a trovare un tentativo adatto alle tue risposta."<<endl;
                	break;
				}
	    		cout<<"                ";
	      		inputStr(risposta);  //L'utente dà la risposta alla macchina.
	    		if(haVinto(risposta))
	    		{
	    			cout<<endl<<endl<<"Wow,a quanto pare ho vinto!";
	    			break;
				}
				else
				  modifica(tentMacchina,tentativo,righe,risposta,controlloTent);
        		cout<<endl;
				numTentativo--;
	  		}
	  		if(!haVinto(risposta))
		   		cout<<"Cavolo, ho perso!";  
		   	cout<<endl;
			break;
		  }
	  }
	  cout<<endl<<"Vuoi rigiocare? <s/n>: ";
	  do
	  {
	  	risp=getche();
	  	if(risp=='s' || risp=='n') //Unici valori accettabili.
	  	 	break;
	  	else
	  		cout<<endl<<"Inserire solo s/n: ";
	  }while(1);
	  system("cls");
	}while(risp=='s');
}

int trovaRighe()  //Per sapere le righe della matrice dei tentativi della macchina.
{
	int num1=1;
	int num2=1;
	for(int i=N;i>0;i--)    //Fattoriale di N.
	  num1*=i;
	for(int i=N-C;i>0;i--)  //Fattoriale di N-C
	  num2*=i;
	return num1/num2;     
}

void riempiSenzaRipetizioni(int comb[C])  //Riempie senza ripetizioni la combinazione da indovinare.
{
	int V[N];
	for(int i=0;i<N;i++)    //Riempio un'array da 1 a N.
      V[i]=i+1;
      
    random_shuffle(V,V+N);  //Faccio lo shuffle dell'array.
	for(int i=0;i<C;i++)    //Inserisco le prime "C" cifre nella combinazione.
	  comb[i]=V[i];
}

void input(int min,int max,int V[C])   //Funzione per il controllo dei caratteri accettabili nel tentativo.
{                                      
	char x;
	int n;
	int cont=0;
	bool isDoppia;  //Per verificare che non inserisca due cifre uguali.
	do
	{
		isDoppia=false;
		x=getch();
		n=x-'0';         //Converto in cifra.
		for(int i=0;i<cont;i++)
		  if(n==V[i])    //Controllo il tentativo e verifico se quella cifra era già stata inserita.
		  {
		  	isDoppia=true;
		  	break;
		  }
		if(n>=min && n<=max && !isDoppia)
	    {
	    	V[cont]=n;
			cout<<V[cont]; 
	    	cont++; 
		}
	}while(cont<C);
}

void stampaLegenda()  //Stampa la legenda.
{
	cout<<"     Master Mind | La Mente Superiore"<<endl<<endl;
	cout<<"                          | LEGENDA |                                             | REGOLE  |"<<endl;
	cout<<"              X: numero giusto al posto giusto              .Inserire numeri tutti diversi nei tentativi"<<endl;
	cout<<"              O: numero  giusto al posto sbagliato          .I numeri utilizzabili vanno da 1 a "<<N<<endl<<endl;
	cout<<endl<<"N Tentativi rimanenti:       TENTATIVO:          Risposta:"<<endl<<endl;
}

void copiaRiga(int M[R][C],int ind)  //Copia la riga precedente nella matrice contachilometri.
{
	for(int i=0;i<C;i++)
	  M[ind][i]=M[ind-1][i];
}

void contaChilometri(int M[R][C])   //Riempie una matrice come se fosse un contachilometri che servirà per prendere le varie permutazioni da mettere nei tentativi della macchina.
{
	int ind;              //Indice che si sposta in base alla posizione da aumentare.
	bool haCambiato;      //Per verificare se è avvenuto un cambio di posizione (es. da unità a decine).
	for(int i=0;i<R;i++)  //Primissima riga inizializzata a 1.
	  M[0][i]=1;
	
	for(int i=1;i<R-1;i++)
	{
		haCambiato=false;
		ind=0;
		copiaRiga(M,i);          //Copio riga precedente.
		M[i][C-1]++;             //Aumento sempre la posizione dell'ultima colonna.
		while(M[i][C-1-ind]==N)  //Finché le ultime colonne sono uguali alla base.
		{
		   if(ind==0)  
		   {
		   	 i++;                //Per fare una nuova riga.
		     copiaRiga(M,i);
		   }
		   M[i][C-1-ind]=1;      //Inizializzo a 1 le posizioni che hanno raggiunto la base.
		   ind++;
		   haCambiato=true;
		}
		if(haCambiato)           //Aumento di 1 la posizione precedente a quelle che avevano già raggiunto la base.
		  M[i][C-1-ind]++;
	}
	for(int i=0;i<C;i++)         //L'ultimissima riga equivale a tutti "N".
	  M[R-1][i]=N;
}

void riempiPermutazioni(int permutazioni[][C],int contaChilometri[R][C])
{
	bool isPermutazione;
	int V[C];
	int cont=0;
	for(int i=0;i<R;i++)      //Controllo tutte le righe del contachilometri.
	{
		isPermutazione=true;
		for(int j=0;j<C;j++)  //Riempio l'array con la riga del contachilometri.
			V[j]=contaChilometri[i][j];
		  
		sort(V,V+C);          //Ordino array per rendere più facili i controlli che verificano se è una permutazione.
		for(int j=1;j<C;j++)
		  if(V[j]==V[j-1])    //Se tutte le cifre sono diverse.
		  {
		  	 isPermutazione=false;
		  	 break;
		  }
		if(isPermutazione)
		{
		  for(int j=0;j<C;j++)    //se è una permutazione la copio nella matrice dei tentativi.
		  	permutazioni[cont][j]=contaChilometri[i][j];
		  
		  cont++;
		} 
	}    
}

bool tentativoMacchina(int mTent[][C],int righe,int tent[C],bool controllo[]) //Il tentativo della macchina leggendo la risposta, ritorna false se non ci sono tentativi validi.
{
	int num;
	for(int i=0;i<righe;i++)
	{
		num=rand()%righe;	        //Prendo un numero a caso.
		if(controllo[num]==true)    //Se l'array di controllo in quella posizione è a true.
		{
		  for(int i=0;i<C;i++)      //Mando a video la relativa riga della matrice, riempiendo il tentativo della macchina con quella riga.
		  {
		  	cout<<mTent[num][i];
		    tent[i]=mTent[num][i];
		  }
		  return true;              //Ritorna true se la macchina ha trovato un tentativo valido.
		}	  
    }
    //Se non ha trovato neanche un possibile tentativo tra tutti ritorna false.
    return false;
}

string creaRisposta(int comb[C],int tent[C]) //Per ritornare la risposta al tentativo dell'utente.
{
    string risp;               
	for(int i=0;i<C;i++)
	  for(int j=0;j<C;j++)
	    if(comb[i]==tent[j] && i==j)  //Se il numero è presente nella combinazione e nella posizione giusta.
	      risp.push_back('X');
	    else
	      if(comb[i]==tent[j])        //Se il numero è presente nella combinazione ma non nella posizione giusta.
	        risp.push_back('O');
	        
	sort(risp.begin(),risp.end());  
	reverse(risp.begin(),risp.end());   //Per far comparire prima le 'X' delle 'O'.
	return risp;
}

void inputStr(string &risp) //Per far inserire all'utente la risposta al tentativo della macchina.
{
    char x;
    risp="";                //Svuoto ogni volta la stringa.
    int cont=0;
	do
	{
		x=getch();
		if(x==13 && cont>0)     //Se preme invio e ha scritto almeno una "X" o una "O".
		  break;
		
		if(x=='X' || x=='O')    //Unici valori accettabili.
	    {
	    	risp.push_back(x);
	    	cout<<x;            //Mando a video ciò che sta digitando.
	    	cont++;
		}
	}while(cont<C); 
	
	sort(risp.begin(),risp.end());  
	reverse(risp.begin(),risp.end());   //Per far comparire prima le 'X' delle 'O'.  
}

bool haVinto(string risp) //Controllando la risposta, per verificare se l'utente o la macchina ha vinto.
{
	for(int i=0;i<C;i++)
	  if(risp[i]!='X')    //Per vincere servono quattro "X".
	    return false;
	return true;
}

void modifica(int mTent[][C], int tent[C], int righe, string risp, bool controllo[]) //Per modificare la matrice dei tentativi della macchina in base alla risposta dell'utente.
{
	string tmp;
	for(int i=0;i<righe;i++)
	{
		tmp=creaRisposta(tent,mTent[i]);   //Confronto la risposta tra il tentativo casuale della macchina e tutte le permutazioni della matrice dei tentativi della macchina.
		if(tmp!=risp)                      //Se le loro risposte sono diverse metto a false l'array di controllo nella posizione relativa al tentativo inutile.
           controllo[i]=false;
	} 
}

void stampa(int comb[C]) //Per mostrare all'utente la combinazione in caso perdesse.
{
	for(int i=0;i<C;i++)
	    cout<<comb[i];
}
