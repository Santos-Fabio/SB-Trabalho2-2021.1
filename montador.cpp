#include <bits/stdc++.h>
using namespace std;

//Variaveis Globais
vector<char> caracteres_especiais;
string Arquivo_de_entrada; 
typedef struct instr{
    string nome;
    int opcode;
    int tamanho;
}instrucao_def;
typedef struct simb{
    string nome;
    int vlr;
    bool def;
    stack<int> pilha;
}Simbolos;
vector<Simbolos> Tabela_Simbolos;
int PC=0,pc_stop=0,correcao_texto=0;
map<string, instrucao_def> Operacao;
string Saida_Data,Saida_Text;
bool Sec_text=false,Sec_data=false,jmp=false;

//Conjunto de definicoes
void def_carc_espec(){
    for(int i=33; i<48;i++){
        caracteres_especiais.push_back(i);
    }
    for(int i=59; i<65;i++){
        if(i!=95)
            caracteres_especiais.push_back(i);
    }
    for(int i=91; i<94;i++){
        caracteres_especiais.push_back(i);
    }
    for(int i=123; i<127;i++){
        caracteres_especiais.push_back(i);
    }

}
string Ler_entrada(char *argumento_main){//Ler arquivo de entrada e por em uma string ignorando comentario
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
            if(c==';'){
                while(true){
                    c = fgetc(pFile);
                    c = towupper(c);
                    if(c=='\n' || c==EOF){break;}
                }
            }
            if(c!=EOF)
            Arquivo.push_back(c);
        }
    }
    fclose(pFile);
    return Arquivo;
}
vector<instrucao_def> Cria_Tab_Comando(){//Tabela com Instruçoes sem Space e Const
    vector<instrucao_def> Tabela;
    instrucao_def instrucao;
    instrucao.nome="ADD";
    instrucao.opcode=1;
    instrucao.tamanho=2;
    Tabela.push_back(instrucao);
    
    instrucao.nome="SUB";
    instrucao.opcode=2;
    instrucao.tamanho=2;
    Tabela.push_back(instrucao);

    instrucao.nome="MUL";
    instrucao.opcode=3;
    instrucao.tamanho=2;
    Tabela.push_back(instrucao);

    instrucao.nome="DIV";
    instrucao.opcode=4;
    instrucao.tamanho=2;
    Tabela.push_back(instrucao);

    instrucao.nome="JMP";
    instrucao.opcode=5;
    instrucao.tamanho=2;
    Tabela.push_back(instrucao);

    instrucao.nome="JMPN";
    instrucao.opcode=6;
    instrucao.tamanho=2;
    Tabela.push_back(instrucao);

    instrucao.nome="JMPP";
    instrucao.opcode=7;
    instrucao.tamanho=2;
    Tabela.push_back(instrucao);

    instrucao.nome="JMPZ";
    instrucao.opcode=8;
    instrucao.tamanho=2;
    Tabela.push_back(instrucao);

    instrucao.nome="COPY";
    instrucao.opcode=9;
    instrucao.tamanho=3;
    Tabela.push_back(instrucao);

    instrucao.nome="LOAD";
    instrucao.opcode=10;
    instrucao.tamanho=2;
    Tabela.push_back(instrucao);

    instrucao.nome="STORE";
    instrucao.opcode=11;
    instrucao.tamanho=2;
    Tabela.push_back(instrucao);

    instrucao.nome="INPUT";
    instrucao.opcode=12;
    instrucao.tamanho=2;
    Tabela.push_back(instrucao);

    instrucao.nome="OUTPUT";
    instrucao.opcode=13;
    instrucao.tamanho=2;
    Tabela.push_back(instrucao);

    instrucao.nome="STOP";
    instrucao.opcode=14;
    instrucao.tamanho=1;
    Tabela.push_back(instrucao);
        
    instrucao.nome="SPACE";
    instrucao.opcode=0;
    instrucao.tamanho=1;
    Tabela.push_back(instrucao);

    instrucao.nome="CONST";
    instrucao.opcode=0;
    instrucao.tamanho=1;
    Tabela.push_back(instrucao);
    


    return Tabela;
}
 
