/*Cabeçalho (Tem q fazer dps)*/

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>
#include <iterator> 
#include <ctype.h>
using namespace std;

class ItemTabelaDeOperacoes
{
public:
	string opcode;
	int n_args;
	int memoria;
};

class ItemTabelaDeSimbolos
{
public:
	string simb;
	int endr = -2; //Codigo para endereço não definido
	bool def;
	vector<int> lista_pendencia;
	vector <int> lista_pendencia_off;	
};

class ItemTabelaDeDados
{
public:
	string tipo; 
 	int valor;	
};

class ItemTabelaDeErros
{
public:
	string label;
	string mensagem;
	int linha;
	
};

auto inicia_tabela_op(){
	map <string, ItemTabelaDeOperacoes> tabela;
	tabela["ADD"].opcode = "01", tabela["ADD"].n_args = 1, tabela["ADD"].memoria = 2;
	tabela["SUB"].opcode = "02", tabela["SUB"].n_args = 1, tabela["SUB"].memoria = 2;
	tabela["MULT"].opcode = "03", tabela["MULT"].n_args = 1, tabela["MULT"].memoria = 2;
	tabela["DIV"].opcode = "04", tabela["DIV"].n_args = 1, tabela["DIV"].memoria = 2;
	tabela["JMP"].opcode = "05", tabela["JMP"].n_args = 1, tabela["JMP"].memoria = 2;
	tabela["JMPN"].opcode = "06", tabela["JMPN"].n_args = 1, tabela["JMPN"].memoria = 2;
	tabela["JMPP"].opcode = "07", tabela["JMPP"].n_args = 1, tabela["JMPP"].memoria = 2;
	tabela["JMPZ"].opcode = "08", tabela["JMPZ"].n_args = 1, tabela["JMPZ"].memoria = 2;
	tabela["COPY"].opcode = "09", tabela["COPY"].n_args = 2, tabela["COPY"].memoria = 3;
	tabela["LOAD"].opcode = "10", tabela["LOAD"].n_args = 1, tabela["LOAD"].memoria = 2;
	tabela["STORE"].opcode = "11", tabela["STORE"].n_args = 1, tabela["STORE"].memoria = 2;
	tabela["INPUT"].opcode = "12", tabela["INPUT"].n_args = 1, tabela["INPUT"].memoria = 2;
	tabela["OUTPUT"].opcode = "13", tabela["OUTPUT"].n_args = 1, tabela["OUTPUT"].memoria = 2;
	tabela["STOP"].opcode = "14", tabela["STOP"].n_args = 0, tabela["STOP"].memoria = 1;
	
	return tabela;
}

auto pega_tokens(string linha) {
	string linha_sem_comentario;
	string rotulo, operacao, arg1,arg2,offset1,offset2,rot_extra;
	string check_off,check_rot;
	vector <string> tokens;

	for (int i = 0; i < linha.length(); ++i)
	{
	 	linha[i] = toupper(linha[i]);
	}
	
	stringstream stream(linha);

	getline(stream,linha_sem_comentario,';');
		
	stringstream ss(linha_sem_comentario);

	if(linha_sem_comentario.find(':') != string::npos){
		getline(ss,rotulo,':');
		check_rot = ss.str().substr(ss.str().find(':'));
		check_rot.erase(0,1);
		if (check_rot.find(':') != string::npos)
		{
			getline(ss,rot_extra,':');
			while(rot_extra[0] == ' ' || rot_extra[0] == '\t')
			{
				rot_extra.erase(0,1);
			}
		}
	}	
	
	ss >> operacao;
	if(linha_sem_comentario.find(',') != string::npos){
		getline(ss, arg1,',');
		
		while(arg1[0] == ' ' || arg1[0] == '\t')
		{
			arg1.erase(0,1);
		}
		
		ss >> arg2; 
	}

	else {
		ss >> arg1;
	}

	stringstream ssarg1(arg1);
	if (arg1.find('+') != string::npos)
	{
		check_off = arg1.substr(arg1.find('+'));
		check_off.erase(0,1);
		if (check_off.find_first_not_of("0123456789") == string::npos)
		{
			getline(ssarg1, arg1,'+');
			ssarg1 >> offset1;
		}	
	}

	if (offset1.empty())
	{
		offset1 = "0";
	}

	stringstream ssarg2(arg2);
	if (arg2.find('+') != string::npos)
	{
		 
		check_off = arg2.substr(arg2.find('+'));
		check_off.erase(0,1);
		if (check_off.find_first_not_of("0123456789") == string::npos)
		{
			getline(ssarg2, arg2,'+');
			ssarg2 >> offset2;
		}	
	}

	if (offset2.empty())
	{
		offset2 = "0";
	}


	tokens.push_back(rotulo);
	tokens.push_back(operacao);
	tokens.push_back(arg1);
	tokens.push_back(arg2);
	tokens.push_back(offset1);
	tokens.push_back(offset2);
	tokens.push_back(rot_extra);
	return tokens;
}

