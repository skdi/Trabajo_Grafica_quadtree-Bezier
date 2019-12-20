#include <bits/stdc++.h>
#include <iostream>
#include <fstream>  
#include <math.h>    
using namespace std;

class Punto{
    public:
        float x,y;
        Punto(float nx=0,float ny=0){
            x=nx;
            y=ny;
        }
        bool esIgual(Punto b){
            if(b.x==x && b.y==y){
                return true;
            }
            return false;
        }
        void print(){
            //cout<<" ("<<x<<","<<y<<") ";
            /*ofstream file; 
            file.open("anish2.txt",std::ios_base::app | std::ios_base::out);
            file<<x<<","<<y<< endl;
            file.close();*/
        }
};

vector<pair<Punto,Punto> >lineas;
vector<Punto> puntos;
vector<Punto> busqueda;
bool contiene(Punto LI,Punto LS,Punto pnt){
    if(LI.x<=pnt.x and LS.x>=pnt.x and LI.y<=pnt.y and LS.y>=pnt.y){
        return true;
    }
    return false;
}

float distEuclidiana(Punto a,Punto b){
    return sqrt(pow(a.x-b.x,2)+pow(a.y-b.y,2));
}

class Nodo{
    public:
        Punto LI,LS; //LI : topLeft  LS: bottonRight
        vector<Nodo*>hijos;
        vector<Punto>data;
        bool hoja;
        Nodo(Punto nLI,Punto nLS){ //0,0 100,100, 3;
            hijos.resize(4);
            LI=nLI;
            LS=nLS;
            hoja=true;
        } 
        bool pertenece(Punto pnt){
            return contiene(LI,LS,pnt);
        }

        Punto centro(){
            float xm=(LI.x+LS.x)/2;
            float ym=(LI.y+LS.y)/2;
            return Punto(xm,ym);
        }
        void dividir(int limEl){
            float xm=(LI.x+LS.x)/2;
            float ym=(LI.y+LS.y)/2;
            //
            lineas.push_back(make_pair(Punto(LI.x,ym),Punto(LS.x,ym)));
            lineas.push_back(make_pair(Punto(xm,LS.y),Punto(xm,LI.y)));
            //
            hijos[0]=new Nodo(Punto(LI.x,ym),Punto(xm,LS.y));
            hijos[1]=new Nodo(Punto(xm,ym),LS);
            hijos[2]=new Nodo(LI,Punto(xm,ym));
            hijos[3]=new Nodo(Punto(xm,LI.y),Punto(LS.x,ym));
            for(int t=0;t<data.size();t++){
                for(int x=0;x<4;x++){
                    if(hijos[x]->pertenece(data[t])){
                        hijos[x]->data.push_back(data[t]);
                        if(hijos[x]->data.size()>limEl){
                            hijos[x]->dividir(limEl);
                        }
                    }   
                }
            }
            hoja=false;
            data.resize(0);
            return;      
        }

        int posicion(Punto pnt){
            for(int x=0;x<data.size();x++){
                if(data[x].esIgual(pnt)){
                    return x;
                }
            }
            return -1;
        }
        void printLimites(){
            cout<<"LI("<<LI.x<<","<<LI.y<<") ";
            cout<<"LS("<<LS.x<<","<<LS.y<<")";
            cout<<endl;
        }

        void printData(){
            //cout<<"Data: ";
            for(int i=0;i<data.size();i++){
                data[i].print();
            }
            //cout<<endl;
        }

        void drawData(){
            for(int i=0;i<data.size();i++){
                puntos.push_back(data[i]);
            }
        }
};

class Quadtree{
    public:
        Nodo *root;
        int maxEl;
        Punto LI,LS;
        Quadtree(Punto nLI,Punto nLS,int nmaxEl){
            maxEl=nmaxEl;
            LI=nLI;
            LS=nLS;
            root=new Nodo(LI,LS);
        }
        void insert(Punto);
        int search(Punto,Nodo*&);
        void searchRadio(Nodo*,Punto,float);
        bool remove(Punto);
        void print();
        void printR(Nodo *);
        void generateTxt();
        void printRT(Nodo *);
        void draw();
        void drawR(Nodo *);
        void buscarCirculo(Punto centro,float radio);
};