vector<instrucao_def> Tabela_Comandos = Cria_Tab_Comando();


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

//Funcoes

string Ler_linha(int n_linha){
    char c;
    string comando,label,argumentos,linha,op_str;
    size_t found;
    vector<char>::iterator it;
    bool fim_label = false;
    int n,espaco_data=0,casas_decimais_variavel=0,op_int;
    Simbolos Rotulo;
    while(Arquivo_de_entrada.front()!='\n'){
        linha.push_back(Arquivo_de_entrada.front());
        Arquivo_de_entrada.erase(Arquivo_de_entrada.begin());
    }
    linha.append("\n");
    found = linha.find(':');
    if(found != string::npos){//Achou :, ou seja, tem label. Aqui salva a label na string
        for(int i=0; i<found;i++){
            c = linha.front();
            it = find(caracteres_especiais.begin(),caracteres_especiais.end(),c);
            if(it!=caracteres_especiais.end()){
                cout<<"Caracter Invalido_ Erro Lexico Linha:"<<n_linha<<endl;
                break;
            }
            linha.erase(linha.begin());
            if(c!=' ' && fim_label==false){
                label.push_back(c);
            }else if(!label.empty()){
                fim_label = true;
            }
            if((c!=' ' && c!=':' && fim_label==true) || (!label.empty() && (label[0]>='0' && label[0]<='9'))){
                cout<<"Erro Lexico na linha: "<<n_linha<<endl;
            }
            if(c==':'){break;}
        }
        while(linha.front()==' ' || linha.front()== ':'){
            if(linha.front()!='\n'){
                linha.erase(linha.begin());
            }
        }
    }
    found = linha.find("SECTION");
    if(found!=string::npos){//Se for linha de SECTION, altera a var global referente para true
        found = linha.find("DATA");
        if(found!=string::npos){Sec_data=true;}else{Sec_text=true;}
        return linha;
    }
    found = linha.find("CONST");
    if(found!=string::npos){//Definindo Constantes
        
        espaco_data = conversor_str_int(linha);
        
        Saida_Data.append(to_string(espaco_data));
        Saida_Data.push_back(' ');
        
    }else{
        found = linha.find("SPACE");
        if(found!=string::npos){//Definindo Spaces
            for(char i='0'; i<='9';i++){
                found=linha.find(i);
                if(found!=string::npos){
                    espaco_data = conversor_str_int(linha);
                    for(int i=0; i<espaco_data;i++){
                        Saida_Data.append(to_string(0));
                        Saida_Data.push_back(' ');
                        
                    }
                    break;
                }else if((i+1)>'9'){
                    Saida_Data.append(to_string(0));
                    Saida_Data.push_back(' ');
                    
                }
            }
        }
    }
    if(!label.empty()){
        vector<Simbolos>::iterator it_tab_S;
        if(!Tabela_Simbolos.empty()){
            it_tab_S = Tabela_Simbolos.begin();
                for(it_tab_S; it_tab_S!=Tabela_Simbolos.end();++it_tab_S){
                if(it_tab_S->nome==label){
                    //Empilhar Linha
                    it_tab_S->pilha.push(PC);
                    it_tab_S->def=true;
                    PC++;
                    //cout<<"Fzi"<<endl;
                    break;
                }
            }
        }
        if(it_tab_S==Tabela_Simbolos.end()){//Não existia o Rotulo
            Rotulo.nome = label;
            Rotulo.pilha.push(-1);
            Rotulo.def = true;
            Rotulo.vlr = PC;
            Tabela_Simbolos.push_back(Rotulo);
        }   
    }

    //Vamos tentar por as def
    if(Sec_data==true && Sec_text==false){PC=0;}



    //sec txt antes
    if(correcao_texto){
        if(linha=="\n"){PC-=2;}
        string argumento_linha;
        vector<char>::iterator carac_esp;
        size_t espec_arg;
        for(carac_esp=caracteres_especiais.begin(); carac_esp!=caracteres_especiais.end();++carac_esp){
            if(*carac_esp!=','){
                espec_arg = argumento_linha.find(*carac_esp);
            }
            if(espec_arg!=string::npos){
                cout<<"Erro Lexico na linha: "<<n_linha<<".Uso de caracter especial";
                break;
            }
        }
        int contador=0,comando_copy=0;
        size_t parou = linha.find(Tabela_Comandos[13].nome);
        if(parou!=string::npos){
            pc_stop=PC;
        }
        for(int i=0; i<14;i++){//ADD ao STOP
            if(i<4 || i>7){
                size_t achar = linha.find(Tabela_Comandos[i].nome);
                if(achar!=string::npos){
                    Saida_Text.append(to_string(Tabela_Comandos[i].opcode));
                    Saida_Text.append(" ");
                    if(Tabela_Comandos[i].nome=="COPY"){
                        comando_copy=1;
                    }
                    char c;
                    while(true){
                        c = linha[Tabela_Comandos[i].nome.size()+contador];
                        if(c==' '){
                            contador++;
                        }else{
                            break;
                        }
                    }
                    while(c!=' ' && c!='\n'){
                        argumento_linha.push_back(c);
                        contador++;
                        c = linha[Tabela_Comandos[i].nome.size()+contador];
                    }
                    break;
                }
            }else{//jmps
                for(int j=7; j>=4;j--){
                    size_t achar = linha.find(Tabela_Comandos[j].nome);
                    if(achar!=string::npos){
                    jmp=true;
                        Saida_Text.append(to_string(Tabela_Comandos[j].opcode));
                        Saida_Text.append(" ");
                        char c;
                        while(true){
                            c = linha[Tabela_Comandos[j].nome.size()+contador];
                            if(c==' '){
                                contador++;
                            }else{
                                break;
                            }
                        }
                        while(c!=' ' && c!='\n'){
                            argumento_linha.push_back(c);
                            contador++;
                            c = linha[Tabela_Comandos[j].nome.size()+contador];
                        }
                        i=7;
                        break;
                    }
                }
            } 
        }

        PC++;
        if(!comando_copy){
            if(Tabela_Simbolos.empty()){
                Rotulo.nome = argumento_linha;
                Rotulo.vlr = 0;
                Rotulo.def=false;
                Rotulo.pilha.push(-1);
                Rotulo.pilha.push(PC);
                Tabela_Simbolos.push_back(Rotulo);
            }else{
                for(int i=0; i<Tabela_Simbolos.size();i++){
                    if(Tabela_Simbolos[i].nome==argumento_linha){
                        Tabela_Simbolos[i].pilha.push(PC);
                        break;
                    }else if(i==Tabela_Simbolos.size()-1){
                        Rotulo.nome = argumento_linha;
                        Rotulo.vlr = 0;
                        Rotulo.def=false;
                        Rotulo.pilha.push(-1);
                        Rotulo.pilha.push(PC);
                        Tabela_Simbolos.push_back(Rotulo);
                        break;
                    }
                }
            }
        }else{
            string arg1,arg2;
            char c = argumento_linha[0];
            int auxiliar=1,virgula=0;
            while(true){
                arg1.push_back(c);
                c=argumento_linha[auxiliar];
                if(c==','){
                    virgula++;
                    break;
                }
                if(c!=' ' && c!='\n'){
                    auxiliar++;
                }else{
                    break;
                }
                if(auxiliar==10){break;}
            }

            while(true){

                auxiliar++;
                if(auxiliar==argumento_linha.size()){break;}
                c=argumento_linha[auxiliar];
                if(c==','){
                    virgula++;
                }else if(c!=' ' && c!='\n'){
                    arg2.push_back(c);
                }else{
                    break;
                }
            }

            if(arg1==arg2){
                cout<<"Error Semantico na Linha: "<<(n_linha)<<".Uso do mesmo argumentos no comando COPY."<<endl;
            }
            if(virgula!=1){
                cout<<"Erro Sintatico na Linha: "<<(n_linha)<<"COPY espera uma única \',\' para separar os argumentos"<<endl;
            }
            if(Tabela_Simbolos.empty()){
                Rotulo.nome = arg1;
                Rotulo.vlr = 0;
                Rotulo.def=false;
                Rotulo.pilha.push(-1);
                Rotulo.pilha.push(PC);
                Tabela_Simbolos.push_back(Rotulo);
                PC++;
                if(arg1!=arg2){
                    Rotulo.nome = arg2;
                    Rotulo.vlr = 0;
                    Rotulo.def=false;
                    Rotulo.pilha.push(-1);
                    Rotulo.pilha.push(PC);
                    Tabela_Simbolos.push_back(Rotulo);
                    PC++;
                }
            }else{
                for(int i=0; i<Tabela_Simbolos.size();i++){
                    if(Tabela_Simbolos[i].nome==arg1){
                        Tabela_Simbolos[i].pilha.push(PC);
                        break;
                    }else if(i==Tabela_Simbolos.size()-1){
                        Rotulo.nome = arg1;
                        Rotulo.vlr = 0;
                        Rotulo.def=false;
                        Rotulo.pilha.push(-1);
                        Rotulo.pilha.push(PC);
                        Tabela_Simbolos.push_back(Rotulo);
                        break;
                    }
                }
                PC++;
                for(int i=0; i<Tabela_Simbolos.size();i++){
                    if(Tabela_Simbolos[i].nome==arg2){
                        Tabela_Simbolos[i].pilha.push(PC);
                        break;
                    }else if(i==Tabela_Simbolos.size()-1){
                        Rotulo.nome = arg2;
                        Rotulo.vlr = 0;
                        Rotulo.def=false;
                        Rotulo.pilha.push(-1);
                        Rotulo.pilha.push(PC);
                        Tabela_Simbolos.push_back(Rotulo);
                    }
                }
            }
        }
        


        PC++;
    }


    

        if(!correcao_texto){
            for(int i=0; i<14;i++){//tab de comandos ADD ao STOP
                if(i<4 || i>7){//Exclui Jumps, mexendo só com variaveis e consts
                    found = linha.find(Tabela_Comandos[i].nome);
                    if(found!=string::npos){
                        op_int = Operacao[Tabela_Comandos[i].nome].opcode;
                        if(op_int!=0 && op_int!=5 && op_int!=6&& op_int!=7&& op_int!=8){
                            op_str.append(to_string(op_int));
                            Saida_Text.append(op_str);
                            Saida_Text.push_back(' ');
                        }
                        PC++;
                        int copy_comando=0;
                        if(op_int==9){copy_comando=1;}
                        for(int j=0; j<Tabela_Simbolos.size();j++){
                            auto found_lab = linha.find(Tabela_Simbolos[j].nome);
                            if(found_lab != string::npos){
                                if(Sec_data==true){//data foi definida, caso da hora
                                    Tabela_Simbolos[j].pilha.push(PC);
                                    PC++;
                                    if(!copy_comando){break;}
                                }else{
                                    Tabela_Simbolos[j].pilha.push(-1);
                                    Tabela_Simbolos[j].pilha.push(PC);
                                    PC++;
                                }
                            }
                        }
                        if(copy_comando){//Corrigindo a troca do COPY
                        PC--;
                            size_t f1,f2;
                            size_t prim,seg;
                            for(int a=0; a<=Tabela_Simbolos.size();a++){
                                f1=linha.find(Tabela_Simbolos[a].nome);
                                if(f1!=string::npos){
                                    break;
                                }
                            }
                            for(int b=Tabela_Simbolos.size(); b>Tabela_Simbolos.size()/2;b--){
                                f2=linha.find(Tabela_Simbolos[b].nome);
                                if(f2!=string::npos){
                                    break;
                                }
                            }
                            prim = min(int(prim),int(seg));
                            seg=max(int(prim),int(seg));
                            if(prim!=f1){
                                for(int j=0; j<Tabela_Simbolos.size();j++){
                                    if(Tabela_Simbolos[j].pilha.top()==PC){
                                        Tabela_Simbolos[j].pilha.pop();
                                        PC--;
                                        Tabela_Simbolos[j].pilha.push(PC);
                                        PC++;
                                    }else if(Tabela_Simbolos[j].pilha.top()==(PC-1)){
                                        Tabela_Simbolos[j].pilha.pop();
                                        //PC--;
                                        Tabela_Simbolos[j].pilha.push(PC);
                                        //PC++;
                                    }
                                }
                            }
                            PC++;
                        }
                    }
                }else{
                    for(int j=7; j>=4;j--){//jumps
                        found = linha.find(Tabela_Comandos[j].nome);
                        if(found != string::npos){
                        jmp=true;
                            op_int = Operacao[Tabela_Comandos[j].nome].opcode;
                            op_str.append(to_string(op_int));
                            Saida_Text.append(op_str);
                            Saida_Text.push_back(' ');
                            PC++;
                            found = linha.find(' ');
                            if(found!=string::npos){
                                linha.erase(linha.begin(),linha.begin()+found);
                                char aux = linha.front();
                                while(aux==' '){
                                    linha.erase(linha.begin());
                                    aux=linha.front();
                                }
                                int final_rot=0;
                                while(true){
                                    aux=linha[final_rot];
                                    if(aux==' ' ||aux=='\n'||aux==';'){
                                        break;
                                    }
                                    final_rot++;
                                }
                                for(int k=0; k<final_rot;k++){
                                    aux = linha[k];
                                    label.push_back(aux);
                                }
                                vector<Simbolos>::iterator it_tab_S;
                                if(!Tabela_Simbolos.empty()){
                                    it_tab_S = Tabela_Simbolos.begin();
                                    for(it_tab_S; it_tab_S!=Tabela_Simbolos.end();++it_tab_S){
                                        if(it_tab_S->nome==label){
                                            it_tab_S->pilha.push(--PC);
                                            PC++;
                                            it_tab_S->def=true;
                                            PC++;
                                            break;
                                        }
                                    }
                                }
                                if(it_tab_S==Tabela_Simbolos.end()){//Não existia o Rotulo
                                    Rotulo.nome = label;
                                    Rotulo.pilha.push(-1);
                                    Rotulo.def = false;
                                    Rotulo.vlr = 0;
                                    Tabela_Simbolos.push_back(Rotulo);
                                }
                            }
                            
                            break;
                        }
                    }
                }
            }
        }
    



    while(Arquivo_de_entrada.front()=='\n'){
        Arquivo_de_entrada.erase(Arquivo_de_entrada.begin());
    }
    linha.pop_back();//removendo o \n
    if(pc_stop>0){
        linha = label + " " + linha;
    }
    return linha;
}