auto atualiza_tab_simb(auto tokens, auto tab_op, auto tab_simb,int pc){
	bool encontrou_simb;
	int ind;
	vector <ItemTabelaDeSimbolos>::iterator it; 
	string simbolo;
	if (!tokens[0].empty())
	{
		simbolo = tokens[0];
		it = find_if(tab_simb.begin(),tab_simb.end(),[&simbolo](const ItemTabelaDeSimbolos tab_item){return tab_item.simb == simbolo;});
		ind = distance(tab_simb.begin(), it);
		encontrou_simb = (it != tab_simb.end());
		if (!encontrou_simb)
		{
			ItemTabelaDeSimbolos tab_item;
			tab_item.simb = tokens[0];
			
			if (pc == 0)
			{	
				tab_item.endr = -1; //codigo para endereço definido antes do trecho de codigo
				tab_item.def = false;
			}
			else
			{
				tab_item.endr = pc;
				tab_item.def = true;
			}
			tab_simb.push_back(tab_item);
		}
		else
		{	
			if (tab_simb[ind].def == false && (tokens[1] == "CONST" || tokens[1] == "SPACE"))
			{
				tab_simb[ind].def = true;
				tab_simb[ind].endr = pc;
			}
		}
	}
	if (!tokens[2].empty())
	{
		if (tokens[1] != "CONST" && tokens[1] != "SECTION" && tokens[1] != "SPACE")
		{
			simbolo = tokens[2];
			it = find_if(tab_simb.begin(),tab_simb.end(),[&simbolo](const ItemTabelaDeSimbolos tab_item){return tab_item.simb == simbolo;});
			ind = distance(tab_simb.begin(), it);
			encontrou_simb = (it != tab_simb.end());
			ItemTabelaDeSimbolos tab_item;
			if (!encontrou_simb)
			{
				tab_item.simb = tokens[2];
				tab_item.def = false;
				tab_item.lista_pendencia.push_back(pc + 1);
				tab_item.lista_pendencia_off.push_back(stoi(tokens[4]));
				tab_simb.push_back(tab_item);

			}
			else
			{
				if (tab_simb[ind].def == false)
				{
					tab_simb[ind].lista_pendencia.push_back((pc+1));
					tab_simb[ind].lista_pendencia_off.push_back(stoi(tokens[4]));
				}
			}
		}
	}
	if (!tokens[3].empty())
	{
		simbolo = tokens[3];
		it = find_if(tab_simb.begin(),tab_simb.end(),[&simbolo](const ItemTabelaDeSimbolos tab_item){return tab_item.simb == simbolo;});
		ind = distance(tab_simb.begin(), it);
		encontrou_simb = (it != tab_simb.end());
		ItemTabelaDeSimbolos tab_item;
		if (!encontrou_simb)
		{
			
			tab_item.simb = tokens[3];
			tab_item.def = false;
			tab_item.lista_pendencia.push_back(pc + 2);
			tab_item.lista_pendencia_off.push_back(stoi(tokens[5]));
			tab_simb.push_back(tab_item);
		}
		else
		{	
			if (tab_simb[ind].def == false)
			{
				tab_simb[ind].lista_pendencia.push_back(pc+2);
				tab_simb[ind].lista_pendencia_off.push_back(stoi(tokens[5]));
			}
		}
	}
	return tab_simb;
}

auto atualiza_tab_dados(auto tokens, auto tab_dados){
	if (!tokens[0].empty())
	{	
		ItemTabelaDeDados dados;
		if (tokens[1] == "CONST")
		{
			dados.tipo = tokens[1];
			dados.valor = stoi(tokens[2]);
			tab_dados[tokens[0]] = dados;
		}

		if (tokens[1] == "SPACE")
		{
			dados.tipo = tokens[1];
			if (!tokens[2].empty())
			{
				dados.valor = stoi(tokens[2]);
			}
			else
			{
				dados.valor = 1;
			}
			tab_dados[tokens[0]] = dados;
		}
	}
	return tab_dados;
}

