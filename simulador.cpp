#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <iterator> 
#include <map>
using namespace std;

auto executa_instrucao(auto codigo_objeto,int* acc, int* pc, bool* terminar_execucao){
	int arg1,arg2;
	string entrada;
	switch (codigo_objeto[(*pc)]){
		case 1: 
		(*pc)++;
		arg1 = codigo_objeto[(*pc)];
		(*acc) += codigo_objeto[arg1];
		(*pc)++;
		break;

		case 2:
		(*pc)++;
		arg1 = codigo_objeto[(*pc)];
		(*acc) -= codigo_objeto[arg1]; 
		(*pc)++;
		break;

		case 3:
		(*pc)++;
		arg1 = codigo_objeto[(*pc)];
		(*acc) *= codigo_objeto[arg1];
		(*pc)++;
		break;

		case 4:
		(*pc)++;
		arg1 = codigo_objeto[(*pc)];
		(*acc) /= codigo_objeto[arg1];
		(*pc)++;
		break;

		case 5:
		(*pc)++;
		arg1 = codigo_objeto[(*pc)];
		(*pc) = arg1;
		break;

		case 6:
		(*pc)++;
		arg1 = codigo_objeto[(*pc)];
		if ((*acc) < 0)
		{
			(*pc) = arg1;
		}
		else
		{
			(*pc)++;
		}
		break;

		case 7:
		(*pc)++;
		arg1 = codigo_objeto[(*pc)];
		if ((*acc) > 0)
		{
			(*pc) = arg1;
		}
		else
		{
			(*pc)++;
		}
		break;

		case 8:
		(*pc)++;
		arg1 = codigo_objeto[(*pc)];
		if ((*acc) == 0)
		{
			(*pc) = arg1;
		}
		else
		{
			(*pc)++;
		}
		break;

		case 9: 
		(*pc)++;
		arg1 = codigo_objeto[(*pc)];
		(*pc)++;
		arg2 = codigo_objeto[(*pc)];
		codigo_objeto[arg2] = codigo_objeto[arg1];
		(*pc)++;
		break;

		case 10: 
		(*pc)++;
		arg1 = codigo_objeto[(*pc)];
		(*acc) = codigo_objeto[arg1];
		(*pc)++;
		break;

		case 11:
		(*pc)++;
		arg1 = codigo_objeto[(*pc)];
		codigo_objeto[arg1] = (*acc); 
		(*pc)++;
		break;

		case 12: 
		(*pc)++;
		arg1 = codigo_objeto[(*pc)];
		cout << "Insira Entrada: ";
		cin >> entrada;
		codigo_objeto[arg1] = stoi(entrada); 
		(*pc)++;
		break;

		case 13:
		(*pc)++;
		arg1 = codigo_objeto[(*pc)];
		cout << "OUTPUT: " << codigo_objeto[arg1] << endl;
		cout << "Pressione ENTER para Continuar" << endl;
		cin.clear();
		cin.get();
		(*pc)++;
		break;

		case 14:
		(*pc)++;
		(*terminar_execucao) = true;
		break;
	}
	return codigo_objeto;
}

auto cria_mapa_de_linhas(string linha, auto mapa_de_linhas){
	int pc  = 0, contador_linha = 1;
	string token;
	stringstream ss(linha);
	while(ss >> token && token != "00"){
		if (mapa_de_linhas.find(pc) == mapa_de_linhas.end())
		{
			mapa_de_linhas[pc] = contador_linha;
		}
		if (token  == "14")
		{
			pc++;	
		}
		else if (token == "09")
		{
			ss >> token;
			ss >> token;
			pc += 3;
		}
		else 
		{
			ss >> token;
			pc += 2;
		}
		contador_linha++;
	}
	return mapa_de_linhas;
}

int main(int argc, char const *argv[])
{
	string nome_arquivo(argv[1]);
	ifstream arquivo(nome_arquivo);
	string linha, token;
	
	vector<int> codigo_objeto;
	int pc = 0,acc = 0;
	bool terminar_execucao = false;

	map<int, int> mapa_de_linhas;
	
	getline (arquivo, linha);
	stringstream ss(linha);
	mapa_de_linhas = cria_mapa_de_linhas(linha,mapa_de_linhas);
	
	while(ss >> token)
	{
		codigo_objeto.push_back(stoi(token));
	}
	for (auto it : mapa_de_linhas)
	{
		cout << "PC: " << it.first << "--> " << "Linha: " << it.second << endl;
	}
	cout << endl;
	while((pc < codigo_objeto.size() - 1) && (terminar_execucao == false))
	{
		cout << "Linha " << mapa_de_linhas[pc] << ':'<<  endl;
		cout << "PC: " << pc << endl;
		cout << "ACC: " << acc << endl;  
		cout << endl;
		codigo_objeto = executa_instrucao(codigo_objeto, &acc, &pc, &terminar_execucao);
	}
	arquivo.close();
	return 0;
}