void Saida_Text_com_Pilha(){
    int opcode_intsrucao_sec,dist,linha,topo,vlr_a_ser_posto;
    map<int,instrucao_def> Operacao_op;
    string rduplas,duplas,saida_text_aux;
    saida_text_aux.append(Saida_Text);
    for(int i=0; i<Tabela_Comandos.size();i++){//Mapa pelo opcode, ao inves do nome
        Operacao_op[Tabela_Comandos[i].opcode] = Tabela_Comandos[i];
    }
    for(string::reverse_iterator rfound = saida_text_aux.rbegin(); rfound!=saida_text_aux.rend();++rfound){
        if(*rfound!=' '){
            rduplas.push_back(*rfound);
            ++rfound;
            rduplas.push_back(*rfound);
            if(rduplas[1]!=' '){
            duplas.push_back(rduplas[1]);}
            duplas.push_back(rduplas[0]);
            opcode_intsrucao_sec = conversor_str_int(duplas);
            if(Operacao_op[opcode_intsrucao_sec].opcode!=14){
                if(Operacao_op[opcode_intsrucao_sec].opcode!=9){
                    for(int j=0; j<Operacao_op[opcode_intsrucao_sec].tamanho;j++){
                        dist = saida_text_aux.size() - distance(saida_text_aux.rbegin(),rfound)+2;
                        for(int i=0; i<Tabela_Simbolos.size();i++){
                            if(Tabela_Simbolos[i].pilha.top()==PC){
                                vlr_a_ser_posto = Tabela_Simbolos[i].vlr;
                                topo = Tabela_Simbolos[i].pilha.top();
                                Tabela_Simbolos[i].pilha.pop();
                                break;
                            }
                        }
                        if(topo!=-1&&vlr_a_ser_posto!=-1){
                            Saida_Text.insert(dist,(to_string(vlr_a_ser_posto)+" "));
                            break;
                        }
                        PC--;
                    }
                    PC--;
                }else{
                    PC++;
                    PC--;
                    for(int a=0; a<2;a++){
                        PC--;
                        for(int b=0; b<Operacao_op[opcode_intsrucao_sec].tamanho;b++){
                            dist = saida_text_aux.size() - distance(saida_text_aux.rbegin(),rfound)+2;
                            for(int c=0; c<Tabela_Simbolos.size();c++){
                                if(Tabela_Simbolos[c].pilha.top()==PC){
                                    vlr_a_ser_posto = Tabela_Simbolos[c].vlr;
                                    topo = Tabela_Simbolos[c].pilha.top();
                                    Tabela_Simbolos[c].pilha.pop();
                                    break;
                                }
                            }
                            if(topo!=-1 && vlr_a_ser_posto!=-1){
                                Saida_Text.insert(dist,(to_string(vlr_a_ser_posto)+" "));
                                break;
                            }
                            PC--;
                        }
                    }
                    PC--;
                }
            }else{PC--;}
        }else{
        }
        duplas.erase(duplas.begin(),duplas.end());
        rduplas.erase(rduplas.begin(),rduplas.end());
        topo=-1;
        vlr_a_ser_posto=-1;
        opcode_intsrucao_sec=-1;
        dist=0;
    }
    
}