auto corrige_endr_tab_simb(int pc, auto tab_simb, auto tab_dados){

	for (auto& i : tab_simb)
	{
		if (i.endr == -1)
		{
			i.endr = pc;
			i.def = true;
			if (tab_dados[i.simb].tipo == "SPACE")
			{
				pc += tab_dados[i.simb].valor;
			}
			else
			{
				pc++;
			}
		}
		else if (i.endr != -2)
		{
			auto it = find_if(tab_simb.begin(),tab_simb.end(),
				[&i](const ItemTabelaDeSimbolos tab_item){return ((tab_item.simb != i.simb) && (tab_item.endr == i.endr));});
			if (it != tab_simb.end())
			{
				i.endr = pc;
				if (tab_dados[i.simb].tipo == "SPACE")
				{
					pc += tab_dados[i.simb].valor;
				}
				else
				{
					pc++;
				}
			}
		}
	}
	return tab_simb;
}

auto gera_codigo_segmento_texto(auto codigo_gerado,auto tokens,auto tab_simb, auto tab_op){
	string operacao = tokens[1];
	bool encontrou_simb = false;
	int ind;
	int endr;
	string simbolo;
	if (!tab_op[operacao].opcode.empty()) 
	{	

		codigo_gerado.push_back(tab_op[operacao].opcode);
		int n_args = tab_op[operacao].n_args;
		for (int i = 0; i < n_args; ++i)
		{
			simbolo = tokens[i+2];
			auto it = find_if(tab_simb.begin(),tab_simb.end(),[&simbolo](const ItemTabelaDeSimbolos tab_item){return tab_item.simb == simbolo;});
			ind = distance(tab_simb.begin(), it);
			encontrou_simb = (it != tab_simb.end());
			if (encontrou_simb)
			{
				if (tab_simb[ind].endr >= 0)
				{
					endr = tab_simb[ind].endr + stoi(tokens[i+4]);
					codigo_gerado.push_back(to_string(endr));
				}
				else
				{
					codigo_gerado.push_back(to_string(tab_simb[ind].endr));
				}
			}
		}
	}
	return codigo_gerado;
}

auto corrige_endr_codigo_segmento_texto(auto codigo_gerado, auto tab_simb){
	int endr;
	for (auto it : tab_simb)
	{
		for (int i = 0; i < it.lista_pendencia.size(); i++)
		{
			endr = it.endr;
			if (i < it.lista_pendencia_off.size() && it.endr >= 0) 
			{
				endr += it.lista_pendencia_off[i];
			}
			codigo_gerado[it.lista_pendencia[i]] = to_string(endr);
		}
	}
	return codigo_gerado;	
}

auto gera_codigo_completo(auto codigo_gerado,auto tab_simb, auto tab_dados){
	for(auto it : tab_simb)
	{
		if (!tab_dados[it.simb].tipo.empty())
		{
			if (tab_dados[it.simb].tipo == "CONST")
			{
				codigo_gerado.push_back(to_string(tab_dados[it.simb].valor));
			}
			if (tab_dados[it.simb].tipo == "SPACE")
			{
				for (int i = 0; i < tab_dados[it.simb].valor ; ++i)
				{
					codigo_gerado.push_back("00");
				}
			}
		}
	}
	return codigo_gerado;
}

