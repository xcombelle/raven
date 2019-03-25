int pieceval(char inpiece) {
	if (inpiece == 'p') return 100;
	if (inpiece == 'n') return 300;
	if (inpiece == 'b') return 300;
	if (inpiece == 'r') return 500;
	if (inpiece == 'q') return 900;
	if (inpiece == 'k') return 9999;
	if (inpiece == 'P') return 100;
	if (inpiece == 'N') return 300;
	if (inpiece == 'B') return 300;
	if (inpiece == 'R') return 500;
	if (inpiece == 'Q') return 900;
	if (inpiece == 'K') return 9999;
	assert(0);
	printf("inpiece: %d\n",inpiece);
	return 0;
}
int evalBoard(struct position *pos) {
	int score = 0;
	for (int i = 0;i<64;i++) {
		char piece = pos->board[i];
		if (piece != '0') {
			int pval = pieceval(piece);
			if ((piece >= 'a') && (piece <= 'z')) {
				pval = -pval;
			}
			int pstscore = PSTval(piece,i);
			score += pstscore;
			score += pval;
		}
	}
	if (pos->tomove == BLACK) return -score;
	return score;
}
int negaMax(struct position *pos,int depth,int timeLeft) {
	assert(depth >= 0);
	nodesSearched++;
	if (depth == 0) {
		return evalBoard(pos);
	}
	int kingpos;
	struct move moves[MAX_MOVES];
	int maxScore = -9999;
	int num_moves = genLegalMoves(pos,moves);
	clock_t begin = clock();
	int timeElapsed = 0;
	/*
	if (num_moves == 0) {
		if (pos->tomove == WHITE) kingpos = pos->Wkingpos;
		if (pos->tomove == BLACK) kingpos = pos->Bkingpos;
		int incheck = isCheck(pos,kingpos);
		if (incheck) {
			return -9999;
		}
		else {
			return 0;
		}
	}
	 */
	int numcheckmoves = 0;
	for (int i = 0;(i < num_moves && timeElapsed == 0);i++) {
		clock_t end = clock();
		double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
		int time_spentms = (int)(time_spent * 1000);
		if (time_spentms >= timeLeft) {
			timeElapsed = 1;
			break;
		}
		makeMove(&moves[i],pos);
		pos->tomove = !pos->tomove;
		if (pos->tomove == WHITE) kingpos = pos->Wkingpos;
		if (pos->tomove == BLACK) kingpos = pos->Bkingpos;
		int incheck = isCheck(pos,kingpos);
		if (incheck) {
			unmakeMove(pos);
			numcheckmoves++;
			continue;
		}
		pos->tomove = !pos->tomove;
		int score = -negaMax(pos,depth - 1, (timeLeft - time_spentms));
		unmakeMove(pos);
		if (score > maxScore) {
			maxScore = score;
		}
	}
	//printf("%d - %d\n",num_moves,numcheckmoves);
	if (num_moves == numcheckmoves) {
		// no legal moves
		if (pos->tomove == WHITE) kingpos = pos->Wkingpos;
		if (pos->tomove == BLACK) kingpos = pos->Bkingpos;
		int incheck = isCheck(pos,kingpos);
		if (incheck) {
			// side to move is in checkmate
			return -99999;
		}
		else {
			// stalemate
			return 0;
		}
	}
	if (isThreefold(pos)) return 0;
	if (pos->halfmoves >= 100) return 0;
	return maxScore;
}
int qSearch(struct position *pos, int alpha, int beta, int timeLeft) {
	int incheck;
	int score;
	int kingpos;
	struct move moves[MAX_MOVES];
	int standpat = evalBoard(pos);
	if (standpat >= beta) {
		nodesSearched++;
		return beta;
	}
	if (alpha < standpat) alpha = standpat;
	int num_moves = genLegalMoves(pos,moves);
	clock_t begin = clock();
	int timeElapsed = 0;
	for (int i = 0;(i < num_moves && timeElapsed == 0);i++) {
		clock_t end = clock();
		double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
		int time_spentms = (int)(time_spent * 1000);
		if (time_spentms >= timeLeft) {
			timeElapsed = 1;
			break;
		}
		int iscap = 0;
		if (pos->board[moves[i].to] != '0') iscap = 1;
		makeMove(&moves[i],pos);
		// check if move is legal (doesn't put in check)
		pos->tomove = !pos->tomove;
		if (pos->tomove == WHITE) kingpos = pos->Wkingpos;
		if (pos->tomove == BLACK) kingpos = pos->Bkingpos;
		incheck = isCheck(pos,kingpos);
		if (incheck) {
			unmakeMove(pos);
			continue;
		}
		pos->tomove = !pos->tomove;
		if (pos->tomove == WHITE) kingpos = pos->Wkingpos;
		if (pos->tomove == BLACK) kingpos = pos->Bkingpos;
		incheck = isCheck(pos,kingpos);
		if ((incheck) || (iscap)) {
			//printf("%s\n",movetostr(moves[i]));
			//printf("%d %d %d\n",-beta,-alpha,(time_spentms - timeLeft));
			score = -qSearch(pos,-beta,-alpha, (time_spentms - timeLeft));
			unmakeMove(pos);
			nodesSearched++;
			if (score >= beta) return beta;
			if (score > alpha) alpha = score;
		}
		else {
			unmakeMove(pos);
			continue;
		}
	}
	return alpha;
}
int alphaBeta(struct position *pos, int alpha, int beta, int depthleft, int timeLeft) {
	if (timeLeft <= 0) {
		return alpha;
	}
	nodesSearched++;
	if (isThreefold(pos)) return 0;
	if (pos->halfmoves >= 100) return 0;
	if (depthleft == 0) {
		return qSearch(pos,alpha,beta,timeLeft);
		//return evalBoard(pos);
	}
	int kingpos;
	struct move moves[MAX_MOVES];
	int num_moves = genLegalMoves(pos,moves);
	clock_t begin = clock();
	int timeElapsed = 0;
	int numcheckmoves = 0;
	for (int i = 0;(i < num_moves && timeElapsed == 0);i++) {
		clock_t end = clock();
		double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
		int time_spentms = (int)(time_spent * 1000);
		makeMove(&moves[i],pos);
		pos->tomove = !pos->tomove;
		if (pos->tomove == WHITE) kingpos = pos->Wkingpos;
		if (pos->tomove == BLACK) kingpos = pos->Bkingpos;
		int incheck = isCheck(pos,kingpos);
		if (incheck) {
			unmakeMove(pos);
			numcheckmoves++;
			continue;
		}
		pos->tomove = !pos->tomove;
		/*
		if (isThreefold(*pos)) {
			unmakeMove(pos);
			return 0;
		}
		if (pos->halfmoves >= 100) {
			unmakeMove(pos);
			return 0;
		}
		 */
		int score = -alphaBeta(pos,-beta,-alpha, depthleft - 1,(timeLeft - time_spentms));
		unmakeMove(pos);
		if (score >= beta) {
			return beta;
		}
		if (score > alpha) {
			alpha = score;
		}
	}
	//printf("%d - %d\n",num_moves,numcheckmoves);
	if (num_moves == numcheckmoves) {
		// no legal moves
		if (pos->tomove == WHITE) kingpos = pos->Wkingpos;
		if (pos->tomove == BLACK) kingpos = pos->Bkingpos;
		int incheck = isCheck(pos,kingpos);
		if (incheck) {
			// side to move is in checkmate
			return -99999;
		}
		else {
			// stalemate
			return 0;
		}
	}
	return alpha;
}
struct move search(struct position pos, int searchdepth,int movetime) {
	nodesSearched = 0;
	struct move moves[MAX_MOVES];
	int kingpos;
	clock_t begin = clock();
	int timeElapsed = 0;
	double time_spent;
	struct move bestmove;
	int curdepth;
	int nps;
	int num_moves = genLegalMoves(&pos,moves);
	struct move lastbestmove = moves[0];
	for (curdepth = 1; (curdepth < searchdepth+1 && timeElapsed == 0);curdepth++) {
		int bestScore = -9999;
		for (int i = 0;i < num_moves;i++) {
			clock_t end = clock();
			time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
			int time_spentms = (int)(time_spent * 1000);
			if (time_spentms >= movetime) {
				bestmove = lastbestmove;
				timeElapsed = 1;
				break;
			}
			makeMove(&moves[i],&pos);
			pos.tomove = !pos.tomove;
			if (pos.tomove == WHITE) kingpos = pos.Wkingpos;
			if (pos.tomove == BLACK) kingpos = pos.Bkingpos;
			int incheck = isCheck(&pos,kingpos);
			if (incheck) {
				unmakeMove(&pos);
				continue;
			}
			pos.tomove = !pos.tomove;
			int curscore;
			//int curscore = -negaMax(&pos,curdepth-1,(movetime - time_spentms));
			if (isThreefold(&pos)) {
				curscore = 0;
			}
			else if (pos.halfmoves >= 100) {
				curscore = 0;
			}
			else {
				curscore = -alphaBeta(&pos,-99999,99999,curdepth-1,(movetime - time_spentms));
			}
			//printf("%d - %s - %d\n",curdepth,movetostr(moves[i]),curscore);
			if (curscore == 99999) {
				printf("info depth %d nodes %d time %d nps %d score mate %d pv %s\n",(curdepth),nodesSearched,((int)(time_spent*1000)),nps,curdepth,movetostr(moves[i]));
				fflush(stdout);
				unmakeMove(&pos);
				return moves[i];
			}
			if (curscore > bestScore) {
				bestScore = curscore;
				bestmove = moves[i];
			}
			unmakeMove(&pos);
			nps = nodesSearched / time_spent;
		}
		lastbestmove = bestmove;
		/*
		printf("moves before: ");
		for (int i =0;i<num_moves;i++) {
			printf("%s ",movetostr(moves[i]));
		}
		printf("\n");
		*/
		/*
		// moves bestmove to start of moves array
		struct move newmoves[MAX_MOVES];
		int num_newmoves = 1;
		newmoves[0] = bestmove;
		int j = 1;
		for (int i =0;i < num_moves;i++) {
			// if bestmove != moves[i]
			if ( (bestmove.to != moves[i].to) || (bestmove.from != moves[i].from) || (bestmove.prom != moves[i].prom) ) {
				newmoves[j] = moves[i];
				j++;
				num_newmoves++;
			}
		}
		moves[0] = bestmove;
		num_moves = 1;
		for (int i=1;i<num_newmoves;i++) {
			// if bestmove != newmoves[i]
			if ( (bestmove.to != newmoves[i].to) || (bestmove.from != newmoves[i].from) || (bestmove.prom != newmoves[i].prom)) {
			moves[num_moves] = newmoves[i];
			num_moves++;
			}
		}
		/*
		printf("moves after: ");
		for (int i =0;i<num_moves;i++) {
			printf("%s ",movetostr(moves[i]));
		}
		printf("\n");
		*/
		
		printf("info depth %d nodes %d time %d nps %d score cp %d pv %s\n",(curdepth),nodesSearched,((int)(time_spent*1000)),nps,bestScore,movetostr(bestmove));
		fflush(stdout);
	}
	return bestmove;
}