void Quadtree::insert(Punto pnt){
    Nodo* obj;
    int posicion=search(pnt,obj);
    if(obj==NULL or posicion!=-1 or (pnt.x==-1 and pnt.y==-1)){
        return;
    }
    obj->data.push_back(pnt);
    puntos.push_back(pnt);

    if(obj->data.size()>maxEl){
        obj->dividir(maxEl);
    }
    return;
}

int Quadtree::search(Punto pnt,Nodo* &temp){
    temp=root;
    bool verif;
    while(!temp->hoja){
        verif=true;
        for(int x=0;x<temp->hijos.size();x++){
            if(temp->hijos[x]->pertenece(pnt)){
                temp=temp->hijos[x];
                verif=false;
                break;
            }
        }
        if(verif){
            
            return temp->posicion(pnt);
        }
        
    }
    return temp->posicion(pnt);
}

bool Quadtree::remove(Punto pnt){
    Nodo*obj;
    int posicion=search(pnt,obj);
    if(obj==NULL or posicion!=-1){
        return false;
    }
    obj->data.erase(obj->data.begin() + posicion);
    return true;
}

void Quadtree::printR(Nodo *p){
    if(p!=NULL){   
        //p->printLimites();
        p->printData();
        if(!p->hoja){
            for(int x=0;x<4;x++){
                //cout<<"Hijo "<<x+1<<endl;
                printR(p->hijos[x]);  
            }
        }
        else{
            ofstream file; 
      /*  file.open("anish.txt",std::ios_base::app | std::ios_base::out);
        file<<p->LI.x<<","<<p->LI.y
        <<","<<p->LS.x<<","<<p->LS.y<< endl;
        
        file.close();*/
        }
    
    }
}


void Quadtree::print(){
    Nodo* temp=root;
    printR(temp);             
}

void Quadtree::draw(){
    Nodo* temp=root;
    puntos.clear();
    drawR(temp);             
}

void Quadtree::drawR(Nodo *p){
    if(p!=NULL){   
        p->drawData();
        if(!p->hoja){
            for(int x=0;x<4;x++){
                //cout<<"Hijo "<<x+1<<endl;
                drawR(p->hijos[x]);  
            }
        }
    
    }
}

void Quadtree::buscarCirculo(Punto centro,float radio){
    busqueda.clear();
    //busqueda.push_back(centro);
    /*Nodo* obj;
    search(centro,obj);    
    if(obj!=NULL){
        for(int i=0;i<obj->data.size();i++){
            float dist=distEuclidiana(centro,obj->data[i]);
            if(dist<=radio){
                busqueda.push_back(obj->data[i]);
            }
            
        }
    }*/
    /*float calx,caly;
    for(double i=0;i<15;i=i+0.01){
			calx = centro.x + radio * cos(i);
			caly = centro.y + radio * sin(i);
            Punto pntTemp(calx,caly);
	        Nodo* obj;
            search(pntTemp,obj);
            searchRadio(obj,centro,radio);
    }*/


    Nodo *obj=root;
    searchRadio(obj,centro,radio);
}

void Quadtree::searchRadio(Nodo* temp,Punto centro,float radio){
    //temp=root;
    if(temp!=NULL){
        if(!temp->hoja){
            for(int x=0;x<temp->hijos.size();x++){
                //Punto pntCentro=temp->centro();
                //if(distEuclidiana(pntCentro,centro)<(radio*3))
                    searchRadio(temp->hijos[x],centro,radio);
                
            }
        }
        else{
            for(int i=0;i<temp->data.size();i++){
                float dist=distEuclidiana(centro,temp->data[i]);
                if(dist<=radio){
                    busqueda.push_back(temp->data[i]);
                }
            }
            
        }
    }
    return;
}