auto analisador_lexico(auto tokens, auto tab_erros, int linha){
	string caracteres_aceitos,caracteres_numericos;
	caracteres_aceitos = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890_";
	caracteres_numericos = "0123456789";
	bool teve_erro = false;
	string str1, str2;
	ItemTabelaDeErros erro;
	if (!tokens[0].empty())
	{
		if (tokens[0].size() > 50)
		{
			erro.label = tokens[0];
			erro.mensagem = "Erro Lexico (Label com  mais de 50 caracteres)";
			erro.linha = linha;
			tab_erros.push_back(erro);
		}
		else if (tokens[0].find_first_not_of(caracteres_aceitos) != string::npos)
		{
			erro.label = tokens[0];
			erro.mensagem = "Erro Lexico";
			erro.linha = linha;
			tab_erros.push_back(erro);
		}
		else if (isdigit(tokens[0][0]))
		{
			erro.label = tokens[0];
			erro.mensagem = "Erro Lexico";
			erro.linha = linha;
			tab_erros.push_back(erro);
		}
	}
	if (!tokens[1].empty())
	{
		if (tokens[1].size() > 50)
		{
			erro.label = tokens[1];
			erro.mensagem = "Erro Lexico (Label com  mais de 50 caracteres)";
			erro.linha = linha;
			tab_erros.push_back(erro);
		}
		else if (tokens[1].find_first_not_of(caracteres_aceitos) != string::npos)
		{
			erro.label = tokens[1];
			erro.mensagem = "Erro Lexico";
			erro.linha = linha;
			tab_erros.push_back(erro);
		}
		else if (isdigit(tokens[1][0]))
		{
			erro.label = tokens[1];
			erro.mensagem = "Erro Lexico";
			erro.linha = linha;
			tab_erros.push_back(erro);
		}
	}
	if (!tokens[2].empty())
	{
		if (tokens[2].size() > 50)
		{
			erro.label = tokens[2];
			erro.mensagem = "Erro Lexico (Label com  mais de 50 caracteres)";
			erro.linha = linha;
			tab_erros.push_back(erro);
		}
		else if (isdigit(tokens[2][0]))
		{
			if (tokens[1] == "CONST" || tokens[1] == "SPACE")
			{
				for (auto it : tokens[2])
				{
					if (!isdigit(it))
					{
						teve_erro = true;
					}
				}
				if (teve_erro)
				{
					erro.label = tokens[2];
					erro.mensagem = "Erro Lexico";
					erro.linha = linha;
					tab_erros.push_back(erro);
				}
			}
			else
			{
				erro.label = tokens[2];
				erro.mensagem = "Erro Lexico";
				erro.linha = linha;
				tab_erros.push_back(erro);
			}	
		}
		else
		{
		 
			stringstream sstoken(tokens[2]);
			getline(sstoken, str1,'+');
			sstoken >> str2;
			if ((str1.find_first_not_of(caracteres_aceitos) != string::npos) || (str2.find_first_not_of(caracteres_numericos) != string::npos))
			{
				
				erro.label = tokens[2];
				erro.mensagem = "Erro Lexico";
				erro.linha = linha;
				tab_erros.push_back(erro);
			}	
		}
	}
	if (!tokens[3].empty())
	{
		if (tokens[3].size() > 50)
		{
			erro.label = tokens[3];
			erro.mensagem = "Erro Lexico (Label com  mais de 50 caracteres)";
			erro.linha = linha;
			tab_erros.push_back(erro);
		}
		else if (isdigit(tokens[3][0]))
		{
			erro.label = tokens[3];
			erro.mensagem = "Erro Lexico";
			erro.linha = linha;
			tab_erros.push_back(erro);
		}
		else 
		{
		 
			stringstream sstoken(tokens[3]);
			getline(sstoken, str1,'+');
			sstoken >> str2;
			if ((str1.find_first_not_of(caracteres_aceitos) != string::npos) || (str2.find_first_not_of(caracteres_numericos) != string::npos))
			{
				
				erro.label = tokens[3];
				erro.mensagem = "Erro Lexico";
				erro.linha = linha;
				tab_erros.push_back(erro);

			}	
		}
	}
	if (!tokens[6].empty())
	{
		if (tokens[6].size() > 50)
		{
			erro.label = tokens[6];
			erro.mensagem = "Erro Lexico (Label com  mais de 50 caracteres)";
			erro.linha = linha;
			tab_erros.push_back(erro);
		}
		else if (tokens[6].find_first_not_of(caracteres_aceitos) != string::npos)
		{
			erro.label = tokens[6];
			erro.mensagem = "Erro Lexico";
			erro.linha = linha;
			tab_erros.push_back(erro);
		}
		else if (isdigit(tokens[6][0]))
		{
			erro.label = tokens[6];
			erro.mensagem = "Erro Lexico";
			erro.linha = linha;
			tab_erros.push_back(erro);
		}
	}
	return tab_erros;
}

