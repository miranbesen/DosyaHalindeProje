#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MAX_HEIGHT 1000
#define INFINITY (1<<20) 

//Ad Ve Soyad: Miran Besen || ��renci No: 190401080

struct dugum {
    int icerik;
    struct dugum *sol;
    struct dugum *sag;
};

struct dugum* dugum_olustur(int icerik){
	
	struct dugum *d = (struct dugum*)malloc(sizeof(struct dugum));
	if(d==NULL){
		printf(" Bellekte yer ayrilamad� otomatik cikis yapiliyor...\n ");
		exit(1);
	}

	d->icerik=icerik; 
	d->sol=NULL;
	d->sag=NULL;
	
	return d;

}
//----------------------------------------------------------------Silme Kismi-------------------------------------------------------------------------------------
struct dugum *deleteNode(struct dugum *root, int key){
	
	if(root==NULL){
		printf("Silinecek eleman kalmamistir veya agamicimizda boyle bir eleman yoktur ...\n");
		return root;
	}
	
	if(key> root->icerik){ //eger silmek istedigimiz eleman o anki d�g�m�m�zden b�y�k ise saga devam ediyoruz.
		root->sag=deleteNode(root->sag,key);
	}
	else if(key<root->icerik){ //eger silmek istedigimiz eleman o anki d�g�m�m�zden kucuk ise saga devam ediyoruz.
		root->sol=deleteNode(root->sol,key);
	}
	else{ //Elemana eristikten sonra kontrollerimizi yap�yoruz.
		if(root->sol==NULL && root->sag==NULL){ // sol ve sag�nda eleman yok ise dugumun kendisini c�kart�yoruz.
			free(root);
			return NULL;
		}
		
		if(root->sol==NULL){ //solu bos ise
			struct dugum *gecici=root->sag;
			free(root);
			return gecici;
			
		}
		else if(root->sag==NULL){ //sag taraf� bos ise 
			struct dugum *gecici=root->sol;
			free(root);
			return gecici;
		
		}
		else{ //Eger sag ve solunda child'� var ise  bu parentin yerine gecicek degerin agacin duzenini bozmamasi gerekir, bunu da saglamak icin sag�n�n en solundaki elemana gidiyoruz.
		
		struct dugum *kokSagTaraf=root->sag;
		struct dugum *gecici=kokSagTaraf; 
		
		while (gecici && gecici->sol != NULL)// sag�n�n en solu yerine gecicek. 
    {
		gecici = gecici->sol; 
  	}
		root->icerik= gecici->icerik;
		root->sag=deleteNode(root->sag,gecici->icerik);		
	
	}
	}
	
	return root;	
	
}

//--------------------------------------------------------------Ekleme Kismi------------------------------------------------------------------
struct dugum *insert(struct dugum *root, int key) 
{ 
    //keyi i�eren d���m�m�z� olu�turduk.
    struct dugum* newnode = dugum_olustur(key); 
  
    struct dugum* x = root; 
  
  
    struct dugum* y = NULL; 
  
    while (x != NULL) { //�imdi eklememiz gereken  yeri buluyoruz.
        y = x; 
        if (key < x->icerik) //Eger key kucuk ise x imizi sol tarafa dogru agactan hareket ettiriyoruz. 
            x = x->sol; 
        else
            x = x->sag;  //Eger keyimiz buyuk ise sag tarafa hareket ediyoruz.
    } 
  
   
    if (y == NULL)  // root eger null olursa x' imiz while a girmez ve direk k�k�m�z olu�turdugumuz d�g�m olur.
        y = newnode; 
  
    //eger keyimiz ulastigimiz d�g�mden k�c�k ise sola yerlesiyor. 
    else if (key < y->icerik)  
        y->sol = newnode; 
  
    // e�er keyimiz ulast�g�m�z d�g�mden buyuk ise sag�na yerlesiyor.
    else
        y->sag = newnode; 
  
    //yeni d�g�m�n eklendigi i�aretci d�n�yor.
    return y; 
} 
  

//--------------------------------------------------------------Listeleme Kismi---------------------------------------------------------------

typedef struct asciinode_struct asciinode;


int gap = 3; //sol ve sag dugum aras� bosluk ayarlama.

int print_next; // bir sonraki olan dugumu ayn� duzeyde yazd�rmak i�in kullan�l�r. (Bu yazd�r�lan bir sonraki karakterin x koordinat�)

int MIN(int X, int Y) { // iki tane integer degeri k�yaslay�p minimumu yolluyor.
    return ((X) < (Y)) ? (X) : (Y);
}

int MAX(int X, int Y) { // iki tane integer degeri k�yaslay�p maksimumu yolluyor.
    return ((X) > (Y)) ? (X) : (Y);
}


struct asciinode_struct {
     asciinode *left, *right; // dugum struct tan�mlamas� ile  kar�smamas� i�in b�yle yazd�m a��s�ndan yap�ld� 

    
    int edge_length; // cocuklar�na olan uzunluklar�na bak�yor.

    int height; //y�kseklik

    int lablen; // buda d���m�n basamak say�s�.

