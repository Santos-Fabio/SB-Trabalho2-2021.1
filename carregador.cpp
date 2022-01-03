#include <bits/stdc++.h>
using namespace std;



//string Arquivo_de_entrada;
pair<int,vector<int>> Arquivo_de_entrada;

map<int,int> instrucao;

int inteiro_auxiliar=0;
vector<int> inteiros_auxiliares;

void cria_tab_instrucao(){
    for(int i=1; i<=14;i++){
        instrucao[i]=2;
        if(i==14){
            instrucao[i]--;
        }else if(i==9){
            instrucao[i]++;
        }
    }
}


string Ler_entrada(char *argumento_main){//Ler arquivo de entrada e poe em uma string ignorando comentario
    FILE *pFile;
    char c;
    string Arquivo;
    pFile = fopen(argumento_main,"r");
    if(pFile == NULL){
        perror("Arquivo Inexistente");
    }else{
        while(c!=EOF){
            c = fgetc(pFile);
            c = towupper(c);
            if(c!=EOF)
            Arquivo.push_back(c);
        }
    }
    fclose(pFile);
    return Arquivo;
}

int conversor_str_int(string str){
    int casas_decimais=0,n=0,aux=0;
    for(int i=0; i<str.size();i++){
        if(str[i]>='0' && str[i]<='9'){
            casas_decimais++;
        }
    }
    if(casas_decimais==1){
        for(int i=0; i<str.size();i++){
            if(str[i]>='0' && str[i]<='9'){
                n=str[i]-'0';
            }
        }
    }else{
        for(int i=str.size()-1; i>=0;i--){
            if(str[i]>='0' && str[i]<='9'){
                n+=(str[i]-'0')*pow(10,aux);
                aux++;
            }
        }
    }
    return n;
}

string Arquivo_Carregador_um_Chunk(int endereco_inicial,vector<int> codigo_obj){
    string saida;
    int contador = 0;
    for(int i=0; i<codigo_obj.size();i++){
        saida.append(to_string(endereco_inicial));
        saida.push_back(' ');
        saida.append(to_string(codigo_obj[i]));
        saida.push_back(' ');
        if(codigo_obj[i]==9){
            i++;
            saida.append(to_string(codigo_obj[i]));
            saida.push_back(' ');
        }else if(codigo_obj[i]==14){
            endereco_inicial+=instrucao[codigo_obj[i]];
            i++;
            contador=i;
            saida.push_back('\n');
            break;

        }
        i++;
        saida.append(to_string(codigo_obj[i]));
        if(codigo_obj[i]==9){
            endereco_inicial++;
        }
        endereco_inicial++;
        endereco_inicial++;
        //endereco_inicial+=instrucao[codigo_obj[i]];
        saida.push_back('\n');
    }
    for(int i=contador; i<codigo_obj.size();i++){
        saida.append(to_string(endereco_inicial));
        saida.push_back(' ');
        saida.append(to_string(codigo_obj[i]));
        saida.push_back('\n');
        endereco_inicial++;
    }
    inteiro_auxiliar = endereco_inicial;
    return saida;
}

string Arquivo_Carregador_Multi_Chunk(vector<int> enderecos_0,vector<int> codigo_obj,vector<int>tamanhos_chunks){
    string saida;
    int contador = 0,endereco_no_momento = 0;
    vector<int> enderecos_f = enderecos_0;
    for(int i=0; i<codigo_obj.size();i++){
        if(enderecos_f[endereco_no_momento]+1>enderecos_f[endereco_no_momento]+tamanhos_chunks[endereco_no_momento]){
            endereco_no_momento++;
            i--;
        }else{
            saida.append(to_string(enderecos_f[endereco_no_momento]));
            saida.push_back(' ');
            saida.append(to_string(codigo_obj[i]));
            saida.push_back(' ');
            if(codigo_obj[i]==9){
                i++;
                if(enderecos_f[endereco_no_momento]+1>enderecos_f[endereco_no_momento]+tamanhos_chunks[endereco_no_momento]){
                    endereco_no_momento++;
                }
                enderecos_f[endereco_no_momento]++;
                saida.append(to_string(enderecos_f[endereco_no_momento]+codigo_obj[i]));
                saida.push_back(' ');
            }else if(codigo_obj[i]==14){
                enderecos_f[endereco_no_momento]+=instrucao[codigo_obj[i]];//como é 1, não preciso checar, ja chequei em cima
                i++;
                if(enderecos_f[endereco_no_momento]+1>enderecos_f[endereco_no_momento]+tamanhos_chunks[endereco_no_momento]){
                    endereco_no_momento++;
                }
                contador=i;
                saida.push_back('\n');
                break;

            }
            i++;
            if(enderecos_f[endereco_no_momento]+1>enderecos_f[endereco_no_momento]+tamanhos_chunks[endereco_no_momento]){
                endereco_no_momento++;
            }
            saida.append(to_string(enderecos_f[endereco_no_momento]+codigo_obj[i]));
            enderecos_f[endereco_no_momento]++;
            if(enderecos_f[endereco_no_momento]+1>enderecos_f[endereco_no_momento]+tamanhos_chunks[endereco_no_momento]){
                endereco_no_momento++;
            }
            enderecos_f[endereco_no_momento]++;
            if(codigo_obj[i-1]==14){
                enderecos_f[endereco_no_momento]--;
            }
            saida.push_back('\n');
        }
    }
    for(int i=contador; i<codigo_obj.size();i++){
        if(enderecos_f[endereco_no_momento]+1>enderecos_f[endereco_no_momento]+tamanhos_chunks[endereco_no_momento]){
            endereco_no_momento++;
        }
        saida.append(to_string(enderecos_f[endereco_no_momento]));
        saida.push_back(' ');
        saida.append(to_string(codigo_obj[i]));
        saida.push_back('\n');
        enderecos_f[endereco_no_momento]++;
    }
    /*for(int i=0; i<enderecos_f.size();i++){
        enderecos_0[i] =  enderecos_f[i]. //errado
    }*/
    for(int i=0; i<enderecos_f.size();i++){
        inteiros_auxiliares.push_back(enderecos_f[i]);
    }

    return saida;



}