auto analisador_sintatico(auto tokens, auto tab_erros,int linha,auto tab_op){
	string operacao = tokens[1], arg1 = tokens[2], arg2 = tokens[3];
	int n_args = 0;
	ItemTabelaDeErros erro;
	if (!operacao.empty())
	{
		if (tab_op[operacao].opcode.empty() &&  operacao != "CONST" && operacao != "SECTION" && operacao != "SPACE")
		{
			erro.label = tokens[1];
			erro.mensagem = "Erro Sintatico";
			erro.linha = linha;
			tab_erros.push_back(erro);
		}
		else 
		{
			if (!arg1.empty())
			{
				n_args++;
			}
			if (!arg2.empty())
			{
				n_args++;
			}
			if (operacao == "CONST")
			{
				if (n_args != 1)
				{
					erro.label = tokens[1];
					erro.mensagem = "Erro Sintatico";
					erro.linha = linha;
					tab_erros.push_back(erro);
				}
			}
			else if (operacao == "SECTION")
			{
				if (n_args != 1)
				{
					erro.label = tokens[1];
					erro.mensagem = "Erro Sintatico";
					erro.linha = linha;
					tab_erros.push_back(erro);
				}
				else if (tokens[2] != "TEXT" && tokens[2] != "DATA")
				{
					erro.label = tokens[2];
					erro.mensagem = "Erro Sintatico";
					erro.linha = linha;
					tab_erros.push_back(erro);
				}
			}
			else if (operacao == "SPACE")
			{
				if (n_args != 1)
				{
					erro.label = tokens[1];
					erro.mensagem = "Erro Sintatico";
					erro.linha = linha;
					tab_erros.push_back(erro);
				}
			}
			else if (n_args != tab_op[operacao].n_args)
			{
				erro.label = tokens[1];
				erro.mensagem = "Erro Sintatico";
				erro.linha = linha;
				tab_erros.push_back(erro);
			}
		}
	}
	return tab_erros;
}

auto analisador_semantico(auto tokens,auto tab_erros,int linha,auto tab_simb){
	ItemTabelaDeErros erro;
	vector<ItemTabelaDeSimbolos>:: iterator it;
	string rotulo = tokens[0], arg1 = tokens[2], arg2 = tokens[3];
	int ind;
	if (!tokens[6].empty())
	{
		erro.label = tokens[6];
		erro.mensagem = "Erro Semantico";
		erro.linha = linha;
		tab_erros.push_back(erro);
	}
	if (!arg1.empty())
	{
		it = find_if(tab_simb.begin(),tab_simb.end(),[&arg1](const ItemTabelaDeSimbolos tab_item){return tab_item.simb == arg1;});
		ind = distance(tab_simb.begin(), it);
		if (it != tab_simb.end())
		{ 
			if (!tab_simb[ind].def)
			{
				erro.label = arg1;
				erro.mensagem = "Endr";
				erro.linha = linha;
				tab_erros.push_back(erro);
			}
		}
	}
	if (!arg2.empty())
	{
		it = find_if(tab_simb.begin(),tab_simb.end(),[&arg2](const ItemTabelaDeSimbolos tab_item){return tab_item.simb == arg2;});
		ind = distance(tab_simb.begin(), it);
		if (it == tab_simb.end() || (!tab_simb[ind].def))
		{
			erro.label = arg2;
			erro.mensagem = "Endr";
			erro.linha = linha;
			tab_erros.push_back(erro);
		}
	}
	return tab_erros;
}

auto verifica_rotulo_duplicado(auto tokens,auto tab_erros,int linha,auto tab_simb){
	ItemTabelaDeErros erro;
	string rotulo = tokens[0];
	if (!rotulo.empty())
	{
		auto it = find_if(tab_simb.begin(),tab_simb.end(),[&rotulo](const ItemTabelaDeSimbolos tab_item){return tab_item.simb == rotulo;});
		int ind = distance(tab_simb.begin(), it);
		if (it != tab_simb.end() && (tab_simb[ind].def))
		{
			erro.label = rotulo;
			erro.mensagem = "Erro Semantico";
			erro.linha = linha;
			tab_erros.push_back(erro);
		}
	}
	return tab_erros;
}

auto corrige_rotulo_ausente(auto tab_erros,auto tab_simb){
	auto it =  begin(tab_erros);
	string label;
	while (it != end(tab_erros))
	{
		if ((*it).mensagem == "Endr")
		{
			label = (*it).label;
			auto it2 = find_if(tab_simb.begin(),tab_simb.end(),[&label](const ItemTabelaDeSimbolos tab_item){return tab_item.simb == label;});
			int ind = distance(tab_simb.begin(), it2);
			if (tab_simb[ind].def)
			{
				it = tab_erros.erase(it);
			}
			else
			{
				(*it).mensagem = "Erro Semantico";
				it++;
			}
		}
		else
		{
			it++;
		}
	}
	return tab_erros;
}

