class Player {
private:
	float x1, y1,x2,y2;
	float l;
	float h;
	int pts;
	bool hasWon;
	bool lr;
public:
	Player(float x,float y, float len,float hgt,bool leftyrighty) {
		x1 = x;
		y1 = y;
		l = len;
		h = hgt;
		pts = 0;
		hasWon = false;
		lr = leftyrighty; // mirrorovan 
		if (lr) {
			x2 = x1 - l;
			y2 = y1 + h;
		}
		else {
			x2 = x1 + l;
			y2 = y1 + h;
		}

	}
	void setLength(float lt) {
		l = lt;
		if (lr) {
			x2 = x1 - l;
			y2 = y1 + h;
		}
		else {
			x2 = x1 + l;
			y2 = y1 + h;
		}
	}
	bool Side() {
		return lr;
	}
	void updateCoords() {
		if (lr) {
			x2 = x1 - l;
			y2 = y1 + h;
		}
		else {
			x2 = x1 + l;
			y2 = y1 + h;
		}
	}
	float getX1() {
		return x1;
	}
	float getY1() {
		return y1;
	}
	float getX2() {
		return x2;
	}
	float getY2() {
		return y2;
	}
	void setX(float x) {
		if (lr) {
			if (x >= (3.5+l) && x <= 7.0)
				x1 = x;
		}
		else {
			if (x >= 0.0 && x <= (3.5 - l))
				x1 = x;
		}
		updateCoords();
	}
	void setY(float y) {
		y1 = y;
		y2 = y1 + h;
		updateCoords();
	}
	void setPt(int p) {
		pts = p;
	}
	void incrementPoints() {
		pts += 1;
	}
	void winCondition(bool gametype) {
		hasWon = false;
		if (!gametype && pts == 11)
			hasWon = true;
		else if (gametype && pts == 15) {
			hasWon = true;
		}
	}


	int getPart(float x,float y) {
		/*//resp deli paddle na tri dela->levi deo blago u X i visoko Y
			srednji deo 45 stepeni
			desni deo ostro i nisko
		*/
		float resp = l / 3; 
	}
};