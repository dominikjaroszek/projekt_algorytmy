#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "allegro5/allegro5.h"
#include "allegro5/allegro_font.h"
#include "allegro5/allegro_primitives.h"
#include <stdbool.h>

void losuj(int *tab, int N){
    srand(time(0));

    int H, L;
    printf("Podaj zakres losowania liczb: ");
    scanf("%d %d", &L, &H);



    if(L>H){
        int temp=L;
        L=H;
        H=temp;
    }

    for(int i=0;i<N;i++){
        tab[i]= L + rand()%(H-L+1);
    }

}

int import_data(int **tab){

        FILE *import;
        import=fopen("sort_data/import.txt","r");

            int N=0;
            int size_counter=100;
            int temp=-1;
            *tab = (int *)realloc(*tab,sizeof(int)*size_counter);
            if(import==NULL){
                puts("Brak pliku do importu");
                return -1;
            }else{
                while(!feof(import)){

                        if(N%100==0){
                            size_counter+=100;
                            *tab = (int *)realloc(*tab,sizeof(int)*size_counter);
                        }

                    fscanf(import,"%d",&temp);
                    (*tab)[N]=temp;
                    printf("%d ", (*tab)[N]);
                    N++;
                }

            }

        fclose(import);
        return N;
}

void save_data(char typ[], int size, int low, int high, float time, int operations){

    FILE *data;

    data=fopen("sort_data/data.txt", "a+");
        if(fgetc(data)==EOF){
            fprintf(data,"Typ;Ilosc elementow;Min;Max;Czas[s];Operacje\n");
        }else{
            fseek(data,0,SEEK_END);
        }

        fprintf(data,"%s;%d;%d;%d;%f;%lu\n",typ,size,low,high,time,operations);

        if(strcmp(typ,"Babelkowe")==0){
            fprintf(data,"\n");
        }


    fclose(data);
}

void save_result(FILE *f, int *tab, int N,char text[]){

    fprintf(f,"%s ",text);
    for(int i=0;i<N;i++){
        fprintf(f,"%d ",tab[i]);
    }
}

void draw_result(int tab[], int elements)
{
    /*al_init();
    //al_install_keyboard();
    al_init_primitives_addon();*/

   /* ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    ALLEGRO_DISPLAY* disp = al_create_display(960, 530);*/

    int size=960/elements;
    int j=0;
    al_clear_to_color(al_map_rgb(0,0,0));
    for(int i=0;i<elements;i++)
    {
        al_draw_filled_rectangle(j, 530, j+size, 530-tab[i], al_map_rgb(80,90,200));
        j+=size;
        al_flip_display();
    }
   // al_rest(0.05);
   /* al_destroy_display(disp);
    al_destroy_event_queue(queue);*/
}

void sort_bablekowe_draw(int *unsorted_tab,int N)
{
    al_init();
    al_init_primitives_addon();
    int *tab = (int *)malloc(sizeof(int)*N);

    for(int i=0;i<N;i++){
        tab[i]=unsorted_tab[i];
    }
    int temp=0;

    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    ALLEGRO_DISPLAY* disp = al_create_display(960, 530);
        for (int i = 0; i<N-1; i++)
        {
            for (int j=0; j<N-1-i; j++)
            {
                if (tab[j] > tab[j+1])
                {

                    temp = tab[j+1];
                    tab[j+1] = tab[j];
                    tab[j] = temp;
                }
            }
            draw_result(tab,N);
            al_rest(0.10);
        }
    al_rest(0.50);
    al_destroy_display(disp);
    al_destroy_event_queue(queue);
}

void sort_bablekowe(int *unsorted_tab,int N,FILE *plik){

    int *tab = (int *)malloc(sizeof(int)*N);

    for(int i=0;i<N;i++){
        tab[i]=unsorted_tab[i];
    }


    unsigned long int count=0;

    clock_t start, stop;
    int temp=0;
    printf("\nSortuje przy uzyciu algorytmu bubblesort");
    start=clock();
        for (int i = 0; i<N-1; i++){

            for (int j=0; j<N-1-i; j++)
            {
                if (tab[j] > tab[j+1])
                {
                    count++;
                    temp = tab[j+1];
                    tab[j+1] = tab[j];
                    tab[j] = temp;
                }
            }
            }
     stop=clock();
    float time_oper=((double)(stop - start))/CLK_TCK;


    save_result(plik,tab,N,"\nSortowana bablekowo:\n");
    save_data("Babelkowe",N,tab[0],tab[N-1],time_oper,count);

    printf("\nLiczba operacji zamiany - babelkowo: %lu\n",count);
    printf("Czas operacji zamiany - babelkowo: %f",time_oper);
}