void error_linha(string linha_processada, int n_linha){
    size_t found;
    int comeco_comando=0,qtd_arg=1,virgulas=0;
    found  = linha_processada.find("STOP");
    if(found!=string::npos){return;}
    found  = linha_processada.find("SPACE");
    if(found!=string::npos){return;}
    found  = linha_processada.find("CONST");
    if(found!=string::npos){return;}

    if(linha_processada.size()==1 && (linha_processada.front()==' ' || linha_processada.front()=='\n') || linha_processada.size()==0){return;}
    //Duplo Rotulo na linha
    found = linha_processada.find(":");
    if(found!=string::npos){
        cout<<"Erro Sintático linha: "<<n_linha<<". Dupla declaração de rótulo na linha."<<endl;
        return;
    }
    //Erros de Argumento
    map<string, instrucao_def>::iterator it = Operacao.begin();
    string comando_l,argumento_l1,argumento_l2;
    for(int i=0; i<linha_processada.size();i++){
        if(linha_processada[i]==' '){
            break;
        }
        comando_l.push_back(linha_processada[i]);
    }
    char c;
    found = linha_processada.find(",");
    if(found!=string::npos){
        qtd_arg=2;

    }
        for(int i=comando_l.size(); i<linha_processada.size();i++){
            c = linha_processada[i];
            if(c==','){virgulas++;}
            switch(virgulas){
                case 0:
                    argumento_l1.push_back(c);
                    break;
                case 1:
                    argumento_l2.push_back(c);
                    break;
                default:
                    cout<<"Erro Sintático linha: "<<n_linha<<endl;
                    break;
            }
        }
    //}
    if(!argumento_l2.empty()){argumento_l2.erase(argumento_l2.begin());}//remove , no inicio
    for(it; it!=Operacao.end();++it){
        if(it->first == comando_l){
            break;
        }
    }
    if(it==Operacao.end()){
        cout<<"Erro Léxico, comando não indentificado linha: "<<n_linha<<endl;
    }
}