string corrigindo_erro_posicao_relativa(string entrada_errada){
    string saida,str;
    char c;
    int primeiro_endereco = 0, cont=0,soma=0,chunk_inicial=0;
    c = entrada_errada[0];
    while(c!=' '){
        str.push_back(c);
        cont++;
        c = entrada_errada[cont];
    }
    chunk_inicial = conversor_str_int(str);
    bool stop =false;
    while(!stop){
        soma=0;
        while(true){
            cont = 0;
            c = entrada_errada[cont];
            str.clear();
            bool endereco_linha = false, opcode = false;
            while(!endereco_linha && !opcode){//Ler até a posição  relativa, que está errada
                cont++;
                c = entrada_errada[cont];
                if(c == ' '){
                    if(!endereco_linha){
                        endereco_linha=true;
                    }else{
                        opcode = true;
                    }
                }
            }
            while(c!='\n'){
                cont++;
                c = entrada_errada[cont];
                str.push_back(c);
            }
            primeiro_endereco = conversor_str_int(str);
            str = to_string(primeiro_endereco);
            break;
        }
    
        cont = 0;
        c = entrada_errada[cont];
        int pos_str = 0;
        bool endereco_linha = false, opcode = false;
        while(!endereco_linha || !opcode){
            cont++;
            c = entrada_errada[cont];
            if(c == ' ' || c=='\n'){
                if(!endereco_linha){
                    endereco_linha=true;
                }else if(opcode==false){
                    opcode = true;
                }else{
                    stop = true;
                    break;
                }
            }
        }
        
        pos_str = cont+1;
        
        str.clear();
        while(c!='\n' && !stop){
            cont++;
            c = entrada_errada[cont];
            str.push_back(c);
        }
        str.pop_back();
        //cout<<str<<endl;
        //cout<<str.size()<<endl;
        bool copy_ = false;
        string str_argumento1,str_argumento2;
        if(str.size()>=3){
            size_t found = str.find(" ");
            string str_arg1 (str,0,found), str_arg2 (str,found+1,str.size()-1);
            copy_ = true;
            str.clear();
            str = str_arg1;
            str_argumento2 = str_arg2;
            str_argumento1 = str_arg1;
            
        }
        soma = conversor_str_int(str);
        soma+=chunk_inicial;
        entrada_errada.erase(entrada_errada.begin()+pos_str,entrada_errada.begin()+pos_str+str.size());
        str.clear();
        str = to_string(soma);
        if(copy_){
            soma = conversor_str_int(str_argumento2);
            soma+=chunk_inicial;
            int sad;
            entrada_errada.erase(entrada_errada.begin()+pos_str,entrada_errada.begin()+pos_str+str.size());
            str.push_back(' ');
            str.append(to_string(soma));
            str.push_back('\n');
            copy_=false;
        }
        
        entrada_errada.insert(pos_str,str);
        
        primeiro_endereco = -1;
        str.clear();
        char linha_corrigida='!';
        //int cont_corrigida = 0;
        
        while(linha_corrigida!='\n'){
            linha_corrigida = entrada_errada[0];
            saida.push_back(linha_corrigida);
            entrada_errada.erase(entrada_errada.begin());
        }
        //saida.pop_back();
        //cin>>sad;
        string op;
        int parar = 0,parou=0;
        char p=entrada_errada[0];
        bool troca = false;
        while(p!='\n'){
            parou++;
            p=entrada_errada[parou];
            if(p==' ' && !troca){
                troca=true;
            }
            if(troca==true){
                if(p>='0'&&p<='9'){
                    op.push_back(p);
                }
            }
        }
            if(op == "14"){
                stop=true;
                saida.append(entrada_errada);
                op.clear();
            }
    }       
    return saida;

}