    //-1=I am left, 0=I am root, 1=right
    int parent_dir; // -1 ise sol cocuk, 0 ise k�k, 1 ise sa� �ocuk.

    char label[11]; //maksimum 10 basamak alabiliyorum.
};

asciinode *build_ascii_tree_recursive(struct dugum *t) {
    asciinode *node;

    if (t == NULL) return NULL;
	
    node = (asciinode*)malloc(sizeof(asciinode));
    node->left = build_ascii_tree_recursive(t->sol);
    node->right = build_ascii_tree_recursive(t->sag);

    if (node->left != NULL) { // bu �ekilde �ocuklar�n solda oldu�unu belirledik. E�er childi solda ise :
        node->left->parent_dir = -1; // sol cocugun parent_dir=-1 yaparak solda oldugunu belirtiyor.
    }

    if (node->right != NULL) 
	{ 
		// sag e�er bos degilse sag cocugu 1 yap�yor.
        node->right->parent_dir = 1;
    }

    sprintf(node->label, "%d", t->icerik); //sprintf kullan�m node->label olan k�sma t->icerigi yaz.
    
	node->lablen = (int) strlen(node->label); // bu da d���m�n ka� basamakl� oldu�unu g�steriyor.

    return node;
}

// A�ac� ascii d���m� yap�s�na kopyalay�n
asciinode *build_ascii_tree(struct dugum *t) {
    asciinode *node;
    if (t == NULL) return NULL;
    node = build_ascii_tree_recursive(t);
    node->parent_dir = 0; // parent olanlara 0 veriyor. 
    return node;
}


//Verilen a�ac�n t�m d���mlerini serbest b�rak
void free_ascii_tree(asciinode *node) {
    if (node == NULL) return;
    free_ascii_tree(node->left);
    free_ascii_tree(node->right);
    free(node);
}


//A�a��daki i�lev, verilen a�a� i�in lprofile dizisini doldurur.
//Bu a�ac�n k�k� etiketinin merkezinin bir konumda (x, y) bulundu�u varsay�lmaktad�r.
//Bu a�a� i�in kenar uzunlu�u alanlar�n�n hesapland���n� varsayar.
int lprofile[MAX_HEIGHT];
int rprofile[MAX_HEIGHT];
// maksimum y�kseklik 1000 verilmis.

void compute_lprofile(asciinode *node, int x, int y) {  // sol k�s�m hesaplamak i�in yaz�lan fonksiyon
    int i, isleft;
    if (node == NULL) return;
    isleft = (node->parent_dir == -1); 
    lprofile[y] = MIN(lprofile[y], x - ((node->lablen - isleft) / 2));
    if (node->left != NULL) {
        for (i = 1; i <= node->edge_length && y + i < MAX_HEIGHT; i++) {
            lprofile[y + i] = MIN(lprofile[y + i], x - i);
        }
    }
    compute_lprofile(node->left, x - node->edge_length - 1, y + node->edge_length + 1);
    compute_lprofile(node->right, x + node->edge_length + 1, y + node->edge_length + 1);
}

void compute_rprofile(asciinode *node, int x, int y) {
    int i, notleft;
    if (node == NULL) return;
    notleft = (node->parent_dir != -1);
    rprofile[y] = MAX(rprofile[y], x + ((node->lablen - notleft) / 2));
    if (node->right != NULL) {
        for (i = 1; i <= node->edge_length && y + i < MAX_HEIGHT; i++) {
            rprofile[y + i] = MAX(rprofile[y + i], x + i);
        }
    }
    compute_rprofile(node->left, x - node->edge_length - 1, y + node->edge_length + 1);
    compute_rprofile(node->right, x + node->edge_length + 1, y + node->edge_length + 1);
}


// Bu i�lev, belirtilen a�ac�n kenar uzunlu�u ve y�kseklik alanlar�n� doldurur. 
//compute_edge_lengths:kenar uzunluklar�n� hesapla
void compute_edge_lengths(asciinode *node) {
    int h, hmin, i, delta;
    if (node == NULL) return;
    compute_edge_lengths(node->left);
    compute_edge_lengths(node->right);

    
    // �nce d���m�n kenar uzunlu�unu doldurun
    if (node->right == NULL && node->left == NULL) 
	{ //edge_length=cocuklar�na olan uzunluklar�na bak�yor.
        node->edge_length = 0; //cocugu yoksa 0' de
    } 
	
	else{
        if (node->left != NULL) 
		{
            for (i = 0; i < node->left->height && i < MAX_HEIGHT; i++) {
                rprofile[i] = -INFINITY;
            }
            compute_rprofile(node->left, 0, 0);
            hmin = node->left->height;
        } 
		else 
		{
            hmin = 0;
        }
        if (node->right != NULL) 
		{
            for (i = 0; i < node->right->height && i < MAX_HEIGHT; i++) {
                lprofile[i] = INFINITY;
            }
            compute_lprofile(node->right, 0, 0);
            hmin = MIN(node->right->height, hmin);
        } 
		else 
		{
            hmin = 0;
        }
        delta = 4;
        for (i = 0; i < hmin; i++) {
            delta = MAX(delta, gap + 1 + rprofile[i] - lprofile[i]);
        }

      
        // d���m�n y�ksekli�i 1 olan 2 �ocu�u var ise  
        if (((node->left != NULL && node->left->height == 1) ||
             (node->right != NULL && node->right->height == 1)) && delta > 4) {
            delta--;
        }

        node->edge_length = ((delta + 1) / 2) - 1;
    }

    //D���m�n y�ksekli�ini doldururuz.
    h = 1;
    if (node->left != NULL) {
        h = MAX(node->left->height + node->edge_length + 1, h);
    }
    if (node->right != NULL) {
        h = MAX(node->right->height + node->edge_length + 1, h);
    }
    node->height = h;
}

