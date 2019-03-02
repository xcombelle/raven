# include <stdio.h>
# include <string.h>

struct position {
	int epsquare;
	char board[65];
	int WcastleQS;
	int WcastleKS; 
	int BcastleKS;
	int BcastleQS;
	int tomove;
	int Wkingpos;
	int Bkingpos;
};

void dspboard(struct position pos) {
	char piece;
	char board[65];
	strcpy(board,pos.board);
	printf("\n");
	printf("  +---+---+---+---+---+---+---+---+\n");
	printf("8 |");
	for (int i = 0;i<64;i++) {
		if ( ((i%8) == 0) && (i != 0) ) {
			printf("\n");
			printf("  +---+---+---+---+---+---+---+---+\n");
			printf("%d",(8 - i/8));
			printf(" |");
		}
		piece = board[i];
		if (piece == '0') piece = ' ';
		printf(" %c |", piece);
	}
	printf("\n  +---+---+---+---+---+---+---+---+");
	printf("\n    A   B   C   D   E   F   G   H  \n");
	printf("Side to move: ");
	if (pos.tomove == 0) printf("Black");
	else printf("White");
	printf("\n");
}

int isCheck(char square[]) {
	// check if a king on square is in check
//	int Ndirs[8][2];
//	Ndirs = {{-2,-1}};
}
/*
void makeMove(char move[]) {
	// makes move (e.g. e2e4)
	char startsquare[3];
	char endsquare[3];
	char prompiece[2];
	startsquare[0] = move[0];
	startsquare[1] = move[1];
	startsquare[2] = 0;
	endsquare[0] = move[2];
	endsquare[1] = move[3];
	endsquare[2] = 0;
	prompiece[0] = move[4];
	prompiece[1] = 0;
	printf("%s%s",startsquare,endsquare);
}
 */
void makeMove(struct position pos) {
	printf("%d",pos.epsquare);
}
int main() {
	char instr[100];
	char splitstr[100][100];
	char * tokstr;
	struct position pos = {.epsquare=-1,.board={'r','n','b','q','k','b','n','r',
					'p','p','p','p','p','p','p','p',
					'0','0','0','0','0','0','0','0',
					'0','0','0','0','0','0','0','0',
					'0','0','0','0','0','0','0','0',
					'0','0','0','0','0','0','0','0',
					'P','P','P','P','P','P','P','P',
					'R','N','B','Q','K','B','N','R'},.WcastleQS=1,.WcastleKS=1,.BcastleQS=1,.BcastleKS=1,
					.tomove=1,.Wkingpos=60,.Bkingpos=4};
	while (1) {
		fgets(instr, 100, stdin);
		tokstr = strtok(instr," ");
		printf("%s",tokstr);
		//if (strcmp(splitstr[0],"jam")) printf("hi");
		if (strcmp(instr,"uci\n") == 0) printf("id name Raven 0.30\nid author JimmyRustles\nuciok\n");
		if (strcmp(instr,"board\n") == 0) dspboard(pos);
		if (strcmp(instr,"move\n") == 0) makeMove(pos);
	}
		
}