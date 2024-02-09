#pragma once

//
// Simple class for manipulating 2d locations
//
class Position
{
private:
	double pos[2];
public:
	const inline double operator[](int i) const { return pos[i]; }
	const inline double* getPos(void) const { return &pos[0]; }
	const inline double getPos(int i) const { return pos[i]; }
	inline void setPos(const double* p) { pos[0] = p[0]; pos[1] = p[1]; }
	inline void setPos(double p, int i) { pos[i] = p; }
	const double getDistance(Position p) const { return sqrt((pos[0] - p[0]) * (pos[0] - p[0]) + (pos[1] - p[1]) * (pos[1] - p[1])); }
	Position operator-(Position p) 
	{ 
		Position q; 
		q.setPos(pos[0] - p.getPos(0), 0);
		q.setPos(pos[1] - p.getPos(1), 1);
		return q; 
	}
	Position(const Position& p) { pos[0] = p[0]; pos[1] = p[1]; }
	Position(void) { pos[0] = 0.0; pos[1] = 0.0; }
	Position(double a, double b) { pos[0] = a; pos[1] = b; }
	void operator=(Position p) { pos[0] = p[0]; pos[1] = p[1]; }
	
};