//i�levi, d���m�n verilen x kordonine sahip oldu�unu varsayarak, verilen a�ac�n verilen seviyesini yazd�r�r.
void print_level(asciinode *node, int x, int level) {
    int i, isleft;
    if (node == NULL) return;
    isleft = (node->parent_dir == -1);
    if (level == 0) {
        for (i = 0; i < (x - print_next - ((node->lablen - isleft) / 2)); i++) {
            printf(" ");
        }
        print_next += i;
        printf("%s", node->label);
        print_next += node->lablen;
    } else if (node->edge_length >= level) {
        if (node->left != NULL) {
            for (i = 0; i < (x - print_next - (level)); i++) {
                printf(" ");
            }
            print_next += i;
            printf("/");
            print_next++;
        }
        if (node->right != NULL) {
            for (i = 0; i < (x - print_next + (level)); i++) {
                printf(" ");
            }
            print_next += i;
            printf("\\");
            print_next++;
        }
    } else {
        print_level(node->left,
                    x - node->edge_length - 1,
                    level - node->edge_length - 1);
        print_level(node->right,
                    x + node->edge_length + 1,
                    level - node->edge_length - 1);
    }
}

//Verilen a�a� yap�s� i�in ascii a�ac�n� yazd�r�r.
void print_ascii_tree(struct dugum *t) {
    asciinode *proot; // bu sekilde cizilecek olan agac i�in struct yap�s� tan�ml�yoruz.
    int xmin, i;
    if (t == NULL) return;
    proot = build_ascii_tree(t);
    compute_edge_lengths(proot);
    for (i = 0; i < proot->height && i < MAX_HEIGHT; i++) {
        lprofile[i] = INFINITY;
    }
    compute_lprofile(proot, 0, 0);
    xmin = 0;
    for (i = 0; i < proot->height && i < MAX_HEIGHT; i++) {
        xmin = MIN(xmin, lprofile[i]);
    }
    for (i = 0; i < proot->height; i++) {
        print_next = 0;
        print_level(proot, -xmin, i);
        printf("\n");
    }
    if (proot->height >= MAX_HEIGHT) {
        printf("(Bu aga� eger %d'den daha uzun ise yanlis �izilme ihtimali vardir\n", MAX_HEIGHT);
    }
    free_ascii_tree(proot);
}

//---------------------------------------------------------------Listeleme Kismi Sonu----------------------------------------------------------

int main() 
{	srand(time(NULL));
	int dizi[10]={0}; // elimizde 10 elemanl� bir dizi var ve random elemanlar� bu dizimizde tutucaz.
	int randomIndex; //random eleman�m�z.
	int i;
	struct dugum *root=NULL;
	int silinecek=0;
	int bayrak=1;
	for(i=0;i<10;i++)
	{
		randomIndex=rand()%100+1; 
		dizi[i]+=randomIndex;	//random 10 elamani diziye ekledik ve bu diziden hareket ederek agac olusturacaz.	
	}	
	printf("random elemanlar:");
	printf("\n");
	for(i=0;i<10;i++)
	{
		printf("%d",dizi[i]);	
		printf("\t");	
	}
	
	root=insert(root,dizi[0]); //Random olusturdugumuz ilk elemani agacimizin k�k�ne ekledik, artik k�k sayesinde diger elemanlari da ekleme yapabiliriz k�ke g�re.
	
	for(i=1;i<10;i++)
	{	
		insert(root,dizi[i]);	//geri kalan 9 elemani ekleme yapt�k.		
	}
	printf("\n");
	printf("Agacimizin ilk hali:");
	printf("\n");
	print_ascii_tree(root);
	printf("\n");
	printf("silme kismi:\n");
	
	while(1)
	{
		
	
		if(bayrak==1)
		{
					
			printf("agactan cikarmak istediginiz sayiyi giriniz:\n");
			scanf("%d",&silinecek);	
			root=deleteNode(root,silinecek);
			printf("\n");
			printf("Agacimiz:\n");
			print_ascii_tree(root);
		}
		else
		{
			printf("cikiliyor...\n");
			break;
		}
			
		printf("Agacimizindan eleman cikarmaya devam etmek istiyorsaniz 1'e basiniz,yoksa 0'a basiniz: \n");
		scanf("%d",&bayrak);
	}
	return 0;
}