void error_tab_simbolos(){//
    vector<Simbolos>::iterator it = Tabela_Simbolos.begin();
    for(it; it!=Tabela_Simbolos.end();++it){
        //cout<<"Nome: "<<it->nome<<"\nDefinido: "<<it->def<<"\nvlr: "<<it->vlr<<"\nTopo: "<<it->pilha.top()<<endl;
        if(it->def==0){
            cout<<"Erro Semantico, rótulo não declarado: "<<it->nome<<endl;
        }
        if(it->nome.size()>50){
            cout<<"Erro Lexico, rotulo com mais de 50 caracteres: "<<it->nome;
        }
    }

}




string trabalho2(string header, int meio_alocacao){
    map<int,int> instrucoes_inteiras;
    for(int i=1; i<=14;i++){
        instrucoes_inteiras[i]=2;
        if(i==14){
            instrucoes_inteiras[i]--;
        }else if(i==9){
            instrucoes_inteiras[i]++;
        }
    }
    
    string saida;
    char c;
    string auxiliar = Saida_Text,str;
    vector<int> Saida_texto_inteiros;
    for(int i=0; i<auxiliar.size();i++){
        char c = auxiliar[i];
        if(c>='0' && c<='9'){
            while(c!=' '){
                i++;
                str.push_back(c);
                c = auxiliar[i];
            }
            Saida_texto_inteiros.push_back(conversor_str_int(str));
            str.clear();
        }
    }

    saida.append("H: ");
    for(int i=0; i<header.size();i++){
        c = towupper(header[i]);
        saida.push_back(c);
    }
    saida.push_back('\n');
    saida.append("H: ");
    saida.append(to_string(Saida_texto_inteiros.size()));
    saida.push_back('\n');
    saida.append("H: ");

    string mapa_bits, lista_enderecos;
    for(int i=0; i<Saida_texto_inteiros.size();i++){
        mapa_bits.push_back('0');
        if(Saida_texto_inteiros[i]==9){
            i++;
            mapa_bits.push_back('1');
            lista_enderecos.append(to_string(i));
            lista_enderecos.push_back(' ');
        }else if(Saida_texto_inteiros[i]==14){
            for(int j=i; j<Saida_texto_inteiros.size()-1;j++){
                mapa_bits.push_back('0');
            }
            break;
        }
        i++;
        mapa_bits.push_back('1');
        lista_enderecos.append(to_string(i));
        lista_enderecos.push_back(' ');
    }
    if(meio_alocacao==0){
        saida.append(mapa_bits);
    }else{
        saida.append(lista_enderecos);
    }
    saida.push_back('\n');
    saida.append("T: ");
    saida.append(Saida_Text);

    return saida;
}



