#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <iterator> 
#include <limits>
using namespace std;

auto executa_instrucao(auto codigo_objeto,int* acc, int* pc,int* contador_linha, bool* terminar_execucao){
	int arg1,arg2;
	string entrada;
	switch (codigo_objeto[(*pc)]){
		case 1:
		cout << "Linha " << (*contador_linha) << endl;
		cout << "PC: " << (*pc) << endl;  
		(*pc)++;
		arg1 = codigo_objeto[(*pc)];
		(*acc) += codigo_objeto[arg1];
		cout << "ACC: " << (*acc) << endl; 
		(*pc)++;
		(*contador_linha)++;
		break;

		case 2:
		cout << "Linha " << (*contador_linha) << endl;
		cout << "PC: " << (*pc) << endl; 
		(*pc)++;
		arg1 = codigo_objeto[(*pc)];
		(*acc) -= codigo_objeto[arg1];
		cout << "ACC: " << (*acc) << endl; 
		(*pc)++;
		(*contador_linha)++;
		break;

		case 3:
		cout << "Linha " << (*contador_linha) << endl;
		cout << "PC: " << (*pc) << endl; 
		(*pc)++;
		arg1 = codigo_objeto[(*pc)];
		(*acc) *= codigo_objeto[arg1];
		cout << "ACC: " << (*acc) << endl; 
		(*pc)++;
		(*contador_linha)++;
		break;

		case 4:
		cout << "Linha " << (*contador_linha) << endl;
		cout << "PC: " << (*pc) << endl; 
		(*pc)++;
		arg1 = codigo_objeto[(*pc)];
		(*acc) /= codigo_objeto[arg1];
		cout << "ACC: " << (*acc) << endl; 
		(*pc)++;
		(*contador_linha)++;
		break;

		case 5:
		cout << "Linha " << (*contador_linha) << endl;
		cout << "PC: " << (*pc) << endl; 
		(*pc)++;
		arg1 = codigo_objeto[(*pc)];
		(*pc) = arg1;
		cout << "ACC: " << (*acc) << endl; 
		(*contador_linha)++;
		break;

		case 6:
		cout << "Linha " << (*contador_linha) << endl;
		cout << "PC: " << (*pc) << endl; 
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
		cout << "ACC: " << (*acc) << endl; 
		(*contador_linha)++;
		break;

		case 7:
		cout << "Linha " << (*contador_linha) << endl;
		cout << "PC: " << (*pc) << endl; 
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
		cout << "ACC: " << (*acc) << endl; 
		(*contador_linha)++;
		break;

		case 8:
		cout << "Linha " << (*contador_linha) << endl;
		cout << "PC: " << (*pc) << endl; 
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
		cout << "ACC: " << (*acc) << endl; 
		(*contador_linha)++;
		break;

		case 9:
		cout << "Linha " << (*contador_linha) << endl;
		cout << "PC: " << (*pc) << endl; 
		(*pc)++;
		arg1 = codigo_objeto[(*pc)];
		(*pc)++;
		arg2 = codigo_objeto[(*pc)];
		codigo_objeto[arg2] = codigo_objeto[arg1];
		cout << "ACC: " << (*acc) << endl; 
		(*pc)++;
		(*contador_linha)++;
		break;

		case 10:
		cout << "Linha " << (*contador_linha) << endl;
		cout << "PC: " << (*pc) << endl; 
		(*pc)++;
		arg1 = codigo_objeto[(*pc)];
		(*acc) = codigo_objeto[arg1];
		cout << "ACC: " << (*acc) << endl; 
		(*pc)++;
		(*contador_linha)++;
		break;

		case 11:
		cout << "Linha " << (*contador_linha) << endl;
		cout << "PC: " << (*pc) << endl; 
		(*pc)++;
		arg1 = codigo_objeto[(*pc)];
		codigo_objeto[arg1] = (*acc);
		cout << "ACC: " << (*acc) << endl; 
		(*pc)++;
		(*contador_linha)++;
		break;

		case 12:
		cout << "Linha " << (*contador_linha) << endl;
		cout << "PC: " << (*pc) << endl; 
		(*pc)++;
		arg1 = codigo_objeto[(*pc)];
		cout << "Insira Entrada: ";
		cin >> entrada;
		codigo_objeto[arg1] = stoi(entrada);
		cout << "ACC: " << (*acc) << endl; 
		(*pc)++;
		(*contador_linha)++;
		break;

		case 13:
		cout << "Linha " << (*contador_linha) << endl;
		cout << "PC: " << (*pc) << endl; 
		(*pc)++;
		arg1 = codigo_objeto[(*pc)];
		cout << "ACC: " << (*acc) << endl;
		cout << "OUTPUT: " << codigo_objeto[arg1] << endl;
		cout << "Pressione ENTER para Continuar" << endl;
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		while(cin.get() != '\n')
		{
			cout << "Pressione ENTER para Continuar" << endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
		(*pc)++;
		(*contador_linha)++;
		break;

		case 14:
		cout << "Linha " << (*contador_linha) << endl;
		cout << "PC: " << (*pc) << endl; 
		(*pc)++;
		(*terminar_execucao) = true;
		cout << "ACC: " << (*acc) << endl; 
		(*contador_linha)++;
		break;
	}
return codigo_objeto;
}


int main(int argc, char const *argv[])
{
	string nome_arquivo(argv[1]);
	ifstream arquivo(nome_arquivo);
	string linha, token;
	vector<int> codigo_objeto;
	int pc = 0,acc = 0, contador_linha = 1;
	bool terminar_execucao = false;
	
	getline (arquivo, linha);
	stringstream ss(linha);
	while(ss >> token)
	{
		codigo_objeto.push_back(stoi(token));
	}
	while((pc < codigo_objeto.size() - 1) && (terminar_execucao == false))
	{
		codigo_objeto = executa_instrucao(codigo_objeto, &acc, &pc, &contador_linha, &terminar_execucao);
		cout << endl;
	}

	return 0;
}