void sort_quicksort_draw(int unsorted_tab[],int N)
{
    al_init();
    al_init_primitives_addon();
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    ALLEGRO_DISPLAY* disp = al_create_display(960, 530);
    int partition_draw(int T[], int l, int r)
    {
        int temp;
        int pivot = T[r];
        int i=l-1;

        for(int j=l;j<=r-1;j++){

            if(T[j]<pivot){
                i++;

                temp=T[i];
                T[i]=T[j];
                T[j]=temp;
            }
            draw_result(T,N);
            al_rest(0.10);
        }
        temp=T[i+1];
        T[i+1]=T[r];
        T[r]=temp;
        return i+1;
    }


    void quicksort_draw(int T[], int l,int r)
    {
        int q;

        if( l<r )
        {
            q=partition_draw(T,l,r);
            quicksort_draw(T,l,q-1);
            quicksort_draw(T,q+1,r);
        }
    }


    int *tab = (int *)malloc(sizeof(int)*N);
    for(int i=0;i<N;i++){
        tab[i]=unsorted_tab[i];
    }
    unsigned long int count=0;
    quicksort_draw(tab,0,N-1);
    al_rest(0.5);
    al_destroy_display(disp);
    al_destroy_event_queue(queue);
}



void sort_quicksort(int *unsorted_tab,int N,FILE *plik){

    int partition(int T[], int l, int r, int *count){
        int temp;
        int pivot = T[r];
        int i=l-1;

        for(int j=l;j<=r-1;j++){

            if(T[j]<pivot){
                i++;

                temp=T[i];
                T[i]=T[j];
                T[j]=temp;
                (*count)++;
            }

        }
        temp=T[i+1];
        T[i+1]=T[r];
        T[r]=temp;
        (*count)++;
        return i+1;
    }


    void quicksort(int T[], int l,int r, int *count){
    int q;
    if( l<r ){
        q=partition(T,l,r,count);
        quicksort(T,l,q-1,count);
        quicksort(T,q+1,r,count);
        }
    }


    int *tab = (int *)malloc(sizeof(int)*N);
    for(int i=0;i<N;i++){
        tab[i]=unsorted_tab[i];
    }

    unsigned long int count=0;
    clock_t start, stop;
    printf("\nSortuje przy uzyciu algorytmu quicksort");
    start=clock();
        quicksort(tab,0,N-1,&count);
    stop=clock();
    float time_oper=((double)(stop - start))/CLK_TCK;

    for(int i=0;i<N;i++){
       // printf("%d ", tab[i]);
    }
    save_result(plik,tab,N,"\nSortowana quicksort:\n");
    save_data("Quicksort",N,tab[0],tab[N-1],time_oper,count);

    printf("\nLiczba operacji zamiany - quicksort: %lu\n",count);
    printf("Czas operacji zamiany - quicksort: %f",time_oper);

}

int main()
{
    int elements = -1;

    int choice = 0;
    int *unsorted_tab = (int *)malloc(sizeof(int)*1);
    printf("1. Losuj tablice \n2.Importuj tablice z pliku \n3.Sortuj babelkowo 100 elementow z graficznym przedstawieniem procesu sortowania \n4.Sortuj przy uzyciu algorytmu quicksort 100 elementow z graficznym przedstawieniem procesu sortowania \nTwoj wybor: ");
    scanf("%d", &choice);
    if(choice==1){
        printf("Ilosc liczb do wylosowania: ");
        scanf("%d", &elements);
        unsorted_tab = realloc(unsorted_tab,sizeof(int)*elements);
        losuj(unsorted_tab,elements);
    }else if(choice==2){
        elements=import_data(&unsorted_tab);
    }
    else if(choice==3){
        unsorted_tab = realloc(unsorted_tab,sizeof(int)*100);
        printf("Zalecane podanie liczb w granicach od 0 do okolo 400 aby wynikprezentacji graficznej byl dobrze widoczny\n");
        losuj(unsorted_tab,100);
        sort_bablekowe_draw(unsorted_tab,100);
        return 0;
    }else if(choice==4){
        unsorted_tab = realloc(unsorted_tab,sizeof(int)*100);
        printf("Zalecane podanie liczb w granicach od 0 do 400 aby wynik prezentacji graficznej byl dobrze widoczny\n");
        losuj(unsorted_tab,100);
        sort_quicksort_draw(unsorted_tab,100);
        return 0;
    }else{
        puts("Blad wyboru");
        exit(0);
    }

    if(elements>0)
    {
        FILE *plik;
        plik=fopen("sort_data/tablica.txt","w+");
        save_result(plik,unsorted_tab,elements,"Wylosowana/importowana tablica:\n");


        sort_quicksort(unsorted_tab,elements, plik);
        sort_bablekowe(unsorted_tab,elements, plik);

        free(unsorted_tab);
        fclose(plik);
    }
    else
        printf("\n\t\t##ERROR##\n");
    return 0;
}
