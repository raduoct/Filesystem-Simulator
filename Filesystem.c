#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Dir {
	char* name;   // numele directorului
	struct Dir* parent;  // pointer catre parintele directorului(null pentru radacina)
	struct File* head_children_files;  // pointer catre primul element de tip File din interiorul directorului
	struct Dir* head_children_dirs; // pointer catre primul element de tip Dir din interiorul directorului
	struct Dir* next; // pointer catre urmatorul element din lista in care se afla directorul
}Dir; // structura de tip director

typedef struct File {
	char* name;  // numele fisierului
	struct Dir* parent; // pointer catre directorul pe
	struct File* next; // pointer catre urmatorul element din lista de fisiere
}File; // structura de tip fisier

// functie in care imi construiesc directorul home
Dir* init() {
	Dir* root = (Dir*)malloc(sizeof(Dir));
	root->name = (char*)malloc(sizeof(char) * 5);
	strcpy(root->name,"home");
	root->parent = NULL;
	root->head_children_files = NULL;
	root->head_children_dirs = NULL;
	root->next = NULL;
	return root;
}
// functie ce imi creeaza un file
void touch(Dir* parent, char* name) {
	File* tmp = parent->head_children_files;
	//cazul in care trebuie creat primul element file din lista de fileuri
	if (tmp == NULL) {
		File* new_f = (File*)malloc(sizeof(File));
		new_f->name = (char*)malloc(strlen(name)+1);
		strcpy(new_f->name, name);
		new_f->next = NULL;
		new_f->parent = parent;
		parent->head_children_files = new_f;
	}
	else
	{	// parcurg lista de fileuri oprindu-ma si afisand mesajul corespunzator daca exista deja un file cu numele dat ca argument
		while (tmp->next != NULL) {
			if (strcmp(name, tmp->name) == 0) {
				printf("File already exists\n");
			}
			tmp = tmp->next;
		}
		if (tmp->next == NULL) {
		//verific ultimul element
			if (strcmp(name, tmp->name) == 0) {
				printf("File already exists\n");
			}
			//adaug la finalul listei noul file deoarece am parcurs toata lista si nu exista alt fisier cu numele dat
			else {
				File* new_f = (File*)malloc(sizeof(File));
				new_f->name = (char*)malloc(strlen(name)+1);
				strcpy(new_f->name, name);
				new_f->next = NULL;
				new_f->parent = parent;
				tmp->next = new_f;
			}
		}
	}
}
// functie ce creeaza un director
void mkdir(Dir* parent, char* name) {
	Dir* tmp = parent->head_children_dirs;
	//cazul in care trebuie creat primul element director din lista de directoare
	if (tmp == NULL) {
		Dir* new_d = (Dir*)malloc(sizeof(Dir));
		new_d->name = (char*)malloc(strlen(name) + 1);
		strcpy(new_d->name, name);
		new_d->next = NULL;
		new_d->parent = parent;
		new_d->head_children_dirs = NULL;
		new_d->head_children_files = NULL;
		parent->head_children_dirs = new_d;
	}
	else {
	// parcurg lista de fileuri oprindu-ma si afisand mesajul corespunzator daca exista deja un director cu numele dat ca argument
		while (tmp->next != NULL) {
			if (strcmp(name, tmp->name) == 0) {
				printf("Directory already exists\n");
			}
			tmp = tmp->next;
		}
		if (tmp->next == NULL) {
		//verific ultimul element
			if (strcmp(name, tmp->name) == 0) {
				printf("Directory already exists\n");
			}
			else {
			//adaug la finalul listei noul director deoarece am parcurs toata lista si nu exista alt director cu numele dat
				Dir* new_d = (Dir*)malloc(sizeof(Dir));
				new_d->name = (char*)malloc(strlen(name) + 1);
				strcpy(new_d->name, name);
				new_d->next = NULL;
				new_d->parent = parent;
				new_d->head_children_dirs = NULL;
				new_d->head_children_files = NULL;
				tmp->next = new_d;
			}
		}
	}
}
// functie ce imi afieaza fileurile si directoarele dintr-un director
// parcurg mai intai lista de directoare, dupa care pe cea de fileuri, afisand numele fiecaturi director/file
void ls(Dir* parent) {
	Dir* tmp = parent->head_children_dirs;
	while (tmp != NULL) {
		printf("%s\n", tmp->name);
		tmp = tmp->next;
	}
	File* tmp1 = parent->head_children_files;
	while (tmp1 != NULL) {
		printf("%s\n", tmp1->name);
		tmp1 = tmp1->next;
	}
}
// functie ce imi sterge un fisier din directorul curent
void rm(Dir* parent, char* name) {
	// verific daca lista de fisiere nu are niciun element, astfel neexistand fisierul pe care doresc sa il sterg
	if (parent->head_children_files == NULL) {
		printf("Could not find the file\n");
	}
	else {
	//verific headul listei, in cazul in care este fileul dorit sters, eliberez memoria corespunzator si noul head devine cel de al doilea element al listei
	// verific separat headul deoarece pentru celelalte elemente trebuie sa verific cu un element in fata pentru a putea conecta celelalte elemente ale listei
	// corespunzator in cazul in care sterg elementul dorit sa fie sters, de asemenea trebuie sa intorc noul head corespunzator
		if (strcmp(name, parent->head_children_files->name) == 0) {
			File* aux = parent->head_children_files;
			parent->head_children_files = parent->head_children_files->next;
			free(aux->name);
			free(aux);
		}
		else {
			File* tmp = parent->head_children_files;
			int ok = 1;
			// parcurg lista iar daca doresc fileul dorit sters, il sterg corespunzator si ma opresc din parcurs
			// mereu ma uit cu un element in fata, in cazul in care trebuie sters elementul, sa pot conecta elementele listei corespunzator
			while (tmp->next != NULL) {
				if (strcmp(name, tmp->next->name) == 0) {
					File* aux = tmp->next;
					tmp->next = tmp->next->next;
					free(aux->name);
					free(aux);
					ok = 0;
					break;
				}
				tmp = tmp->next;
			}
			//ajuns la finalul listei inseamna ca nu exista un file ce trebuie sters
			if (ok)
				printf("Could not find the file\n");
		}
	}
}
// functie ce imi elibereaza recursiv un director
void freedir(Dir* parent) {
	//cazul de baza in care ma opresc din apelurile recursive
	if (parent == NULL) {
		return;
	}
	//eliberez numele directorului
	free(parent->name);
	File* tmp = parent->head_children_files;
	//eliberez lista de fileuri
	while (tmp) {
		free(tmp->name);
		File* aux = tmp;
		tmp = tmp->next;
		free(aux);
	}
	Dir* tmp1 = parent->head_children_dirs;
	//apelez recursiv pentru fiecare director copil freedir
	while (tmp1) {
		Dir* aux1 = tmp1;
		tmp1 = tmp1->next;
		freedir(aux1);
	}
	// din intoarcerea din apelul recursiv tot ce ramane de eliberat este directorul parinte
	free(parent);
}
// functie ce imi sterge un director din directorul curent
void rmdir(Dir* parent, char* name) {
	// verific daca lista de fisiere nu are niciun element, astfel neexistand fisierul pe care doresc sa il sterg
	if (parent->head_children_dirs == NULL) {
		printf("Could not find the dir\n");
	}
	else {
		//verific headul listei, in cazul in care este directorul dorit sters, eliberez memoria corespunzator si noul head devine cel de al doilea element al listei
		// verific separat headul deoarece pentru celelalte elemente trebuie sa verific cu un element in fata pentru a putea conecta celelalte elemente ale listei
		// corespunzator in cazul in care sterg elementul dorit sa fie sters, de asemenea trebuie sa intorc noul head corespunzator
		if (strcmp(name, parent->head_children_dirs->name) == 0) {
			Dir* aux = parent->head_children_dirs;
			parent->head_children_dirs = parent->head_children_dirs->next;
			freedir(aux);
		}
		else {
			// parcurg lista iar daca doresc directorul dorit sters, il sterg corespunzator si ma opresc din parcurs
			// mereu ma uit cu un element in fata, in cazul in care trebuie sters elementul, ca sa pot conecta elementele listei corespunzator
			Dir* tmp = parent->head_children_dirs;
			int ok = 1;
			while (tmp->next != NULL) {
				if (strcmp(name, tmp->next->name) == 0) {
					Dir* aux = tmp->next;
					tmp->next = tmp->next->next;
					freedir(aux);
					ok = 0;
					break;
				}
				tmp = tmp->next;
			}
			//ajuns la finalul listei inseamna ca nu exista un director ce trebuie sters
			if (ok)
				printf("Could not find the dir\n");
		}
	}
}
// functie ce imi intoarce prin parametru noul director in care doresc sa ma mut
void cd(Dir** target, char* name) {
	//cazul in care doresc sa ma mut in directorul parinte
	if (strcmp(name, "..") == 0) {
		// verific directorul parinte sa nu fie NULL, adica daca sunt in directorul home, sa imi intoarca tot directorul home, director ce este
		// radacina ierarhiei de fisiere si directoare
		if((*target)->parent)
			(*target) = (*target)->parent;
	}
	else {
		Dir* tmp = (*target)->head_children_dirs;
		while (tmp) {
			//gasit directorul copil cautat ma mut in el si ma opresc din parcurs lista de directoare copil
			if (strcmp(tmp->name, name) == 0) {
				(*target) = tmp;
				break;
			}
			tmp = tmp->next;
		}
		// daca directorul nu are niciun copil director afisez mesajul corespunzator
		if (tmp == NULL) {
			printf("No directories found!\n");
		}
	}
}
// realizeaza afisarea ierarhiei de fisiere si directoare a directorului curent
void tree(Dir* target, int level) {
	//la nivelul 0 nu trebuie afisate spatii
	if(level != 0){
		for (int i = 0; i < 4*(level-1); i++)
			printf(" ");
		printf("%s\n", target->name);
	}
	Dir* tmp = target->head_children_dirs;
	while (tmp) {
		tree(tmp,level+1);
		tmp = tmp->next;
	}
	File* tmp1 = target->head_children_files;
	while (tmp1) {
		for (int i = 0; i < 4*level; i++)
			printf(" ");
		printf("%s\n", tmp1->name);
		tmp1 = tmp1->next;
	}
}
// functie recursiva ce imi intoarce calea directorului curent
char* pwd(Dir* target) {
	if (strcmp(target->name, "home") == 0) {
		char* sir;
		sir = (char*)malloc(500*sizeof(char));
		strcpy(sir,"/home");
		sir[5] = 0;
		return sir;
	}
	return strcat(strcat(pwd(target->parent),"/"),target->name);
}
// functie ce imi elibereaza memoria folosita in program
void stop(Dir* target) {
	while(strcmp(target->name, "home") != 0){
	    target = target->parent;
	}
	freedir(target);
}
// functie ce verifica daca exista un file/director cu numele newname
int mvexists(Dir* parent, char* newname) {
	File* tmp = parent->head_children_files;
	if (tmp != NULL){
		if (strcmp(tmp->name, newname) == 0)
		    return 1;
		while (tmp->next) {
			if (strcmp(tmp->next->name, newname) == 0)
			    return 1;
			tmp = tmp->next;
		}
	}
	Dir* tmp1 = parent->head_children_dirs;
	if (tmp1 != NULL){
		if (strcmp(tmp1->name, newname) == 0)
		    return 1;
		while (tmp1->next) {
			if (strcmp(tmp1->next->name, newname) == 0)
			    return 1;
		tmp1 = tmp1->next;
		}
	}
	return 0;
}
//functie ce realizeaza mv ul pentru un file
int mvfiles(Dir* parent, char* oldname, char* newname) {
	//variabila okold este setata pe 1 daca gasesc fileul oldname
	int okold = 0;
	File* tmp = parent->head_children_files;
	//in variabila found retin elementul dinainte de fileul oldname, daca exista
	File* found = NULL;
	if (tmp == NULL)
		return 0;
	if (strcmp(tmp->name, oldname) == 0)
		okold = 1;
	while (tmp->next) {
		if (strcmp(tmp->next->name, oldname) == 0) {
			okold = 1;
			found = tmp;
		}
		tmp = tmp->next;
	}
	if (okold && mvexists(parent,newname))
		return -1;
	if (okold) {
		// singurul element din lista, doar ii schimbam numele
		if (strcmp(parent->head_children_files->name, oldname) == 0 && parent->head_children_files->next == NULL) {
			strcpy(parent->head_children_files->name, newname);
			return 1;
		}
		// lista compusa doar din 2 elemente
		if (strcmp(parent->head_children_files->name, oldname) == 0 && parent->head_children_files->next->next == NULL) {
			File* aux = parent->head_children_files;
			parent->head_children_files = parent->head_children_files->next;
			parent->head_children_files->next = aux;
			aux->next = NULL;
			strcpy(aux->name, newname);
			return 1;
		}
		// parcurgem lista pentru a ajunge la ultimul element 
		File* temp = parent->head_children_files;
		while (temp->next) {
			temp = temp->next;
		}
		//cazul in care trebuie schimbat headul listei
		if (strcmp(parent->head_children_files->name, oldname) == 0) {
			File* aux = parent->head_children_files;
			parent->head_children_files = parent->head_children_files->next;
			temp->next = aux;
			aux->next = NULL;
			strcpy(aux->name, newname);
			return 1;
		}
		if (found->next->next == NULL) {
			strcpy(found->next->name, newname);
			return 1;
		}
		File* aux = found->next;
		found->next = found->next->next;
		temp->next = aux;
		aux->next = NULL;
		strcpy(aux->name, newname);
		return 1;
	}
	return 0;
}
//functie ce realizeaza mv ul pentru un director
int mvdir(Dir* parent, char* oldname, char* newname) {
	//variabila okold este setata pe 1 daca gasesc directorul oldname
	int okold = 0;
	Dir* tmp = parent->head_children_dirs;
	//in variabila found retin elementul dinainte de directorul oldname, daca exista
	Dir* found = NULL;
	if (tmp == NULL)
		return 0;
	if (strcmp(tmp->name, oldname) == 0)
		okold = 1;
	while (tmp->next) {
		if (strcmp(tmp->next->name, oldname) == 0) {
			okold = 1;
			found = tmp;
		}
		tmp = tmp->next;
	}
	if (okold && mvexists(parent,newname))
		return -1;
	if (okold) {
		// singurul element din lista, doar ii schimbam numele
		if (strcmp(parent->head_children_dirs->name, oldname) == 0 && parent->head_children_dirs->next == NULL) {
		//	#pragma warning(suppress : 4996)
			strcpy(parent->head_children_dirs->name, newname);
			return 1;
		}
		// lista compusa doar din 2 elemente
		if (strcmp(parent->head_children_dirs->name, oldname) == 0 && parent->head_children_dirs->next->next == NULL) {
			Dir* aux = parent->head_children_dirs;
			parent->head_children_dirs = parent->head_children_dirs->next;
			parent->head_children_dirs->next = aux;
			aux->next = NULL;
			strcpy(aux->name, newname);
			return 1;
		}
		// parcurgem lista pentru a ajunge la ultimul element 
		Dir* temp = parent->head_children_dirs;
		while (temp->next) {
			temp = temp->next;
		}
		//cazul in care trebuie schimbat headul listei
		if (strcmp(parent->head_children_dirs->name, oldname) == 0) {
			Dir* aux = parent->head_children_dirs;
			parent->head_children_dirs = parent->head_children_dirs->next;
			temp->next = aux;
			aux->next = NULL;
			strcpy(aux->name, newname);
			return 1;
		}
		if (found->next->next == NULL) {
			strcpy(found->next->name, newname);
			return 1;
		}
		Dir* aux = found->next;
		found->next = found->next->next;
		temp->next = aux;
		aux->next = NULL;
		strcpy(aux->name, newname);
		return 1;
	}
	return 0;
}
//functie ce schimba numele unui file/director
void mv(Dir* parent, char* oldname, char* newname) {
	 if (mvdir(parent, oldname, newname) == 0 && mvfiles(parent, oldname, newname) == 0) {
		 printf("File/Director not found\n");
	 }
	 else if (mvdir(parent, oldname, newname) == -1 || mvfiles(parent, oldname, newname) == -1) {
		 printf("File/Director already exists\n");

	 }
}
int main() {
	char buffer[100], arg1[100], arg2[100];
	Dir* root = init();
	scanf("%s", buffer);
	while (strcmp(buffer, "stop") != 0) {
		if (strcmp(buffer, "mv") == 0) {
			scanf("%s%s", arg1, arg2);
			mv(root, arg1, arg2);
		}
		else if (strcmp(buffer, "ls") == 0) {
			ls(root);
		}
		else if (strcmp(buffer, "tree") == 0) {
			tree(root, 0);
		}
		else if (strcmp(buffer, "pwd") == 0) {
			char* aux;
			aux = pwd(root);
			printf("%s\n", aux);
			//eliberez manual memoria alocata in functia pwd
			free(aux);
		}
		else {
			if (strcmp(buffer, "touch") == 0) {
				scanf("%s", arg1);
				touch(root, arg1);
			}
			if (strcmp(buffer, "mkdir") == 0) {
				scanf("%s", arg1);
				mkdir(root, arg1);
			}
			if (strcmp(buffer, "rm") == 0) {
				scanf("%s", arg1);
				rm(root, arg1);
			}
			if (strcmp(buffer, "rmdir") == 0) {
				scanf("%s", arg1);
				rmdir(root, arg1);
			}
			if (strcmp(buffer, "cd") == 0) {
				scanf("%s", arg1);
				cd(&root, arg1);
			}

		}
		scanf("%s", buffer);
	}
	stop(root);
	return 0;
}
