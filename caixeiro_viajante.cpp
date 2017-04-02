#include <iostream>
#include <cstdio>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <algorithm>
#include <tr1/random>
#include <cstring>

using namespace std;

struct caso{
  int vertice;
  float x;
  float y;
};

struct individuo{
  vector<int> codigo;   // Estrutura que armazena um vetor de todos os vértices pegos como entrada, e calcula o fitness.
  double fitness;

  bool operator < (const individuo& i) const {
		return (fitness < i.fitness);
	}
};

vector<caso> grafo;

float distancia_rel(caso pega_dados1, caso pega_dados2){
  return sqrt(pow(pega_dados1.x - pega_dados2.x, 2) + pow(pega_dados1.y - pega_dados2.y, 2)); //Calcula a distância relativa do vértice de um pai até o vértice de outro pai
}

//distância absoluta de um vértice até o outro
float distancia_abs(vector<int> v, int n_vertice){
  double custo=0;

  for(int i=0;i<n_vertice;i++){
    if(i == n_vertice-1){
      custo+= distancia_rel(grafo.at(v.at(i)-1), grafo.at(v.at(0)-1));
    }
    else{
      custo+= distancia_rel(grafo.at(v.at(i)-1), grafo.at(v.at(i+1)-1));
    }
  }
  return custo;
}

individuo ind1, ind2, ind3;

void eliminaverticesrepetidos(individuo &filho, vector<int> &vet_zero){
  int i, cont=0;
  vector<int> naorepetidos;



  cout << endl << endl;



  cout << endl << endl;

  for(i=0;i<filho.codigo.size();i++){
    if(vet_zero[filho.codigo[i]]>0){
      vet_zero[filho.codigo[i]]++;
      filho.codigo[i] = -1;
    }
    else{
      vet_zero[filho.codigo[i]]++;
    }
  }


  cout << endl << endl;

  for(i=1;i<vet_zero.size();i++){
    if(vet_zero[i] == 0){
      naorepetidos.push_back(i);
    }
  }

random_shuffle(naorepetidos.begin(), naorepetidos.end());


  cout << endl << endl;


  for(i=0;i<filho.codigo.size();i++){
    if(filho.codigo[i] == -1){
      filho.codigo[i] = naorepetidos.back();
      naorepetidos.pop_back();
    }
  }


  cout << endl << endl;

}

vector<individuo> todos_filhos;

void Selecao(vector<individuo> &populacao, individuo &pai1, individuo &pai2){

  vector<individuo> PossiveisPais;
  int i,k = 4;
  for(i=0; i<k; i++) {
    PossiveisPais.push_back(populacao.at(rand()%(populacao.size())));
  }
  sort(PossiveisPais.begin(), PossiveisPais.end());
  pai1 = PossiveisPais.at(0);
  pai2 = PossiveisPais.at(1);


  cout << endl;
}

void Crossover(individuo pai1, individuo pai2, int n_vertice){

  int i1, i2, j,k,l;
  vector<int> vet_zero;
  individuo filho1, filho2, filho3, filho4, filho5, filho6;

  do{
    i1 = rand() % (n_vertice) + 1;
    i2 = rand() % (n_vertice) + 1;
  }while(i2 <= i1);

  for(j=0;j<=n_vertice;j++){
    vet_zero.push_back(0);
  }

  for(j=0;j<i1;j++){
    filho1.codigo.push_back(pai1.codigo[j]); //|A|
    filho2.codigo.push_back(pai2.codigo[j]); //|D|
    filho3.codigo.push_back(pai1.codigo[j]); //|A|
    filho4.codigo.push_back(pai1.codigo[j]); //|A|
    filho5.codigo.push_back(pai2.codigo[j]); //|D|
    filho6.codigo.push_back(pai2.codigo[j]); //|D|
  }
  for(j=i1;j<i2;j++){
    filho1.codigo.push_back(pai1.codigo[j]); //|A|B|
    filho2.codigo.push_back(pai2.codigo[j]); //|D|E|
    filho3.codigo.push_back(pai2.codigo[j]); //|A|E|
    filho4.codigo.push_back(pai2.codigo[j]); //|A|E|
    filho5.codigo.push_back(pai1.codigo[j]); //|D|B|
    filho6.codigo.push_back(pai1.codigo[j]); //|D|B|
  }
  for(j=i2;j<n_vertice;j++){                 // pai1: ABC , pai2: DEF
    filho1.codigo.push_back(pai2.codigo[j]); // |A|B|F
    filho2.codigo.push_back(pai1.codigo[j]); // |D|E|C
    filho3.codigo.push_back(pai1.codigo[j]); // |A|E|C
    filho4.codigo.push_back(pai2.codigo[j]); // |A|E|F
    filho5.codigo.push_back(pai2.codigo[j]); // |D|B|F
    filho6.codigo.push_back(pai1.codigo[j]); // |D|B|C
  }



  todos_filhos.push_back(filho1);
  todos_filhos.push_back(filho2);
  todos_filhos.push_back(filho3);
  todos_filhos.push_back(filho4);
  todos_filhos.push_back(filho5);
  todos_filhos.push_back(filho6);

  int i;

  for(j=0;j<todos_filhos.size();j++){
    eliminaverticesrepetidos(todos_filhos[j], vet_zero);
    break;
  }
}