int main(int argc, char *argv[]){
    Arquivo_de_entrada = Ler_entrada(argv[3]);
    int text_true_data_false_final=0;
    string ordem_sec_data;

    for(int i=0; i<Tabela_Comandos.size();i++){//Criando Tab de comando
        Operacao[Tabela_Comandos[i].nome] = Tabela_Comandos[i];
    }

    def_carc_espec();


    for(int i=0; !Arquivo_de_entrada.empty();i++){
        string teste = Ler_linha(i);
        if((Sec_text==true&&Sec_data==false)&&teste!="\n"){
            text_true_data_false_final=1;
            correcao_texto=1;
            for(int j=0; j<=14;j++){
                size_t comando_contar_PC = teste.find(Tabela_Comandos[j].nome);
                if(comando_contar_PC!=string::npos){
                    break;
                }
            }
        }
        if(text_true_data_false_final && pc_stop>0){
            for(vector<Simbolos>::iterator a = Tabela_Simbolos.begin()+1; a!=Tabela_Simbolos.end();++a){
                size_t it = teste.find(a->nome);
                    if(it!=string::npos){
                        ordem_sec_data.append(a->nome);
                        ordem_sec_data.append(" ");
                    }
                
            }
        }
        //cout<<teste<<endl;
        size_t achar_sd,achar_st;
        achar_sd= teste.find("SECTION DATA");
        achar_st=teste.find("SECTION TEXT");
        //cout<<achar_st<<":st "<<achar_sd<<":sd "<<jmp<<":jmp"<<endl;
        if(achar_sd==string::npos && achar_st==string::npos && !jmp){
            error_linha(teste,i);}
        teste.erase(teste.begin(),teste.end());
        jmp=false;
    }
    ordem_sec_data.pop_back();
    if(text_true_data_false_final){
        Tabela_Simbolos.erase(Tabela_Simbolos.begin());
        int new_pc = pc_stop;
                new_pc++;
        for(int i=0; i<Saida_Data.size();i++){
            if(Saida_Data[i]!=' '&&Saida_Data[i]!='\n'){
                char c;
                if(!ordem_sec_data.empty()){
                    c=ordem_sec_data[0];
                }
                int tam_dado=0;
                string dado;
                while(c!=' '){
                    if(!ordem_sec_data.empty()){
                        c = ordem_sec_data[0];
                    }else{c=' ';}
                    if(c!=' '){
                        tam_dado++;
                        dado.push_back(c);
                    }
                    if(!ordem_sec_data.empty()){
                        ordem_sec_data.erase(ordem_sec_data.begin());
                    }
                }

                for(int j=0; j<Tabela_Simbolos.size();j++){
                    if(dado == Tabela_Simbolos[j].nome){
                        Tabela_Simbolos[j].vlr=new_pc;
                        Tabela_Simbolos[j].pilha.pop();
                        new_pc++;
                        break;
                    }
                }
                dado.erase(dado.begin(),dado.end());
            }
        }
    }


        for(int i=0; i<Saida_Data.size();i++){
            if(Saida_Data[i]!=' '&&Saida_Data[i]!='\n'){
                for(int j=0; j<Tabela_Simbolos.size();j++){
                    if(Tabela_Simbolos[j].vlr==0){    
                        Tabela_Simbolos[j].vlr=PC;
                        break;
                    }
                }
                PC++;
            }
        }
        PC--;

        if(!text_true_data_false_final){
            for(int i=0; i<Saida_Data.size();i++){
                if(Saida_Data[i]!=' ' && Saida_Data[i]!='\n'){
                    PC--;
                }
            }
        }

        if(text_true_data_false_final){PC=pc_stop+1;}
        Saida_Text_com_Pilha();
        Saida_Text.append(Saida_Data);
        string arquivo_final, str1=".obj";
        arquivo_final.append(argv[3]);
        arquivo_final.replace(arquivo_final.size()-4,arquivo_final.size()-1,str1);

        error_tab_simbolos();


        string header = arquivo_final;
        string arquivo_final_2;
        header.erase(header.size()-4,header.size()-1);
        int meio_aloc = -1;
        if(*argv[2]=='0'){
            meio_aloc=0;
        }else{
            meio_aloc=1;
        }
        arquivo_final_2 = trabalho2(header,meio_aloc);

        ofstream Arquivo_de_saida(arquivo_final);
        Arquivo_de_saida<<arquivo_final_2;
        Arquivo_de_saida.close();
        //cout<<"TEXT PRONTO: "<<Saida_Text<<endl;;
    return 0;
}