void gera_arquivo_obj(auto codigo_gerado, auto nome_arquivo){
	auto pos = nome_arquivo.find('.');
	string nome_obj = nome_arquivo.substr(0, pos);
	nome_obj = nome_obj + ".obj";
	ofstream arquivo_obj(nome_obj);
	for(auto it : codigo_gerado){
		arquivo_obj << it << ' ';
	}
	arquivo_obj.close();
}


int main(int argc, char* argv[]) {
    string nome_arquivo(argv[1]);
    ifstream arquivo(nome_arquivo);
    string linha;
	
	int pc = 0, contador_linha = 1;     
    vector <string> tokens, tokens_aux;
	
	map <string, ItemTabelaDeOperacoes> tab_op;
	map <string, ItemTabelaDeDados> tab_dados;
	vector <ItemTabelaDeSimbolos> tab_simb;
	vector <ItemTabelaDeErros> tab_erros;
	
	vector<string> codigo_gerado;
	
	tab_op = inicia_tabela_op();
    
    while (getline (arquivo, linha)) {
    	tokens  = pega_tokens(linha);
    	if ((!(tokens[0].empty()))&& (tokens[1].empty()) && (tokens[2].empty()) && (tokens[3].empty()))
    	{
    		getline(arquivo,linha);
    		tokens_aux = pega_tokens(linha);
    		for (int i = 1; i < tokens_aux.size(); ++i)
    		{
    			tokens[i] = tokens_aux[i];
    			contador_linha++;
    		}
    	}
    	tab_erros = verifica_rotulo_duplicado(tokens,tab_erros,contador_linha,tab_simb);
    	tab_simb = atualiza_tab_simb(tokens,tab_op,tab_simb,pc);
    	tab_dados = atualiza_tab_dados(tokens,tab_dados);
    	codigo_gerado = gera_codigo_segmento_texto(codigo_gerado,tokens,tab_simb,tab_op);
    	tab_erros = analisador_lexico(tokens,tab_erros,contador_linha);
    	tab_erros = analisador_sintatico(tokens,tab_erros,contador_linha,tab_op);
    	tab_erros = analisador_semantico(tokens,tab_erros,contador_linha,tab_simb);
    	pc += tab_op[tokens[1]].memoria; 
    	contador_linha++;
    	/*cout << "L: "<<  linha << endl;
		cout << endl;*/
		
    	/*cout << "Rot: " << tokens[0] +  '\n' <<"OP: " << tokens[1] +  '\n' <<
    	 "Arg1: " <<  tokens[2] +  '\n' <<"Arg2: " <<  tokens[3] +  '\n' <<
    	  "Off1: " <<  tokens[4] +  '\n'<< "Off2: " << tokens[5] +  '\n' << endl;*/
	}
	tab_simb = corrige_endr_tab_simb(pc,tab_simb,tab_dados);
	tab_erros = corrige_rotulo_ausente(tab_erros,tab_simb);
	codigo_gerado = corrige_endr_codigo_segmento_texto(codigo_gerado,tab_simb);
	codigo_gerado = gera_codigo_completo(codigo_gerado,tab_simb,tab_dados);
	gera_arquivo_obj(codigo_gerado,nome_arquivo);
	
	for (auto i: tab_simb)
	{
		cout << "Simbolo: " << i.simb << endl;
		cout << "Endereco: " << i.endr << endl;
		cout << "Definido: " << i.def << endl;
		cout << "Lista de Pendencia: ";
		for (auto j: i.lista_pendencia)
		{
			cout << "->" << j << " ";
		}
		cout << endl;
		cout << "Lista de Pendencia Offset: ";
		for (auto k: i.lista_pendencia_off)
		{
			cout << "->" << k << " ";
		}
		cout << endl;
		cout << endl;
	}
	for (auto it : tab_dados)
	{
		cout << "Rotulo: " << it.first << endl;
		cout << "Tipo: " << it.second.tipo << endl;
		cout << "Valor: " << it.second.valor << endl;
	}
	cout << endl << endl;
	cout <<"Codigo Gerado: " << endl;

	for(auto it : codigo_gerado){
		cout << it << ' ';
	}
	cout << endl << endl;
	for (auto it : tab_erros)
	{
		cout << "Rotulo:" << it.label << endl;
		cout << "Tipo de Erro:" << it.mensagem << endl;
		cout << "Linha:" << it.linha << endl;
		cout << endl;
	}
	arquivo.close();
    return 0;
}