int conte=0;
void Mutacao(int taxamutacao, int n_vertices){

  int i1, i2, aux,j,k=1;

 while(k<=taxamutacao){
   do{
     i1 = rand()%(n_vertices);
     i2 = rand()%(n_vertices);
   }while(i1==i2);

   for(j=0;j<todos_filhos.size();j++){
     aux = todos_filhos[j].codigo[i1];
     todos_filhos[j].codigo.at(i1) = todos_filhos[j].codigo.at(i2);
     todos_filhos[j].codigo.at(i2) = aux;

   }
    k++;

 }
}

void atualizacao(vector<individuo> &populacao, int tampop){
  //cout << "ATUALIZAÇÃO" << endl;
  int i;
  individuo aux;
  sort(populacao.begin(), populacao.end());
  for(i=0; i<int(todos_filhos.size()); i++) {
    if(int(populacao.size())<tampop){
      populacao.push_back(todos_filhos.at(i));
    }

  }

  for(i=0;i<6;i++){
    populacao[populacao.size()-1-i] = todos_filhos[i];

  }
}

double AG(individuo pai1, individuo pai2, int taxamutacao, int n_vertice, int max_geracoes, vector<individuo> &populacao, int tampop){
   int j, geracoes=0;
   Crossover(ind1, ind2, n_vertice);
   Mutacao(taxamutacao, n_vertice);
   for(j=0;j<todos_filhos.size();j++){
     todos_filhos[j].fitness = distancia_abs(todos_filhos[j].codigo, n_vertice);
   }


   while(geracoes<max_geracoes){

     atualizacao(populacao, tampop);
     todos_filhos.clear();
     Selecao(populacao, pai1, pai2);
     Crossover(pai1, pai2, n_vertice);
     Mutacao(taxamutacao, n_vertice);

     for(j=0;j<todos_filhos.size();j++){
       todos_filhos[j].fitness = distancia_abs(todos_filhos[j].codigo,n_vertice);

     }
     geracoes++;

   }



   cout << endl;

   return populacao[0].fitness;
 }

int main(){

 FILE *arquivo;
 caso pega_dados;
 int num_vertice;
 arquivo = fopen("brd14051", "r");
 if(fopen==NULL){
   cout << "Falha ao abrir o arquivo" << endl;
 }
 else{
   cout << "O arquivo abriu com sucesso" << endl;
 }

 fscanf(arquivo, "%i", &num_vertice);
 cout << endl;

 int i=1, tampopulacao = 50, maxgeracoes = 100000; // 100 200000 pra kro

 vector<individuo> vet_populacao;

 cout << num_vertice << endl;
 while(fscanf(arquivo, "%i %f %f", &pega_dados.vertice ,&pega_dados.x ,&pega_dados.y)!= EOF){
   cout << pega_dados.vertice << " " << pega_dados.x << " " << pega_dados.y << endl;
   grafo.push_back(pega_dados);
 }

 cout << endl;
 fclose(arquivo);

 for(i=1;i<=num_vertice;i++){
   ind1.codigo.push_back(i);
   ind2.codigo.push_back(i);
 }

random_shuffle(ind1.codigo.begin(), ind1.codigo.end());
random_shuffle(ind2.codigo.begin(), ind2.codigo.end());

for(i=0;i<50;i++){
  for(i=1;i<=num_vertice;i++){
    ind3.codigo.push_back(i);
    //ind2.codigo.push_back(i);
  }
  random_shuffle(ind3.codigo.begin(), ind3.codigo.end());
  ind3.fitness = distancia_abs(ind3.codigo, num_vertice);
  vet_populacao.push_back(ind3);
}


 random_shuffle(ind1.codigo.begin(), ind1.codigo.end());
 random_shuffle(ind2.codigo.begin(), ind2.codigo.end());

 ind1.fitness = distancia_abs(ind1.codigo, num_vertice);
 ind2.fitness = distancia_abs(ind2.codigo, num_vertice);

 vet_populacao.push_back(ind1);
 vet_populacao.push_back(ind2);


 cout << AG(ind1, ind2, 1, num_vertice, maxgeracoes, vet_populacao, tampopulacao) << endl;

}