int main(int argc, char *argv[]){
    cria_tab_instrucao();
    
    string arquivo_final;
    vector<string> headers;
    int quantidade_de_numeros=0,quantidade_de_obj=0,tamanho_programa,saidas_trabalhadas=0;
    string Arquivo_de_entrada_nao_tratado, str_obj = ".obj",nome_arquivo_saida1,nome_arquivo_saida2,nome_arquivo_saida3;    
    for(int i=1; i<argc;i++){
        if(*argv[i]>='0' && *argv[i]<='9'){
            quantidade_de_numeros++;
        }else{
            quantidade_de_obj++;
            if(nome_arquivo_saida1.empty()){
                nome_arquivo_saida1 = argv[i];
                nome_arquivo_saida1.replace(nome_arquivo_saida1.size()-4,nome_arquivo_saida1.size()-1,".saida");
            }else if(nome_arquivo_saida2.empty()){
                nome_arquivo_saida2 = argv[i];
                nome_arquivo_saida2.replace(nome_arquivo_saida2.size()-4,nome_arquivo_saida2.size()-1,".saida");
            }else if(nome_arquivo_saida3.empty()){
                nome_arquivo_saida3 = argv[i];
                nome_arquivo_saida3.replace(nome_arquivo_saida3.size()-4,nome_arquivo_saida3.size()-1,".saida");
            }
        }
    }
    char letra;
    string header_nome1,header_nome2,header_nome3;
    if(!nome_arquivo_saida1.empty()){
        for(int i=0; i<nome_arquivo_saida1.size();i++){
            letra = nome_arquivo_saida1[i];
            if(letra=='.' && nome_arquivo_saida1[i+1]=='s'){break;}
            header_nome1.push_back((letra));
        }
        headers.push_back(header_nome1);
    }
    if(!nome_arquivo_saida2.empty()){
        for(int i=0; i<nome_arquivo_saida2.size();i++){
            letra = nome_arquivo_saida2[i];
            if(letra=='.' && nome_arquivo_saida2[i+1]=='s'){break;}
            header_nome2.push_back((letra));
        }
        headers.push_back(header_nome2);
    }
    if(!nome_arquivo_saida3.empty()){
        for(int i=0; i<nome_arquivo_saida3.size();i++){
            letra = nome_arquivo_saida3[i];
            if(letra=='.' && nome_arquivo_saida3[i+1]=='s'){break;}
            header_nome3.push_back((letra));
        }
        headers.push_back(header_nome3);
    }


    vector<int> entrada_numeral;
    for(int i=argc-quantidade_de_numeros; i<argc;i++){
        entrada_numeral.push_back(conversor_str_int(argv[i]));
    }
    int qtd_chunks_disponiveis = entrada_numeral[0];
    entrada_numeral.erase(entrada_numeral.begin());
    map<int,pair<int,int>> chunks;//key: tamanho+endereço inicial
    for(int i=0; i<qtd_chunks_disponiveis;i++){
        chunks[i] = make_pair(entrada_numeral[i],entrada_numeral[i+qtd_chunks_disponiveis]);
    }
   


    int headers_trabalhados = 0;
    for(int i=1; i<argc-quantidade_de_numeros;i++){
        Arquivo_de_entrada_nao_tratado = Ler_entrada(argv[i]);
        char c='z';//Comecei em z pois o header do .obj é diferente disso smepre 
        int contador=0;
        while(c!='\n'){
            c = Arquivo_de_entrada_nao_tratado[contador];
            contador++;
        }
        while(c!=':'){
            c = Arquivo_de_entrada_nao_tratado[contador];
            contador++;
        }
        c = Arquivo_de_entrada_nao_tratado[contador];
        while(c==' '){
            contador++;
            c = Arquivo_de_entrada_nao_tratado[contador];
        }
        if(c>='0' && c<='9'){
            string str_tamanho_programa;
            while(c>='0'&& c<='9'){
                str_tamanho_programa.push_back(c);
                contador++;
                c = Arquivo_de_entrada_nao_tratado[contador];
            }
            tamanho_programa = conversor_str_int(str_tamanho_programa);
        }

        vector<int> codigo_objeto;
        while(c!='T'){
            contador++;
            c = Arquivo_de_entrada_nao_tratado[contador];
        }
        while(c!=':'){
            c = Arquivo_de_entrada_nao_tratado[contador];
            contador++;
        }
        c = Arquivo_de_entrada_nao_tratado[contador];
        while(c==' '){
            contador++;
            c = Arquivo_de_entrada_nao_tratado[contador];
        }
        while(c!='\0'){
            if(c>='0' && c<='9'){
                string str_c_obj;;
                while(c>='0'&& c<='9'){
                    str_c_obj.push_back(c);
                    contador++;
                    c = Arquivo_de_entrada_nao_tratado[contador];
                }
                codigo_objeto.push_back(conversor_str_int(str_c_obj));
                str_c_obj.clear();
            }
            contador++;
            c = Arquivo_de_entrada_nao_tratado[contador];
        }
        
        string saida_um_chunk;
        int chunk_escolhido = -1;
        bool chunk_insuficiente = false, nao_alocou = true;
        for(int j=0; j<chunks.size();j++){//For Para quando um único chunk chunk consegue resolver solo 
            if(chunks[j].first>=tamanho_programa){
                chunk_escolhido = j;
                cout<<headers[headers_trabalhados]<<" utilizando 1 CHUNK. Endereços iniciais: "<<chunks[j].second<<endl;
                headers_trabalhados++;
                chunks[j].first-=tamanho_programa;
                saida_um_chunk = Arquivo_Carregador_um_Chunk(chunks[j].second,codigo_objeto);
                chunks[j].second = inteiro_auxiliar;
                inteiro_auxiliar=0;
                if(saidas_trabalhadas==0){
                    arquivo_final = header_nome1;
                }else if(saidas_trabalhadas==1){
                    arquivo_final=header_nome2;
                }else if(saidas_trabalhadas==2){
                    arquivo_final=header_nome3;
                }
                saidas_trabalhadas++;

                saida_um_chunk = corrigindo_erro_posicao_relativa(saida_um_chunk);

                ofstream Arquivo_de_saida_um_chunk(arquivo_final);
                Arquivo_de_saida_um_chunk<<saida_um_chunk;
                Arquivo_de_saida_um_chunk.close();
                saida_um_chunk.clear();
                arquivo_final.clear();
                nao_alocou = false;
                break;
            }
        }


        if(chunk_escolhido == -1){//Não cabia em nenhum
            chunk_escolhido = 0;
            chunk_insuficiente = true;
        }

        string saida_multi_chunk;
        vector<int> tamanhos_para_parametro;
        if(chunk_insuficiente){
            int espaco_total_dos_chunks = 0;
            vector<int> endercos_init;
            for(int j=0; j<chunks.size();j++){
                espaco_total_dos_chunks+=chunks[j].first;
                endercos_init.push_back(chunks[j].second);
                tamanhos_para_parametro.push_back(chunks[j].first);
                if(espaco_total_dos_chunks>=tamanho_programa){
                    nao_alocou=false;
                    cout<<headers[headers_trabalhados]<<" utilizando "<<endercos_init.size()<<" CHUNKS. Endereços iniciais: ";    
                    for(int s=0; s<endercos_init.size();s++){
                        cout<<endercos_init[s]<<" ";
                    }
                    cout<<endl;
                    headers_trabalhados++;
                    saida_multi_chunk = Arquivo_Carregador_Multi_Chunk(endercos_init,codigo_objeto,tamanhos_para_parametro);

                    for(int a=0; a<endercos_init.size();a++){
                        for(int b=0; b<chunks.size();b++){
                            if(chunks[b].second==endercos_init[a])
                                chunks[b].second = inteiros_auxiliares[0];
                                inteiros_auxiliares.erase(inteiros_auxiliares.begin());
                                break;
                        }
                    }

                    if(saidas_trabalhadas==0){
                        arquivo_final = header_nome1;
                    }else if(saidas_trabalhadas==1){
                        arquivo_final=header_nome2;
                    }else if(saidas_trabalhadas==2){
                        arquivo_final=header_nome3;
                    }
                    saidas_trabalhadas++;
                    //saida_multi_chunk = corrigindo_erro_posicao_relativa(saida_multi_chunk);
                    ofstream Arquivo_de_saida_multi_chunk(arquivo_final);
                    Arquivo_de_saida_multi_chunk<<saida_multi_chunk;
                    Arquivo_de_saida_multi_chunk.close();
                    saida_multi_chunk.clear();
                    arquivo_final.clear();
                    nao_alocou = false;
                    break;

                }
            }
        }

        if(nao_alocou==true){
            cout<<headers[headers_trabalhados]<<" NAO FOI POSSIVEL ALOCAR"<<endl;
            headers_trabalhados++;
            saidas_trabalhadas++;
        }
        //codigo_objeto.erase(codigo_objeto.begin(),codigo_objeto.end());
    }


    return 0;
}


//NÃO TESTEI PRA MULTIPLOS CHUNKS, ACREDITO ESTAR ERRADO (principalmente COPY)